; 
;   This file is part of Signus: The Artefact Wars (http://signus.sf.net)
; 
;   Copyright (C) 1997, 1998, 2002
;   Vaclav Slavik, Richard Wunsch, Marek Wunsch
; 
;   This program is free software; you can redistribute it and/or modify
;   it under the terms of the GNU General Public License version 2 as
;   published by the Free Software Foundation.
; 
;   This program is distributed in the hope that it will be useful,
;   but WITHOUT ANY WARRANTY; without even the implied warranty of
;   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;   GNU General Public License for more details.
; 
;   You should have received a copy of the GNU General Public License
;   along with this program; if not, write to the Free Software
;   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
; 


;
; 
; Modul s rutinami pro vykreslovani terenu.
; cili: DrawTerrA az DrawTerrM; 
; plus rutiny na vykreslovani do lokalizacniho bufferu
; plus skrolovani
;


bits 32

%include "c32.mac"


section .text

extern MapBuf
extern LocalBufX
extern LocalBufY
extern DarkingTable


; 800x600 resolution: Res=screen width, Mbd=???
%define Res        800
%define Mbd        168

%include "engdraw_x86_1.asm"
%include "engdraw_x86_2.asm"

