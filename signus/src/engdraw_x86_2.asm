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




; Parametr kreslene bitmapy je vzdy ulozen do ESI,
; Y-souradnice do EAX a X do EDI:


proc DrawUpTerrA

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
add edi,eax
add edi,dword [MapBuf]

add edi,26
lodsb
cmp al,0
jz  .label_c0
mov [edi],al
.label_c0:
inc edi
lodsb
cmp al,0
jz  .label_c1
mov [edi],al
.label_c1:
inc edi
lodsb
cmp al,0
jz  .label_c2
mov [edi],al
.label_c2:
inc edi
lodsb
cmp al,0
jz  .label_c3
mov [edi],al
.label_c3:
inc edi
add edi,Mbd+554
mov ecx,8
.label_l0:
lodsb
cmp al,0
jz  .label_c4
mov [edi],al
.label_c4:
inc edi
loop .label_l0
add edi,Mbd+550
mov ecx,12
.label_l1:
lodsb
cmp al,0
jz  .label_c5
mov [edi],al
.label_c5:
inc edi
loop .label_l1
add edi,Mbd+546
mov ecx,16
.label_l2:
lodsb
cmp al,0
jz  .label_c6
mov [edi],al
.label_c6:
inc edi
loop .label_l2
add edi,Mbd+542
mov ecx,20
.label_l3:
lodsb
cmp al,0
jz  .label_c7
mov [edi],al
.label_c7:
inc edi
loop .label_l3
add edi,Mbd+538
mov ecx,24
.label_l4:
lodsb
cmp al,0
jz  .label_c8
mov [edi],al
.label_c8:
inc edi
loop .label_l4
add edi,Mbd+534
mov ecx,28
.label_l5:
lodsb
cmp al,0
jz  .label_c9
mov [edi],al
.label_c9:
inc edi
loop .label_l5
add edi,Mbd+530
mov ecx,32
.label_l6:
lodsb
cmp al,0
jz  .label_c10
mov [edi],al
.label_c10:
inc edi
loop .label_l6
add edi,Mbd+526
mov ecx,36
.label_l7:
lodsb
cmp al,0
jz  .label_c11
mov [edi],al
.label_c11:
inc edi
loop .label_l7
add edi,Mbd+522
mov ecx,40
.label_l8:
lodsb
cmp al,0
jz  .label_c12
mov [edi],al
.label_c12:
inc edi
loop .label_l8
add edi,Mbd+518
mov ecx,44
.label_l9:
lodsb
cmp al,0
jz  .label_c13
mov [edi],al
.label_c13:
inc edi
loop .label_l9
add edi,Mbd+514
mov ecx,48
.label_l10:
lodsb
cmp al,0
jz  .label_c14
mov [edi],al
.label_c14:
inc edi
loop .label_l10
add edi,Mbd+510
mov ecx,52
.label_l11:
lodsb
cmp al,0
jz  .label_c15
mov [edi],al
.label_c15:
inc edi
loop .label_l11
add edi,Mbd+506
mov ecx,56
.label_l12:
lodsb
cmp al,0
jz  .label_c16
mov [edi],al
.label_c16:
inc edi
loop .label_l12
add edi,Mbd+506
mov ecx,52
.label_l13:
lodsb
cmp al,0
jz  .label_c17
mov [edi],al
.label_c17:
inc edi
loop .label_l13
add edi,Mbd+510
mov ecx,48
.label_l14:
lodsb
cmp al,0
jz  .label_c18
mov [edi],al
.label_c18:
inc edi
loop .label_l14
add edi,Mbd+514
mov ecx,44
.label_l15:
lodsb
cmp al,0
jz  .label_c19
mov [edi],al
.label_c19:
inc edi
loop .label_l15
add edi,Mbd+518
mov ecx,40
.label_l16:
lodsb
cmp al,0
jz  .label_c20
mov [edi],al
.label_c20:
inc edi
loop .label_l16
add edi,Mbd+522
mov ecx,36
.label_l17:
lodsb
cmp al,0
jz  .label_c21
mov [edi],al
.label_c21:
inc edi
loop .label_l17
add edi,Mbd+526
mov ecx,32
.label_l18:
lodsb
cmp al,0
jz  .label_c22
mov [edi],al
.label_c22:
inc edi
loop .label_l18
add edi,Mbd+530
mov ecx,28
.label_l19:
lodsb
cmp al,0
jz  .label_c23
mov [edi],al
.label_c23:
inc edi
loop .label_l19
add edi,Mbd+534
mov ecx,24
.label_l20:
lodsb
cmp al,0
jz  .label_c24
mov [edi],al
.label_c24:
inc edi
loop .label_l20
add edi,Mbd+538
mov ecx,20
.label_l21:
lodsb
cmp al,0
jz  .label_c25
mov [edi],al
.label_c25:
inc edi
loop .label_l21
add edi,Mbd+542
mov ecx,16
.label_l22:
lodsb
cmp al,0
jz  .label_c26
mov [edi],al
.label_c26:
inc edi
loop .label_l22
add edi,Mbd+546
mov ecx,12
.label_l23:
lodsb
cmp al,0
jz  .label_c27
mov [edi],al
.label_c27:
inc edi
loop .label_l23
add edi,Mbd+550
mov ecx,8
.label_l24:
lodsb
cmp al,0
jz  .label_c28
mov [edi],al
.label_c28:
inc edi
loop .label_l24
add edi,Mbd+554
lodsb
cmp al,0
jz  .label_c29
mov [edi],al
.label_c29:
inc edi
lodsb
cmp al,0
jz  .label_c30
mov [edi],al
.label_c30:
inc edi
lodsb
cmp al,0
jz  .label_c31
mov [edi],al
.label_c31:
inc edi
lodsb
cmp al,0
jz  .label_c32
mov [edi],al
.label_c32:
inc edi

pop  edx
pop  ecx

            pop  esi
            pop  eax
            pop  edi
endproc






proc DrawUpTerrB

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
add edi,eax
add edi,dword [MapBuf]

add edi,26
lodsb
cmp al,0
jz  .label_c33
mov [edi],al
.label_c33:
inc edi
lodsb
cmp al,0
jz  .label_c34
mov [edi],al
.label_c34:
inc edi
lodsb
cmp al,0
jz  .label_c35
mov [edi],al
.label_c35:
inc edi
lodsb
cmp al,0
jz  .label_c36
mov [edi],al
.label_c36:
inc edi
add edi,Mbd+555
lodsb
cmp al,0
jz  .label_c37
mov [edi],al
.label_c37:
inc edi
lodsb
cmp al,0
jz  .label_c38
mov [edi],al
.label_c38:
inc edi
lodsb
cmp al,0
jz  .label_c39
mov [edi],al
.label_c39:
inc edi
lodsb
cmp al,0
jz  .label_c40
mov [edi],al
.label_c40:
inc edi
lodsb
cmp al,0
jz  .label_c41
mov [edi],al
.label_c41:
inc edi
lodsb
cmp al,0
jz  .label_c42
mov [edi],al
.label_c42:
inc edi
lodsb
cmp al,0
jz  .label_c43
mov [edi],al
.label_c43:
inc edi
add edi,Mbd+552
mov ecx,8
.label_l25:
lodsb
cmp al,0
jz  .label_c44
mov [edi],al
.label_c44:
inc edi
loop .label_l25
lodsb
cmp al,0
jz  .label_c45
mov [edi],al
.label_c45:
inc edi
lodsb
cmp al,0
jz  .label_c46
mov [edi],al
.label_c46:
inc edi
add edi,Mbd+549
mov ecx,12
.label_l26:
lodsb
cmp al,0
jz  .label_c47
mov [edi],al
.label_c47:
inc edi
loop .label_l26
lodsb
cmp al,0
jz  .label_c48
mov [edi],al
.label_c48:
inc edi
add edi,Mbd+546
mov ecx,16
.label_l27:
lodsb
cmp al,0
jz  .label_c49
mov [edi],al
.label_c49:
inc edi
loop .label_l27
add edi,Mbd+543
mov ecx,16
.label_l28:
lodsb
cmp al,0
jz  .label_c50
mov [edi],al
.label_c50:
inc edi
loop .label_l28
lodsb
cmp al,0
jz  .label_c51
mov [edi],al
.label_c51:
inc edi
lodsb
cmp al,0
jz  .label_c52
mov [edi],al
.label_c52:
inc edi
lodsb
cmp al,0
jz  .label_c53
mov [edi],al
.label_c53:
inc edi
add edi,Mbd+540
mov ecx,20
.label_l29:
lodsb
cmp al,0
jz  .label_c54
mov [edi],al
.label_c54:
inc edi
loop .label_l29
lodsb
cmp al,0
jz  .label_c55
mov [edi],al
.label_c55:
inc edi
lodsb
cmp al,0
jz  .label_c56
mov [edi],al
.label_c56:
inc edi
add edi,Mbd+537
mov ecx,24
.label_l30:
lodsb
cmp al,0
jz  .label_c57
mov [edi],al
.label_c57:
inc edi
loop .label_l30
lodsb
cmp al,0
jz  .label_c58
mov [edi],al
.label_c58:
inc edi
add edi,Mbd+534
mov ecx,28
.label_l31:
lodsb
cmp al,0
jz  .label_c59
mov [edi],al
.label_c59:
inc edi
loop .label_l31
add edi,Mbd+531
mov ecx,28
.label_l32:
lodsb
cmp al,0
jz  .label_c60
mov [edi],al
.label_c60:
inc edi
loop .label_l32
lodsb
cmp al,0
jz  .label_c61
mov [edi],al
.label_c61:
inc edi
lodsb
cmp al,0
jz  .label_c62
mov [edi],al
.label_c62:
inc edi
lodsb
cmp al,0
jz  .label_c63
mov [edi],al
.label_c63:
inc edi
add edi,Mbd+528
mov ecx,32
.label_l33:
lodsb
cmp al,0
jz  .label_c64
mov [edi],al
.label_c64:
inc edi
loop .label_l33
lodsb
cmp al,0
jz  .label_c65
mov [edi],al
.label_c65:
inc edi
lodsb
cmp al,0
jz  .label_c66
mov [edi],al
.label_c66:
inc edi
add edi,Mbd+525
mov ecx,36
.label_l34:
lodsb
cmp al,0
jz  .label_c67
mov [edi],al
.label_c67:
inc edi
loop .label_l34
lodsb
cmp al,0
jz  .label_c68
mov [edi],al
.label_c68:
inc edi
add edi,Mbd+522
mov ecx,40
.label_l35:
lodsb
cmp al,0
jz  .label_c69
mov [edi],al
.label_c69:
inc edi
loop .label_l35
add edi,Mbd+519
mov ecx,40
.label_l36:
lodsb
cmp al,0
jz  .label_c70
mov [edi],al
.label_c70:
inc edi
loop .label_l36
lodsb
cmp al,0
jz  .label_c71
mov [edi],al
.label_c71:
inc edi
lodsb
cmp al,0
jz  .label_c72
mov [edi],al
.label_c72:
inc edi
lodsb
cmp al,0
jz  .label_c73
mov [edi],al
.label_c73:
inc edi
add edi,Mbd+516
mov ecx,40
.label_l37:
lodsb
cmp al,0
jz  .label_c74
mov [edi],al
.label_c74:
inc edi
loop .label_l37
lodsb
cmp al,0
jz  .label_c75
mov [edi],al
.label_c75:
inc edi
lodsb
cmp al,0
jz  .label_c76
mov [edi],al
.label_c76:
inc edi
add edi,Mbd+517
mov ecx,40
.label_l38:
lodsb
cmp al,0
jz  .label_c77
mov [edi],al
.label_c77:
inc edi
loop .label_l38
lodsb
cmp al,0
jz  .label_c78
mov [edi],al
.label_c78:
inc edi
lodsb
cmp al,0
jz  .label_c79
mov [edi],al
.label_c79:
inc edi
add edi,Mbd+517
mov ecx,40
.label_l39:
lodsb
cmp al,0
jz  .label_c80
mov [edi],al
.label_c80:
inc edi
loop .label_l39
lodsb
cmp al,0
jz  .label_c81
mov [edi],al
.label_c81:
inc edi
lodsb
cmp al,0
jz  .label_c82
mov [edi],al
.label_c82:
inc edi
add edi,Mbd+517
mov ecx,40
.label_l40:
lodsb
cmp al,0
jz  .label_c83
mov [edi],al
.label_c83:
inc edi
loop .label_l40
lodsb
cmp al,0
jz  .label_c84
mov [edi],al
.label_c84:
inc edi
lodsb
cmp al,0
jz  .label_c85
mov [edi],al
.label_c85:
inc edi
add edi,Mbd+517
mov ecx,40
.label_l41:
lodsb
cmp al,0
jz  .label_c86
mov [edi],al
.label_c86:
inc edi
loop .label_l41
lodsb
cmp al,0
jz  .label_c87
mov [edi],al
.label_c87:
inc edi
lodsb
cmp al,0
jz  .label_c88
mov [edi],al
.label_c88:
inc edi
add edi,Mbd+517
mov ecx,40
.label_l42:
lodsb
cmp al,0
jz  .label_c89
mov [edi],al
.label_c89:
inc edi
loop .label_l42
lodsb
cmp al,0
jz  .label_c90
mov [edi],al
.label_c90:
inc edi
lodsb
cmp al,0
jz  .label_c91
mov [edi],al
.label_c91:
inc edi
add edi,Mbd+517
mov ecx,40
.label_l43:
lodsb
cmp al,0
jz  .label_c92
mov [edi],al
.label_c92:
inc edi
loop .label_l43
lodsb
cmp al,0
jz  .label_c93
mov [edi],al
.label_c93:
inc edi
lodsb
cmp al,0
jz  .label_c94
mov [edi],al
.label_c94:
inc edi
add edi,Mbd+517
mov ecx,40
.label_l44:
lodsb
cmp al,0
jz  .label_c95
mov [edi],al
.label_c95:
inc edi
loop .label_l44
lodsb
cmp al,0
jz  .label_c96
mov [edi],al
.label_c96:
inc edi
lodsb
cmp al,0
jz  .label_c97
mov [edi],al
.label_c97:
inc edi
add edi,Mbd+517
mov ecx,40
.label_l45:
lodsb
cmp al,0
jz  .label_c98
mov [edi],al
.label_c98:
inc edi
loop .label_l45
lodsb
cmp al,0
jz  .label_c99
mov [edi],al
.label_c99:
inc edi
lodsb
cmp al,0
jz  .label_c100
mov [edi],al
.label_c100:
inc edi
add edi,Mbd+517
mov ecx,40
.label_l46:
lodsb
cmp al,0
jz  .label_c101
mov [edi],al
.label_c101:
inc edi
loop .label_l46
lodsb
cmp al,0
jz  .label_c102
mov [edi],al
.label_c102:
inc edi
lodsb
cmp al,0
jz  .label_c103
mov [edi],al
.label_c103:
inc edi
add edi,Mbd+517
mov ecx,40
.label_l47:
lodsb
cmp al,0
jz  .label_c104
mov [edi],al
.label_c104:
inc edi
loop .label_l47
lodsb
cmp al,0
jz  .label_c105
mov [edi],al
.label_c105:
inc edi
lodsb
cmp al,0
jz  .label_c106
mov [edi],al
.label_c106:
inc edi
add edi,Mbd+517
mov ecx,40
.label_l48:
lodsb
cmp al,0
jz  .label_c107
mov [edi],al
.label_c107:
inc edi
loop .label_l48
lodsb
cmp al,0
jz  .label_c108
mov [edi],al
.label_c108:
inc edi
lodsb
cmp al,0
jz  .label_c109
mov [edi],al
.label_c109:
inc edi
add edi,Mbd+517
mov ecx,40
.label_l49:
lodsb
cmp al,0
jz  .label_c110
mov [edi],al
.label_c110:
inc edi
loop .label_l49
lodsb
cmp al,0
jz  .label_c111
mov [edi],al
.label_c111:
inc edi
lodsb
cmp al,0
jz  .label_c112
mov [edi],al
.label_c112:
inc edi
add edi,Mbd+518
mov ecx,40
.label_l50:
lodsb
cmp al,0
jz  .label_c113
mov [edi],al
.label_c113:
inc edi
loop .label_l50
lodsb
cmp al,0
jz  .label_c114
mov [edi],al
.label_c114:
inc edi
add edi,Mbd+521
mov ecx,36
.label_l51:
lodsb
cmp al,0
jz  .label_c115
mov [edi],al
.label_c115:
inc edi
loop .label_l51
lodsb
cmp al,0
jz  .label_c116
mov [edi],al
.label_c116:
inc edi
lodsb
cmp al,0
jz  .label_c117
mov [edi],al
.label_c117:
inc edi
add edi,Mbd+524
mov ecx,32
.label_l52:
lodsb
cmp al,0
jz  .label_c118
mov [edi],al
.label_c118:
inc edi
loop .label_l52
lodsb
cmp al,0
jz  .label_c119
mov [edi],al
.label_c119:
inc edi
lodsb
cmp al,0
jz  .label_c120
mov [edi],al
.label_c120:
inc edi
lodsb
cmp al,0
jz  .label_c121
mov [edi],al
.label_c121:
inc edi
add edi,Mbd+527
mov ecx,32
.label_l53:
lodsb
cmp al,0
jz  .label_c122
mov [edi],al
.label_c122:
inc edi
loop .label_l53
add edi,Mbd+530
mov ecx,28
.label_l54:
lodsb
cmp al,0
jz  .label_c123
mov [edi],al
.label_c123:
inc edi
loop .label_l54
lodsb
cmp al,0
jz  .label_c124
mov [edi],al
.label_c124:
inc edi
add edi,Mbd+533
mov ecx,24
.label_l55:
lodsb
cmp al,0
jz  .label_c125
mov [edi],al
.label_c125:
inc edi
loop .label_l55
lodsb
cmp al,0
jz  .label_c126
mov [edi],al
.label_c126:
inc edi
lodsb
cmp al,0
jz  .label_c127
mov [edi],al
.label_c127:
inc edi
add edi,Mbd+536
mov ecx,20
.label_l56:
lodsb
cmp al,0
jz  .label_c128
mov [edi],al
.label_c128:
inc edi
loop .label_l56
lodsb
cmp al,0
jz  .label_c129
mov [edi],al
.label_c129:
inc edi
lodsb
cmp al,0
jz  .label_c130
mov [edi],al
.label_c130:
inc edi
lodsb
cmp al,0
jz  .label_c131
mov [edi],al
.label_c131:
inc edi
add edi,Mbd+539
mov ecx,20
.label_l57:
lodsb
cmp al,0
jz  .label_c132
mov [edi],al
.label_c132:
inc edi
loop .label_l57
add edi,Mbd+542
mov ecx,16
.label_l58:
lodsb
cmp al,0
jz  .label_c133
mov [edi],al
.label_c133:
inc edi
loop .label_l58
lodsb
cmp al,0
jz  .label_c134
mov [edi],al
.label_c134:
inc edi
add edi,Mbd+545
mov ecx,12
.label_l59:
lodsb
cmp al,0
jz  .label_c135
mov [edi],al
.label_c135:
inc edi
loop .label_l59
lodsb
cmp al,0
jz  .label_c136
mov [edi],al
.label_c136:
inc edi
lodsb
cmp al,0
jz  .label_c137
mov [edi],al
.label_c137:
inc edi
add edi,Mbd+548
mov ecx,8
.label_l60:
lodsb
cmp al,0
jz  .label_c138
mov [edi],al
.label_c138:
inc edi
loop .label_l60
lodsb
cmp al,0
jz  .label_c139
mov [edi],al
.label_c139:
inc edi
lodsb
cmp al,0
jz  .label_c140
mov [edi],al
.label_c140:
inc edi
lodsb
cmp al,0
jz  .label_c141
mov [edi],al
.label_c141:
inc edi
add edi,Mbd+551
mov ecx,8
.label_l61:
lodsb
cmp al,0
jz  .label_c142
mov [edi],al
.label_c142:
inc edi
loop .label_l61
add edi,Mbd+554
lodsb
cmp al,0
jz  .label_c143
mov [edi],al
.label_c143:
inc edi
lodsb
cmp al,0
jz  .label_c144
mov [edi],al
.label_c144:
inc edi
lodsb
cmp al,0
jz  .label_c145
mov [edi],al
.label_c145:
inc edi
lodsb
cmp al,0
jz  .label_c146
mov [edi],al
.label_c146:
inc edi
lodsb
cmp al,0
jz  .label_c147
mov [edi],al
.label_c147:
inc edi
add edi,Mbd+557
lodsb
cmp al,0
jz  .label_c148
mov [edi],al
.label_c148:
inc edi
lodsb
cmp al,0
jz  .label_c149
mov [edi],al
.label_c149:
inc edi

