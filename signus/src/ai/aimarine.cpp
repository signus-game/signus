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
//        Object: TMarine
//
// Programmed by Richard Wunsch
//

#include "aimarine.h"
#include "untables.h"



TMarine *Marine1 = NULL;
TMarine *Marine2 = NULL;
TMarine *Marine3 = NULL;
TMarine *Marine4 = NULL;
TMarine *Marine5 = NULL;
TMarine *Marine6 = NULL;
TMarine *Marine7 = NULL;
TMarine *Marine8 = NULL;
TMarine *Marine9 = NULL;
TMarine *Marine10 = NULL;



TMarine::TMarine () : TArmy ()
{
            
}



TMarine::TMarine (ReadStream &stream) : TArmy (stream)
{
            
}

    

void TMarine::InsertUnits () {

    for (int i = 0; i < UNITS_TOP; i++) {
        if (Units [i] == NULL) continue;
        // vsechny lode
        if ((((TUnit *)Units [i]) -> Type >= unFlint + BADLIFE
        && ((TUnit *)Units [i]) -> Type <= unBarracuda + BADLIFE)
        || ((TUnit *)Units [i]) -> Type < BADLIFE)
            // ktere jsou v AR
            if (IsInRect (Units [i] -> X, Units [i] -> Y, ActionRadius)) 
                Insert (i);
    }   
}
    


