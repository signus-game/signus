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
// Hlavickovy soubor pro jednotky
// Obsah: - trida TObject
//


#ifndef _UNITS_H
#define _UNITS_H


#include "global.h"
#include "engine.h"
#include "ui_toolkit.h"
#include "sound.h"
#include "stream.h"

#include "etables.h"

#include "weapons.h"
#include "visiblty.h"

// idecka jednotek:
#include "unit_id.h"


typedef struct {
    int State;
    TPoint point;
    int Hps;
} TAI_Info;

// Zakladni objekt, jednotky plus budovy:

class TObject {
        public:
            int ID;                               // poradove cislo
            int Type;                                                     // konst. urcuje typ jednotky
            int MaxHitPoints;                     // max. zivotu
            int MaxTimeUnits;                     // max. casu
            int Defense[3];                       // obranna cisla
            int Visib;                            // viditelnost
            
            int X, Y, Alt;                        // pozice jednotky
            int LittleX, LittleY;                 // pozice na policku - pri anim
            int HitPoints;
            int TimeUnits;                                              // zbyvajici cas
            
            int ActualSprite;
            
            int CanChoose;                        // pomoc. pro ikonku select jeddnou za kolo
            int SmokeOn;                          // priznak povoleni kreslit kour
            
            TAI_Info AI_Info;                     // ...

            TObject() {};
            virtual void Init(int x, int y, int party, ReadStream *stream = NULL);
                    // Init() je skutecny konstruktor, proto se musi! volat po k.!

            virtual void Read(ReadStream &stream);
            virtual void Write(WriteStream &stream);
                    // cteni a zapis aktualnich hodnot jednotky
            virtual void WriteInitReq(WriteStream &stream) {};
                    // zapis hodnot ctenych primo v konstruktoru
                    // (vola se vzdy! tesne pred Write()...)
            virtual int GetType() = 0;
                    // vraci typ jednotky unXXXX
            virtual int GetWeight() {return 0;};
                    // vraci hmotnost jednotky (vizHLP)
            virtual TSprite *GetSprite();
                    // bezne vraci sprite ActualSprite, muze byt zmeneno
            TSprite *GetSmoke();
                    // vraci typ koure pri poskozeni
            inline char *GetName(); 
            inline char *GetDescript();
                    // vraci jmeno a popisny text pro tuto jednotku
            void ChangeInstCnt(int delta);
                    // snizi/zvysi pocet instanci daneho typu v tabulkach
            virtual void PaintUnit(int DoCheck = TRUE);
                    // vykresli jednotku na obrazovku (zabezpeci i precnivani)
                    // parametr urcuje, jestli se ma pohled centrovat, kdyz neni 
                    // videt na jednotku
            virtual int PaintUnitInMove(int bx, int by);
                    // vykresli PaintUnit() pro dve sousedni policka - pri pohybu j.
                    // kreslena policka: to s jednotkou a [bx;by]
            virtual void Setup();
                // zakladni nastaveni tabulek jednotky
            virtual void AfterSetup();
                    // vola se po Setupu(), nastavi veci na Max
          virtual void PlaceGround(int place);
                // polozi (place=TRUE) nebo sunda (FALSE) jednotku z mapy
            virtual void Draw();
                // provede vykresleni spritu, bez aktualizace na obrazovce
            virtual void GetDrawRect(TRect *r);
                    // vraci oblast pokreslenou volanim Draw(), ktere by se provedlo
                    // v tomto okamziku
            virtual void Select();
                // zamereni jednotky
            virtual void Deselect();
                // odmereni jednotky
            virtual void TurnReset();
                // vynulovani hodnot na konci kola
            virtual void GetCursor(int x, int y, int *cursor, int *selbold);
                // kurzor pouz. nad danym polickem a typ zvyrazneni policka
                // selbold je TRUE, kdyz ma byt policko zvyrazneno (dostrel apod.)
            virtual void SwitchActionType();
                    // prepne typ kurzoru - alwaysshoot/move ason.
            virtual unsigned GetSupportedActions();
                    // vraci seznam akci (uatXXXX), kterych je tato jednotka schopna
            virtual unsigned GetAvailableActions();
                    // vraci seznam akci, jake je mozno vykonat NAD touto jednotkou
            virtual unsigned GetFieldActions(int x, int y);
                    // vraci kombinaci GSA a GAA
            virtual void FieldSelected(int x, int y);
                    // vola se, kdyz se zmeni zamer. policko
            virtual void Action(int x, int y);
                    // provede akci nad danym polickem
            virtual int MoveRel(int rx, int ry);
                    // relativni pohyb o jedno policko
                    // vraci true, kdyz se objevila nova jednotka,
                    // zabezpeci prekresleni nove objevenych terenu
            virtual int MoveSUB(int x, int y);
                // pohyb na dane policko - vraci true kdyz zije jinak false
            virtual int MoveFarSUB(int x, int y);
                    // pohyb na policko, osetruje i ta prilis vzdalena
                    // vraci: TRUE kdyz dojela,
                    //        -1 kdyz nema dostatek casu - dojela, kam mohla                    
                    //        -2 kdyz mela dojet na nepristupny teren nebo nenalez. cestu
                    //        -3 kdyz doslo palivo
                    //        FALSE kdyz jina chyba
            int Move(int x, int y);
            int MoveFar(int x, int y);

