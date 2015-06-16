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
//      Object: TGroundArmy
//
// Programmed by Richard Wunsch
//

#include "aiground.h"
#include "untables.h"



TGroundArmy *Army1 = NULL;
TGroundArmy *Army2 = NULL;
TGroundArmy *Army3 = NULL;
TGroundArmy *Army4 = NULL;
TGroundArmy *Army5 = NULL;
TGroundArmy *Army6 = NULL;
TGroundArmy *Army7 = NULL;
TGroundArmy *Army8 = NULL;
TGroundArmy *Army9 = NULL;
TGroundArmy *Army10 = NULL;
TGroundArmy *Army11 = NULL;
TGroundArmy *Army12 = NULL;
TGroundArmy *Army13 = NULL;
TGroundArmy *Army14 = NULL;
TGroundArmy *Army15 = NULL;
TGroundArmy *Army16 = NULL;

TPoint *MBPlaces;
TPoint *DeniedPlaces;


TGroundArmy::TGroundArmy () : TArmy ()
{
                        
}



TGroundArmy::TGroundArmy (FILE *f) : TArmy (f)
{
                        
}

void TGroundArmy::InsertUnits () {

        for (int i = 0; i < UNITS_TOP; i++) {
                if (Units [i] == NULL) continue;
                // pozemni bojova BL
                if ( ((((TUnit *) Units [i]) -> Type <= unIris + BADLIFE) 
                && (((TUnit *) Units [i]) -> Type > BADLIFE)) 
                || (((TUnit *)Units [i]) -> Type == unRex + BADLIFE)
                
                || (((TUnit *)Units [i]) -> Type >= unNeptun + BADLIFE 
          && ((TUnit *)Units [i]) -> Type <= unGanymedes + BADLIFE)
          || (((TUnit *)Units [i]) -> Type == unSatan + BADLIFE)
          || (((TUnit *)Units [i]) -> ID < BADLIFE)) {
                        
                        // ktere jsou v AR
                        if (IsInRect (Units [i] -> X, Units [i] -> Y, ActionRadius)) {
                                Insert (i);
                                if (Units [i] -> Type - BADLIFE == unRex) {
                                        ((TRex *)Units [i]) -> Land ();
                                }
                        }
                }
        }
        
}
        

int TGroundArmy::MakeTurn ()
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
                        UnlockDraw (FALSE);     ProcessMapAnim (); LockDraw ();
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
                        UnlockDraw (FALSE);     ProcessMapAnim (); LockDraw ();
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
                        UnlockDraw (FALSE);     ProcessMapAnim (); LockDraw ();
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
                UnlockDraw (FALSE);     ProcessMapAnim (); LockDraw ();
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
                        t = Units [BL [i]] -> TimeUnits;                // pomocne promenne
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



int TGroundArmy::ProblemJednotky (int Unit) 
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
        
        if (((TUnit *)Units [Unit]) -> Velocity < 0) {
                AIError ();
                return TRUE;
                
        }
        
        if (Units [Unit] -> X < 0 || Units [Unit] -> X >= MapSizeX 
        || Units [Unit] -> Y < 0 || Units [Unit] -> Y >= MapSizeY) {
                Units [Unit] -> AI_Info.State = aistPlaced;
                ProgressInc ();
                return TRUE;
        }

        if (Units [Unit] -> Type - BADLIFE == unGolias) {
                return PJGroundFight (Unit);
        }
        
        if ((Units [Unit] -> Type - BADLIFE >= unPerseusAlfa 
        && Units [Unit] -> Type - BADLIFE <= unPatriot)
        || (Units [Unit] -> Type - BADLIFE == unHelios)) return PJGroundFight (Unit);
        
        if (Units [Unit] -> Type - BADLIFE == unMohykan) return PJGroundFight (Unit);
        if (Units [Unit] -> Type - BADLIFE == unMesias) return PJGroundFight (Unit);
        if (Units [Unit] -> Type - BADLIFE == unRex) return PJRex (Unit);
        if (Units [Unit] -> Type - BADLIFE == unBumerang) return PJBumerang (Unit);
        if (Units [Unit] -> Type - BADLIFE == unCeres) return PJCeres (Unit);
        if (Units [Unit] -> Type - BADLIFE == unGnom) return PJGnom (Unit);
        if (Units [Unit] -> Type - BADLIFE == unUran) return PJUran (Unit);
        if (Units [Unit] -> Type - BADLIFE == unMedea) return PJMedea (Unit);
        if (Units [Unit] -> Type - BADLIFE == unSkiatron) return PJSkiatron (Unit);
        if (Units [Unit] -> Type - BADLIFE == unIris) return PJIris (Unit);
        if (Units [Unit] -> Type - BADLIFE == unGanymedes) return PJGanymedes (Unit);
        if (Units [Unit] -> Type - BADLIFE == unSatan) return PJSatan (Unit);
        if (Units [Unit] -> Type - BADLIFE == unGargantua) return PJGargantua (Unit);
        
        Units [Unit] -> AI_Info.State = aistPlaced; //////!!!!!!!!!!!!!!!!!!!!!!!
        
        return TRUE;
}




