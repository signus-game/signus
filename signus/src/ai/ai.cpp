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
// Artificial Intelligence of Signus
//
//       Local Intelligence
//
//        Object: TArmy
//
// Programmed by Richard Wunsch
//



#include "headers.h"
#include "ai.h"
#include "aiglobal.h"
#include "aitowers.h"
#include "aiground.h"
#include "consts.h"
#include "engine.h"





int ArtefactTrans;
int Base1, Base2, Base3, Base4, Base5, Base6, Base7, Base8;
int Radar1, Radar2, Radar3, Radar4, Radar5, Radar6, Radar7, Radar8, Radar9, Radar10, Radar11, Radar12, Radar13, Radar14, Radar15;
int Factory1, Factory2, Factory3, Factory4, Factory5, Factory6, Factory7, Factory8;
int Airport1, Airport2, Airport3, Airport4, Airport5, Airport6, Airport7, Airport8;
int Heliport1, Heliport2, Heliport3, Heliport4, Heliport5, Heliport6, Heliport7, Heliport8;
int Tower1, Tower2, Tower3, Tower4;
int Pagoda1, Pagoda2;
int Warehouse1, Warehouse2, Warehouse3, Warehouse4;
int Olymp1, Olymp2, Olymp3, Olymp4, Olymp5, Olymp6, Olymp7, Olymp8, Olymp9, Olymp10;
int Mohykan1, Mohykan2, Mohykan3, Mohykan4, Mohykan5;
int Uran1, Uran2, Uran3, Uran4, Uran5, Uran6, Uran7, Uran8, Uran9, Uran10, Uran11, Uran12, Uran13, Uran14, Uran15;
int Xenon1, Xenon2, Xenon3;
int Kobra1;
int Saturn1, Saturn2;
int Caesar1, Caesar2, Caesar3, Caesar4, Caesar5;
int Gargantua1, Gargantua2, Gargantua3;
int Laguna1, Laguna2, Laguna3;
int Oasa1, Oasa2, Oasa3, Oasa4, Oasa5, Oasa6, Oasa7;
int Neptun1, Neptun2, Neptun3, Neptun4, Neptun5;
int Docks1, Docks2, Docks3, Docks4, Docks5;
int Mesias1;
int Ganymedes1;

int ActivationTurn1, ActivationTurn2, ActivationTurn3, ActivationTurn4;
int ActivationTurn5, ActivationTurn6, ActivationTurn7, ActivationTurn8;
int ActivationTurn9, ActivationTurn10, ActivationTurn11, ActivationTurn12;
int ActivationTurn13, ActivationTurn14;

// General Factory List
int GFactoryList [] = {unBumerang, unPerseusAlfa, unPerseusBeta,
                                             unSkorpion, unPatriot,
                                             unCeres, unGnom, 0};


//////////////////////////// InitAI




////////////////// TArmy

TArmy::TArmy ()
{
  BL = (int *) memalloc (BADLIFE * sizeof (int));
  GL = (int *) memalloc (BADLIFE * sizeof (int));
  PoziceUtoku = (TPoint *) memalloc (BADLIFE * sizeof (TPoint));
  DangerArray = (double *) memalloc (BADLIFE * sizeof (double));

  Status = asSleeping;
  nofGoodLife = 0;
  nofBadLife = 0;
  velikost = 7;
  ProbHitTable = NULL;
  Attackers = NULL;
  Targets = NULL;
  WayPoint.x = -1; WayPoint.y = -1;
  SetAIConst (1);
}



TArmy::TArmy (FILE *f)
{

  BL = (int *) memalloc (BADLIFE * sizeof (int));
  GL = (int *) memalloc (BADLIFE * sizeof (int));
  PoziceUtoku = (TPoint *) memalloc (BADLIFE * sizeof (TPoint));
  DangerArray = (double *) memalloc (BADLIFE * sizeof (double));

  fread (&velikost, sizeof (int), 1, f);
  fread (&ActionRadius, sizeof (TRect), 1, f);
  fread (&Status, sizeof (int), 1, f);

  fread (&nofGoodLife, sizeof (int), 1, f);
  fread (GL, nofGoodLife * sizeof (int), 1, f);

  fread (&nofBadLife, sizeof (int), 1, f);
    fread (BL, nofBadLife * sizeof (int), 1, f);

  fread (&WayPoint, sizeof (TPoint), 1, f);
  fread (&RadarPos, sizeof (TPoint), 1, f);

  fread (&CDestroy, sizeof (double), 1, f);
    fread (&CBanzai, sizeof (double), 1, f);
    fread (&CTogether, sizeof (double), 1, f);
    fread (&CToThem, sizeof (double), 1, f);
    fread (&CToThemMin, sizeof (double), 1, f);
    fread (&CLazy, sizeof (double), 1, f);
    fread (&CDist, sizeof (double), 1, f);
    fread (&CPropexDist, sizeof (double), 1, f);
    fread (&CDanger, sizeof (double), 1, f);
    fread (&CContact, sizeof (double), 1, f);
    fread (&CCapture, sizeof (double), 1, f);
    fread (&CToWayPoint, sizeof (double), 1, f);
    fread (&CFuel, sizeof (double), 1, f);


  ProbHitTable = NULL;
  Attackers = NULL;
  Targets = NULL;

}



void TArmy::Save (FILE *f)
{
    DeleteKilled2 ();
    DeleteKilled ();
    fwrite (&velikost, sizeof (int), 1, f);
  fwrite (&ActionRadius, sizeof (TRect), 1, f);
  fwrite (&Status, sizeof (int), 1, f);

    fwrite (&nofGoodLife, sizeof (int), 1, f);
    fwrite (GL, nofGoodLife * sizeof (int), 1, f);

    fwrite (&nofBadLife, sizeof (int), 1, f);
    fwrite (BL, nofBadLife * sizeof (int), 1, f);

    fwrite (&WayPoint, sizeof (TPoint), 1, f);
    fwrite (&RadarPos, sizeof (TPoint), 1, f);

  fwrite (&CDestroy, sizeof (double), 1, f);
    fwrite (&CBanzai, sizeof (double), 1, f);
    fwrite (&CTogether, sizeof (double), 1, f);
    fwrite (&CToThem, sizeof (double), 1, f);
    fwrite (&CToThemMin, sizeof (double), 1, f);
    fwrite (&CLazy, sizeof (double), 1, f);
    fwrite (&CDist, sizeof (double), 1, f);
    fwrite (&CPropexDist, sizeof (double), 1, f);
    fwrite (&CDanger, sizeof (double), 1, f);
    fwrite (&CContact, sizeof (double), 1, f);
    fwrite (&CCapture, sizeof (double), 1, f);
    fwrite (&CToWayPoint, sizeof (double), 1, f);
    fwrite (&CFuel, sizeof (double), 1, f);
}


void TArmy::Insert (int Unit)
{
  int i, j, t;

    if (Unit < 0 || Unit >= UNITS_TOP) {
#ifdef DEBUG
        fprintf (dbgOutput, "Error: Insert Unit = %i \n",Unit);
#endif
        AIError ();
        return;
    }
    if (Units [Unit] == NULL) {
#ifdef DEBUG
        fprintf (dbgOutput, "Error: Insert Unit %i (=NULL) \n",Unit);
#endif
        AIError ();
        return;
    }

  if (Unit < BADLIFE) {
    t = FALSE;
    for (j = 0; j < nofGoodLife; j++) {
        if (GL [j] == Unit) t = TRUE;
    }
    if (!t) {
        GL [nofGoodLife] = Unit;
        Units [Unit] -> AI_Info.State = aistNought;
        nofGoodLife++;
    }
  }
  if (Unit >= BADLIFE) {
    if (Units [Unit] -> Type % BADLIFE >= unRadar) return; //!!!!!!!!!!!!!!!!!!!!!!!!!!
    t = FALSE;
    for (j = 0; j < nofBadLife; j++) {
        if (BL [j] == Unit) t = TRUE;
    }
    if (!t) {
        BL [nofBadLife] = Unit;
        Units [BL [nofBadLife]] -> AI_Info.State = aistNought;
        nofBadLife++;
    }
  }

}





int TArmy::MakeTurn ()
{
    return TRUE;
}



int TArmy::ProblemJednotky (int Unit)
{
    return (0);
}



