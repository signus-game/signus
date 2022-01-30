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



/*

MAINMENU : Vstupni menu hry - osetreni

*/

#ifndef _MAINMENU_H
#define _MAINMENU_H

#define MAINMENU_NEWGAME 0
#define MAINMENU_LOADGAME 1
#define MAINMENU_CREDITS 2
#define MAINMENU_EXIT 3
#define MAINMENU_MAX 3

extern int DoMainMenu();
    // provadi vyber v menu a vraci prislusne hodnoty

extern void ShowCredits();

#endif
