/*
 *  This file is part of Signus: The Artefact Wars (http://signus.sf.net)
 *
 *  Copyright (C) 1997, 1998, 2002, 2003, 2004
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

  //********************************//
  // Missions' Briefing for Signus  //
  //       (hypertext viewer)       //
  //   (c) 1997 Valacirca by M.W.   //
  //********************************//




#include "briefing.h"
#include "global.h"
#include "graphio.h"
#include "events.h"
#include "fonts.h"
#include "mouse.h"

#include <cassert>
#include <SDL_timer.h>

// Fces:
void SetConstants();
int  BriefGetEvent();
void BriefHandleEvent(int What);
void BriefScrollUp(unsigned Num);
void BriefScrollDown(unsigned Num);
void BriefRedrawScr();


static BriefingPage *curpage = NULL;
static char **pageStack = NULL;
static size_t pagecount = 0, stacksize = 0;

void *BigDrawBuffer = NULL;

unsigned Ofset; // Posunuti scrollingu



// Consts:
int LinePixels;
size_t minheight;
int EXIT_X;
int EXIT_Y;
int EXIT_W;
int EXIT_H;
int BACK_X;
int BACK_Y;
int BACK_W;
int BACK_H;
int SCUP_X;
int SCUP_Y;
int SCUP_W;
int SCUP_H;
int SCDN_X;
int SCDN_Y;
int SCDN_W;
int SCDN_H;

BriefingPage::BriefingPage(const char *name, int width) : _width(width),
	_height(1), _curx(0), _cury(0), _name(NULL), _tokens(NULL),
	_links(NULL), _layout(NULL), _active_spots(NULL), _layout_size(0),
	_layout_max(1024), _link_count(0), _active_count(0) {

	char *tokptr, *text = (char*)TextsDF->get(name);
	unsigned char *ptr;
	unsigned i, w, h, link_max = 32;
	int ret, newpara = 0;

	if (!text) {
		return;
	}

	_name = (char*)memalloc(strlen(name) + 1);
	tokptr = _tokens = (char*)memalloc(strlen(text) + 1);
	_links = (char**)memalloc(link_max * sizeof(char*));
	_layout = (LayoutBox*)memalloc(_layout_max * sizeof(LayoutBox));

	if (!_name || !_tokens || !_links || !_layout) {
		memfree(text);
		memfree(_name);
		memfree(_tokens);
		memfree(_links);
		memfree(_layout);
		return;
	}

	strcpy(_name, name);

	for (ptr = (unsigned char*)text; *ptr; ) {
		// Skip whitespace
		for (; *ptr && *ptr <= ' '; ptr++);

		if (!*ptr) {
			break;
		// Handle regular word
		} else if (*ptr != '#') {
			for (i = 0; ptr[i] > ' ' && ptr[i] != '#'; i++) {
				tokptr[i] = ptr[i];
			}

			tokptr[i] = '\0';
			addBox(LAYOUT_TEXT, tokptr, newpara);
			newpara = 0;
			tokptr += i + 1;
			ptr += i;

			if (*ptr && *ptr <= ' ') {
				addBox(LAYOUT_SPACE, " ", 0);
			}

			continue;
		}

		// Handle markup
		ptr++;

		// End of main text
		if (*ptr == 'e') {
			break;
		}

		switch (*ptr) {
		// Start new paragraph
		case '>':
			newpara++;
			ptr++;
			break;

		// Link number
		case '0':
			for (i = 0; i < 3 && isdigit(ptr[i]); i++) {
				tokptr[i] = ptr[i];
			}

			tokptr[i] = '\0';
			// Do not increment tokptr here, the token won't be
			// needed during rendering
			ptr += i;
			makeLink(atoi(tokptr) - 1);

			if (*ptr && *ptr <= ' ') {
				addBox(LAYOUT_SPACE, " ", 0);
			}

			break;

		// Picture
		case 'p':
			ptr++;
			ret = sscanf((char*)ptr, ".%[^.].%u.%u.", tokptr, &w,
				&h);

			if (ret != 3) {
				break;
			}

			addBox(LAYOUT_IMAGE, tokptr, 0, w, h);
			newpara = 0;
			while (*tokptr++);

			for (i = 0; *ptr && i < 4; ptr++) {
				if (*ptr == '.') {
					i++;
				}
			}

			break;
		}
	}

	// Load link values
	while (*ptr) {
		// Find next markup token
		for (; *ptr && *ptr != '#'; ptr++);
		ptr++;

		// End of file
		if (*ptr == 'e') {
			break;
		// Some garbage that needs to be skipped
		} else if (*ptr != 'l') {
			continue;
		}

		// Skip whitespace
		for (ptr++; *ptr && *ptr <= ' '; ptr++);

		// Load link value
		for (i = 0; ptr[i] && ptr[i] != '#'; i++) {
			tokptr[i] = ptr[i];
		}

		tokptr[i++] = '\0';
		ptr += i;

		if (_link_count >= link_max) {
			char **ptr;

			link_max *= 2;
			ptr = (char**)memrealloc(_links,
				link_max * sizeof(char*));

			if (!ptr) {
				break;
			}

			_links = ptr;
		}

		_links[_link_count++] = tokptr;
		tokptr += i;
	}

	memfree(text);
	_active_spots = (LayoutBox**)memalloc(_active_count*sizeof(LayoutBox*));

	if (!_active_spots) {
		_active_count = 0;
		return;
	}

	// Validate link indices and create lookup table
	for (i = 0, w = 0; i < _layout_size; i++) {
		if (_layout[i].type == LAYOUT_LINK) {
			if (_layout[i].index >= _link_count) {
				_layout[i].type = LAYOUT_TEXT;
				_active_count--;
			} else {
				_active_spots[w++] = _layout + i;
			}
		}
	}
}