pop  edx
pop  ecx

            pop  esi
            pop  eax
            pop  edi
endproc






proc DrawUpTerrC

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
add edi,eax
add edi,dword [MapBuf]


add edi,26
lodsb
cmp al,0
jz  .label_c150
mov [edi],al
.label_c150:
inc edi
lodsb
cmp al,0
jz  .label_c151
mov [edi],al
.label_c151:
inc edi
lodsb
cmp al,0
jz  .label_c152
mov [edi],al
.label_c152:
inc edi
lodsb
cmp al,0
jz  .label_c153
mov [edi],al
.label_c153:
inc edi
add edi,Mbd+554
lodsb
cmp al,0
jz  .label_c154
mov [edi],al
.label_c154:
inc edi
lodsb
cmp al,0
jz  .label_c155
mov [edi],al
.label_c155:
inc edi
lodsb
cmp al,0
jz  .label_c156
mov [edi],al
.label_c156:
inc edi
lodsb
cmp al,0
jz  .label_c157
mov [edi],al
.label_c157:
inc edi
lodsb
cmp al,0
jz  .label_c158
mov [edi],al
.label_c158:
inc edi
lodsb
cmp al,0
jz  .label_c159
mov [edi],al
.label_c159:
inc edi
lodsb
cmp al,0
jz  .label_c160
mov [edi],al
.label_c160:
inc edi
add edi,Mbd+551
mov ecx,8
.label_l62:
lodsb
cmp al,0
jz  .label_c161
mov [edi],al
.label_c161:
inc edi
loop .label_l62
lodsb
cmp al,0
jz  .label_c162
mov [edi],al
.label_c162:
inc edi
lodsb
cmp al,0
jz  .label_c163
mov [edi],al
.label_c163:
inc edi
add edi,Mbd+548
mov ecx,12
.label_l63:
lodsb
cmp al,0
jz  .label_c164
mov [edi],al
.label_c164:
inc edi
loop .label_l63
lodsb
cmp al,0
jz  .label_c165
mov [edi],al
.label_c165:
inc edi
add edi,Mbd+545
mov ecx,16
.label_l64:
lodsb
cmp al,0
jz  .label_c166
mov [edi],al
.label_c166:
inc edi
loop .label_l64
add edi,Mbd+542
mov ecx,16
.label_l65:
lodsb
cmp al,0
jz  .label_c167
mov [edi],al
.label_c167:
inc edi
loop .label_l65
lodsb
cmp al,0
jz  .label_c168
mov [edi],al
.label_c168:
inc edi
lodsb
cmp al,0
jz  .label_c169
mov [edi],al
.label_c169:
inc edi
lodsb
cmp al,0
jz  .label_c170
mov [edi],al
.label_c170:
inc edi
add edi,Mbd+539
mov ecx,20
.label_l66:
lodsb
cmp al,0
jz  .label_c171
mov [edi],al
.label_c171:
inc edi
loop .label_l66
lodsb
cmp al,0
jz  .label_c172
mov [edi],al
.label_c172:
inc edi
lodsb
cmp al,0
jz  .label_c173
mov [edi],al
.label_c173:
inc edi
add edi,Mbd+536
mov ecx,24
.label_l67:
lodsb
cmp al,0
jz  .label_c174
mov [edi],al
.label_c174:
inc edi
loop .label_l67
lodsb
cmp al,0
jz  .label_c175
mov [edi],al
.label_c175:
inc edi
add edi,Mbd+533
mov ecx,28
.label_l68:
lodsb
cmp al,0
jz  .label_c176
mov [edi],al
.label_c176:
inc edi
loop .label_l68
add edi,Mbd+530
mov ecx,28
.label_l69:
lodsb
cmp al,0
jz  .label_c177
mov [edi],al
.label_c177:
inc edi
loop .label_l69
lodsb
cmp al,0
jz  .label_c178
mov [edi],al
.label_c178:
inc edi
lodsb
cmp al,0
jz  .label_c179
mov [edi],al
.label_c179:
inc edi
lodsb
cmp al,0
jz  .label_c180
mov [edi],al
.label_c180:
inc edi
add edi,Mbd+527
mov ecx,32
.label_l70:
lodsb
cmp al,0
jz  .label_c181
mov [edi],al
.label_c181:
inc edi
loop .label_l70
lodsb
cmp al,0
jz  .label_c182
mov [edi],al
.label_c182:
inc edi
lodsb
cmp al,0
jz  .label_c183
mov [edi],al
.label_c183:
inc edi
add edi,Mbd+524
mov ecx,36
.label_l71:
lodsb
cmp al,0
jz  .label_c184
mov [edi],al
.label_c184:
inc edi
loop .label_l71
lodsb
cmp al,0
jz  .label_c185
mov [edi],al
.label_c185:
inc edi
add edi,Mbd+521
mov ecx,40
.label_l72:
lodsb
cmp al,0
jz  .label_c186
mov [edi],al
.label_c186:
inc edi
loop .label_l72
add edi,Mbd+518
mov ecx,40
.label_l73:
lodsb
cmp al,0
jz  .label_c187
mov [edi],al
.label_c187:
inc edi
loop .label_l73
lodsb
cmp al,0
jz  .label_c188
mov [edi],al
.label_c188:
inc edi
lodsb
cmp al,0
jz  .label_c189
mov [edi],al
.label_c189:
inc edi
lodsb
cmp al,0
jz  .label_c190
mov [edi],al
.label_c190:
inc edi
add edi,Mbd+519
mov ecx,40
.label_l74:
lodsb
cmp al,0
jz  .label_c191
mov [edi],al
.label_c191:
inc edi
loop .label_l74
lodsb
cmp al,0
jz  .label_c192
mov [edi],al
.label_c192:
inc edi
lodsb
cmp al,0
jz  .label_c193
mov [edi],al
.label_c193:
inc edi
add edi,Mbd+519
mov ecx,40
.label_l75:
lodsb
cmp al,0
jz  .label_c194
mov [edi],al
.label_c194:
inc edi
loop .label_l75
lodsb
cmp al,0
jz  .label_c195
mov [edi],al
.label_c195:
inc edi
lodsb
cmp al,0
jz  .label_c196
mov [edi],al
.label_c196:
inc edi
add edi,Mbd+519
mov ecx,40
.label_l76:
lodsb
cmp al,0
jz  .label_c197
mov [edi],al
.label_c197:
inc edi
loop .label_l76
lodsb
cmp al,0
jz  .label_c198
mov [edi],al
.label_c198:
inc edi
lodsb
cmp al,0
jz  .label_c199
mov [edi],al
.label_c199:
inc edi
add edi,Mbd+519
mov ecx,40
.label_l77:
lodsb
cmp al,0
jz  .label_c200
mov [edi],al
.label_c200:
inc edi
loop .label_l77
lodsb
cmp al,0
jz  .label_c201
mov [edi],al
.label_c201:
inc edi
lodsb
cmp al,0
jz  .label_c202
mov [edi],al
.label_c202:
inc edi
add edi,Mbd+519
mov ecx,40
.label_l78:
lodsb
cmp al,0
jz  .label_c203
mov [edi],al
.label_c203:
inc edi
loop .label_l78
lodsb
cmp al,0
jz  .label_c204
mov [edi],al
.label_c204:
inc edi
lodsb
cmp al,0
jz  .label_c205
mov [edi],al
.label_c205:
inc edi
add edi,Mbd+519
mov ecx,40
.label_l79:
lodsb
cmp al,0
jz  .label_c206
mov [edi],al
.label_c206:
inc edi
loop .label_l79
lodsb
cmp al,0
jz  .label_c207
mov [edi],al
.label_c207:
inc edi
lodsb
cmp al,0
jz  .label_c208
mov [edi],al
.label_c208:
inc edi
add edi,Mbd+519
mov ecx,40
.label_l80:
lodsb
cmp al,0
jz  .label_c209
mov [edi],al
.label_c209:
inc edi
loop .label_l80
lodsb
cmp al,0
jz  .label_c210
mov [edi],al
.label_c210:
inc edi
lodsb
cmp al,0
jz  .label_c211
mov [edi],al
.label_c211:
inc edi
add edi,Mbd+519
mov ecx,40
.label_l81:
lodsb
cmp al,0
jz  .label_c212
mov [edi],al
.label_c212:
inc edi
loop .label_l81
lodsb
cmp al,0
jz  .label_c213
mov [edi],al
.label_c213:
inc edi
lodsb
cmp al,0
jz  .label_c214
mov [edi],al
.label_c214:
inc edi
add edi,Mbd+519
mov ecx,40
.label_l82:
lodsb
cmp al,0
jz  .label_c215
mov [edi],al
.label_c215:
inc edi
loop .label_l82
lodsb
cmp al,0
jz  .label_c216
mov [edi],al
.label_c216:
inc edi
lodsb
cmp al,0
jz  .label_c217
mov [edi],al
.label_c217:
inc edi
add edi,Mbd+519
mov ecx,40
.label_l83:
lodsb
cmp al,0
jz  .label_c218
mov [edi],al
.label_c218:
inc edi
loop .label_l83
lodsb
cmp al,0
jz  .label_c219
mov [edi],al
.label_c219:
inc edi
lodsb
cmp al,0
jz  .label_c220
mov [edi],al
.label_c220:
inc edi
add edi,Mbd+519
mov ecx,40
.label_l84:
lodsb
cmp al,0
jz  .label_c221
mov [edi],al
.label_c221:
inc edi
loop .label_l84
lodsb
cmp al,0
jz  .label_c222
mov [edi],al
.label_c222:
inc edi
lodsb
cmp al,0
jz  .label_c223
mov [edi],al
.label_c223:
inc edi
add edi,Mbd+519
mov ecx,40
.label_l85:
lodsb
cmp al,0
jz  .label_c224
mov [edi],al
.label_c224:
inc edi
loop .label_l85
lodsb
cmp al,0
jz  .label_c225
mov [edi],al
.label_c225:
inc edi
lodsb
cmp al,0
jz  .label_c226
mov [edi],al
.label_c226:
inc edi
add edi,Mbd+519
mov ecx,40
.label_l86:
lodsb
cmp al,0
jz  .label_c227
mov [edi],al
.label_c227:
inc edi
loop .label_l86
lodsb
cmp al,0
jz  .label_c228
mov [edi],al
.label_c228:
inc edi
lodsb
cmp al,0
jz  .label_c229
mov [edi],al
.label_c229:
inc edi
add edi,Mbd+519
mov ecx,40
.label_l87:
lodsb
cmp al,0
jz  .label_c230
mov [edi],al
.label_c230:
inc edi
loop .label_l87
lodsb
cmp al,0
jz  .label_c231
mov [edi],al
.label_c231:
inc edi
add edi,Mbd+520
mov ecx,36
.label_l88:
lodsb
cmp al,0
jz  .label_c232
mov [edi],al
.label_c232:
inc edi
loop .label_l88
lodsb
cmp al,0
jz  .label_c233
mov [edi],al
.label_c233:
inc edi
lodsb
cmp al,0
jz  .label_c234
mov [edi],al
.label_c234:
inc edi
add edi,Mbd+523
mov ecx,32
.label_l89:
lodsb
cmp al,0
jz  .label_c235
mov [edi],al
.label_c235:
inc edi
loop .label_l89
lodsb
cmp al,0
jz  .label_c236
mov [edi],al
.label_c236:
inc edi
lodsb
cmp al,0
jz  .label_c237
mov [edi],al
.label_c237:
inc edi
lodsb
cmp al,0
jz  .label_c238
mov [edi],al
.label_c238:
inc edi
add edi,Mbd+526
mov ecx,32
.label_l90:
lodsb
cmp al,0
jz  .label_c239
mov [edi],al
.label_c239:
inc edi
loop .label_l90
add edi,Mbd+529
mov ecx,28
.label_l91:
lodsb
cmp al,0
jz  .label_c240
mov [edi],al
.label_c240:
inc edi
loop .label_l91
lodsb
cmp al,0
jz  .label_c241
mov [edi],al
.label_c241:
inc edi
add edi,Mbd+532
mov ecx,24
.label_l92:
lodsb
cmp al,0
jz  .label_c242
mov [edi],al
.label_c242:
inc edi
loop .label_l92
lodsb
cmp al,0
jz  .label_c243
mov [edi],al
.label_c243:
inc edi
lodsb
cmp al,0
jz  .label_c244
mov [edi],al
.label_c244:
inc edi
add edi,Mbd+535
mov ecx,20
.label_l93:
lodsb
cmp al,0
jz  .label_c245
mov [edi],al
.label_c245:
inc edi
loop .label_l93
lodsb
cmp al,0
jz  .label_c246
mov [edi],al
.label_c246:
inc edi
lodsb
cmp al,0
jz  .label_c247
mov [edi],al
.label_c247:
inc edi
lodsb
cmp al,0
jz  .label_c248
mov [edi],al
.label_c248:
inc edi
add edi,Mbd+538
mov ecx,20
.label_l94:
lodsb
cmp al,0
jz  .label_c249
mov [edi],al
.label_c249:
inc edi
loop .label_l94
add edi,Mbd+541
mov ecx,16
.label_l95:
lodsb
cmp al,0
jz  .label_c250
mov [edi],al
.label_c250:
inc edi
loop .label_l95
lodsb
cmp al,0
jz  .label_c251
mov [edi],al
.label_c251:
inc edi
add edi,Mbd+544
mov ecx,12
.label_l96:
lodsb
cmp al,0
jz  .label_c252
mov [edi],al
.label_c252:
inc edi
loop .label_l96
lodsb
cmp al,0
jz  .label_c253
mov [edi],al
.label_c253:
inc edi
lodsb
cmp al,0
jz  .label_c254
mov [edi],al
.label_c254:
inc edi
add edi,Mbd+547
mov ecx,8
.label_l97:
lodsb
cmp al,0
jz  .label_c255
mov [edi],al
.label_c255:
inc edi
loop .label_l97
lodsb
cmp al,0
jz  .label_c256
mov [edi],al
.label_c256:
inc edi
lodsb
cmp al,0
jz  .label_c257
mov [edi],al
.label_c257:
inc edi
lodsb
cmp al,0
jz  .label_c258
mov [edi],al
.label_c258:
inc edi
add edi,Mbd+550
mov ecx,8
.label_l98:
lodsb
cmp al,0
jz  .label_c259
mov [edi],al
.label_c259:
inc edi
loop .label_l98
add edi,Mbd+553
lodsb
cmp al,0
jz  .label_c260
mov [edi],al
.label_c260:
inc edi
lodsb
cmp al,0
jz  .label_c261
mov [edi],al
.label_c261:
inc edi
lodsb
cmp al,0
jz  .label_c262
mov [edi],al
.label_c262:
inc edi
lodsb
cmp al,0
jz  .label_c263
mov [edi],al
.label_c263:
inc edi
lodsb
cmp al,0
jz  .label_c264
mov [edi],al
.label_c264:
inc edi
add edi,Mbd+556
lodsb
cmp al,0
jz  .label_c265
mov [edi],al
.label_c265:
inc edi
lodsb
cmp al,0
jz  .label_c266
mov [edi],al
.label_c266:
inc edi

