#!/usr/bin/perl -w

# ------------------------------------------------------------------------
# 功能:
#     将两个elf文件合并，生成新的elf文件
# 解决问题：
#     1. 
# ------------------------------------------------------------------------

use strict;
use Config;
use File::Find;
use Getopt::Long;
use File::Basename;
use Cwd;
use Cwd 'abs_path';

my $g_dbg_level = "0";
   #0 No debug info
   #1 Main
   #2 Subroutines
   #3 Loops in Main
   #4 Loops in Subroutines
   #5 all

my $input_elf_file_1 = undef;
my $input_elf_file_2 = undef;
my $output_elf_file = undef;



my $os=$Config{osname};

my $objstrip;
my $objdump;
my $ld;

if($os  eq  "linux"){
    $objstrip = '"mips-elf-strip"';#objstrip工具
    $objdump = '"mips-elf-objdump"';#objdump工具
    $ld = '"mips-elf-ld"';#ld工具
}
else{
    $objstrip = '"mips-elf-strip.exe"';#objstrip工具
    $objdump = '"mips-elf-objdump.exe"';#objdump工具
    $ld = '"mips-elf-ld.exe"';#ld工具
}


my $ld_parameters = "-nostdlib --no-strip-discarded --oformat=elf32-littlemips --gc-sections";

my @ignore_sections_names = (
    ".reginfo",
    ".gnu.",
    ".comment",
    ".debug_",
);

my $ld_script_header = 
"
ENTRY (boot_Sector)
EXTERN (__bb_int_handler)
EXTERN (_sxr_Irq_handler)
EXTERN (boot_LoaderEnter)
EXTERN (boot_Sector)
EXTERN (boot_Sector_Nand)
";

#/*********************************************************
#  Function: dbg_out
#  Description: 输出调试信息
#  Input:
#    1. 输出信息的等级
#    2. 需要输出的信息
#  Output:
#  Return: 
#  Others:
#     根据当前全局变量g_dbg_level来决定是否需要输出信息
#*********************************************************/
sub dbg_out
{
    my $dbg_level = $_[0];
    my $dbg_info = $_[1];

    if ( $g_dbg_level >= $dbg_level )
    {
       printf "** $dbg_info\n";
    }
}

#/*********************************************************
#  Function: usage
#  Description: 输出该pl脚本的全部功能选项
#  Input:
#    1. 
#  Output:
#  Return: 
#  Others:
#     
#*********************************************************/
sub usage
{
    print "\n";
    print "Usage: perl $0 [arguments]\n";
    print "  -h\t\thelp information\n";
    print "  -e1 file\tinput elf file\n";
    print "  -e2 file\tinput elf file\n";
    print "  -o file\toutput combined elf file\n";
    print "\n";
    exit 1;
}

sub ignore_section
{
    my ($section_name) = @_;
    
    #print "section_name=$section_name\n";
    foreach my $name (@ignore_sections_names)
    {
        #print "name=$name\n";
        return(1) if($section_name =~ /^$name/);
    }
    
    return(0);
}

sub read_elf_section
{
    my ($elf_file, $sect_table_ptr, $addr_table_ptr) = @_;
    my $sectionstr;
    my $loadstr;
    my $vmastr;
    my $lmastr;
    my ($str,$vma_start,$vma_end,$lma_start,$lma_end,$len);
    
    die "Cannot find input elf file: $elf_file" if( ! -f $elf_file);
    open FH, '-|', "$objdump -h $elf_file" or die "Cannot run objdunp($!)";
    while( <FH> )
    {
        #print ;
        if(/ *[0-9]+ +([^ ]+) +([0-9a-zA-Z]+) +([0-9a-zA-Z]+) +([0-9a-zA-Z]+) /)
        {
            $sectionstr = $1;
            next if(1 == ignore_section($sectionstr));
            
            $vmastr = (hex($3)==0)?"0x0":"0xffffffff".$3;
            $lmastr = (hex($4)==0)?"0x0":"0xffffffff".$4;
            
            $len = hex($2);
            $vma_start = hex($3);
            $vma_end = $vma_start + $len;
            $lma_start = hex($4);
            $lma_end = $lma_start + $len;
            
            $loadstr = "(NOLOAD)";
            $loadstr = "" if(<FH> =~ /LOAD/);

            #printf("%s (%s) %s : AT (%s) { KEEP($elf_file(%s)) }\n",$sectionstr,$vmastr,$loadstr,$lmastr,$sectionstr);
            if(exists $sect_table_ptr->{$sectionstr})
            {
                die "Same section name in sections hash: $sectionstr";
            }
            if(exists $addr_table_ptr->{$sectionstr})
            {
                die "Same section name in address hash: $sectionstr";
            }
            
            foreach my $str (keys %{$addr_table_ptr})
            {
                my $item = $addr_table_ptr->{$str};
                if(($item->{'vma_start'} < $vma_start && $item->{'vma_end'} > $vma_start) ||
                   ($item->{'vma_start'} < $vma_end && $item->{'vma_end'} > $vma_end)
                  )
                {
                    if($sectionstr =~ /overlay/ && $str =~ /overlay/)
                    {
                         #overlay section 允许VMA地址重复
                    }
                    else
                    {
                         die "Sections ADDR VMA overlap: $sectionstr and $str";
                    }
                }
                if(($item->{'lma_start'} < $lma_start && $item->{'lma_end'} > $lma_start) ||
                   ($item->{'lma_start'} < $lma_end && $item->{'lma_end'} > $lma_end)
                  )
                {
                    die "Sections ADDR LMA overlap: $sectionstr and $str";
                }
            }
            
            my $addr;
            $addr->{'vma_start'} = $vma_start;
            $addr->{'vma_end'} = $vma_end;
            $addr->{'lma_start'} = $lma_start;
            $addr->{'lma_end'} = $lma_end;
            $addr_table_ptr->{$sectionstr} = $addr;

            my $sect;
            $sect->{'file'} = $elf_file;
            $sect->{'vma'} = $vmastr;
            $sect->{'lma'} = $lmastr;
            $sect->{'load'} = $loadstr;
            
            $sect_table_ptr->{$sectionstr} = $sect;
        }
    }
    close FH;
}

