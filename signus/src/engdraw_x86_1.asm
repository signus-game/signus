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


; in Watcom C, it used to be:
;   extern void DrawTerr?int x, int y, void *bmp);
;   #pragma aux DrawTerr? parm [EDI] [EAX] [ESI]


; Parametr kreslene bitmapy je vzdy ulozen do ESI,
; Y-souradnice do EAX a X do EDI:


proc DrawTerrA

arg         %$x
arg         %$y
arg         %$bmp
            push edi
            push eax
            push esi
            mov  edi,[ebp + %$x]
            mov  eax,[ebp + %$y]
            mov  esi,[ebp + %$bmp]

			push ecx
			push edx
			mov  edx,(560+Mbd)
			mul  edx
			add  edi,eax
			add  edi, dword [MapBuf]
	
			add edi,26
			movsd
			add edi,Mbd+554
			mov ecx,2
			rep movsd
			add edi,Mbd+550
			mov ecx,3
			rep movsd
			add edi,Mbd+546
			mov ecx,4
			rep movsd
			add edi,Mbd+542
			mov ecx,5
			rep movsd
			add edi,Mbd+538
			mov ecx,6
			rep movsd
			add edi,Mbd+534
			mov ecx,7
			rep movsd
			add edi,Mbd+530
			mov ecx,8
			rep movsd
			add edi,Mbd+526
			mov ecx,9
			rep movsd
			add edi,Mbd+522
			mov ecx,10
			rep movsd
			add edi,Mbd+518
			mov ecx,11
			rep movsd
			add edi,Mbd+514
			mov ecx,12
			rep movsd
			add edi,Mbd+510
			mov ecx,13
			rep movsd
			add edi,Mbd+506
			mov ecx,14
			rep movsd
			add edi,Mbd+506
			mov ecx,13
			rep movsd
			add edi,Mbd+510
			mov ecx,12
			rep movsd
			add edi,Mbd+514
			mov ecx,11
			rep movsd
			add edi,Mbd+518
			mov ecx,10
			rep movsd
			add edi,Mbd+522
			mov ecx,9
			rep movsd
			add edi,Mbd+526
			mov ecx,8
			rep movsd
			add edi,Mbd+530
			mov ecx,7
			rep movsd
			add edi,Mbd+534
			mov ecx,6
			rep movsd
			add edi,Mbd+538
			mov ecx,5
			rep movsd
			add edi,Mbd+542
			mov ecx,4
			rep movsd
			add edi,Mbd+546
			mov ecx,3
			rep movsd
			add edi,Mbd+550
			mov ecx,2
			rep movsd
			add edi,Mbd+554
			movsd

			pop  edx
			pop  ecx

            pop  esi
            pop  eax
            pop  edi
endproc






proc DrawTerrB

arg         %$x
arg         %$y
arg         %$bmp
            push edi
            push eax
            push esi
            mov  edi,[ebp + %$x]
            mov  eax,[ebp + %$y]
            mov  esi,[ebp + %$bmp]

			push ecx
			push edx
			mov  edx,(560+Mbd)
			mul  edx
			add  edi,eax
			add  edi, dword [MapBuf]

			add edi,26
			movsd
			add edi,Mbd+555
			movsd
			movsw
			movsb
			add edi,Mbd+552
			mov ecx,2
			rep movsd
			movsw
			add edi,Mbd+549
			mov ecx,3
			rep movsd
			movsb
			add edi,Mbd+546
			mov ecx,4
			rep movsd
			add edi,Mbd+543
			mov ecx,4
			rep movsd
			movsw
			movsb
			add edi,Mbd+540
			mov ecx,5
			rep movsd
			movsw
			add edi,Mbd+537
			mov ecx,6
			rep movsd
			movsb
			add edi,Mbd+534
			mov ecx,7
			rep movsd
			add edi,Mbd+531
			mov ecx,7
			rep movsd
			movsw
			movsb
			add edi,Mbd+528
			mov ecx,8
			rep movsd
			movsw
			add edi,Mbd+525
			mov ecx,9
			rep movsd
			movsb
			add edi,Mbd+522
			mov ecx,10
			rep movsd
			add edi,Mbd+519
			mov ecx,10
			rep movsd
			movsw
			movsb
			add edi,Mbd+516
			mov ecx,10
			rep movsd
			movsw
			add edi,Mbd+517
			mov ecx,10
			rep movsd
			movsw
			add edi,Mbd+517
			mov ecx,10
			rep movsd
			movsw
			add edi,Mbd+517
			mov ecx,10
			rep movsd
			movsw
			add edi,Mbd+517
			mov ecx,10
			rep movsd
			movsw
			add edi,Mbd+517
			mov ecx,10
			rep movsd
			movsw
			add edi,Mbd+517
			mov ecx,10
			rep movsd
			movsw
			add edi,Mbd+517
			mov ecx,10
			rep movsd
			movsw
			add edi,Mbd+517
			mov ecx,10
			rep movsd
			movsw
			add edi,Mbd+517
			mov ecx,10
			rep movsd
			movsw
			add edi,Mbd+517
			mov ecx,10
			rep movsd
			movsw
			add edi,Mbd+517
			mov ecx,10
			rep movsd
			movsw
			add edi,Mbd+517
			mov ecx,10
			rep movsd
			movsw
			add edi,Mbd+518
			mov ecx,10
			rep movsd
			movsb
			add edi,Mbd+521
			mov ecx,9
			rep movsd
			movsw
			add edi,Mbd+524
			mov ecx,8
			rep movsd
			movsw
			movsb
			add edi,Mbd+527
			mov ecx,8
			rep movsd
			add edi,Mbd+530
			mov ecx,7
			rep movsd
			movsb
			add edi,Mbd+533
			mov ecx,6
			rep movsd
			movsw
			add edi,Mbd+536
			mov ecx,5
			rep movsd
			movsw
			movsb
			add edi,Mbd+539
			mov ecx,5
			rep movsd
			add edi,Mbd+542
			mov ecx,4
			rep movsd
			movsb
			add edi,Mbd+545
			mov ecx,3
			rep movsd
			movsw
			add edi,Mbd+548
			mov ecx,2
			rep movsd
			movsw
			movsb
			add edi,Mbd+551
			mov ecx,2
			rep movsd
			add edi,Mbd+554
			movsd
			movsb
			add edi,Mbd+557
			movsw

			pop  edx
			pop  ecx

            pop  esi
            pop  eax
            pop  edi
endproc






proc DrawTerrC