int TGroundArmy::PJGroundFight (int UnID)
{
        int i,j,h,t, weapon, tta;
        TField *f;
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
	
	
        TPoint AkceP, p;
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
                UnlockDraw (FALSE);     ProcessMapAnim (); LockDraw ();
                // neni na to videt - nebude se utocit
                if (GetBadlifeVisib (Units [GL [i]] -> X, Units [GL [i]] -> Y) != 2) continue;
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
                UnlockDraw (FALSE);     ProcessMapAnim (); LockDraw ();
                // neni na to videt
                if (GetBadlifeVisib (Units [GL [i]] -> X, Units [GL [i]] -> Y) != 2) continue;
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
                UnlockDraw (FALSE);     ProcessMapAnim (); LockDraw ();
                // nei na to videt
                if (GetBadlifeVisib (Units [GL [i]] -> X, Units [GL [i]] -> Y) != 2) continue;
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
//      Akce = 4;

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
                                RedrawMap ();
                                LockDraw ();  // Unit znicena
                                return TRUE;    
                        }
                        // UTOK 1
                        TargetHps = AkceUUUP.Target -> HitPoints;                       
                        Unit -> CurWpn = ChooseWeapon (UnID, AkceUUUP.Target -> ID);
                        shot = Unit -> Attack (AkceUUUP.Target -> X, AkceUUUP.Target -> Y);
                        if (DeleteKilled () == FALSE) {
                                RedrawMap ();
                                LockDraw ();                    // Konec hry
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
                        TargetHps = AkceUUUP.Target -> HitPoints;
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

                        // Obrati se spravnym smerem                    
                        if (L1TerrainType[GetField(Unit -> X, Unit -> Y) -> Terrain] == tofsL1A
                        && Unit -> Type - BADLIFE <= unIris) {
                                for (j = 0; j < 440; j++) {
                                        p = CartezianSnail (j);
                                        if ((AkceUUUP.cil.x + p.x < 0) || (AkceUUUP.cil.x + p.x >= MapSizeX) 
                                        || (AkceUUUP.cil.y + p.y < 0) || (AkceUUUP.cil.y + p.y >= MapSizeY)) continue;
                                        f = GetField (AkceUUUP.cil.x + p.x, AkceUUUP.cil.y + p.y);
                                        if (f -> Unit != NO_UNIT && Units [f -> Unit] -> Type < BADLIFE) {
                                                Unit -> Rotate (AkceUUUP.cil.x + p.x, AkceUUUP.cil.y + p.y);
                                                break;
                                        }
                                }
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
                        // Obrati se spravnym smerem                    
                        if (L1TerrainType[GetField(Unit -> X, Unit -> Y) -> Terrain] == tofsL1A
                        && Unit -> Type - BADLIFE <= unIris) {
                                for (j = 0; j < 440; j++) {
                                        p = CartezianSnail (j);
                                        if ((AkceUUUP.cil.x + p.x < 0) || (AkceUUUP.cil.x + p.x >= MapSizeX) 
                                        || (AkceUUUP.cil.y + p.y < 0) || (AkceUUUP.cil.y + p.y >= MapSizeY)) continue;
                                        f = GetField (AkceUUUP.cil.x + p.x, AkceUUUP.cil.y + p.y);
                                        if (f -> Unit != NO_UNIT && Units [f -> Unit] -> Type < BADLIFE) {
                                                Unit -> Rotate (AkceUUUP.cil.x + p.x, AkceUUUP.cil.y + p.y);
                                                break;
                                        }
                                }
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

                        // Obrati se spravnym smerem                    
                        if (L1TerrainType[GetField(Unit -> X, Unit -> Y) -> Terrain] == tofsL1A
                        && Unit -> Type - BADLIFE <= unIris) {
                                for (j = 0; j < 440; j++) {
                                        p = CartezianSnail (j);
                                        if ((AkceUUUP.cil.x + p.x < 0) || (AkceUUUP.cil.x + p.x >= MapSizeX) 
                                        || (AkceUUUP.cil.y + p.y < 0) || (AkceUUUP.cil.y + p.y >= MapSizeY)) continue;
                                        f = GetField (AkceUUUP.cil.x + p.x, AkceUUUP.cil.y + p.y);
                                        if (f -> Unit != NO_UNIT && Units [f -> Unit] -> Type < BADLIFE) {
                                                Unit -> Rotate (AkceUUUP.cil.x + p.x, AkceUUUP.cil.y + p.y);
                                                break;
                                        }
                                }
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
                        // Obrati se spravnym smerem                    
                        if (L1TerrainType[GetField(Unit -> X, Unit -> Y) -> Terrain] == tofsL1A
                        && Unit -> Type - BADLIFE <= unIris) {
                                for (j = 0; j < 440; j++) {
                                        p = CartezianSnail (j);
                                        if ((AkceUUUP.cil.x + p.x < 0) || (AkceUUUP.cil.x + p.x >= MapSizeX) 
                                        || (AkceUUUP.cil.y + p.y < 0) || (AkceUUUP.cil.y + p.y >= MapSizeY)) continue;
                                        f = GetField (AkceUUUP.cil.x + p.x, AkceUUUP.cil.y + p.y);
                                        if (f -> Unit != NO_UNIT && Units [f -> Unit] -> Type < BADLIFE) {
                                                Unit -> Rotate (AkceUUUP.cil.x + p.x, AkceUUUP.cil.y + p.y);
                                                break;
                                        }
                                }
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



int TGroundArmy::PJBumerang (int Unit)
{
        TField *f;
        
        f = GetField (Units [Unit] -> X + 1, Units [Unit] -> Y);
        if (f -> Unit != NO_UNIT && Units [f -> Unit] -> Type >= unRadar && f -> Unit < BADLIFE) {
                Units [Unit] -> Select ();
                ((TBumerang *)Units [Unit]) -> OccupyBuilding (Units [f -> Unit]);
                DeleteKilled ();
                Units [Unit] -> AI_Info.State = aistPlaced;
                ProgressInc ();
                return TRUE;
        }
        f = GetField (Units [Unit] -> X - 1, Units [Unit] -> Y);
        if (f -> Unit != NO_UNIT && Units [f -> Unit] -> Type >= unRadar && f -> Unit < BADLIFE) {
                Units [Unit] -> Select ();              
                ((TBumerang *)Units [Unit]) -> OccupyBuilding (Units [f -> Unit]);              
                DeleteKilled ();                
                Units [Unit] -> AI_Info.State = aistPlaced;             
                ProgressInc ();
                return TRUE;            
        }
        f = GetField (Units [Unit] -> X, Units [Unit] -> Y + 1);
        if (f -> Unit != NO_UNIT && Units [f -> Unit] -> Type >= unRadar && f -> Unit < BADLIFE) {
                Units [Unit] -> Select ();              
                ((TBumerang *)Units [Unit]) -> OccupyBuilding (Units [f -> Unit]);              
                DeleteKilled ();                
                Units [Unit] -> AI_Info.State = aistPlaced;             
                ProgressInc ();
                return TRUE;            
        }
        f = GetField (Units [Unit] -> X, Units [Unit] -> Y - 1);
        if (f -> Unit != NO_UNIT && Units [f -> Unit] -> Type >= unRadar && f -> Unit < BADLIFE) {
                Units [Unit] -> Select ();              
                ((TBumerang *)Units [Unit]) -> OccupyBuilding (Units [f -> Unit]);              
                DeleteKilled ();                
                Units [Unit] -> AI_Info.State = aistPlaced;             
                ProgressInc ();
                return TRUE;            
        }
        
        return PJGroundFight (Unit);                    
}




int TGroundArmy::PJCeres (int UnID)
{
        TPoint *TankPosition;
        int t, ttm, mint, imax, i;
        double h, hmax;
        TPoint start, cil;
        TCeres *Unit;
        
        Unit = (TCeres *)Units [UnID];
        Unit -> Select ();
        // Co kdyz dosel benzin
        if (Unit -> Fuel == 0) {
                Unit -> Fuel = Unit -> MaxFuel;
        }
        
        // Nejdriv doplnime Rexe
        for (i = BADLIFE; i < UNITS_TOP; i++) {
                if (Units [i] != NULL && Units [i] -> Type % BADLIFE == unRex 
                && ((TRex *)Units [i]) -> FlyLevel == 0 
                && Units [i] -> AI_Info.Hps == UnID) {
                        UnlockDraw ();
                        Unit -> DoSupport (Units [i] -> X, Units [i] -> Y);
                        if (DeleteKilled () == FALSE) {
                                RedrawMap ();
                                LockDraw ();   // znicena armada, konec hry apod.
                                return FALSE;
                        }
                        if (Units [UnID] == NULL) {
                                RedrawMap ();
                                LockDraw (); // Jednotka znicena
                                return TRUE;
                        }
                        if (Units [i] == NULL) {
                                RedrawMap ();
                                LockDraw (); // Jednotka znicena
                                return TRUE;                            
                        }                       
                        RedrawMap ();
                        LockDraw ();
                }
        }
                
        

        do {    
                start.x = Unit -> X; start.y = Unit -> Y;
                // Naplnime pole TankPosition nejlepsimi pozicemi tankovani vsech BL
                TankPosition = (TPoint *)memalloc (sizeof (TPoint) * nofBadLife);
                for (i = 0; i < nofBadLife; i++) {
                        TankPosition [i].x = MinInt; TankPosition [i].y = MinInt;
                        if (Units [BL [i]] -> Type - BADLIFE == unCeres
                        || Units [BL [i]] -> Type - BADLIFE == unGanymedes) continue;
                        if (!Unit -> WillSupport ((TUnit *)Units [BL [i]])) continue;
                        
                        mint = MaxInt;
                        t = -1;
                        if (Units [BL [i]] -> X < 0 || Units [BL [i]] -> Y < 0) continue;
                        ttm = Unit -> TimeToMove (Units [BL [i]] -> X - 1, Units [BL [i]] -> Y + 1);
                        t = Unit -> TimeUnits - ttm;
                        if (t >= utSUPP_SUPPORT && t < mint && ttm > 0) {
                                mint = t;
                                TankPosition [i].x = Units [BL [i]] -> X - 1;
                                TankPosition [i].y = Units [BL [i]] -> Y + 1;
                        }
                        ttm = Unit -> TimeUnits - Unit -> TimeToMove (Units [BL [i]] -> X, Units [BL [i]] -> Y + 1);
                        t = Unit -> TimeUnits - ttm;
                        if (t >= utSUPP_SUPPORT && t < mint && ttm > 0) {
                                mint = t;
                                TankPosition [i].x = Units [BL [i]] -> X;
                                TankPosition [i].y = Units [BL [i]] -> Y + 1;
                        }
                        ttm = Unit -> TimeUnits - Unit -> TimeToMove (Units [BL [i]] -> X + 1, Units [BL [i]] -> Y + 1);
                        t = Unit -> TimeUnits - ttm;
                        if (t >= utSUPP_SUPPORT && t < mint && ttm > 0) {
                                mint = t;
                                TankPosition [i].x = Units [BL [i]] -> X + 1;
                                TankPosition [i].y = Units [BL [i]] -> Y + 1;
                        }
                        
                        ttm = Unit -> TimeUnits - Unit -> TimeToMove (Units [BL [i]] -> X - 1, Units [BL [i]] -> Y);
                        t = Unit -> TimeUnits - ttm;
                        if (t >= utSUPP_SUPPORT && t < mint && ttm > 0) {
                                mint = t;
                                TankPosition [i].x = Units [BL [i]] -> X - 1;
                                TankPosition [i].y = Units [BL [i]] -> Y;
                        }
                        ttm = Unit -> TimeUnits - Unit -> TimeToMove (Units [BL [i]] -> X + 1, Units [BL [i]] -> Y);
                        t = Unit -> TimeUnits - ttm;
                        if (t >= utSUPP_SUPPORT && t < mint && ttm > 0) {
                                mint = t;
                                TankPosition [i].x = Units [BL [i]] -> X + 1;
                                TankPosition [i].y = Units [BL [i]] -> Y;
                        }       

                        ttm = Unit -> TimeUnits - Unit -> TimeToMove (Units [BL [i]] -> X - 1, Units [BL [i]] -> Y - 1);
                        t = Unit -> TimeUnits - ttm;
                        if (t >= utSUPP_SUPPORT && t < mint && ttm > 0) {
                                mint = t;
                                TankPosition [i].x = Units [BL [i]] -> X - 1;
                                TankPosition [i].y = Units [BL [i]] -> Y - 1;
                        }
                        ttm = Unit -> TimeUnits - Unit -> TimeToMove (Units [BL [i]] -> X, Units [BL [i]] -> Y - 1);
                        t = Unit -> TimeUnits - ttm;
                        if (t >= utSUPP_SUPPORT && t < mint && ttm > 0) {
                                mint = t;
                                TankPosition [i].x = Units [BL [i]] -> X;
                                TankPosition [i].y = Units [BL [i]] -> Y - 1;
                        }
                        ttm = Unit -> TimeUnits - Unit -> TimeToMove (Units [BL [i]] -> X + 1, Units [BL [i]] -> Y - 1);
                        t = Unit -> TimeUnits - ttm;
                                if (t >= utSUPP_SUPPORT && t < mint && ttm > 0) {
                                mint = t;
                                TankPosition [i].x = Units [BL [i]] -> X + 1;
                                TankPosition [i].y = Units [BL [i]] -> Y - 1;
                        }
                }
        
                h = 0;
                hmax = 0;
                imax = -1;
                for (i = 0; i < nofBadLife; i++) {
                        if (TankPosition[i].x == MinInt) continue;
                        t = Unit -> TimeUnits;
                        t -= Unit -> TimeToMove (TankPosition [i].x, TankPosition [i].y);
                        t -= utSUPP_SUPPORT;
                        if (t < 0) continue;
                        h = ((TUnit *)Units [BL [i]]) -> MaxFuel - ((TUnit *)Units [BL [i]]) -> Fuel;                   
                        cil = BestPlace (TankPosition [i], UnID, t);
                        h = CFuel * h + PointStatus (UnID, start) - PointStatus (UnID, cil);
                        
                        if (h > hmax) {
                                hmax = h;
                                imax = i;
                        }
                }
                
                if (imax != -1) {
                        UnlockDraw ();
                        // Dojedeme na misto supportu
                        Unit -> Move (TankPosition [imax].x, TankPosition [imax].y);
                        if (DeleteKilled () == FALSE) {
                                RedrawMap ();
                                LockDraw ();   // konec hry 
                                if (TankPosition != NULL) memfree (TankPosition);                               
                                return FALSE;
                        }
                        if (Units [UnID] == NULL) {
                                RedrawMap ();
                                LockDraw (); // Jednotka znicena
                                if (TankPosition != NULL) memfree (TankPosition);                               
                                return TRUE;
                        }

                        // doplnime palivo
                        Unit -> DoSupport (Units [BL [imax]] -> X, Units [BL [imax]] -> Y);
                        if (DeleteKilled () == FALSE) {
                                RedrawMap ();
                                LockDraw ();   // konec hry 
                                if (TankPosition != NULL) memfree (TankPosition);                               
                                return FALSE;
                        }
                        if (Units [UnID] == NULL) {
                                RedrawMap ();
                                LockDraw (); // Jednotka znicena
                                if (TankPosition != NULL) memfree (TankPosition);                               
                                return TRUE;
                        }
                        LockDraw ();
                        
                }
                if (TankPosition != NULL) memfree (TankPosition);
                
        }
        while (Unit -> TimeUnits >= utSUPP_SUPPORT && hmax > 0 && Unit -> Fuel > 0);
        
        // Dojedeme do cile     
        cil = BestPlace (start, UnID, Unit -> TimeUnits);
        UnlockDraw ();
        Unit -> Move (cil.x, cil.y);            
        if (DeleteKilled () == FALSE) {
                RedrawMap ();
                LockDraw ();   // znicena armada, konec hry apod.
                return FALSE;
        }
        if (Units [UnID] == NULL) {
                RedrawMap ();
                LockDraw (); // Jednotka znicena
                return TRUE;
        }
        
        RedrawMap ();
        LockDraw ();    
        Unit -> AI_Info.State = aistPlaced;
        ProgressInc ();
        return TRUE;
}



int TGroundArmy::PJGnom (int UnID)
{
        TPoint *TankPosition;
        int t, ttm, mint, imax, i;
        double h, hmax;
        TPoint start, cil;
        TGnom *Unit;
        int maxammo, ammo, an;
        
        Unit = (TGnom *)Units [UnID];
        Unit -> Select ();

        // Co kdyz dosel benzin
        if (Unit -> Fuel == 0) {
                RedrawMap ();
                Unit -> AI_Info.State = aistPlaced;
                ProgressInc ();
                return TRUE;
        }

        do {    
                start.x = Unit -> X; start.y = Unit -> Y;
                // Naplnime pole TankPosition nejlepsimi pozicemi tankovani vsech BL
                TankPosition = (TPoint *)memalloc (sizeof (TPoint) * nofBadLife);
                for (i = 0; i < nofBadLife; i++) {
                        TankPosition [i].x = MinInt; TankPosition [i].y = MinInt;
                        if (Units [BL [i]] -> Type - BADLIFE == unGnom
                        || Units [BL [i]] -> Type - BADLIFE == unGanymedes) continue;
                        if (!Unit -> WillSupport ((TUnit *)Units [BL [i]])) continue;
                        
                        mint = MaxInt;
                        t = -1;
                        if (Units [BL [i]] -> X < 0 || Units [BL [i]] -> Y < 0) continue;
                        ttm = Unit -> TimeToMove (Units [BL [i]] -> X - 1, Units [BL [i]] -> Y + 1);
                        t = Unit -> TimeUnits - ttm;
                        if (t >= utSUPP_SUPPORT && t < mint && ttm > 0) {
                                mint = t;
                                TankPosition [i].x = Units [BL [i]] -> X - 1;
                                TankPosition [i].y = Units [BL [i]] -> Y + 1;
                        }
                        ttm = Unit -> TimeUnits - Unit -> TimeToMove (Units [BL [i]] -> X, Units [BL [i]] -> Y + 1);
                        t = Unit -> TimeUnits - ttm;
                        if (t >= utSUPP_SUPPORT && t < mint && ttm > 0) {
                                mint = t;
                                TankPosition [i].x = Units [BL [i]] -> X;
                                TankPosition [i].y = Units [BL [i]] -> Y + 1;
                        }
                        ttm = Unit -> TimeUnits - Unit -> TimeToMove (Units [BL [i]] -> X + 1, Units [BL [i]] -> Y + 1);
                        t = Unit -> TimeUnits - ttm;
                        if (t >= utSUPP_SUPPORT && t < mint && ttm > 0) {
                                mint = t;
                                TankPosition [i].x = Units [BL [i]] -> X + 1;
                                TankPosition [i].y = Units [BL [i]] -> Y + 1;
                        }
                        
                        ttm = Unit -> TimeUnits - Unit -> TimeToMove (Units [BL [i]] -> X - 1, Units [BL [i]] -> Y);
                        t = Unit -> TimeUnits - ttm;
                        if (t >= utSUPP_SUPPORT && t < mint && ttm > 0) {
                                mint = t;
                                TankPosition [i].x = Units [BL [i]] -> X - 1;
                                TankPosition [i].y = Units [BL [i]] -> Y;
                        }
                        ttm = Unit -> TimeUnits - Unit -> TimeToMove (Units [BL [i]] -> X + 1, Units [BL [i]] -> Y);
                        t = Unit -> TimeUnits - ttm;
                        if (t >= utSUPP_SUPPORT && t < mint && ttm > 0) {
                                mint = t;
                                TankPosition [i].x = Units [BL [i]] -> X + 1;
                                TankPosition [i].y = Units [BL [i]] -> Y;
                        }       

                        ttm = Unit -> TimeUnits - Unit -> TimeToMove (Units [BL [i]] -> X - 1, Units [BL [i]] -> Y - 1);
                        t = Unit -> TimeUnits - ttm;
                        if (t >= utSUPP_SUPPORT && t < mint && ttm > 0) {
                                mint = t;
                                TankPosition [i].x = Units [BL [i]] -> X - 1;
                                TankPosition [i].y = Units [BL [i]] -> Y - 1;
                        }
                        ttm = Unit -> TimeUnits - Unit -> TimeToMove (Units [BL [i]] -> X, Units [BL [i]] -> Y - 1);
                        t = Unit -> TimeUnits - ttm;
                        if (t >= utSUPP_SUPPORT && t < mint && ttm > 0) {
                                mint = t;
                                TankPosition [i].x = Units [BL [i]] -> X;
                                TankPosition [i].y = Units [BL [i]] -> Y - 1;
                        }
                        ttm = Unit -> TimeUnits - Unit -> TimeToMove (Units [BL [i]] -> X + 1, Units [BL [i]] -> Y - 1);
                        t = Unit -> TimeUnits - ttm;
                                if (t >= utSUPP_SUPPORT && t < mint && ttm > 0) {
                                mint = t;
                                TankPosition [i].x = Units [BL [i]] -> X + 1;
                                TankPosition [i].y = Units [BL [i]] -> Y - 1;
                        }
                }
        
                h = 0;
                hmax = 0;
                imax = -1;
                for (i = 0; i < nofBadLife; i++) {
                        if (TankPosition[i].x == MinInt) continue;
                        t = Unit -> TimeUnits;
                        t -= Unit -> TimeToMove (TankPosition [i].x, TankPosition [i].y);
                        t -= utSUPP_SUPPORT;
                        if (t < 0) continue;
                        h = 0;
                        if (Units [BL [i]] -> Type % BADLIFE < unRadar) {
                                if (((TUnit *)Units [BL [i]]) -> Weapons [0] != NULL) {
                                        maxammo = ((TUnit *)Units [BL [i]]) -> Weapons [0] -> MaxAmmo;
                                        ammo = ((TUnit *)Units [BL [i]]) -> Weapons [0] -> Ammo;
                                        an = ((TUnit *)Units [BL [i]]) -> Weapons [0] -> AttackNum
                                        + ((TUnit *)Units [BL [i]]) -> Weapons [0] -> BonusNum;
                                        h += (maxammo - ammo) / maxammo * an;
                                }
                                if (((TUnit *)Units [BL [i]]) -> Weapons [1] != NULL) {
                                        maxammo = ((TUnit *)Units [BL [i]]) -> Weapons [1] -> MaxAmmo;
                                        ammo = ((TUnit *)Units [BL [i]]) -> Weapons [1] -> Ammo;
                                        an = ((TUnit *)Units [BL [i]]) -> Weapons [1] -> AttackNum
                                        + ((TUnit *)Units [BL [i]]) -> Weapons [1] -> BonusNum;
                                        h += (maxammo - ammo) / maxammo * an;
                                }
                                if (((TUnit *)Units [BL [i]]) -> Weapons [2] != NULL) {
                                        maxammo = ((TUnit *)Units [BL [i]]) -> Weapons [2] -> MaxAmmo;
                                        ammo = ((TUnit *)Units [BL [i]]) -> Weapons [2] -> Ammo;
                                        an = ((TUnit *)Units [BL [i]]) -> Weapons [2] -> AttackNum
                                        + ((TUnit *)Units [BL [i]]) -> Weapons [2] -> BonusNum;
                                        h += (maxammo - ammo) / maxammo * an;
                                }
                                if (((TUnit *)Units [BL [i]]) -> Weapons [3] != NULL) {
                                        maxammo = ((TUnit *)Units [BL [i]]) -> Weapons [3] -> MaxAmmo;
                                        ammo = ((TUnit *)Units [BL [i]]) -> Weapons [3] -> Ammo;
                                        an = ((TUnit *)Units [BL [i]]) -> Weapons [3] -> AttackNum
                                        + ((TUnit *)Units [BL [i]]) -> Weapons [3] -> BonusNum;
                                        h += (maxammo - ammo) / maxammo * an;
                                }
                        }
                        cil = BestPlace (TankPosition [i], UnID, t);
                        h = CFuel * h + PointStatus (UnID, start) - PointStatus (UnID, cil);
                        
                        if (h > hmax) {
                                hmax = h;
                                imax = i;
                        }
                }
                
                if (imax != -1) {
                        UnlockDraw ();
                        // Dojedeme na misto supportu
                        Unit -> Move (TankPosition [imax].x, TankPosition [imax].y);
                        if (DeleteKilled () == FALSE) {
                                RedrawMap ();
                                LockDraw ();   // konec hry 
                                if (TankPosition != NULL) memfree (TankPosition);                               
                                return FALSE;
                        }
                        if (Units [UnID] == NULL) {
                                RedrawMap ();
                                LockDraw (); // Jednotka znicena
                                if (TankPosition != NULL) memfree (TankPosition);                               
                                return TRUE;
                        }

                        // doplnime palivo
                        Unit -> DoSupport (Units [BL [imax]] -> X, Units [BL [imax]] -> Y);                     
                        LockDraw ();
                        
                }
                if (TankPosition != NULL) memfree (TankPosition);
                
        }
        while (Unit -> TimeUnits >= utSUPP_SUPPORT && hmax > 0 && Unit -> Fuel > 0);
        
        // Dojedeme do cile     
        cil = BestPlace (start, UnID, Unit -> TimeUnits);
        UnlockDraw ();
        Unit -> Move (cil.x, cil.y);            
        if (DeleteKilled () == FALSE) {
                RedrawMap ();
                LockDraw ();   // znicena armada, konec hry apod.
                return FALSE;
        }
        if (Units [UnID] == NULL) {
                RedrawMap ();
                LockDraw (); // Jednotka znicena
                return TRUE;
        }
        
        RedrawMap ();
        LockDraw ();    
        Unit -> AI_Info.State = aistPlaced;
        ProgressInc ();
        return TRUE;
}




int TGroundArmy::PJUran (int UnID)
{
        TUran *Unit;
        TPoint start, cil;
        
        Unit = (TUran *)Units [UnID];
        Unit -> Select ();
        start.x = Unit -> X; start.y = Unit -> Y;
        
        // Dojedeme do cile     
        cil = BestPlace (start, UnID, Unit -> TimeUnits);
        UnlockDraw ();
        Unit -> Move (cil.x, cil.y);            
        if (DeleteKilled () == FALSE) {
                RedrawMap ();
                LockDraw ();   // znicena armada, konec hry apod.
                return FALSE;
        }
        if (Units [UnID] == NULL) {
                RedrawMap ();
                LockDraw (); // Jednotka znicena
                return TRUE;
        }
        
        RedrawMap ();
        LockDraw ();    
        Unit -> AI_Info.State = aistPlaced;
        ProgressInc ();
        return TRUE;
        
}



int TGroundArmy::PJRex (int UnID)
{
        TRex *Unit;
        int i, j, t, mint, bestCeres;
        double r, minr;
        TPoint bestplace = TPoint();
        
        Unit = (TRex *)Units [UnID];
        Unit -> Select ();
        if (Unit -> Weapons [0] -> Ammo == 0) 
                Unit -> Weapons [0] -> Ammo = Unit -> Weapons [0] -> MaxAmmo;
        if (Unit -> FlyLevel == 0) {
                UnlockDraw ();
                Unit -> TakeOff (); // Vzletne
                LockDraw ();
        }
        
        if (Unit -> Fuel < (Unit -> MaxFuel / 3)) Unit -> AI_Info.State = aistFuel;
        
        if (Unit -> AI_Info.State == aistFuel) {
                mint = MaxInt;
                for (i = BADLIFE; i < UNITS_TOP; i++) {
                        if (Units [i] != NULL && Units [i] -> Type % BADLIFE == unCeres) {
                                
                                r = 1;
                                do {
                                        for (j = -r; j <= r; j++) {
                                                if (Units [i] -> X + j < 0 || Units [i] -> X + j >= MapSizeX
                                                || Units [i] -> Y - r < 0 || Units [i] -> Y - r >= MapSizeY) continue;
        
                                                t = Unit -> TimeToMove (Units [i] -> X + j, Units [i] -> Y - r);
                                                if (t >= 0 && t < mint 
                                                && Unit -> CanLand (Units [i] -> X + j, Units [i] -> Y - r)) {
                                                        bestplace.x = Units [i] -> X + j;
                                                        bestplace.y = Units [i] -> Y - r;       
                                                        bestCeres = i;
                                                        mint = t;
                                                }
                                        }
                                        for (j = -r; j <= r; j++) {
                                                if (Units [i] -> X + j < 0 || Units [i] -> X + j >= MapSizeX
                                                || Units [i] -> Y + r < 0 || Units [i] -> Y + r >= MapSizeY) continue;
        
                                                t = Unit -> TimeToMove (Units [i] -> X + j, Units [i] -> Y + r);
                                                if (t >= 0 && t < mint
                                                && Unit -> CanLand (Units [i] -> X + j, Units [i] -> Y + r)) {
                                                        bestplace.x = Units [i] -> X + j;
                                                        bestplace.y = Units [i] -> Y - r;       
                                                        bestCeres = i;
                                                        mint = t;
                                                }
                                        }
                                        for (j = -r + 1; j <= r - 1; j++) {
                                                if (Units [i] -> X + r < 0 || Units [i] -> X + r >= MapSizeX
                                                || Units [i] -> Y + j < 0 || Units [i] -> Y + j >= MapSizeY) continue;
        
                                                t = Unit -> TimeToMove (Units [i] -> X + r, Units [i] -> Y + j);
                                                if (t >= 0 && t < mint
                                                && Unit -> CanLand (Units [i] -> X + r, Units [i] -> Y + j)) {
                                                        bestplace.x = Units [i] -> X + r;
                                                        bestplace.y = Units [i] -> Y + j;       
                                                        bestCeres = i;
                                                        mint = t;
                                                }
                                        }
                                        for (j = -r + 1; j <= r - 1; j++) {
                                                if (Units [i] -> X - r < 0 || Units [i] -> X - r >= MapSizeX
                                                || Units [i] -> Y + j < 0 || Units [i] -> Y + j >= MapSizeY) continue;
        
                                                t = Unit -> TimeToMove (Units [i] -> X - r, Units [i] -> Y + j);
                                                if (t >= 0 && t < mint
                                                && Unit -> CanLand (Units [i] -> X - r, Units [i] -> Y + j)) {
                                                        bestplace.x = Units [i] -> X - r;
                                                        bestplace.y = Units [i] -> Y + j;       
                                                        bestCeres = i;
                                                        mint = t;
                                                }
                                        }
                                        r++;
                                
                                } while (r < 3);
                        }
                }
                if (mint == MaxInt) { // Nenalezen Ceres v doletu, leti se k nejblizsimu
                        minr = MaxInt;
                        for (i = BADLIFE; i < UNITS_TOP; i++) {
                                if (Units [i] != NULL && Units [i] -> Type % BADLIFE == unCeres) {
                                        r = sqrt ((Unit -> X - Units [i] -> X)*(Unit -> X - Units [i] -> X)
                                        + (Unit -> Y - Units [i] -> Y)*(Unit -> Y - Units [i] -> Y));
                                        if (r < minr) {
                                                bestplace.x = Units [i] -> X;
                                                bestplace.y = Units [i] -> Y;
                                                minr = r;
                                        }
                                }
                        }
                        if (minr == MaxInt) {
                                Unit -> AI_Info.State = aistNought;
                                return PJGroundFight (UnID);    
                        }
                        else {
                                UnlockDraw ();
                                Unit -> MoveFar (bestplace.x, bestplace.y);
                                if (DeleteKilled () == FALSE) {
                                        RedrawMap ();
                                        LockDraw ();   // znicena armada, konec hry apod.
                                        return FALSE;
                                }
                                if (Units [UnID] == NULL) {
                                        RedrawMap ();
                                        LockDraw (); // Jednotka znicena
                                        return TRUE;
                                }
                                RedrawMap ();
                                LockDraw ();
                                Unit -> AI_Info.State = aistPlaced;
                                ProgressInc ();
                                return TRUE;
                                
                        }
                        
                }
                else { // Ceres v doletu, leti se tankovat
                        UnlockDraw ();
                        Unit -> Move (bestplace.x, bestplace.y);
                        if (DeleteKilled () == FALSE) {
                                RedrawMap ();
                                LockDraw ();   // znicena armada, konec hry apod.
                                return FALSE;
                        }
                        if (Units [UnID] == NULL) {
                                RedrawMap ();
                                LockDraw (); // Jednotka znicena
                                return TRUE;
                        }
                        Unit -> Land ();
                        Unit -> AI_Info.Hps = bestCeres;
                        RedrawMap ();
                        LockDraw ();
                        Unit -> AI_Info.State = aistPlaced;
                        ProgressInc ();
                        return TRUE;    
                }               
        }
        else { 
                return PJGroundFight (UnID);
        }       
        
}


int TGroundArmy::PJMedea (int UnID)
{
        TMedea *Unit;
        int i, t, bestt, tn, ts, te, tw, hp, ha;
        TPoint bp, BuildPlace = TPoint(), start, cil;
        TField *f;
        
        Unit = (TMedea *)Units [UnID];
        Unit -> Select ();
        if (MBPlaces != NULL) {
                do {
                        start.x = Unit -> X; start.y = Unit -> Y;
                        // Vybereme nejblizsi misto na staveni
                        bestt = MaxInt;
                        for (i = 0; MBPlaces [i].x != -1; i++) {
                                f = GetField (MBPlaces [i].x, MBPlaces [i].y);
                                if ((f -> Terrain2 > 0 && f -> Terrain2 < 369) || f -> Terrain2 > 404)
                                        continue;
                                tn = Unit -> TimeToMove (MBPlaces [i].x, MBPlaces [i].y - 1);
                                ts = Unit -> TimeToMove (MBPlaces [i].x, MBPlaces [i].y + 1);
                                tw = Unit -> TimeToMove (MBPlaces [i].x - 1, MBPlaces [i].y);
                                te = Unit -> TimeToMove (MBPlaces [i].x + 1, MBPlaces [i].y);
                                if (tn > -1 && tn < bestt) {
                                        bestt = tn;
                                        BuildPlace = MBPlaces [i];
                                        bp.x = MBPlaces [i].x, bp.y = MBPlaces [i].y - 1;
                                }
                                if (ts > -1 && ts < bestt) {
                                        bestt = ts;
                                        BuildPlace = MBPlaces [i];
                                        bp.x = MBPlaces [i].x, bp.y = MBPlaces [i].y + 1;
                                }
                                if (tw > -1 && tw < bestt) {
                                        bestt = tw;
                                        BuildPlace = MBPlaces [i];
                                        bp.x = MBPlaces [i].x - 1, bp.y = MBPlaces [i].y;
                                }
                                if (te > -1 && te < bestt) {    
                                        bestt = te;
                                        BuildPlace = MBPlaces [i];
                                        bp.x = MBPlaces [i].x + 1, bp.y = MBPlaces [i].y;
                                }
                        }
                        
                        // hodnoceni jen pohybu
                        cil = BestPlace (start, UnID, Unit -> TimeUnits);
                        hp = PointStatus (UnID, start) - PointStatus (UnID, cil);
                        // hodnoceni akce a pohybu
                        if (bestt <= Unit -> TimeUnits - utMD_TIME) {
                                t = Unit -> TimeUnits;
                                t -= Unit -> TimeToMove (bp.x, bp.y);
                                t -= utMD_TIME;
                                cil = BestPlace (bp, UnID, t);          
                                ha = 80 + PointStatus (UnID, start) - PointStatus (UnID, cil);
                        }
                        else {
                                ha = -1;
                        }
                        
                        // Provedeme akci, jestli to stoji za to
                        if (ha >= hp) {
                                UnlockDraw ();
                                Unit -> Move (bp.x, bp.y);
                                if (DeleteKilled () == FALSE) {
                                        RedrawMap ();
                                        LockDraw ();   // konec hry 
                                        return FALSE;
                                }
                                if (Units [UnID] == NULL) {
                                        RedrawMap ();
                                        LockDraw (); // Jednotka znicena
                                        return TRUE;
                                }
                                Unit -> BuildField (BuildPlace.x, BuildPlace.y);
                                if (DeleteKilled () == FALSE) {
                                        RedrawMap ();
                                        LockDraw ();   // konec hry 
                                        return FALSE;
                                }
                                if (Units [UnID] == NULL) {
                                        RedrawMap ();
                                        LockDraw (); // Jednotka znicena
                                        return TRUE;
                                }
                                RedrawMap ();
                                LockDraw ();
                        }
                        else {
                                bestt = MaxInt;
                        }
                                
                } while (bestt != MaxInt);
        }
        
        // Dojedeme na cilovou pozici
        start.x = Unit -> X; start.y = Unit -> Y;       
        cil = BestPlace (start, UnID, Unit -> TimeUnits);
        UnlockDraw ();
        Unit -> Move (cil.x, cil.y);
        if (DeleteKilled () == FALSE) {
                RedrawMap ();
                LockDraw ();   // konec hry 
                return FALSE;
        }
        if (Units [UnID] == NULL) {
                RedrawMap ();
                LockDraw (); // Jednotka znicena
                return TRUE;
        }
        RedrawMap ();
        LockDraw ();
        Unit -> AI_Info.State = aistPlaced;
        ProgressInc ();
        return TRUE;
        
}

int TGroundArmy::PJSkiatron (int UnID)
{
        TPoint start, cil;
        TSkiatron *Unit = (TSkiatron *)Units [UnID];

        Unit -> Select ();      
        start.x = Unit -> X; start.y = Unit -> Y;       
        cil = BestPlace (start, UnID, Unit -> TimeUnits);
        UnlockDraw ();
        Unit -> Move (cil.x, cil.y);
        if (DeleteKilled () == FALSE) {
                RedrawMap ();
                LockDraw ();   // konec hry 
                return FALSE;
        }
        if (Units [UnID] == NULL) {
                RedrawMap ();
                LockDraw (); // Jednotka znicena
                return TRUE;
        }
        RedrawMap ();
        LockDraw ();
        Unit -> AI_Info.State = aistPlaced;
        ProgressInc ();
        return TRUE;
}       


int TGroundArmy::PJIris (int UnID)
{
        TPoint *TankPosition;
        int t, ttm, mint, imax, i;
        double h, hmax;
        TPoint start, cil;
        TIris *Unit;
        
        Unit = (TIris *)Units [UnID];
        Unit -> Select ();

        do {    
                start.x = Unit -> X; start.y = Unit -> Y;
                // Naplnime pole TankPosition nejlepsimi pozicemi tankovani vsech BL
                TankPosition = (TPoint *)memalloc (sizeof (TPoint) * nofBadLife);
                for (i = 0; i < nofBadLife; i++) {
                        TankPosition [i].x = MinInt; TankPosition [i].y = MinInt;
                        if (Units [BL [i]] -> ID == Unit -> ID) continue;
                        if (!Unit -> WillSupport ((TUnit *)Units [BL [i]])) continue;
                        
                        mint = MaxInt;
                        t = -1;
                        if (Units [BL [i]] -> X < 0 || Units [BL [i]] -> Y < 0) continue;
                        ttm = Unit -> TimeToMove (Units [BL [i]] -> X - 1, Units [BL [i]] -> Y + 1);
                        t = Unit -> TimeUnits - ttm;
                        if (t >= utSUPP_SUPPORT && t < mint && ttm > 0) {
                                mint = t;
                                TankPosition [i].x = Units [BL [i]] -> X - 1;
                                TankPosition [i].y = Units [BL [i]] -> Y + 1;
                        }
                        ttm = Unit -> TimeUnits - Unit -> TimeToMove (Units [BL [i]] -> X, Units [BL [i]] -> Y + 1);
                        t = Unit -> TimeUnits - ttm;
                        if (t >= utSUPP_SUPPORT && t < mint && ttm > 0) {
                                mint = t;
                                TankPosition [i].x = Units [BL [i]] -> X;
                                TankPosition [i].y = Units [BL [i]] -> Y + 1;
                        }
                        ttm = Unit -> TimeUnits - Unit -> TimeToMove (Units [BL [i]] -> X + 1, Units [BL [i]] -> Y + 1);
                        t = Unit -> TimeUnits - ttm;
                        if (t >= utSUPP_SUPPORT && t < mint && ttm > 0) {
                                mint = t;
                                TankPosition [i].x = Units [BL [i]] -> X + 1;
                                TankPosition [i].y = Units [BL [i]] -> Y + 1;
                        }
                        
                        ttm = Unit -> TimeUnits - Unit -> TimeToMove (Units [BL [i]] -> X - 1, Units [BL [i]] -> Y);
                        t = Unit -> TimeUnits - ttm;
                        if (t >= utSUPP_SUPPORT && t < mint && ttm > 0) {
                                mint = t;
                                TankPosition [i].x = Units [BL [i]] -> X - 1;
                                TankPosition [i].y = Units [BL [i]] -> Y;
                        }
                        ttm = Unit -> TimeUnits - Unit -> TimeToMove (Units [BL [i]] -> X + 1, Units [BL [i]] -> Y);
                        t = Unit -> TimeUnits - ttm;
                        if (t >= utSUPP_SUPPORT && t < mint && ttm > 0) {
                                mint = t;
                                TankPosition [i].x = Units [BL [i]] -> X + 1;
                                TankPosition [i].y = Units [BL [i]] -> Y;
                        }       

                        ttm = Unit -> TimeUnits - Unit -> TimeToMove (Units [BL [i]] -> X - 1, Units [BL [i]] -> Y - 1);
                        t = Unit -> TimeUnits - ttm;
                        if (t >= utSUPP_SUPPORT && t < mint && ttm > 0) {
                                mint = t;
                                TankPosition [i].x = Units [BL [i]] -> X - 1;
                                TankPosition [i].y = Units [BL [i]] -> Y - 1;
                        }
                        ttm = Unit -> TimeUnits - Unit -> TimeToMove (Units [BL [i]] -> X, Units [BL [i]] -> Y - 1);
                        t = Unit -> TimeUnits - ttm;
                        if (t >= utSUPP_SUPPORT && t < mint && ttm > 0) {
                                mint = t;
                                TankPosition [i].x = Units [BL [i]] -> X;
                                TankPosition [i].y = Units [BL [i]] -> Y - 1;
                        }
                        ttm = Unit -> TimeUnits - Unit -> TimeToMove (Units [BL [i]] -> X + 1, Units [BL [i]] -> Y - 1);
                        t = Unit -> TimeUnits - ttm;
                                if (t >= utSUPP_SUPPORT && t < mint && ttm > 0) {
                                mint = t;
                                TankPosition [i].x = Units [BL [i]] -> X + 1;
                                TankPosition [i].y = Units [BL [i]] -> Y - 1;
                        }
                }
        
                h = 0;
                hmax = 0;
                imax = -1;
                for (i = 0; i < nofBadLife; i++) {
                        if (TankPosition[i].x == MinInt) continue;
                        t = Unit -> TimeUnits;
                        t -= Unit -> TimeToMove (TankPosition [i].x, TankPosition [i].y);
                        t -= utSUPP_SUPPORT;
                        if (t < 0) continue;
                        h = TabCareful [Units [BL [i]] -> Type % BADLIFE] 
                        * (Units [BL [i]] -> MaxHitPoints - Units [BL [i]] -> HitPoints);
                        cil = BestPlace (TankPosition [i], UnID, t);
                        h = h + PointStatus (UnID, start) - PointStatus (UnID, cil);
                        
                        if (h > hmax) {
                                hmax = h;
                                imax = i;
                        }
                }
                
                if (imax != -1) {
                        UnlockDraw ();
                        // Dojedeme na misto supportu
                        Unit -> Move (TankPosition [imax].x, TankPosition [imax].y);
                        if (DeleteKilled () == FALSE) {
                                RedrawMap ();
                                LockDraw ();   // konec hry 
                                if (TankPosition != NULL) memfree (TankPosition);                               
                                return FALSE;
                        }
                        if (Units [UnID] == NULL) {
                                RedrawMap ();
                                LockDraw (); // Jednotka znicena
                                if (TankPosition != NULL) memfree (TankPosition);                               
                                return TRUE;
                        }

                        // doplnime, opravime 
                        Unit -> DoSupport (Units [BL [imax]] -> X, Units [BL [imax]] -> Y);                     
                        LockDraw ();
                        
                }
                if (TankPosition != NULL) memfree (TankPosition);
                
        }
        while (Unit -> TimeUnits >= utSUPP_SUPPORT && hmax > 0 && Unit -> Fuel > 0);
        
        // Dojedeme do cile     
        cil = BestPlace (start, UnID, Unit -> TimeUnits);
        UnlockDraw ();
        Unit -> Move (cil.x, cil.y);            
        if (DeleteKilled () == FALSE) {
                RedrawMap ();
                LockDraw ();   // znicena armada, konec hry apod.
                return FALSE;
        }
        if (Units [UnID] == NULL) {
                RedrawMap ();
                LockDraw (); // Jednotka znicena
                return TRUE;
        }
        
        RedrawMap ();
        LockDraw ();    
        Unit -> AI_Info.State = aistPlaced;
        ProgressInc ();
        return TRUE;
}



int TGroundArmy::PJGanymedes (int UnID)
{
        TPoint *TankPosition;
        int t, ttm, mint, imax, i;
        double h, hmax;
        TPoint start, cil;
        TGanymedes *Unit;
        
        Unit = (TGanymedes *)Units [UnID];
        Unit -> Select ();

        do {    
                start.x = Unit -> X; start.y = Unit -> Y;
                // Naplnime pole TankPosition nejlepsimi pozicemi tankovani vsech BL
                TankPosition = (TPoint *)memalloc (sizeof (TPoint) * nofBadLife);
                for (i = 0; i < nofBadLife; i++) {
                        TankPosition [i].x = MinInt; TankPosition [i].y = MinInt;
                        if (Units [BL [i]] -> Type - BADLIFE == unCeres
                        || Units [BL [i]] -> Type - BADLIFE == unGanymedes
                        || Units [BL [i]] -> Type - BADLIFE == unGnom) continue;
                        if (!Unit -> WillSupport ((TUnit *)Units [BL [i]])) continue;
                        
                        mint = MaxInt;
                        t = -1;
                        if (Units [BL [i]] -> X < 0 || Units [BL [i]] -> Y < 0) continue;
                        ttm = Unit -> TimeToMove (Units [BL [i]] -> X - 1, Units [BL [i]] -> Y + 1);
                        t = Unit -> TimeUnits - ttm;
                        if (t >= utSUPP_SUPPORT && t < mint && ttm > 0) {
                                mint = t;
                                TankPosition [i].x = Units [BL [i]] -> X - 1;
                                TankPosition [i].y = Units [BL [i]] -> Y + 1;
                        }
                        ttm = Unit -> TimeUnits - Unit -> TimeToMove (Units [BL [i]] -> X, Units [BL [i]] -> Y + 1);
                        t = Unit -> TimeUnits - ttm;
                        if (t >= utSUPP_SUPPORT && t < mint && ttm > 0) {
                                mint = t;
                                TankPosition [i].x = Units [BL [i]] -> X;
                                TankPosition [i].y = Units [BL [i]] -> Y + 1;
                        }
                        ttm = Unit -> TimeUnits - Unit -> TimeToMove (Units [BL [i]] -> X + 1, Units [BL [i]] -> Y + 1);
                        t = Unit -> TimeUnits - ttm;
                        if (t >= utSUPP_SUPPORT && t < mint && ttm > 0) {
                                mint = t;
                                TankPosition [i].x = Units [BL [i]] -> X + 1;
                                TankPosition [i].y = Units [BL [i]] -> Y + 1;
                        }
                        
                        ttm = Unit -> TimeUnits - Unit -> TimeToMove (Units [BL [i]] -> X - 1, Units [BL [i]] -> Y);
                        t = Unit -> TimeUnits - ttm;
                        if (t >= utSUPP_SUPPORT && t < mint && ttm > 0) {
                                mint = t;
                                TankPosition [i].x = Units [BL [i]] -> X - 1;
                                TankPosition [i].y = Units [BL [i]] -> Y;
                        }
                        ttm = Unit -> TimeUnits - Unit -> TimeToMove (Units [BL [i]] -> X + 1, Units [BL [i]] -> Y);
                        t = Unit -> TimeUnits - ttm;
                        if (t >= utSUPP_SUPPORT && t < mint && ttm > 0) {
                                mint = t;
                                TankPosition [i].x = Units [BL [i]] -> X + 1;
                                TankPosition [i].y = Units [BL [i]] -> Y;
                        }       

                        ttm = Unit -> TimeUnits - Unit -> TimeToMove (Units [BL [i]] -> X - 1, Units [BL [i]] -> Y - 1);
                        t = Unit -> TimeUnits - ttm;
                        if (t >= utSUPP_SUPPORT && t < mint && ttm > 0) {
                                mint = t;
                                TankPosition [i].x = Units [BL [i]] -> X - 1;
                                TankPosition [i].y = Units [BL [i]] -> Y - 1;
                        }
                        ttm = Unit -> TimeUnits - Unit -> TimeToMove (Units [BL [i]] -> X, Units [BL [i]] -> Y - 1);
                        t = Unit -> TimeUnits - ttm;
                        if (t >= utSUPP_SUPPORT && t < mint && ttm > 0) {
                                mint = t;
                                TankPosition [i].x = Units [BL [i]] -> X;
                                TankPosition [i].y = Units [BL [i]] -> Y - 1;
                        }
                        ttm = Unit -> TimeUnits - Unit -> TimeToMove (Units [BL [i]] -> X + 1, Units [BL [i]] -> Y - 1);
                        t = Unit -> TimeUnits - ttm;
                                if (t >= utSUPP_SUPPORT && t < mint && ttm > 0) {
                                mint = t;
                                TankPosition [i].x = Units [BL [i]] -> X + 1;
                                TankPosition [i].y = Units [BL [i]] -> Y - 1;
                        }
                }
        
                h = 0;
                hmax = 0;
                imax = -1;
                for (i = 0; i < nofBadLife; i++) {
                        if (TankPosition[i].x == MinInt) continue;
                        t = Unit -> TimeUnits;
                        t -= Unit -> TimeToMove (TankPosition [i].x, TankPosition [i].y);
                        t -= utSUPP_SUPPORT;
                        if (t < 0) continue;
                        h = TabCareful [Units [BL [i]] -> Type % BADLIFE] 
                        * (Units [BL [i]] -> MaxHitPoints - Units [BL [i]] -> HitPoints);
                        if (Units [BL [i]] -> Type % BADLIFE < unRadar) {
                                h += ((TUnit *)Units [BL [i]]) -> MaxFuel - ((TUnit *)Units [BL [i]]) -> Fuel;
                        }
                        cil = BestPlace (TankPosition [i], UnID, t);
                        h = h + PointStatus (UnID, start) - PointStatus (UnID, cil);
                        
                        if (h > hmax) {
                                hmax = h;
                                imax = i;
                        }
                }
                
                if (imax != -1) {
                        UnlockDraw ();
                        // Dojedeme na misto supportu
                        Unit -> Move (TankPosition [imax].x, TankPosition [imax].y);
                        if (DeleteKilled () == FALSE) {
                                RedrawMap ();
                                LockDraw ();   // konec hry 
                                if (TankPosition != NULL) memfree (TankPosition);                               
                                return FALSE;
                        }
                        if (Units [UnID] == NULL) {
                                RedrawMap ();
                                LockDraw (); // Jednotka znicena
                                if (TankPosition != NULL) memfree (TankPosition);                               
                                return TRUE;
                        }

                        // doplnime, opravime 
                        Unit -> DoSupport (Units [BL [imax]] -> X, Units [BL [imax]] -> Y);                     
                        LockDraw ();
                        
                }
                if (TankPosition != NULL) memfree (TankPosition);
                
        }
        while (Unit -> TimeUnits >= utSUPP_SUPPORT && hmax > 0 && Unit -> Fuel > 0);
        
        // Dojedeme do cile     
        cil = BestPlace (start, UnID, Unit -> TimeUnits);
        UnlockDraw ();
        Unit -> Move (cil.x, cil.y);            
        if (DeleteKilled () == FALSE) {
                RedrawMap ();
                LockDraw ();   // znicena armada, konec hry apod.
                return FALSE;
        }
        if (Units [UnID] == NULL) {
                RedrawMap ();
                LockDraw (); // Jednotka znicena
                return TRUE;
        }
        
        RedrawMap ();
        LockDraw ();    
        Unit -> AI_Info.State = aistPlaced;
        ProgressInc ();
        return TRUE;
}




///////////////////////////// 6 6 6 ////////////////////////////////

double TGroundArmy::SatanPS (int moverID, TPoint pozice)
{
        int i, r;
        double st;
        TSatan *mover;
        
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
        if (Units [moverID] -> Type - BADLIFE != unSatan) {
#ifdef DEBUG                            
                fprintf (dbgOutput, "Error: PointStatus mover %i not TUnit \n",moverID);                
#endif                          
                AIError ();
                return 0;
        }
        
        mover = (TSatan *)Units [moverID];

        st = 0;
        
        // Deme tvrde po nich
        for (i = 0; i < nofGoodLife; i++) {
                if (Units [GL [i]] -> X < 0 || Units [GL [i]] -> Y < 0) continue;
                if (Units [GL [i]] -> Type >= unIkaros && Units [GL [i]] -> Type <= unRex) continue;
                r = sqrt((pozice.x - Units [GL [i]] -> X) * (pozice.x - Units [GL [i]] -> X) 
                + (pozice.y - Units [GL [i]] -> Y) * (pozice.y - Units [GL [i]] -> Y));

                if (r < mover -> TimeUnits / 5) 
                        st += CToThem * r;
                else 
                        st += CToThemMin * r + (mover -> TimeUnits / 5) * CToThem;
                
        }


        // Aby nejezdily do hajzlu, kdyz to neni potreba
        st += CLazy * sqrt ((pozice.x - mover -> X) * (pozice.x - mover -> X )  
        + (pozice.y - mover -> Y) * (pozice.y - mover -> Y));
        
        // aby nebyly tesne u sebe
        for (i = 0; i < nofBadLife; i++) {
                if (Units [BL [i]] -> AI_Info.State != aistPlaced) continue;
                if (abs (pozice.x - Units [BL [i]] -> X) <= 3 
                && abs (pozice.y - Units [BL [i]] -> Y) <= 3) st += 5 * CDist;
        }
        
        // hledam cil
        for (i = 0; i < nofGoodLife; i++) {

                if (abs (pozice.x - Units [GL [i]] -> X) <= 3 
                && abs (pozice.y - Units [GL [i]] -> Y) <= 3
                && (Units [GL [i]] -> Type < unIkaros || Units [GL [i]] -> Type > unRex))
                        st -= 20;
        }
        
//      if (st < 0) st = 0;
        return (st);
}

TPoint TGroundArmy::SatanBP (TPoint start, int UnID, int Time)
{
        TSatan *Unit;
        TPoint a, besta;
        int r, x, y, t;
        TPoint OldPos;
        double st, bestst;

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
        if (Units [UnID] -> Type - BADLIFE != unSatan) {
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


        
        Unit = (TSatan *)Units [UnID];
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
                                st = SatanPS (UnID, a);
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


int TGroundArmy::PJSatan (int UnID)
{
        TPoint start, cil;
        TSatan *Unit = (TSatan *)Units [UnID];
        int t, i;

        Unit -> Select ();      
        start.x = Unit -> X; start.y = Unit -> Y;       
        cil = SatanBP (start, UnID, Unit -> TimeUnits);
        UnlockDraw ();
        Unit -> Move (cil.x, cil.y);
        if (DeleteKilled () == FALSE) {
                RedrawMap ();
                LockDraw ();   // konec hry 
                return FALSE;
        }
        if (Units [UnID] == NULL) {
                RedrawMap ();
                LockDraw (); // Jednotka znicena
                return TRUE;
        }

        // hledam cil
        t = 0;
        for (i = 0; i < nofGoodLife; i++) {
                if (abs (Unit -> X - Units [GL [i]] -> X) <= 3 
                && abs (Unit -> Y - Units [GL [i]] -> Y) <= 3
                && (Units [GL [i]] -> Type < unIkaros || Units [GL [i]] -> Type > unRex)) 
                t += 1;
        }
        
        if (t != 0) {
                Unit -> GlobalDestruction ();
        }

        if (DeleteKilled () == FALSE) {
                RedrawMap ();
                LockDraw ();   // konec hry 
                return FALSE;
        }
        
        RedrawMap ();
        LockDraw ();
        Unit -> AI_Info.State = aistPlaced;
        ProgressInc ();
        return TRUE;


        Units [UnID] -> AI_Info.State = aistPlaced;
        ProgressInc ();
        return TRUE;
}


int TGroundArmy::PJGargantua (int UnID)
{
        TGargantua *Unit;
        TPoint start, cil;
        
        Unit = (TGargantua *)Units [UnID];
        Unit -> Select ();
        start.x = Unit -> X; start.y = Unit -> Y;
        
        // Dojedeme do cile     
        cil = BestPlace (start, UnID, Unit -> TimeUnits);
        UnlockDraw ();
        Unit -> Move (cil.x, cil.y);            
        if (DeleteKilled () == FALSE) {
                RedrawMap ();
                LockDraw ();   // znicena armada, konec hry apod.
                return FALSE;
        }
        if (Units [UnID] == NULL) {
                RedrawMap ();
                LockDraw (); // Jednotka znicena
                return TRUE;
        }
        
        RedrawMap ();
        LockDraw ();    
        Unit -> AI_Info.State = aistPlaced;
        ProgressInc ();
        return TRUE;
        
}



