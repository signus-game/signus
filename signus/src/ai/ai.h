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
// Artificial Intelligence of Signus
//
//       Local Intelligence
//
//         Object: TArmy
//
//          Header File
//
// Programmed by Richard Wunsch
//

#ifndef _AI_H
#define _AI_H


#include <math.h>
#include "units.h"
#include "fields.h"


#define pi 3.141592654

//Typy jednotek
#define aiunNaraz    0
#define aiunDalek    1
#define aiunPodpor   2
#define aiunTrans    3

//Typy formaci
#define fcStandart   0

#define MaxInt 32767
#define MinInt -32768


// AI CONSTANTS OF STANDART ARMY
#define C1Destroy     4    // Konstanta jis se nasobi zaporne zivoty
#define C1Banzai      4    // Konstanta, jis se nasobi hodnota utoku
#define C1Together    0.1  // Konstanta soudrznosti jednotek
#define C1ToThem      0.2  // Jak hodne se snazi priblizit nepratelum daleko
#define C1ToThemMin   0.02 // Jak hodne se snazi priblizit nepratelum blizko
#define C1Lazy        0.01 // Konstanta lenosti jednotek
#define C1Dist        5    // Postih za pozici vedle jednotky
#define C1PropexDist 20    // Pocet zapornych hps pro zarazeni do Propexu
#define C1Danger      0.5  // Cim se nasobi nebezp cele army
#define C1Contact     1    // Kontaktni vzdalenost
#define C1Capture     30   // Odecte se pokud je Bumerang vedle budovy
#define C1ToWayPoint  0.5  // Snaha zustat v blizkosti WayPointu
#define C1Fuel        1    // Nasobi se ji pocet doplneneho paliva v relaci s pozici


// AI CONSTANTS OF AGRESIVE ARMY
#define C2Destroy     6    // Konstanta jis se nasobi zaporne zivoty
#define C2Banzai      6    // Konstanta, jis se nasobi hodnota utoku
#define C2Together    0.1  // Konstanta soudrznosti jednotek
#define C2ToThem      0.3  // Jak hodne se snazi priblizit nepratelum daleko
#define C2ToThemMin   0.03 // Jak hodne se snazi priblizit nepratelum blizko
#define C2Lazy        0.01 // Konstanta lenosti jednotek
#define C2Dist        5    // Postih za pozici vedle jednotky
#define C2PropexDist  5    // Pocet zapornych hps pro zarazeni do Propexu
#define C2Danger      0.8  // Cim se nasobi nebezp cele army
#define C2Contact     2    // Kontaktni vzdalenost
#define C2Capture     30   // Odecte se pokud je Bumerang vedle budovy
#define C2ToWayPoint  0.5  // Snaha zustat v blizkosti WayPointu
#define C2Fuel        1    // Nasobi se ji pocet doplneneho paliva v relaci s pozici


// AI CONSTANTS OF MOVING ARMY
#define C3Destroy     4    // Konstanta jis se nasobi zaporne zivoty
#define C3Banzai      4    // Konstanta, jis se nasobi hodnota utoku
#define C3Together    0.05 // Konstanta soudrznosti jednotek
#define C3ToThem      0    // Jak hodne se snazi priblizit nepratelum daleko
#define C3ToThemMin   0    // Jak hodne se snazi priblizit nepratelum blizko
#define C3Lazy        0.01 // Konstanta lenosti jednotek
#define C3Dist        1    // Postih za pozici vedle jednotky
#define C3PropexDist 20    // Pocet zapornych hps pro zarazeni do Propexu
#define C3Danger      0.5  // Cim se nasobi nebezp cele army
#define C3Contact     1    // Kontaktni vzdalenost
#define C3Capture     30   // Odecte se pokud je Bumerang vedle budovy
#define C3ToWayPoint  1    // Snaha zustat v blizkosti WayPointu
#define C3Fuel        1    // Nasobi se ji pocet doplneneho paliva v relaci s pozici