BriefingPage::~BriefingPage(void) {
	memfree(_name);
	memfree(_tokens);
	memfree(_links);
	memfree(_layout);
	memfree(_active_spots);
}

// Break and justify the last line in layout
void BriefingPage::breakLine(int wordwrap) {
	unsigned i, x, y, spaces = 0, adjust = 0, space_left, tmp;
	unsigned linewidth = _width - 2;
	size_t lineend, linestart;
	LayoutBox *ptr;

	if (!_layout_size) {
		return;
	}

	// find linebreak point before the last word (unless the line ends
	// with a space or the caller doesn't want wordwrap)
	i = _layout_size;
	ptr = _layout + _layout_size - 1;
	y = ptr->y;
	_curx = 0;
	_cury = ptr->y + ptr->height;
	for (; wordwrap && i > 0 && ptr->y == y && ptr->type != LAYOUT_SPACE;
		i--, ptr--);
	linestart = i;
	for (; i > 0 && ptr->type == LAYOUT_SPACE; i--, ptr--);
	lineend = i;

	// no spaces on the last line, cannot break
	if (!i || ptr->y != y) {
		return;
	}

	if (lineend >= _layout_size) {
		x = ptr->x + ptr->width;
	} else {
		x = _layout[lineend].x;
	}

	// line ends with a space, drop dangling spaces
	if (linestart == _layout_size) {
		_layout_size = lineend;
	}

	space_left = linewidth - x;

	for (i = lineend; i > 0 && _layout[i-1].y == y; i--) {
		if (_layout[i-1].type == LAYOUT_SPACE) {
			spaces++;
		}
	}

	// Divide remaining width among space boxes
	for (ptr = _layout + i; i < lineend; i++, ptr++) {
		ptr->x += adjust;

		if (ptr->type == LAYOUT_SPACE) {
			tmp = space_left / spaces;
			ptr->width += tmp;
			adjust += tmp;
			space_left -= tmp;
			spaces--;
		}
	}

	if (space_left > 0) {
		_layout[lineend - 1].width += space_left;
	}

	if (linestart >= _layout_size) {
		return;
	}

	// wrap remaining boxes to the next line and drop spaces from beginning
	// of line
	ptr = _layout + lineend;
	_cury = _layout[linestart].y + _layout[linestart].height;

	for (i = linestart; i < _layout_size; i++, ptr++, lineend++) {
		*ptr = _layout[i];
		ptr->x = _curx;
		ptr->y = _cury;
		_curx += ptr->width;
	}

	_layout_size = lineend;
}