void TArmy::MakeAttackPoints (int UnID)
{
    int r, n, i, j, weapon;
    TPoint p;
    TUnit *Unit;

    // vynulujeme pole
    for (i = 0; i < nofGoodLife; i++) {
        PoziceUtoku [i].x = MaxInt;
        PoziceUtoku [i].y = MaxInt;
    }


    if (UnID < 0 || UnID >= UNITS_TOP) {
#ifdef DEBUG
        fprintf (dbgOutput, "Error: MakeAttackPoinst Unit = %i \n",UnID);
#endif
        AIError ();
        return;
    }

    if (Units [UnID] == NULL) {
#ifdef DEBUG
        fprintf (dbgOutput, "Error: MakeAttackPoinst Unit %i (=NULL) \n",UnID);
#endif
        AIError ();
        return;
    }

    if (Units [UnID] -> Type % BADLIFE >= unRadar) {
#ifdef DEBUG
        fprintf (dbgOutput, "Error: MakeAttackPoinst Unit %i not TUnit \n",UnID);
#endif
        AIError ();
        return;
    }

    Unit = (TUnit *)Units [UnID];
    for (i = 0; i < nofGoodLife; i++) { //pro vsechny Goodlife
        weapon = ChooseWeapon (UnID, GL [i]);
        if (weapon == -1) continue;
        r = (Unit -> TimeUnits - Unit -> Weapons [weapon] -> TimeLost) / 5;
        n = 4*(r*r + r);
        for (j = 0; j <= n; j++) {
            p = CartezianSnail (j);
            p.x += Unit -> X; p.y += Unit -> Y;
            if (p.x < 0 || p.x >= MapSizeX || p.y < 0 || p.y >= MapSizeY) continue;
            // Mame dost casu?
            if (Unit -> TimeUnits - Unit -> TimeToMove (p.x, p.y)
            < Unit -> Weapons [weapon] -> TimeLost) continue;
            // Je cil odsud na dostrel?
            if (!Unit -> Weapons [weapon] -> IsInRangeAI (Unit, p.x, p.y
            , Units [GL [i]] -> X, Units [GL [i]] -> Y, FALSE))
                continue;
            // vsechno OK - pozice nalezena
            PoziceUtoku [i].x = p.x;
            PoziceUtoku [i].y = p.y;
            break;
        }
    }

}



TPoint TArmy::BestPlace (TPoint start, int UnID, int Time)
{
    TUnit *Unit;
    TPoint a, besta;
    int r, i, x, y, t;
    TPoint OldPos;
    double st, bestst;
    int rx, ry, drawx, drawy, ak;
    TField *f;
    char str[5];

    if (UnID < 0 || UnID >= UNITS_TOP) {
#ifdef DEBUG
        fprintf (dbgOutput, "Error: BestPlace UnID = %i \n",UnID);
#endif
        AIError ();
        return start;
    }

    if (Units [UnID] == NULL) {
#ifdef DEBUG
        fprintf (dbgOutput, "Error: BestPlace UnID %i (=NULL) \n",UnID);
#endif
        AIError ();
        return start;
    }
    if (Units [UnID] -> Type % BADLIFE >= unRadar) {
#ifdef DEBUG
        fprintf (dbgOutput, "Error: BestPlace UnID %i not TUnit \n",UnID);
#endif
        AIError ();
        return start;
    }
    if (start.x < 0 || start.x >=MapSizeX || start.y < 0 || start.y >= MapSizeY) {
#ifdef DEBUG
        fprintf (dbgOutput, "Error: BestPlace UnID %i start out of Map \n",UnID);
#endif
        AIError ();
        return start;
    }



    Unit = (TUnit *)Units [UnID];
    OldPos.x = Unit -> X, OldPos.y = Unit -> Y;
    Unit -> PlaceGround (FALSE);
    Unit -> X = start.x, Unit -> Y = start.y;
    Unit -> PlaceGround (TRUE);
    Unit -> Select();

    r = Time / 5;
    bestst = MaxInt;
    besta.x = Unit -> X;
    besta.y = Unit -> Y;
    for (x = -r; x <= r; x++) {
        for (y = -r; y <= r; y++) { // Range
            t = Unit -> TimeToMove (x + Unit -> X, y + Unit -> Y);
            if ((t < Time)  && (t >= 0)) {
                a.x = x + Unit -> X;
                a.y = y + Unit -> Y;
                st = PointStatus (UnID, a);
                if (st < bestst) {
                    bestst = st;
                    besta = a;
                }
            }
        }
    }

    Unit -> PlaceGround (FALSE);
    Unit -> X = OldPos.x, Unit -> Y = OldPos.y;
    Unit -> PlaceGround (TRUE);
    Unit -> Select();

    return (besta);
}


