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

//
// Artificial Intelligence of Signus
//
//       Local Intelligence
//
//        Object: TTowers
//
// Programmed by Richard Wunsch
//

#include "aitowers.h"
#include "utowers.h"


TTowers *Towers = NULL;


TTowers::TTowers () : TArmy ()
{

}



TTowers::TTowers (ReadStream &stream) : TArmy (stream)
{

}

int TTowers::DeleteKilled ()
{
        int i;

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

//        if (AssignResult () != 0) return FALSE;

        return TRUE;

}


int TTowers::MakeTurn ()
{
        int i, st;

        if (MusicOn && (!IsMusicPlaying())) JukeboxNext();

        if (nofBadLife == 0) return TRUE;

        if (DeleteKilled () == FALSE) return FALSE;

        for (i = 0; i < nofBadLife; i++) {
                do {
                        st = ProblemJednotky (BL[i]);
                        if (st == FALSE) {
                                ProgressNull ();
                                return FALSE;
                        }
                } while (st == -1);
                ProgressInc ();
        }

        return TRUE;

}



int TTowers::ProblemJednotky (int Unit)
{
        int i, weapon, target, oldHP, damage;
        double hmax;

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

        UnlockDraw ();
        // HORIZONT - nutno vyzkouset
        if (Units [Unit] -> Type % BADLIFE == unHorizont) {
                do {
                        if (DeleteKilled () == FALSE) return FALSE;
                        MakeDangerArray ();
                        hmax = 0;
			target = -1;

                        for (i = 0; i < nofGoodLife; i++) { // Pro vsechny GL
                                ProcessMapAnim ();
                                weapon = ChooseWeapon (Unit, GL [i]);
                                if (weapon == -1) continue;
                                if ((AttackStatus (Unit, weapon, GL [i], 1) > hmax)  // Vyberu nejlepsi cil
                                && (((TUnit *)Units [Unit]) -> Weapons [weapon] ->
                                IsInRange (Units [Unit], Units [Unit]   -> X, Units [Unit] -> Y
                                , Units [GL[i]] -> X, Units [GL[i]] -> Y))) {
                                        hmax = AttackStatus (Unit, weapon, GL [i], 1);
					target = i;
                                }
                        }

                        if (hmax > 0) {  // Nasel jsem cil a de se strilet
                                Units [Unit] -> Select ();
				oldHP = Units[GL[target]]->HitPoints;
				((TUnit*)Units[Unit])->CurWpn = ChooseWeapon(Unit, GL[target]);
				damage = Units[Unit]->Attack(Units[GL[target]]->X, Units[GL[target]]->Y);
                                if (DeleteKilled () == FALSE) {
                                        RedrawMap ();
                                        LockDraw (); // Konec hry
                                        return FALSE;
                                }
                                if (damage >= oldHP || Units [Unit] == NULL) {
                                        RedrawMap ();
                                        LockDraw (); // vez nebo cil znicen
                                        return TRUE;
                                }
                        }

                }
                while ((Units [Unit] -> TimeUnits >=
                ((TUnit *)Units [Unit]) -> Weapons[0] -> TimeLost) && (hmax > 0));
          // hraju si dokud je cas na strelbu a je na co strilet
        }


        // THOR - komplet
        if (Units [Unit] -> Type % BADLIFE == unThor) {
                do {
                        if (DeleteKilled () == FALSE) return FALSE;
                        MakeDangerArray ();
                        hmax = 0;
			target = -1;

                        for (i = 0; i < nofGoodLife; i++) { // pro vsechny GL
                                ProcessMapAnim ();
                                weapon = ChooseWeapon (Unit, GL [i]);
                                if (weapon == -1) continue;
                                if ((AttackStatus (Unit, weapon, GL [i], 1) > hmax)  // vybereme nejlepsi cil
                                && (((TThor *)Units [Unit]) -> Weapons [weapon] -> IsInRange (Units [Unit]
                                , Units [Unit] -> X, Units [Unit] -> Y
                                , Units [GL[i]] -> X, Units [GL[i]] -> Y))) {
                                        hmax = AttackStatus (Unit, weapon, GL [i], 1);
					target = i;
                                }
                        }

                        if ((hmax <= 0) || (((TThor *)Units [Unit]) -> Weapons [0] -> Ammo <= 0)) {
                           // zalezeme, protoze neni na co strilet, nebo dosli naboje
                                if (((TThor *)Units [Unit]) -> IsOverground) {
                                        Units [Unit] -> Select ();
                                        ((TThor *)Units [Unit]) -> GoOverground (0);
                                }
                        }

                        if (hmax > 0) { // Tady se bude strilet!!!
                                if (((TThor *)Units [Unit]) -> IsOverground) {  // je-li venku - pal!
					Units [Unit] -> Select ();
					oldHP = Units[GL[target]]->HitPoints;
					((TUnit*)Units[Unit])->CurWpn = ChooseWeapon(Unit, GL[target]);
					damage = Units[Unit]->Attack(Units[GL[target]]->X, Units[GL[target]]->Y);
                                        if (DeleteKilled () == FALSE) {
                                                LockDraw ();
                                                return FALSE;
                                        }
					if (damage >= oldHP || Units [Unit] == NULL) {
						RedrawMap ();
						LockDraw ();
						return TRUE;
					}
                                }
                                else {  //neni-li venku - vyleze
                                        Units [Unit] -> Select ();
                                        ((TThor *)Units [Unit]) -> GoOverground (1);

					// Cannot enter attack mode in this turn
					if (!((TThor *)Units[Unit])->IsOverground) {
						break;
					}

                                }
                        }
                }

                while ((((TThor *)Units [Unit]) -> TimeUnits >=
                ((TThor *)Units [Unit]) -> Weapons[0] -> TimeLost) && (hmax > 0));
                  // hraju si dokud je cas na strelbu a je na co strilet


        }

        // PAGODA, MINOTAURUS a SPEKTRUM - komplet
        if ((Units [Unit] -> Type % BADLIFE == unPagoda)
        || (Units [Unit] -> Type % BADLIFE == unMinotaurus)
        || (Units [Unit] -> Type % BADLIFE == unSpektrum)) {
                do {
                        if (DeleteKilled () == FALSE) return FALSE;
                        MakeDangerArray ();
                        hmax = 0;
			target = -1;

                        for (i = 0; i < nofGoodLife; i++) { // Pro vsechny GL
                                ProcessMapAnim ();
                                weapon = ChooseWeapon (Unit, GL [i]);
                                if (weapon == -1) continue;

                                if ((AttackStatus (Unit, weapon, GL [i], 1) > hmax)  // Vyberu nejlepsi cil
                                && (((TUnit *)Units [Unit]) -> Weapons [weapon] -> IsInRange (Units [Unit]
                                , Units [Unit] -> X, Units [Unit] -> Y
                                , Units [GL[i]] -> X, Units [GL[i]] -> Y))) {
                                        hmax = AttackStatus (Unit, weapon, GL [i], 1);
					target = i;
                                }
                        }

                        if (hmax > 0) {  // Nasel jsem cil a de se strilet
                                Units [Unit] -> Select ();
				oldHP = Units[GL[target]]->HitPoints;
				((TUnit*)Units[Unit])->CurWpn = ChooseWeapon(Unit, GL[target]);
				damage = Units[Unit]->Attack(Units[GL[target]]->X, Units[GL[target]]->Y);
                                if (DeleteKilled () == FALSE) {
                                        RedrawMap ();
                                        LockDraw ();
                                        return FALSE;
                                }
                                if (damage >= oldHP || Units [Unit] == NULL) {
                                        RedrawMap ();
                                        LockDraw ();
                                        return TRUE;
                                }
                        }

                }
                while ((((TUnit *)Units [Unit]) -> TimeUnits
                >= ((TUnit *)Units [Unit]) -> Weapons[0] -> TimeLost) && (hmax > 0));
          // hraju si dokud je cas na strelbu a je na co strilet
        }

        RedrawMap ();
        LockDraw ();
        return TRUE;
}


