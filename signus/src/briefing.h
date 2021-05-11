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


#ifndef _MENU_H
#define _MENU_H

#include <cstddef>

// consts:
#define MaxNumOfWords 1500
#define MaxNumOfLines 150

#define ScrollPixels 8

#define LeftSpace      148
#define RightSpace      28
#define UpSpace         28
#define DownSpace       28
#define ArticleSpace    10
#define PARA_SPACING     4

typedef enum {
	LAYOUT_SPACE = 0,
	LAYOUT_TEXT,
	LAYOUT_LINK,
	LAYOUT_IMAGE
} layout_box_t;

struct LayoutBox {
	layout_box_t type;
	unsigned x, y, width, height, index;
	const char *text;
};

class BriefingPage {
private:
	unsigned _width, _height, _curx, _cury;
	char *_name, *_tokens, **_links;
	LayoutBox *_layout, **_active_spots;
	size_t _layout_size, _layout_max, _link_count, _active_count;

	// Do NOT implement
	BriefingPage(const BriefingPage &other);
	const BriefingPage &operator=(const BriefingPage &other);

protected:
	void breakLine(int wordwrap);
	int addBox(layout_box_t type, const char *text, int newpara,
		unsigned width = 0, unsigned height = 0);
	void makeLink(unsigned index);

public:
	BriefingPage(const char *name, int width);
	~BriefingPage(void);

	int width(void) const;
	int height(void) const;
	const char *name(void) const;
	const char *get_link(unsigned x, unsigned y) const;
	void *render(void) const;
};


// fce:
extern void BriefGo(char *MissionName);




#endif