//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
int TArmy::ChooseWeapon (int attackerID, int defenderID)
{
    TUnit *attacker;
    TObject *defender;

    if (attackerID < 0 || attackerID >= UNITS_TOP) {
#ifdef DEBUG
        fprintf (dbgOutput, "Error: ChooseWeapon attackerID = %i \n",attackerID);
#endif
        AIError ();
        return -1;
    }
    if (Units [attackerID] == NULL) {
#ifdef DEBUG
        fprintf (dbgOutput, "Error: ChooseWeapon attacker %i (=NULL) \n",attackerID);
#endif
        AIError ();
        return -1;
    }
    if (Units [attackerID] -> Type % BADLIFE >= unRadar) {
#ifdef DEBUG
        fprintf (dbgOutput, "Error: ChooseWeapon attacker %i not TUnit \n",attackerID);
#endif
        AIError ();
        return -1;
    }
    attacker = (TUnit *)Units [attackerID];

    if (defenderID < 0 || defenderID >= UNITS_TOP) {
#ifdef DEBUG
        fprintf (dbgOutput, "Error: ChooseWeapon defenderID = %i \n",defenderID);
#endif
        AIError ();
        return -1;
    }
    if (Units [defenderID] == NULL) {
#ifdef DEBUG
        fprintf (dbgOutput, "Error: ChooseWeapon defender %i (=NULL) \n",defenderID);
#endif
        AIError ();
        return -1;
    }
    defender = Units [defenderID];

    switch (attacker -> Type % BADLIFE) {
        case 1 : { //PerseusAlfa  OK
            // Pozemni, podpurne, vlaky, vznasedla, veze, lode mimo ponorku
            if (defender -> Type % BADLIFE >= unPerseusAlfa
            && defender -> Type % BADLIFE <= unLaguna
            && attacker -> Weapons [0] -> Ammo > 0) return 0;
            // Satan a baraky
            if (defender -> Type % BADLIFE >= unSatan
            && defender -> Type % BADLIFE <= unWarehouse
            && attacker -> Weapons [0] -> Ammo > 0) return 0;
            // Letadla na zemi
            if ((defender -> Type % BADLIFE == unCaesar
            || defender -> Type % BADLIFE == unRex)
            && attacker -> Weapons [0] -> Ammo > 0
            && defender -> Alt != 8) return 0;
            // jine jednotky
            return -1;
            break;
        }
        case 2 : { //PerseusBeta OK
            // Pozemni, podpurne, vlaky, vznasedla, veze, lode mimo ponorku
            if (defender -> Type % BADLIFE >= unPerseusAlfa
            && defender -> Type % BADLIFE <= unLaguna
            && attacker -> Weapons [0] -> Ammo > 0) return 0;
            // Satan a baraky
            if (defender -> Type % BADLIFE >= unSatan
            && defender -> Type % BADLIFE <= unWarehouse
            && attacker -> Weapons [0] -> Ammo > 0) return 0;
            // Letadla na zemi
            if ((defender -> Type % BADLIFE == unCaesar
            || defender -> Type % BADLIFE == unRex)
            && attacker -> Weapons [0] -> Ammo > 0
            && defender -> Alt != 8) return 0;
            // jine jednotky
            return -1;
            break;
        }
        case 3 : { // PerseusGama OK
            // Pozemni, podpurne, vlaky, vznasedla, veze, lode mimo ponorku
            if (defender -> Type % BADLIFE >= unPerseusAlfa
            && defender -> Type % BADLIFE <= unLaguna
            && attacker -> Weapons [0] -> Ammo > 0) return 0;
            // Satan a baraky
            if (defender -> Type % BADLIFE >= unSatan
            && defender -> Type % BADLIFE <= unWarehouse
            && attacker -> Weapons [0] -> Ammo > 0) return 0;
            // Letadla na zemi
            if ((defender -> Type % BADLIFE == unCaesar
            || defender -> Type % BADLIFE == unRex)
            && attacker -> Weapons [0] -> Ammo > 0
            && defender -> Alt != 8) return 0;
            // Letadla
            if (defender -> Type % BADLIFE >= unIkaros
            && defender -> Type % BADLIFE <= unRex
            && attacker -> Weapons [1] -> Ammo > 0
            && defender -> Alt == 8) return 1;
            // jine jednotky
            return -1;
            break;
        }
        case 4 : { //Dirrac OK
            // Pozemni, podpurne, vlaky, vznasedla, veze, lode mimo ponorku
            if (defender -> Type % BADLIFE >= unPerseusAlfa
            && defender -> Type % BADLIFE <= unLaguna
            && attacker -> Weapons [0] -> Ammo > 0) return 0;
            // Satan a baraky
            if (defender -> Type % BADLIFE >= unSatan
            && defender -> Type % BADLIFE <= unWarehouse
            && attacker -> Weapons [0] -> Ammo > 0) return 0;
            // Letadla na zemi
            if ((defender -> Type % BADLIFE == unCaesar
            || defender -> Type % BADLIFE == unRex)
            && attacker -> Weapons [0] -> Ammo > 0
            && defender -> Alt != 8) return 0;
            // jine jednotky
            return -1;
            break;
        }
        case 5 : { //Agni OK
            // Pozemni, podpurne, vlaky, vznasedla, veze, lode mimo ponorku
            if (defender -> Type % BADLIFE >= unPerseusAlfa
            && defender -> Type % BADLIFE <= unLaguna
            && attacker -> Weapons [0] -> Ammo > 0) return 0;
            // Satan a baraky
            if (defender -> Type % BADLIFE >= unSatan
            && defender -> Type % BADLIFE <= unWarehouse
            && attacker -> Weapons [0] -> Ammo > 0) return 0;
            // Letadla na zemi
            if ((defender -> Type % BADLIFE == unCaesar
            || defender -> Type % BADLIFE == unRex)
            && attacker -> Weapons [0] -> Ammo > 0
            && defender -> Alt != 8) return 0;
            // jine jednotky
            return -1;
            break;
        }
        case 6 : { //Skorpion OK
            // Pozemni, podpurne, vlaky, vznasedla, veze, lode mimo ponorku
            if (defender -> Type % BADLIFE >= unPerseusAlfa
            && defender -> Type % BADLIFE <= unLaguna
            && attacker -> Weapons [0] -> Ammo > 0) return 0;
            // Satan a baraky
            if (defender -> Type % BADLIFE >= unSatan
            && defender -> Type % BADLIFE <= unWarehouse
            && attacker -> Weapons [0] -> Ammo > 0) return 0;
            // Letadla na zemi
            if ((defender -> Type % BADLIFE == unCaesar
            || defender -> Type % BADLIFE == unRex)
            && attacker -> Weapons [0] -> Ammo > 0
            && defender -> Alt != 8) return 0;
            // jine jednotky
            return -1;
            break;
        }
        case 7 : { //Herkules OK
            // Pozemni, podpurne, vlaky, vznasedla, veze, lode mimo ponorku
            if (defender -> Type % BADLIFE >= unPerseusAlfa
            && defender -> Type % BADLIFE <= unLaguna
            && attacker -> Weapons [0] -> Ammo > 0) return 0;
            // Satan a baraky
            if (defender -> Type % BADLIFE >= unSatan
            && defender -> Type % BADLIFE <= unWarehouse
            && attacker -> Weapons [0] -> Ammo > 0) return 0;
            // Letadla na zemi
            if ((defender -> Type % BADLIFE == unCaesar
            || defender -> Type % BADLIFE == unRex)
            && attacker -> Weapons [0] -> Ammo > 0
            && defender -> Alt != 8) return 0;
            // jine jednotky
            return -1;
            break;
        }
        case 8 : { //Falanga OK
            // Pozemni, podpurne, vlaky, vznasedla, veze, lode mimo ponorku
            if (defender -> Type % BADLIFE >= unPerseusAlfa
            && defender -> Type % BADLIFE <= unLaguna
            && attacker -> Weapons [1] -> Ammo > 0) return 1;
            // Satan a baraky
            if (defender -> Type % BADLIFE >= unSatan
            && defender -> Type % BADLIFE <= unWarehouse
            && attacker -> Weapons [1] -> Ammo > 0) return 1;
            // Letadla na zemi
            if ((defender -> Type % BADLIFE == unCaesar
            || defender -> Type % BADLIFE == unRex)
            && attacker -> Weapons [1] -> Ammo > 0
            && defender -> Alt != 8) return 1;
            // letadla
            if (defender -> Type % BADLIFE >= unIkaros
            && defender -> Type % BADLIFE <= unRex
            && attacker -> Weapons [0] -> Ammo > 0
            && defender -> Alt == 8) return 0;
            // jine jednotky
            return -1;
            break;
        }
        case 9 : { //Golias OK
            // Pozemni, podpurne, vlaky, vznasedla, veze, lode mimo ponorku
            if (defender -> Type % BADLIFE >= unPerseusAlfa
            && defender -> Type % BADLIFE <= unLaguna
            && attacker -> Weapons [0] -> Ammo > 0) return 0;
            // Satan a baraky
            if (defender -> Type % BADLIFE >= unSatan
            && defender -> Type % BADLIFE <= unWarehouse
            && attacker -> Weapons [0] -> Ammo > 0) return 0;
            // Letadla na zemi
            if ((defender -> Type % BADLIFE == unCaesar
            || defender -> Type % BADLIFE == unRex)
            && attacker -> Weapons [0] -> Ammo > 0
            && defender -> Alt != 8) return 0;
            // jine jednotky
            return -1;
            break;
        }
        case 10 : { //Patriot OK
            // Pozemni, podpurne, vlaky, vznasedla, veze, lode mimo ponorku
            if (defender -> Type % BADLIFE >= unPerseusAlfa
            && defender -> Type % BADLIFE <= unLaguna
            && attacker -> Weapons [0] -> Ammo > 0) return 0;
            // Satan a baraky
            if (defender -> Type % BADLIFE >= unSatan
            && defender -> Type % BADLIFE <= unWarehouse
            && attacker -> Weapons [0] -> Ammo > 0) return 0;
            // Letadla na zemi
            if ((defender -> Type % BADLIFE == unCaesar
            || defender -> Type % BADLIFE == unRex)
            && attacker -> Weapons [0] -> Ammo > 0
            && defender -> Alt != 8) return 0;
            // jine jednotky
            return -1;
            break;
        }
        case 11 : { //Bumerang OK
            // Pozemni, podpurne, vlaky, vznasedla, veze, lode mimo ponorku
            if (defender -> Type % BADLIFE >= unPerseusAlfa
            && defender -> Type % BADLIFE <= unLaguna
            && attacker -> Weapons [0] -> Ammo > 0) return 0;
            // Satan a baraky
            if (defender -> Type % BADLIFE >= unSatan
            && defender -> Type % BADLIFE <= unWarehouse
            && attacker -> Weapons [0] -> Ammo > 0) return 0;
            // Letadla na zemi
            if ((defender -> Type % BADLIFE == unCaesar
            || defender -> Type % BADLIFE == unRex)
            && attacker -> Weapons [0] -> Ammo > 0
            && defender -> Alt != 8) return 0;
            // jine jednotky
            return -1;
            break;
        }
        case 12 : { //Skiatron
            return -1;
            break;
        }
        case 13 : { //Xenon
            return -1;
            break;
        }
        case 14 : { //Uran
            return -1;
            break;
        }
        case 15 : { //Gragantua
            return -1;
            break;
        }
        case 16 : { //Medea
            return -1;
            break;
        }
        case 17 : { //Ceres
            return -1;
            break;
        }
        case 18 : { //Gnom
            return -1;
            break;
        }
        case 19 : { //Iris
            return -1;
            break;
        }
        case 20 : { //Horizont OK
            if (defender -> Type % BADLIFE >= unIkaros
            && defender -> Type % BADLIFE <= unRex
            && attacker -> Weapons [0] -> Ammo > 0
            && defender -> Alt == 8) return 0;
            return -1;
            break;
        }
        case 21 : { //Pagoda OK
            // Pozemni, podpurne, vlaky, vznasedla, veze, lode mimo ponorku
            if (defender -> Type % BADLIFE >= unPerseusAlfa
            && defender -> Type % BADLIFE <= unLaguna
            && attacker -> Weapons [0] -> Ammo > 0) return 0;
            // Satan a baraky
            if (defender -> Type % BADLIFE >= unSatan
            && defender -> Type % BADLIFE <= unWarehouse
            && attacker -> Weapons [0] -> Ammo > 0) return 0;
            // Letadla na zemi
            if ((defender -> Type % BADLIFE == unCaesar
            || defender -> Type % BADLIFE == unRex)
            && attacker -> Weapons [0] -> Ammo > 0
            && defender -> Alt != 8) return 0;
            // jine jednotky
            return -1;
            break;
        }
        case 22 : { //Minotaurus OK
            // Pozemni, podpurne, vlaky, vznasedla, veze, lode mimo ponorku
            if (defender -> Type % BADLIFE >= unPerseusAlfa
            && defender -> Type % BADLIFE <= unLaguna
            && attacker -> Weapons [0] -> Ammo > 0) return 0;
            // Satan a baraky
            if (defender -> Type % BADLIFE >= unSatan
            && defender -> Type % BADLIFE <= unWarehouse
            && attacker -> Weapons [0] -> Ammo > 0) return 0;
            // Letadla na zemi
            if ((defender -> Type % BADLIFE == unCaesar
            || defender -> Type % BADLIFE == unRex)
            && attacker -> Weapons [0] -> Ammo > 0
            && defender -> Alt != 8) return 0;
            // jine jednotky
            return -1;
            break;
        }
        case 23 : { //Thor OK
            // Pozemni, podpurne, vlaky, vznasedla, veze, lode mimo ponorku
            if (defender -> Type % BADLIFE >= unPerseusAlfa
            && defender -> Type % BADLIFE <= unLaguna)
                return 0;
            // Satan a baraky
            if (defender -> Type % BADLIFE >= unSatan
            && defender -> Type % BADLIFE <= unWarehouse)
            return 0;
            // Letadla na zemi
            if ((defender -> Type % BADLIFE == unCaesar
            || defender -> Type % BADLIFE == unRex)
            && attacker -> Weapons [0] -> Ammo > 0
            && defender -> Alt != 8) return 0;
            // jine jednotky
            return -1;
            break;
        }
        case 24 : { //Spektrum OK
            // Pozemni, podpurne, vlaky, vznasedla, veze, lode mimo ponorku
            if (defender -> Type % BADLIFE >= unPerseusAlfa
            && defender -> Type % BADLIFE <= unLaguna
            && attacker -> Weapons [0] -> Ammo > 0) return 0;
            // Satan a baraky
            if (defender -> Type % BADLIFE >= unSatan
            && defender -> Type % BADLIFE <= unWarehouse
            && attacker -> Weapons [0] -> Ammo > 0) return 0;
            // Letadla na zemi
            if ((defender -> Type % BADLIFE == unCaesar
            || defender -> Type % BADLIFE == unRex)
            && attacker -> Weapons [0] -> Ammo > 0
            && defender -> Alt != 8) return 0;
            // jine jednotky
            return -1;
            break;
        }
        case 25 : { //Neptun
            return -1;
            break;
        }
        case 26 : { //Helios
            // Pozemni, podpurne, vlaky, vznasedla, veze, lode mimo ponorku
            if (defender -> Type % BADLIFE >= unPerseusAlfa
            && defender -> Type % BADLIFE <= unLaguna
            && attacker -> Weapons [0] -> Ammo > 0) return 0;
            // Satan a baraky
            if (defender -> Type % BADLIFE >= unSatan
            && defender -> Type % BADLIFE <= unWarehouse
            && attacker -> Weapons [0] -> Ammo > 0) return 0;
            // Letadla na zemi
            if ((defender -> Type % BADLIFE == unCaesar
            || defender -> Type % BADLIFE == unRex)
            && attacker -> Weapons [0] -> Ammo > 0
            && defender -> Alt != 8) return 0;
            // Letadla
            if (defender -> Type % BADLIFE >= unIkaros
            && defender -> Type % BADLIFE <= unRex
            && attacker -> Weapons [1] -> Ammo > 0
            && defender -> Alt == 8) return 1;
            return -1;
            break;
        }
        case 27 : { //Olymp
            return -1;
            break;
        }
        case 28 : { //Mohykan
            // Pozemni, podpurne, vlaky, vznasedla, veze, lode mimo ponorku
            if (defender -> Type % BADLIFE >= unPerseusAlfa
            && defender -> Type % BADLIFE <= unLaguna
            && attacker -> Weapons [0] -> Ammo > 0) return 0;
            // Satan a baraky
            if (defender -> Type % BADLIFE >= unSatan
            && defender -> Type % BADLIFE <= unWarehouse
            && attacker -> Weapons [0] -> Ammo > 0) return 0;
            // Letadla na zemi
            if ((defender -> Type % BADLIFE == unCaesar
            || defender -> Type % BADLIFE == unRex)
            && attacker -> Weapons [0] -> Ammo > 0
            && defender -> Alt != 8) return 0;
            // jine jednotky
            return -1;
            break;
        }
        case 29 : { //Mesias
            // Pozemni, podpurne, vlaky, vznasedla, veze, lode mimo ponorku
            if (defender -> Type % BADLIFE >= unPerseusAlfa
            && defender -> Type % BADLIFE <= unLaguna
            && attacker -> Weapons [0] -> Ammo > 0) return 0;
            // Satan a baraky
            if (defender -> Type % BADLIFE >= unSatan
            && defender -> Type % BADLIFE <= unWarehouse
            && attacker -> Weapons [0] -> Ammo > 0) return 0;
            // Letadla na zemi
            if ((defender -> Type % BADLIFE == unCaesar
            || defender -> Type % BADLIFE == unRex)
            && attacker -> Weapons [0] -> Ammo > 0
            && defender -> Alt != 8) return 0;
            // jine jednotky
            return -1;
            break;
        }
        case 30 : { //Ganymedes
            return -1;
            break;
        }
        case 31 : { //Flint
            // Pozemni, podpurne, vlaky, vznasedla, veze, lode mimo ponorku
            if (defender -> Type % BADLIFE >= unPerseusAlfa
            && defender -> Type % BADLIFE <= unLaguna
            && attacker -> Weapons [0] -> Ammo > 0) return 0;
            // Satan a baraky
            if (defender -> Type % BADLIFE >= unSatan
            && defender -> Type % BADLIFE <= unWarehouse
            && attacker -> Weapons [0] -> Ammo > 0) return 0;
            // Letadla na zemi
            if ((defender -> Type % BADLIFE == unCaesar
            || defender -> Type % BADLIFE == unRex)
            && attacker -> Weapons [0] -> Ammo > 0
            && defender -> Alt != 8) return 0;
            // jine jednotky
            return -1;
            break;
        }
        case 32 : { //Viking
            // Pozemni, podpurne, vlaky, vznasedla, veze, lode mimo ponorku
            if (defender -> Type % BADLIFE >= unPerseusAlfa
            && defender -> Type % BADLIFE <= unLaguna
            && attacker -> Weapons [0] -> Ammo > 0) return 0;
            // Satan a baraky
            if (defender -> Type % BADLIFE >= unSatan
            && defender -> Type % BADLIFE <= unWarehouse
            && attacker -> Weapons [0] -> Ammo > 0) return 0;
            // Letadla na zemi
            if ((defender -> Type % BADLIFE == unCaesar
            || defender -> Type % BADLIFE == unRex)
            && attacker -> Weapons [0] -> Ammo > 0
            && defender -> Alt != 8) return 0;
            // Barracuda - hlubinna mina
            if (defender -> Type % BADLIFE == unBarracuda
            && attacker -> Weapons [1] -> Ammo > 0) return 1;
            // Letadla
            if (defender -> Type % BADLIFE >= unIkaros
            && defender -> Type % BADLIFE <= unRex
            && attacker -> Weapons [2] -> Ammo > 0
            && defender -> Alt == 8) return 2;
            // jine jednotky
            return -1;
            break;
        }
        case 33 : { //Poseidon
            // Barracuda
            if (defender -> Type % BADLIFE == unBarracuda
            && attacker -> Weapons [1] -> Ammo > 0) return 1;
            // Pozemni, podpurne, vlaky, vznasedla, veze, lode mimo ponorku
            if (defender -> Type % BADLIFE >= unPerseusAlfa
            && defender -> Type % BADLIFE <= unLaguna
            && attacker -> Weapons [0] -> Ammo > 0) return 0;
            // Satan a baraky
            if (defender -> Type % BADLIFE >= unSatan
            && defender -> Type % BADLIFE <= unWarehouse) return 0;
            // Letadla na zemi
            if ((defender -> Type % BADLIFE == unCaesar
            || defender -> Type % BADLIFE == unRex)
            && attacker -> Weapons [0] -> Ammo > 0
            && defender -> Alt != 8) return 0;
            // Letadla
            if (defender -> Type % BADLIFE >= unIkaros
            && defender -> Type % BADLIFE <= unRex
            && attacker -> Weapons [2] -> Ammo > 0
            && defender -> Alt == 8) return 2;
            // jine jednotky
            return -1;
            break;
        }
        case 34 : { //Kraken
            // Barracuda
            if (defender -> Type % BADLIFE == unBarracuda
            && attacker -> Weapons [1] -> Ammo > 0) return 1;
            // Pozemni, podpurne, vlaky, vznasedla, veze, lode mimo ponorku
            if (defender -> Type % BADLIFE >= unPerseusAlfa
            && defender -> Type % BADLIFE <= unLaguna
            && attacker -> Weapons [0] -> Ammo > 0) return 0;
            // Satan a baraky
            if (defender -> Type % BADLIFE >= unSatan
            && defender -> Type % BADLIFE <= unWarehouse) return 0;
            // Letadla na zemi
            if ((defender -> Type % BADLIFE == unCaesar
            || defender -> Type % BADLIFE == unRex)
            && attacker -> Weapons [0] -> Ammo > 0
            && defender -> Alt != 8) return 0;
            // Letadla
            if (defender -> Type % BADLIFE >= unIkaros
            && defender -> Type % BADLIFE <= unRex
            && attacker -> Weapons [2] -> Ammo > 0
            && defender -> Alt == 8) return 2;
            // jine jednotky
            return -1;
            break;
        }
        case 35 : { //Oasa
            return -1;
            break;
        }
        case 36 : { //Laguna
            // Letadla
            if (defender -> Type % BADLIFE >= unIkaros
            && defender -> Type % BADLIFE <= unRex
            && attacker -> Weapons [0] -> Ammo > 0
            && defender -> Alt == 8) return 0;
            // jine jednotky
            return -1;
            break;
        }
        case 37 : { //Barakuda
            // Torpeda na lode krome Flinta
            if (defender -> Type % BADLIFE >= unFlint
            && defender -> Type % BADLIFE <= unBarracuda
            && attacker -> Weapons [0] -> Ammo > 0) return 0;
            // jine jednotky
            return -1;
            break;
        }
        case 38 : { //Ikaros
            // rakety
            if (defender -> Type % BADLIFE >= unIkaros
            && defender -> Type % BADLIFE <= unRex
            && attacker -> Weapons [1] -> Ammo > 0
            && defender -> Alt == 8) return 1;
            // kulomet
            if (defender -> Type % BADLIFE >= unIkaros
            && defender -> Type % BADLIFE <= unRex
            && attacker -> Weapons [0] -> Ammo > 0
            && defender -> Alt == 8) return 0;
            // jine jednotky
            return -1;
            break;
        }
        case 39 : { //Kobra
            // kulomet AA
            if (defender -> Type % BADLIFE >= unIkaros
            && defender -> Type % BADLIFE <= unRex
            && attacker -> Weapons [0] -> Ammo > 0
            && defender -> Alt == 8) return 0;
            // Pozemni, podpurne, vlaky, vznasedla, veze, lode mimo ponorku - AGcan
            if (defender -> Type % BADLIFE >= unPerseusAlfa
            && defender -> Type % BADLIFE <= unLaguna
            && attacker -> Weapons [1] -> Ammo > 0) return 1;
            // Satan a baraky - bomby
            if (defender -> Type % BADLIFE >= unSatan
            && defender -> Type % BADLIFE <= unWarehouse
            && attacker -> Weapons [1] -> Ammo > 0) return 1;
            // Letadla na zemi
            if ((defender -> Type % BADLIFE == unCaesar
            || defender -> Type % BADLIFE == unRex)
            && attacker -> Weapons [1] -> Ammo > 0
            && defender -> Alt != 8) return 1;
            // jine jednotky
            return -1;
            break;
        }
        case 40 : { //Saturn
            return -1;
            break;
        }
        case 41 : { //Mystik
            // rakety AA
            if (defender -> Type % BADLIFE >= unIkaros
            && defender -> Type % BADLIFE <= unRex
            && attacker -> Weapons [1] -> Ammo > 0
            && defender -> Alt == 8) return 1;
            // kulomet AA
            if (defender -> Type % BADLIFE >= unIkaros
            && defender -> Type % BADLIFE <= unRex
            && attacker -> Weapons [0] -> Ammo > 0
            && defender -> Alt == 8) return 0;
            // Pozemni, podpurne, vlaky, vznasedla, veze, lode mimo ponorku - rakety AG
            if (defender -> Type % BADLIFE >= unPerseusAlfa
            && defender -> Type % BADLIFE <= unLaguna
            && attacker -> Weapons [2] -> Ammo > 0) return 2;
            // Satan a baraky - bomby
            if (defender -> Type % BADLIFE >= unSatan
            && defender -> Type % BADLIFE <= unWarehouse
            && attacker -> Weapons [2] -> Ammo > 0) return 2;
            // Letadla na zemi
            if ((defender -> Type % BADLIFE == unCaesar
            || defender -> Type % BADLIFE == unRex)
            && attacker -> Weapons [2] -> Ammo > 0
            && defender -> Alt != 8) return 2;
            // jine jednotky
            return -1;

            break;
        }
        case 42 : { //Cesar
            return -1;
            break;
        }
        case 43 : { //Proton
            return -1;
            break;
        }
        case 44 : { //Rex
            // Pozemni, podpurne, vlaky, vznasedla, veze, lode mimo ponorku
            if (defender -> Type % BADLIFE >= unPerseusAlfa
            && defender -> Type % BADLIFE <= unLaguna
            && attacker -> Weapons [0] -> Ammo > 0) return 0;
            // Satan a baraky
            if (defender -> Type % BADLIFE >= unSatan
            && defender -> Type % BADLIFE <= unWarehouse
            && attacker -> Weapons [0] -> Ammo > 0) return 0;
            // Letadla na zemi
            if ((defender -> Type % BADLIFE == unCaesar
            || defender -> Type % BADLIFE == unRex)
            && attacker -> Weapons [0] -> Ammo > 0
            && defender -> Alt != 8) return 0;
            // letadla
            if (defender -> Type % BADLIFE >= unIkaros
            && defender -> Type % BADLIFE <= unRex
            && attacker -> Weapons [1] -> Ammo > 0
            && defender -> Alt == 8) return 1;
            // jine jednotky
            return -1;
            break;
        }
        case 45 : { //Satan
            // Pozemni, podpurne, vlaky, vznasedla, veze, lode mimo ponorku
            if (defender -> Type % BADLIFE >= unPerseusAlfa
            && defender -> Type % BADLIFE <= unLaguna
            && attacker -> Weapons [0] -> Ammo > 0) return 0;
            // Satan a baraky
            if (defender -> Type % BADLIFE >= unSatan
            && defender -> Type % BADLIFE <= unWarehouse
            && attacker -> Weapons [0] -> Ammo > 0) return 0;
            // Letadla na zemi
            if ((defender -> Type % BADLIFE == unCaesar
            || defender -> Type % BADLIFE == unRex)
            && attacker -> Weapons [0] -> Ammo > 0
            && defender -> Alt != 8) return 0;
            // jine jednotky
            return -1;
            break;
        }
        default : {
            return -1;
        }


    }
}