// AI CONSTANTS OF SCATTERED ARMY
#define C4Destroy     6    // Konstanta jis se nasobi zaporne zivoty
#define C4Banzai      6    // Konstanta, jis se nasobi hodnota utoku
#define C4Together    0    // Konstanta soudrznosti jednotek
#define C4ToThem      0    // Jak hodne se snazi priblizit nepratelum daleko
#define C4ToThemMin   0.02 // Jak hodne se snazi priblizit nepratelum blizko
#define C4Lazy        0.01 // Konstanta lenosti jednotek
#define C4Dist        5    // Postih za pozici vedle jednotky
#define C4PropexDist 20    // Pocet zapornych hps pro zarazeni do Propexu
#define C4Danger      0.7  // Cim se nasobi nebezp cele army
#define C4Contact     1    // Kontaktni vzdalenost
#define C4Capture     30   // Odecte se pokud je Bumerang vedle budovy
#define C4ToWayPoint  0.5  // Snaha zustat v blizkosti WayPointu
#define C4Fuel        1    // Nasobi se ji pocet doplneneho paliva v relaci s pozici

// AI CONSTANTS OF ULTRA HYPER AGRESIVE ARMY
#define C5Destroy     6    // Konstanta jis se nasobi zaporne zivoty
#define C5Banzai      10    // Konstanta, jis se nasobi hodnota utoku
#define C5Together    0.1  // Konstanta soudrznosti jednotek
#define C5ToThem      0.4  // Jak hodne se snazi priblizit nepratelum daleko
#define C5ToThemMin   0.03 // Jak hodne se snazi priblizit nepratelum blizko
#define C5Lazy        0.01 // Konstanta lenosti jednotek
#define C5Dist        20   // Postih za pozici vedle jednotky
#define C5PropexDist  5    // Pocet zapornych hps pro zarazeni do Propexu
#define C5Danger      1    // Cim se nasobi nebezp cele army
#define C5Contact     2    // Kontaktni vzdalenost
#define C5Capture     30   // Odecte se pokud je Bumerang vedle budovy
#define C5ToWayPoint  0.7  // Snaha zustat v blizkosti WayPointu
#define C5Fuel        1    // Nasobi se ji pocet doplneneho paliva v relaci s pozici


// AI constants of Unit state
#define aistNought   0
#define aistSleeping 1
#define aistMoving   2
#define aistOrdered  3
#define aistPlaced   4
#define aistPropex   5
#define aistAttacked 6
#define aistFuel     7
#define aistAmmo     8

// AI constants of Army status
#define asSleeping   0
#define asActive     1
#define asDestroyed  2
#define asSuicide    3
typedef struct {
	word Targets[10];
	word nofTargets;
} TAttacker;

typedef struct {
	word Attackers[10];
	word nofAttackers;
} TTarget;

class TArmy {

    public:
    	TPoint WayPoint;                  // Misto, ke kteremu je army vazana
      TRect ActionRadius;               // Akcni radius
      int Status;                       // Sleeping, Active, Destroyed
      int *BL;          		            // Seznam vlozenych jednotek
      int *GL;    			                // Seznam nepratel
      int velikost;                     // Velikost (max dovolena vzdal jed.)
      int nofGoodLife,nofBadLife;       // Number of ...      
      int ActualGLIndex, ActualBLIndex;
      TPoint *PoziceUtoku;  // pole pozic utoku na jednotlive GL pro danou un
      double *DangerArray; // pole nebezpecnosti pro kazdou GoodLife
      int *ProbHitTable;
      TAttacker *Attackers;
      TTarget *Targets;
      double Dangerous;
      TPoint RadarPos;
            

			double CDestroy;     // Konstanta jis se nasobi zaporne zivoty
			double CBanzai;      // Konstanta, jis se nasobi hodnota utoku
			double CTogether;    // Konstanta soudrznosti jednotek
			double CToThem;      // Jak hodne se snazi priblizit nepratelum daleko
			double CToThemMin;   // Jak hodne se snazi priblizit nepratelum blizko
			double CLazy;        // Konstanta lenosti jednotek
			double CDist;        // Postih za pozici vedle jednotky
			double CPropexDist;  // Pocet zapornych hps pro zarazeni do Propexu
			double CDanger;      // Cim se nasobi nebezp cele army
			double CContact;     // Kontaktni vzdalenost
			double CCapture;     // Odecte se pokud je Bumerang vedle budovy
			double CToWayPoint;  // Snaha zustat v blizkosti WayPointu
			double CFuel;        // Nasobi se ji pocet doplneneho paliva v relaci s pozici


            
      TArmy ();
      
