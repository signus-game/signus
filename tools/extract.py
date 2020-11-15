#!/usr/bin/python3
#
# This file is part of Signus: The Artefact Wars
#
# Copyright (C) 2020 next_ghost
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License version 2 as
# published by the Free Software Foundation.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
#
# ----
#
# Library/utility for extracting raw assets from DAT archives.

import io
import struct
import sys

def cleanstr(charbuf):
    return charbuf.partition(b'\0')[0].decode('ASCII')

class Datfile:
    def __init__(self, fr):
        self._file = fr
        self._index = dict()
        self._file.seek(0, io.SEEK_SET)
        (offset,) = struct.unpack('<I', self._file.read(4))
        self._file.seek(offset, io.SEEK_SET)
        (idxsize,) = struct.unpack('<I', self._file.read(4))

        for pos in range(idxsize):
            name, pos, size = struct.unpack('<12sII', self._file.read(20))
            self._index[cleanstr(name)] = (pos, size)

    def __iter__(self):
        return self._index.__iter__()

    def __contains__(self, name):
        return name in self._index

    def get_offset(self, name):
        return self._index[name][0]

    def get_size(self, name):
        return self._index[name][1]

    def get_data(self, name):
        pos, size = self._index[name]
        self._file.seek(pos)
        (tmp,) = struct.unpack('<I', self._file.read(4))
        if tmp + 4 == size:
            size = tmp
        else:
            self._file.seek(-4, io.SEEK_CUR)
        return self._file.read(size)

    def get_palette(self, name):
        data = self.get_data(name)
        return bytes(x << 2 for x in data)

if __name__ == '__main__':
    if len(sys.argv) < 2:
        print('Usage: %s file.dat [assetname [...]]' % sys.argv[0],
            file=sys.stderr)
        sys.exit(1)
    with open(sys.argv[1], 'rb') as fr:
        dfile = Datfile(fr)
        if len(sys.argv) < 3:
            for name in dfile:
                print(name)
        for name in sys.argv[2:]:
            data = dfile.get_data(name)
            with open(name, 'wb') as fw:
                fw.write(data)