//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
void TArmy::MakeDangerArray ()
{
    int t;

    for (int i = 0; i < BADLIFE; i++) {
        if (Units [i] == NULL) {
            DangerArray [i] = 0;
            continue;
        }
        if (Units [i] -> Type >= unRadar) {
            t = 0;
            for (int j = 0; j < nofBadLife; j++) {
                if (Units [BL [j]] -> Type - BADLIFE == unBumerang) t++;
            }
            if (t != 0) {
                DangerArray [i] = 0;
                continue;
            }
        }

        DangerArray [i] = TabDanger [Units [i] -> Type];
    }
}



double TArmy::AttackStatus (int attacker, int weapon, int defender
                                                        , int kolikrat)
{
    double ph, status;

    if (weapon == -1) return 0;
    if (attacker < 0 || attacker >= UNITS_TOP) {
#ifdef DEBUG
        fprintf (dbgOutput, "Error: AttackStatus attacker = %i \n",attacker);
#endif
        AIError ();
        return 0;
    }
    if (Units [attacker] == NULL) {
#ifdef DEBUG
        fprintf (dbgOutput, "Error: AttackStatus attacker %i (=NULL) \n",attacker);
#endif
        AIError ();
        return 0;
    }
    if (Units [attacker] -> Type % BADLIFE >= unRadar) {
#ifdef DEBUG
        fprintf (dbgOutput, "Error: AttackStatus attacker %i not TUnit \n",attacker);
#endif
        AIError ();
        return 0;
    }
    if (defender < 0 || defender >= UNITS_TOP) {
#ifdef DEBUG
        fprintf (dbgOutput, "Error: AttackStatus defender = %i \n",defender);
#endif
        AIError ();
        return 0;
    }
    if (Units [defender] == NULL) {
#ifdef DEBUG
        fprintf (dbgOutput, "Error: AttackStatus defender %i (=NULL) \n",defender);
#endif
        AIError ();
        return 0;
    }


    ph = ProbHit (attacker, weapon, defender);
    status = DangerArray [defender] * ph * kolikrat;
    return status;
}



