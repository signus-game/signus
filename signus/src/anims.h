/*
 *  This file is part of Signus: The Artefact Wars
 *  https://github.com/signus-game
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


// 
// Cutscene playback header file
//

#ifndef _ANIMS_H
#define _ANIMS_H

#include "stream.h"

class VVFStream {
private:
	SeekableReadStream *_file;
	uint8_t *_palette, *_videobuf, *_audiobuf, *_frame_map;
	unsigned _stream_count, _block_count, _bundle_count, _playtime;
	unsigned _frames, _width, _height, _audiosize, _audiofreq, _audiochans;
	int _audio_stream, _video_stream, _curstream;
	unsigned _frametime, _startpos, _curblock, _curchunk, _curframe;
	unsigned _block_frames, *_block_chunks, *_block_sizes;

	// Do no implement
	VVFStream(const VVFStream &other);
	const VVFStream &operator=(const VVFStream &other);

protected:
	unsigned init_block(void);
	unsigned decode_palette(SeekableReadStream &stream);
	unsigned decode_raw(SeekableReadStream &stream);
	unsigned decode_rle(SeekableReadStream &stream);
	unsigned decode_delta(SeekableReadStream &stream);
	unsigned decode_video(SeekableReadStream &stream);
	unsigned decode_audio(SeekableReadStream &stream);
	void clear(void);

public:
	typedef enum {
		VVF_AUDIO = 1,
		VVF_VIDEO = 2,
		// VVF_SUBTITLES = 4,
		VVF_PALETTE = 0x100
	} FrameType;

	explicit VVFStream(SeekableReadStream *stream);
	~VVFStream(void);

	unsigned next_frame(void);

	const uint8_t *palette(void) const;
	const uint8_t *videobuf(void) const;
	unsigned video_width(void) const;
	unsigned video_height(void) const;
	unsigned frame_time(void) const;

	const uint8_t *audiobuf(void) const;
	unsigned audio_freq(void) const;
	unsigned audio_channels(void) const;
	unsigned audio_size(void) const;

	bool error(void) const;
	void reset(void);
};

int open_anim_file(File &file, const char *name);

// Play fullscreen cutscene
int PlayAnimation(const char *name);

#endif