arg         %$x
arg         %$y
arg         %$bmp
            push edi
            push eax
            push esi
            mov  edi,[ebp + %$x]
            mov  eax,[ebp + %$y]
            mov  esi,[ebp + %$bmp]

			push ecx
			push edx
			mov  edx,(560+Mbd)
			mul  edx
			add  edi,eax
			add  edi, dword [MapBuf]


			add edi,26
			movsd
			add edi,Mbd+554
			movsd
			movsw
			movsb
			add edi,Mbd+551
			mov ecx,2
			rep movsd
			movsw
			add edi,Mbd+548
			mov ecx,3
			rep movsd
			movsb
			add edi,Mbd+545
			mov ecx,4
			rep movsd
			add edi,Mbd+542
			mov ecx,4
			rep movsd
			movsw
			movsb
			add edi,Mbd+539
			mov ecx,5
			rep movsd
			movsw
			add edi,Mbd+536
			mov ecx,6
			rep movsd
			movsb
			add edi,Mbd+533
			mov ecx,7
			rep movsd
			add edi,Mbd+530
			mov ecx,7
			rep movsd
			movsw
			movsb
			add edi,Mbd+527
			mov ecx,8
			rep movsd
			movsw
			add edi,Mbd+524
			mov ecx,9
			rep movsd
			movsb
			add edi,Mbd+521
			mov ecx,10
			rep movsd
			add edi,Mbd+518
			mov ecx,10
			rep movsd
			movsw
			movsb
			add edi,Mbd+519
			mov ecx,10
			rep movsd
			movsw
			add edi,Mbd+519
			mov ecx,10
			rep movsd
			movsw
			add edi,Mbd+519
			mov ecx,10
			rep movsd
			movsw
			add edi,Mbd+519
			mov ecx,10
			rep movsd
			movsw
			add edi,Mbd+519
			mov ecx,10
			rep movsd
			movsw
			add edi,Mbd+519
			mov ecx,10
			rep movsd
			movsw
			add edi,Mbd+519
			mov ecx,10
			rep movsd
			movsw
			add edi,Mbd+519
			mov ecx,10
			rep movsd
			movsw
			add edi,Mbd+519
			mov ecx,10
			rep movsd
			movsw
			add edi,Mbd+519
			mov ecx,10
			rep movsd
			movsw
			add edi,Mbd+519
			mov ecx,10
			rep movsd
			movsw
			add edi,Mbd+519
			mov ecx,10
			rep movsd
			movsw
			add edi,Mbd+519
			mov ecx,10
			rep movsd
			movsw
			add edi,Mbd+519
			mov ecx,10
			rep movsd
			movsb
			add edi,Mbd+520
			mov ecx,9
			rep movsd
			movsw
			add edi,Mbd+523
			mov ecx,8
			rep movsd
			movsw
			movsb
			add edi,Mbd+526
			mov ecx,8
			rep movsd
			add edi,Mbd+529
			mov ecx,7
			rep movsd
			movsb
			add edi,Mbd+532
			mov ecx,6
			rep movsd
			movsw
			add edi,Mbd+535
			mov ecx,5
			rep movsd
			movsw
			movsb
			add edi,Mbd+538
			mov ecx,5
			rep movsd
			add edi,Mbd+541
			mov ecx,4
			rep movsd
			movsb
			add edi,Mbd+544
			mov ecx,3
			rep movsd
			movsw
			add edi,Mbd+547
			mov ecx,2
			rep movsd
			movsw
			movsb
			add edi,Mbd+550
			mov ecx,2
			rep movsd
			add edi,Mbd+553
			movsd
			movsb
			add edi,Mbd+556
			movsw

			pop  edx
			pop  ecx

            pop  esi
            pop  eax
            pop  edi
endproc






proc DrawTerrD

arg         %$x
arg         %$y
arg         %$bmp
            push edi
            push eax
            push esi
            mov  edi,[ebp + %$x]
            mov  eax,[ebp + %$y]
            mov  esi,[ebp + %$bmp]

			push ecx
			push edx
			mov  edx,(560+Mbd)
			mul  edx
			add  edi,eax
			add  edi, dword [MapBuf]


			add edi,14*(560+Mbd)+2
			mov ecx,7
			rep movsd
			add edi,Mbd+534
			mov ecx,7
			rep movsd
			add edi,Mbd+534
			mov ecx,7
			rep movsd
			add edi,Mbd+534
			mov ecx,7
			rep movsd
			add edi,Mbd+534
			mov ecx,7
			rep movsd
			add edi,Mbd+534
			mov ecx,7
			rep movsd
			add edi,Mbd+534
			mov ecx,7
			rep movsd
			add edi,Mbd+534
			mov ecx,7
			rep movsd
			add edi,Mbd+534
			mov ecx,7
			rep movsd
			add edi,Mbd+534
			mov ecx,7
			rep movsd
			add edi,Mbd+534
			mov ecx,7
			rep movsd
			add edi,Mbd+534
			mov ecx,7
			rep movsd
			add edi,Mbd+534
			mov ecx,7
			rep movsd
			add edi,Mbd+534
			mov ecx,7
			rep movsd

			pop  edx
			pop  ecx

            pop  esi
            pop  eax
            pop  edi
endproc






proc DrawTerrE

arg         %$x
arg         %$y
arg         %$bmp
            push edi
            push eax
            push esi
            mov  edi,[ebp + %$x]
            mov  eax,[ebp + %$y]
            mov  esi,[ebp + %$bmp]

			push ecx
			push edx
			mov  edx,(560+Mbd)
			mul  edx
			add  edi,eax
			add  edi, dword [MapBuf]
			add edi,14*(560+Mbd)+26
			mov ecx,7
			rep movsd
			add edi,Mbd+530
			mov ecx,7
			rep movsd
			add edi,Mbd+530
			mov ecx,7
			rep movsd
			add edi,Mbd+530
			mov ecx,7
			rep movsd
			add edi,Mbd+530
			mov ecx,7
			rep movsd
			add edi,Mbd+530
			mov ecx,7
			rep movsd
			add edi,Mbd+530
			mov ecx,7
			rep movsd
			add edi,Mbd+530
			mov ecx,7
			rep movsd
			add edi,Mbd+530
			mov ecx,7
			rep movsd
			add edi,Mbd+530
			mov ecx,7
			rep movsd
			add edi,Mbd+530
			mov ecx,7
			rep movsd
			add edi,Mbd+530
			mov ecx,7
			rep movsd
			add edi,Mbd+530
			mov ecx,7
			rep movsd
			add edi,Mbd+530
			mov ecx,7
			rep movsd

			pop  edx
			pop  ecx

            pop  esi
            pop  eax
            pop  edi
endproc






proc DrawTerrF

arg         %$x
arg         %$y
arg         %$bmp
            push edi
            push eax
            push esi
            mov  edi,[ebp + %$x]
            mov  eax,[ebp + %$y]
            mov  esi,[ebp + %$bmp]

			push ecx
			push edx
			mov  edx,(560+Mbd)
			mul  edx
			add  edi,eax
			add  edi, dword [MapBuf]


			add edi,26
			movsd
			add edi,Mbd+555
			movsd
			movsw
			add edi,Mbd+553
			mov ecx,2
			rep movsd
			add edi,Mbd+551
			mov ecx,2
			rep movsd
			movsw
			add edi,Mbd+549
			mov ecx,3
			rep movsd
			add edi,Mbd+547
			mov ecx,3
			rep movsd
			movsw
			add edi,Mbd+545
			mov ecx,4
			rep movsd
			add edi,Mbd+543
			mov ecx,4
			rep movsd
			movsw
			add edi,Mbd+541
			mov ecx,5
			rep movsd
			add edi,Mbd+539
			mov ecx,5
			rep movsd
			movsw
			add edi,Mbd+537
			mov ecx,6
			rep movsd
			add edi,Mbd+535
			mov ecx,6
			rep movsd
			movsw
			add edi,Mbd+533
			mov ecx,7
			rep movsd
			add edi,Mbd+531
			mov ecx,7
			rep movsd
			movsw
			add edi,Mbd+529
			mov ecx,8
			rep movsd
			add edi,Mbd+527
			mov ecx,8
			rep movsd
			movsw
			add edi,Mbd+525
			mov ecx,9
			rep movsd
			add edi,Mbd+523
			mov ecx,9
			rep movsd
			movsw
			add edi,Mbd+521
			mov ecx,10
			rep movsd
			add edi,Mbd+519
			mov ecx,10
			rep movsd
			movsw
			add edi,Mbd+517
			mov ecx,11
			rep movsd
			add edi,Mbd+515
			mov ecx,11
			rep movsd
			movsw
			add edi,Mbd+513
			mov ecx,12
			rep movsd
			add edi,Mbd+511
			mov ecx,12
			rep movsd
			movsw
			add edi,Mbd+509
			mov ecx,13
			rep movsd
			add edi,Mbd+507
			mov ecx,13
			rep movsd
			movsw
			add edi,Mbd+505
			mov ecx,14
			rep movsd
			add edi,Mbd+504
			mov ecx,14
			rep movsd
			add edi,Mbd+506
			mov ecx,13
			rep movsd
			add edi,Mbd+510
			mov ecx,12
			rep movsd
			add edi,Mbd+514
			mov ecx,11
			rep movsd
			add edi,Mbd+518
			mov ecx,10
			rep movsd
			add edi,Mbd+522
			mov ecx,9
			rep movsd
			add edi,Mbd+526
			mov ecx,8
			rep movsd
			add edi,Mbd+530
			mov ecx,7
			rep movsd
			add edi,Mbd+534
			mov ecx,6
			rep movsd
			add edi,Mbd+538
			mov ecx,5
			rep movsd
			add edi,Mbd+542
			mov ecx,4
			rep movsd
			add edi,Mbd+546
			mov ecx,3
			rep movsd
			add edi,Mbd+550
			mov ecx,2
			rep movsd
			add edi,Mbd+554
			movsd

			pop  edx
			pop  ecx

            pop  esi
            pop  eax
            pop  edi
