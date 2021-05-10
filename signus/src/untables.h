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


/*

Tento h-file obsahuje popisy strategickych vlastnosti jednotek-
utocna, obranna cisla, paliva, dohledy, dojezdy...

*/


#ifndef _UNTABLES_H
#define _UNTABLES_H






// Perseus Alfa:

#define utPA_VEL                      70    /* animation velocity */
#define utPA_VIS                       6    /* visib range */
#define utPA_FUEL                    100    /* max fuel */
#define utPA_TU                       70    /* time units */
#define utPA_HP                       20    /* hit points */
#define utPA_DN0                       5    /* defence no */
#define utPA_DN1                       8    
#define utPA_DN2                       7    

#define utPA_AN                       10    /* attack no */
#define utPA_ANB                      +3    /* attack bonus no */
#define utPA_AT                       20    /* attack time lost */
#define utPA_MAXR                      2    /* max attack range */
#define utPA_AMMO                     15    /* ammo */

#define utPA_INC_HP                    2    /* next level growing... */
#define utPA_INC_DN                  1/3
#define utPA_INC_AN                  1/2





// Perseus Beta:

#define utPB_VEL                      90
#define utPB_VIS                       7
#define utPB_FUEL                     80
#define utPB_TU                       60
#define utPB_HP                       30
#define utPB_DN0                       6
#define utPB_DN1                      10
#define utPB_DN2                       9

#define utPB_AN                       12
#define utPB_ANB                     +10
#define utPB_AT                       25
#define utPB_MAXR                      4
#define utPB_AMMO                     20

#define utPB_INC_HP                    2
#define utPB_INC_DN                  1/3
#define utPB_INC_AN                  1/2





// Perseus Gama:

#define utPG_VEL                      91
#define utPG_VIS                       8
#define utPG_FUEL                     80
#define utPG_TU                       60
#define utPG_HP                       45
#define utPG_DN0                      13
#define utPG_DN1                      13
#define utPG_DN2                      13

#define utPG_AN                       18
#define utPG_ANB                     +20
#define utPG_AT                       35
#define utPG_MAXR                      6
#define utPG_AMMO                     15

#define utPG_2AN                      15    /* 2nd weapon */
#define utPG_2ANB                     +5
#define utPG_2AT                      20
#define utPG_2MAXR                     4
#define utPG_2AMMO                    30

#define utPG_INC_HP                    3
#define utPG_INC_DN                  1/3
#define utPG_INC_AN                  1/2
#define utPG_INC_2AN                 1/3





// Golias:

#define utGO_VEL                     200
#define utGO_VIS                      12
#define utGO_FUEL                     50
#define utGO_TU                       20
#define utGO_HP                       20
#define utGO_DN0                       4
#define utGO_DN1                       5
#define utGO_DN2                       4

#define utGO_AN                       20
#define utGO_ANB                     +25
#define utGO_AN2                      17    /* AN, ANB for outer fields */
#define utGO_ANB2                    +20
#define utGO_AT                       20
#define utGO_MINR                      3    /*  min attack range */
#define utGO_MAXR                     20
#define utGO_AMMO                      7

#define utGO_INC_HP                    2
#define utGO_INC_DN                  1/3
#define utGO_INC_AN                  2/3





// Patriot:

#define utPT_VEL                     181
#define utPT_VIS                      10
#define utPT_FUEL                     70
#define utPT_TU                       40
#define utPT_HP                       15
#define utPT_DN0                       5
#define utPT_DN1                       5
#define utPT_DN2                       6

#define utPT_AN                       13
#define utPT_ANB                     +10
#define utPT_AN2                       5
#define utPT_ANB2                     +5
#define utPT_AT                       30
#define utPT_MINR                      3
#define utPT_MAXR                     10
#define utPT_AMMO                     15

#define utPT_INC_HP                    1
#define utPT_INC_DN                  1/4
#define utPT_INC_AN                  1/3






// Herkules:

#define utHE_VEL                     235
#define utHE_VIS                      10
#define utHE_FUEL                     50
#define utHE_TU                       30
#define utHE_HP                       20
#define utHE_DN0                       5
#define utHE_DN1                       5
#define utHE_DN2                       6

#define utHE_AN                       15
#define utHE_ANB                     +20
#define utHE_AN2                      10
#define utHE_ANB2                    +10
#define utHE_AT                       30
#define utHE_MINR                      7
#define utHE_MAXR                     19
#define utHE_AMMO                      6

