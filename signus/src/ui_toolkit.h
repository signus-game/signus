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
// Hlavickovy soubor pro INTERACT.CPP
// Obsah: iact grafika - ikony, tlacitka, dialogy atd.
//        
//


#ifndef _UI_TOOLKIT_H
#define _UI_TOOLKIT_H


#include "events.h"
#include "briefing.h"


// Inicializace modulu - df jen nacteni nejakych dat
extern int InitInteract();
extern int DoneInteract();


///////////////////////////// IKONA //////////////////////////////////

class TIcon {
        public:
            void *IconPic[2];
            int x, y, w, h;
            char Shortcut;
        
            TIcon(int ax, int ay, int aw, int ah, const char *iconid, char sh = 0);
            void Draw(int isdown = FALSE);
            int Handle(TEvent *e);
            ~TIcon() {DisposeArray(IconPic, 2);};
    };



class TIconPanel {
        public:
            byte *Bmps[6];
            byte *Buf;
            byte *Mask;         
            int IconState[11];
            char *HelpStrs[11];
            int Selected;
        
            TIconPanel();
            void Draw();
            int Handle(TEvent *e);
            void Update();
            void LightsOff();
            int Help();
            ~TIconPanel();
    };





////////////////////////// PANEL MENU /////////////////////////////////


class TMenu {
        public:
            byte *Bmps[3];
            byte *Buf, *Bkg;
            int Selected;
            
            TMenu();
            void Draw(int phase);      // vykresli se na scr
            void Show();               // vysune menu
            void Hide();               // zasune menu
            int Exec();                // provadi volbu polozky v menu
            void Select(int x, int y); // zvyrazni zadane policko
            ~TMenu();
    };





///////////////////////////// DIALOG //////////////////////////////////

// Max. pocet subprvku dialogu:
#define MAX_DLGVIEWS   64

// seznam zakladnich prikazu dialogu:
#define cmCancel  1
#define cmOk      2
#define cmYes     4
#define cmNo      8
#define cmAlways 16


// Obecny predek prvku dialogu:
class TView {
        public:
            int x, y, w, h;
            void *Owner;
            
            TView(int ax, int ay, int aw, int ah) {x = ax, y = ay, w = aw, h = ah; Owner = NULL;};
            virtual int HandleEvent(TEvent *e) {return FALSE;};
                    // osetri udalost, vraci prikaz nebo FALSE
            virtual void Draw() {};
                    // vykresli view
            virtual void Paint(int ay = 0, int ah = -1);
                    // zobrazi view na obrazovce
            virtual ~TView() {};
    };



// Dialog:
class TDialog {
        public:
            int x, y, w, h;
            void *BkgBuf, *DrwBuf;
            TView *Subviews[MAX_DLGVIEWS];
            int SubviewsCnt;
            
            TDialog(int ax, int ay, int aw, int ah, const char *backimg);
            void PaintRect(int ay, int ah);
                    // kresli cast dialogu na obrazovku - volit. meze y1, y2
            void Draw();
                    // kresli dialog vcetne subprvku
            void Insert(TView *v);
                    // vlozi do dialogu prvek 
            virtual int SpecialHandle(TEvent *e, int Cmd);
                    // osetri udalosti ukonceni dlg apod.
            virtual int Exec();
                    // spusti dialog a vraci prislusny prikaz.
            virtual void FadeDlg(int In);
                    // prvni/posledni vykresleni dialogu;
                    
            virtual ~TDialog();
    };
    
extern void *DrwViewBf; extern int DrwViewBfSz;
    // to je k bufferu na kresleni dialogu, smi to pouzivat jenom
    // potomci TView...
    
    
    
    

// Tlacitko:
class TButton : public TView {
        public:
            int Cmd;
            int IsDefault;                 // reaguje na Enter?
            int Position;                  // stisknuto, povoleno
            char *Text;
            void *Bmp;
            
            TButton(int ax, int ay, char *txt, int aCmd, int aIsDef = FALSE,
                    char *aBmp = NULL);
            void Draw();
            int HandleEvent(TEvent *e);
            ~TButton() {if (Bmp) memfree(Bmp);};
    };
    

// Text:
class TStaticText : public TView {
        public:
            const char *Text;                   // vlastni text
            int BigFnt;                   // pouz. font Huge ?
            
            TStaticText(int ax, int ay, int aw, int ah, const char *aTxt, int aBigFnt = FALSE) 
              : TView(ax, ay, aw, ah) {Text = aTxt; BigFnt = aBigFnt;};
            void Draw();
    };