endproc






proc DrawTerrG

arg         %$x
arg         %$y
arg         %$bmp
            push edi
            push eax
            push esi
            mov  edi,[ebp + %$x]
            mov  eax,[ebp + %$y]
            mov  esi,[ebp + %$bmp]

			push ecx
			push edx
			mov  edx,(560+Mbd)
			mul  edx
			add  edi,eax
			add  edi, dword [MapBuf]


			add edi, 14*(560+Mbd)+2
			mov ecx,7
			rep movsd
			add edi,Mbd+533
			mov ecx,7
			rep movsd
			movsb
			add edi,Mbd+532
			mov ecx,7
			rep movsd
			movsw
			add edi,Mbd+531
			mov ecx,7
			rep movsd
			movsw
			movsb
			add edi,Mbd+530
			mov ecx,8
			rep movsd
			add edi,Mbd+529
			mov ecx,8
			rep movsd
			movsb
			add edi,Mbd+528
			mov ecx,8
			rep movsd
			movsw
			add edi,Mbd+527
			mov ecx,8
			rep movsd
			movsw
			movsb
			add edi,Mbd+526
			mov ecx,9
			rep movsd
			add edi,Mbd+525
			mov ecx,9
			rep movsd
			movsb
			add edi,Mbd+524
			mov ecx,9
			rep movsd
			movsw
			add edi,Mbd+523
			mov ecx,9
			rep movsd
			movsw
			movsb
			add edi,Mbd+522
			mov ecx,10
			rep movsd
			add edi,Mbd+521
			mov ecx,10
			rep movsd
			movsb
			add edi,Mbd+520
			mov ecx,9
			rep movsd
			movsw
			add edi,Mbd+523
			mov ecx,8
			rep movsd
			movsw
			movsb
			add edi,Mbd+526
			mov ecx,8
			rep movsd
			add edi,Mbd+529
			mov ecx,7
			rep movsd
			movsb
			add edi,Mbd+532
			mov ecx,6
			rep movsd
			movsw
			add edi,Mbd+535
			mov ecx,5
			rep movsd
			movsw
			movsb
			add edi,Mbd+538
			mov ecx,5
			rep movsd
			add edi,Mbd+541
			mov ecx,4
			rep movsd
			movsb
			add edi,Mbd+544
			mov ecx,3
			rep movsd
			movsw
			add edi,Mbd+547
			mov ecx,2
			rep movsd
			movsw
			movsb
			add edi,Mbd+550
			mov ecx,2
			rep movsd
			add edi,Mbd+553
			movsd
			movsb
			add edi,Mbd+556
			movsw

			pop  edx
			pop  ecx

            pop  esi
            pop  eax
            pop  edi
endproc






proc DrawTerrH

arg         %$x
arg         %$y
arg         %$bmp
            push edi
            push eax
            push esi
            mov  edi,[ebp + %$x]
            mov  eax,[ebp + %$y]
            mov  esi,[ebp + %$bmp]

			push ecx
			push edx
			mov  edx,(560+Mbd)
			mul  edx
			add  edi,eax
			add  edi, dword [MapBuf]


			add edi,14*(560+Mbd)+26
			movsd
			add edi,Mbd+554
			mov ecx,2
			rep movsd
			add edi,Mbd+550
			mov ecx,3
			rep movsd
			add edi,Mbd+546
			mov ecx,4
			rep movsd
			add edi,Mbd+542
			mov ecx,5
			rep movsd
			add edi,Mbd+538
			mov ecx,6
			rep movsd
			add edi,Mbd+534
			mov ecx,7
			rep movsd
			add edi,Mbd+530
			mov ecx,8
			rep movsd
			add edi,Mbd+526
			mov ecx,9
			rep movsd
			add edi,Mbd+522
			mov ecx,10
			rep movsd
			add edi,Mbd+518
			mov ecx,11
			rep movsd
			add edi,Mbd+514
			mov ecx,12
			rep movsd
			add edi,Mbd+510
			mov ecx,13
			rep movsd
			add edi,Mbd+506
			mov ecx,14
			rep movsd


			pop  edx
			pop  ecx

            pop  esi
            pop  eax
            pop  edi
endproc






proc DrawTerrI

arg         %$x
arg         %$y
arg         %$bmp
            push edi
            push eax
            push esi
            mov  edi,[ebp + %$x]
            mov  eax,[ebp + %$y]
            mov  esi,[ebp + %$bmp]

			push ecx
			push edx
			mov  edx,(560+Mbd)
			mul  edx
			add  edi,eax
			add  edi, dword [MapBuf]

			add edi,14*(560+Mbd)+26
			mov ecx,7
			rep movsd
			add edi,Mbd+530
			mov ecx,7
			rep movsd
			movsb
			add edi,Mbd+529
			mov ecx,7
			rep movsd
			movsw
			add edi,Mbd+528
			mov ecx,7
			rep movsd
			movsw
			movsb
			add edi,Mbd+527
			mov ecx,8
			rep movsd
			add edi,Mbd+526
			mov ecx,8
			rep movsd
			movsb
			add edi,Mbd+525
			mov ecx,8
			rep movsd
			movsw
			add edi,Mbd+524
			mov ecx,8
			rep movsd
			movsw
			movsb
			add edi,Mbd+523
			mov ecx,9
			rep movsd
			add edi,Mbd+522
			mov ecx,9
			rep movsd
			movsb
			add edi,Mbd+521
			mov ecx,9
			rep movsd
			movsw
			add edi,Mbd+520
			mov ecx,9
			rep movsd
			movsw
			movsb
			add edi,Mbd+519
			mov ecx,10
			rep movsd
			add edi,Mbd+518
			mov ecx,10
			rep movsd
			movsb
			add edi,Mbd+521
			mov ecx,9
			rep movsd
			movsw
			add edi,Mbd+524
			mov ecx,8
			rep movsd
			movsw
			movsb
			add edi,Mbd+527
			mov ecx,8
			rep movsd
			add edi,Mbd+530
			mov ecx,7
			rep movsd
			movsb
			add edi,Mbd+533
			mov ecx,6
			rep movsd
			movsw
			add edi,Mbd+536
			mov ecx,5
			rep movsd
			movsw
			movsb
			add edi,Mbd+539
			mov ecx,5
			rep movsd
			add edi,Mbd+542
			mov ecx,4
			rep movsd
			movsb
			add edi,Mbd+545
			mov ecx,3
			rep movsd
			movsw
			add edi,Mbd+548
			mov ecx,2
			rep movsd
			movsw
			movsb
			add edi,Mbd+551
			mov ecx,2
			rep movsd
			add edi,Mbd+554
			movsd
			movsb
			add edi,Mbd+557
			movsw

			pop  edx
			pop  ecx

            pop  esi
            pop  eax
            pop  edi