pop  edx
pop  ecx

            pop  esi
            pop  eax
            pop  edi
endproc






proc DrawUpTerrD

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
add edi,eax
add edi,dword [MapBuf]


add edi,14*(560+Mbd)+2
mov ecx,28
.label_l99:
lodsb
cmp al,0
jz  .label_c267
mov [edi],al
.label_c267:
inc edi
loop .label_l99
add edi,Mbd+534
mov ecx,28
.label_l100:
lodsb
cmp al,0
jz  .label_c268
mov [edi],al
.label_c268:
inc edi
loop .label_l100
add edi,Mbd+534
mov ecx,28
.label_l101:
lodsb
cmp al,0
jz  .label_c269
mov [edi],al
.label_c269:
inc edi
loop .label_l101
add edi,Mbd+534
mov ecx,28
.label_l102:
lodsb
cmp al,0
jz  .label_c270
mov [edi],al
.label_c270:
inc edi
loop .label_l102
add edi,Mbd+534
mov ecx,28
.label_l103:
lodsb
cmp al,0
jz  .label_c271
mov [edi],al
.label_c271:
inc edi
loop .label_l103
add edi,Mbd+534
mov ecx,28
.label_l104:
lodsb
cmp al,0
jz  .label_c272
mov [edi],al
.label_c272:
inc edi
loop .label_l104
add edi,Mbd+534
mov ecx,28
.label_l105:
lodsb
cmp al,0
jz  .label_c273
mov [edi],al
.label_c273:
inc edi
loop .label_l105
add edi,Mbd+534
mov ecx,28
.label_l106:
lodsb
cmp al,0
jz  .label_c274
mov [edi],al
.label_c274:
inc edi
loop .label_l106
add edi,Mbd+534
mov ecx,28
.label_l107:
lodsb
cmp al,0
jz  .label_c275
mov [edi],al
.label_c275:
inc edi
loop .label_l107
add edi,Mbd+534
mov ecx,28
.label_l108:
lodsb
cmp al,0
jz  .label_c276
mov [edi],al
.label_c276:
inc edi
loop .label_l108
add edi,Mbd+534
mov ecx,28
.label_l109:
lodsb
cmp al,0
jz  .label_c277
mov [edi],al
.label_c277:
inc edi
loop .label_l109
add edi,Mbd+534
mov ecx,28
.label_l110:
lodsb
cmp al,0
jz  .label_c278
mov [edi],al
.label_c278:
inc edi
loop .label_l110
add edi,Mbd+534
mov ecx,28
.label_l111:
lodsb
cmp al,0
jz  .label_c279
mov [edi],al
.label_c279:
inc edi
loop .label_l111
add edi,Mbd+534
mov ecx,28
.label_l112:
lodsb
cmp al,0
jz  .label_c280
mov [edi],al
.label_c280:
inc edi
loop .label_l112

pop  edx
pop  ecx

            pop  esi
            pop  eax
            pop  edi
endproc






proc DrawUpTerrE

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
add edi,eax
add edi,dword [MapBuf]


add edi,14*(560+Mbd)+26
mov ecx,28
.label_l113:
lodsb
cmp al,0
jz  .label_c281
mov [edi],al
.label_c281:
inc edi
loop .label_l113
add edi,Mbd+530
mov ecx,28
.label_l114:
lodsb
cmp al,0
jz  .label_c282
mov [edi],al
.label_c282:
inc edi
loop .label_l114
add edi,Mbd+530
mov ecx,28
.label_l115:
lodsb
cmp al,0
jz  .label_c283
mov [edi],al
.label_c283:
inc edi
loop .label_l115
add edi,Mbd+530
mov ecx,28
.label_l116:
lodsb
cmp al,0
jz  .label_c284
mov [edi],al
.label_c284:
inc edi
loop .label_l116
add edi,Mbd+530
mov ecx,28
.label_l117:
lodsb
cmp al,0
jz  .label_c285
mov [edi],al
.label_c285:
inc edi
loop .label_l117
add edi,Mbd+530
mov ecx,28
.label_l118:
lodsb
cmp al,0
jz  .label_c286
mov [edi],al
.label_c286:
inc edi
loop .label_l118
add edi,Mbd+530
mov ecx,28
.label_l119:
lodsb
cmp al,0
jz  .label_c287
mov [edi],al
.label_c287:
inc edi
loop .label_l119
add edi,Mbd+530
mov ecx,28
.label_l120:
lodsb
cmp al,0
jz  .label_c288
mov [edi],al
.label_c288:
inc edi
loop .label_l120
add edi,Mbd+530
mov ecx,28
.label_l121:
lodsb
cmp al,0
jz  .label_c289
mov [edi],al
.label_c289:
inc edi
loop .label_l121
add edi,Mbd+530
mov ecx,28
.label_l122:
lodsb
cmp al,0
jz  .label_c290
mov [edi],al
.label_c290:
inc edi
loop .label_l122
add edi,Mbd+530
mov ecx,28
.label_l123:
lodsb
cmp al,0
jz  .label_c291
mov [edi],al
.label_c291:
inc edi
loop .label_l123
add edi,Mbd+530
mov ecx,28
.label_l124:
lodsb
cmp al,0
jz  .label_c292
mov [edi],al
.label_c292:
inc edi
loop .label_l124
add edi,Mbd+530
mov ecx,28
.label_l125:
lodsb
cmp al,0
jz  .label_c293
mov [edi],al
.label_c293:
inc edi
loop .label_l125
add edi,Mbd+530
mov ecx,28
.label_l126:
lodsb
cmp al,0
jz  .label_c294
mov [edi],al
.label_c294:
inc edi
loop .label_l126

