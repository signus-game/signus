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
// Hlavickovy soubor pro zbrane
// Obsah: - trida TWeapon, TGun atd.
//


#ifndef _WEAPONS_H
#define _WEAPONS_H


#include "engine.h"



// Ini. a deini. zbrani - volano z InitUnits()
extern int InitWeapons();
extern int DoneWeapons();



// Typy zbrani:
#define wpnEnergy      0
#define wpnGun         1
#define wpnExplos      2
#define wpnMine        3




extern int WeaponAttack(int x, int y, int WpnType, int AtN, int BoN);
            // provede uder na dane policko s urcenym UC

class TWeapon {
        public:
            int AttackNum, BonusNum;
            int MinRange, MaxRange;
            int Ammo,MaxAmmo;
            int TimeLost;
            char *Name;
        
            TWeapon(char *aname, int an, int bn, int tl, int minr, int maxr, int maxam);
            int GetAmmo() {return Ammo;};
            virtual int IsInRange(void *shooter, int fromx, int fromy, int x, int y);
                // vraci jestli se da na dane policko dostrelit
                  // (vcetne terennich prekazek apod.)
            virtual int IsInRangeAI(void *shooter, int fromx, int fromy, int x, int y, int ignore_units);
            virtual int Shoot(void *shooter, int fromx, int fromy, int x, int y);
                  // vystreli na dane policko (+anim)
                  // return -1 kdyz nejsou naboje
            virtual void Read(FILE *f);
            virtual void Write(FILE *f);
                    // cteni a zapis aktualniho stavu zbrane
            virtual int GetType() {return -1;};
            virtual int AnimGetHeight(int fromx, int fromy, int tox, int toy, 
                                      int path_length, int act_pos) {return 0;};
                // vraci vysku projektilu nad terenem behem strelby
            virtual void AnimDraw(int x, int y, int h) {};
                    // vykresli do danych souradnic sprity naboje. x,y jsou v 28nach
                    // policek (tj. policko je slozeno z 28 podbodu). Vyska h
                    // je v 14nach hodnoty Tfield->Height
            virtual void AnimStart(int fromx, int fromy, int tox, int toy) {};
            virtual void AnimStop(int fromx, int fromy, int tox, int toy) {};
                    // ukonci/zahaji strelbu - animaci
                    // pozn: AnimXXXX() metody jsou volany z Shoot()
            virtual void AnimProcess(int fromx, int fromy, int tox, int toy,
                                     int *resx, int *resy, int draw = TRUE);
                // provadi vlastni let naboje. Pokud je draw, kresli se to
                // jinak se pouze pocita (v IsInRange). Misto, kam naboj doletel,
                // se ulozi do resx, resy
            virtual int CanShootThrough(int x, int y);
                // ur?uje, jestli se dan2m pol??kem d? st??let
            virtual int AnimFlyFrom(int length) {return 0;};
                    // urcuje, od kdy je videt projektil.
            virtual int AnimGetDelay() {return iniAnimDelay2 / 8;};
            virtual int ShootResult(int x, int y);
                // zaver strelby - poskozeni cile
            virtual TSprite *AnimGetSprite() {return NULL;};
                    // vraci sprite, ktery se ma kreslit behem strelby
            virtual void IncAN(int delta);
                    // pri postupu na vyssi uroven zmeni UC
            virtual ~TWeapon() {};
    };



// Zbran prime strelby - naboj neni videt:
class TGun : public TWeapon {
        public:
            TGun(char *aname, int an, int bn, int tl, int maxr, int maxam);
            void AnimStart(int fromx, int fromy, int tox, int toy);
            void AnimStop(int fromx, int fromy, int tox, int toy);
            int GetType() {return wpnGun;};
    };  




// bomba - letoun s ni doleti a hodi ji:
class TBomb : public TWeapon {
        public:
            int AttackNum2, BonusNum2;

            TBomb(char *aname, int an, int bn, int an2, int bn2, int tl, int maxam);
            void AnimStart(int fromx, int fromy, int tox, int toy);
            void AnimStop(int fromx, int fromy, int tox, int toy);
            int GetType() {return wpnExplos;};
            void Read(FILE *f);
            void Write(FILE *f);
            int ShootResult(int x, int y);
            void IncAN(int delta);
            void AnimProcess(int fromx, int fromy, int tox, int toy,
                                     int *resx, int *resy, int draw = TRUE);
    };  



/////////////////////////////////////////////
// Zbran prime strelby s anim. nabojem:

class TProjectilWeapon : public TWeapon {
        public:
            TProjectilWeapon(char *aname, int an, int bn, int tl, int minr, int maxr, int maxam);
            int AnimGetHeight(int fromx, int fromy, int tox, int toy, int path_length, int act_pos);
            int GetType() {return wpnGun;};
            void AnimStart(int fromx, int fromy, int tox, int toy);
            void AnimDraw(int x, int y, int h);
            void AnimStop(int fromx, int fromy, int tox, int toy);
            TSprite *AnimGetSprite();
            int AnimFlyFrom(int length) {return length/10;};
    };



class TCannon : public TProjectilWeapon {
        public:
            TCannon(char *aname, int an, int bn, int tl, int maxr, int maxam) : 
                    TProjectilWeapon(aname, an, bn, tl, 1, maxr, maxam) {};
            void AnimStart(int fromx, int fromy, int tox, int toy);
    };  