      TArmy (FILE *f);
      
      virtual void Save (FILE *f);
      
      virtual void Insert(int Unit);       
        // Vlozi jednotku do armady
      virtual int MakeTurn ();
        // vygeneruje a provede vsechny akce v kole      
      virtual int ProblemJednotky (int Unit);
        // Zjisti a provede nejlepsi akci danou jednotkou
      virtual void MakeAttackPoints (int Unit);
        // Vygeneruje pole PoziceUtoku
      virtual TPoint BestPlace (TPoint start, int UnID, int Time);
        // Vybere pro danou un nejlepsi misto v dosahu
      virtual int ChooseWeapon (int attackerID, int defenderID);
        // Vybere vhodnou zbran
      virtual void MakeDangerArray ();
        // Vygeneruje pole DangerArray
      virtual double AttackStatus (int attacker, int weapon
                                 , int defender, int kolikrat);
        // Vyhodnost utoku
      virtual double ProbHit (int attacker, int weapon, int defender); 
        // Nejpravdepodobnejsi vysledek stretnuti (Monte Carlo)
      virtual void DeleteBL (int index);
      
      virtual void DeleteGL (int index);
      
			virtual int DeleteKilled ();      
			
			virtual void DeleteKilled2 ();
			
			virtual void DoAttackedFields ();
			
			virtual void CountDangerous ();
			
			virtual double PointStatus (int moverID, TPoint pozice);
						            
			virtual void MakeProbHitTable ();
			
			virtual void SetAR (int x1, int y1, int x2, int y2);
			  // Set ActionRadius
			virtual void SetAIConst (int set);
				// Set AI Constants                        
			virtual void Displace ();
			  // Odstrani army z mapy
			virtual void DisplaceGL ();
			  
			virtual void Place ();
			  // Vlozi Army zpet do mapy
			virtual void PlaceGL ();
			  
			virtual int IsAtWP ();
			  // vraci TRUE, je-li army v blizkosti WayPointu
			virtual void SetRadar (int x, int y);
			  // Nasatavi RadarPos
			virtual int IsInVisualRange (int x, int y);
			  // Vraci TRUE, je-li bod na dohled
      
      virtual ~TArmy();

};



extern void AIError ();



extern void InitArtificialIntelligence (int mission);

extern void (*InitAI) ();

extern void LoadArtificialIntelligence (FILE *f);

extern void (*SaveArtificialIntelligence) (FILE *f);

extern int  (*ArtificialIntelligence) ();

extern void	(*DoneArtificialIntelligence) (); 

extern int  (*AssignResult) ();

extern int *(*GetManufacturableUnits) (int FactoryID);



extern void InitAI1 ();
extern int  ArtificialIntelligence1 ();
extern int  AssignResult1 ();
extern int *GetManufacturableUnits1 (int FactoryID);
extern void LoadArtificialIntelligence1 (FILE *f);
extern void SaveArtificialIntelligence1 (FILE *f);
extern void	DoneArtificialIntelligence1 (); 

extern void InitAI2 ();
extern int  ArtificialIntelligence2 ();
extern int  AssignResult2 ();
extern int *GetManufacturableUnits2 (int FactoryID);
extern void LoadArtificialIntelligence2 (FILE *f);
extern void SaveArtificialIntelligence2 (FILE *f);
extern void	DoneArtificialIntelligence2 (); 

extern void InitAI3 ();
extern int  ArtificialIntelligence3 ();
extern int  AssignResult3 ();
extern int *GetManufacturableUnits3 (int FactoryID);
extern void LoadArtificialIntelligence3 (FILE *f);
extern void SaveArtificialIntelligence3 (FILE *f);
extern void	DoneArtificialIntelligence3 (); 

