#!/usr/bin/perl -w


# ------------------------------------------------------------------------
# ����:
#     ��lod�ļ��������ļ��ϲ��������µ�lod�ļ�
# ������⣺
#     1. ����AMC���ù��ܵİ汾����Ҫ��AMC���ú�ƽ̨lod�ļ��ϲ�
#     2. Lua�汾����Ҫ��lua�ű���ƽ̨lod�ļ��ϲ�
#     3. OpenAT�汾����Ҫ��cust lod��ƽ̨lod�ļ��ϲ�
# ------------------------------------------------------------------------

use strict;
use Config;
use File::Find;
use Getopt::Long;
use File::Basename;

my $g_dbg_level = "1";
   #0 No debug info
   #1 Main
   #2 Subroutines
   #3 Loops in Main
   #4 Loops in Subroutines
   #5 all

use constant {
    BIN_MODE => 1, #�������ļ���ֱ�Ӷ�ȡ����ת��Ϊhex�����lod�ļ�
    LOD_MODE => 2, #lod�ļ�������Ϊ�Ѿ�ת������
};

my $combine_type; #amc, lua or openat
my $platform_lod_file;
my $combine_file;
my $output_lod_file;
my $output_ota_file;
my $flash_base;
my @sector_layout;

my %combine_type_hash = (
    "amc" =>    [BIN_MODE, "NV_BASE", "NV_SIZE"],
    "lua" =>    [BIN_MODE, "LUA_SCRIPT_BASE", "LUA_SCRIPT_SIZE"],
    "openat" => [LOD_MODE, "USER_BASE", "USER_SIZE"],
    "user" =>   [BIN_MODE, "USER_CONFIG_DATA_BASE", "USER_CONFIG_DATA_SIZE"],
    "loader" => [LOD_MODE, "CUST_LOADER_BASE", "CUST_LOADER_SIZE"]
);

#/*********************************************************
#  Function: dbg_out
#  Description: ���������Ϣ
#  Input:
#    1. �����Ϣ�ĵȼ�
#    2. ��Ҫ�������Ϣ
#  Output:
#  Return: 
#  Others:
#     ���ݵ�ǰȫ�ֱ���g_dbg_level�������Ƿ���Ҫ�����Ϣ
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
#  Description: �����pl�ű���ȫ������ѡ��
#  Input:
#    1. 
#  Output:
#  Return: 
#  Others:
#     
#*********************************************************/
sub usage
{
    my $char = "";
    print "\n";
    print "Usage: perl $0 ";
    foreach my $type (keys %combine_type_hash)
    {
        print "$char\[$type\]";
        $char = "/" if($char eq "");
    }
    print " [arguments]\n";
    print "  More information, please enter:\n";
    print "  perl $0 ";
    $char = "";
    foreach my $type (keys %combine_type_hash)
    {
        print "$char\[$type\]";
        $char = "/" if($char eq "");
    }
    print " -h\n";
    print "\n";
    exit 1;
}

#/*********************************************************
#  Function: lod_combine_usage
#  Description: ���amc�ϲ�����ѡ��
#  Input:
#    1. 
#  Output:
#  Return: 
#  Others:
#     
#*********************************************************/
sub lod_combine_usage
{
    my ($type) = @_;
    
    if($type eq "")
    {
        usage();
    }
    else
    {
        print "\n";
        print "Usage: perl $0 $type [arguments]\n";
        print "  -h\t\thelp information\n";
        print "  -l file\tinput platform lod file\n";
        print "  -i file\tinput $type file\n";
        print "  -o file\toutput combined lod file\n";
        print "\n";
    }
    exit 1;
}