#define utHE_INC_HP                    2
#define utHE_INC_DN                  1/3
#define utHE_INC_AN                    1






// Skorpion:

#define utSK_VEL                      94
#define utSK_VIS                       8
#define utSK_FUEL                    100
#define utSK_TU                       50
#define utSK_HP                       15
#define utSK_DN0                       5
#define utSK_DN1                       6
#define utSK_DN2                       5

#define utSK_AN                       10
#define utSK_ANB                     +15
#define utSK_AN2                       8
#define utSK_ANB2                    +10
#define utSK_AT                       20
#define utSK_MINR                      5
#define utSK_MAXR                     10
#define utSK_AMMO                      4

#define utSK_INC_HP                    2
#define utSK_INC_DN                  1/3
#define utSK_INC_AN                    1






// Bumerang:

#define utBM_VEL                      40
#define utBM_VIS                      15
#define utBM_FUEL                    150
#define utBM_TU                      100
#define utBM_HP                       10
#define utBM_DN0                       2
#define utBM_DN1                       3
#define utBM_DN2                       3

#define utBM_AN                        8
#define utBM_ANB                      +2
#define utBM_AT                       30
#define utBM_MAXR                      1
#define utBM_AMMO                     20

#define utBM_INC_HP                    1
#define utBM_INC_DN                  1/4
#define utBM_INC_AN                  1/3






// Ceres:

#define utCR_VEL                     130
#define utCR_VIS                       6
#define utCR_FUEL                   2000
#define utCR_TU                       80
#define utCR_HP                        1
#define utCR_DN0                       5
#define utCR_DN1                       4
#define utCR_DN2                       2
#define utSUPP_SUPPORT                15






// Gnom:

#define utGN_VEL                     130
#define utGN_VIS                       6
#define utGN_FUEL                    150
#define utGN_TU                       70
#define utGN_HP                        1
#define utGN_DN0                       7
#define utGN_DN1                       7
#define utGN_DN2                       7

#define utGN_AMGN                    150
#define utGN_AMEX                    100
#define utGN_AMEN                    100
#define utGN_AMMI                     10






// Satan:

#define utST_VEL                      75
#define utST_VIS                      10
#define utST_FUEL                     90
#define utST_TU                       70
#define utST_HP                        1
#define utST_DN0                       5
#define utST_DN1                       7
#define utST_DN2                       7

#define utST_AN                       15
#define utST_ANB                     +20
#define utST_AN2                       8
#define utST_ANB2                    +10
#define utST_AT                       20
#define utST_MINR                      2
#define utST_MAXR                      5
#define utST_AMMO                      5

#define utST_DAN1                     35    /* destruction AN, ANB */
#define utST_DANB1                   +30
#define utST_DAN2                     25
#define utST_DANB2                   +10

#define utST_INC_DN                  1/3
#define utST_INC_AN                  2/3
#define utST_INC_DAN                 3/2






// Thor:

#define utTH_VIS                      15
#define utTH_TU                       50
#define utTH_HP                       30
#define utTH_DN0                      15    /* inside */
#define utTH_DN1                      15
#define utTH_DN2                      15
#define utTH_DND0                     13    /* delta between outside and  */
#define utTH_DND1                     12    /* inside state of defence no */
#define utTH_DND2                     12

#define utTH_AN                       13
#define utTH_ANB                     +20
#define utTH_AN2                       8
#define utTH_ANB2                    +10
#define utTH_AT                       20
#define utTH_MINR                      5
#define utTH_MAXR                     17
#define utTH_AMMO                    100

#define utTH_INC_HP                    3
#define utTH_INC_DN                  1/3
#define utTH_INC_AN                  2/3

#define utTH_HIDE                     30





// Pagoda:

#define utPD_VIS                      10
#define utPD_TU                       50
#define utPD_HP                       40
#define utPD_DN0                       9
#define utPD_DN1                      10
#define utPD_DN2                      10

#define utPD_AN                       15
#define utPD_ANB                      +7
#define utPD_AT                       25
#define utPD_MAXR                      5
#define utPD_AMMO                    100

#define utPD_INC_HP                    3
#define utPD_INC_DN                  1/2
#define utPD_INC_AN                  2/3






// Minotaurus:

#define utMI_VIS                      18
#define utMI_TU                       50
#define utMI_HP                       40
#define utMI_DN0                       8
#define utMI_DN1                       7
#define utMI_DN2                       7