            virtual int Attack(int x, int y) {return 0;};
                // dle svych moznosti zautoci na dane policko
            virtual int TimeToMove(int x, int y);
                    // vraci cas potreb na pohyb. -1 kdyz nejde -2 na nepristupnem
            void PrepareMoveTable();
                    // priprav tabulky por pohyb - volano ze Select()
            virtual void PrepareFieldTime(int x, int y);
                    // vraci interni hodnotu pro policko pro pouz. v Prep.MoveTable
            virtual void GetTerrMove(int **terr, int **terr2) {};
                    // vraci tabulku pohybu pro dany typ jedntotky
            int IsOnScreen();
                // je jednotka cela videt?
            int IsPartOnScreen();
                    // zjisti, jestli je alespon kousek jednotky viditelny
            void Center();
                    // jednotka do stredu obrazovky
            virtual void GetFieldInfo(int x, int y, char *buf);
                    // vraci text, ktery se zobrazi u kurzoru pri delsim cekani
            void ShowUnitInfo();
                    // Zobrazi info o jednotce - virt mthd GetUI
            virtual void GetUnitInfo(bool alt_wpinfo = false);
                    // Vykresli UI do bufferu UInfoBuf
            virtual TDialog *DetailedInfo();
                    // vraci ukazatel na dialog s podrobnym popisem jednotky vcetne
                    // textu a obrazku
            virtual int InfoEvent(TEvent *e);
                    // Zpracuje udalost v infoboxu
            virtual TSprite *GetStatusBar() {return NULL;};
                    // vytvori sprite s ukazatelem zivotu a ammo (nad jednotkou)
            virtual int Damage(int WpnType, int AttackN, int BonusN);
                    // Strelba na jednotku, vraci pocet sebranych zivotu
            virtual void Destroy();
                    // Jednotka znicena, obhospodari to
            virtual void Explode();
                    // volano z Destroy() pro vyvolani nejakeho efektu
            virtual void RemoveFromWorld();
                    // neco jako destruktor - vyjme z mem, z mapy, znici to
                    // (volano po Destroy a pote uz objekt neexistuje)
            virtual int DoVisib();
                    // Provede vypocet viditelnosti jednotky a vraci jestli nasel 
                    // noveho nepritele
            virtual void Rotate(int angle) {};
            virtual void Rotate(int x, int y) {};
                    // natoci jednotku do daneho smeru (uhel 0..7, sourad. kam)
            virtual void RotateRel(int ra) {};
                    // pootoci jednotku jenom o jeden stupen
            virtual void WpnRotate(int x, int y) {};
                    // namiri zbran na policko x, y
            virtual void ShowShootAt(int x, int y, int phase) {};
                    // natoci jednotku a vykresli VYSTRELENI naboje (blize viz. Help)
            virtual void CreateRangeHelpers();
                    // vygeneruje ukazatele dostrelu, dojezdu
            virtual int IsAnimated();
                    // vraci, jestli jednotka ma nejakou animaci. Napr. poskozene
                    // jednotky maji animovany dym apod.
            virtual void DoAnim();
                    // Provadi animaci. Napr. dym
            virtual void ChangeParty();
                    // jednotka prejde na druhou stranu (pri zajmuti zakladny atd)
            MIDASsample GetUnitSound(int sound_id);
                    // vraci zvuk jednotky pro dany typ akce
                    // (0 - zamerena
                    //  1 - pohyb
                    //  2345 - strelba)
            virtual ~TObject();
    };