pop  edx
pop  ecx

            pop  esi
            pop  eax
            pop  edi
endproc






proc DrawUpTerrF

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
add edi,eax
add edi,dword [MapBuf]


add edi,26
lodsb
cmp al,0
jz  .label_c295
mov [edi],al
.label_c295:
inc edi
lodsb
cmp al,0
jz  .label_c296
mov [edi],al
.label_c296:
inc edi
lodsb
cmp al,0
jz  .label_c297
mov [edi],al
.label_c297:
inc edi
lodsb
cmp al,0
jz  .label_c298
mov [edi],al
.label_c298:
inc edi
add edi,Mbd+555
lodsb
cmp al,0
jz  .label_c299
mov [edi],al
.label_c299:
inc edi
lodsb
cmp al,0
jz  .label_c300
mov [edi],al
.label_c300:
inc edi
lodsb
cmp al,0
jz  .label_c301
mov [edi],al
.label_c301:
inc edi
lodsb
cmp al,0
jz  .label_c302
mov [edi],al
.label_c302:
inc edi
lodsb
cmp al,0
jz  .label_c303
mov [edi],al
.label_c303:
inc edi
lodsb
cmp al,0
jz  .label_c304
mov [edi],al
.label_c304:
inc edi
add edi,Mbd+553
mov ecx,8
.label_l127:
lodsb
cmp al,0
jz  .label_c305
mov [edi],al
.label_c305:
inc edi
loop .label_l127
add edi,Mbd+551
mov ecx,8
.label_l128:
lodsb
cmp al,0
jz  .label_c306
mov [edi],al
.label_c306:
inc edi
loop .label_l128
lodsb
cmp al,0
jz  .label_c307
mov [edi],al
.label_c307:
inc edi
lodsb
cmp al,0
jz  .label_c308
mov [edi],al
.label_c308:
inc edi
add edi,Mbd+549
mov ecx,12
.label_l129:
lodsb
cmp al,0
jz  .label_c309
mov [edi],al
.label_c309:
inc edi
loop .label_l129
add edi,Mbd+547
mov ecx,12
.label_l130:
lodsb
cmp al,0
jz  .label_c310
mov [edi],al
.label_c310:
inc edi
loop .label_l130
lodsb
cmp al,0
jz  .label_c311
mov [edi],al
.label_c311:
inc edi
lodsb
cmp al,0
jz  .label_c312
mov [edi],al
.label_c312:
inc edi
add edi,Mbd+545
mov ecx,16
.label_l131:
lodsb
cmp al,0
jz  .label_c313
mov [edi],al
.label_c313:
inc edi
loop .label_l131
add edi,Mbd+543
mov ecx,16
.label_l132:
lodsb
cmp al,0
jz  .label_c314
mov [edi],al
.label_c314:
inc edi
loop .label_l132
lodsb
cmp al,0
jz  .label_c315
mov [edi],al
.label_c315:
inc edi
lodsb
cmp al,0
jz  .label_c316
mov [edi],al
.label_c316:
inc edi
add edi,Mbd+541
mov ecx,20
.label_l133:
lodsb
cmp al,0
jz  .label_c317
mov [edi],al
.label_c317:
inc edi
loop .label_l133
add edi,Mbd+539
mov ecx,20
.label_l134:
lodsb
cmp al,0
jz  .label_c318
mov [edi],al
.label_c318:
inc edi
loop .label_l134
lodsb
cmp al,0
jz  .label_c319
mov [edi],al
.label_c319:
inc edi
lodsb
cmp al,0
jz  .label_c320
mov [edi],al
.label_c320:
inc edi
add edi,Mbd+537
mov ecx,24
.label_l135:
lodsb
cmp al,0
jz  .label_c321
mov [edi],al
.label_c321:
inc edi
loop .label_l135
add edi,Mbd+535
mov ecx,24
.label_l136:
lodsb
cmp al,0
jz  .label_c322
mov [edi],al
.label_c322:
inc edi
loop .label_l136
lodsb
cmp al,0
jz  .label_c323
mov [edi],al
.label_c323:
inc edi
lodsb
cmp al,0
jz  .label_c324
mov [edi],al
.label_c324:
inc edi
add edi,Mbd+533
mov ecx,28
.label_l137:
lodsb
cmp al,0
jz  .label_c325
mov [edi],al
.label_c325:
inc edi
loop .label_l137
add edi,Mbd+531
mov ecx,28
.label_l138:
lodsb
cmp al,0
jz  .label_c326
mov [edi],al
.label_c326:
inc edi
loop .label_l138
lodsb
cmp al,0
jz  .label_c327
mov [edi],al
.label_c327:
inc edi
lodsb
cmp al,0
jz  .label_c328
mov [edi],al
.label_c328:
inc edi
add edi,Mbd+529
mov ecx,32
.label_l139:
lodsb
cmp al,0
jz  .label_c329
mov [edi],al
.label_c329:
inc edi
loop .label_l139
add edi,Mbd+527
mov ecx,32
.label_l140:
lodsb
cmp al,0
jz  .label_c330
mov [edi],al
.label_c330:
inc edi
loop .label_l140
lodsb
cmp al,0
jz  .label_c331
mov [edi],al
.label_c331:
inc edi
lodsb
cmp al,0
jz  .label_c332
mov [edi],al
.label_c332:
inc edi
add edi,Mbd+525
mov ecx,36
.label_l141:
lodsb
cmp al,0
jz  .label_c333
mov [edi],al
.label_c333:
inc edi
loop .label_l141
add edi,Mbd+523
mov ecx,36
.label_l142:
lodsb
cmp al,0
jz  .label_c334
mov [edi],al
.label_c334:
inc edi
loop .label_l142
lodsb
cmp al,0
jz  .label_c335
mov [edi],al
.label_c335:
inc edi
lodsb
cmp al,0
jz  .label_c336
mov [edi],al
.label_c336:
inc edi
add edi,Mbd+521
mov ecx,40
.label_l143:
lodsb
cmp al,0
jz  .label_c337
mov [edi],al
.label_c337:
inc edi
loop .label_l143
add edi,Mbd+519
mov ecx,40
.label_l144:
lodsb
cmp al,0
jz  .label_c338
mov [edi],al
.label_c338:
inc edi
loop .label_l144
lodsb
cmp al,0
jz  .label_c339
mov [edi],al
.label_c339:
inc edi
lodsb
cmp al,0
jz  .label_c340
mov [edi],al
.label_c340:
inc edi
add edi,Mbd+517
mov ecx,44
.label_l145:
lodsb
cmp al,0
jz  .label_c341
mov [edi],al
.label_c341:
inc edi
loop .label_l145
add edi,Mbd+515
mov ecx,44
.label_l146:
lodsb
cmp al,0
jz  .label_c342
mov [edi],al
.label_c342:
inc edi
loop .label_l146
lodsb
cmp al,0
jz  .label_c343
mov [edi],al
.label_c343:
inc edi
lodsb
cmp al,0
jz  .label_c344
mov [edi],al
.label_c344:
inc edi
add edi,Mbd+513
mov ecx,48
.label_l147:
lodsb
cmp al,0
jz  .label_c345
mov [edi],al
.label_c345:
inc edi
loop .label_l147
add edi,Mbd+511
mov ecx,48
.label_l148:
lodsb
cmp al,0
jz  .label_c346
mov [edi],al
.label_c346:
inc edi
loop .label_l148
lodsb
cmp al,0
jz  .label_c347
mov [edi],al
.label_c347:
inc edi
lodsb
cmp al,0
jz  .label_c348
mov [edi],al
.label_c348:
inc edi
add edi,Mbd+509
mov ecx,52
.label_l149:
lodsb
cmp al,0
jz  .label_c349
mov [edi],al
.label_c349:
inc edi
loop .label_l149
add edi,Mbd+507
mov ecx,52
.label_l150:
lodsb
cmp al,0
jz  .label_c350
mov [edi],al
.label_c350:
inc edi
loop .label_l150
lodsb
cmp al,0
jz  .label_c351
mov [edi],al
.label_c351:
inc edi
lodsb
cmp al,0
jz  .label_c352
mov [edi],al
.label_c352:
inc edi
add edi,Mbd+505
mov ecx,56
.label_l151:
lodsb
cmp al,0
jz  .label_c353
mov [edi],al
.label_c353:
inc edi
loop .label_l151
add edi,Mbd+504
mov ecx,56
.label_l152:
lodsb
cmp al,0
jz  .label_c354
mov [edi],al
.label_c354:
inc edi
loop .label_l152
add edi,Mbd+506
mov ecx,52
.label_l153:
lodsb
cmp al,0
jz  .label_c355
mov [edi],al
.label_c355:
inc edi
loop .label_l153
add edi,Mbd+510
mov ecx,48
.label_l154:
lodsb
cmp al,0
jz  .label_c356
mov [edi],al
.label_c356:
inc edi
loop .label_l154
add edi,Mbd+514
mov ecx,44
.label_l155:
lodsb
cmp al,0
jz  .label_c357
mov [edi],al
.label_c357:
inc edi
loop .label_l155
add edi,Mbd+518
mov ecx,40
.label_l156:
lodsb
cmp al,0
jz  .label_c358
mov [edi],al
.label_c358:
inc edi
loop .label_l156
add edi,Mbd+522
mov ecx,36
.label_l157:
lodsb
cmp al,0
jz  .label_c359
mov [edi],al
.label_c359:
inc edi
loop .label_l157
add edi,Mbd+526
mov ecx,32
.label_l158:
lodsb
cmp al,0
jz  .label_c360
mov [edi],al
.label_c360:
inc edi
loop .label_l158
add edi,Mbd+530
mov ecx,28
.label_l159:
lodsb
cmp al,0
jz  .label_c361
mov [edi],al
.label_c361:
inc edi
loop .label_l159
add edi,Mbd+534
mov ecx,24
.label_l160:
lodsb
cmp al,0
jz  .label_c362
mov [edi],al
.label_c362:
inc edi
loop .label_l160
add edi,Mbd+538
mov ecx,20
.label_l161:
lodsb
cmp al,0
jz  .label_c363
mov [edi],al
.label_c363:
inc edi
loop .label_l161
add edi,Mbd+542
mov ecx,16
.label_l162:
lodsb
cmp al,0
jz  .label_c364
mov [edi],al
.label_c364:
inc edi
loop .label_l162
add edi,Mbd+546
mov ecx,12
.label_l163:
lodsb
cmp al,0
jz  .label_c365
mov [edi],al
.label_c365:
inc edi
loop .label_l163
add edi,Mbd+550
mov ecx,8
.label_l164:
lodsb
cmp al,0
jz  .label_c366
mov [edi],al
.label_c366:
inc edi
loop .label_l164
add edi,Mbd+554
lodsb
cmp al,0
jz  .label_c367
mov [edi],al
.label_c367:
inc edi
lodsb
cmp al,0
jz  .label_c368
mov [edi],al
.label_c368:
inc edi
lodsb
cmp al,0
jz  .label_c369
mov [edi],al
.label_c369:
inc edi
lodsb
cmp al,0
jz  .label_c370
mov [edi],al
.label_c370:
inc edi

pop  edx
pop  ecx

            pop  esi
            pop  eax
            pop  edi
endproc






proc DrawUpTerrG

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
add edi,eax
add edi,dword [MapBuf]