#define utMI_AN                       20
#define utMI_ANB                     +15
#define utMI_AN2                      10
#define utMI_ANB2                    +10
#define utMI_AT                       50
#define utMI_MINR                      3
#define utMI_MAXR                     18
#define utMI_AMMO                    100

#define utMI_INC_HP                    3
#define utMI_INC_DN                  1/3
#define utMI_INC_AN                    1






// Helios:

#define utHL_VEL                      80
#define utHL_VIS                       8
#define utHL_FUEL                    250
#define utHL_TU                       80
#define utHL_HP                       30
#define utHL_DN0                       8
#define utHL_DN1                       9
#define utHL_DN2                       9

#define utHL_AN                       18
#define utHL_ANB                     +10
#define utHL_AT                       25
#define utHL_MAXR                      7
#define utHL_AMMO                     20

#define utHL_2AN                      17    /* 2nd weapon */
#define utHL_2ANB                     +5
#define utHL_2AT                      20
#define utHL_2MAXR                     5
#define utHL_2AMMO                    30

#define utHL_INC_HP                    2
#define utHL_INC_DN                  1/3 
#define utHL_INC_AN                  2/3
#define utHL_INC_2AN                 1/2





// Uran:

#define utUR_VEL                     250
#define utUR_VIS                       6
#define utUR_FUEL                    300
#define utUR_TU                       70
#define utUR_HP                       25
#define utUR_DN0                       4
#define utUR_DN1                       4
#define utUR_DN2                       4






// Mystik:

#define utMY_VEL                      40
#define utMY_VIS                      15
#define utMY_FUEL                    150
#define utMY_TU                       90
#define utMY_HP                       40
#define utMY_DN0                      12
#define utMY_DN1                      12
#define utMY_DN2                      12

#define utMY_AN                       17
#define utMY_ANB                      +5
#define utMY_AT                       30
#define utMY_MAXR                      3
#define utMY_AMMO                     20

#define utMY_2AN                      20    /* 2nd weapon */
#define utMY_2ANB                    +30
#define utMY_2AT                      50
#define utMY_2MINR                     2
#define utMY_2MAXR                    10
#define utMY_2AMMO                     4

#define utMY_3AN                      20    /* 3rd weapon */
#define utMY_3ANB                    +30
#define utMY_3AN2                     10
#define utMY_3ANB2                    +5
#define utMY_3AT                      50
#define utMY_3MINR                     3
#define utMY_3MAXR                     7
#define utMY_3AMMO                     6

#define utMY_INC_HP                    3
#define utMY_INC_DN                  1/2
#define utMY_INC_AN                  1/2
#define utMY_INC_2AN                   1
#define utMY_INC_3AN                   1





// Olymp:

#define utOL_VEL                     150
#define utOL_VIS                       6
#define utOL_FUEL                    180
#define utOL_TU                       50
#define utOL_HP                       15
#define utOL_DN0                       5
#define utOL_DN1                       6
#define utOL_DN2                       6





// Mohykan:

#define utMH_VEL                     120
#define utMH_VIS                       8
#define utMH_FUEL                    140
#define utMH_TU                       50
#define utMH_HP                       35
#define utMH_DN0                       8
#define utMH_DN1                       9
#define utMH_DN2                       9

#define utMH_AN                       18
#define utMH_ANB                     +10
#define utMH_AT                       20
#define utMH_MAXR                      5
#define utMH_AMMO                     25

#define utMH_2AN                      12    /* 2nd weapon */
#define utMH_2ANB                    +10
#define utMH_2AT                      20
#define utMH_2MAXR                     5
#define utMH_2AMMO                    30

#define utMH_INC_HP                    3
#define utMH_INC_DN                  1/2
#define utMH_INC_AN                  2/3
#define utMH_INC_2AN                 1/2





// Mesias:

#define utME_VEL                     130
#define utME_VIS                      20
#define utME_FUEL                     80
#define utME_TU                       50
#define utME_HP                       20
#define utME_DN0                       5
#define utME_DN1                       5
#define utME_DN2                       5

#define utME_AN                       20
#define utME_ANB                     +30
#define utME_AN2                      15    /* AN, ANB for outer fields */
#define utME_ANB2                    +15
#define utME_AT                       50
#define utME_MINR                      5    /*  min attack range */
#define utME_MAXR                     20
#define utME_AMMO                     10

