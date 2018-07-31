#!/usr/bin/python

import os, sys
import re
import optparse

try:
    dict.iteritems
except AttributeError:
    def itervalues(d):
        return iter(d.values())
    def iteritems(d):
        return iter(d.items())
else:
    def itervalues(d):
        return d.itervalues()
    def iteritems(d):
        return d.iteritems()

class LOD(object):

    def __init__(self):
        self.attrnames = []
        self.attrs = {}
        self.blocks = {}

    def checksum(self):
        sum = 0
        for block_base, block_data in iteritems(self.blocks):
            for data in block_data:
                sum += data
        return sum & 0xffffffff

    def load(self, fname):
        try:
            fh = open(fname, 'r')
        except:
            print("failed to open %s for read" % fname)
            return False

        self.attrnames = []
        self.attrs = {}
        self.blocks = {}

        lineno = 0
        block_base_re = re.compile("@([0-9A-Fa-f]{8})")
        block_data_re = re.compile("([0-9A-Fa-f]{8})")
        block_base = None
        block_data = []
        for line in fh.readlines():
            lineno += 1
            if line.startswith("#$"):
                fields = line[2:].strip().split("=")
                if len(fields) != 2:
                    print("LOD syntax error at %s:%d" %(fname, lineno))
                    fh.close()
                    return False
                self.attrnames.append(fields[0])
                self.attrs[fields[0]] = fields[1]
            elif line.startswith("#checksum="):
                if block_base is not None:
                    self.blocks[block_base] = block_data
                    block_base = None
                    block_data = []
                chk = int(line.strip()[10:], 16)
                expected = self.checksum()
                if chk != expected:
                    print("LOD checksum error, read %08x expected %08x" %(chk, expected))
                    fh.close()
                    return False
            elif line.startswith("@"):
                m = block_base_re.search(line)
                if m is None:
                    print("LOD syntax error at %s:%d" %(fname, lineno))
                    fh.close()
                    return False
                if block_base is not None:
                    self.blocks[block_base] = block_data
                    block_base = None
                    block_data = []
                block_base = int(m.group(1), 16)
            else:
                m = block_data_re.search(line)
                if m is None or block_base is None:
                    print("LOD syntax error at %s:%d" %(fname, lineno))
                    fh.close()
                    return False
                block_data.append(int(m.group(1), 16))

        # in case there are no checksum
        if block_base is not None:
            self.blocks[block_base] = block_data
            block_base = None
            block_data = []

        fh.close()
        return True

    def store(self, fname):
        try:
            fh = open(fname, 'w')
        except:
            print("failed to open %s for write" % fname)
            return False

        for attr in self.attrnames:
            fh.write("#$%s=%s\n" %(attr, self.attrs[attr]))
        for block_base in sorted(self.blocks.keys()):
            fh.write("@%08x\n" % block_base)
            for data in self.blocks[block_base]:
                fh.write("%08x\n" % data)
        fh.write("#checksum=%08x\n" % self.checksum())
        fh.close()
        return True

    def end_address(self):
        ea = 0
        for block_base, block_data in iteritems(self.blocks):
            bea = block_base + len(block_data) *4
            if bea > ea:
                ea = bea
        return ea

    def start_address(self):
        return sorted(self.blocks.keys())[0]

    def remove_boot(self,outlod):
        for block_base, block_data in iteritems(self.blocks):
            if(int(block_base) < 0x08010000):
                continue
            outlod.blocks[block_base] = block_data

def load_lod(fname):
    lod = LOD()
    if lod.load(fname):
        return lod
    return None

def dual_merge(bl, lod):
    if 'USER_BASE' not in bl.attrnames:
        print('USER_BASE not in base LOD')
        return None
    if 'USER_BASE' not in lod.attrnames:
        print('USER_BASE not in second LOD')
        return None


    ub1 = int(bl.attrs['USER_BASE'], 0) | 0x08000000
    ub2 = int(lod.attrs['USER_BASE'], 0) | 0x08000000
    if ub1 != ub2:
        print('USER_BASE are not the same  %x %x'%(ub1, ub2) )
        return None

    sa2 = lod.start_address()
    if ub2 != sa2:
        print('USER_BASE of second LOD is not the start address')
        return None

    ea1 = bl.end_address()
    if ea1 > sa2:
        print('USER_BASE is too big')
        return None

    output = LOD()
    output.blocks = lod.blocks

    pa = ea1
    while pa < sa2:
        block_base = pa
        block_size = sa2 - pa
        if pa % 0x10000 == 0 and block_size >= 0x10000:
            block_size = 0x10000
        elif pa % 0x1000 == 0 and block_size >= 0x1000:
            block_size = 0x1000
        else:
            print('failed to padding at 0x%x, size %d' % (block_base, block_size))
            return None

        block_data = [0xffffffff] * int(block_size/4)
        output.blocks[block_base] = block_data
        pa += block_size

    output.attrnames = lod.attrnames
    for attr in output.attrnames:
        output.attrs[attr] = lod.attrs[attr]
    # output.attrs['BOOT_SECTOR'] = bl.attrs['BOOT_SECTOR']
    output.blocks.update(bl.blocks)
    return output