int TMarine::MakeTurn ()
{
    int i, j, k;
    int t, h, besth, besttargetix, weapon, tta;
    word *ActionOrder = NULL;
    int maxt, maxj, n;

    if (MusicOn && (!IsMusicPlaying())) JukeboxNext();
    
    // Vyradime znicene
    if (DeleteKilled () == FALSE) return FALSE;

    // je-li army znicena nebo nema nepratele
    if (nofBadLife == 0) {
        return FALSE;
    }

    
    // Vynulejeme AI_Info
    for (i = 0; i < nofGoodLife; i++)
        Units [GL [i]] -> AI_Info.State = aistNought;
    for (i = 0; i < nofBadLife; i++)
        if ((Units [BL [i]] -> AI_Info.State != aistFuel)
        && (Units [BL [i]] -> AI_Info.State != aistAmmo)) 
            Units [BL [i]] -> AI_Info.State = aistNought;
    

    
    do {

    // Vyradime znicene
    if (DeleteKilled () == FALSE) return FALSE;


    // je-li army znicena nebo nema nepratele
    if (nofBadLife == 0) {
        return FALSE;
    }

    
    
    // Zrusime zarazeni do poradi
    for (i = 0; i < nofBadLife; i++)
        if (Units [BL [i]] -> AI_Info.State == aistOrdered) 
            Units [BL [i]] -> AI_Info.State = aistNought;


    // Generate Attackers   
    Attackers = (TAttacker *)memalloc (nofBadLife * sizeof (TAttacker));
    for (i = 0; i < nofBadLife; i++) {
        Attackers [i].nofTargets = 0;
                
        if (Units [BL [i]] -> X < 0 || Units [BL [i]] -> X >= MapSizeX 
        || Units [BL [i]] -> Y < 0 || Units [BL [i]] -> Y >= MapSizeY) continue;                
        Units [BL [i]] -> Select ();
        MakeAttackPoints (BL [i]);

        for (j = 0; j < nofGoodLife; j++) {
            UnlockDraw (FALSE); ProcessMapAnim (); LockDraw ();
            if (PoziceUtoku[j].x != MaxInt) {
                Attackers [i].Targets [Attackers [i].nofTargets] = j;
                Attackers [i].nofTargets++;
            }
            if (Attackers [i].nofTargets >= 9) break;
        }
    }
    
    // Generate Targets
    if (nofGoodLife > 0) {
        Targets = (TTarget *)memalloc (nofGoodLife * sizeof (TTarget));
        for (i = 0; i < nofGoodLife; i++) {
            UnlockDraw (FALSE); ProcessMapAnim (); LockDraw ();
            Targets [i].nofAttackers = 0;
            for (j = 0; j < nofBadLife; j++) {
                if (Targets [i].nofAttackers >= 9) break;
                for (k = 0; k < Attackers [j].nofTargets; k++) {
                    if (Attackers [j].Targets[k] == i) {
                        Targets [i].Attackers [Targets [i].nofAttackers] = j;
                        Targets [i].nofAttackers++;
                        break;
                    }
                }
            }
        }
    }
    else {
        Targets = NULL;
    }


    // Vygenerujeme poradi akci
    n = 0; // citac na nulu
    ActionOrder = (word *)memalloc (nofBadLife * sizeof (word)); 
    MakeDangerArray ();
    if (nofGoodLife > 0) {
        for (i = 0; i < nofGoodLife; i++) { // pro veschny GL
            UnlockDraw (FALSE); ProcessMapAnim (); LockDraw ();
            maxt = MinInt;         // nulujeme helpvar pro vyber maxima
            maxj = 0;
            
            for (j = 0; j < nofGoodLife; j++) { // vybereme nejnebezp jednotku
                if (DangerArray [j] > maxt && Units [GL [j]] -> AI_Info.State != aistAttacked) {
                    maxt = DangerArray [j];
                    maxj = j;
                    DangerArray [j] = MinInt;
                }
            }
            Units [GL [maxj]] -> AI_Info.State = aistAttacked; // Tato jednotka uz je napadena
            
            // do orderu nacpeme vsechny attackery nejnebezp jednotky
            for (j = 0; j < Targets [maxj].nofAttackers; j++) {
                // pokud je volna
                if (Units [BL [Targets [maxj].Attackers [j]]] -> AI_Info.State == aistNought) {             
                    ActionOrder [n] = Targets [maxj].Attackers [j];
                    n++;
                    Units [BL [Targets [maxj].Attackers [j]]] -> AI_Info.State = aistOrdered;
                }
            }
            
        } // end of i cyklus
    }
    
    // Zaradime zbytek - to, co nema na co utocit       
    for (i = 0; i < nofBadLife; i++) {
        // volna
        if (Units [BL [i]] -> AI_Info.State == aistNought) {
        
            ActionOrder [n] = i;
            n++;
            Units [BL [i]] -> AI_Info.State = aistOrdered;
        }
    } // end of i cyklus

    // Zaradime ty, co jedou tankovat
    for (i = 0; i < nofBadLife; i++) {
        // volna
        if (Units [BL [i]] -> AI_Info.State == aistFuel
        || Units [BL [i]] -> AI_Info.State == aistAmmo) {
            ActionOrder [n] = i;
            n++;
        }
    } // end of i cyklus


    // Urcime propexy - propably ex-living units
    for (i = 0; i < nofGoodLife; i++) {
        Units [GL [i]] -> AI_Info.State = aistNought;
        Units [GL [i]] -> AI_Info.Hps = 0;
    }
    
    for (i = 0; i < nofBadLife; i++) {
        UnlockDraw (FALSE); ProcessMapAnim (); LockDraw ();
        if (Units [BL [i]] -> AI_Info.State != aistOrdered) continue; // tato BL nebude utocit
        
        if (Units [BL [i]] -> X < 0 || Units [BL [i]] -> X >= MapSizeX 
        || Units [BL [i]] -> Y < 0 || Units [BL [i]] -> Y >= MapSizeY) continue;
        
        Units [BL [i]] -> Select ();
        besth = MinInt; // nulujeme nejlepsi vysledek
        besttargetix = MinInt;
        MakeAttackPoints (BL [i]);  // generujeme pozice utoku

        for (j = 0; j < nofGoodLife; j++) {  // pro vsechny GL
            if (Units [BL [i]] -> Type % BADLIFE >= unRadar) continue;
            h = 0;      // nulujeme akt hodnoceni           
            t = Units [BL [i]] -> TimeUnits;        // pomocne promenne
            weapon = ChooseWeapon (BL [i], GL [j]);
            if (weapon == -1) continue;
            tta = ((TUnit *)Units [BL [i]]) -> Weapons [weapon] -> TimeLost;
            
            // Da se na to vubec utocit
            if (Units [BL [i]] -> TimeToMove (PoziceUtoku[j].x, PoziceUtoku[j].y) < 0) 
                continue; // neda, de se na dalsi GL
            else // da, ubere se prislusny pocet timeun
                t -= Units [BL [i]] -> TimeToMove (PoziceUtoku[j].x, PoziceUtoku[j].y);
            
            // zkusme 1 attack
            if (t >= 1 * tta) {
                h = AttackStatus (BL [i], weapon, GL [j], 1);
            }
            // zkusme 2 attacky
            if (t >= 2 * tta) {
                h = AttackStatus (BL [i], weapon, GL [j], 2);
            }
            // zkusme 3 attacky
            if (t >= 3 * tta) {
                h = AttackStatus (BL [i], weapon, GL [j], 3);
            }
            
            // je to zatim nejlepsi target          
            if (h > besth) {
                besth = h;
                besttargetix = j;
            }
            
        } // end of j index - GL
        
        if (besttargetix != MinInt)
            Units [GL [besttargetix]] -> AI_Info.Hps += besth; // zvysime damage
        
    } // end of i index - BL
        
    // Oznacime propexy
    for (i = 0; i < nofGoodLife; i++) {
        if (Units [GL [i]] -> AI_Info.Hps > Units [GL [i]] -> HitPoints + CPropexDist) {    
            Units [GL [i]] -> AI_Info.State = aistPropex;
        }
    }

        
    // DO IT
    for (i = 0; i < n; i++) {
        DeleteKilled();
        UnlockDraw (FALSE);     ProcessMapAnim (); LockDraw ();
        if ((ActionOrder [i] < nofBadLife) && (Units [BL [ActionOrder [i]]] != NULL)
            && (Units [BL [ActionOrder [i]]] -> AI_Info.State == aistOrdered)) { // fucking problem
                ActualBLIndex = ActionOrder [i];
                if (ProblemJednotky (BL [ActionOrder [i]]) == FALSE) {
                     return FALSE;
                }
        }
    }
    
    // Uvolneni pameti
    if (Attackers != NULL) {memfree (Attackers); Attackers = NULL;}
    if (Targets != NULL) {memfree (Targets); Targets = NULL;}
    if (ActionOrder != NULL) {memfree (ActionOrder); ActionOrder = NULL;}

    // Urcime, je-li konec
    n = 0;
    DeleteKilled();
    for (i = 0; i < nofBadLife; i++)
        if (Units [BL [i]] -> AI_Info.State != aistPlaced) n++;
    
    
    } while (n > 0);
    
    return TRUE;
        
}