int BriefingPage::addBox(layout_box_t type, const char *text, int newpara,
	unsigned width, unsigned height) {
	unsigned i, x = 0, y = 0, linewidth = _width - 2;
	LayoutBox *ptr = NULL;

	x = _curx;
	y = _cury;

	// Drop spaces from end of line
	if (newpara || type == LAYOUT_IMAGE) {
		i = _layout_size;

		for (; i > 0 && _layout[i-1].type == LAYOUT_SPACE; i--) {
			_layout_size--;
		}

		if (_curx > 0 && _layout_size) {
			ptr = _layout + _layout_size - 1;
			x = 0;
			y = ptr->y + ptr->height;
		}
	}

	if (_layout_size >= _layout_max) {
		size_t size = 2 * _layout_max;
		LayoutBox *ptr = (LayoutBox*)memrealloc(_layout,
			size * sizeof(LayoutBox));

		if (!ptr) {
			return 0;
		}

		_layout = ptr;
		_layout_max = size;
	}

	// Add image
	if (type == LAYOUT_IMAGE) {
		ptr = _layout + _layout_size;
		ptr->type = type;
		ptr->x = (_width - width) / 2;
		ptr->y = y + 16;
		ptr->width = width;
		ptr->height = height;
		ptr->index = 0;
		ptr->text = text;
		_layout_size++;
		_curx = 0;
		_cury = ptr->y + ptr->height;
		_height = _cury + 16;
		return 1;
	}

	if (!width) {
		width = GetStrWidth(text, NormalFont);
	}

	if (!height) {
		height = GetStrHeight(text, NormalFont);
	}

	// start new paragraph
	if (newpara) {
		x = GetStrWidth("MM", NormalFont);
		y += PARA_SPACING + (newpara - 1) * height;
	// line overflow, justify text and continue on next line
	} else if (_layout_size > 0 && x > 0 && x + width > linewidth) {
		breakLine(type != LAYOUT_SPACE);
		x = _curx;
		y = _cury;

		// Don't add space immediately after linebreak
		if (type == LAYOUT_SPACE) {
			return 1;
		}
	}

	// Add text box
	ptr = _layout + _layout_size;
	ptr->type = type;
	ptr->x = x;
	ptr->y = y;
	ptr->width = width;
	ptr->height = height;
	ptr->index = 0;
	ptr->text = text;
	_layout_size++;
	_curx = ptr->x + ptr->width;
	_cury = ptr->y;
	_height = ptr->y + 2 * ptr->height;

	if (type == LAYOUT_LINK) {
		_active_count++;
	}

	return 1;
}

void BriefingPage::makeLink(unsigned index) {
	size_t i = _layout_size;

	if (!i || _layout[--i].type != LAYOUT_TEXT) {
		return;
	}

	_layout[i].index = index;
	_layout[i].type = LAYOUT_LINK;
	_active_count++;
}

int BriefingPage::width(void) const {
	return _width;
}

int BriefingPage::height(void) const {
	return _height;
}

const char *BriefingPage::name(void) const {
	return _name;
}

const char *BriefingPage::get_link(unsigned x, unsigned y) const {
	size_t i;
	LayoutBox **ptr;

	for (i = 0, ptr = _active_spots; i < _active_count; i++, ptr++) {
		if (x >= (*ptr)->x && x < (*ptr)->x + (*ptr)->width &&
			y >= (*ptr)->y && y < (*ptr)->y + (*ptr)->height) {
			return _links[(*ptr)->index];
		}
	}

	return NULL;
}

void *BriefingPage::render(void) const {
	size_t i, size;
	LayoutBox *ptr;
	void *img, *ret;

	size = _width * (_height <= minheight ? minheight + 1 : _height);
	ret = memalloc(size);
	memset(ret, 0, size);

	for (i = 0, ptr = _layout; i < _layout_size; i++, ptr++) {
		switch (ptr->type) {
		case LAYOUT_TEXT:
			PutStr(ret, _width, _height, ptr->x, ptr->y, ptr->text,
				NormalFont, clrWhite, clrBlack);
			break;

		case LAYOUT_LINK:
			PutStr(ret, _width, _height, ptr->x, ptr->y, ptr->text,
				NormalFont, clrRed, clrBlack);
			break;

		case LAYOUT_IMAGE:
			img = GraphicsDF->get(ptr->text);

			if (img) {
				CopyBmpNZ(ret, _width, ptr->x, ptr->y, img,
					ptr->width, ptr->height);
				memfree(img);
			}

			break;

		default:
			break;
		}
	}

	return ret;
}