endproc






proc DrawTerrJ

arg         %$x
arg         %$y
arg         %$bmp
            push edi
            push eax
            push esi
            mov  edi,[ebp + %$x]
            mov  eax,[ebp + %$y]
            mov  esi,[ebp + %$bmp]

			push ecx
			push edx
			mov  edx,(560+Mbd)
			mul  edx
			add  edi,eax
			add  edi, dword [MapBuf]


			add edi,26
			movsd
			add edi,Mbd+554
			mov ecx,2
			rep movsd
			add edi,Mbd+550
			mov ecx,3
			rep movsd
			add edi,Mbd+546
			mov ecx,4
			rep movsd
			add edi,Mbd+542
			mov ecx,5
			rep movsd
			add edi,Mbd+538
			mov ecx,6
			rep movsd
			add edi,Mbd+534
			mov ecx,7
			rep movsd
			add edi,Mbd+530
			mov ecx,8
			rep movsd
			add edi,Mbd+526
			mov ecx,9
			rep movsd
			add edi,Mbd+522
			mov ecx,10
			rep movsd
			add edi,Mbd+518
			mov ecx,11
			rep movsd
			add edi,Mbd+514
			mov ecx,12
			rep movsd
			add edi,Mbd+510
			mov ecx,13
			rep movsd
			add edi,Mbd+506
			mov ecx,14
			rep movsd
			add edi,Mbd+506
			mov ecx,13
			rep movsd
			add edi,Mbd+509
			mov ecx,12
			rep movsd
			movsw
			add edi,Mbd+511
			mov ecx,12
			rep movsd
			add edi,Mbd+513
			mov ecx,11
			rep movsd
			movsw
			add edi,Mbd+515
			mov ecx,11
			rep movsd
			add edi,Mbd+517
			mov ecx,10
			rep movsd
			movsw
			add edi,Mbd+519
			mov ecx,10
			rep movsd
			add edi,Mbd+521
			mov ecx,9
			rep movsd
			movsw
			add edi,Mbd+523
			mov ecx,9
			rep movsd
			add edi,Mbd+525
			mov ecx,8
			rep movsd
			movsw
			add edi,Mbd+527
			mov ecx,8
			rep movsd
			add edi,Mbd+529
			mov ecx,7
			rep movsd
			movsw
			add edi,Mbd+531
			mov ecx,7
			rep movsd
			add edi,Mbd+533
			mov ecx,6
			rep movsd
			movsw
			add edi,Mbd+535
			mov ecx,6
			rep movsd
			add edi,Mbd+537
			mov ecx,5
			rep movsd
			movsw
			add edi,Mbd+539
			mov ecx,5
			rep movsd
			add edi,Mbd+541
			mov ecx,4
			rep movsd
			movsw
			add edi,Mbd+543
			mov ecx,4
			rep movsd
			add edi,Mbd+545
			mov ecx,3
			rep movsd
			movsw
			add edi,Mbd+547
			mov ecx,3
			rep movsd
			add edi,Mbd+549
			mov ecx,2
			rep movsd
			movsw
			add edi,Mbd+551
			mov ecx,2
			rep movsd
			add edi,Mbd+553
			movsd
			movsw
			add edi,Mbd+555
			movsd
			add edi,Mbd+557
			movsw

			pop  edx
			pop  ecx

            pop  esi
            pop  eax
            pop  edi
endproc






proc DrawTerrK

arg         %$x
arg         %$y
arg         %$bmp
            push edi
            push eax
            push esi
            mov  edi,[ebp + %$x]
            mov  eax,[ebp + %$y]
            mov  esi,[ebp + %$bmp]

			push ecx
			push edx
			mov  edx,(560+Mbd)
			mul  edx
			add  edi,eax
			add  edi, dword [MapBuf]


			add edi,26
			movsd
			add edi,Mbd+554
			movsd
			movsw
			movsb
			add edi,Mbd+551
			mov ecx,2
			rep movsd
			movsw
			add edi,Mbd+548
			mov ecx,3
			rep movsd
			movsb
			add edi,Mbd+545
			mov ecx,4
			rep movsd
			add edi,Mbd+542
			mov ecx,4
			rep movsd
			movsw
			movsb
			add edi,Mbd+539
			mov ecx,5
			rep movsd
			movsw
			add edi,Mbd+536
			mov ecx,6
			rep movsd
			movsb
			add edi,Mbd+533
			mov ecx,7
			rep movsd
			add edi,Mbd+530
			mov ecx,7
			rep movsd
			movsw
			movsb
			add edi,Mbd+527
			mov ecx,8
			rep movsd
			movsw
			add edi,Mbd+524
			mov ecx,9
			rep movsd
			movsb
			add edi,Mbd+521
			mov ecx,10
			rep movsd
			add edi,Mbd+518
			mov ecx,10
			rep movsd
			movsw
			movsb
			add edi,Mbd+519
			mov ecx,10
			rep movsd
			movsw
			add edi,Mbd+520
			mov ecx,10
			rep movsd
			movsb
			add edi,Mbd+521
			mov ecx,10
			rep movsd
			add edi,Mbd+522
			mov ecx,9
			rep movsd
			movsw
			movsb
			add edi,Mbd+523
			mov ecx,9
			rep movsd
			movsw
			add edi,Mbd+524
			mov ecx,9
			rep movsd
			movsb
			add edi,Mbd+525
			mov ecx,9
			rep movsd
			add edi,Mbd+526
			mov ecx,8
			rep movsd
			movsw
			movsb
			add edi,Mbd+527
			mov ecx,8
			rep movsd
			movsw
			add edi,Mbd+528
			mov ecx,8
			rep movsd
			movsb
			add edi,Mbd+529
			mov ecx,8
			rep movsd
			add edi,Mbd+530
			mov ecx,7
			rep movsd
			movsw
			movsb
			add edi,Mbd+531
			mov ecx,7
			rep movsd
			movsw
			add edi,Mbd+532
			mov ecx,7
			rep movsd

			pop  edx
			pop  ecx

            pop  esi
            pop  eax
            pop  edi
endproc






proc DrawTerrL