double TArmy::ProbHit (int attackerID, int weapon, int defenderID)
{
    TUnit *attacker;
    TObject *defender;

    if (weapon == -1) return 0;
    if (attackerID < 0 || attackerID >= UNITS_TOP) {
#ifdef DEBUG
        fprintf (dbgOutput, "Error: ProbHit attackerID = %i \n",attackerID);
#endif
        AIError ();
        return 0;
    }
    if (Units [attackerID] == NULL) {
#ifdef DEBUG
        fprintf (dbgOutput, "Error: ProbHit  attacker %i (=NULL) \n",attackerID);
#endif
        AIError ();
        return 0;
    }
    if (Units [attackerID] -> Type % BADLIFE >= unRadar) {
#ifdef DEBUG
        fprintf (dbgOutput, "Error: ProbHit  attacker %i not TUnit \n",attackerID);
#endif
        AIError ();
        return 0;
    }
    attacker = (TUnit *)Units [attackerID];

    if (defenderID < 0 || defenderID >= UNITS_TOP) {
#ifdef DEBUG
        fprintf (dbgOutput, "Error: ProbHit  defenderID = %i \n",defenderID);
#endif
        AIError ();
        return 0;
    }
    if (Units [defenderID] == NULL) {
#ifdef DEBUG
        fprintf (dbgOutput, "Error: ProbHit  defender %i (=NULL) \n",defenderID);
#endif
        AIError ();
        return 0;
    }
    defender = Units [defenderID];

    if (attacker -> Weapons [weapon] == NULL) {
#ifdef DEBUG
        fprintf (dbgOutput, "Error: ProbHit  attacker %i (weapon [%i]=NULL) \n",attackerID, weapon);
#endif
        AIError ();
        return 0;
    }

    int UC = attacker -> Weapons [weapon] -> AttackNum;
    int BN = attacker -> Weapons [weapon] -> BonusNum;
    int typ = attacker -> Weapons [weapon] -> GetType ();
    int OC = defender -> Defense [typ];
    int hps = defender -> HitPoints;
    double dam = 0, utok, obrana;

    for (int i = 0; i < 100; i++) {
        utok = UC + RollDice ();
        obrana = OC + RollDice ();
        if (utok > obrana) {
            dam += utok - obrana + BN;
            if (utok - obrana + BN > hps)
                dam += (utok - obrana + BN - hps) * CDestroy;
        }
    }
    return( dam / 100);
}