// Fces:
void SetConstants()
{
    int IR ;
    
  LinePixels = RES_X-(LeftSpace+RightSpace);
    if (iniResolution-0x0100 == 1) IR =   0;
    if (iniResolution-0x0100 == 3) IR = 120;
    if (iniResolution-0x0100 == 5) IR = 288;
    
  minheight = RES_Y - UpSpace - DownSpace;//544
  
  EXIT_X = 15;
  EXIT_Y = 152 + IR;
  EXIT_W = 91;
  EXIT_H = 91;
  
  BACK_X = 15;
  BACK_Y = 262 + IR;
  BACK_W = 91;
  BACK_H = 91;
  
  
  SCUP_X = 15;
  SCUP_Y = 371 + IR;
  SCUP_W = 32;
  SCUP_H = 91;


  SCDN_X = 71;
  SCDN_Y = 371 + IR;
  SCDN_W = 32;
  SCDN_H = 91;
    
}


void drawGUI(void) {
	void *ptr;
	char buf[20];

	sprintf(buf, "%ibriefbk", iniResolution - 0x0100);
	ptr = GraphicsDF->get(buf);
	MouseHide();
	DrawPicture(ptr);
	MouseShow();
	memfree(ptr);
}

int changePage(const char *pagename) {
	BriefingPage *page = new BriefingPage(pagename, LinePixels);

	if (!page->name()) {
		delete page;
		return 0;
	}

	delete curpage;
	curpage = page;
	memfree(BigDrawBuffer);
	BigDrawBuffer = curpage->render();
	Ofset = 0;
	BriefRedrawScr();
	return 1;
}

void pushPage(const char *pagename) {
	size_t size;
	char *stackname = (char*)memalloc(strlen(pagename) + 1);

	if (stackname) {
		strcpy(stackname, pagename);
	}

	if (!changePage(pagename)) {
		memfree(stackname);
		return;
	}

	if (!stackname) {
		return;
	}

	if (pagecount >= stacksize) {
		char **ptr;

		size = stacksize ? 2 * stacksize : 32;
		ptr = (char**)memrealloc(pageStack, size * sizeof(char*));

		if (!ptr) {
			return;
		}

		pageStack = ptr;
		stacksize = size;
	}

	pageStack[pagecount++] = stackname;
}

void popPage(void) {
	if (pagecount < 2) {
		return;
	}

	pagecount--;
	memfree(pageStack[pagecount]);
	changePage(pageStack[pagecount - 1]);
}

void firstPage(void) {
	size_t i;

	for (i = 1; i < pagecount; i++) {
		memfree(pageStack[i]);
	}

	pagecount = 1;
	changePage(pageStack[0]);
}

void freeStack(void) {
	size_t i;

	for (i = 0; i < pagecount; i++) {
		memfree(pageStack[i]);
	}

	memfree(pageStack);
	pagecount = 0;
	stacksize = 0;
}

void BriefGo(char *MissionName) {
	int BriefEvent;
        TEvent e;

	SetConstants();
	drawGUI();
	pushPage(MissionName);
	FadeIn(Palette, 0);

	// -7: Exit, -4: Up, -3: Down, -2: PgUp, -1: PgDown
	do {
		BriefEvent = BriefGetEvent();
		BriefHandleEvent(BriefEvent);
	} while (BriefEvent != -7);

	delete curpage;
	curpage = NULL;
	memfree(BigDrawBuffer);
	freeStack();
	FadeOut(Palette, 0);

	do {
		GetEvent(&e);
	} while (e.What != evNothing);
}

// GET EVENT