#define utME_INC_HP                    2
#define utME_INC_DN                  1/3
#define utME_INC_AN                  3/2






// Ganymedes:

#define utGA_VEL                     120
#define utGA_VIS                       6
#define utGA_FUEL                   1500
#define utGA_TU                       70
#define utGA_HP                       15
#define utGA_DN0                       6
#define utGA_DN1                       6
#define utGA_DN2                       6

#define utGA_AMGN                    100
#define utGA_AMEX                    100
#define utGA_AMEN                    100
#define utGA_AMMI                     20



// Radar:

#define utRD_VIS                      20 
#define utRD_HP                       50
#define utRD_DN0                       4
#define utRD_DN1                       6
#define utRD_DN2                       6
#define utRD_MONEY                     0






// Zakladna:

#define utBS_VIS                      10
#define utBS_HP                      650
#define utBS_DN0                       6
#define utBS_DN1                       6
#define utBS_DN2                       6
#define utBS_MONEY                    15




// Sklad:

#define utWR_VIS                       5
#define utWR_HP                      250
#define utWR_DN0                       4
#define utWR_DN1                       4
#define utWR_DN2                       4
#define utWR_MONEY                     6







// Gargantua:

#define utGR_VEL                     250
#define utGR_VIS                       6
#define utGR_FUEL                    150
#define utGR_TU                       40
#define utGR_HP                       30
#define utGR_DN0                       2
#define utGR_DN1                       5
#define utGR_DN2                       2






// Xenon:

#define utXE_VEL                     140
#define utXE_VIS                       8
#define utXE_FUEL                    100
#define utXE_TU                       70
#define utXE_HP                       20
#define utXE_DN0                       7
#define utXE_DN1                       7
#define utXE_DN2                       7
#define utXE_MINES                    10
#define utXE_TL                       30

#define utMINE_AN                     17
#define utMINE_BN                    +25 
#define utMINE_AN2                    10
#define utMINE_BN2                   +10 






// Rex:

#define utRX_VEL                      90
#define utRX_VIS                      10
#define utRX_FUEL                    200
#define utRX_TU                       80
#define utRX_HP                       30
#define utRX_DN0                      10
#define utRX_DN1                      10
#define utRX_DN2                      10

#define utRX_AN                       17
#define utRX_ANB                      +5
#define utRX_AT                       30
#define utRX_MAXR                      3
#define utRX_AMMO                     20

#define utRX_2AN                      20    /* 2nd weapon */
#define utRX_2ANB                    +30
#define utRX_2AN2                     10
#define utRX_2ANB2                   +10
#define utRX_2AT                      40
#define utRX_2MINR                     2
#define utRX_2MAXR                     7
#define utRX_2AMMO                     6

#define utRX_INC_HP                    2
#define utRX_INC_DN                  1/2
#define utRX_INC_AN                  1/2
#define utRX_INC_2AN                   1

#define utRX_LAND                     40






// Falanga:

#define utFL_VEL                     100
#define utFL_VIS                      10
#define utFL_FUEL                     80
#define utFL_TU                       70
#define utFL_HP                       25
#define utFL_DN0                       6
#define utFL_DN1                       7
#define utFL_DN2                       6

#define utFL_AN                       17
#define utFL_ANB                      +5
#define utFL_AT                       30
#define utFL_MAXR                      8
#define utFL_AMMO                     20

#define utFL_2AN                       8    /* 2nd weapon */
#define utFL_2ANB                     +3
#define utFL_2AT                      15
#define utFL_2MAXR                     1
#define utFL_2AMMO                    30

#define utFL_INC_HP                    2
#define utFL_INC_DN                  1/3 
#define utFL_INC_AN                  1/2
#define utFL_INC_2AN                 1/2






// Horizont:

#define utHR_VIS                      13
#define utHR_TU                       50
#define utHR_HP                       30
#define utHR_DN0                       8
#define utHR_DN1                       9
#define utHR_DN2                       8

#define utHR_AN                       18
#define utHR_ANB                      +7
#define utHR_AT                       25
#define utHR_MAXR                     12
#define utHR_AMMO                    100

#define utHR_INC_HP                    2
#define utHR_INC_DN                  1/3 
#define utHR_INC_AN                  1/2








// Vyrobna jednotek:

#define utFA_VIS                       5
#define utFA_HP                      500
#define utFA_DN0                       6
#define utFA_DN1                       5
#define utFA_DN2                       7
#define utFA_MONEY                    10