#/*********************************************************
#  Function: elf_combine
#  Description: prev_ld功能参数解析
#  Input:
#    1. 参数列表数组
#  Output:
#  Return: 
#  Others:
#     
#*********************************************************/
sub elf_combine
{
    my $i = 0;
    my $ret;
    my %sect_table;
    my %addr_table;
    
    my $tmp_ld_file = "tmp_ld_script.ld";
    
    #分析elf中section信息
    # `$objstrip -g $input_elf_file_1`;
    # `$objstrip -g $input_elf_file_2`;
    read_elf_section($input_elf_file_1, \%sect_table, \%addr_table);
    read_elf_section($input_elf_file_2, \%sect_table, \%addr_table);
    
    
    #生成链接时使用的 ld script 文件
    open(FH, ">$tmp_ld_file") or die "Cannot open tmp ld file:$!";
    print FH "$ld_script_header";
    print FH "SECTIONS\n{\n\n";
    foreach my $sectionstr (keys %sect_table)
    {
        my $item = $sect_table{$sectionstr};
        my $str = sprintf("%s (%s) %s : AT (%s) { KEEP(%s(%s)) }\n",$sectionstr,$item->{'vma'},$item->{'load'},$item->{'lma'},$item->{'file'},$sectionstr);
        print FH $str;
    }
    print FH "\n}\n";
    close(FH);
    
    #生成新的elf文件
    #${LD} -nostdlib -o ${BUILD_ROOT}/1.elf --no-strip-discarded --oformat=elf32-littlemips --script ${BUILD_ROOT}/3.ld -Map ${BUILD_ROOT}/1.map --gc-sections ${BUILD_ROOT}/SW_V001_A6300V_OPENAT.elf ${BUILD_ROOT}/SW_V867_A6300_ZH.elf
    my $map_file = $output_elf_file;
    $map_file =~ s/\.elf$/\.map/;
    $ld_parameters .= " -o $output_elf_file";
    $ld_parameters .= " -Map $map_file";
    $ld_parameters .= " --script $tmp_ld_file";
    $ld_parameters .= " $input_elf_file_1 $input_elf_file_2";
    `$ld $ld_parameters`;
    $ret = $? >> 8;
    if(0 != $ret)
    {
        print "Cannot merge elf\n";
    }

    unlink($tmp_ld_file);
}

#**********************************************************
# lua
#**********************************************************

#**********************************************************
# openat
#**********************************************************

# ------------------------------------------------------------------------
# Main
# ------------------------------------------------------------------------
if( not defined $ARGV[0])
{
    dbg_out("1", "no parameters");
    usage();
}
else
{
    my $i=0;
    while( $ARGV[$i] )
    {
        my $key = $ARGV[$i];

        dbg_out("1", "key: $key");

        # -h help
        if ($key eq "-h")
        {
            usage();
        }

        # -e1 input elf file
        elsif ($key eq "-e1")
        {
            $i++;
            $input_elf_file_1 = $ARGV[$i];
            dbg_out("1", "input_elf_file_1=$input_elf_file_1");
            $i++;
        }
        
        # -e2 input elf file
        elsif ($key eq "-e2")
        {
            $i++;
            $input_elf_file_2 = $ARGV[$i];
            dbg_out("1", "input_elf_file_2=$input_elf_file_2");
            $i++;
        }
        
        # -o output elf file
        elsif ($key eq "-o")
        {
            $i++;
            $output_elf_file = $ARGV[$i];
            dbg_out("1", "output_elf_file=$output_elf_file");
            $i++;
        }

        # not valid parameter
        else
        {
            dbg_out("1", "not valid parameter");
            usage();
        }
    }
    
    if(!defined($input_elf_file_1) or !defined($input_elf_file_2) or !defined($output_elf_file))
    {
        dbg_out("1", "parameter error");
        usage();
    }
    
    dbg_out("1", "parameter ok");

    my $cwd = getcwd();
    my $index1 = index($input_elf_file_1,$cwd)+length($cwd)+1;
    my $index2 = length($input_elf_file_1);
    $input_elf_file_1 = substr($input_elf_file_1,$index1,$index2);
    # print "\n----$input_elf_file_1------\n";
    $index1 = index($input_elf_file_2,$cwd)+length($cwd)+1;
    $index2 = length($input_elf_file_2);
    $input_elf_file_2 = substr($input_elf_file_2,$index1,$index2);
    # print "\n----$input_elf_file_2------\n";

    elf_combine();

}

exit 0;