class TTorpedo : public TCannon {
        public:
            TTorpedo(char *aname, int an, int bn, int tl, int maxr, int maxam) : 
                    TCannon(aname, an, bn, tl, maxr, maxam) {};
            TSprite *AnimGetSprite();
            int CanShootThrough(int x, int y);
    };  


class TCannonAntiAir : public TCannon {
        public:
            TCannonAntiAir(char *aname, int an, int bn, int tl, int maxr, int maxam) : 
                    TCannon(aname, an, bn, tl, maxr, maxam) {};
            int AnimGetHeight(int fromx, int fromy, int tox, int toy, int path_length, int act_pos);
            int ShootResult(int x, int y);
    };  

class TSonic : public TCannon {
        public:
            TSonic(char *aname, int an, int bn, int tl, int maxr, int maxam) : 
                    TCannon(aname, an, bn, tl, maxr, maxam) {};
            int GetType() {return wpnEnergy;};
            void AnimProcess(int fromx, int fromy, int tox, int toy,
                                     int *resx, int *resy, int draw = TRUE);
    };  


class TLaser : public TCannon {
        public:
            TLaser(char *aname, int an, int bn, int tl, int maxr, int maxam) : 
                    TCannon(aname, an, bn, tl, maxr, maxam) {};
            int GetType() {return wpnEnergy;};
            void AnimDraw(int x, int y, int h);
            void AnimStop(int fromx, int fromy, int tox, int toy);
            int AnimFlyFrom(int length);
            void AnimProcess(int fromx, int fromy, int tox, int toy,
                                     int *resx, int *resy, int draw = TRUE);
            TSprite *AnimGetSprite();
    };




/////////////////////////////////////////////
// balisticka zbran:

class TBalisticWeapon : public TProjectilWeapon {
        public:
            int AttackNum2, BonusNum2;

            TBalisticWeapon(char *aname, int an, int bn, int an2, int bn2, int tl, int minr, int maxr, int maxam);
            int AnimGetHeight(int fromx, int fromy, int tox, int toy, int path_length, int act_pos);
            void AnimStop(int fromx, int fromy, int tox, int toy);
            virtual int GetAmplitude(int path_length);
            int AnimFlyFrom(int length) {return length / 3;};
            TSprite *AnimGetSprite();
            void Read(FILE *f);
            void Write(FILE *f);
            int GetType() {return wpnExplos;};
            virtual int ShootResult(int x, int y);
            void IncAN(int delta);
    };


class TBalisticArtefactWeapon : public TBalisticWeapon {
        public:
            TBalisticArtefactWeapon(char *aname, int an, int bn, int an2, int bn2, int tl, int minr, int maxr, int maxam)
                    : TBalisticWeapon(aname, an, bn, an2, bn2, tl, minr, maxr, maxam) {};
            void AnimStop(int fromx, int fromy, int tox, int toy);
    };




// Raketomet:
class TRocket : public TBalisticWeapon {
        public:
            int FlyDirection;
        
            TRocket(char *aname, int an, int bn, int an2, int bn2, int tl, int minr, int maxr, int maxam) : 
                    TBalisticWeapon(aname, an, bn, an2, bn2, tl, minr, maxr, maxam) {FlyDirection = 0;};
            void AnimStart(int fromx, int fromy, int tox, int toy);
            void AnimStop(int fromx, int fromy, int tox, int toy);
            TSprite *AnimGetSprite();
            int GetAmplitude(int path_length) {return path_length / 14;};
            void AnimDraw(int x, int y, int h);
            int AnimGetDelay() {return iniAnimDelay2 / 6;};
            int AnimFlyFrom(int length) {return 56;};
    };  


// Rakety s primou trajektorii:
class TRocketDir : public TRocket {
        public:
            TRocketDir(char *aname, int an, int bn, int an2, int bn2, int tl, int minr, int maxr, int maxam) : 
                    TRocket(aname, an, bn, an2, bn2, tl, minr, maxr, maxam) {};
            int GetAmplitude(int path_length) {return 0;};
    };  


// Protiletadlove rakety:
class TRocketAntiAir : public TRocketDir {
        public:
            TRocketAntiAir(char *aname, int an, int bn, int tl, int minr, int maxr, int maxam) : 
                    TRocketDir(aname, an, bn, 0, 0, tl, minr, maxr, maxam) {};
            int ShootResult(int x, int y);
            int AnimGetHeight(int fromx, int fromy, int tox, int toy, int path_length, int act_pos);
            void AnimStop(int fromx, int fromy, int tox, int toy);
    };



// Minomet: (jako TRocket, jenom leti kulicka a ne raketa)
class TMine : public TBalisticWeapon {
        public:
            TMine(char *aname, int an, int bn, int an2, int bn2, int tl, int minr, int maxr, int maxam) :
                        TBalisticWeapon(aname, an, bn, an2, bn2, tl, minr, maxr, maxam) {};
            int GetAmplitude(int path_length) {return 0;};
            int AnimFlyFrom(int length) {return 0;};
    };  







// Sprity leticich naboju:

extern TSprite *SpriteProjectil;
extern TSprite *SpriteRocket[11];

#endif