// Letiste:

#define utAI_VIS                      15
#define utAI_HP                      500
#define utAI_DN0                       6
#define utAI_DN1                       5
#define utAI_DN2                       5
#define utAI_MONEY                     7






// Heliport:

#define utHP_VIS                      10
#define utHP_HP                      300
#define utHP_DN0                       7
#define utHP_DN1                       8
#define utHP_DN2                       6
#define utHP_MONEY                     3






// Ikaros:

#define utIK_VEL                      50
#define utIK_VIS                      15
#define utIK_FUEL                    120
#define utIK_TU                       90
#define utIK_HP                       20
#define utIK_DN0                       8
#define utIK_DN1                       8
#define utIK_DN2                       8

#define utIK_AN                       15
#define utIK_ANB                      +7
#define utIK_AT                       30
#define utIK_MAXR                      3
#define utIK_AMMO                     15

#define utIK_2AN                      15    /* 2nd weapon */
#define utIK_2ANB                    +30
#define utIK_2AT                      50
#define utIK_2MINR                     2
#define utIK_2MAXR                    10
#define utIK_2AMMO                     6

#define utIK_INC_HP                    2
#define utIK_INC_DN                  1/3
#define utIK_INC_AN                  1/2
#define utIK_INC_2AN                   1






// Kobra:

#define utKO_VEL                      90
#define utKO_VIS                      15
#define utKO_FUEL                    150
#define utKO_TU                       70
#define utKO_HP                       35
#define utKO_DN0                       6
#define utKO_DN1                       6
#define utKO_DN2                       6

#define utKO_AN                       15
#define utKO_ANB                      +3
#define utKO_AT                       30
#define utKO_MAXR                      3
#define utKO_AMMO                     20

#define utKO_2AN                      17    /* 2nd weapon */
#define utKO_2ANB                     +7
#define utKO_2AT                      30
#define utKO_2MAXR                     3
#define utKO_2AMMO                    20

#define utKO_3AN                      20    /* 3rd weapon */
#define utKO_3ANB                    +30
#define utKO_3AN2                      7
#define utKO_3ANB2                   +10
#define utKO_3AT                      30
#define utKO_3AMMO                     2

#define utKO_INC_HP                    3
#define utKO_INC_DN                  1/3
#define utKO_INC_AN                  1/2
#define utKO_INC_2AN                 1/2
#define utKO_INC_3AN                 2/3






// Caesar:

#define utCA_VEL                     140
#define utCA_VIS                      15
#define utCA_FUEL                    120
#define utCA_TU                       70
#define utCA_HP                       20
#define utCA_DN0                       6
#define utCA_DN1                       6
#define utCA_DN2                       6






// Saturn:

#define utSA_VEL                     170
#define utSA_VIS                      12
#define utSA_FUEL                    140
#define utSA_TU                       60
#define utSA_HP                       50
#define utSA_DN0                       5
#define utSA_DN1                       5
#define utSA_DN2                       5

#define utSA_BAN                      20    /* 3rd weapon */
#define utSA_BANB                    +30
#define utSA_BAT                      10
#define utSA_BAMMO                     6

#define utSA_INC_HP                    3
#define utSA_INC_DN                  1/3
#define utSA_INC_BAN                   1






// Medea:

#define utMD_VEL                     210
#define utMD_VIS                      10
#define utMD_FUEL                    120
#define utMD_TU                       80
#define utMD_HP                       30
#define utMD_DN0                       8
#define utMD_DN1                       8
#define utMD_DN2                       9
#define utMD_TIME                     20 






// Flint:

#define utFT_VEL                     100
#define utFT_VIS                      12
#define utFT_FUEL                    230
#define utFT_TU                       50
#define utFT_HP                       40
#define utFT_DN0                       7
#define utFT_DN1                       8
#define utFT_DN2                       8

#define utFT_AN                       15
#define utFT_ANB                      +3
#define utFT_AT                       30
#define utFT_MAXR                      4
#define utFT_AMMO                     20

#define utFT_INC_HP                    2
#define utFT_INC_DN                  1/3
#define utFT_INC_AN                  1/2






// Poseidon:

#define utPO_VEL                     200
#define utPO_VIS                      15
#define utPO_FUEL                    250
#define utPO_TU                       50
#define utPO_HP                      120
#define utPO_DN0                      11
#define utPO_DN1                      12
#define utPO_DN2                      11