arg         %$x
arg         %$y
arg         %$bmp
            push edi
            push eax
            push esi
            mov  edi,[ebp + %$x]
            mov  eax,[ebp + %$y]
            mov  esi,[ebp + %$bmp]

			push ecx
			push edx
			mov  edx,(560+Mbd)
			mul  edx
			add  edi,eax
			add  edi, dword [MapBuf]


			add edi,14*(560+Mbd)+2
			mov ecx,13
			rep movsd
			add edi,Mbd+510
			mov ecx,12
			rep movsd
			add edi,Mbd+514
			mov ecx,11
			rep movsd
			add edi,Mbd+518
			mov ecx,10
			rep movsd
			add edi,Mbd+522
			mov ecx,9
			rep movsd
			add edi,Mbd+526
			mov ecx,8
			rep movsd
			add edi,Mbd+530
			mov ecx,7
			rep movsd
			add edi,Mbd+534
			mov ecx,6
			rep movsd
			add edi,Mbd+538
			mov ecx,5
			rep movsd
			add edi,Mbd+542
			mov ecx,4
			rep movsd
			add edi,Mbd+546
			mov ecx,3
			rep movsd
			add edi,Mbd+550
			mov ecx,2
			rep movsd
			add edi,Mbd+554
			movsd

			pop  edx
			pop  ecx

            pop  esi
            pop  eax
            pop  edi
endproc






proc DrawTerrM

arg         %$x
arg         %$y
arg         %$bmp
            push edi
            push eax
            push esi
            mov  edi,[ebp + %$x]
            mov  eax,[ebp + %$y]
            mov  esi,[ebp + %$bmp]

			push ecx
			push edx
			mov  edx,(560+Mbd)
			mul  edx
			add  edi,eax
			add  edi, dword [MapBuf]


			add edi,26
			movsd
			add edi,Mbd+555
			movsd
			movsw
			movsb
			add edi,Mbd+552
			mov ecx,2
			rep movsd
			movsw
			add edi,Mbd+549
			mov ecx,3
			rep movsd
			movsb
			add edi,Mbd+546
			mov ecx,4
			rep movsd
			add edi,Mbd+543
			mov ecx,4
			rep movsd
			movsw
			movsb
			add edi,Mbd+540
			mov ecx,5
			rep movsd
			movsw
			add edi,Mbd+537
			mov ecx,6
			rep movsd
			movsb
			add edi,Mbd+534
			mov ecx,7
			rep movsd
			add edi,Mbd+531
			mov ecx,7
			rep movsd
			movsw
			movsb
			add edi,Mbd+528
			mov ecx,8
			rep movsd
			movsw
			add edi,Mbd+525
			mov ecx,9
			rep movsd
			movsb
			add edi,Mbd+522
			mov ecx,10
			rep movsd
			add edi,Mbd+519
			mov ecx,10
			rep movsd
			movsw
			movsb
			add edi,Mbd+516
			mov ecx,10
			rep movsd
			movsw
			add edi,Mbd+517
			mov ecx,10
			rep movsd
			movsb
			add edi,Mbd+518
			mov ecx,10
			rep movsd
			add edi,Mbd+519
			mov ecx,9
			rep movsd
			movsw
			movsb
			add edi,Mbd+520
			mov ecx,9
			rep movsd
			movsw
			add edi,Mbd+521
			mov ecx,9
			rep movsd
			movsb
			add edi,Mbd+522
			mov ecx,9
			rep movsd
			add edi,Mbd+523
			mov ecx,8
			rep movsd
			movsw
			movsb
			add edi,Mbd+524
			mov ecx,8
			rep movsd
			movsw
			add edi,Mbd+525
			mov ecx,8
			rep movsd
			movsb
			add edi,Mbd+526
			mov ecx,8
			rep movsd
			add edi,Mbd+527
			mov ecx,7
			rep movsd
			movsw
			movsb
			add edi,Mbd+528
			mov ecx,7
			rep movsd
			movsw
			add edi,Mbd+530
			mov ecx,7
			rep movsd

			pop  edx
			pop  ecx

            pop  esi
            pop  eax
            pop  edi
endproc




;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; Lokalizacni bufer ;;;;;;;;;;;;;;;;;;;;;;;;;;;;


; extern void DrawLocalA640(void *LocBuf, int x, int y, int color);
; #pragma aux DrawLocalA640 parm [ESI] [EDI] [EAX] [EBX]


; Barva bitmapy (tj. souradnice) do BL, do ESI ukazatel na l.buf.
; Y-souradnice do EAX a X do EDI:


proc DrawLocalA

arg         %$LocBuf
arg         %$x
arg         %$y
arg         %$color
            push esi
            push edi
            push eax
            push ebx
            mov  esi,[ebp + %$LocBuf]
            mov  edi,[ebp + %$x]
            mov  eax,[ebp + %$y]
            mov  ebx,[ebp + %$color]

			push ecx
			push edx
			mov  edx,(280+(Mbd/2))
			mul  edx
			add  edi,eax
      mov  bh,bl
      mov  ax,bx
      mov  cl,16
      shl  eax,cl
      mov  ax,bx
			add  edi,esi

			add edi,13
			stosw
			add edi,(Mbd/2)+276
			stosd
			stosw
			add edi,(Mbd/2)+272
			mov ecx,2
			rep stosd
			stosw
			add edi,(Mbd/2)+268
			mov ecx,3
			rep stosd
			stosw
			add edi,(Mbd/2)+264
			mov ecx,4
			rep stosd
			stosw
			add edi,(Mbd/2)+260
			mov ecx,5
			rep stosd
			stosw
			add edi,(Mbd/2)+256
			mov ecx,6
			rep stosd
			stosw
			add edi,(Mbd/2)+254
			mov ecx,6
			rep stosd
			stosw
			add edi,(Mbd/2)+256
			mov ecx,5
			rep stosd
			stosw
			add edi,(Mbd/2)+260
			mov ecx,4
			rep stosd
			stosw
			add edi,(Mbd/2)+264
			mov ecx,3
			rep stosd
			stosw
			add edi,(Mbd/2)+268
			mov ecx,2
			rep stosd
			stosw
			add edi,(Mbd/2)+272
			stosd
			stosw
			add edi,(Mbd/2)+276
			stosw

			pop  edx
			pop  ecx

            pop  ebx
            pop  eax
            pop  edi
            pop  esi
endproc




proc DrawLocalB

arg         %$LocBuf
arg         %$x
arg         %$y
arg         %$color
            push esi
            push edi
            push eax
            push ebx
            mov  esi,[ebp + %$LocBuf]
            mov  edi,[ebp + %$x]
            mov  eax,[ebp + %$y]
            mov  ebx,[ebp + %$color]

			push ecx
			push edx
			mov  edx,(280+(Mbd/2))
			mul  edx
			add  edi,eax
      mov  bh,bl
      mov  ax,bx
      mov  cl,16
      shl  eax,cl
      mov  ax,bx
			add  edi,esi

			add edi,13
			stosw
			add edi,(Mbd/2)+277
			stosd
			stosb
			add edi,(Mbd/2)+274
			mov ecx,2
			rep stosd
			add edi,(Mbd/2)+271
			mov ecx,2
			rep stosd
			stosw
			stosb
			add edi,(Mbd/2)+268
			mov ecx,3
			rep stosd
			stosw
			add edi,(Mbd/2)+265
			mov ecx,4
			rep stosd
			stosb
			add edi,(Mbd/2)+262
			mov ecx,5
			rep stosd
			add edi,(Mbd/2)+259
			mov ecx,5
			rep stosd
			stosb
			add edi,(Mbd/2)+258
			mov ecx,5
			rep stosd
			stosb
			add edi,(Mbd/2)+258
			mov ecx,5
			rep stosd
			stosb
			add edi,(Mbd/2)+258
			mov ecx,5
			rep stosd
			stosb
			add edi,(Mbd/2)+258
			mov ecx,5
			rep stosd
			stosb
			add edi,(Mbd/2)+258
			mov ecx,5
			rep stosd
			stosb
			add edi,(Mbd/2)+258
			mov ecx,5
			rep stosd
			stosb
			add edi,(Mbd/2)+260
			mov ecx,4
			rep stosd
			stosw
			stosb
			add edi,(Mbd/2)+263
			mov ecx,4
			rep stosd
			add edi,(Mbd/2)+266
			mov ecx,3
			rep stosd
			stosb
			add edi,(Mbd/2)+269
			mov ecx,2
			rep stosd
			stosw
			add edi,(Mbd/2)+272
			stosd
			stosw
			stosb
			add edi,(Mbd/2)+275
			stosd
			add edi,(Mbd/2)+278
			stosb

			pop  edx
			pop  ecx

            pop  ebx
            pop  eax
            pop  edi
            pop  esi