sub init_sector_layout
{
    my ($tags_hash_ptr) = @_;
    my $sectormap_str;
    my @sectormap;
    my $sector_total_size = 0;
    
    if(!exists $tags_hash_ptr->{'sectormap'} || !exists $tags_hash_ptr->{'base'})
    {
        die "ERROR: platform lod file must contain tags: sectormap & base";
    }
    
    $flash_base = hex($tags_hash_ptr->{'base'});
    
    $sectormap_str = $tags_hash_ptr->{'sectormap'};
    $sectormap_str =~ s/\(|\)//g;
    $sectormap_str =~ s/\s//g;
    @sectormap = split(',', $sectormap_str);
    foreach my $item (@sectormap)
    {
        $item = lc($item);
        if($item =~ /(\d+)x(\d+)k/)
        {
            my $sector;
            $sector->{count} = $1;
            $sector->{size} = $2*1024;
            push(@sector_layout, $sector);
            $sector_total_size += $sector->{count} * $sector->{size};
        }
        else
        {
            die "ERROR: platform lod file with wrong sectormap: $tags_hash_ptr->{'sectormap'}";
        }
    }
    #���һ��flash size�Ƿ���ȷ
    if(exists $tags_hash_ptr->{'FLASH_SIZE'})
    {
        my $flash_size = hex($tags_hash_ptr->{'FLASH_SIZE'});
        
        if($sector_total_size != $flash_size)
        {
            die "ERROR: platform lod file with wrong sectormap & flash_size: $sector_total_size<>$flash_size";
        }
    }
}

sub get_sector_size
{
    my ($addr) = @_;
    my $addr_start;
    my $sector_start_addr = 0;
    my $sector_size;
    
    if($addr < $flash_base)
    {
        die "ERROR: sector addr error: $addr < $flash_base";
    }
    
    $addr_start = $addr - $flash_base;
    
    foreach my $item (@sector_layout)
    {
        my $count = $item->{count} - 1;
        $sector_size = $item->{size};
        #dbg_out(5, "count:$item->{count}, size:$sector_size");
        foreach my $i (0..$count)
        {
            #dbg_out(5, "Sector size:$sector_start_addr, $addr_start");
            $sector_start_addr += $sector_size;
            if($sector_start_addr == $addr_start)
            {
                return $sector_size;
            }
        }
    }
    
    die "ERROR: cannot find sector size: $addr";
}

#/*********************************************************
#  Function: platform_lod_checksum
#  Description: ����platform lod�ļ���checksum
#  Input:
#    1. 
#  Output:
#  Return: 
#  Others:
#     
#*********************************************************/
sub platform_lod_checksum
{
    my $line;
    my $checksum = 0;
    my $checksum_str;
    
    open( INPUT, "<$platform_lod_file" ) or die "Cannot open input platform lod file: $platform_lod_file\n";
    while( defined( $line = <INPUT> ) )
    {
        #chop( $line );#!!! window �����������ݴ���\r\n��chop��\nȥ��������\r
        #$line =~ s/[\015\012]$//g;#����β��\r\nȥ��
        $line =~ s/^[\s]+//g;#����ͷ�Ŀհ��ַ�ȥ��
        $line =~ s/[\s]+$//g;#����β�Ŀհ��ַ�ȥ��

        if($line eq "")
        {
            #���м���
            next;
        }

        #tags
        if($line =~ /^([0-9A-Fa-f]+)$/)
        {
            $checksum += hex($1);
        }
    }
    close(INPUT);
    
    $checksum &= 0xffffffff;
    $checksum_str = sprintf("%08x", $checksum);
    dbg_out(1, "checksum=$checksum_str");
}

#/*********************************************************
#  Function: output_lod
#  Description: �����output lod�ļ���
#  Input:
#    1. output lod�ļ����
#    2. �����Ϣ
#    3. Ҫ���µ�checksum
#  Output:
#  Return: 
#  Others:
#     
#*********************************************************/
sub output_lod
{
    my ($output, $line, $checksum) = @_;
    
    if($line =~ /^([0-9A-Fa-f]+)$/)
    {
        $$checksum += hex($1);
    }
    
    # print $output "$line\r\n";
    print $output "$line\n";
}

