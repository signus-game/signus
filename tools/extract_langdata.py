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
# Utility for extracting language files from original game data. Use it on
# the following game files, then follow any additional instructions:
# anims.dat, graphics.dat, screens.dat, sounds16.dat, texts.dat
#
# The extracted files will be stored in ./lang/

import os.path
import struct
import sys
import wave
from extract import Datfile
from PIL import Image

keybcs2_table = (
    'Č', 'ü', 'é', 'ď', 'ä', 'Ď', 'Ť', 'č', 'ě', 'Ě', 'Ĺ', 'Í', 'ľ', 'ĺ', 'Ä',
    'Á', 'É', 'ž', 'Ž', 'ô', 'ö', 'Ó', 'ů', 'Ú', 'ý', 'Ö', 'Ü', 'Š', 'Ľ', 'Ý',
    'Ř', 'ť', 'á', 'í', 'ó', 'ú', 'ň', 'Ň', 'Ů', 'Ô', 'š', 'ř', 'ŕ', 'Ŕ', '¼',
    '§', '«', '»', '░', '▒', '▓', '│', '┤', '╡', '╢', '╖', '╕', '╣', '║', '╗',
    '╝', '╜', '╛', '┐', '└', '┴', '┬', '├', '─', '┼', '╞', '╟', '╚', '╔', '╩',
    '╦', '╠', '═', '╬', '╧', '╨', '╤', '╥', '╙', '╘', '╒', '╓', '╫', '╪', '┘',
    '┌', '█', '▄', '▌', '▐', '▀', 'α', 'ß', 'Γ', 'π', 'Σ', 'σ', 'µ', 'τ', 'Φ',
    'Θ', 'Ω', 'δ', '∞', 'φ', 'ε', '∩', '≡', '±', '≥', '≤', '⌠', '⌡', '÷', '≈',
    '°', '∙', '·', '√', 'ⁿ', '²', '■', ' '
)

def decode_keybcs2(bstr):
    unclist = list()
    for byte in bstr:
        if byte < 128:
            unclist.append(chr(byte))
        else:
            unclist.append(keybcs2_table[byte - 128])
    return ''.join(unclist)

def decode_cp1252(bstr):
    return bstr.decode('cp1252', 'strict')

def copy_file(src, dst):
    with open(src, 'rb') as fr:
        data = fr.read()
    with open(dst, 'wb') as fw:
        fw.write(data)

def create_dirs(toolsdir):
    bitmap_list = ('mmnu0.png', 'mmnu1.png', 'mmnu2.png', 'mmnu3.png')
    text_list = (
        'credits.txt', 'udes55.txt', 'unam59.txt', 'unam60.txt', 'wpnnm12.txt'
    )
    path_list = (
        ('lang', 'anims'), ('lang', 'graphics', 'bitmaps'),
        ('lang', 'graphics', 'screens'), ('lang', 'speech'), ('lang', 'texts')
    )

    basedir = os.path.dirname(os.path.abspath(toolsdir))
    datadir = os.path.join(basedir, 'signus-data', 'data', 'en')

    if not os.path.isfile(os.path.join(datadir, 'Makefile.am')):
        raise RuntimeError('Cannot find Signus data files.')

    for item in path_list:
        path = os.path.join(*item)
        os.makedirs(path, 0o755, True)
        relpath = []

        for token in item:
            relpath.append(token)
            tmppath = relpath + ['Makefile.am']
            dstpath = os.path.join(*tmppath)
            srcpath = os.path.join(datadir, *tmppath[1:])
            if os.path.isfile(srcpath) and not os.path.isfile(dstpath):
                copy_file(srcpath, dstpath)

    for item in bitmap_list:
        dstpath = os.path.join('lang', 'graphics', 'bitmaps', item)
        if not os.path.isfile(dstpath):
            srcpath = os.path.join(datadir, 'graphics', 'bitmaps', item)
            copy_file(srcpath, dstpath)

    for item in text_list:
        dstpath = os.path.join('lang', 'texts', item)
        if not os.path.isfile(dstpath):
            srcpath = os.path.join(datadir, 'texts', item)
            copy_file(srcpath, dstpath)

def write_wav(name, data):
    fw = wave.open(name + '.wav', 'wb')
    fw.setnchannels(1)
    fw.setsampwidth(2)
    fw.setframerate(22050)
    fw.writeframes(data)
    fw.close()