extern void InitAI4 ();
extern int  ArtificialIntelligence4 ();
extern int  AssignResult4 ();
extern int *GetManufacturableUnits4 (int FactoryID);
extern void LoadArtificialIntelligence4 (FILE *f);
extern void SaveArtificialIntelligence4 (FILE *f);
extern void	DoneArtificialIntelligence4 (); 

extern void InitAI5 ();
extern int  ArtificialIntelligence5 ();
extern int  AssignResult5 ();
extern int *GetManufacturableUnits5 (int FactoryID);
extern void LoadArtificialIntelligence5 (FILE *f);
extern void SaveArtificialIntelligence5 (FILE *f);
extern void	DoneArtificialIntelligence5 (); 

extern void InitAI6 ();
extern int  ArtificialIntelligence6 ();
extern int  AssignResult6 ();
extern int *GetManufacturableUnits6 (int FactoryID);
extern void LoadArtificialIntelligence6 (FILE *f);
extern void SaveArtificialIntelligence6 (FILE *f);
extern void	DoneArtificialIntelligence6 (); 

extern void InitAI7 ();
extern int  ArtificialIntelligence7 ();
extern int  AssignResult7 ();
extern int *GetManufacturableUnits7 (int FactoryID);
extern void LoadArtificialIntelligence7 (FILE *f);
extern void SaveArtificialIntelligence7 (FILE *f);
extern void	DoneArtificialIntelligence7 (); 

extern void InitAI8 ();
extern int  ArtificialIntelligence8 ();
extern int  AssignResult8 ();
extern int *GetManufacturableUnits8 (int FactoryID);
extern void LoadArtificialIntelligence8 (FILE *f);
extern void SaveArtificialIntelligence8 (FILE *f);
extern void	DoneArtificialIntelligence8 (); 

extern void InitAI9 ();
extern int  ArtificialIntelligence9 ();
extern int  AssignResult9 ();
extern int *GetManufacturableUnits9 (int FactoryID);
extern void LoadArtificialIntelligence9 (FILE *f);
extern void SaveArtificialIntelligence9 (FILE *f);
extern void	DoneArtificialIntelligence9 (); 

extern void InitAI10 ();
extern int  ArtificialIntelligence10 ();
extern int  AssignResult10 ();
extern int *GetManufacturableUnits10 (int FactoryID);
extern void LoadArtificialIntelligence10 (FILE *f);
extern void SaveArtificialIntelligence10 (FILE *f);
extern void	DoneArtificialIntelligence10 (); 
 
extern void InitAI11 ();
extern int  ArtificialIntelligence11 ();
extern int  AssignResult11 ();
extern int *GetManufacturableUnits11 (int FactoryID);
extern void LoadArtificialIntelligence11 (FILE *f);
extern void SaveArtificialIntelligence11 (FILE *f);
extern void	DoneArtificialIntelligence11 (); 

extern void InitAI12 ();
extern int  ArtificialIntelligence12 ();
extern int  AssignResult12 ();
extern int *GetManufacturableUnits12 (int FactoryID);
extern void LoadArtificialIntelligence12 (FILE *f);
extern void SaveArtificialIntelligence12 (FILE *f);
extern void	DoneArtificialIntelligence12 (); 

extern void InitAI13 ();
extern int  ArtificialIntelligence13 ();
extern int  AssignResult13 ();
extern int *GetManufacturableUnits13 (int FactoryID);
extern void LoadArtificialIntelligence13 (FILE *f);
extern void SaveArtificialIntelligence13 (FILE *f);
extern void	DoneArtificialIntelligence13 (); 

extern void InitAI14 ();
extern int  ArtificialIntelligence14 ();
extern int  AssignResult14 ();
extern int *GetManufacturableUnits14 (int FactoryID);
extern void LoadArtificialIntelligence14 (FILE *f);
extern void SaveArtificialIntelligence14 (FILE *f);
extern void	DoneArtificialIntelligence14 (); 

