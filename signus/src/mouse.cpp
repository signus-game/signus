/*
 *  This file is part of Signus: The Artefact Wars (http://signus.sf.net)
 *
 *  Copyright (C) 1997, 1998, 2002
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


#include "headers.h"

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

    Mouse.backbuf = memalloc(32 * 32);
    Mouse.cachebuf = memalloc(2*32 * 2*32);
    Mouse.back = 0;
            
    Mouse.Locks = 1;
    MouseSetCursor(mcurArrow);
    MouseSetRect(0, 0, RES_X-1, RES_Y-1);
    MouseSetRatio(iniMouseRatioX, iniMouseRatioY);
    MouseSetPos(320, 240);
    
    MouseShow();
    return Mouse.backbuf && Mouse.cachebuf;
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

    int x1 = x - (32 - Mouse.xrpt), y1 = y - (32 - Mouse.yrpt), 
        x2 = x + w + Mouse.xrpt, y2 = y + h + Mouse.yrpt;

    if (!Mouse.back) {
        Mouse.SuperLocks++;
        if (((Mouse.x >= x1) && (Mouse.x <= x2)) &&
            ((Mouse.y >= y1) && (Mouse.y <= y2))) {
            Mouse.FreezeHided++;
            MouseHide();
        }
    }
    else {
        Mouse.SuperLocks++;
        if ((((Mouse.x >= x1) && (Mouse.x <= x2)) &&
             ((Mouse.y >= y1) && (Mouse.y <= y2))) ||
        (((Mouse.bx >= x1) && (Mouse.bx <= x2)) &&
             ((Mouse.by >= y1) && (Mouse.by <= y2)))) {
            Mouse.FreezeHided++;
            MouseHide();
        }
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
    TRect r;

    r.x1 = x1, r.y1 = y1, r.x2 = x2, r.y2 = y2;
#if 0
    ClipCursor(&r); /// FIXME -- SDL cannot clip mouse cursor, do it ourselves!
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





void MousePaint()
{
    int dx1, dy1, dx2, dy2, i, j, i2, j2, dsx, dsy;
    byte *pb;

    if (Mouse.Locks) {
        if (Mouse.back) {
            PutCurBack(Mouse.backbuf, Mouse.bx, Mouse.by, Mouse.sx, Mouse.sy, Mouse.fx, Mouse.fy);
            Mouse.back = 0;
        }
        return;
    }
    
    Mouse.sx2 = 32, Mouse.sy2 = 32, Mouse.fx2 = 0, Mouse.fy2 = 0;
    Mouse.bx2 = Mouse.x - Mouse.xrpt, Mouse.by2 = Mouse.y - Mouse.yrpt;
    if (Mouse.bx2 < 0) {Mouse.fx2 = -Mouse.bx2; Mouse.sx2 = 32 - Mouse.fx2; Mouse.bx2 = 0;}
    else if (Mouse.bx2 > 160+608) {Mouse.sx2 = 32 - (Mouse.bx2 - (160+608));}
    if (Mouse.by2 < 0) {Mouse.fy2 = -Mouse.by2; Mouse.sy2 = 32 - Mouse.fy2; Mouse.by2 = 0;}
    else if (Mouse.by2 > 120+448) {Mouse.sy2 = 32 - (Mouse.by2 - 120-448);}


    mouse_painting = TRUE;

    if ((Mouse.back) && (IsIntersect(Mouse.bx, Mouse.by, Mouse.bx + Mouse.sx-1, Mouse.by + Mouse.sy-1,
      Mouse.bx2, Mouse.by2, Mouse.bx2 + Mouse.sx2-1, Mouse.by2 + Mouse.sy2-1))) {
    dx1 = min(Mouse.bx, Mouse.bx2),
    dy1 = min(Mouse.by, Mouse.by2),
    dx2 = max(Mouse.bx + Mouse.sx, Mouse.bx2 + Mouse.sx2),
    dy2 = max(Mouse.by + Mouse.sy, Mouse.by2 + Mouse.sy2),
    dsx = dx2 - dx1+1, dsy = dy2 - dy1+1;
    GetBitmap(dx1, dy1, Mouse.cachebuf, dsx, dsy);
    // obnov pozadi:
    for (i = Mouse.by - dy1, i2 = 0, pb = (byte*)Mouse.backbuf + Mouse.fx + Mouse.fy * 32; 
         i2 < Mouse.sy; i++, i2++, pb += 32-Mouse.sx)
        for (j = Mouse.bx - dx1, j2 = 0; j2 < Mouse.sx; j++, j2++, pb++)
            ((byte*)Mouse.cachebuf)[j + dsx * i] = *pb;
    // sejmi pozadi:
    for (i = Mouse.by2 - dy1, i2 = 0, pb = (byte*)Mouse.backbuf + Mouse.fx2 + Mouse.fy2 * 32; 
         i2 < Mouse.sy2; i++, i2++, pb += 32-Mouse.sx2)
        for (j = Mouse.bx2 - dx1, j2 = 0; j2 < Mouse.sx2; j++, j2++, pb++)
            *pb = ((byte*)Mouse.cachebuf)[j + dsx * i];
    // nakrelsi kurzor:
    for (i = Mouse.by2 - dy1, i2 = 0, pb = (byte*)Mouse.MouseCurs[Mouse.ActCur] + Mouse.fx2 + Mouse.fy2 * 32; 
         i2 < Mouse.sy2; i++, i2++, pb += 32-Mouse.sx2)
        for (j = Mouse.bx2 - dx1, j2 = 0; j2 < Mouse.sx2; j++, j2++, pb++)
            if (*pb != 0) ((byte*)Mouse.cachebuf)[j + dsx * i] = *pb;

    PutBitmap(dx1, dy1, Mouse.cachebuf, dsx, dsy);
    }



    else {
        if (Mouse.back) {
            PutCurBack(Mouse.backbuf, Mouse.bx, Mouse.by, Mouse.sx, Mouse.sy, Mouse.fx, Mouse.fy);
            Mouse.back = 0;
        }
        GetCurBack(Mouse.backbuf, Mouse.bx2, Mouse.by2, Mouse.sx2, Mouse.sy2, Mouse.fx2, Mouse.fy2);
        DrawCursor(Mouse.MouseCurs[Mouse.ActCur], 
                   Mouse.bx2, Mouse.by2, Mouse.sx2, Mouse.sy2, Mouse.fx2, Mouse.fy2);
        Mouse.back = 1;
    }

    mouse_painting = FALSE;
    
    Mouse.sx = Mouse.sx2; Mouse.sy = Mouse.sy2; 
    Mouse.bx = Mouse.bx2; Mouse.by = Mouse.by2;
    Mouse.fx = Mouse.fx2; Mouse.fy = Mouse.fy2;
}