def extract_speech(dfile):
    speech_set = set((
        'failed', 'increas', 'mis1in', 'mis10in', 'mis11a', 'mis11in',
        'mis12a', 'mis12b', 'mis12in', 'mis13in', 'mis14in', 'mis15a',
        'mis15in', 'mis16in', 'mis17in', 'mis18in', 'mis19in', 'mis2in',
        'mis3in', 'mis4in', 'mis5in', 'mis6a', 'mis6in', 'mis7in', 'mis8in',
        'mis9in', 'msg0', 'msg1', 'msg10', 'msg11', 'msg2', 'msg3', 'msg4',
        'msg5', 'msg6', 'msg7', 'msg8', 'msg9', 'reammo', 'refuel', 'repair',
        'success'
    ))

    for item in dfile:
        if item[1:] not in speech_set:
            continue
        data = dfile.get_data(item)
        path = os.path.join('lang', 'speech', item[1:])
        write_wav(path, data)

    print('''
Audio files were exported as plain WAVs to lang/speech/. You need to convert
them to Ogg Vorbis before playing the game.
''')

def extract_anims(dfile):
    anim_list = ('intro', 'intro2', 'outro', 'war_on')

    for item in anim_list:
        data = dfile.get_data(item)
        path = os.path.join('lang', 'anims', item + '.vvf')
        with open(path, 'wb') as fw:
            fw.write(data)

def extract_graphics(dfile):
    bitmap_info = {
        'load': (400, 50),
        'menu0': (300, 126),
        'menu1': (300, 126),
        'menu2': (300, 126),
        'repair0': (16, 52),
        'repair1': (16, 52),
        'vsettr0': (54, 63),
        'vsettr1': (54, 63),
        'vsettr2': (54, 63),
    }

    palette = dfile.get_palette('palette')
    prefix_set = set()

    for item in dfile:
        if item[1:] in bitmap_info:
            prefix_set.add(item[0])

    for item in dfile:
        if item[1:] not in bitmap_info:
            continue
        data = dfile.get_data(item)
        img = Image.new('P', bitmap_info[item[1:]])
        img.putdata(data)
        img.putpalette(palette)
        if len(prefix_set) == 1:
            fname = item[1:]
        path = os.path.join('lang', 'graphics', 'bitmaps', item + '.png')
        img.save(path)

    if len(prefix_set) > 1:
        print('''
Graphics.dat contains multiple language versions. You need to manually check
the files in lang/graphics/bitmaps/, find the correct language version and
delete the prefix character from the filenames (e.g. eload.png => load.png).
You can safely delete any duplicate files. Do not touch mmnu*.png.
''')

def extract_screens(dfile):
    palette = dfile.get_palette('scrpal')

    for pos in range(1, 20):
        data = dfile.get_data('3mis%d' % pos)
        dstname = 'mis%d-name.jpg' % pos
        path = os.path.join('lang', 'graphics', 'screens', dstname)
        img = Image.new('P', (800, 600))
        img.putdata(data)
        img.putpalette(palette)
        img = img.crop((58, 64, 744, 136))
        img = img.convert('RGB')
        img.save(path)

decoding_table = {
    'c': decode_keybcs2,
    'e': decode_cp1252
}

def extract_texts(dfile):
    for item in dfile:
        data = dfile.get_data(item)
        data = decoding_table[item[0]](data[:-1])
        path = os.path.join('lang', 'texts', item[1:] + '.txt')
        with open(path, 'wb') as fw:
            fw.write(data.encode('utf-8'))


extraction_table = {
    'anims.dat': extract_anims,
    'graphics.dat': extract_graphics,
    'screens.dat': extract_screens,
    'sounds16.dat': extract_speech,
    'texts.dat': extract_texts,
}

if __name__ == '__main__':
    if len(sys.argv) < 2:
        print('Usage: %s path/to/file1.dat [path/to/file2.dat ...]' %
            sys.argv[0], file=sys.stderr)
        sys.exit(1)
    create_dirs(os.path.dirname(sys.argv[0]))
    for datname in sys.argv[1:]:
        basename = os.path.basename(datname).lower()
        if basename not in extraction_table:
            continue
        with open(datname, 'rb') as fr:
            dfile = Datfile(fr)
            print('Extracting %s' % basename)
            extraction_table[basename](dfile)

    print('''
Extraction complete. You can now move the 'lang' directory to signus-data/data/
and rename it to the respective ISO-639 language code. Then edit all
Makefile.am files under it and change the 'en' to the correct language code
in the installation paths. Finally, register the directory in
signus-data/cofigure.ac and signus-data/data/Makefile.am.''')