add edi,14*(560+Mbd)+2
mov ecx,28
.label_l165:
lodsb
cmp al,0
jz  .label_c371
mov [edi],al
.label_c371:
inc edi
loop .label_l165
add edi,Mbd+533
mov ecx,28
.label_l166:
lodsb
cmp al,0
jz  .label_c372
mov [edi],al
.label_c372:
inc edi
loop .label_l166
lodsb
cmp al,0
jz  .label_c373
mov [edi],al
.label_c373:
inc edi
add edi,Mbd+532
mov ecx,28
.label_l167:
lodsb
cmp al,0
jz  .label_c374
mov [edi],al
.label_c374:
inc edi
loop .label_l167
lodsb
cmp al,0
jz  .label_c375
mov [edi],al
.label_c375:
inc edi
lodsb
cmp al,0
jz  .label_c376
mov [edi],al
.label_c376:
inc edi
add edi,Mbd+531
mov ecx,28
.label_l168:
lodsb
cmp al,0
jz  .label_c377
mov [edi],al
.label_c377:
inc edi
loop .label_l168
lodsb
cmp al,0
jz  .label_c378
mov [edi],al
.label_c378:
inc edi
lodsb
cmp al,0
jz  .label_c379
mov [edi],al
.label_c379:
inc edi
lodsb
cmp al,0
jz  .label_c380
mov [edi],al
.label_c380:
inc edi
add edi,Mbd+530
mov ecx,32
.label_l169:
lodsb
cmp al,0
jz  .label_c381
mov [edi],al
.label_c381:
inc edi
loop .label_l169
add edi,Mbd+529
mov ecx,32
.label_l170:
lodsb
cmp al,0
jz  .label_c382
mov [edi],al
.label_c382:
inc edi
loop .label_l170
lodsb
cmp al,0
jz  .label_c383
mov [edi],al
.label_c383:
inc edi
add edi,Mbd+528
mov ecx,32
.label_l171:
lodsb
cmp al,0
jz  .label_c384
mov [edi],al
.label_c384:
inc edi
loop .label_l171
lodsb
cmp al,0
jz  .label_c385
mov [edi],al
.label_c385:
inc edi
lodsb
cmp al,0
jz  .label_c386
mov [edi],al
.label_c386:
inc edi
add edi,Mbd+527
mov ecx,32
.label_l172:
lodsb
cmp al,0
jz  .label_c387
mov [edi],al
.label_c387:
inc edi
loop .label_l172
lodsb
cmp al,0
jz  .label_c388
mov [edi],al
.label_c388:
inc edi
lodsb
cmp al,0
jz  .label_c389
mov [edi],al
.label_c389:
inc edi
lodsb
cmp al,0
jz  .label_c390
mov [edi],al
.label_c390:
inc edi
add edi,Mbd+526
mov ecx,36
.label_l173:
lodsb
cmp al,0
jz  .label_c391
mov [edi],al
.label_c391:
inc edi
loop .label_l173
add edi,Mbd+525
mov ecx,36
.label_l174:
lodsb
cmp al,0
jz  .label_c392
mov [edi],al
.label_c392:
inc edi
loop .label_l174
lodsb
cmp al,0
jz  .label_c393
mov [edi],al
.label_c393:
inc edi
add edi,Mbd+524
mov ecx,36
.label_l175:
lodsb
cmp al,0
jz  .label_c394
mov [edi],al
.label_c394:
inc edi
loop .label_l175
lodsb
cmp al,0
jz  .label_c395
mov [edi],al
.label_c395:
inc edi
lodsb
cmp al,0
jz  .label_c396
mov [edi],al
.label_c396:
inc edi
add edi,Mbd+523
mov ecx,36
.label_l176:
lodsb
cmp al,0
jz  .label_c397
mov [edi],al
.label_c397:
inc edi
loop .label_l176
lodsb
cmp al,0
jz  .label_c398
mov [edi],al
.label_c398:
inc edi
lodsb
cmp al,0
jz  .label_c399
mov [edi],al
.label_c399:
inc edi
lodsb
cmp al,0
jz  .label_c400
mov [edi],al
.label_c400:
inc edi
add edi,Mbd+522
mov ecx,40
.label_l177:
lodsb
cmp al,0
jz  .label_c401
mov [edi],al
.label_c401:
inc edi
loop .label_l177
add edi,Mbd+521
mov ecx,40
.label_l178:
lodsb
cmp al,0
jz  .label_c402
mov [edi],al
.label_c402:
inc edi
loop .label_l178
lodsb
cmp al,0
jz  .label_c403
mov [edi],al
.label_c403:
inc edi
add edi,Mbd+520
mov ecx,36
.label_l179:
lodsb
cmp al,0
jz  .label_c404
mov [edi],al
.label_c404:
inc edi
loop .label_l179
lodsb
cmp al,0
jz  .label_c405
mov [edi],al
.label_c405:
inc edi
lodsb
cmp al,0
jz  .label_c406
mov [edi],al
.label_c406:
inc edi
add edi,Mbd+523
mov ecx,32
.label_l180:
lodsb
cmp al,0
jz  .label_c407
mov [edi],al
.label_c407:
inc edi
loop .label_l180
lodsb
cmp al,0
jz  .label_c408
mov [edi],al
.label_c408:
inc edi
lodsb
cmp al,0
jz  .label_c409
mov [edi],al
.label_c409:
inc edi
lodsb
cmp al,0
jz  .label_c410
mov [edi],al
.label_c410:
inc edi
add edi,Mbd+526
mov ecx,32
.label_l181:
lodsb
cmp al,0
jz  .label_c411
mov [edi],al
.label_c411:
inc edi
loop .label_l181
add edi,Mbd+529
mov ecx,28
.label_l182:
lodsb
cmp al,0
jz  .label_c412
mov [edi],al
.label_c412:
inc edi
loop .label_l182
lodsb
cmp al,0
jz  .label_c413
mov [edi],al
.label_c413:
inc edi
add edi,Mbd+532
mov ecx,24
.label_l183:
lodsb
cmp al,0
jz  .label_c414
mov [edi],al
.label_c414:
inc edi
loop .label_l183
lodsb
cmp al,0
jz  .label_c415
mov [edi],al
.label_c415:
inc edi
lodsb
cmp al,0
jz  .label_c416
mov [edi],al
.label_c416:
inc edi
add edi,Mbd+535
mov ecx,20
.label_l184:
lodsb
cmp al,0
jz  .label_c417
mov [edi],al
.label_c417:
inc edi
loop .label_l184
lodsb
cmp al,0
jz  .label_c418
mov [edi],al
.label_c418:
inc edi
lodsb
cmp al,0
jz  .label_c419
mov [edi],al
.label_c419:
inc edi
lodsb
cmp al,0
jz  .label_c420
mov [edi],al
.label_c420:
inc edi
add edi,Mbd+538
mov ecx,20
.label_l185:
lodsb
cmp al,0
jz  .label_c421
mov [edi],al
.label_c421:
inc edi
loop .label_l185
add edi,Mbd+541
mov ecx,16
.label_l186:
lodsb
cmp al,0
jz  .label_c422
mov [edi],al
.label_c422:
inc edi
loop .label_l186
lodsb
cmp al,0
jz  .label_c423
mov [edi],al
.label_c423:
inc edi
add edi,Mbd+544
mov ecx,12
.label_l187:
lodsb
cmp al,0
jz  .label_c424
mov [edi],al
.label_c424:
inc edi
loop .label_l187
lodsb
cmp al,0
jz  .label_c425
mov [edi],al
.label_c425:
inc edi
lodsb
cmp al,0
jz  .label_c426
mov [edi],al
.label_c426:
inc edi
add edi,Mbd+547
mov ecx,8
.label_l188:
lodsb
cmp al,0
jz  .label_c427
mov [edi],al
.label_c427:
inc edi
loop .label_l188
lodsb
cmp al,0
jz  .label_c428
mov [edi],al
.label_c428:
inc edi
lodsb
cmp al,0
jz  .label_c429
mov [edi],al
.label_c429:
inc edi
lodsb
cmp al,0
jz  .label_c430
mov [edi],al
.label_c430:
inc edi
add edi,Mbd+550
mov ecx,8
.label_l189:
lodsb
cmp al,0
jz  .label_c431
mov [edi],al
.label_c431:
inc edi
loop .label_l189
add edi,Mbd+553
lodsb
cmp al,0
jz  .label_c432
mov [edi],al
.label_c432:
inc edi
lodsb
cmp al,0
jz  .label_c433
mov [edi],al
.label_c433:
inc edi
lodsb
cmp al,0
jz  .label_c434
mov [edi],al
.label_c434:
inc edi
lodsb
cmp al,0
jz  .label_c435
mov [edi],al
.label_c435:
inc edi
lodsb
cmp al,0
jz  .label_c436
mov [edi],al
.label_c436:
inc edi
add edi,Mbd+556
lodsb
cmp al,0
jz  .label_c437
mov [edi],al
.label_c437:
inc edi
lodsb
cmp al,0
jz  .label_c438
mov [edi],al
.label_c438:
inc edi

pop  edx
pop  ecx

            pop  esi
            pop  eax
            pop  edi
endproc






proc DrawUpTerrH

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
add edi,eax
add edi,dword [MapBuf]


add edi,14*(560+Mbd)+26
lodsb
cmp al,0
jz  .label_c439
mov [edi],al
.label_c439:
inc edi
lodsb
cmp al,0
jz  .label_c440
mov [edi],al
.label_c440:
inc edi
lodsb
cmp al,0
jz  .label_c441
mov [edi],al
.label_c441:
inc edi
lodsb
cmp al,0
jz  .label_c442
mov [edi],al
.label_c442:
inc edi
add edi,Mbd+554
mov ecx,8
.label_l190:
lodsb
cmp al,0
jz  .label_c443
mov [edi],al
.label_c443:
inc edi
loop .label_l190
add edi,Mbd+550
mov ecx,12
.label_l191:
lodsb
cmp al,0
jz  .label_c444
mov [edi],al
.label_c444:
inc edi
loop .label_l191
add edi,Mbd+546
mov ecx,16
.label_l192:
lodsb
cmp al,0
jz  .label_c445
mov [edi],al
.label_c445:
inc edi
loop .label_l192
add edi,Mbd+542
mov ecx,20
.label_l193:
lodsb
cmp al,0
jz  .label_c446
mov [edi],al
.label_c446:
inc edi
loop .label_l193
add edi,Mbd+538
mov ecx,24
.label_l194:
lodsb
cmp al,0
jz  .label_c447
mov [edi],al
.label_c447:
inc edi
loop .label_l194
add edi,Mbd+534
mov ecx,28
.label_l195:
lodsb
cmp al,0
jz  .label_c448
mov [edi],al
.label_c448:
inc edi
loop .label_l195
add edi,Mbd+530
mov ecx,32
.label_l196:
lodsb
cmp al,0
jz  .label_c449
mov [edi],al
.label_c449:
inc edi
loop .label_l196
add edi,Mbd+526
mov ecx,36
.label_l197:
lodsb
cmp al,0
jz  .label_c450
mov [edi],al
.label_c450:
inc edi
loop .label_l197
add edi,Mbd+522
mov ecx,40
.label_l198:
lodsb
cmp al,0
jz  .label_c451
mov [edi],al
.label_c451:
inc edi
loop .label_l198
add edi,Mbd+518
mov ecx,44
.label_l199:
lodsb
cmp al,0
jz  .label_c452
mov [edi],al
.label_c452:
inc edi
loop .label_l199
add edi,Mbd+514
mov ecx,48
.label_l200:
lodsb
cmp al,0
jz  .label_c453
mov [edi],al
.label_c453:
inc edi
loop .label_l200
add edi,Mbd+510
mov ecx,52
.label_l201:
lodsb
cmp al,0
jz  .label_c454
mov [edi],al
.label_c454:
inc edi
loop .label_l201
add edi,Mbd+506
mov ecx,56
.label_l202:
lodsb
cmp al,0
jz  .label_c455
mov [edi],al
.label_c455:
inc edi
loop .label_l202


pop  edx
pop  ecx

            pop  esi
            pop  eax
            pop  edi
endproc






proc DrawUpTerrI

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
add edi,eax
add edi,dword [MapBuf]