#/*********************************************************
#  Function: lod_combine_process
#  Description: post_ld����ʵ��
#  Input:
#    1. 
#  Output:
#  Return: 
#  Others:
#     ���ܴ����裺
#     step 1. ���������rename file����hash��
#     step 2. ���� step 1 ���ɵ�hash��ִ���޸Ŀ����ƵĲ���
#*********************************************************/
sub lod_combine_process
{
    my $line;
    my $line_num = 0;
    my @tags;
    my %tags_hash;
    my $addr = 0;
    my $combine_addr;
    my $combine_size;
    my $error;
    my $input_file_eof = 0;
    my $checksum = 0;
    my $OUTPUT;
    my $OTAOUTPUT;
    
    open( INPUT, "<$platform_lod_file" ) or die "Cannot open input platform lod file: $platform_lod_file\n";
    open( COMBINE, "<$combine_file" ) or die "Cannot open $combine_type combine file: $combine_file\n";
    open( $OUTPUT, ">$output_lod_file" ) or die "Cannot open output combined lod file: $output_lod_file\n";
    open( $OTAOUTPUT, ">$output_ota_file" ) or die "Cannot open output combined lod file: $output_ota_file\n";
    
    # step 1
    #��ȡplatform lod�ļ�ͷ����Ϣ
    while( defined( $line = <INPUT> ) )
    {
        $line_num++;
        #chop( $line );#!!! window �����������ݴ���\r\n��chop��\nȥ��������\r
        #$line =~ s/[\015\012]$//g;#����β��\r\nȥ��
        $line =~ s/^[\s]+//g;#����ͷ�Ŀհ��ַ�ȥ��
        $line =~ s/[\s]+$//g;#����β�Ŀհ��ַ�ȥ��

        if($line eq "")
        {
            #���м���
            next;
        }

        #tags
        if($line =~ /^#\$([^=]+)=(.+)$/) #��#$��ʼ
        {
            if(exists $tags_hash{$1})
            {
                die "ERROR: same tag in platform lod file:$platform_lod_file, tag($1)";
            }
            push(@tags, $1);
            $tags_hash{$1} = $2;
            
            next;
        }

        #first address
        elsif($line =~ /^@([0-9A-Fa-f]+)$/)
        {
            $addr = hex($1);
            last;
        }

        # other unknown content
        else
        {
            $error = "Bad platform lod file content:line$line_num\n";
            goto ERROR;
        }
    }
    init_sector_layout(\%tags_hash);
    dbg_out(5, "step 1 OK");
    
    # step 2
    #����Ҫ�ϲ����ļ����ͣ��ҵ���ʼ��ַ�ͺϲ�����Ĵ�С
    if(!exists $combine_type_hash{$combine_type})
    {
        $error = "Unkown combine type: $combine_type";
        goto ERROR;
    }
    if(!exists $tags_hash{$combine_type_hash{$combine_type}[1]})
    {
        $error = "Cannot find combine address tag in platform lod file: $combine_type_hash{$combine_type}[1]";
        goto ERROR;
    }
    $combine_addr = hex($tags_hash{$combine_type_hash{$combine_type}[1]}) + $flash_base;
    if(!exists $tags_hash{$combine_type_hash{$combine_type}[2]})
    {
        $error = "Cannot find combine aera size tag in platform lod file: $combine_type_hash{$combine_type}[2]";
        goto ERROR;
    }
    $combine_size = hex($tags_hash{$combine_type_hash{$combine_type}[2]});
    dbg_out(5, "combine_addr=$combine_addr, combine_size=$combine_size");
    
    # step 3
    #��� $combine_addr �ĺϷ���
    #��ȡҪ�ϲ����ļ��������Ƿ񳬳�����ʹ�õ�����
    if(BIN_MODE == $combine_type_hash{$combine_type}[0])
    {
        my @file_args = stat($combine_file);
        dbg_out(5, "Combine file size: $file_args[7]");
    
        #ֻ��binģʽ���ļ�����ͨ��ֱ�ӱȽ��ļ���С�������Ƿ���Է���
        if($file_args[7] > $combine_size)
        {
            $error = "Combine file is too large($file_args[7] bytes) than $combine_type area($combine_size bytes)";
            goto ERROR;
        }
    }
    else # LOD_MODE
    {
        my $first_addr = 0;
        my $last_addr = 0;
        seek(COMBINE, 0, 0);
        while( defined( $line = <COMBINE> ) )
        { 
            $line =~ s/^[\s]+//g;#����ͷ�Ŀհ��ַ�ȥ��
            $line =~ s/[\s]+$//g;#����β�Ŀհ��ַ�ȥ��
    
            if($line =~ /^@([0-9A-Fa-f]+)$/)
            {
                $first_addr = hex($1) if(0 == $first_addr);
                $last_addr = hex($1);
            }
            
            #tag
          if($line =~ /^#\$([^=]+)=(.+)$/) #��#$��ʼ
	        {
	            if(!exists $tags_hash{$1})
	            {
                 	push(@tags, $1);
                 	$tags_hash{$1} = $2;
	            }
	            else
	            {
	                if($tags_hash{$1} ne $2 && $1 ne "sectormap")#����sectormap
	                {
	                  die "the parameter $1 $tags_hash{$1} and $2 is not same\n"
	                }	
	            }	            
	            next;
	        }            
        }
        seek(COMBINE, 0, 0);
        
        if($first_addr < $combine_addr)
        {
            $error = "cust lod start addrss($first_addr) outside of combine address($combine_addr)";
            goto ERROR;
        }
        if($last_addr > ($combine_addr+$combine_size))
        {
            $error = "cust lod end addrss($last_addr) outside of combine address($combine_addr+$combine_size)";
            goto ERROR;
        }
    }
    
    # step 4
    #�ϲ������������checksum
    #���ͷ��Ϣ
    foreach my $item (@tags)
    {
        #print OUTPUT "#\$$item=$tags_hash{$item}\r\n";
        output_lod($OUTPUT, "#\$$item=$tags_hash{$item}", \$checksum);
        output_lod($OTAOUTPUT, "#\$$item=$tags_hash{$item}", \$checksum);
    }
    
    #���combine_addr֮ǰ������
    while($addr < $combine_addr)
    {
        $line = sprintf("@%08x", $addr);#��Ҫ��0
        my $last_addr = $addr;
        # print  "$line\r\n";
        output_lod($OUTPUT, $line, \$checksum);
        if($addr >= 0x08010000)
        {
            output_lod($OTAOUTPUT, $line, \$checksum);
        }
        while( defined( $line = <INPUT> ) )
        {
            $line =~ s/^[\s]+//g;#����ͷ�Ŀհ��ַ�ȥ��
            $line =~ s/[\s]+$//g;#����β�Ŀհ��ַ�ȥ��
            
            if($line =~ /^@([0-9A-Fa-f]+)$/)
            {
                $addr = hex($1);
                last;
            }
            # print  "$line\r\n";
            output_lod($OUTPUT, $line, \$checksum);
            if($addr >= 0x08010000)
            {
                output_lod($OTAOUTPUT, $line, \$checksum);
            }
        }
        if($addr == $last_addr)
        {
            seek($OUTPUT, -20, 1);
            seek($OTAOUTPUT, -20, 1);
            while($addr < ($combine_addr - 0x10000) )
            {
                if($addr % 0x10000 == 0)
                {
                    $line = sprintf("@%08x", $addr+0x10000);
                    output_lod($OUTPUT, $line, \$checksum);
                    output_lod($OTAOUTPUT, $line, \$checksum);
                }
                $line = sprintf("ffffffff");
                output_lod($OUTPUT, $line, \$checksum);
                output_lod($OTAOUTPUT, $line, \$checksum);
                $addr += 4;
            }
            last;
        }
        $last_addr = $addr;
    }
    # print "out addr $addr ";
    dbg_out(5, "befor finish");
    
    #��ԭ��lod�ļ������ݶ�����
    dbg_out(5, "$addr,$combine_addr,$combine_size");
    while($addr < ($combine_addr + $combine_size))
    {
        while( defined( $line = <INPUT> ) )
        {
            $line =~ s/^[\s]+//g;#����ͷ�Ŀհ��ַ�ȥ��
            $line =~ s/[\s]+$//g;#����β�Ŀհ��ַ�ȥ��
            
            if($line =~ /^@([0-9A-Fa-f]+)$/)
            {
                $addr = hex($1);
                last;
            }
        }
        
        if( !defined($line) )
        {
            $input_file_eof = 1;
            last;
        }
    }
    
    #���combine file����
    #�����������һ����amc/lua�������ļ�ת����һ����lod�ļ���
    #$combine_addr�ᱻ�޸�
    dbg_out(5, "Combine start ==========>>>>>>>>>>>>>>>>>>");
    if(BIN_MODE == $combine_type_hash{$combine_type}[0])
    {
        my $buf;
        my @data;
        my $num = 0;
        my $write_size = 0;
        my $sector_size = get_sector_size($combine_addr);
        $line = sprintf("@%08x", $combine_addr);#��Ҫ��0
        dbg_out(5, "$line");
        #print OUTPUT "$line\r\n";
        output_lod($OUTPUT, $line, \$checksum);
        binmode( COMBINE );#������ģʽ
        while( defined($num = read(COMBINE, $buf, 4)) )
        {
            if($num == 0)
            {
                last;
            }
            $line = unpack("H*", $buf);
            $num = 4 - $num;
            $line .= "ff"x$num;
            @data = $line =~ /\w{2}/g;#ÿ2λ�ָ�
            @data[0,1,2,3] = @data[3,2,1,0];#����
            $line = "@data";
    		$line =~ s/\s//g;#�滻�ո�
            #print OUTPUT "$line\r\n";
            output_lod($OUTPUT, $line, \$checksum);
            $write_size += 4;
    
            #д��1��sector����Ҫд����sector�ĵ�ַ
            if($write_size >= $sector_size)
            {
                $write_size = 0;
                $combine_addr += $sector_size;
                $sector_size = get_sector_size($combine_addr);
                $line = sprintf("@%08x", $combine_addr);#��Ҫ��0
                dbg_out(5, "$line");
                #print OUTPUT "$line\r\n";
                output_lod($OUTPUT, $line, \$checksum);
            }
        }
        #�����sector
=cut
        while($write_size < $sector_size)
        {
            $line = "ffffffff";
            #print OUTPUT "$line\r\n";
            output_lod($OUTPUT, $line, \$checksum);
            $write_size += 4;
        }
=cut
    }
    elsif(LOD_MODE == $combine_type_hash{$combine_type}[0])
    {
        my %cust_lod_tags_hash;
        my $cust_lod_addr = 0;
        my $cust_lod_line_num = 0;
        
        while( defined( $line = <COMBINE> ) )
        {
            $cust_lod_line_num++;
            $line =~ s/^[\s]+//g;#����ͷ�Ŀհ��ַ�ȥ��
            $line =~ s/[\s]+$//g;#����β�Ŀհ��ַ�ȥ��
    
            if($line eq "")
            {
                #���м���
                next;
            }
    
            #tags
            if($line =~ /^#\$([^=]+)=(.+)$/) #��#$��ʼ
            {
                if(exists $cust_lod_tags_hash{$1})
                {
                    die "ERROR: same tag in custom lod file:$combine_file, tag($1)";
                }
                $cust_lod_tags_hash{$1} = $2;
                
                next;
            }
            #first address
            elsif($line =~ /^@([0-9A-Fa-f]+)$/)
            {
                $cust_lod_addr = hex($1);
                last;
            }
    
            # other unknown content
            else
            {
                $error = "Bad custom lod file content:line$cust_lod_line_num\n";
                goto ERROR;
            }
        }
        #cust lod�еĵ�һ����ַһ��Ҫ������merge�ĵ�ַҪС
        if($cust_lod_addr < $combine_addr)
        {
            $error = "Bad custom lod file: cust first address $cust_lod_addr < $combine_addr\n";
            goto ERROR;
        }
        
        #���ﻹ��������lod�ļ���flash�����Ƿ�ƥ�����Ϣ
        
        #copy cust lod�ļ����ݵ�output lod�ļ���
        $combine_addr = $cust_lod_addr;
        $line = sprintf("@%08x", $combine_addr);#��Ҫ��0
        dbg_out(5, "$line");
        #print OUTPUT "$line\r\n";
        output_lod($OUTPUT, $line, \$checksum);
        output_lod($OTAOUTPUT, $line, \$checksum);
        while( defined( $line = <COMBINE> ) )
        {
            $line =~ s/^[\s]+//g;#����ͷ�Ŀհ��ַ�ȥ��
            $line =~ s/[\s]+$//g;#����β�Ŀհ��ַ�ȥ��
            
            if($line =~ /^#checksum/)
            {
                #�ļ�������
                dbg_out(5, "cust lod last");
                last;
            }
            elsif($line =~ /^@([0-9A-Fa-f]+)$/)
            {
                $combine_addr = hex($1);
                dbg_out(5, "$line");
            }
            elsif($line eq "")
            {
                next;
            }
            #print OUTPUT "$line\r\n";
            output_lod($OUTPUT, $line, \$checksum);
            output_lod($OTAOUTPUT, $line, \$checksum);
        }
    }
    else
    {
        $error = "Unknown combine type:$combine_type";
        goto ERROR;
    }
    dbg_out(5, "Combine finish ==========>>>>>>>>>>>>>>>>>>");
    
    #���combine_addr֮�������
    if($input_file_eof == 0)
    {
        if($addr < $combine_addr)#�˴���combine_addr�Ѿ����޸�Ϊ���һ����ַ��
        {
            $error = "ERROR: combine data too large: $addr < $combine_addr!";
            goto ERROR;
        }
        $line = sprintf("@%08x", $addr);#��Ҫ��0
        #print OUTPUT "$line\r\n";
        output_lod($OUTPUT, $line, \$checksum);
        output_lod($OTAOUTPUT, $line, \$checksum);
        while( defined( $line = <INPUT> ) )
        {
            $line =~ s/^[\s]+//g;#����ͷ�Ŀհ��ַ�ȥ��
            $line =~ s/[\s]+$//g;#����β�Ŀհ��ַ�ȥ��
            
            if($line =~ /^#checksum/)
            {
                #�ļ�������
                dbg_out(5, "last");
                last;
            }
            elsif($line eq "")
            {
                next;
            }
            #print OUTPUT "$line\r\n";
            output_lod($OUTPUT, $line, \$checksum);
            output_lod($OTAOUTPUT, $line, \$checksum);
        }
    }
    
    #���checksum
    $checksum &= 0xffffffff;
    $line = sprintf("#checksum=%08x", $checksum);
    output_lod($OUTPUT, $line, \$checksum);
    output_lod($OTAOUTPUT, $line, \$checksum);
    
    close(INPUT);
    close(COMBINE);
    close($OUTPUT);
    close($OTAOUTPUT);
    return;
    
ERROR:
    close(INPUT);
    close(COMBINE);
    close($OUTPUT);
    close($OTAOUTPUT);
    unlink($output_lod_file);
    unlink($output_ota_file);
    
    die "$error";
}

#/*********************************************************
#  Function: lod_combine
#  Description: prev_ld���ܲ�������
#  Input:
#    1. �����б�����
#  Output:
#  Return: 
#  Others:
#     
#*********************************************************/
sub lod_combine
{
    my $i = 0;
    
    #��ȡ����
    while( $_[$i] )
    {
        my $key = $_[$i];

        # dbg_out("1", "key: $key");
        
        # -h help
        if ($key eq "-h")
        {
            lod_combine_usage($combine_type);
        }

        # -l input lod file
        elsif ($key eq "-l")
        {
            $i++;
            $platform_lod_file = $_[$i];
            # dbg_out("1", "platform_lod_file=$platform_lod_file");
            $i++;
        }

        # -i input combine file
        elsif ($key eq "-i" )
        {
            $i++;
            $combine_file = $_[$i];
            # dbg_out("1", "combine_file=$combine_file");
            $i++;
        }

        # -o output combined lod file
        elsif ($key eq "-o" )
        {
            $i++;
            $output_lod_file = $_[$i];
            # dbg_out("1", "output_lod_file=$output_lod_file");
            $i++;
        }

        # -u output combined lod file
        elsif ($key eq "-u" )
        {
            $i++;
            $output_ota_file = $_[$i];
            # dbg_out("1", "output_ota_file=$output_ota_file");
            $i++;
        }
        
        # not valid parameter
        else
        {
            dbg_out("1", "not valid parameter");
            lod_combine_usage($combine_type);
        }
    }
    
    if($platform_lod_file eq "" || $combine_file eq "" || $output_lod_file eq "" || $output_ota_file eq "")
    {
        print "Parameter error!\n\n";
        lod_combine_usage($combine_type);
    }
    
    # dbg_out("1", "parameter ok");

    lod_combine_process();
    #platform_lod_checksum();
}


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
    my $key = $ARGV[0];

    # dbg_out("1", "key: $key");
    
    #��ȡ�ϲ�����
    if(defined($combine_type_hash{$key}))
    {
        shift @ARGV;
        $combine_type = $key;
    }
    
    # not valid parameter
    else
    {
        dbg_out("1", "not valid parameter");
        usage();
    }
    
    lod_combine(@ARGV);
}

exit 0;