endproc



proc DrawLocalC

arg         %$LocBuf
arg         %$x
arg         %$y
arg         %$color
            push esi
            push edi
            push eax
            push ebx
            mov  esi,[ebp + %$LocBuf]
            mov  edi,[ebp + %$x]
            mov  eax,[ebp + %$y]
            mov  ebx,[ebp + %$color]

			push ecx
			push edx
			mov  edx,(280+(Mbd/2))
			mul  edx
			add  edi,eax
      mov  bh,bl
      mov  ax,bx
      mov  cl,16
      shl  eax,cl
      mov  ax,bx
			add  edi,esi

			add edi,13
			stosw
			add edi,(Mbd/2)+276
			stosd
			stosb
			add edi,(Mbd/2)+273
			mov ecx,2
			rep stosd
			add edi,(Mbd/2)+270
			mov ecx,2
			rep stosd
			stosw
			stosb
			add edi,(Mbd/2)+267
			mov ecx,3
			rep stosd
			stosw
			add edi,(Mbd/2)+264
			mov ecx,4
			rep stosd
			stosb
			add edi,(Mbd/2)+261
			mov ecx,5
			rep stosd
			add edi,(Mbd/2)+260
			mov ecx,5
			rep stosd
			stosb
			add edi,(Mbd/2)+260
			mov ecx,5
			rep stosd
			stosb
			add edi,(Mbd/2)+260
			mov ecx,5
			rep stosd
			stosb
			add edi,(Mbd/2)+260
			mov ecx,5
			rep stosd
			stosb
			add edi,(Mbd/2)+260
			mov ecx,5
			rep stosd
			stosb
			add edi,(Mbd/2)+260
			mov ecx,5
			rep stosd
			stosb
			add edi,(Mbd/2)+260
			mov ecx,5
			rep stosd
			stosb
			add edi,(Mbd/2)+260
			mov ecx,4
			rep stosd
			stosw
			stosb
			add edi,(Mbd/2)+262
			mov ecx,4
			rep stosd
			add edi,(Mbd/2)+265
			mov ecx,3
			rep stosd
			stosb
			add edi,(Mbd/2)+268
			mov ecx,2
			rep stosd
			stosw
			add edi,(Mbd/2)+271
			stosd
			stosw
			stosb
			add edi,(Mbd/2)+274
			stosd
			add edi,(Mbd/2)+277
			stosb

			pop  edx
			pop  ecx

            pop  ebx
            pop  eax
            pop  edi
            pop  esi
endproc



proc DrawLocalD

arg         %$LocBuf
arg         %$x
arg         %$y
arg         %$color
            push esi
            push edi
            push eax
            push ebx
            mov  esi,[ebp + %$LocBuf]
            mov  edi,[ebp + %$x]
            mov  eax,[ebp + %$y]
            mov  ebx,[ebp + %$color]

			push ecx
			push edx
			mov  edx,(280+(Mbd/2))
			mul  edx
			add  edi,eax
      mov  bh,bl
      mov  ax,bx
      mov  cl,16
      shl  eax,cl
      mov  ax,bx
			add  edi,esi

			add edi,7*(280+(Mbd/2))+1
			mov ecx,3
			rep stosd
			stosw
			add edi,(Mbd/2)+268
			mov ecx,3
			rep stosd
			stosw
			add edi,(Mbd/2)+268
			mov ecx,3
			rep stosd
			stosw
			add edi,(Mbd/2)+268
			mov ecx,3
			rep stosd
			stosw
			add edi,(Mbd/2)+268
			mov ecx,3
			rep stosd
			stosw
			add edi,(Mbd/2)+268
			mov ecx,3
			rep stosd
			stosw
			add edi,(Mbd/2)+268
			mov ecx,3
			rep stosd
			stosw

			pop  edx
			pop  ecx

            pop  ebx
            pop  eax
            pop  edi
            pop  esi
endproc



proc DrawLocalE

arg         %$LocBuf
arg         %$x
arg         %$y
arg         %$color
            push esi
            push edi
            push eax
            push ebx
            mov  esi,[ebp + %$LocBuf]
            mov  edi,[ebp + %$x]
            mov  eax,[ebp + %$y]
            mov  ebx,[ebp + %$color]

			push ecx
			push edx
			mov  edx,(280+(Mbd/2))
			mul  edx
			add  edi,eax
      mov  bh,bl
      mov  ax,bx
      mov  cl,16
      shl  eax,cl
      mov  ax,bx
			add  edi,esi

			add edi,14+7*(280+(Mbd/2))-1
			mov ecx,3
			rep stosd
			stosw
			add edi,(Mbd/2)+264
			mov ecx,3
			rep stosd
			stosw
			add edi,(Mbd/2)+264
			mov ecx,3
			rep stosd
			stosw
			add edi,(Mbd/2)+264
			mov ecx,3
			rep stosd
			stosw
			add edi,(Mbd/2)+264
			mov ecx,3
			rep stosd
			stosw
			add edi,(Mbd/2)+264
			mov ecx,3
			rep stosd
			stosw
			add edi,(Mbd/2)+264
			mov ecx,3
			rep stosd
			stosw

			pop  edx
			pop  ecx

            pop  ebx
            pop  eax
            pop  edi
            pop  esi
endproc



proc DrawLocalF

arg         %$LocBuf
arg         %$x
arg         %$y
arg         %$color
            push esi
            push edi
            push eax
            push ebx
            mov  esi,[ebp + %$LocBuf]
            mov  edi,[ebp + %$x]
            mov  eax,[ebp + %$y]
            mov  ebx,[ebp + %$color]

			push ecx
			push edx
			mov  edx,(280+(Mbd/2))
			mul  edx
			add  edi,eax
      mov  bh,bl
      mov  ax,bx
      mov  cl,16
      shl  eax,cl
      mov  ax,bx
			add  edi,esi

			add edi,13
			stosw
			add edi,(Mbd/2)+277
			stosd
			add edi,(Mbd/2)+275
			stosd
			stosw
			add edi,(Mbd/2)+273
			mov ecx,2
			rep stosd
			add edi,(Mbd/2)+271
			mov ecx,2
			rep stosd
			stosw
			add edi,(Mbd/2)+269
			mov ecx,3
			rep stosd
			add edi,(Mbd/2)+267
			mov ecx,3
			rep stosd
			stosw
			add edi,(Mbd/2)+265
			mov ecx,4
			rep stosd
			add edi,(Mbd/2)+263
			mov ecx,4
			rep stosd
			stosw
			add edi,(Mbd/2)+261
			mov ecx,5
			rep stosd
			add edi,(Mbd/2)+259
			mov ecx,5
			rep stosd
			stosw
			add edi,(Mbd/2)+257
			mov ecx,6
			rep stosd
			add edi,(Mbd/2)+255
			mov ecx,6
			rep stosd
			stosw
			add edi,(Mbd/2)+253
			mov ecx,7
			rep stosd
			add edi,(Mbd/2)+253
			mov ecx,6
			rep stosd
			stosw
			add edi,(Mbd/2)+256
			mov ecx,5
			rep stosd
			stosw
			add edi,(Mbd/2)+260
			mov ecx,4
			rep stosd
			stosw
			add edi,(Mbd/2)+264
			mov ecx,3
			rep stosd
			stosw
			add edi,(Mbd/2)+268
			mov ecx,2
			rep stosd
			stosw
			add edi,(Mbd/2)+272
			stosd
			stosw
			add edi,(Mbd/2)+276
			stosw

			pop  edx
			pop  ecx

            pop  ebx
            pop  eax
            pop  edi
            pop  esi