void TArmy::MakeProbHitTable() {

    if (ProbHitTable != NULL) {
        memfree (ProbHitTable);
        ProbHitTable = NULL;
    }
    if (nofGoodLife == 0 || nofBadLife == 0) return;
    ProbHitTable = (int *)memalloc (nofBadLife * nofGoodLife * sizeof (int));

    for (int i = 0; i < nofBadLife; i++) {
/////////////////////////////////////////////////////////////////////////
        if (DrawLocks == 1) {
            UnlockDraw (FALSE); ProcessMapAnim (); LockDraw ();
        }
        if (DrawLocks == 0) ProcessMapAnim ();
////////////////////////////////////////////////////////////////////////
        for (int j = 0; j < nofGoodLife; j++) {
            if (Units [GL [j]] -> Type >= unRadar) {
                ProbHitTable [i * nofGoodLife + j] = 0;
            }
            else {
                ProbHitTable [i * nofGoodLife + j] = (int) ProbHit (GL [j]
                , ChooseWeapon (GL [j], BL [i]), BL [i]);
            }

        }
    }

}



void TArmy::DeleteBL (int index)
{
    int i;

    nofBadLife--;
    for (i = index; i < nofBadLife; i++) {
        BL[i] = BL [i+1];
    }
}



void TArmy::DeleteGL (int index)
{
    int i;

    nofGoodLife--;
    for (i = index; i < nofGoodLife; i++) {
        GL[i] = GL [i+1];
    }
}



int TArmy::DeleteKilled ()
{
    int i,j,t=0;

    for (i = 0; i < nofBadLife; i++) {
        if (DrawLocks == 1) {
            UnlockDraw (FALSE); ProcessMapAnim (); LockDraw ();
        }
        if (DrawLocks == 0) ProcessMapAnim ();
        if ((BL [i] < 0) || (BL [i] >= UNITS_TOP)) {
            DeleteBL (i);
            i--;
            continue;
        }
        else {
            if (Units [BL [i]] == NULL || BL [i] < BADLIFE) {
                DeleteBL (i);
                i--;
            }
        }
    }

    for (i = 0; i < nofGoodLife; i++) {
        if (DrawLocks == 1) {
            UnlockDraw (FALSE); ProcessMapAnim (); LockDraw ();
        }
        if (DrawLocks == 0) ProcessMapAnim ();
        if ((GL [i] < 0) || (GL [i] >= UNITS_TOP)) {
            DeleteGL (i);
            i--;
            continue;
        }
        else {
            if (Units [GL [i]] == NULL || GL [i] >= BADLIFE) {
                DeleteGL (i);
                i--;
            }
        }
    }


//    if (AssignResult () != 0) return FALSE;

    MakeProbHitTable ();
    CountDangerous ();
    return TRUE;

}



void TArmy::DeleteKilled2 ()
{
    int i, j;

    for (i = 0; i < DUPos; i++) {
        if (DeletedUnits [i] < BADLIFE) {
            for (j = 0; j < nofGoodLife; j++) {
                if (DeletedUnits [i] == GL [j]) {
                    DeleteGL (j);
                    j--;
                }
            }
        }
        else {
            for (j = 0; j < nofBadLife; j++) {
                if (DeletedUnits [i] == BL [j]) {
                    DeleteBL (j);
                    j--;
                }
            }
        }
    }
}



void TArmy::DoAttackedFields ()
{
    int i;

    for (i = 0; i < AttackFieldPos; i++) {
        // 1. Zaradime utocici jednotky do armady
        if (AttackedField [i].Unit < BADLIFE) {
            if (Status == asActive) {
                if (IsInVisualRange (AttackedField [i].x, AttackedField [i].y)) {
                    if (Units [AttackedField [i].Unit] != NULL) {
                        Insert (AttackedField [i].Unit);
                    }
                }
            }
        }
        // 2. Aktivujeme armadu, pokud se strilelo do jejiho dohledu
        if (Status == asSleeping) {
            if (IsInVisualRange (AttackedField [i].x, AttackedField [i].y)) {
                Status = asActive;
            }
        }
    }

}




void TArmy::CountDangerous ()
{
    MakeDangerArray ();
    Dangerous = 0;
    for (int i = 0; i < nofBadLife; i++) {
        Dangerous += TabDanger [Units [BL [i]] -> Type % BADLIFE];
    }
}