add edi,14*(560+Mbd)+26
mov ecx,28
.label_l203:
lodsb
cmp al,0
jz  .label_c456
mov [edi],al
.label_c456:
inc edi
loop .label_l203
add edi,Mbd+530
mov ecx,28
.label_l204:
lodsb
cmp al,0
jz  .label_c457
mov [edi],al
.label_c457:
inc edi
loop .label_l204
lodsb
cmp al,0
jz  .label_c458
mov [edi],al
.label_c458:
inc edi
add edi,Mbd+529
mov ecx,28
.label_l205:
lodsb
cmp al,0
jz  .label_c459
mov [edi],al
.label_c459:
inc edi
loop .label_l205
lodsb
cmp al,0
jz  .label_c460
mov [edi],al
.label_c460:
inc edi
lodsb
cmp al,0
jz  .label_c461
mov [edi],al
.label_c461:
inc edi
add edi,Mbd+528
mov ecx,28
.label_l206:
lodsb
cmp al,0
jz  .label_c462
mov [edi],al
.label_c462:
inc edi
loop .label_l206
lodsb
cmp al,0
jz  .label_c463
mov [edi],al
.label_c463:
inc edi
lodsb
cmp al,0
jz  .label_c464
mov [edi],al
.label_c464:
inc edi
lodsb
cmp al,0
jz  .label_c465
mov [edi],al
.label_c465:
inc edi
add edi,Mbd+527
mov ecx,32
.label_l207:
lodsb
cmp al,0
jz  .label_c466
mov [edi],al
.label_c466:
inc edi
loop .label_l207
add edi,Mbd+526
mov ecx,32
.label_l208:
lodsb
cmp al,0
jz  .label_c467
mov [edi],al
.label_c467:
inc edi
loop .label_l208
lodsb
cmp al,0
jz  .label_c468
mov [edi],al
.label_c468:
inc edi
add edi,Mbd+525
mov ecx,32
.label_l209:
lodsb
cmp al,0
jz  .label_c469
mov [edi],al
.label_c469:
inc edi
loop .label_l209
lodsb
cmp al,0
jz  .label_c470
mov [edi],al
.label_c470:
inc edi
lodsb
cmp al,0
jz  .label_c471
mov [edi],al
.label_c471:
inc edi
add edi,Mbd+524
mov ecx,32
.label_l210:
lodsb
cmp al,0
jz  .label_c472
mov [edi],al
.label_c472:
inc edi
loop .label_l210
lodsb
cmp al,0
jz  .label_c473
mov [edi],al
.label_c473:
inc edi
lodsb
cmp al,0
jz  .label_c474
mov [edi],al
.label_c474:
inc edi
lodsb
cmp al,0
jz  .label_c475
mov [edi],al
.label_c475:
inc edi
add edi,Mbd+523
mov ecx,36
.label_l211:
lodsb
cmp al,0
jz  .label_c476
mov [edi],al
.label_c476:
inc edi
loop .label_l211
add edi,Mbd+522
mov ecx,36
.label_l212:
lodsb
cmp al,0
jz  .label_c477
mov [edi],al
.label_c477:
inc edi
loop .label_l212
lodsb
cmp al,0
jz  .label_c478
mov [edi],al
.label_c478:
inc edi
add edi,Mbd+521
mov ecx,36
.label_l213:
lodsb
cmp al,0
jz  .label_c479
mov [edi],al
.label_c479:
inc edi
loop .label_l213
lodsb
cmp al,0
jz  .label_c480
mov [edi],al
.label_c480:
inc edi
lodsb
cmp al,0
jz  .label_c481
mov [edi],al
.label_c481:
inc edi
add edi,Mbd+520
mov ecx,36
.label_l214:
lodsb
cmp al,0
jz  .label_c482
mov [edi],al
.label_c482:
inc edi
loop .label_l214
lodsb
cmp al,0
jz  .label_c483
mov [edi],al
.label_c483:
inc edi
lodsb
cmp al,0
jz  .label_c484
mov [edi],al
.label_c484:
inc edi
lodsb
cmp al,0
jz  .label_c485
mov [edi],al
.label_c485:
inc edi
add edi,Mbd+519
mov ecx,40
.label_l215:
lodsb
cmp al,0
jz  .label_c486
mov [edi],al
.label_c486:
inc edi
loop .label_l215
add edi,Mbd+518
mov ecx,40
.label_l216:
lodsb
cmp al,0
jz  .label_c487
mov [edi],al
.label_c487:
inc edi
loop .label_l216
lodsb
cmp al,0
jz  .label_c488
mov [edi],al
.label_c488:
inc edi
add edi,Mbd+521
mov ecx,36
.label_l217:
lodsb
cmp al,0
jz  .label_c489
mov [edi],al
.label_c489:
inc edi
loop .label_l217
lodsb
cmp al,0
jz  .label_c490
mov [edi],al
.label_c490:
inc edi
lodsb
cmp al,0
jz  .label_c491
mov [edi],al
.label_c491:
inc edi
add edi,Mbd+524
mov ecx,32
.label_l218:
lodsb
cmp al,0
jz  .label_c492
mov [edi],al
.label_c492:
inc edi
loop .label_l218
lodsb
cmp al,0
jz  .label_c493
mov [edi],al
.label_c493:
inc edi
lodsb
cmp al,0
jz  .label_c494
mov [edi],al
.label_c494:
inc edi
lodsb
cmp al,0
jz  .label_c495
mov [edi],al
.label_c495:
inc edi
add edi,Mbd+527
mov ecx,32
.label_l219:
lodsb
cmp al,0
jz  .label_c496
mov [edi],al
.label_c496:
inc edi
loop .label_l219
add edi,Mbd+530
mov ecx,28
.label_l220:
lodsb
cmp al,0
jz  .label_c497
mov [edi],al
.label_c497:
inc edi
loop .label_l220
lodsb
cmp al,0
jz  .label_c498
mov [edi],al
.label_c498:
inc edi
add edi,Mbd+533
mov ecx,24
.label_l221:
lodsb
cmp al,0
jz  .label_c499
mov [edi],al
.label_c499:
inc edi
loop .label_l221
lodsb
cmp al,0
jz  .label_c500
mov [edi],al
.label_c500:
inc edi
lodsb
cmp al,0
jz  .label_c501
mov [edi],al
.label_c501:
inc edi
add edi,Mbd+536
mov ecx,20
.label_l222:
lodsb
cmp al,0
jz  .label_c502
mov [edi],al
.label_c502:
inc edi
loop .label_l222
lodsb
cmp al,0
jz  .label_c503
mov [edi],al
.label_c503:
inc edi
lodsb
cmp al,0
jz  .label_c504
mov [edi],al
.label_c504:
inc edi
lodsb
cmp al,0
jz  .label_c505
mov [edi],al
.label_c505:
inc edi
add edi,Mbd+539
mov ecx,20
.label_l223:
lodsb
cmp al,0
jz  .label_c506
mov [edi],al
.label_c506:
inc edi
loop .label_l223
add edi,Mbd+542
mov ecx,16
.label_l224:
lodsb
cmp al,0
jz  .label_c507
mov [edi],al
.label_c507:
inc edi
loop .label_l224
lodsb
cmp al,0
jz  .label_c508
mov [edi],al
.label_c508:
inc edi
add edi,Mbd+545
mov ecx,12
.label_l225:
lodsb
cmp al,0
jz  .label_c509
mov [edi],al
.label_c509:
inc edi
loop .label_l225
lodsb
cmp al,0
jz  .label_c510
mov [edi],al
.label_c510:
inc edi
lodsb
cmp al,0
jz  .label_c511
mov [edi],al
.label_c511:
inc edi
add edi,Mbd+548
mov ecx,8
.label_l226:
lodsb
cmp al,0
jz  .label_c512
mov [edi],al
.label_c512:
inc edi
loop .label_l226
lodsb
cmp al,0
jz  .label_c513
mov [edi],al
.label_c513:
inc edi
lodsb
cmp al,0
jz  .label_c514
mov [edi],al
.label_c514:
inc edi
lodsb
cmp al,0
jz  .label_c515
mov [edi],al
.label_c515:
inc edi
add edi,Mbd+551
mov ecx,8
.label_l227:
lodsb
cmp al,0
jz  .label_c516
mov [edi],al
.label_c516:
inc edi
loop .label_l227
add edi,Mbd+554
lodsb
cmp al,0
jz  .label_c517
mov [edi],al
.label_c517:
inc edi
lodsb
cmp al,0
jz  .label_c518
mov [edi],al
.label_c518:
inc edi
lodsb
cmp al,0
jz  .label_c519
mov [edi],al
.label_c519:
inc edi
lodsb
cmp al,0
jz  .label_c520
mov [edi],al
.label_c520:
inc edi
lodsb
cmp al,0
jz  .label_c521
mov [edi],al
.label_c521:
inc edi
add edi,Mbd+557
lodsb
cmp al,0
jz  .label_c522
mov [edi],al
.label_c522:
inc edi
lodsb
cmp al,0
jz  .label_c523
mov [edi],al
.label_c523:
inc edi

pop  edx
pop  ecx

            pop  esi
            pop  eax
            pop  edi
endproc






proc DrawUpTerrJ

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
add edi,eax
add edi,dword [MapBuf]


add edi,26
lodsb
cmp al,0
jz  .label_c524
mov [edi],al
.label_c524:
inc edi
lodsb
cmp al,0
jz  .label_c525
mov [edi],al
.label_c525:
inc edi
lodsb
cmp al,0
jz  .label_c526
mov [edi],al
.label_c526:
inc edi
lodsb
cmp al,0
jz  .label_c527
mov [edi],al
.label_c527:
inc edi
add edi,Mbd+554
mov ecx,8
.label_l228:
lodsb
cmp al,0
jz  .label_c528
mov [edi],al
.label_c528:
inc edi
loop .label_l228
add edi,Mbd+550
mov ecx,12
.label_l229:
lodsb
cmp al,0
jz  .label_c529
mov [edi],al
.label_c529:
inc edi
loop .label_l229
add edi,Mbd+546
mov ecx,16
.label_l230:
lodsb
cmp al,0
jz  .label_c530
mov [edi],al
.label_c530:
inc edi
loop .label_l230
add edi,Mbd+542
mov ecx,20
.label_l231:
lodsb
cmp al,0
jz  .label_c531
mov [edi],al
.label_c531:
inc edi
loop .label_l231
add edi,Mbd+538
mov ecx,24
.label_l232:
lodsb
cmp al,0
jz  .label_c532
mov [edi],al
.label_c532:
inc edi
loop .label_l232
add edi,Mbd+534
mov ecx,28
.label_l233:
lodsb
cmp al,0
jz  .label_c533
mov [edi],al
.label_c533:
inc edi
loop .label_l233
add edi,Mbd+530
mov ecx,32
.label_l234:
lodsb
cmp al,0
jz  .label_c534
mov [edi],al
.label_c534:
inc edi
loop .label_l234
add edi,Mbd+526
mov ecx,36
.label_l235:
lodsb
cmp al,0
jz  .label_c535
mov [edi],al
.label_c535:
inc edi
loop .label_l235
add edi,Mbd+522
mov ecx,40
.label_l236:
lodsb
cmp al,0
jz  .label_c536
mov [edi],al
.label_c536:
inc edi
loop .label_l236
add edi,Mbd+518
mov ecx,44
.label_l237:
lodsb
cmp al,0
jz  .label_c537
mov [edi],al
.label_c537:
inc edi
loop .label_l237
add edi,Mbd+514
mov ecx,48
.label_l238:
lodsb
cmp al,0
jz  .label_c538
mov [edi],al
.label_c538:
inc edi
loop .label_l238
add edi,Mbd+510
mov ecx,52
.label_l239:
lodsb
cmp al,0
jz  .label_c539
mov [edi],al
.label_c539:
inc edi
loop .label_l239
add edi,Mbd+506
mov ecx,56
.label_l240:
lodsb
cmp al,0
jz  .label_c540
mov [edi],al
.label_c540:
inc edi
loop .label_l240
add edi,Mbd+506
mov ecx,52
.label_l241:
lodsb
cmp al,0
jz  .label_c541
mov [edi],al
.label_c541:
inc edi
loop .label_l241
add edi,Mbd+509
mov ecx,48
.label_l242:
lodsb
cmp al,0
jz  .label_c542
mov [edi],al
.label_c542:
inc edi
loop .label_l242
lodsb
cmp al,0
jz  .label_c543
mov [edi],al
.label_c543:
inc edi
lodsb
cmp al,0
jz  .label_c544
mov [edi],al
.label_c544:
inc edi
add edi,Mbd+511
mov ecx,48
.label_l243:
lodsb
cmp al,0
jz  .label_c545
mov [edi],al
.label_c545:
inc edi
loop .label_l243
add edi,Mbd+513
mov ecx,44
.label_l244:
lodsb
cmp al,0
jz  .label_c546
mov [edi],al
.label_c546:
inc edi
loop .label_l244
lodsb
cmp al,0
jz  .label_c547
mov [edi],al
.label_c547:
inc edi
lodsb
cmp al,0
jz  .label_c548
mov [edi],al
.label_c548:
inc edi
add edi,Mbd+515
mov ecx,44
.label_l245:
lodsb
cmp al,0
jz  .label_c549
mov [edi],al
.label_c549:
inc edi
loop .label_l245
add edi,Mbd+517
mov ecx,40
.label_l246:
lodsb
cmp al,0
jz  .label_c550
mov [edi],al
.label_c550:
inc edi
loop .label_l246
lodsb
cmp al,0
jz  .label_c551
mov [edi],al
.label_c551:
inc edi
lodsb
cmp al,0
jz  .label_c552
mov [edi],al
.label_c552:
inc edi
add edi,Mbd+519
mov ecx,40
.label_l247:
lodsb
cmp al,0
jz  .label_c553
mov [edi],al
.label_c553:
inc edi
loop .label_l247
add edi,Mbd+521
mov ecx,36
.label_l248:
lodsb
cmp al,0
jz  .label_c554
mov [edi],al
.label_c554:
inc edi
loop .label_l248
lodsb
cmp al,0
jz  .label_c555
mov [edi],al
.label_c555:
inc edi
lodsb
cmp al,0
jz  .label_c556
mov [edi],al
.label_c556:
inc edi
add edi,Mbd+523
mov ecx,36
.label_l249:
lodsb
cmp al,0
jz  .label_c557
mov [edi],al
.label_c557:
inc edi
loop .label_l249
add edi,Mbd+525
mov ecx,32
.label_l250:
lodsb
cmp al,0
jz  .label_c558
mov [edi],al
.label_c558:
inc edi
loop .label_l250
lodsb
cmp al,0
jz  .label_c559
mov [edi],al
.label_c559:
inc edi
lodsb
cmp al,0
jz  .label_c560
mov [edi],al
.label_c560:
inc edi
add edi,Mbd+527
mov ecx,32
.label_l251:
lodsb
cmp al,0
jz  .label_c561
mov [edi],al
.label_c561:
inc edi
loop .label_l251
add edi,Mbd+529
mov ecx,28
.label_l252:
lodsb
cmp al,0
jz  .label_c562
mov [edi],al
.label_c562:
inc edi
loop .label_l252
lodsb
cmp al,0
jz  .label_c563
mov [edi],al
.label_c563:
inc edi
lodsb
cmp al,0
jz  .label_c564
mov [edi],al
.label_c564:
inc edi
add edi,Mbd+531
mov ecx,28
.label_l253:
lodsb
cmp al,0
jz  .label_c565
mov [edi],al
.label_c565:
inc edi
loop .label_l253
add edi,Mbd+533
mov ecx,24
.label_l254:
lodsb
cmp al,0
jz  .label_c566
mov [edi],al
.label_c566:
inc edi
loop .label_l254
lodsb
cmp al,0
jz  .label_c567
mov [edi],al
.label_c567:
inc edi
lodsb
cmp al,0
jz  .label_c568
mov [edi],al
.label_c568:
inc edi
add edi,Mbd+535
mov ecx,24
.label_l255:
lodsb
cmp al,0
jz  .label_c569
mov [edi],al
.label_c569:
inc edi
loop .label_l255
add edi,Mbd+537
mov ecx,20
.label_l256:
lodsb
cmp al,0
jz  .label_c570
mov [edi],al
.label_c570:
inc edi
loop .label_l256
lodsb
cmp al,0
jz  .label_c571
mov [edi],al
.label_c571:
inc edi
lodsb
cmp al,0
jz  .label_c572
mov [edi],al
.label_c572:
inc edi
add edi,Mbd+539
mov ecx,20
.label_l257:
lodsb
cmp al,0
jz  .label_c573
mov [edi],al
.label_c573:
inc edi
loop .label_l257
add edi,Mbd+541
mov ecx,16
.label_l258:
lodsb
cmp al,0
jz  .label_c574
mov [edi],al
.label_c574:
inc edi
loop .label_l258
lodsb
cmp al,0
jz  .label_c575
mov [edi],al
.label_c575:
inc edi
lodsb
cmp al,0
jz  .label_c576
mov [edi],al
.label_c576:
inc edi
add edi,Mbd+543
mov ecx,16
.label_l259:
lodsb
cmp al,0
jz  .label_c577
mov [edi],al
.label_c577:
inc edi
loop .label_l259
add edi,Mbd+545
mov ecx,12
.label_l260:
lodsb
cmp al,0
jz  .label_c578
mov [edi],al
.label_c578:
inc edi
loop .label_l260
lodsb
cmp al,0
jz  .label_c579
mov [edi],al
.label_c579:
inc edi
lodsb
cmp al,0
jz  .label_c580
mov [edi],al
.label_c580:
inc edi
add edi,Mbd+547
mov ecx,12
.label_l261:
lodsb
cmp al,0
jz  .label_c581
mov [edi],al
.label_c581:
inc edi
loop .label_l261
add edi,Mbd+549
mov ecx,8
.label_l262:
lodsb
cmp al,0
jz  .label_c582
mov [edi],al
.label_c582:
inc edi
loop .label_l262
lodsb
cmp al,0
jz  .label_c583
mov [edi],al
.label_c583:
inc edi
lodsb
cmp al,0
jz  .label_c584
mov [edi],al
.label_c584:
inc edi
add edi,Mbd+551
mov ecx,8
.label_l263:
lodsb
cmp al,0
jz  .label_c585
mov [edi],al
.label_c585:
inc edi
loop .label_l263
add edi,Mbd+553
lodsb
cmp al,0
jz  .label_c586
mov [edi],al
.label_c586:
inc edi
lodsb
cmp al,0
jz  .label_c587
mov [edi],al
.label_c587:
inc edi
lodsb
cmp al,0
jz  .label_c588
mov [edi],al
.label_c588:
inc edi
lodsb
cmp al,0
jz  .label_c589
mov [edi],al
.label_c589:
inc edi
lodsb
cmp al,0
jz  .label_c590
mov [edi],al
.label_c590:
inc edi
lodsb
cmp al,0
jz  .label_c591
mov [edi],al
.label_c591:
inc edi
add edi,Mbd+555
lodsb
cmp al,0
jz  .label_c592
mov [edi],al
.label_c592:
inc edi
lodsb
cmp al,0
jz  .label_c593
mov [edi],al
.label_c593:
inc edi
lodsb
cmp al,0
jz  .label_c594
mov [edi],al
.label_c594:
inc edi
lodsb
cmp al,0
jz  .label_c595
mov [edi],al
.label_c595:
inc edi
add edi,Mbd+557
lodsb
cmp al,0
jz  .label_c596
mov [edi],al
.label_c596:
inc edi
lodsb
cmp al,0
jz  .label_c597
mov [edi],al
.label_c597:
inc edi