int TMarine::ProblemJednotky (int Unit) 
{

    DeleteKilled ();
    
    if (Unit < 0 || Unit >= UNITS_TOP) {
#ifdef DEBUG        
        fprintf (dbgOutput, "Error: ProblemJednotky Unit = %i \n",Unit);        
#endif      
        AIError ();
        return TRUE;
    }
    if (Units [Unit] == NULL) {
#ifdef DEBUG        
        fprintf (dbgOutput, "Error: ProblemJednotky Unit %i (=NULL) \n",Unit);      
#endif      
        AIError ();
        return TRUE;
    }
    
    if (((TUnit *)Units [Unit]) -> Velocity < 0) { //////////////////////////
        AIError ();
        return TRUE;
        
    }
    
    if (Units [Unit] -> X < 0 || Units [Unit] -> X >= MapSizeX 
    || Units [Unit] -> Y < 0 || Units [Unit] -> Y >= MapSizeY) {
        Units [Unit] -> AI_Info.State = aistPlaced;
        ProgressInc ();
        return TRUE;
    }
    
    

    if (Units [Unit] -> Type - BADLIFE >= unFlint
    && Units [Unit] -> Type - BADLIFE <= unKraken) return PJShip (Unit);
    
    if (Units [Unit] -> Type - BADLIFE == unBarracuda) return PJShip (Unit);
    
    Units [Unit] -> AI_Info.State = aistPlaced; //////!!!!!!!!!!!!!!!!!!!!!!!
    return TRUE;
}