#define utPO_AN                       25
#define utPO_ANB                     +20
#define utPO_AN2                       9
#define utPO_ANB2                    +15
#define utPO_MINR                      2
#define utPO_MAXR                     10
#define utPO_AMMO                     20
#define utPO_AT                       25
#define utPO_2AN                      22
#define utPO_2ANB                    +35
#define utPO_2MAXR                     7
#define utPO_2AMMO                     4
#define utPO_2AT                      30
#define utPO_3AN                      17
#define utPO_3ANB                     +5
#define utPO_3MAXR                     7
#define utPO_3AMMO                    50
#define utPO_3AT                      20

#define utPO_INC_HP                    7
#define utPO_INC_DN                  2/3
#define utPO_INC_AN                    1
#define utPO_INC_2AN                   1
#define utPO_INC_3AN                 1/2






// Kraken:

#define utKR_VEL                     230
#define utKR_VIS                      20
#define utKR_FUEL                    280
#define utKR_TU                       50
#define utKR_HP                      200
#define utKR_DN0                      12
#define utKR_DN1                      13
#define utKR_DN2                      13

#define utKR_AN                       35
#define utKR_ANB                     +25
#define utKR_AN2                      12
#define utKR_ANB2                    +15
#define utKR_MINR                      2
#define utKR_MAXR                     20
#define utKR_AMMO                     20
#define utKR_AT                       20
#define utKR_2AN                      22
#define utKR_2ANB                    +35
#define utKR_2MAXR                     7
#define utKR_2AMMO                     6
#define utKR_2AT                      30
#define utKR_3AN                      18
#define utKR_3ANB                     +5
#define utKR_3MAXR                     7
#define utKR_3AMMO                    50
#define utKR_3AT                      20

#define utKR_INC_HP                   12
#define utKR_INC_DN                    1
#define utKR_INC_AN                  3/2
#define utKR_INC_2AN                   1
#define utKR_INC_3AN                 1/2






// Oasa:

#define utOA_VEL                     170
#define utOA_VIS                      10
#define utOA_FUEL                    300
#define utOA_TU                       55
#define utOA_HP                       30 
#define utOA_DN0                       5
#define utOA_DN1                       5
#define utOA_DN2                       5

#define utOA_AMGN                    200
#define utOA_AMEX                    300
#define utOA_AMEN                    100
#define utOA_AMTO                     30
#define utOA_AMMI                     50







// Laguna:

#define utLA_VEL                     250
#define utLA_VIS                      30
#define utLA_FUEL                    270
#define utLA_TU                       40
#define utLA_HP                       40
#define utLA_DN0                       7
#define utLA_DN1                       7
#define utLA_DN2                       7

#define utLA_AN                       22
#define utLA_ANB                     +35
#define utLA_MAXR                      7
#define utLA_AMMO                      2
#define utLA_AT                       30

#define utLA_INC_HP                    2
#define utLA_INC_DN                  1/3
#define utLA_INC_AN                    1






// Iris:

#define utIR_VEL                     200
#define utIR_VIS                       6
#define utIR_FUEL                    100
#define utIR_TU                       80
#define utIR_HP                       15
#define utIR_DN0                       4
#define utIR_DN1                       5
#define utIR_DN2                       5






// Skiatron:

#define utSI_VEL                      60
#define utSI_VIS                      20
#define utSI_FUEL                    120
#define utSI_TU                       80
#define utSI_HP                       10
#define utSI_DN0                       4
#define utSI_DN1                       4
#define utSI_DN2                       4






// Spektrum:

#define utSP_VIS                      10
#define utSP_TU                       50
#define utSP_HP                       30
#define utSP_DN0                      15
#define utSP_DN1                       7
#define utSP_DN2                       7

#define utSP_AN                       20
#define utSP_ANB                     +25
#define utSP_AT                       50
#define utSP_MAXR                     14
#define utSP_AMMO                    100

#define utSP_INC_HP                    2
#define utSP_INC_DN                  1/3
#define utSP_INC_AN                    1






// Viking:

#define utVI_VEL                     140
#define utVI_VIS                      12
#define utVI_FUEL                    300
#define utVI_TU                       60
#define utVI_HP                       70
#define utVI_DN0                       9
#define utVI_DN1                      10
#define utVI_DN2                      10