double TArmy::PointStatus (int moverID, TPoint pozice)
{
    int i, weapon, r, x, y, max;
    TPoint ap;
    double st, ohr;
    TField *f;
    TUnit *mover;

    if (moverID < 0 || moverID >= UNITS_TOP) {
#ifdef DEBUG
        fprintf (dbgOutput, "Error: PointStatus moverID = %i \n",moverID);
#endif
        AIError ();
        return 0;
    }
    if (Units [moverID] == NULL) {
#ifdef DEBUG
        fprintf (dbgOutput, "Error: PointStatus  mover %i (=NULL) \n",moverID);
#endif
        AIError ();
        return 0;
    }
    if (Units [moverID] -> Type % BADLIFE >= unRadar) {
#ifdef DEBUG
        fprintf (dbgOutput, "Error: PointStatus mover %i not TUnit \n",moverID);
#endif
        AIError ();
        return 0;
    }
    mover = (TUnit *)Units [moverID];




    st = 0;


    // Spocitame ohrozeni pole
    ohr = 0;
    for (i = 0; i < nofGoodLife; i++) {
        if (Units [GL [i]] -> Type >= unRadar) continue;
        if (Units [GL [i]] -> X < 0 || Units [GL [i]] -> Y < 0) continue;
        weapon = ChooseWeapon (GL [i], moverID);
        if (weapon == -1) continue;
          // vybereme zbran
        r = (Units [GL [i]] -> MaxTimeUnits - ((TUnit *)Units [GL [i]])
        -> Weapons [weapon] -> TimeLost) / 5 + ((TUnit *)Units [GL [i]])
        -> Weapons [weapon] -> MaxRange;

        x = pozice.x - Units [GL [i]] -> X;
        y = pozice.y - Units [GL [i]] -> Y;

        if (abs (x) <= r && abs (y) <= r
        && Units [GL [i]] -> AI_Info.State != aistPropex) {
            if (weapon == 0 && UnAnalyse [GL [i]].w0 != NULL)
                ohr += UnAnalyse [GL [i]].w0 [(y + r) * (2*r + 1) + (x + r)]
                * ProbHitTable [ActualBLIndex * nofGoodLife + i];
            if (weapon == 1 && UnAnalyse [GL [i]].w1 != NULL)
                ohr += UnAnalyse [GL [i]].w1 [(y + r) * (2*r + 1) + (x + r)]
                * ProbHitTable [ActualBLIndex * nofGoodLife + i];
            if (weapon == 2 && UnAnalyse [GL [i]].w2 != NULL)
                ohr += UnAnalyse [GL [i]].w2 [(y + r) * (2*r + 1) + (x + r)]
                * ProbHitTable [ActualBLIndex * nofGoodLife + i];
            if (weapon == 3 && UnAnalyse [GL [i]].w3 != NULL)
                ohr += UnAnalyse [GL [i]].w2 [(y + r) * (2*r + 1) + (x + r)]
                * ProbHitTable [ActualBLIndex * nofGoodLife + i];
        }
    } // end of for
    ohr /= 3; // Delime 3, protoze viditelne maji UnAnalyse 3 * vetsi
    ohr *= TabCareful [mover -> Type % BADLIFE];
    // Pro ULTRA HYPER AGRESIVE ARMY je ohrozeni zanedbatelne
    if (CBanzai == C5Banzai) ohr /= 10;

    if (Status != asSuicide)
        st = ohr / (Dangerous * CDanger);
    else
        st = 0;

    // Pro ULTRA HYPER AGRESIVE ARMY se nesmi na pontony
    if (CBanzai == C5Banzai) {
        f = GetField (pozice.x, pozice.y);
        if (f -> Terrain2 == 361) {
            if (mover -> Type - BADLIFE == unMedea)
                st -= 10;
            else
                st += 10;
        }
    }


    // Aby se nerozutekly
    for (i = 0; i < nofBadLife; i++) {
        r = sqrt((pozice.x - Units [BL [i]] -> X) * (pozice.x - Units [BL [i]] -> X)
        + (pozice.y - Units [BL [i]] -> Y) * (pozice.y - Units [BL [i]] -> Y));
        // max = MAX (velikost, BL[i]...MaxRange, Unit...MaxRanga)
        max = velikost;
        if (Units [BL [i]] -> Type % BADLIFE < unRadar) {
            if (((TUnit *)Units [BL [i]]) -> Weapons [0] != NULL)
                if (((TUnit *)Units [BL [i]]) -> Weapons [0] -> MaxRange > max)
                    max = ((TUnit *)Units [BL [i]]) -> Weapons [0] -> MaxRange;
            if (((TUnit *)Units [BL [i]]) -> Weapons [1] != NULL)
                if (((TUnit *)Units [BL [i]]) -> Weapons [1] -> MaxRange > max)
                    max = ((TUnit *)Units [BL [i]]) -> Weapons [1] -> MaxRange;
            if (((TUnit *)Units [BL [i]]) -> Weapons [2] != NULL)
                if (((TUnit *)Units [BL [i]]) -> Weapons [2] -> MaxRange > max)
                    max = ((TUnit *)Units [BL [i]]) -> Weapons [2] -> MaxRange;
            if (((TUnit *)Units [BL [i]]) -> Weapons [3] != NULL)
                if (((TUnit *)Units [BL [i]]) -> Weapons [3] -> MaxRange > max)
                    max = ((TUnit *)Units [BL [i]]) -> Weapons [3] -> MaxRange;
        }

        if (mover -> Weapons [0] != NULL)
            if (mover -> Weapons [0] -> MaxRange > max)
                max = mover -> Weapons [0] -> MaxRange;
        if (mover -> Weapons [1] != NULL)
            if (mover -> Weapons [1] -> MaxRange > max)
                max = mover -> Weapons [1] -> MaxRange;
        if (mover -> Weapons [2] != NULL)
            if (mover -> Weapons [2] -> MaxRange > max)
                max = mover -> Weapons [2] -> MaxRange;
        if (mover -> Weapons [3] != NULL)
            if (mover -> Weapons [3] -> MaxRange > max)
                max = mover -> Weapons [3] -> MaxRange;

        if (Units [BL [i]] -> AI_Info.State == aistPlaced) {
            if (r > max)    st += CTogether * r;
        }
        else {
            if (r > max)    st += CTogether * r / 3;
        }
    }

    // Deme tvrde po nich
    for (i = 0; i < nofGoodLife; i++) {
        if (Units [GL [i]] -> X < 0 || Units [GL [i]] -> Y < 0) continue;
        r = sqrt((pozice.x - Units [GL [i]] -> X) * (pozice.x - Units [GL [i]] -> X)
        + (pozice.y - Units [GL [i]] -> Y) * (pozice.y - Units [GL [i]] -> Y));

        weapon = ChooseWeapon (moverID, GL [i]);
        if (weapon == -1) continue;
        max = (mover) -> Weapons [weapon] -> MaxRange
        + (mover -> MaxTimeUnits - mover -> Weapons [weapon] -> TimeLost) / 5
        - CContact;

        if (r > max)
            st += CToThem * r;
        else
            st += CToThemMin * r;


    }


    // Aby nejezdily do hajzlu, kdyz to neni potreba
    st += CLazy * sqrt ((pozice.x - mover -> X) * (pozice.x - mover -> X )
    + (pozice.y - mover -> Y) * (pozice.y - mover -> Y));

    // aby nebyly tesne u sebe
    for (i = 0; i < nofBadLife; i++) {
        if (Units [BL [i]] -> AI_Info.State != aistPlaced) continue;
        if (abs (pozice.x - Units [BL [i]] -> X) <= 1
        && abs (pozice.y - Units [BL [i]] -> Y) <= 1) st += CDist;
    }

    // snaha zustat u WayPoint
    if (WayPoint.x != -1) {
        r = sqrt((pozice.x - WayPoint.x)*(pozice.x - WayPoint.x)
        + (pozice.y - WayPoint.y)*(pozice.y - WayPoint.y));
        if (r > velikost - CContact)
            st += CToWayPoint * r;

    }

    // Bumerang obsazuje budovy
    if (mover -> Type % BADLIFE == unBumerang) {
        f = GetField (pozice.x + 1, pozice.y);
        if (f -> Unit != NO_UNIT && Units [f -> Unit] -> Type >= unRadar && f -> Unit < BADLIFE) st -= CCapture;
        f = GetField (pozice.x - 1, pozice.y);
        if (f -> Unit != NO_UNIT && Units [f -> Unit] -> Type >= unRadar && f -> Unit < BADLIFE) st -= CCapture;
        f = GetField (pozice.x, pozice.y + 1);
        if (f -> Unit != NO_UNIT && Units [f -> Unit] -> Type >= unRadar && f -> Unit < BADLIFE) st -= CCapture;
        f = GetField (pozice.x, pozice.y - 1);
        if (f -> Unit != NO_UNIT && Units [f -> Unit] -> Type >= unRadar && f -> Unit < BADLIFE) st -= CCapture;

    }

    // Zakazana policka
    if (DeniedPlaces != NULL) {
        i = 0;
        while (DeniedPlaces [i].x != -1) {
            if (DeniedPlaces [i].x == pozice.x && DeniedPlaces [i].y == pozice.y)
                st += 5*CDist;
            i++;
        }
    }

    // Medea - zakaz zastaveni na pontonu
    if (mover -> Type - BADLIFE == unMedea && MBPlaces != NULL) {
        for (i = 0; MBPlaces [i].x != -1; i++) {
            if (pozice.x == MBPlaces [i].x && pozice.y == MBPlaces [i].y) {
                if (GetField (MBPlaces [i].x, MBPlaces [i].y) -> Terrain2 == 361/*ponton*/) {
                    st += 20;
                }
            }
        }
    }

    if (st < 0) st = 0;
    return (st);
}



void TArmy::SetAR (int x1, int y1, int x2, int y2)
{
    ActionRadius.x1 = x1;
    ActionRadius.y1 = y1;
    ActionRadius.x2 = x2;
    ActionRadius.y2 = y2;
}