pop  edx
pop  ecx

            pop  esi
            pop  eax
            pop  edi
endproc






proc DrawUpTerrK

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
add edi,eax
add edi,dword [MapBuf]


add edi,26
lodsb
cmp al,0
jz  .label_c598
mov [edi],al
.label_c598:
inc edi
lodsb
cmp al,0
jz  .label_c599
mov [edi],al
.label_c599:
inc edi
lodsb
cmp al,0
jz  .label_c600
mov [edi],al
.label_c600:
inc edi
lodsb
cmp al,0
jz  .label_c601
mov [edi],al
.label_c601:
inc edi
add edi,Mbd+554
lodsb
cmp al,0
jz  .label_c602
mov [edi],al
.label_c602:
inc edi
lodsb
cmp al,0
jz  .label_c603
mov [edi],al
.label_c603:
inc edi
lodsb
cmp al,0
jz  .label_c604
mov [edi],al
.label_c604:
inc edi
lodsb
cmp al,0
jz  .label_c605
mov [edi],al
.label_c605:
inc edi
lodsb
cmp al,0
jz  .label_c606
mov [edi],al
.label_c606:
inc edi
lodsb
cmp al,0
jz  .label_c607
mov [edi],al
.label_c607:
inc edi
lodsb
cmp al,0
jz  .label_c608
mov [edi],al
.label_c608:
inc edi
add edi,Mbd+551
mov ecx,8
.label_l264:
lodsb
cmp al,0
jz  .label_c609
mov [edi],al
.label_c609:
inc edi
loop .label_l264
lodsb
cmp al,0
jz  .label_c610
mov [edi],al
.label_c610:
inc edi
lodsb
cmp al,0
jz  .label_c611
mov [edi],al
.label_c611:
inc edi
add edi,Mbd+548
mov ecx,12
.label_l265:
lodsb
cmp al,0
jz  .label_c612
mov [edi],al
.label_c612:
inc edi
loop .label_l265
lodsb
cmp al,0
jz  .label_c613
mov [edi],al
.label_c613:
inc edi
add edi,Mbd+545
mov ecx,16
.label_l266:
lodsb
cmp al,0
jz  .label_c614
mov [edi],al
.label_c614:
inc edi
loop .label_l266
add edi,Mbd+542
mov ecx,16
.label_l267:
lodsb
cmp al,0
jz  .label_c615
mov [edi],al
.label_c615:
inc edi
loop .label_l267
lodsb
cmp al,0
jz  .label_c616
mov [edi],al
.label_c616:
inc edi
lodsb
cmp al,0
jz  .label_c617
mov [edi],al
.label_c617:
inc edi
lodsb
cmp al,0
jz  .label_c618
mov [edi],al
.label_c618:
inc edi
add edi,Mbd+539
mov ecx,20
.label_l268:
lodsb
cmp al,0
jz  .label_c619
mov [edi],al
.label_c619:
inc edi
loop .label_l268
lodsb
cmp al,0
jz  .label_c620
mov [edi],al
.label_c620:
inc edi
lodsb
cmp al,0
jz  .label_c621
mov [edi],al
.label_c621:
inc edi
add edi,Mbd+536
mov ecx,24
.label_l269:
lodsb
cmp al,0
jz  .label_c622
mov [edi],al
.label_c622:
inc edi
loop .label_l269
lodsb
cmp al,0
jz  .label_c623
mov [edi],al
.label_c623:
inc edi
add edi,Mbd+533
mov ecx,28
.label_l270:
lodsb
cmp al,0
jz  .label_c624
mov [edi],al
.label_c624:
inc edi
loop .label_l270
add edi,Mbd+530
mov ecx,28
.label_l271:
lodsb
cmp al,0
jz  .label_c625
mov [edi],al
.label_c625:
inc edi
loop .label_l271
lodsb
cmp al,0
jz  .label_c626
mov [edi],al
.label_c626:
inc edi
lodsb
cmp al,0
jz  .label_c627
mov [edi],al
.label_c627:
inc edi
lodsb
cmp al,0
jz  .label_c628
mov [edi],al
.label_c628:
inc edi
add edi,Mbd+527
mov ecx,32
.label_l272:
lodsb
cmp al,0
jz  .label_c629
mov [edi],al
.label_c629:
inc edi
loop .label_l272
lodsb
cmp al,0
jz  .label_c630
mov [edi],al
.label_c630:
inc edi
lodsb
cmp al,0
jz  .label_c631
mov [edi],al
.label_c631:
inc edi
add edi,Mbd+524
mov ecx,36
.label_l273:
lodsb
cmp al,0
jz  .label_c632
mov [edi],al
.label_c632:
inc edi
loop .label_l273
lodsb
cmp al,0
jz  .label_c633
mov [edi],al
.label_c633:
inc edi
add edi,Mbd+521
mov ecx,40
.label_l274:
lodsb
cmp al,0
jz  .label_c634
mov [edi],al
.label_c634:
inc edi
loop .label_l274
add edi,Mbd+518
mov ecx,40
.label_l275:
lodsb
cmp al,0
jz  .label_c635
mov [edi],al
.label_c635:
inc edi
loop .label_l275
lodsb
cmp al,0
jz  .label_c636
mov [edi],al
.label_c636:
inc edi
lodsb
cmp al,0
jz  .label_c637
mov [edi],al
.label_c637:
inc edi
lodsb
cmp al,0
jz  .label_c638
mov [edi],al
.label_c638:
inc edi
add edi,Mbd+519
mov ecx,40
.label_l276:
lodsb
cmp al,0
jz  .label_c639
mov [edi],al
.label_c639:
inc edi
loop .label_l276
lodsb
cmp al,0
jz  .label_c640
mov [edi],al
.label_c640:
inc edi
lodsb
cmp al,0
jz  .label_c641
mov [edi],al
.label_c641:
inc edi
add edi,Mbd+520
mov ecx,40
.label_l277:
lodsb
cmp al,0
jz  .label_c642
mov [edi],al
.label_c642:
inc edi
loop .label_l277
lodsb
cmp al,0
jz  .label_c643
mov [edi],al
.label_c643:
inc edi
add edi,Mbd+521
mov ecx,40
.label_l278:
lodsb
cmp al,0
jz  .label_c644
mov [edi],al
.label_c644:
inc edi
loop .label_l278
add edi,Mbd+522
mov ecx,36
.label_l279:
lodsb
cmp al,0
jz  .label_c645
mov [edi],al
.label_c645:
inc edi
loop .label_l279
lodsb
cmp al,0
jz  .label_c646
mov [edi],al
.label_c646:
inc edi
lodsb
cmp al,0
jz  .label_c647
mov [edi],al
.label_c647:
inc edi
lodsb
cmp al,0
jz  .label_c648
mov [edi],al
.label_c648:
inc edi
add edi,Mbd+523
mov ecx,36
.label_l280:
lodsb
cmp al,0
jz  .label_c649
mov [edi],al
.label_c649:
inc edi
loop .label_l280
lodsb
cmp al,0
jz  .label_c650
mov [edi],al
.label_c650:
inc edi
lodsb
cmp al,0
jz  .label_c651
mov [edi],al
.label_c651:
inc edi
add edi,Mbd+524
mov ecx,36
.label_l281:
lodsb
cmp al,0
jz  .label_c652
mov [edi],al
.label_c652:
inc edi
loop .label_l281
lodsb
cmp al,0
jz  .label_c653
mov [edi],al
.label_c653:
inc edi
add edi,Mbd+525
mov ecx,36
.label_l282:
lodsb
cmp al,0
jz  .label_c654
mov [edi],al
.label_c654:
inc edi
loop .label_l282
add edi,Mbd+526
mov ecx,32
.label_l283:
lodsb
cmp al,0
jz  .label_c655
mov [edi],al
.label_c655:
inc edi
loop .label_l283
lodsb
cmp al,0
jz  .label_c656
mov [edi],al
.label_c656:
inc edi
lodsb
cmp al,0
jz  .label_c657
mov [edi],al
.label_c657:
inc edi
lodsb
cmp al,0
jz  .label_c658
mov [edi],al
.label_c658:
inc edi
add edi,Mbd+527
mov ecx,32
.label_l284:
lodsb
cmp al,0
jz  .label_c659
mov [edi],al
.label_c659:
inc edi
loop .label_l284
lodsb
cmp al,0
jz  .label_c660
mov [edi],al
.label_c660:
inc edi
lodsb
cmp al,0
jz  .label_c661
mov [edi],al
.label_c661:
inc edi
add edi,Mbd+528
mov ecx,32
.label_l285:
lodsb
cmp al,0
jz  .label_c662
mov [edi],al
.label_c662:
inc edi
loop .label_l285
lodsb
cmp al,0
jz  .label_c663
mov [edi],al
.label_c663:
inc edi
add edi,Mbd+529
mov ecx,32
.label_l286:
lodsb
cmp al,0
jz  .label_c664
mov [edi],al
.label_c664:
inc edi
loop .label_l286
add edi,Mbd+530
mov ecx,28
.label_l287:
lodsb
cmp al,0
jz  .label_c665
mov [edi],al
.label_c665:
inc edi
loop .label_l287
lodsb
cmp al,0
jz  .label_c666
mov [edi],al
.label_c666:
inc edi
lodsb
cmp al,0
jz  .label_c667
mov [edi],al
.label_c667:
inc edi
lodsb
cmp al,0
jz  .label_c668
mov [edi],al
.label_c668:
inc edi
add edi,Mbd+531
mov ecx,28
.label_l288:
lodsb
cmp al,0
jz  .label_c669
mov [edi],al
.label_c669:
inc edi
loop .label_l288
lodsb
cmp al,0
jz  .label_c670
mov [edi],al
.label_c670:
inc edi
lodsb
cmp al,0
jz  .label_c671
mov [edi],al
.label_c671:
inc edi
add edi,Mbd+532
mov ecx,28
.label_l289:
lodsb
cmp al,0
jz  .label_c672
mov [edi],al
.label_c672:
inc edi
loop .label_l289