#define utVI_AN                       20
#define utVI_ANB                     +10
#define utVI_AN2                      10
#define utVI_ANB2                     +5
#define utVI_MINR                      1
#define utVI_MAXR                      7
#define utVI_AMMO                     25
#define utVI_AT                       30
#define utVI_2AN                      10
#define utVI_2ANB                    +20
#define utVI_2MAXR                     1
#define utVI_2AMMO                    50
#define utVI_2AT                      20
#define utVI_3AN                      15
#define utVI_3ANB                     +3
#define utVI_3MAXR                     7
#define utVI_3AMMO                    50
#define utVI_3AT                      25

#define utVI_INC_HP                    3
#define utVI_INC_DN                  1/2
#define utVI_INC_AN                    1
#define utVI_INC_2AN                 2/3
#define utVI_INC_3AN                 1/2






// Opravna:

#define utRB_VIS                       8
#define utRB_HP                      350
#define utRB_DN0                       7
#define utRB_DN1                       6
#define utRB_DN2                       7
#define utRB_MONEY                     8





// Doky:

#define utDC_VIS                      10
#define utDC_HP                      450
#define utDC_DN0                       9
#define utDC_DN1                      10
#define utDC_DN2                       9
#define utDC_MONEY                    12





// Dirrac:

#define utDI_VEL                      90
#define utDI_VIS                      10
#define utDI_FUEL                     90
#define utDI_TU                       70
#define utDI_HP                       25
#define utDI_DN0                      17
#define utDI_DN1                       6
#define utDI_DN2                       6

#define utDI_AN                       17
#define utDI_ANB                     +25
#define utDI_AT                       50
#define utDI_MAXR                     11
#define utDI_AMMO                     10

#define utDI_INC_HP                    2
#define utDI_INC_DN                  1/3
#define utDI_INC_AN                    1





// Agni:

#define utAG_VEL                      90
#define utAG_VIS                      10
#define utAG_FUEL                    110
#define utAG_TU                       70
#define utAG_HP                       25
#define utAG_DN0                      15
#define utAG_DN1                       8
#define utAG_DN2                       8

#define utAG_AN                       17
#define utAG_ANB                     +30
#define utAG_AT                       50
#define utAG_MAXR                     10
#define utAG_AMMO                     20

#define utAG_INC_HP                    2
#define utAG_INC_DN                  1/3
#define utAG_INC_AN                    1





// Neptun:

#define utNE_VEL                     180
#define utNE_VIS                       8
#define utNE_FUEL                    250
#define utNE_TU                       80
#define utNE_HP                       15
#define utNE_DN0                       4
#define utNE_DN1                       5
#define utNE_DN2                       4






// Proton:

#define utPR_VEL                     130
#define utPR_VIS                       6
#define utPR_FUEL                    750
#define utPR_TU                       70
#define utPR_HP                        5
#define utPR_DN0                       8
#define utPR_DN1                       8
#define utPR_DN2                       8






// Barracuda:

#define utBA_VEL                     120
#define utBA_VIS                       6
#define utBA_FUEL                    250
#define utBA_TU                       60
#define utBA_HP                       30
#define utBA_DN0                      10
#define utBA_DN1                      10
#define utBA_DN2                      10

#define utBA_AN                       25
#define utBA_ANB                     +35
#define utBA_AT                       30
#define utBA_MAXR                      9
#define utBA_AMMO                     12

#define utBA_INC_HP                    3
#define utBA_INC_DN                  1/2
#define utBA_INC_AN                    2









// Elektrarna:

#define utPP_VIS                       3 
#define utPP_HP                       50
#define utPP_DN0                       4
#define utPP_DN1                       6
#define utPP_DN2                       6
#define utPP_MONEY                     1






// cecenci:

#define utH_VIS                        3
#define utH_HP                        20
#define utH_DN0                        3
#define utH_DN1                        3
#define utH_DN2                        3




// artefakti:

#define utAH_VIS                       3 
#define utAH_HP                      400
#define utAH_DN0                      10
#define utAH_DN1                       6
#define utAH_DN2                       6




// Ruzne destrukcni akce, sebedestrukce apod.:

#define utSUPPORT_DESTROY_AN          10
#define utSUPPORT_DESTROY_BN         +15

#define utBARREL_DESTROY_AN            9
#define utBARREL_DESTROY_BN           +7

#define utBUILDING_BOOM_AN            10
#define utBUILDING_BOOM_BN           +15


#endif