class TUnit : public TObject {
protected:
	// Special action button in the unit info box
	// e.g. take off/land button for Rex and Caesar
	void draw_special_button(const void *bitmap, unsigned width,
		unsigned height, int apcost);

        public:
            int MaxFuel;                                                  // max. paliva
            
            int Fuel;                             // zbyvajici palivo
            int Orient;                           // orientace jednotky
            int Experience, Level;                // zkusenost a uroven
            
            int WeaponsCnt;                       // Pocet zbrani
            int CurWpn;                           // aktualni zbran (-1 none)
            TWeapon *Weapons[4];                  // seznam zbrani jednotky
            
            int Velocity;                         // real-live sviznost pohybu
            
            TUnit() : TObject() {};
            void Init(int x, int y, int party, ReadStream *stream = NULL);
            void Setup();
            void AfterSetup();      
            void Read(ReadStream &stream);
            void Write(WriteStream &stream);
            void WriteInitReq(WriteStream &stream);
            void GetCursor(int x, int y, int *cursor, int *selbold);
            void Action(int x, int y);
            unsigned GetSupportedActions();
            unsigned GetAvailableActions();
            int MoveRel(int rx, int ry);
            int MoveSUB(int x, int y);
            int MoveFarSUB(int x, int y);
            int Attack(int x, int y);
                  // povede utok na dane policko. Vraci -1 kdyz nejsou naboje,
                  // -2 kdyz neni cas, -3 kdyz mimo dostrel, -4 kdyz na spatnem terenu
                  // 0 kdyz jina chyba, 
                  // pocet sebranych zivotu na OK
            virtual void Rotate(int angle);
            virtual void Rotate(int x, int y);
                    // natoci jednotku do daneho smeru (uhel 0..7, sourad. kam)
            virtual void RotateRel(int ra);
                    // pootoci jednotku jenom o jeden stupen
            virtual void WpnRotate(int x, int y);
            virtual void MakeTrack() {};
                    // vytvori za jednotkou stopu
            void GetUnitInfo(bool alt_wpinfo = false);
            void GetFieldInfo(int x, int y, char *buf);
            TSprite *GetStatusBar();
            void IncExper(int value);
            virtual void IncLevel(int alevel) {};
                    // provede modifikaci parametru v zavis na urovni
            int InfoEvent(TEvent *e);
            void ShowShootAt(int x, int y, int phase);
            void CreateRangeHelpers();
            void FieldSelected(int x, int y);
            virtual void GoOnField();
                    // vola se kdyz jednotka najede na policko
            virtual void GoOnMine();
                    // vola se kdyz jednotka najede na minu
            virtual int CanGoOnField(int x, int y);
                    // vraci, jestli je mozne se vylodit na tohle policko
                    // (tj. jestli tam ta jednotka vubec muze stat = viz. vlaky) 
            int GetRepairCost();
                    // vraci cenu za opravu jednotky
            void Repair();
                    // opravi jednotku
            ~TUnit();
    };




// variace tunit:

class TToweredUnit : public TUnit {
        public:
            int WpnOrient;
        
            TToweredUnit() : TUnit() {};
            void Init(int x, int y, int party, ReadStream *stream);
            void Draw();
            void GetDrawRect(TRect *r);
            void Read(ReadStream &stream);
            void Write(WriteStream &stream);
            void WpnRotate(int x, int y);
            void Rotate(int angle);
            void WpnRot(int ang);
            void WpnRotRel(int r);
            void ShowShootAt(int x, int y, int phase);
    };