// Zarovnavany text - pro InfoBox:
class TStaticText2 : public TStaticText {
      private:
        int IPosX, IPosY;
        int ISizeX, ISizeY;
      int WordCnt;                                  // Pocet slov
      void *BigDrawBuffer;
      int NumOfLines;
      int NumOfArticles;
      
      
      char *Words[MaxNumOfWords];
      double LineSpace[MaxNumOfLines]; // Velikost mezery v kazde radce
      int WordsTypes[MaxNumOfWords]; // 1: Prvni slovo v odstavci
      
        public:
            TStaticText2(int ax, int ay, int aw, int ah, const char *aTxt);
//           : TStaticText(int ax, int ay, int aw, int ah, char *aTxt, int aBigFnt = FALSE) {};
            virtual void Draw();
    };
    

// Animovany text - postupne se s efektem vypisuje:
class TAnimText : public TStaticText {
        public:
            int Ended;                      // uz se skoncilo vypisovani?
            int Phase;                      // pozice vypisovani
            int Line, OldLine;              // pomocne - akt. radek
            TView *After;                   // vypis. zacne, az se skonci u 
                                            // predchoziho anim. textu - prom. After
            void *OldBmp;                   // puvodni podklad
            
            TAnimText(int ax, int ay, int aw, int ah, char *aTxt, TView *aAfter = NULL);
            void Draw();
            int HandleEvent(TEvent *e);
            ~TAnimText() {if (OldBmp != NULL) memfree(OldBmp);}
    };
    



// Posouvatelny ukazatel:

typedef void (*TGaugeUpdateProc)(int *Value, int Release);

class TBarGauge : public TView {
        public:
            TGaugeUpdateProc UpdatProc;         // proc. se vola pri kazde zmene
            int MaxVal, *Value;
            void *Back;                         // bitmapa pozadi
            
            TBarGauge(int ax, int ay, int aw, int ah, int *aValue, int aMax, TGaugeUpdateProc aUpdat = NULL);
            void Draw();
            int HandleEvent(TEvent *e); 
            ~TBarGauge();
    };



// Checkbox - zaskrtnuti moznosti:

class TCheckBox : public TView {
        public: 
            int *Value;
            char *Text;
            
            TCheckBox(int ax, int ay, int aw, char *aText, int *aValue);
            void Draw();
            int HandleEvent(TEvent *e);
    };



// Radiobutton - volba z moznosti

class TRadioButtons : public TView {
        public:
            char **Items;
            int Cnt;
            int *Selected;
            
      TRadioButtons(int ax, int ay, int aw, char **aitems, int acnt, int *asel);
            void Draw();
      int HandleEvent(TEvent *e);
    };


// Zobrazovac bitmapy:

class TBitmap : public TView {
        public:
            void *Pic;
            int sx, sy, frame;               // frame urcuje, zda delat ram
            int DisposeIt;
            
            TBitmap(int ax, int ay, int aframe, void *aPic, int aSx, int aSy,
                    int aDispose = TRUE);
            void Draw();
            ~TBitmap() {if (DisposeIt) memfree(Pic);}
    };



// Seznam retezcu (listbox):

class TListBox : public TView {
        public:
            char **Data;                // uk. na retezce
            int Cnt;                    // pocet polozek
            int Current;                // pozice zamereneho policko
            int Delta;                  // poz. skrolovaciho sloupku
            int ScrLn;                  // pocet radek viditelnych najednou
            int Cmd;                    // prikaz vyvolany kliknutim na polozku         
            int UpArrow, DownArrow, PosArrow;// pomocne promnene, urcuj. barvu sipek
            void (*FocusProc)(TListBox *l);
            void (*SelectProc)(TListBox *l);
            
            TListBox(int ax, int ay, int aw, int aLns, char *aData[], int aCnt,
                     int aCmd = cmOk, 
                     void (*aFocusProc)(TListBox *l) = NULL, 
                     void (*aSelectProc)(TListBox *l) = NULL);
            void Draw();
            int HandleEvent(TEvent *e);
    };






// Vstupni radka, takova, ktera je jenom jedna v celem dialogu:

class TSingleInput : public TView {
        public:
            int Modified;
            char *Buf;
            int MaxChars;
            int MaxPixels;
            
            TSingleInput(int ax, int ay, char *aBuf, int aMC, int aMP);
            void Draw();
            int HandleEvent(TEvent *e);
    };





extern int PromtBox(const char *text, int buttons);
            // vypise text a ceka na odezvu. pod textem se objevi tlacitka
            // podle toho, jaky je parametr buttons: hodnoty:
            // cmOk | cmCancel
            // cmYes | cmNo
            // cmYes | cmNo | cmAlways
            // cmOk
            
extern int InputBox(const char *promt, char *buf, int maxchars);
            // nabidne vlozit text a pod nim tlacika OK a Cancel



#endif