endproc



proc DrawLocalG

arg         %$LocBuf
arg         %$x
arg         %$y
arg         %$color
            push esi
            push edi
            push eax
            push ebx
            mov  esi,[ebp + %$LocBuf]
            mov  edi,[ebp + %$x]
            mov  eax,[ebp + %$y]
            mov  ebx,[ebp + %$color]

			push ecx
			push edx
			mov  edx,(280+(Mbd/2))
			mul  edx
			add  edi,eax
      mov  bh,bl
      mov  ax,bx
      mov  cl,16
      shl  eax,cl
      mov  ax,bx
			add  edi,esi

			add edi,1+7*(280+(Mbd/2))
			mov ecx,3
			rep stosd
			stosw
			add edi,(Mbd/2)+267
			mov ecx,3
			rep stosd
			stosw
			stosb
			add edi,(Mbd/2)+266
			mov ecx,4
			rep stosd
			add edi,(Mbd/2)+265
			mov ecx,4
			rep stosd
			stosb
			add edi,(Mbd/2)+264
			mov ecx,4
			rep stosd
			stosw
			add edi,(Mbd/2)+263
			mov ecx,4
			rep stosd
			stosw
			stosb
			add edi,(Mbd/2)+262
			mov ecx,5
			rep stosd
			add edi,(Mbd/2)+261
			mov ecx,4
			rep stosd
			stosw
			stosb
			add edi,(Mbd/2)+262
			mov ecx,4
			rep stosd
			add edi,(Mbd/2)+265
			mov ecx,3
			rep stosd
			stosb
			add edi,(Mbd/2)+268
			mov ecx,2
			rep stosd
			stosw
			add edi,(Mbd/2)+271
			stosd
			stosw
			stosb
			add edi,(Mbd/2)+274
			stosd
			add edi,(Mbd/2)+277
			stosb

			pop  edx
			pop  ecx

            pop  ebx
            pop  eax
            pop  edi
            pop  esi
endproc



proc DrawLocalH

arg         %$LocBuf
arg         %$x
arg         %$y
arg         %$color
            push esi
            push edi
            push eax
            push ebx
            mov  esi,[ebp + %$LocBuf]
            mov  edi,[ebp + %$x]
            mov  eax,[ebp + %$y]
            mov  ebx,[ebp + %$color]

			push ecx
			push edx
			mov  edx,(280+(Mbd/2))
			mul  edx
			add  edi,eax
      mov  bh,bl
      mov  ax,bx
      mov  cl,16
      shl  eax,cl
      mov  ax,bx
			add  edi,esi

			add edi,13+7*(280+(Mbd/2))
			stosw
			add edi,(Mbd/2)+276
			stosd
			stosw
			add edi,(Mbd/2)+272
			mov ecx,2
			rep stosd
			stosw
			add edi,(Mbd/2)+268
			mov ecx,3
			rep stosd
			stosw
			add edi,(Mbd/2)+264
			mov ecx,4
			rep stosd
			stosw
			add edi,(Mbd/2)+260
			mov ecx,5
			rep stosd
			stosw
			add edi,(Mbd/2)+256
			mov ecx,6
			rep stosd
			stosw

			pop  edx
			pop  ecx

            pop  ebx
            pop  eax
            pop  edi
            pop  esi
endproc



proc DrawLocalI

arg         %$LocBuf
arg         %$x
arg         %$y
arg         %$color
            push esi
            push edi
            push eax
            push ebx
            mov  esi,[ebp + %$LocBuf]
            mov  edi,[ebp + %$x]
            mov  eax,[ebp + %$y]
            mov  ebx,[ebp + %$color]

			push ecx
			push edx
			mov  edx,(280+(Mbd/2))
			mul  edx
			add  edi,eax
      mov  bh,bl
      mov  ax,bx
      mov  cl,16
      shl  eax,cl
      mov  ax,bx
			add  edi,esi

			add edi,13+7*(280+(Mbd/2))
			mov ecx,3
			rep stosd
			stosw
			add edi,(Mbd/2)+264
			mov ecx,3
			rep stosd
			stosw
			stosb
			add edi,(Mbd/2)+263
			mov ecx,4
			rep stosd
			add edi,(Mbd/2)+262
			mov ecx,4
			rep stosd
			stosb
			add edi,(Mbd/2)+261
			mov ecx,4
			rep stosd
			stosw
			add edi,(Mbd/2)+260
			mov ecx,4
			rep stosd
			stosw
			stosb
			add edi,(Mbd/2)+259
			mov ecx,5
			rep stosd
			add edi,(Mbd/2)+260
			mov ecx,4
			rep stosd
			stosw
			stosb
			add edi,(Mbd/2)+263
			mov ecx,4
			rep stosd
			add edi,(Mbd/2)+266
			mov ecx,3
			rep stosd
			stosb
			add edi,(Mbd/2)+269
			mov ecx,2
			rep stosd
			stosw
			add edi,(Mbd/2)+272
			stosd
			stosw
			stosb
			add edi,(Mbd/2)+275
			stosd
			add edi,(Mbd/2)+278
			stosb

			pop  edx
			pop  ecx

            pop  ebx
            pop  eax
            pop  edi
            pop  esi
endproc



proc DrawLocalJ

arg         %$LocBuf
arg         %$x
arg         %$y
arg         %$color
            push esi
            push edi
            push eax
            push ebx
            mov  esi,[ebp + %$LocBuf]
            mov  edi,[ebp + %$x]
            mov  eax,[ebp + %$y]
            mov  ebx,[ebp + %$color]

			push ecx
			push edx
			mov  edx,(280+(Mbd/2))
			mul  edx
			add  edi,eax
      mov  bh,bl
      mov  ax,bx
      mov  cl,16
      shl  eax,cl
      mov  ax,bx
			add  edi,esi

			add edi,13
			stosw
			add edi,(Mbd/2)+276
			stosd
			stosw
			add edi,(Mbd/2)+272
			mov ecx,2
			rep stosd
			stosw
			add edi,(Mbd/2)+268
			mov ecx,3
			rep stosd
			stosw
			add edi,(Mbd/2)+264
			mov ecx,4
			rep stosd
			stosw
			add edi,(Mbd/2)+260
			mov ecx,5
			rep stosd
			stosw
			add edi,(Mbd/2)+256
			mov ecx,6
			rep stosd
			stosw
			add edi,(Mbd/2)+254
			mov ecx,6
			rep stosd
			stosw
			add edi,(Mbd/2)+255
			mov ecx,6
			rep stosd
			add edi,(Mbd/2)+257
			mov ecx,5
			rep stosd
			stosw
			add edi,(Mbd/2)+259
			mov ecx,5
			rep stosd
			add edi,(Mbd/2)+261
			mov ecx,4
			rep stosd
			stosw
			add edi,(Mbd/2)+263
			mov ecx,4
			rep stosd
			add edi,(Mbd/2)+265
			mov ecx,3
			rep stosd
			stosw
			add edi,(Mbd/2)+267
			mov ecx,3
			rep stosd
			add edi,(Mbd/2)+269
			mov ecx,2
			rep stosd
			stosw
			add edi,(Mbd/2)+271
			mov ecx,2
			rep stosd
			add edi,(Mbd/2)+273
			stosd
			stosw
			add edi,(Mbd/2)+275
			stosd
			add edi,(Mbd/2)+277
			stosw

			pop  edx
			pop  ecx

            pop  ebx
            pop  eax
            pop  edi
            pop  esi