class TSupportUnit : public TUnit {
        public:
            void Explode();
            void GetCursor(int x, int y, int *cursor, int *selbold);
            void Action(int x, int y);
            unsigned GetSupportedActions();
            
            virtual void DoSupport(int x, int y);
                // provede zasobovani/opravu jednotky na policku x,y
            virtual void SupportUnit(TUnit *Unit) {};
                // vlastni operace "nalozeni zasob"
            virtual int WillSupport(TUnit *Unit) {return TRUE;}
                // vraci, jestli ma smysl zasobovat tu jednotku
    };






// hmotnosti jednotek:

#define WGT_LIGHT        1
#define WGT_MEDIUM       2
#define WGT_HEAVY        3
#define WGT_ULTRAHEAVY   4



// Fce:

extern int InitUnits();
extern int DoneUnits();

extern void ReadUnits(ReadStream &stream);
        // Nacte ze souboru seznam jednotek
extern void WriteUnits(WriteStream &stream);
        // Zapise do souboru stav jednotek
        
extern void UpdateUnitsMem(int UnType);
        // Kdyz se nejaka jednotka uvolni nebo prida, nactou/uvolni se sprity,
        // popisne texty, jmena, zvuky atd prislusne jednotky (samozrejme jen
        // v pripade, ze byla prvni ci posledni sveho druhu.).


extern void ClearLastDraws();
        // vynuluje pole zaznamu o poslednich kreslenich urcitych jednotek





// Promnene:


#define UNIT_TYPES_COUNT 128
#define UNIT_VARIANTS_COUNT (2 * UNIT_TYPES_COUNT)
#define UNITS_TOP      0x800
        // maximalni pocet polozek v seznamu jednotek
#define GOODLIFE           0
#define BADLIFE        0x400
#define GOODLIFE_TOP   0x400



extern int SelectionHistory[GOODLIFE_TOP];
        // seznam poradi zamerovani jednotek...


// Zaznamy mist, kam se naposledy kreslily jednotky:
extern int LD_used[UNITS_TOP];
extern TRect LD_rect[UNITS_TOP];

// Data pro pritomne objekty jednotek a budov:
extern int UnitsCounts[UNITS_TOP];
        // mnozstvi jednotek daneho typu
extern TSprite **UnitsSprites[UNITS_TOP];
        // sprity prislusici jednotkam
extern void *UnitsTransIcons[GOODLIFE_TOP];
        // obrazky-ikonky zobrazene v transporteru jako tato jednotka
extern void *UnitsPictures[GOODLIFE_TOP];
        // doprovodne obrazky k jednotkym
extern char *UnitsNames[UNITS_TOP];
        // jmena jednotek (i pro Badlife, protoze se hlasi pri utoku)
extern char *UnitsDescripts[GOODLIFE_TOP];
        // popisne texty k jednotkam GODLIFE


extern TObject *SelectedUnit;
        // Aktualne vybrana jednotka
extern TObject *Units[UNITS_TOP];
        // Seznam jednotek
extern TObject **Goodlife;
        // Seznam tech dobrych (ukaz. na Units 0..BADLIFe-1)
extern TObject **Badlife; //=pocitac
        // Seznam tech zlych (ukaz. na Units BADLIFE..UNITS_TOP)


extern int UnitActionType;
        // Typ : urcuje operaci provadenou nad polickem

// konstanty UAT, mj. vracena GetAvailableActions(), GetSupportedActions()
#define uatNothing     0x00000000
#define uatSelect      0x00000001
#define uatMove        0x00000002
#define uatSupport     0x00000004
#define uatLoadIn      0x00000008
#define uatOccupy      0x00000010
#define uatMoveAir     0x00000020
#define uatAttack      0x00000040
#define uatAttackField 0x00000080
#define uatSelectAir   0x00000100
#define uatAttackGood  0x00000200
#define uatAttackAir   0x00000400
#define uatAttackAirG  0x00000800
#define uatSupportAir  0x00001000
#define uatLandHelip   0x00002000
#define uatLandAircr   0x00004000
#define uatBuild       0x00008000
#define uatAttackSub   0x00010000