pop  edx
pop  ecx

            pop  esi
            pop  eax
            pop  edi
endproc






proc DrawUpTerrL

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
add edi,eax
add edi,dword [MapBuf]


add edi,14*(560+Mbd)+2
mov ecx,52
.label_l290:
lodsb
cmp al,0
jz  .label_c673
mov [edi],al
.label_c673:
inc edi
loop .label_l290
add edi,Mbd+510
mov ecx,48
.label_l291:
lodsb
cmp al,0
jz  .label_c674
mov [edi],al
.label_c674:
inc edi
loop .label_l291
add edi,Mbd+514
mov ecx,44
.label_l292:
lodsb
cmp al,0
jz  .label_c675
mov [edi],al
.label_c675:
inc edi
loop .label_l292
add edi,Mbd+518
mov ecx,40
.label_l293:
lodsb
cmp al,0
jz  .label_c676
mov [edi],al
.label_c676:
inc edi
loop .label_l293
add edi,Mbd+522
mov ecx,36
.label_l294:
lodsb
cmp al,0
jz  .label_c677
mov [edi],al
.label_c677:
inc edi
loop .label_l294
add edi,Mbd+526
mov ecx,32
.label_l295:
lodsb
cmp al,0
jz  .label_c678
mov [edi],al
.label_c678:
inc edi
loop .label_l295
add edi,Mbd+530
mov ecx,28
.label_l296:
lodsb
cmp al,0
jz  .label_c679
mov [edi],al
.label_c679:
inc edi
loop .label_l296
add edi,Mbd+534
mov ecx,24
.label_l297:
lodsb
cmp al,0
jz  .label_c680
mov [edi],al
.label_c680:
inc edi
loop .label_l297
add edi,Mbd+538
mov ecx,20
.label_l298:
lodsb
cmp al,0
jz  .label_c681
mov [edi],al
.label_c681:
inc edi
loop .label_l298
add edi,Mbd+542
mov ecx,16
.label_l299:
lodsb
cmp al,0
jz  .label_c682
mov [edi],al
.label_c682:
inc edi
loop .label_l299
add edi,Mbd+546
mov ecx,12
.label_l300:
lodsb
cmp al,0
jz  .label_c683
mov [edi],al
.label_c683:
inc edi
loop .label_l300
add edi,Mbd+550
mov ecx,8
.label_l301:
lodsb
cmp al,0
jz  .label_c684
mov [edi],al
.label_c684:
inc edi
loop .label_l301
add edi,Mbd+554
lodsb
cmp al,0
jz  .label_c685
mov [edi],al
.label_c685:
inc edi
lodsb
cmp al,0
jz  .label_c686
mov [edi],al
.label_c686:
inc edi
lodsb
cmp al,0
jz  .label_c687
mov [edi],al
.label_c687:
inc edi
lodsb
cmp al,0
jz  .label_c688
mov [edi],al
.label_c688:
inc edi

pop  edx
pop  ecx

            pop  esi
            pop  eax
            pop  edi
endproc






proc DrawUpTerrM

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
add edi,eax
add edi,dword [MapBuf]


add edi,26
lodsb
cmp al,0
jz  .label_c689
mov [edi],al
.label_c689:
inc edi
lodsb
cmp al,0
jz  .label_c690
mov [edi],al
.label_c690:
inc edi
lodsb
cmp al,0
jz  .label_c691
mov [edi],al
.label_c691:
inc edi
lodsb
cmp al,0
jz  .label_c692
mov [edi],al
.label_c692:
inc edi
add edi,Mbd+555
lodsb
cmp al,0
jz  .label_c693
mov [edi],al
.label_c693:
inc edi
lodsb
cmp al,0
jz  .label_c694
mov [edi],al
.label_c694:
inc edi
lodsb
cmp al,0
jz  .label_c695
mov [edi],al
.label_c695:
inc edi
lodsb
cmp al,0
jz  .label_c696
mov [edi],al
.label_c696:
inc edi
lodsb
cmp al,0
jz  .label_c697
mov [edi],al
.label_c697:
inc edi
lodsb
cmp al,0
jz  .label_c698
mov [edi],al
.label_c698:
inc edi
lodsb
cmp al,0
jz  .label_c699
mov [edi],al
.label_c699:
inc edi
add edi,Mbd+552
mov ecx,8
.label_l302:
lodsb
cmp al,0
jz  .label_c700
mov [edi],al
.label_c700:
inc edi
loop .label_l302
lodsb
cmp al,0
jz  .label_c701
mov [edi],al
.label_c701:
inc edi
lodsb
cmp al,0
jz  .label_c702
mov [edi],al
.label_c702:
inc edi
add edi,Mbd+549
mov ecx,12
.label_l303:
lodsb
cmp al,0
jz  .label_c703
mov [edi],al
.label_c703:
inc edi
loop .label_l303
lodsb
cmp al,0
jz  .label_c704
mov [edi],al
.label_c704:
inc edi
add edi,Mbd+546
mov ecx,16
.label_l304:
lodsb
cmp al,0
jz  .label_c705
mov [edi],al
.label_c705:
inc edi
loop .label_l304
add edi,Mbd+543
mov ecx,16
.label_l305:
lodsb
cmp al,0
jz  .label_c706
mov [edi],al
.label_c706:
inc edi
loop .label_l305
lodsb
cmp al,0
jz  .label_c707
mov [edi],al
.label_c707:
inc edi
lodsb
cmp al,0
jz  .label_c708
mov [edi],al
.label_c708:
inc edi
lodsb
cmp al,0
jz  .label_c709
mov [edi],al
.label_c709:
inc edi
add edi,Mbd+540
mov ecx,20
.label_l306:
lodsb
cmp al,0
jz  .label_c710
mov [edi],al
.label_c710:
inc edi
loop .label_l306
lodsb
cmp al,0
jz  .label_c711
mov [edi],al
.label_c711:
inc edi
lodsb
cmp al,0
jz  .label_c712
mov [edi],al
.label_c712:
inc edi
add edi,Mbd+537
mov ecx,24
.label_l307:
lodsb
cmp al,0
jz  .label_c713
mov [edi],al
.label_c713:
inc edi
loop .label_l307
lodsb
cmp al,0
jz  .label_c714
mov [edi],al
.label_c714:
inc edi
add edi,Mbd+534
mov ecx,28
.label_l308:
lodsb
cmp al,0
jz  .label_c715
mov [edi],al
.label_c715:
inc edi
loop .label_l308
add edi,Mbd+531
mov ecx,28
.label_l309:
lodsb
cmp al,0
jz  .label_c716
mov [edi],al
.label_c716:
inc edi
loop .label_l309
lodsb
cmp al,0
jz  .label_c717
mov [edi],al
.label_c717:
inc edi
lodsb
cmp al,0
jz  .label_c718
mov [edi],al
.label_c718:
inc edi
lodsb
cmp al,0
jz  .label_c719
mov [edi],al
.label_c719:
inc edi
add edi,Mbd+528
mov ecx,32
.label_l310:
lodsb
cmp al,0
jz  .label_c720
mov [edi],al
.label_c720:
inc edi
loop .label_l310
lodsb
cmp al,0
jz  .label_c721
mov [edi],al
.label_c721:
inc edi
lodsb
cmp al,0
jz  .label_c722
mov [edi],al
.label_c722:
inc edi
add edi,Mbd+525
mov ecx,36
.label_l311:
lodsb
cmp al,0
jz  .label_c723
mov [edi],al
.label_c723:
inc edi
loop .label_l311
lodsb
cmp al,0
jz  .label_c724
mov [edi],al
.label_c724:
inc edi
add edi,Mbd+522
mov ecx,40
.label_l312:
lodsb
cmp al,0
jz  .label_c725
mov [edi],al
.label_c725:
inc edi
loop .label_l312
add edi,Mbd+519
mov ecx,40
.label_l313:
lodsb
cmp al,0
jz  .label_c726
mov [edi],al
.label_c726:
inc edi
loop .label_l313
lodsb
cmp al,0
jz  .label_c727
mov [edi],al
.label_c727:
inc edi
lodsb
cmp al,0
jz  .label_c728
mov [edi],al
.label_c728:
inc edi
lodsb
cmp al,0
jz  .label_c729
mov [edi],al
.label_c729:
inc edi
add edi,Mbd+516
mov ecx,40
.label_l314:
lodsb
cmp al,0
jz  .label_c730
mov [edi],al
.label_c730:
inc edi
loop .label_l314
lodsb
cmp al,0
jz  .label_c731
mov [edi],al
.label_c731:
inc edi
lodsb
cmp al,0
jz  .label_c732
mov [edi],al
.label_c732:
inc edi
add edi,Mbd+517
mov ecx,40
.label_l315:
lodsb
cmp al,0
jz  .label_c733
mov [edi],al
.label_c733:
inc edi
loop .label_l315
lodsb
cmp al,0
jz  .label_c734
mov [edi],al
.label_c734:
inc edi
add edi,Mbd+518
mov ecx,40
.label_l316:
lodsb
cmp al,0
jz  .label_c735
mov [edi],al
.label_c735:
inc edi
loop .label_l316
add edi,Mbd+519
mov ecx,36
.label_l317:
lodsb
cmp al,0
jz  .label_c736
mov [edi],al
.label_c736:
inc edi
loop .label_l317
lodsb
cmp al,0
jz  .label_c737
mov [edi],al
.label_c737:
inc edi
lodsb
cmp al,0
jz  .label_c738
mov [edi],al
.label_c738:
inc edi
lodsb
cmp al,0
jz  .label_c739
mov [edi],al
.label_c739:
inc edi
add edi,Mbd+520
mov ecx,36
.label_l318:
lodsb
cmp al,0
jz  .label_c740
mov [edi],al
.label_c740:
inc edi
loop .label_l318
lodsb
cmp al,0
jz  .label_c741
mov [edi],al
.label_c741:
inc edi
lodsb
cmp al,0
jz  .label_c742
mov [edi],al
.label_c742:
inc edi
add edi,Mbd+521
mov ecx,36
.label_l319:
lodsb
cmp al,0
jz  .label_c743
mov [edi],al
.label_c743:
inc edi
loop .label_l319
lodsb
cmp al,0
jz  .label_c744
mov [edi],al
.label_c744:
inc edi
add edi,Mbd+522
mov ecx,36
.label_l320:
lodsb
cmp al,0
jz  .label_c745
mov [edi],al
.label_c745:
inc edi
loop .label_l320
add edi,Mbd+523
mov ecx,32
.label_l321:
lodsb
cmp al,0
jz  .label_c746
mov [edi],al
.label_c746:
inc edi
loop .label_l321
lodsb
cmp al,0
jz  .label_c747
mov [edi],al
.label_c747:
inc edi
lodsb
cmp al,0
jz  .label_c748
mov [edi],al
.label_c748:
inc edi
lodsb
cmp al,0
jz  .label_c749
mov [edi],al
.label_c749:
inc edi
add edi,Mbd+524
mov ecx,32
.label_l322:
lodsb
cmp al,0
jz  .label_c750
mov [edi],al
.label_c750:
inc edi
loop .label_l322
lodsb
cmp al,0
jz  .label_c751
mov [edi],al
.label_c751:
inc edi
lodsb
cmp al,0
jz  .label_c752
mov [edi],al
.label_c752:
inc edi
add edi,Mbd+525
mov ecx,32
.label_l323:
lodsb
cmp al,0
jz  .label_c753
mov [edi],al
.label_c753:
inc edi
loop .label_l323
lodsb
cmp al,0
jz  .label_c754
mov [edi],al
.label_c754:
inc edi
add edi,Mbd+526
mov ecx,32
.label_l324:
lodsb
cmp al,0
jz  .label_c755
mov [edi],al
.label_c755:
inc edi
loop .label_l324
add edi,Mbd+527
mov ecx,28
.label_l325:
lodsb
cmp al,0
jz  .label_c756
mov [edi],al
.label_c756:
inc edi
loop .label_l325
lodsb
cmp al,0
jz  .label_c757
mov [edi],al
.label_c757:
inc edi
lodsb
cmp al,0
jz  .label_c758
mov [edi],al
.label_c758:
inc edi
lodsb
cmp al,0
jz  .label_c759
mov [edi],al
.label_c759:
inc edi
add edi,Mbd+528
mov ecx,28
.label_l326:
lodsb
cmp al,0
jz  .label_c760
mov [edi],al
.label_c760:
inc edi
loop .label_l326
lodsb
cmp al,0
jz  .label_c761
mov [edi],al
.label_c761:
inc edi
lodsb
cmp al,0
jz  .label_c762
mov [edi],al
.label_c762:
inc edi
add edi,Mbd+530
mov ecx,28
.label_l327:
lodsb
cmp al,0
jz  .label_c763
mov [edi],al
.label_c763:
inc edi
loop .label_l327

pop  edx
pop  ecx

            pop  esi
            pop  eax
            pop  edi
endproc