def dual_otapack(lodstr):
    lod = load_lod(lodstr)
    if lod is None:
        return 1
    output = LOD()
    if output is None:
        return 1
    lod.remove_boot(output)
    for attr in lod.attrnames:
        output.attrs[attr] = lod.attrs[attr]
    otaout = lodstr[:-4] + "_ota.lod"
    if not output.store(otaout):
        return 1
    return 0

def dual_addpack(fname, str):
    try:
        fh = open(fname, 'r+')
    except:
        print("failed to open %s for read" % fname)
        return False

    fh.seek(0,2)
    len = fh.tell()
    str += "\n%d\n"%len
    fh.seek(0,0)
    fh.write(str)
    fh.close()
    return True


def main(argv):
    opt = optparse.OptionParser(usage="""%prog [options]

This utility will merge 2 LODs of dual boot into one LOD. During merge:
* check BOOT_SECTOR of both inputs;
* BOOT_SECTOR of the output LOD will follow bootloader LOD;
* first word of the 2nd LOD will be changed to magic number;
* padding from bootloader to second LOD;
* checksum will be regenerated.
""")

    opt.add_option("--opt", action="store", dest="op",
                   help="bootloader LOD file name")

    opt.add_option("--bl", action="store", dest="bl",
                   help="bootloader LOD file name")
    opt.add_option("--lod", action="store", dest="lod",
                   help="second LOD file name")
    opt.add_option("--output", action="store", dest="output",
                   help="output LOD file name")

    opt.add_option("--old", action="store", dest="old",
                   help="old LOD file name")
    opt.add_option("--new", action="store", dest="new",
                   help="new LOD file name")

    opt.add_option("--pack", action="store", dest="pack",
                   help="pack file name")
    opt.add_option("--str", action="store", dest="str",
                   help="str need to add")

    opt, argv = opt.parse_args(argv)

    if opt.op is None:
        print("please set you option!")
        return 1

        #merge
        #merge
    if opt.op == "merge":
        if opt.bl is None:
            print("bootloader LOD file name is not specified!")
            return 1
        if opt.lod is None:
            print("second LOD file name is not specified!")
            return 1
        if opt.output is None:
            print("output LOD file name is not specified!")
            return 1
        
        bl = load_lod(opt.bl)
        if bl is None:
            return 1
        lod = load_lod(opt.lod)
        if lod is None:
            return 1

        output = dual_merge(bl, lod)
        if output is None:
            return 1

        if not output.store(opt.output):
            return 1
        #otapack
        #otapack
    elif opt.op == "otapack":
        # python platform\compilation\lodCombine.py --opt otapack --old fota\fota_B1508_debugnew.lod --new fota\fota_B1509_debug.lod
        if opt.old is None:
            print("old LOD file name is not specified!")
            return 1
        if opt.new is None:
            print("new LOD file name is not specified!")
            return 1
        print("old: %s new: %s"%(opt.old[:-4], opt.new[:-4]))

        dual_otapack(opt.old)
        dual_otapack(opt.new)
        #otapack
        #otapack
    elif opt.op == "addpack":
        # python platform\compilation\lodCombine.py --opt addpack --pack fota\fota1.pack --str fota\fota_B1509_debug.lod
        if opt.pack is None:
            print("pack file name is not specified!")
            return 1
        if opt.str is None:
            print("str is not specified!")
            return 1
        print("output LOD file name is not errro!")
        dual_addpack(opt.pack, opt.str)
        #other
        #other
    else :
        print("output LOD file name is not errro!")
        return 1

    return 0


if __name__ == '__main__':
    sys.exit(main(sys.argv[1:]))