extern int SelectCrossLock;
    // Indik. zda se ma zobrazovat kriz na zamerene jednotce
    // (promnene udava pocet zamku to zakazujicich)
inline void LockSelectCross() {SelectCrossLock++;}
inline void UnlockSelectCross() {SelectCrossLock--;}

extern int StatusbarLock; 
        // zamyka zobraz ukazatele stavu
extern int UnitInfoLock;
        // zamyka zobraz. unitinfo

//// pomocne zobrazovace:
void ShowHelpers();
void HideHelpers();
    // on/off ukazatel zamer. policka, uk. dostrelu, dojezdu, sel.cross




// Grafika:
extern void *BmpSelected[13];       // zamerovac zamerene jednotky
extern void *UInfoBuf, *UInfoBkgr; // buffer a podklad pro infobox (pravo dole)
extern void *WpnInfoBkg[2]; // podklad pro napis se jmenem zbrane
extern void *LevelBmps[10]; // ikonky jednotlivych urovni zkusenosti
#ifdef DEBUG
extern TSprite *SpriteUniversal, *SpriteUniversalBad;
#endif
extern void *BmpAmmoIcons[4]; // ikonky typu zbrani - Gnom, ...
extern void *BmpBigInventory, // pozadi pro ukazovac obsahu zakladny
            *BmpSmallInventory,
            *BmpMediumInventory;
extern void *BmpRepair[2];    // ikonka na opravovani
extern void *BmpBombing[2];   // ikonka na bombardovani

extern TIcon *InfoIcon;
        // ikonka na informace o jednotce
extern TIcon *IconThorOut, *IconThorIn;
        // ikonky pro raketovou vez Thor
extern TIcon *IconSatan;
        // ikonky pro samo=d minu Satan
extern TIcon *IconXenon, *IconXenon2;
        // ikonky pro minera
extern TIcon *IconTransport;    
        // ik. pro zakladnu, Urana, Gargantuu, Oasu
extern TIcon *IconLand, *IconTakeoff;
        // ikonky pro vrtulniky - pristani/vzlet ze zeme

extern MIDASsample SelectSnd, WpnSelectSnd;
        // zakladni zvuky pri klikani


extern int GoodlifeDeads, BadlifeDeads;
        // Pocet zabitych potvor behem mise

extern int TimeReserve; 
        // priznak vyhrazeni casy na vystrely: 0,1,2,3

extern int *SpriteLocators[UNITS_TOP];
        // tabulka obsahuje specialni informace pro sprity jednotek.
        // u towered units je to ofset veze vuci podstave (viz help)



// Infobox pro jednotky:
#define UINFO_X      (RES_X-118)
#define UINFO_Y      ((iniResolution == SVGA_640x480) ? (RES_Y-217) : (RES_Y-225))
#define UINFO_SX     108
#define UINFO_SY     208


// Jmena typu zbrani
#define WPNNM_COUNT     13

#define WPNNM_CANNON     0
#define WPNNM_MACHGUN    1
#define WPNNM_ROCKETS    2
#define WPNNM_MINES      3
#define WPNNM_AAROCKETS  4
#define WPNNM_AGROCKETS  5
#define WPNNM_AIRCANNON  6
#define WPNNM_AGCANNON   7
#define WPNNM_BOMB       8
#define WPNNM_TORPEDO    9
#define WPNNM_LASER     10
#define WPNNM_SONIC     11
#define WPNNM_ANTISUB   12

extern char *WpnNames[WPNNM_COUNT];



// Inliny:
inline char *TObject::GetName()
{
	if (Type < BADLIFE)
		return UnitsNames[Type];
	else
		return UnitsNames[Type-BADLIFE];
}
inline char *TObject::GetDescript()
{
    return UnitsDescripts[Type];
}




// Zarazeni odvozenych jednotek:
#include "building.h"
#include "uamphi.h"
#include "uground.h"
#include "utowers.h"
#include "utrans.h"
#include "uaircraf.h"
#include "utrains.h"
#include "uships.h"

#endif
