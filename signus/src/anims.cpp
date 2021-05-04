/*
 *  This file is part of Signus: The Artefact Wars (http://signus.sf.net)
 *
 *  Copyright (C) 1997, 1998, 2002, 2003
 *  Vaclav Slavik, Richard Wunsch, Marek Wunsch
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2 as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include <cstdio>
#include <cstring>
#include <climits>
#include <cmath>
#include <SDL_timer.h>
#include "global.h"
#include "graphio.h"
#include "events.h"
#include "mouse.h"
#include "sound.h"
#include "anims.h"
#include "engtimer.h"

MemoryReadStream *decompress(ReadStream &stream) {
	unsigned long bufsize, pos, size;
	uint8_t *buf;
	unsigned tmp, bitmask, i, j;
	int offset;
	MemoryReadStream *ret;

	bufsize = stream.readUint32LE();
	buf = new uint8_t[bufsize];
	pos = 0;

	while (1) {
		// Bitmask encoding write/copy operations
		bitmask = stream.readUint8();

		if (stream.eos()) {
			break;
		}

		for (i = 0; i < 8; i++) {
			tmp = stream.readUint8();

			if (stream.eos() || pos >= bufsize) {
				break;
			}

			// Simple byte write to output
			if (bitmask & (1 << i)) {
				buf[pos++] = tmp;
				continue;
			}

			// Handle copy operation
			size = stream.readUint8();

			if (stream.eos()) {
				break;
			}

			// Offset is shifted by 18 bytes and points to a moving
			// 4KB window
			tmp |= (size & 0xf0) << 4;
			offset = (pos & ~0xfff) | ((tmp + 18) & 0xfff);
			size = 3 + (size & 0xf);

			if (pos + size > bufsize) {
				delete[] buf;
				fprintf(stderr, "Decompression buffer overflow\n");
				return NULL;
			}

			if ((unsigned)offset >= pos) {
				offset -= 0x1000;
			}

			// Negative offset stands for a block of 0x20
			if (offset < 0) {
				tmp = min(size, (unsigned long)-offset);
				memset(buf + pos, 0x20, tmp);
				offset = 0;
				size -= tmp;
				pos += tmp;
			}

			for (j = 0; j < size; j++) {
				buf[pos++] = buf[offset++];
			}
		}
	}

	if (pos != bufsize) {
		delete[] buf;
		fprintf(stderr, "Decompressed size mismatch: %lu != %lu\n",
			pos, bufsize);
		return NULL;
	}

	ret = new MemoryReadStream(buf, bufsize);
	delete[] buf;
	return ret;
}

VVFStream::VVFStream(SeekableReadStream *stream) : _file(stream),
	_palette(NULL), _videobuf(NULL), _audiobuf(NULL), _frame_map(NULL),
	_stream_count(0), _block_count(0), _bundle_count(0), _playtime(0),
	_frames(0), _width(0), _height(0), _audiosize(0), _audiofreq(0),
	_audiochans(0), _audio_stream(-1), _video_stream(-1), _curstream(-1),
	_frametime(0), _startpos(0), _curblock(0), _curchunk(0), _curframe(0),
	_block_frames(0), _block_chunks(NULL), _block_sizes(NULL) {

	char buf[256];
	size_t size;
	unsigned bitmask, depth, i, *streamtypes;

	if (!_file) {
		return;
	}

	size = _file->read(buf, 23);
	buf[size] = '\0';

	if (strcmp(buf, "Valacirca Video Format")) {
		fprintf(stderr, "VVFStream(): Invalid file format\n");
		_file = NULL;
		return;
	}

	_file->seek(261, SEEK_CUR);
	_stream_count = _file->readUint32LE();
	_block_count = _file->readUint32LE();
	_bundle_count = _file->readUint32LE();
	_playtime = _file->readUint32LE();
	_frames = _file->readUint32LE();
	_file->seek(212, SEEK_CUR);

	if (_file->eos()) {
		fprintf(stderr, "VVFStream(): Premature end of file\n");
		_file = NULL;
		return;
	}

	if (_stream_count < 1) {
		fprintf(stderr, "VVFStream(): No streams found\n");
		_file = NULL;
		return;
	}

	if (_bundle_count < 1) {
		fprintf(stderr, "VVFStream(): No stream bundle found\n");
		_file = NULL;
		return;
	}

	streamtypes = new unsigned[_stream_count];

	for (i = 0; i < _stream_count; i++) {
		streamtypes[i] = _file->readUint32LE();
		_file->readUint32LE();
	}

	_file->seek(64, SEEK_CUR);
	bitmask = _file->readUint32LE();
	// Bitmask may have 36 bytes in total but only 3 bits are actually
	// used by Signus video files.
	_file->seek(32, SEEK_CUR);
	_width = _file->readUint32LE();
	_height = _file->readUint32LE();
	depth = _file->readUint8();

	if (depth != 8) {
		fprintf(stderr, "VVFStream(): Unsupported video bit depth %u\n",
			depth);
		delete[] streamtypes;
		_file = NULL;
		return;
	}

	// TODO: Allow bundle selection
	_file->seek(256 * _bundle_count - 109, SEEK_CUR);

	if (_file->eos()) {
		fprintf(stderr, "VVFStream(): Premature end of file\n");
		delete[] streamtypes;
		_file = NULL;
		return;
	}

	_startpos = _file->pos();

	// Select streams from bundle
	for (i = 0; i < _stream_count; i++) {
		if (!(bitmask & (1 << i))) {
			continue;
		}

		if (streamtypes[i] == VVF_AUDIO) {
			if (_audio_stream >= 0) {
				fprintf(stderr,
					"VVFStream(): Audio stream conflict\n");
				delete[] streamtypes;
				_file = NULL;
				return;
			}

			_audio_stream = i;
		}

		if (streamtypes[i] == VVF_VIDEO) {
			if (_video_stream >= 0) {
				fprintf(stderr,
					"VVFStream(): Video stream conflict\n");
				delete[] streamtypes;
				_file = NULL;
				return;
			}

			_video_stream = i;
		}
	}

	delete[] streamtypes;

	if (_video_stream < 0) {
		fprintf(stderr, "VVFStream(): No video stream selected\n");
		_file = NULL;
		return;
	}

	if (_audio_stream > _video_stream) {
		fprintf(stderr, "VVFStream(): Wrong stream order\n");
		_file = NULL;
		return;
	}

	if (!_width || !_height) {
		fprintf(stderr, "VVFStream(): Invalid video resolution\n");
		_file = NULL;
		return;
	}

	_videobuf = new uint8_t[_width * _height];
	_palette = new uint8_t[768];
	_block_chunks = new unsigned[_stream_count];
	_block_sizes = new unsigned[_stream_count];
	memset(_videobuf, 0, _width * _height * sizeof(uint8_t));
	memset(_palette, 0, 768 * sizeof(uint8_t));
	memset(_block_chunks, 0, _stream_count * sizeof(unsigned));
	memset(_block_sizes, 0, _stream_count * sizeof(unsigned));

	if (!init_block()) {
		_file = NULL;
		return;
	}
}

VVFStream::~VVFStream(void) {
	clear();
}

unsigned VVFStream::init_block(void) {
	MemoryReadStream *header;
	size_t header_size;
	unsigned i, opcode, size;
	int streamid;

	if (!_file) {
		return 0;
	}

	_block_frames = _file->readUint32LE();
	_file->readUint32LE();	// block play time, ignore
	header_size = _file->readUint32LE();

	for (i = 0; i < _stream_count; i++) {
		_block_chunks[i] = _file->readUint16LE();
	}

	_file->seek(512 - 2 * _stream_count, SEEK_CUR);

	for (i = 0; i < _stream_count; i++) {
		_block_sizes[i] = _file->readUint32LE();
	}

	_file->seek(1524 - 4 * _stream_count, SEEK_CUR);
	header = _file->readStream(header_size);

	if (_file->eos()) {
		delete header;
		fprintf(stderr, "init_block(): Premature end of file\n");
		return 0;
	}

	delete[] _frame_map;
	_frame_map = NULL;

	if (!_block_frames) {
		delete header;
		fprintf(stderr, "init_block(): No frames in block\n");
		return 0;
	}

	_frame_map = new uint8_t[_block_frames];
	memset(_frame_map, 0, _block_frames * sizeof(uint8_t));
	i = 0;

	while (1) {
		opcode = header->readUint16LE();
		size = header->readUint8();

		if (header->eos()) {
			delete header;
			fprintf(stderr, "init_block(): Invalid block header format\n");
			return 0;
		}

		// End of header marker
		if (opcode == 0x100) {
			break;
		}

		if (i >= _block_frames) {
			delete header;
			fprintf(stderr, "init_block(): Frame map overflow\n");
			return 0;
		}

		switch (opcode) {
		case 0:
			i++;
			break;

		case 0x200:
			if (size != 2) {
				delete header;
				fprintf(stderr, "init_block(): Frametime argument size mismatch: %u != 2\n", size);
				return 0;
			}

			_frametime = header->readUint16LE();
			break;

		case 0x300:
			if (size != 3) {
				delete header;
				fprintf(stderr, "init_block(): Frame chunk ID argument size mismatch: %u != 3\n", size);
				return 0;
			}

			streamid = header->readUint8();
			// Chunk ID, ignore. Chunk reordering not supported.
			header->readUint16LE();

			// Mark frames which update video buffer
			// Non-video streams will be processed all at once
			// in the first frame. Dirty but simpler.
			if (streamid == _video_stream) {
				_frame_map[i] = 1;
			}

			break;

		default:
			delete header;
			fprintf(stderr, "init_block(): Unknown opcode %u\n",
				opcode);
			return 0;
		}
	}

	delete header;
	_curstream = 0;
	_curchunk = 0;
	_curframe = 0;
	memset(_videobuf, 0, _width * _height * sizeof(uint8_t));
	return 1;
}

// Frame type 0x4
unsigned VVFStream::decode_palette(SeekableReadStream &stream) {
	unsigned i, packets, pos, size;

	packets = stream.readUint16LE();

	for (pos = 0, i = 0; i < packets; i++) {
		pos += 3 * (unsigned)stream.readUint8();
		size = 3 * (unsigned)stream.readUint8();

		if (stream.eos()) {
			fprintf(stderr, "decode_palette(): Premature end of video chunk\n");
			return 0;

		}

		if (!size) {
			size = 768;
		}

		if (pos + size > 768) {
			fprintf(stderr, "decode_palette(): Buffer overflow\n");
			return 0;
		}

		if (stream.read(_palette + pos, size) != size) {
			fprintf(stderr, "decode_palette(): Read error\n");
			return 0;
		}

		pos += size;
	}

	return 1;
}

// Frame type 0x10
unsigned VVFStream::decode_raw(SeekableReadStream &stream) {
	unsigned size;

	size = stream.read(_videobuf, _width * _height);

	if (size != _width * _height) {
		fprintf(stderr, "decode_raw(): Premature end of video chunk\n");
		return 0;
	}

	return 1;
}

// Frame type 0xf
unsigned VVFStream::decode_rle(SeekableReadStream &stream) {
	size_t pos = 0;
	unsigned i, x, y, runs;
	int size;
	uint8_t tmp;

	for (y = 0; y < _height; y++) {
		runs = stream.readUint8();

		for (x = 0, i = 0; i < runs; i++) {
			size = stream.readSint8();

			if (x + abs(size) > _width) {
				fprintf(stderr, "decode_rle(): Scanline overflow %u\n", x+abs(size));
				return 0;
			}

			if (size >= 0) {
				tmp = stream.readUint8();
				memset(_videobuf + pos, tmp, size);
			} else {
				size = -size;
				stream.read(_videobuf + pos, size);
			}

			pos += size;
			x += size;
		}

		if (stream.eos()) {
			fprintf(stderr, "decode_rle(): Premature end of video chunk\n");
			return 0;
		}

		if (x != _width) {
			fprintf(stderr, "decode_rle(): Scanline length mismatch, %u != %u\n", x, _width);
			return 0;
		}
	}

	return 1;
}

// Frame type 0x7
unsigned VVFStream::decode_delta(SeekableReadStream &stream) {
	unsigned opcode, offset, pos, x, y, line, delta_height;
	int i, j, size;
	uint8_t pix1, pix2;

	delta_height = stream.readUint16LE();

	for (y = 0, line = 0; line < delta_height;) {
		x = 0;
		opcode = stream.readUint16LE();

		if (stream.eos()) {
			fprintf(stderr, "decode_delta(): Premature end of video chunk 1\n");
			return 0;
		}

		switch (opcode >> 14) {
		case 3:	// Skip (-opcode) lines
			y += 0x10000 - opcode;
			continue;

		case 2:	// change last pixel on current line
			line++;
			y++;
			_videobuf[y * _width - 1] = opcode & 0xff;
			continue;

		case 0:	// regular delta line, decoded below
			break;

		default:
			fprintf(stderr, "decode_delta(): Invalid opcode %x\n",
				opcode);
			return 0;
		}

		if (y >= _height) {
			fprintf(stderr, "decode_delta(): Video height overflow\n");
			return 0;
		}

		for (i = 0; i < (int)opcode; i++) {
			offset = stream.readUint8();
			size = stream.readSint8();

			if (stream.eos()) {
				fprintf(stderr, "decode_delta(): Premature end of video chunk 2\n");
				return 0;
			}

			x += offset;
			pos = y * _width + x;

			if (pos + 2 * abs(size) > _width * _height) {
				fprintf(stderr, "decode_delta(): Video buffer overflow\n");
				return 0;
			}

			if (size > 0) {
				stream.read(_videobuf + pos, 2 * size);
			} else {
				size = -size;
				pix1 = stream.readUint8();
				pix2 = stream.readUint8();

				for (j = 0; j < size; j++) {
					_videobuf[pos++] = pix1;
					_videobuf[pos++] = pix2;
				}
			}

			if (stream.eos()) {
				fprintf(stderr, "decode_delta(): Premature end of video chunk 3\n");
				return 0;
			}

			x += 2 * size;
		}

		line++;
		y++;
	}

	return 1;
}

// Chunk type 0x1 (uncompressed) or 0xd (compressed)
unsigned VVFStream::decode_video(SeekableReadStream &stream) {
	unsigned segments, i, tmp, size, type, frames = 0;
	int palchange = 0, ret = 0;

	stream.readUint32LE();
	tmp = stream.readUint16LE();
	segments = stream.readUint16LE();

	if (stream.eos() || tmp != 0xf1fa) {
		fprintf(stderr, "decode_video(): Invalid video chunk format\n");
		return 0;
	}

	stream.seek(8, SEEK_CUR);

	for (i = 0; i < segments; i++) {
		size = stream.readUint32LE();
		type = stream.readUint16LE();

		if (stream.eos()) {
			fprintf(stderr, "decode_video(): Premature end of video chunk\n");
			return 0;
		}

		switch (type) {
		case 0x4:
			if (!decode_palette(stream)) {
				return 0;
			}

			palchange = 1;
			break;

		case 0x7:
			frames++;
			ret = decode_delta(stream);
			break;

		case 0xf:
			frames++;
			ret = decode_rle(stream);
			break;

		case 0x10:
			frames++;
			ret = decode_raw(stream);
			break;

		// TODO: Find out what this segment actually is
		case 0x12:
			stream.seek(size - 6, SEEK_CUR);
			break;

		default:
			fprintf(stderr, "decode_video(): Unknown video segment type %u\n",
				type);
			return 0;
		}
	}

	if (!frames) {
		fprintf(stderr, "decode_video(): No frame data in video chunk\n");
		return 0;
	} else if (frames > 1) {
		fprintf(stderr, "decode_video(): Multiple frame segments in video chunk\n");
		return 0;
	}

	if (!ret) {
		return 0;
	}

	return VVF_VIDEO | (palchange ? VVF_PALETTE : 0);
}

// Chunk type 0xe
unsigned VVFStream::decode_audio(SeekableReadStream &stream) {
	unsigned long size;
	unsigned format;

	format = stream.readUint8();

	if (format) {
		fprintf(stderr, "decode_audio(): Unknown audio format %u\n",
			format);
		return 0;
	}

	_audiofreq = stream.readUint32LE();
	format = stream.readUint8();
	_audiochans = stream.readUint8() + 1;
	size = stream.readUint32LE();

	if (stream.eos()) {
		fprintf(stderr, "decode_audio(): Premature end of audio chunk\n");
		return 0;
	}

	delete[] _audiobuf;
	_audiosize = size;
	_audiobuf = new uint8_t[_audiosize];
	size = stream.read(_audiobuf, _audiosize);

	if (size != _audiosize) {
		fprintf(stderr, "decode_audio(): Premature end of audio chunk %lu != %u\n", size, _audiosize);
		delete[] _audiobuf;
		_audiobuf = NULL;
		_audiosize = 0;
		return 0;
	}

	return VVF_AUDIO;
}

void VVFStream::clear(void) {
	delete[] _palette;
	delete[] _videobuf;
	delete[] _audiobuf;
	delete[] _frame_map;
	delete[] _block_chunks;
	delete[] _block_sizes;
}

unsigned VVFStream::next_frame(void) {
	MemoryReadStream *stream;
	size_t size;
	unsigned tmp, ctype, ret = 0;

	if (!_file) {
		return 0;
	}

	// End of block, initialize the next one
	if (_curframe >= _block_frames) {
		for (; _curchunk < _block_chunks[_curstream]; _curchunk++) {
			_file->readUint8();
			size = _file->readUint32LE();
			_file->seek(size, SEEK_CUR);
		}

		_curstream++;

		for (; _curstream < (int)_stream_count; _curstream++) {
			_file->seek(_block_sizes[_curstream], SEEK_CUR);
		}

		// End of file
		if (++_curblock >= _block_count) {
			return 0;
		}

		if (!init_block()) {
			_file = NULL;
			return 0;
		}
	}

	// Handle the audio stream in the first frame
	if (!_curframe && _curstream <= _audio_stream) {
		for (; _curstream < _audio_stream; _curstream++) {
			_file->seek(_block_sizes[_curstream], SEEK_CUR);
		}

		_curchunk = 0;

		for (; _curchunk < _block_chunks[_curstream]; _curchunk++) {
			ctype = _file->readUint8();
			size = _file->readUint32LE();

			if (_file->eos()) {
				_file = NULL;
				fprintf(stderr, "next_frame(): Premature end of file\n");
				return 0;
			}

			if (ctype != 3 && ctype != 14) {
				_file->seek(size, SEEK_CUR);
				continue;
			}

			stream = _file->readStream(size);

			if (_file->eos()) {
				delete stream;
				_file = NULL;
				fprintf(stderr, "next_frame(): Premature end of file\n");
				return 0;
			}

			if (ctype == 14) {
				ret = decode_audio(*stream);
			} else {
				PlayMusicBuffer(stream->dataPtr(), size);
			}

			delete stream;

			if (ctype == 14 && !ret) {
				_file = NULL;
				return 0;
			}
		}

		_curstream++;
		_curchunk = 0;
	}

	for (; _curstream < _video_stream; _curstream++) {
		_file->seek(_block_sizes[_curstream], SEEK_CUR);
	}

	if (!_frame_map[_curframe++]) {
		return ret | VVF_VIDEO;
	}

	if (_curchunk >= _block_chunks[_curstream]) {
		_file = NULL;
		fprintf(stderr, "next_frame(): Chunk past end of video stream\n");
		return 0;
	}

	ctype = _file->readUint8();
	size = _file->readUint32LE();

	if (_file->eos()) {
		_file = NULL;
		fprintf(stderr, "next_frame(): Premature end of file\n");
		return 0;
	}

	if (ctype == 13) {
		MemoryReadStream *srcstream = _file->readStream(size);

		if (_file->eos()) {
			delete srcstream;
			_file = NULL;
			fprintf(stderr, "next_frame(): Premature end of file\n");
			return 0;
		}

		stream = decompress(*srcstream);
		delete srcstream;

		if (!stream) {
			_file = NULL;
			return 0;
		}
	} else if (ctype == 1) {
		stream = _file->readStream(size);
	} else {
		_file = NULL;
		fprintf(stderr, "next_frame(): Unknown chunk type %u\n", ctype);
		return 0;
	}

	if (_file->eos()) {
		delete stream;
		_file = NULL;
		fprintf(stderr, "next_frame(): Premature end of file\n");
		return 0;
	}

	tmp = decode_video(*stream);
	delete stream;

	if (!tmp) {
		_file = NULL;
		return 0;
	}

	_curchunk++;
	return tmp | ret;
}

const uint8_t *VVFStream::palette(void) const {
	return _palette;
}

const uint8_t *VVFStream::videobuf(void) const {
	return _videobuf;
}

unsigned VVFStream::video_width(void) const {
	return _width;
}

unsigned VVFStream::video_height(void) const {
	return _height;
}

unsigned VVFStream::frame_time(void) const {
	return _frametime;
}

const uint8_t *VVFStream::audiobuf(void) const {
	return _audiobuf;
}

unsigned VVFStream::audio_freq(void) const {
	return _audiofreq;
}

unsigned VVFStream::audio_channels(void) const {
	return _audiochans;
}

unsigned VVFStream::audio_size(void) const {
	return _audiosize;
}

bool VVFStream::error(void) const {
	return !_file;
}

void VVFStream::reset(void) {
	if (!_file) {
		return;
	}

	_file->seek(_startpos, SEEK_SET);
	_curblock = 0;

	if (!init_block()) {
		_file = NULL;
	}
}

int open_anim_file(File &file, const char *name) {
	char fullname[PATH_MAX];

	snprintf(fullname, PATH_MAX, "anims/%s.vvf", name);
	multipath_fopen(file, fullname, File::READ);

	if (!file.isOpen()) {
		return 0;
	}

	return 1;
}

int check_skip_event(void) {
	TEvent e;

	do {
		GetEvent(&e);

		if ((e.What == evKeyDown) || (e.What == evMouseDown)) {
			return 1;
		}
	} while (e.What != evNothing);

	return 0;
}

int PlayAnimation(const char *name) {
	File stream;
	TEvent e;
	unsigned frame, width, height, timer, curtime;
	MIDASsample audio = INVALID_SAMPLE;

	if (!open_anim_file(stream, name)) {
		return 0;
	}

	VVFStream anim(&stream);

	MouseHide();
	StopMusic();
	ClearScr();
	DoneTimer();

	do {
		GetEvent(&e);
	} while (e.What != evNothing);

	width = anim.video_width();
	height = anim.video_height();
	timer = SDL_GetTicks();

	while ((frame = anim.next_frame())) {
		if (frame & VVFStream::VVF_AUDIO) {
			if (audio != INVALID_SAMPLE) {
				StopSample(audio);
				FreeSample(audio);
			}

			audio = ConvertSample(anim.audiobuf(),
				anim.audio_size(), anim.audio_freq(), -16,
				anim.audio_channels(), 0);
			PlaySample(audio, 0, SpeechVolume, 128);

			// Fix audio conversion delay
			timer = SDL_GetTicks();

			if (!(frame & VVFStream::VVF_VIDEO)) {
				if (timer > anim.frame_time()) {
					timer -= anim.frame_time();
				} else {
					timer = 0;
				}
			}
		}

		if (frame & VVFStream::VVF_PALETTE) {
			SetRawPalette(anim.palette());
		}

		if (frame & VVFStream::VVF_VIDEO) {
			DrawVideoFrame(anim.videobuf(), width, height);
			UpdateScreen();
		}

		if (check_skip_event()) {
			break;
		}

		timer += anim.frame_time();
		curtime = SDL_GetTicks();

		if (timer > curtime) {
			SDL_Delay(timer - curtime);
		}
	}

	StopMusic();

	if (audio != INVALID_SAMPLE) {
		StopSample(audio);
		FreeSample(audio);
	}

	do {
		GetEvent(&e);
	} while (e.What != evNothing);

	InitTimer();
	ClearScr();
	SetPalette(Palette);
	MouseShow();
	return !anim.error();
}