extern void InitAI15 ();
extern int  ArtificialIntelligence15 ();
extern int  AssignResult15 ();
extern int *GetManufacturableUnits15 (int FactoryID);
extern void LoadArtificialIntelligence15 (FILE *f);
extern void SaveArtificialIntelligence15 (FILE *f);
extern void	DoneArtificialIntelligence15 (); 

extern void InitAI16 ();
extern int  ArtificialIntelligence16 ();
extern int  AssignResult16 ();
extern int *GetManufacturableUnits16 (int FactoryID);
extern void LoadArtificialIntelligence16 (FILE *f);
extern void SaveArtificialIntelligence16 (FILE *f);
extern void	DoneArtificialIntelligence16 (); 

extern void InitAI17 ();
extern int  ArtificialIntelligence17 ();
extern int  AssignResult17 ();
extern int *GetManufacturableUnits17 (int FactoryID);
extern void LoadArtificialIntelligence17 (FILE *f);
extern void SaveArtificialIntelligence17 (FILE *f);
extern void	DoneArtificialIntelligence17 (); 

extern void InitAI18 ();
extern int  ArtificialIntelligence18 ();
extern int  AssignResult18 ();
extern int *GetManufacturableUnits18 (int FactoryID);
extern void LoadArtificialIntelligence18 (FILE *f);
extern void SaveArtificialIntelligence18 (FILE *f);
extern void	DoneArtificialIntelligence18 (); 

extern void InitAI19 ();
extern int  ArtificialIntelligence19 ();
extern int  AssignResult19 ();
extern int *GetManufacturableUnits19 (int FactoryID);
extern void LoadArtificialIntelligence19 (FILE *f);
extern void SaveArtificialIntelligence19 (FILE *f);
extern void	DoneArtificialIntelligence19 (); 



extern int ArtefactTrans;
extern int Base1, Base2, Base3, Base4, Base5, Base6, Base7, Base8;
extern int Radar1, Radar2, Radar3, Radar4, Radar5, Radar6, Radar7, Radar8, Radar9, Radar10, Radar11, Radar12, Radar13, Radar14, Radar15;
extern int Factory1, Factory2, Factory3, Factory4, Factory5, Factory6, Factory7, Factory8;
extern int Airport1, Airport2, Airport3, Airport4, Airport5, Airport6, Airport7, Airport8; 
extern int Heliport1, Heliport2, Heliport3, Heliport4, Heliport5, Heliport6, Heliport7, Heliport8;
extern int Tower1, Tower2, Tower3, Tower4;
extern int Pagoda1, Pagoda2;
extern int Warehouse1, Warehouse2, Warehouse3, Warehouse4;
extern int Olymp1, Olymp2, Olymp3, Olymp4, Olymp5, Olymp6, Olymp7, Olymp8, Olymp9, Olymp10; 
extern int Mohykan1, Mohykan2, Mohykan3, Mohykan4, Mohykan5;
extern int Uran1, Uran2, Uran3, Uran4, Uran5, Uran6, Uran7, Uran8, Uran9, Uran10, Uran11, Uran12, Uran13, Uran14, Uran15;
extern int Xenon1, Xenon2, Xenon3;
extern int Kobra1;
extern int Saturn1, Saturn2;
extern int Caesar1, Caesar2, Caesar3, Caesar4, Caesar5;
extern int Gargantua1, Gargantua2, Gargantua3;
extern int Laguna1, Laguna2, Laguna3;
extern int Oasa1, Oasa2, Oasa3, Oasa4, Oasa5, Oasa6, Oasa7;
extern int Neptun1, Neptun2, Neptun3, Neptun4, Neptun5;
extern int Docks1, Docks2, Docks3, Docks4, Docks5;
extern int Mesias1;
extern int Ganymedes1;



extern int ActivationTurn1, ActivationTurn2, ActivationTurn3, ActivationTurn4;
extern int ActivationTurn5, ActivationTurn6, ActivationTurn7, ActivationTurn8;
extern int ActivationTurn9, ActivationTurn10, ActivationTurn11, ActivationTurn12;
extern int ActivationTurn13, ActivationTurn14;

// General Factory List
extern int GFactoryList [];

extern int DrawLocks;

#endif

