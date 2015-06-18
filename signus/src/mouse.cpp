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


//
// Modul pro klavesnici a mys
//


#include "mouse.h"
#include "graphio.h"
#include "system.h"
#include "global.h"
#include <time.h>
#include <SDL_mouse.h>


//////////////////////////////// MYS /////////////////////////////////////

static int mouse_painting = FALSE;

TMouseCBD Mouse = {0};
// pomocna promnena - obsahuje vsechny data pouzivana behem INT handleru




int MouseInit()
{
	Mouse.MouseCurs[mcurArrow] = GraphicsDF->get("curarrow");
	Mouse.MouseCurs[mcurWait] = GraphicsDF->get("curwait");
	Mouse.MouseCurs[mcurSelect] = GraphicsDF->get("cursel");
	Mouse.MouseCurs[mcurSelectBig] = GraphicsDF->get("curselb");
	Mouse.MouseCurs[mcurSelectSmall] = GraphicsDF->get("cursels");
	Mouse.MouseCurs[mcurTarget] = GraphicsDF->get("curtar");
	Mouse.MouseCurs[mcurTargetBig] = GraphicsDF->get("curtarb");
	Mouse.MouseCurs[mcurTargetSmall] = GraphicsDF->get("curtars");
	Mouse.MouseCurs[mcurSupport] = GraphicsDF->get("cursupp");
	Mouse.MouseCurs[mcurLoadIn] = GraphicsDF->get("curload");
	Mouse.MouseCurs[mcurBuild] = GraphicsDF->get("curbuild");
	
	Mouse.back_buf = memalloc(32 * 32);
//	Mouse.cachebuf = memalloc(2*32 * 2*32);
//	Mouse.back = 0;
	
	Mouse.Locks = 1;
	MouseSetCursor(mcurArrow);
	MouseSetRect(0, 0, RES_X-1, RES_Y-1);
	MouseSetRatio(iniMouseRatioX, iniMouseRatioY);
	MouseSetPos(320, 240);
	
	MouseShow();
	return Mouse.back_buf != NULL;
}



void MouseDone()
{
	//MouseHide();
}




void MouseSetCursor(int cnum)
{
	if (Mouse.ActCur == cnum) return;
	Mouse.SuperLocks++;
	MouseHide();
	Mouse.ActCur = cnum;
	switch (cnum) {
		case mcurSupport :
		case mcurArrow : Mouse.xrpt = 0; Mouse.yrpt = 0; break;
		case mcurWait : Mouse.xrpt = 16; Mouse.yrpt = 16; break;
		case mcurBuild :
		case mcurLoadIn :
		case mcurSelect : 
		case mcurSelectBig : 
		case mcurSelectSmall : 
		case mcurTarget :
		case mcurTargetBig :                
		case mcurTargetSmall : Mouse.xrpt = 16; Mouse.yrpt = 16; break;
	}
	MouseShow();
	Mouse.SuperLocks--;
}



void MouseHide()
{
	Mouse.SuperLocks++;
	Mouse.Locks++;
	MousePaint();
	Mouse.SuperLocks--;
}



void MouseShow()
{    
	Mouse.SuperLocks++;
	Mouse.Locks--;
	MousePaint();
	Mouse.SuperLocks--;
}



// Zakaze kurzor v dane oblasti:
void MouseFreeze(int x, int y, int w, int h)
{
	if (mouse_painting) return;
	if (Mouse.FreezeHided) {
		Mouse.FreezeHided++, Mouse.SuperLocks++; 
		return;
	}
	
	int x1 = x - (32 - Mouse.xrpt);
	int y1 = y - (32 - Mouse.yrpt); 
	int x2 = x + w + Mouse.xrpt;
	int y2 = y + h + Mouse.yrpt;
	
	int mx = Mouse.x;
	int my = Mouse.y;
	int bx = (Mouse.back_x < 0) ? mx : Mouse.back_x;
	int by = (Mouse.back_y < 0) ? my : Mouse.back_y;
	
	Mouse.SuperLocks++;
	if (((mx >= x1 && mx <= x2) && (my >= y1 && my <= y2)) || ((bx >= x1 && bx <= x2) && (by >= y1 && by <= y2))) {
		Mouse.FreezeHided++;
		MouseHide();
	}
}



void MouseUnfreeze()
{
	if (mouse_painting) return;
	if (Mouse.FreezeHided) {
		if (!(--Mouse.FreezeHided)) MouseShow();
	}
	Mouse.SuperLocks--;
}



void MouseSetRect(int x1, int y1, int x2, int y2)
{
	#if 0
	TRect r;
	
	r.x1 = x1, r.y1 = y1, r.x2 = x2, r.y2 = y2;
	ClipCursor(&r); /// FIXME -- SDL cannot clip mouse cursor, do it ourselves!
SDL_WM_GrabInput(SDL_GRAB_ON);
	#endif
}



void MouseSetPos(int x, int y)
{
	Mouse.x = x;
	Mouse.y = y;
	MousePaint();
	SDL_WarpMouse(Mouse.x, Mouse.y);
}



void MouseSetRatio(int rx, int ry)
{
}









int IsIntersect(int ax, int ay, int bx, int by, int rax, int ray, int rbx, int rby)
{
	return ((max(ax, rax) <= min(bx, rbx)) && 
	(max(ay, ray) <= min(by, rby)));    
}

static void RestoreMouseBackground()
{
	if (Mouse.back_x >= 0) {
		int back_sx = min(RES_X - Mouse.back_x, 32);
		int back_sy = min(RES_Y - Mouse.back_y, 32);
		int x = Mouse.back_x;
		if (x < 0) {
			back_sx += x;
			x = 0;
		}
		int y = Mouse.back_y;
		if (y < 0) {
			back_sy += y;
			y = 0;
		}
	
		PutCurBack(Mouse.back_buf, x, y, back_sx, back_sy, 0, 0);
	}
	
	Mouse.back_x = -1;
	Mouse.back_y = -1;
}

static void StoreMouseBackground()
{
	Mouse.back_x = Mouse.x - Mouse.xrpt;
	Mouse.back_y = Mouse.y - Mouse.yrpt;
	
	int back_sx = min(RES_X - Mouse.back_x, 32);
	int back_sy = min(RES_Y - Mouse.back_y, 32);
	int x = Mouse.back_x;
	if (x < 0) {
		back_sx += x;
		x = 0;
	}
	int y = Mouse.back_y;
	if (y < 0) {
		back_sy += y;
		y = 0;
	}
	
	GetCurBack(Mouse.back_buf, x, y, back_sx, back_sy, 0, 0);
}

void MousePaint()
{
	RestoreMouseBackground();
	
	if (Mouse.Locks) {
		return;
	}
	
	mouse_painting = TRUE;
	
	StoreMouseBackground();
	
	int fx = max(0, -Mouse.back_x);
	int fy = max(0, -Mouse.back_y);
	int sx = min(RES_X - Mouse.back_x, 32) - fx;
	int sy = min(RES_Y - Mouse.back_y, 32) - fy;
	
	DrawCursor(Mouse.MouseCurs[Mouse.ActCur], max(Mouse.back_x, 0), max(Mouse.back_y, 0), sx, sy, fx, fy);
	
	mouse_painting = FALSE;
}