endproc



proc DrawLocalK

arg         %$LocBuf
arg         %$x
arg         %$y
arg         %$color
            push esi
            push edi
            push eax
            push ebx
            mov  esi,[ebp + %$LocBuf]
            mov  edi,[ebp + %$x]
            mov  eax,[ebp + %$y]
            mov  ebx,[ebp + %$color]

			push ecx
			push edx
			mov  edx,(280+(Mbd/2))
			mul  edx
			add  edi,eax
      mov  bh,bl
      mov  ax,bx
      mov  cl,16
      shl  eax,cl
      mov  ax,bx
			add  edi,esi

			add edi,13
			stosw
			add edi,(Mbd/2)+276
			stosd
			stosb
			add edi,(Mbd/2)+273
			mov ecx,2
			rep stosd
			add edi,(Mbd/2)+270
			mov ecx,2
			rep stosd
			stosw
			stosb
			add edi,(Mbd/2)+267
			mov ecx,3
			rep stosd
			stosw
			add edi,(Mbd/2)+264
			mov ecx,4
			rep stosd
			stosb
			add edi,(Mbd/2)+261
			mov ecx,5
			rep stosd
			add edi,(Mbd/2)+260
			mov ecx,5
			rep stosd
			stosb
			add edi,(Mbd/2)+261
			mov ecx,5
			rep stosd
			add edi,(Mbd/2)+262
			mov ecx,4
			rep stosd
			stosw
			stosb
			add edi,(Mbd/2)+263
			mov ecx,4
			rep stosd
			stosw
			add edi,(Mbd/2)+264
			mov ecx,4
			rep stosd
			stosb
			add edi,(Mbd/2)+265
			mov ecx,4
			rep stosd
			add edi,(Mbd/2)+266
			mov ecx,3
			rep stosd
			stosw
			stosb

			pop  edx
			pop  ecx

            pop  ebx
            pop  eax
            pop  edi
            pop  esi
endproc



proc DrawLocalL

arg         %$LocBuf
arg         %$x
arg         %$y
arg         %$color
            push esi
            push edi
            push eax
            push ebx
            mov  esi,[ebp + %$LocBuf]
            mov  edi,[ebp + %$x]
            mov  eax,[ebp + %$y]
            mov  ebx,[ebp + %$color]

			push ecx
			push edx
			mov  edx,(280+(Mbd/2))
			mul  edx
			add  edi,eax
      mov  bh,bl
      mov  ax,bx
      mov  cl,16
      shl  eax,cl
      mov  ax,bx
			add  edi,esi

			add edi,1+7*(280+(Mbd/2))
			mov ecx,6
			rep stosd
			stosw
			add edi,(Mbd/2)+256
			mov ecx,5
			rep stosd
			stosw
			add edi,(Mbd/2)+260
			mov ecx,4
			rep stosd
			stosw
			add edi,(Mbd/2)+264
			mov ecx,3
			rep stosd
			stosw
			add edi,(Mbd/2)+268
			mov ecx,2
			rep stosd
			stosw
			add edi,(Mbd/2)+272
			stosd
			stosw
			add edi,(Mbd/2)+276
			stosw

			pop  edx
			pop  ecx

            pop  ebx
            pop  eax
            pop  edi
            pop  esi
endproc



proc DrawLocalM

arg         %$LocBuf
arg         %$x
arg         %$y
arg         %$color
            push esi
            push edi
            push eax
            push ebx
            mov  esi,[ebp + %$LocBuf]
            mov  edi,[ebp + %$x]
            mov  eax,[ebp + %$y]
            mov  ebx,[ebp + %$color]

			push ecx
			push edx
			mov  edx,280+(Mbd/2)
			mul  edx
			add  edi,eax
      mov  bh,bl
      mov  ax,bx
      mov  cl,16
      shl  eax,cl
      mov  ax,bx
			add  edi,esi

			add edi,13
			stosw
			add edi,(Mbd/2)+277
			stosd
			stosb
			add edi,(Mbd/2)+274
			mov ecx,2
			rep stosd
			add edi,(Mbd/2)+271
			mov ecx,2
			rep stosd
			stosw
			stosb
			add edi,(Mbd/2)+268
			mov ecx,3
			rep stosd
			stosw
			add edi,(Mbd/2)+265
			mov ecx,4
			rep stosd
			stosb
			add edi,(Mbd/2)+262
			mov ecx,5
			rep stosd
			add edi,(Mbd/2)+259
			mov ecx,5
			rep stosd
			stosb
			add edi,(Mbd/2)+258
			mov ecx,5
			rep stosd
			add edi,(Mbd/2)+259
			mov ecx,4
			rep stosd
			stosw
			stosb
			add edi,(Mbd/2)+260
			mov ecx,4
			rep stosd
			stosw
			add edi,(Mbd/2)+261
			mov ecx,4
			rep stosd
			stosb
			add edi,(Mbd/2)+262
			mov ecx,4
			rep stosd
			add edi,(Mbd/2)+263
			mov ecx,3
			rep stosd
			stosw
			stosb

			pop  edx
			pop  ecx

            pop  ebx
            pop  eax
            pop  edi
            pop  esi
endproc








; extern void PutSpritePart640(void *screen, int sizes, void *data, int adding);
; #pragma aux PutSpritePart640 parm   [EDI] [ECX] [ESI] [EBX] 
;                              modify [EAX EBX ECX EDX EDI ESI]

proc PutSpritePart

arg         %$screen
arg         %$sizes
arg         %$data
arg         %$adding
            pushad
            mov  edi,[ebp + %$screen]
            mov  ecx,[ebp + %$sizes]
            mov  esi,[ebp + %$data]
            mov  ebx,[ebp + %$adding]

    		xor  eax,eax
			mov  al,cl
			mov  edx,(560+Mbd)
			sub  edx,eax
			push cx
		.psp_rep:
			lodsb
			cmp  al,0
			jz   .psp_cont
			cmp  al,63 ;shadow
			jz   .psp_shadow
			mov  byte [edi],al
		.psp_cont:
			inc  edi
			dec  cl
			jnz  .psp_rep
			mov  cl,byte [esp]
			add  esi,ebx
			add  edi,edx
			dec  ch
			jnz  .psp_rep
			pop  cx
			jmp  .endofproc
		.psp_shadow:
		  mov  al,byte [edi]
			mov  al,byte [DarkingTable + eax]
			mov  byte [edi],al
			jmp  .psp_cont
        .endofproc:
            popad
endproc