void TArmy::SetAIConst (int set)
{
    switch (set) {
        case 1 : {
            CDestroy = C1Destroy;
            CBanzai = C1Banzai;
            CTogether = C1Together;
            CToThem = C1ToThem;
            CToThemMin = C1ToThemMin;
            CLazy = C1Lazy;
            CDist = C1Dist;
            CPropexDist = C1PropexDist;
            CDanger = C1Danger;
            CContact = C1Contact;
            CCapture = C1Capture;
            CToWayPoint = C1ToWayPoint;
            CFuel = C1Fuel;
            break;
        }
        case 2 : {
            CDestroy = C2Destroy;
            CBanzai = C2Banzai;
            CTogether = C2Together;
            CToThem = C2ToThem;
            CToThemMin = C2ToThemMin;
            CLazy = C2Lazy;
            CDist = C2Dist;
            CPropexDist = C2PropexDist;
            CDanger = C2Danger;
            CContact = C2Contact;
            CCapture = C2Capture;
            CToWayPoint = C2ToWayPoint;
            CFuel = C2Fuel;
            break;
        }
        case 3 : {
            CDestroy = C3Destroy;
            CBanzai = C3Banzai;
            CTogether = C3Together;
            CToThem = C3ToThem;
            CToThemMin = C3ToThemMin;
            CLazy = C3Lazy;
            CDist = C3Dist;
            CPropexDist = C3PropexDist;
            CDanger = C3Danger;
            CContact = C3Contact;
            CCapture = C3Capture;
            CToWayPoint = C3ToWayPoint;
            CFuel = C3Fuel;
            break;
        }
        case 4 : {
            CDestroy = C4Destroy;
            CBanzai = C4Banzai;
            CTogether = C4Together;
            CToThem = C4ToThem;
            CToThemMin = C4ToThemMin;
            CLazy = C4Lazy;
            CDist = C4Dist;
            CPropexDist = C4PropexDist;
            CDanger = C4Danger;
            CContact = C4Contact;
            CCapture = C4Capture;
            CToWayPoint = C4ToWayPoint;
            CFuel = C4Fuel;
            break;
        }
        case 5 : {
            CDestroy = C5Destroy;
            CBanzai = C5Banzai;
            CTogether = C5Together;
            CToThem = C5ToThem;
            CToThemMin = C5ToThemMin;
            CLazy = C5Lazy;
            CDist = C5Dist;
            CPropexDist = C5PropexDist;
            CDanger = C5Danger;
            CContact = C5Contact;
            CCapture = C5Capture;
            CToWayPoint = C5ToWayPoint;
            CFuel = C5Fuel;
            break;
        }

    }
}



void TArmy::Displace ()
{
    for (int i = 0; i < nofBadLife; i++) {
        Units [BL [i]] -> AI_Info.point.x = Units [BL [i]] -> X;
        Units [BL [i]] -> AI_Info.point.y = Units [BL [i]] -> Y;
        Units [BL [i]] -> PlaceGround (FALSE);
        Units [BL [i]] -> X = Units [BL [i]] -> Y = -1;
    }
}



void TArmy::DisplaceGL ()
{
    for (int i = 0; i < nofGoodLife; i++) {
        Units [GL [i]] -> AI_Info.point.x = Units [GL [i]] -> X;
        Units [GL [i]] -> AI_Info.point.y = Units [GL [i]] -> Y;
        Units [GL [i]] -> PlaceGround (FALSE);
        Units [GL [i]] -> X = Units [GL [i]] -> Y = -1;
    }
}



void TArmy::Place ()
{
    int j, r;
    TField *f;
    TPoint p;

    for (int i = 0; i < nofBadLife; i++) {
        if (Units [BL [i]] -> AI_Info.point.x >= 0 && Units [BL [i]] -> AI_Info.point.x < MapSizeX
        && Units [BL [i]] -> AI_Info.point.y >= 0 && Units [BL [i]] -> AI_Info.point.y < MapSizeY) {

            for (j = 0; j < 120; j++) {
                p = CartezianSnail (j);
                p.x += Units [BL [i]] -> AI_Info.point.x;
                p.y += Units [BL [i]] -> AI_Info.point.y;
                if (p.x < 0 || p.x >= MapSizeX || p.y < 0 || p.y >= MapSizeY) continue;
                f = GetField (p.x, p.y);
                if (f -> Unit == NO_UNIT && GetAircraftAt (p.x, p.y) == NULL) {
                    Units [BL [i]] -> X = p.x;
                    Units [BL [i]] -> Y = p.y;
                    Units [BL [i]] -> PlaceGround (TRUE);
                    Units [BL [i]] -> AI_Info.point.x = 0;
                    Units [BL [i]] -> AI_Info.point.y = 0;
                    break;
                }
            }

        }
    }

}



void TArmy::PlaceGL ()
{
    int j, r;
    TField *f;
    TPoint p;

    for (int i = 0; i < nofGoodLife; i++) {
        if (Units [GL [i]] -> AI_Info.point.x >= 0 && Units [GL [i]] -> AI_Info.point.x < MapSizeX
        && Units [GL [i]] -> AI_Info.point.y >= 0 && Units [GL [i]] -> AI_Info.point.y < MapSizeY) {

            for (j = 0; j < 120; j++) {
                p = CartezianSnail (j);
                p.x += Units [GL [i]] -> AI_Info.point.x;
                p.y += Units [GL [i]] -> AI_Info.point.y;
                if (p.x < 0 || p.x >= MapSizeX || p.y < 0 || p.y >= MapSizeY) continue;
                f = GetField (p.x, p.y);
                if (f -> Unit == NO_UNIT && GetAircraftAt (p.x, p.y) == NULL) {
                    Units [GL [i]] -> X = p.x;
                    Units [GL [i]] -> Y = p.y;
                    Units [GL [i]] -> PlaceGround (TRUE);
                    Units [GL [i]] -> AI_Info.point.x = 0;
                    Units [GL [i]] -> AI_Info.point.y = 0;
                    break;
                }
            }

        }
    }

}



int TArmy::IsAtWP ()
{
    int t1 = 0, t2 = 0;
    int i;

    for (i = 0; i < nofBadLife; i++) {
        if (Units[BL[i]] == NULL) continue;
        if (IsInRect (Units [BL [i]] -> X, Units [BL [i]] -> Y,
        WayPoint.x - velikost, WayPoint.y - velikost
        , WayPoint.x + velikost, WayPoint.y + velikost)) {
            t1++;
        }
    }

    for (i = 0; i < nofBadLife; i++) {
        if (Units[BL[i]] == NULL) continue;
        if (IsInRect (Units [BL [i]] -> X, Units [BL [i]] -> Y,
        WayPoint.x - 2*velikost, WayPoint.y - 2*velikost
        , WayPoint.x + 2*velikost, WayPoint.y + 2*velikost)) {
            t2++;
        }
    }

    if (t1 > nofBadLife / 4 && t2 > nofBadLife / 2)
        return TRUE;
    else
        return FALSE;

}



void TArmy::SetRadar (int x, int y)
{
    RadarPos.x = x;
    RadarPos.y = y;
}



int TArmy::IsInVisualRange (int x, int y)
{
    TField *f;
    int j;

    if (x < 0 || x >= MapSizeX || y < 0 || y >= MapSizeY) return FALSE;
    if (GetBadlifeVisib (x, y) != 2) return FALSE;

    // RADAR
    if (RadarPos.x >= 0 && RadarPos.x < MapSizeX
    && RadarPos.y >= 0 && RadarPos.y < MapSizeY) {
        f = GetField (RadarPos.x, RadarPos.y);
        if (f -> Unit != NO_UNIT && Units [f -> Unit] -> Type == unRadar + BADLIFE) {
            if (sqrt ((x - RadarPos.x) * (x - RadarPos.x)
            + (y - RadarPos.y) * (y - RadarPos.y))
            <= Units [f -> Unit] -> Visib) {
                return TRUE;
            }
        }
    }

    // BADLIFE
    for (j = 0; j < nofBadLife; j++) {
        if (Units [BL [j]] -> X < 0 || Units [BL [j]] -> X >= MapSizeX
        || Units [BL [j]] -> Y < 0 || Units [BL [j]] -> Y >= MapSizeY) continue;

        if (sqrt ((x - Units [BL [j]] -> X) * (x - Units [BL [j]] -> X)
        + (y - Units [BL [j]] -> Y) * (y - Units [BL [j]] -> Y))
        <= Units [BL [j]] -> Visib) {
            return TRUE;
        }
    }
    return FALSE;

}



TArmy::~TArmy()
{
  memfree (BL); BL = NULL;
  memfree (GL); GL = NULL;
  if (PoziceUtoku != NULL) {memfree (PoziceUtoku); PoziceUtoku = NULL;}
    if (DangerArray != NULL) {memfree (DangerArray); DangerArray = NULL;}
    if (ProbHitTable != NULL) {memfree (ProbHitTable); ProbHitTable = NULL;}
}



void AIError () {
#ifdef DEBUG
    fclose (dbgOutput);
#endif
    exit (1);
}