int BriefGetEvent()
{

    int x,y;
    TEvent Ev;
    GetEvent(&Ev);

  // Scrolling (mouse)
  if (Ev.What == evNothing) {
    if (IsInRect(Mouse.x, Mouse.y, LeftSpace, RES_Y-DownSpace, RES_X-RightSpace, RES_Y)) 
        return -5;
    if (IsInRect(Mouse.x, Mouse.y, LeftSpace, 0, RES_X-RightSpace, UpSpace)) 
        return -6;
  }
    if (Mouse.buttons == mbLeftButton) {
            // Kliknuti na Exit
        if (IsInRect(Mouse.x, Mouse.y, EXIT_X, EXIT_Y, EXIT_X + EXIT_W-1, EXIT_Y + EXIT_H-1)) 
        {
            return -7;
      }
            // Kliknuti na Back
        if (IsInRect(Mouse.x, Mouse.y, BACK_X, BACK_Y, BACK_X + BACK_W-1, BACK_Y + BACK_H-1)) 
        {
            return -8;
      }
      
            // Kliknuti na UP
        if (IsInRect(Mouse.x, Mouse.y, SCUP_X, SCUP_Y, SCUP_X + SCUP_W-1, SCUP_Y + SCUP_H-1)) 
        {
            return -4;
      }
      
            // Kliknuti na DOWN
        if (IsInRect(Mouse.x, Mouse.y, SCDN_X, SCDN_Y, SCDN_X + SCDN_W-1, SCDN_Y + SCDN_H-1)) 
        {
            return -3;
      }
    }



    if (Ev.What == evMouseDown) {
        if (Ev.Mouse.Buttons == mbRightButton) {
            return -8;
        } else
        

        if (Ev.Mouse.Buttons == mbLeftButton) {
            // Kliknuti na Exit
        if (IsInRect(Ev.Mouse.Where.x, Ev.Mouse.Where.y, EXIT_X, EXIT_Y, EXIT_X + EXIT_W-1, EXIT_Y + EXIT_H-1)) 
        {
            return -7;
      }
            // Kliknuti na Back
        if (IsInRect(Ev.Mouse.Where.x, Ev.Mouse.Where.y, BACK_X, BACK_Y, BACK_X + BACK_W-1, BACK_Y + BACK_H-1)) 
        {
            return -8;
      }
      
            // Kliknuti na UP
        if (IsInRect(Ev.Mouse.Where.x, Ev.Mouse.Where.y, SCUP_X, SCUP_Y, SCUP_X + SCUP_W-1, SCUP_Y + SCUP_H-1)) 
        {
            return -4;
      }
      
            // Kliknuti na DOWN
        if (IsInRect(Ev.Mouse.Where.x, Ev.Mouse.Where.y, SCDN_X, SCDN_Y, SCDN_X + SCDN_W-1, SCDN_Y + SCDN_H-1)) 
        {
            return -3;
      }
      
      // Kliknuti do oblasti textu
        if (IsInRect(Ev.Mouse.Where.x, Ev.Mouse.Where.y, LeftSpace, UpSpace, RES_X-RightSpace, RES_Y-DownSpace)) 
        {
            x = Ev.Mouse.Where.x       -LeftSpace;
            y = Ev.Mouse.Where.y+Ofset-UpSpace;
		const char *link;

		if ((link = curpage->get_link(x, y))) {
			pushPage(link);
			return 0;
		}
      }
      
      
        }
    } else if (Ev.What == evMouseScroll) {
		return Ev.Mouse.Scroll > 0 ? -4 : -3;
    // Udalosti z klavesnice:   
    } else if (Ev.What == evKeyDown) {
/*      HandleIcons(e);
        SelectedUnit->InfoEvent(e);*/
        
        switch (Ev.Key.KeyCode) {
            case kbEsc : {
		return pagecount > 1 ? -9 : -7;
            }
            case kbEnter : {
              return -7;
            }
            case kbUp :
              return -4;
            case kbDown :
              return -3;
            case kbPgUp :
              return -2;
            case kbPgDn :
              return -1;
            case kbBack :
              return -8;
	    default:
		return 0;
    }
    }
    return 0;
}

void BriefHandleEvent(int What) 
// -7: Exit, -4: Up, -3: Down, -2: PgUp, -1: PgDown
{
    SDL_Delay(2);
  switch (What) {
    case -7 : break;
    case -6 : BriefScrollUp(6); break;
    case -5 : BriefScrollDown(6); break;
    case -4 : BriefScrollUp(32); break;
    case -3 : BriefScrollDown(32); break;
    case -2 : BriefScrollUp(minheight); break;
    case -1 : BriefScrollDown(minheight);break;
    case -8 : popPage(); break;
    case -9 : firstPage(); break;
  }
}

void BriefRedrawScr() {
	PutBitmap32(LeftSpace, UpSpace,
		(char*)BigDrawBuffer+(LinePixels*Ofset), LinePixels, minheight);
}


void BriefScrollUp(unsigned Num) {
	unsigned OldOfs = Ofset;

	if (Ofset < Num) {
		Ofset = 0;
	} else {
		Ofset -= Num;
	}

	if (OldOfs != Ofset) {
		MouseHide();
		BriefRedrawScr();
		MouseShow();
	}
}
void BriefScrollDown(unsigned Num) {
	unsigned OldOfs = Ofset;
	unsigned height = curpage->height();

	if (height <= minheight) {
		Ofset = 0;
	} else if (Ofset + Num > height - minheight) {
		Ofset = height - minheight;
	} else {
		Ofset += Num;
	}
    
	if (OldOfs != Ofset) {
		MouseHide();
		BriefRedrawScr();
		MouseShow();
	}
}