int TMarine::PJShip (int UnID)
{
    int i,j,h,t, weapon, tta, VBcounter;
    struct {
        TObject *Target;
        int Index;
        TPoint AttackPoint;
        TPoint cil;
    } AkceUUUP,AkceUUP,AkceUP;
    AkceUUUP.Target = NULL;
    AkceUUUP.Index = 0;
    AkceUUUP.AttackPoint = TPoint();
    AkceUUUP.cil = TPoint();
    AkceUUP = AkceUP = AkceUUUP;
    
    TPoint AkceP;
    TUnit *Unit;
    
    double hUUUP,hUUP,hUP,hP;
    int Akce, shot, TargetHps;
    TPoint start, cil, AttackPos;
    
    Unit = (TUnit *)Units [UnID];
    start.x = Unit -> X, start.y = Unit -> Y;
    Unit -> Select ();
    MakeAttackPoints (UnID);
    MakeDangerArray ();

    
    //Vybereme nejlepsi UUUP
    hUUUP = MinInt;
    for (i = 0; i < nofGoodLife; i++) {
        UnlockDraw (FALSE); ProcessMapAnim (); LockDraw ();
        // neni na to videt - nebude se utocit
        if (GetBadlifeVisib (Units [GL [i]] -> X, Units [GL [i]] -> Y) != 2) continue;
        // osetreni viditelnosti Barracudy
        if (Units [GL [i]] -> Type % BADLIFE == unBarracuda) {
            VBcounter = 0;
            for (j = 0; j < nofBadLife; j++) {
                if (Units [BL [j]] -> Type % BADLIFE == unViking
                || Units [BL [j]] -> Type % BADLIFE == unBarracuda) {
                    if (((Units [BL [j]] -> X - Units [GL [i]] -> X)*(Units [BL [j]] -> X - Units [GL [i]] -> X)
                    + (Units [BL [j]] -> Y - Units [GL [i]] -> Y)*(Units [BL [j]] -> Y - Units [GL [i]] -> Y))
                    <= Units [BL [j]] -> Visib * Units [BL [j]] -> Visib) VBcounter++;
                }
            }
            if (VBcounter == 0) continue;
        }

        weapon = ChooseWeapon (UnID, GL [i]);
        if (weapon == -1) continue;     
        
        h = 0;
        t = Unit -> TimeUnits;      
        tta = Unit -> Weapons [weapon] -> TimeLost;     
        AttackPos = PoziceUtoku [i];
        // je to vubec na dostrel?
        if (Unit -> TimeToMove (AttackPos.x, AttackPos.y) < 0) {
            continue;       // neni - de se na dalsi GL
        }

        t -= Unit -> TimeToMove (AttackPos.x, AttackPos.y);         
        t -= 3 * tta; // snizme tu
        
        if (t >= 0) { // akci lze provest
            cil = BestPlace (AttackPos, UnID, t);
            h = PointStatus (UnID, start) - PointStatus (UnID, cil);
            h += CBanzai * AttackStatus (UnID, weapon, GL [i], 3);
        }
        
        // vybereme maximum
        if (h > hUUUP) {
            hUUUP = h;
            AkceUUUP.Target = Units [GL [i]];
            AkceUUUP.Index = i;
            AkceUUUP.AttackPoint = AttackPos;
            AkceUUUP.cil = cil;
        }
    }

    //Vybereme nejlepsi UUP
    hUUP = MinInt;
    for (i = 0; i < nofGoodLife; i++) {
        UnlockDraw (FALSE); ProcessMapAnim (); LockDraw ();
        // neni na to videt
        if (GetBadlifeVisib (Units [GL [i]] -> X, Units [GL [i]] -> Y) != 2) continue;
        // osetreni viditelnosti Barracudy
        if (Units [GL [i]] -> Type % BADLIFE == unBarracuda) {
            VBcounter = 0;
            for (j = 0; j < nofBadLife; j++) {
                if (Units [BL [j]] -> Type % BADLIFE == unViking
                || Units [BL [j]] -> Type % BADLIFE == unBarracuda) {
                    if (((Units [BL [j]] -> X - Units [GL [i]] -> X)*(Units [BL [j]] -> X - Units [GL [i]] -> X)
                    + (Units [BL [j]] -> Y - Units [GL [i]] -> Y)*(Units [BL [j]] -> Y - Units [GL [i]] -> Y))
                    <= Units [BL [j]] -> Visib * Units [BL [j]] -> Visib) VBcounter++;
                }
            }
            if (VBcounter == 0) continue;
        }

        weapon = ChooseWeapon (UnID, GL [i]);
        if (weapon == -1) continue;
        
        h = 0;
        t = Unit -> TimeUnits;      
        tta = Unit -> Weapons [weapon] -> TimeLost;     
        AttackPos = PoziceUtoku [i];        
        // je to vubec na dostrel?
        if (Unit -> TimeToMove (AttackPos.x, AttackPos.y) < 0) {
            continue; // neni - de se na dalsi GL
        }

        t -= Unit -> TimeToMove (AttackPos.x, AttackPos.y);         
        t -= 2 * tta; // snizme tu
        
        if (t >= 0) { // akci lze provest
            cil = BestPlace (AttackPos, UnID, t);
            h = PointStatus (UnID, start) - PointStatus (UnID, cil);
            h += CBanzai * AttackStatus (UnID, weapon, GL [i], 2);
        }
        
        // vybereme maximum
        if (h > hUUP) {
            hUUP = h;
            AkceUUP.Target = Units [GL [i]];
            AkceUUP.Index = i;
            AkceUUP.AttackPoint = AttackPos;
            AkceUUP.cil = cil;
        }
    }

    //Vybereme nejlepsi UP
    hUP = MinInt;
    for (i = 0; i < nofGoodLife; i++) {
        UnlockDraw (FALSE); ProcessMapAnim (); LockDraw ();
        // nei na to videt
        if (GetBadlifeVisib (Units [GL [i]] -> X, Units [GL [i]] -> Y) != 2) continue;
        // osetreni viditelnosti Barracudy
        if (Units [GL [i]] -> Type % BADLIFE == unBarracuda) {
            VBcounter = 0;
            for (j = 0; j < nofBadLife; j++) {
                if (Units [BL [j]] -> Type % BADLIFE == unViking
                || Units [BL [j]] -> Type % BADLIFE == unBarracuda) {
                    if (((Units [BL [j]] -> X - Units [GL [i]] -> X)*(Units [BL [j]] -> X - Units [GL [i]] -> X)
                    + (Units [BL [j]] -> Y - Units [GL [i]] -> Y)*(Units [BL [j]] -> Y - Units [GL [i]] -> Y))
                    <= Units [BL [j]] -> Visib * Units [BL [j]] -> Visib) VBcounter++;
                }
            }
            if (VBcounter == 0) continue;
        }

        weapon = ChooseWeapon (UnID, GL [i]);
        if (weapon == -1) continue;
        
        h = 0;
        t = Unit -> TimeUnits;      
        tta = Unit -> Weapons [weapon] -> TimeLost;     
        AttackPos = PoziceUtoku [i];                
        // je to vubec na dostrel?
        if (Unit -> TimeToMove (AttackPos.x, AttackPos.y) < 0) {
            continue;  // neni - de se na dalsi GL
        }

        t -= Unit -> TimeToMove (AttackPos.x, AttackPos.y);         
        t -= 1 * tta; // snizme tu
        
        if (t >= 0) { // akci lze provest
            cil = BestPlace (AttackPos, UnID, t);
            h = PointStatus (UnID, start) - PointStatus (UnID, cil);
            h += CBanzai * AttackStatus (UnID, weapon, GL [i], 1);
        }
        
        // vybereme maximum
        if (h > hUP) {
            hUP = h;
            AkceUP.Target = Units [GL [i]];
            AkceUP.Index = i;
            AkceUP.AttackPoint = AttackPos;
            AkceUP.cil = cil;
        }
    }


    //Vybereme nejlepsi P
    t = Unit -> TimeUnits;      
    AkceP = BestPlace (start, UnID, t);         
    hP = PointStatus (UnID, start) - PointStatus(UnID, AkceP);

    // Vybereme nejlepsi akci
    h = 0;
    Akce = 0;
    if (hUUUP > h) {h = hUUUP; Akce = 1;}
    if (hUUP > h) {h = hUUP; Akce = 2;}
    if (hUP > h) {h = hUP; Akce = 3;}
    if (hP > h) {h = hP; Akce = 4;}

    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    if (Unit -> Weapons [0] -> Ammo == 0) Akce = 4;
//  Akce = 4;

    // Provedeme Akci
    UnlockDraw ();
    switch (Akce) {
        case 0 : { // Zadna akce
            Unit -> AI_Info.State = aistPlaced;
            ProgressInc ();
            break; 
        }
        
        case 1 : { // UTOK, UTOK, UTOK, POHYB
            // Dojede na pozici strelby
            Unit -> Move (AkceUUUP.AttackPoint.x, AkceUUUP.AttackPoint.y);
            if (Units [UnID] == NULL) {
                LockDraw ();  // Unit znicena
                return TRUE;    
            }
            // UTOK 1
            TargetHps = AkceUUUP.Target -> HitPoints;           
            Unit -> CurWpn = ChooseWeapon (UnID, AkceUUUP.Target -> ID);
            shot = Unit -> Attack (AkceUUUP.Target -> X, AkceUUUP.Target -> Y);
            if (DeleteKilled () == FALSE) {
                RedrawMap ();
                LockDraw ();            // Konec hry
                return FALSE;
            }
            if (shot >= TargetHps) {
                RedrawMap ();
                LockDraw ();
                return TRUE;  // nepritel znicen, de se pryc
            }
            if (Units [UnID] == NULL) {
                RedrawMap ();
                LockDraw ();  // Unit znicena
                return TRUE;    
            }
            // UTOK 2
            TargetHps = AkceUUUP.Target->HitPoints;
            shot = Unit -> Attack (AkceUUUP.Target -> X, AkceUUUP.Target -> Y);
            if (DeleteKilled () == FALSE) {
                RedrawMap ();
                LockDraw ();        // konec hry
                return FALSE;
            }
            if (shot >= TargetHps) {
                RedrawMap ();
                LockDraw ();
                return TRUE;  // nepritel znicen, de se pryc
            }
            if (Units [UnID] == NULL) {
                RedrawMap ();
                LockDraw ();  // Unit znicena
                return TRUE;    
            }
            // UTOK 3           
            TargetHps = AkceUUUP.Target -> HitPoints;
            shot = Unit -> Attack (AkceUUUP.Target -> X, AkceUUUP.Target -> Y);     
            if (DeleteKilled () == FALSE) {
                RedrawMap ();
                LockDraw ();
                return FALSE;
            }
            if (Units [UnID] == NULL) {
                RedrawMap ();
                LockDraw ();  // Unit znicena
                return TRUE;    
            }
            // Dojede do cile
            Unit -> Move (AkceUUUP.cil.x, AkceUUUP.cil.y);
            if (DeleteKilled () == FALSE) {
                RedrawMap ();
                LockDraw ();
                return FALSE;
            }
            if (Units [UnID] == NULL) {
                RedrawMap ();
                LockDraw ();  // Unit znicena
                return TRUE;    
            }
            Unit -> AI_Info.State = aistPlaced;
            ProgressInc ();
            
            break;
        }
        
        case 2 : { // UTOK, UTOK, POHYB
            // Dojede na pozici strelby
            Unit -> Move (AkceUUP.AttackPoint.x, AkceUUP.AttackPoint.y);
            if (Units [UnID] == NULL) {
                RedrawMap ();
                LockDraw ();  // Unit znicena
                return TRUE;    
            }
            // UTOK 1
            TargetHps = AkceUUP.Target -> HitPoints;
            Unit -> CurWpn = ChooseWeapon (UnID, AkceUUP.Target -> ID);
            shot = Unit -> Attack (AkceUUP.Target -> X, AkceUUP.Target -> Y);
            if (DeleteKilled () == FALSE) {
                RedrawMap ();
                LockDraw ();   // konec hry
                return FALSE;
            }
            if (shot >= TargetHps) {
                RedrawMap ();
                LockDraw ();
                return TRUE;  // nepritel znicen, de se pryc
            }
            if (Units [UnID] == NULL) {
                RedrawMap ();
                LockDraw ();  // Unit znicena
                return TRUE;    
            }
            // UTOK 2
            TargetHps = AkceUUP.Target -> HitPoints;
            shot = Unit -> Attack (AkceUUP.Target -> X, AkceUUP.Target -> Y);           
            if (DeleteKilled () == FALSE) {
                RedrawMap ();
                LockDraw (); // konec hry
                return FALSE;
            }
            if (Units [UnID] == NULL) {
                RedrawMap ();
                LockDraw ();  // Unit znicena
                return TRUE;    
            }
            // Dojede do cile
            Unit -> Move (AkceUUP.cil.x, AkceUUP.cil.y);            
            if (Units [UnID] == NULL) {
                RedrawMap ();
                LockDraw ();  // Unit znicena
                return TRUE;    
            }

            Unit -> AI_Info.State = aistPlaced;
            ProgressInc ();
                        
            break;
        }
        
        case 3 : { // UTOK, POHYB
            // Dojede na pozici strelby
            Unit -> Move (AkceUP.AttackPoint.x, AkceUP.AttackPoint.y);
            if (Units [UnID] == NULL) {
                RedrawMap ();
                LockDraw ();  // Unit znicena
                return TRUE;    
            }
            // UTOK 1
            TargetHps = AkceUP.Target -> HitPoints;
            Unit -> CurWpn = ChooseWeapon (UnID, AkceUP.Target -> ID);
            shot = Unit -> Attack (AkceUP.Target -> X, AkceUP.Target -> Y);
            if (DeleteKilled () == FALSE) {
                RedrawMap ();
                LockDraw ();   // konec hry
                return FALSE;
            }
            if (Units [UnID] == NULL) {
                RedrawMap ();
                LockDraw ();  // Unit znicena
                return TRUE;    
            }
            // Dojede do cile
            Unit -> Move (AkceUP.cil.x, AkceUP.cil.y);
            if (Units [UnID] == NULL) {
                RedrawMap ();
                LockDraw ();  // Unit znicena
                return TRUE;    
            }
            
            Unit -> AI_Info.State = aistPlaced;
            ProgressInc ();
            
            break;
        }
        
        case 4 : { // JEN POHYB
            // Dojede do cile
            Unit -> Move (AkceP.x, AkceP.y);
            if (Units [UnID] == NULL) {
                RedrawMap ();
                LockDraw ();  // Unit znicena
                return TRUE;    
            }
            
            Unit -> AI_Info.State = aistPlaced;
            ProgressInc ();
            
            break;
        }
    }   
    
    RedrawMap ();
    LockDraw ();    
    return TRUE;
    
}

