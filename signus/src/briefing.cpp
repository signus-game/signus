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

  //********************************//
  // Missions' Briefing for Signus  //
  //       (hypertext viewer)       //
  //   (c) 1997 Valacirca by M.W.   //
  //********************************//



#include "headers.h"

#include "briefing.h"
#include "global.h"
#include "graphio.h"
#include "events.h"
#include "fonts.h"
#include "mouse.h"

#include <SDL_timer.h>

// Fces:
void SetConstants();
void BriefInit(char *FileName);
int  BriefGetEvent();
void BriefHandleEvent(int What);
void BriefScrollUp(int Num);
void BriefScrollDown(int Num);
void BriefRedrawScr();
void BriefDone();
void DrawOnScreen(int DrawBackground);



// Vars(WARS):
char *Words[MaxNumOfWords];

//int LineWordsNum[MaxNumOfLines]; // Pocet slov v kazde radce

double LineSpace[MaxNumOfLines]; // Velikost mezery v kazde radce

int WordsTypes[MaxNumOfWords]; // 1: Prvni slovo v odstavci   
                               // 2: Posledni slovo na radce
                               // 3: Konec
                               // 4: Obrazek
int Links[MaxNumOfWords]; // Cisla odkazu na Lnk-files
int Links2[MaxNumOfLinks]; // Cisla odkazu na Lnk-files(tak, jak dou po sobe)

TRect LinksXY[MaxNumOfLinks];

char *LinksFiles[MaxNumOfLinks];

char *PicFiles[MaxNumOfPics];
TPoint PicSize[MaxNumOfPics];

char *LinksSeq[MaxNumOfJumps];

int LinksSeqCnt = 0;

void *BigDrawBuffer;

int SizeOfBigBuf;

int WordCnt; // Pocet slov

int NumOfLines;

int NumOfArticles;

int Ofset; // Posunuti scrollingu

int LastLink;

int LastPic; // Pocet obrazku

int BigBufLines;


// Consts:
int LinePixels;
int MinNumOfYPixels;
int EXIT_X;
int EXIT_Y;
int EXIT_W;
int EXIT_H;
int BACK_X;
int BACK_Y;
int BACK_W;
int BACK_H;
int SCUP_X;
int SCUP_Y;
int SCUP_W;
int SCUP_H;
int SCDN_X;
int SCDN_Y;
int SCDN_W;
int SCDN_H;



// Fces:
void SetConstants()
{
    int IR ;
    
  LinePixels = RES_X-(LeftSpace+RightSpace);
    if (iniResolution-0x0100 == 1) IR =   0;
    if (iniResolution-0x0100 == 3) IR = 120;
    if (iniResolution-0x0100 == 5) IR = 288;
    
  MinNumOfYPixels = RES_Y - UpSpace - DownSpace;//544
  
  EXIT_X = 15;
  EXIT_Y = 152 + IR;
  EXIT_W = 91;
  EXIT_H = 91;
  
  BACK_X = 15;
  BACK_Y = 262 + IR;
  BACK_W = 91;
  BACK_H = 91;
  
  
  SCUP_X = 15;
  SCUP_Y = 371 + IR;
  SCUP_W = 32;
  SCUP_H = 91;


  SCDN_X = 71;
  SCDN_Y = 371 + IR;
  SCDN_W = 32;
  SCDN_H = 91;
    
}



void BriefGo(char *MissionName)
{
    SetConstants();
    LinksSeq[0] = (char*)memalloc(8);
    
    strcpy(LinksSeq[0], MissionName);
    
    BriefInit(LinksSeq[0]);

  DrawOnScreen(1);
  FadeIn(Palette, 0);
  int BriefEvent; 
    // -7: Exit, -4: Up, -3: Down, -2: PgUp, -1: PgDown, 1..n: Odkazy HT
    do {
      BriefEvent = BriefGetEvent();
      BriefHandleEvent(BriefEvent);
  } while (BriefEvent != -7);
  
    BriefDone();
    FadeOut(Palette, 0);
    {
        TEvent e;
        do {GetEvent(&e);} while (e.What != evNothing);
    }
}




void BriefInit(char *FileName)
{
  // Vars:  
    int Exit = 0;
    int CharCnt = 0;
    int WordCharCnt = 0;
    int i;
    int Type = 0;
    int LinkNum = 0;
    int LinkCnt;
    
    char *Character;
    char *Word;
    char *Brief;
    char *StrLinkNum;
    
    Ofset = 0;
    LastLink = 0;
    WordCnt = 0;
    LastPic = 0;
    
    Word = (char*)memalloc(32);
    StrLinkNum = (char*)memalloc(4);
    Character = (char*)memalloc(1);

    //nacte briefing ze souboru
    Brief = (char*) TextsDF->get(FileName);
        //vynuluje Links & WordsTypes
    for (i = 0; i < MaxNumOfWords; i++) { Links[i] = 0; WordsTypes[i] = 0;}
    for (i = 0; i < MaxNumOfLines; i++) LineSpace[i] = 0;
    
    
    
    // vytvori seznam slov (priradi zacatky odstavcu a odkazy)
    do {
    *Character = Brief[CharCnt]; CharCnt++;

        // eliminuje ENTER
        if (*Character == 10) {         
      *Character = Brief[CharCnt]; CharCnt++;
      if (*Character == 13) {
        *Character = Brief[CharCnt]; CharCnt++;
      }
        }
        
        // zjisti specialni znak
        if (*Character == '#') {            
      *Character = Brief[CharCnt]; CharCnt++;
      
          if (*Character == '>') {  // new article
        *Character = Brief[CharCnt]; CharCnt++;
          Type = 1;
          } else
          
          
          if (*Character == 'e') {  // end of text
        *Character = Brief[CharCnt]; CharCnt++;
            Exit = 1;
          } else
          
          if (*Character == '0') {  // link file
        *Character = Brief[CharCnt]; CharCnt++;
          if (*Character == '0') {
           *Character = Brief[CharCnt]; CharCnt++;
               StrLinkNum[0] = *Character;
               StrLinkNum[1] = '\0';
          } else {
             StrLinkNum[0] = *Character;
           *Character = Brief[CharCnt]; CharCnt++;
               StrLinkNum[1] = *Character;
               StrLinkNum[2] = '\0';
          }
            LinkNum = (int)atol(StrLinkNum);
            
            
            Links[WordCnt] = (int)LinkNum;
          
        *Character = Brief[CharCnt]; CharCnt++;
          }
        }
        
        // dalsi slovo
        if ((*Character == ' ') || (Exit != 0)) {
          // ulozeni jednoho slova
          Word[WordCharCnt] = ' ';
          Word[WordCharCnt+1] = '\0';
          Words[WordCnt] = (char*)memalloc(strlen(Word)+1);
          strcpy(Words[WordCnt], Word);
          WordCharCnt = 0;
//        Links[WordCnt] = LinkNum; LinkNum = 0;
          WordsTypes[WordCnt+1] = Type; Type = 0;
          
            WordCnt++;
      *Character = Brief[CharCnt]; CharCnt++;
        }
//      NEXTWORD:
        
        // nacpe dalsi znak do aktualniho slova
        Word[WordCharCnt] = *Character;
        WordCharCnt++;
        
  } while (Exit == 0);
  
  // WordCnt ... Pocet slov
  
  // WordsTypes::    1: First in article, 
  //                 2: Last in line(number of w. in line)
  //                 3: End
  //                 3: Picture
  

  ///////////////////////////////////////////////////////////////
  // Nacte jmena Link-fajluu
  
  WordCharCnt = 0;
  LinkCnt = 1;
  Exit = 0;
  
    do {
    *Character = Brief[CharCnt]; CharCnt++;

        // eliminuje ENTER
        if (*Character == 10) {         
      *Character = Brief[CharCnt]; CharCnt++;
      if (*Character == 13) {
        *Character = Brief[CharCnt]; CharCnt++;
      }
        }
        
        // zjisti specialni znak
        if (*Character == '#') {            
      *Character = Brief[CharCnt]; CharCnt++;
      
          if (*Character == 'e') {
        *Character = Brief[CharCnt]; CharCnt++;
            Exit = 1;
            
          } else
          if (*Character == 'l') {
        *Character = Brief[CharCnt]; CharCnt++; // nacetl mezeru
        do {
            *Character = Brief[CharCnt]; CharCnt++;
              // nacpe dalsi znak do link-fajl-nejmu
            Word[WordCharCnt] = *Character;
            WordCharCnt++;
        } while (*Character != '#');
          // ulozeni jednoho slova
          Word[WordCharCnt-1] = '\0';
          LinksFiles[LinkCnt] = (char*)memalloc(strlen(Word)+1);
          strcpy(LinksFiles[LinkCnt], Word);
          WordCharCnt = 0;
          
            LinkCnt++;
          } 
        }
        
  } while (Exit == 0);

  /////////////////////////////////////////////////
  // Detekuje obrazky
  int WordsChCnt = 1;
  int AllPicsSizeY = 0;
  WordCharCnt = 0;
  
  
  for (i = 0; i < WordCnt; i++) {
    // Test pochybnejch slov
    if ((strcmp(Words[i], " ") == 0) || (strcmp(Words[i], "  ") == 0))
      Words[i][0] = '\0';
    
    int dummy = strlen(Words[i])-2;
    if (Words [i] [dummy] == ' ')
      Words [i] [dummy+1] = '\0';
      
    
    if (Words[i][0] == '#') {
        WordsChCnt = 1;
      *Character = Words[i][WordsChCnt]; WordsChCnt++;
    
        if (*Character == 'p') {  // picture
            LastPic++;
          
        *Character = Words[i][WordsChCnt]; WordsChCnt++;
        
              // ulozeni jmena obrazku
        do {
          *Character = Words[i][WordsChCnt]; WordsChCnt++;
            Word[WordCharCnt] = *Character;
            WordCharCnt++;
        } while (*Character != '.');
        
          Word[WordCharCnt-1] = '\0';
          PicFiles[LastPic] = (char*)memalloc(strlen(Word)+1);
          strcpy(PicFiles[LastPic], Word);
          WordCharCnt = 0;
          
              // ulozeni rozmeru X
        do {
          *Character = Words[i][WordsChCnt]; WordsChCnt++;
            Word[WordCharCnt] = *Character;
            WordCharCnt++;
        } while (*Character != '.');
        
          Word[WordCharCnt-1] = '\0';
            PicSize[LastPic].x = (int)atol(Word);
          WordCharCnt = 0;
              // ulozeni rozmeru Y
        do {
          *Character = Words[i][WordsChCnt]; WordsChCnt++;
            Word[WordCharCnt] = *Character;
            WordCharCnt++;
        } while (*Character != '.');
          Word[WordCharCnt-1] = '\0';
            PicSize[LastPic].y = (int)atol(Word);
          WordCharCnt = 0;
          Words[i][0] = '\0';
          WordsTypes[i] = 4;
          
          AllPicsSizeY += PicSize[LastPic].y + 16;
          }
      }
  }




    
  ///////////////////////////////////////////////////////////////
  //  Provede analizu delky slov a zjisti posledni na radkach
  
  
    
  int LineLength = 0;
  int LineWordCnt = 0;
  int LastWordLength;
  int FirstInArt = 0;
  
  NumOfArticles = 0;
  NumOfLines = -1;
  
  for (i=0; i<WordCnt; i++) {
    
    // Dalsi odstavec
    if (WordsTypes[i] == 1) {
        NumOfLines++;
        LineLength = GetStrWidth("  ", NormalFont);
        LineWordCnt = 0;
        NumOfArticles++;
        FirstInArt = 1;
    }
    
    LastWordLength = GetStrWidth(Words[i], NormalFont);
    LineLength += LastWordLength;
    LineWordCnt++;
    
    // Preteceni pres velikost jedne radky
    if (LineLength >= LinePixels) {
        LineWordCnt--;
        i--;
        LineLength -= LastWordLength;
        
        WordsTypes[i] = 2;
            LineSpace[NumOfLines] = (double)(LinePixels-LineLength)/(double)(LineWordCnt-1);
        
        FirstInArt = 0;
        LineLength = 0;
        LineWordCnt = 0;
        NumOfLines++;
    }
  }
  
  
  if (LineWordCnt > 0) {
    NumOfLines++;
  }
  WordsTypes[WordCnt-1] = 3;
  
  
  BigBufLines = NumOfLines*16+(NumOfArticles-1)*ArticleSpace + AllPicsSizeY;
  
  if (BigBufLines < MinNumOfYPixels) BigBufLines = MinNumOfYPixels+2;
  
  SizeOfBigBuf = LinePixels*BigBufLines;
  BigDrawBuffer = memalloc(SizeOfBigBuf);
  memset(BigDrawBuffer, 0, SizeOfBigBuf);
  memfree(Brief);
    memfree(Word);
    memfree(StrLinkNum);
    memfree(Character);
}


// GET EVENT

int BriefGetEvent()
{

    int x,y;
    TEvent Ev;
    GetEvent(&Ev);

  // Scrolling (mouse)
  if (Ev.What == evNothing) {
    if (IsInRect(Mouse.x, Mouse.y, LeftSpace, RES_Y-DownSpace, RES_X-RightSpace, RES_Y)) 
        return -5;
    if (IsInRect(Mouse.x, Mouse.y, LeftSpace, 0, RES_X-RightSpace, UpSpace)) 
        return -6;
  }
    if (Mouse.buttons == mbLeftButton) {
            // Kliknuti na Exit
        if (IsInRect(Mouse.x, Mouse.y, EXIT_X, EXIT_Y, EXIT_X + EXIT_W-1, EXIT_Y + EXIT_H-1)) 
        {
            return -7;
      }
            // Kliknuti na Back
        if (IsInRect(Mouse.x, Mouse.y, BACK_X, BACK_Y, BACK_X + BACK_W-1, BACK_Y + BACK_H-1)) 
        {
            return -8;
      }
      
            // Kliknuti na UP
        if (IsInRect(Mouse.x, Mouse.y, SCUP_X, SCUP_Y, SCUP_X + SCUP_W-1, SCUP_Y + SCUP_H-1)) 
        {
            return -4;
      }
      
            // Kliknuti na DOWN
        if (IsInRect(Mouse.x, Mouse.y, SCDN_X, SCDN_Y, SCDN_X + SCDN_W-1, SCDN_Y + SCDN_H-1)) 
        {
            return -3;
      }
    }



    if (Ev.What == evMouseDown) {
        if (Ev.Mouse.Buttons == mbRightButton) {
            return -8;
        } else
        

        if (Ev.Mouse.Buttons == mbLeftButton) {
            // Kliknuti na Exit
        if (IsInRect(Ev.Mouse.Where.x, Ev.Mouse.Where.y, EXIT_X, EXIT_Y, EXIT_X + EXIT_W-1, EXIT_Y + EXIT_H-1)) 
        {
            return -7;
      }
            // Kliknuti na Back
        if (IsInRect(Ev.Mouse.Where.x, Ev.Mouse.Where.y, BACK_X, BACK_Y, BACK_X + BACK_W-1, BACK_Y + BACK_H-1)) 
        {
            return -8;
      }
      
            // Kliknuti na UP
        if (IsInRect(Ev.Mouse.Where.x, Ev.Mouse.Where.y, SCUP_X, SCUP_Y, SCUP_X + SCUP_W-1, SCUP_Y + SCUP_H-1)) 
        {
            return -4;
      }
      
            // Kliknuti na DOWN
        if (IsInRect(Ev.Mouse.Where.x, Ev.Mouse.Where.y, SCDN_X, SCDN_Y, SCDN_X + SCDN_W-1, SCDN_Y + SCDN_H-1)) 
        {
            return -3;
      }
      
      // Kliknuti do oblasti textu
        if (IsInRect(Ev.Mouse.Where.x, Ev.Mouse.Where.y, LeftSpace, UpSpace, RES_X-RightSpace, RES_Y-DownSpace)) 
        {
            x = Ev.Mouse.Where.x       -LeftSpace;
            y = Ev.Mouse.Where.y+Ofset-UpSpace;
            int i;
            for (i = 1; i <= LastLink; i++) {
                if ((x>=LinksXY[i].x1) && (y>=LinksXY[i].y1)&&
                    (x<=LinksXY[i].x2) && (y<=LinksXY[i].y2)) 
                {
                  return Links2[i];
              }
            }
      }
      
      
        }
    }

    

    // Udalosti z klavesnice:   
    else if (Ev.What == evKeyDown) {
/*      HandleIcons(e);
        SelectedUnit->InfoEvent(e);*/
        
        switch (Ev.Key.KeyCode) {
            case kbEsc : {
              if (LinksSeqCnt > 0) return -9;
              if (LinksSeqCnt == 0) return -7;
            }
            case kbEnter : {
              return -7;
            }
            case kbUp :
              return -4;
            case kbDown :
              return -3;
            case kbPgUp :
              return -2;
            case kbPgDn :
              return -1;
            case kbBack :
              return -8;
    }
    }
    return 0;
}

void BriefHandleEvent(int What) 
// -7: Exit, -4: Up, -3: Down, -2: PgUp, -1: PgDown, 1..n: Odkazy HT
{
    int i;
    SDL_Delay(2);
  switch (What) {
    case -7 : break;
    case -6 : BriefScrollUp(6); break;
    case -5 : BriefScrollDown(6); break;
    case -4 : BriefScrollUp(32); break;
    case -3 : BriefScrollDown(32); break;
    case -2 : BriefScrollUp(MinNumOfYPixels); break;
    case -1 : BriefScrollDown(MinNumOfYPixels);break;
    case -8 : {
        if (LinksSeqCnt < 1) return;
        memfree(LinksSeq[LinksSeqCnt]);
      LinksSeqCnt--;
     
      BriefDone();
      BriefInit(LinksSeq[LinksSeqCnt]);
      DrawOnScreen(0);
      break;
    }
    case -9 : {
        if (LinksSeqCnt < 1) return;
        for (i = 1; i <= LinksSeqCnt; i++) memfree(LinksSeq[i]);
      LinksSeqCnt = 0;
     
      BriefDone();
      BriefInit(LinksSeq[LinksSeqCnt]);
      DrawOnScreen(0);
      break;
    }
  }
  
  if ((What >= 1) && (What <= LastLink)) {
    // Hyper-textovej odkaz
    

      if ( (LinksSeqCnt > 0) && (strcmp(LinksSeq[LinksSeqCnt-1], LinksFiles[What]) == 0)) {
        
        if (LinksSeqCnt < 1) return;
        memfree(LinksSeq[LinksSeqCnt]);
      LinksSeqCnt--;
     
      BriefDone();
      BriefInit(LinksSeq[LinksSeqCnt]);
      DrawOnScreen(0);
        
      } else {
      LinksSeqCnt++;
      LinksSeq[LinksSeqCnt] = (char*)memalloc(8);
      strcpy(LinksSeq[LinksSeqCnt], LinksFiles[What]);
      if (What > 2) {
        What--;
      }
    }

    
    BriefDone();
    BriefInit(LinksSeq[LinksSeqCnt]);
    DrawOnScreen(0);
  }
  
}


void DrawOnScreen(int DrawBackground)
{
    int i;
    int WordLineCnt;
    int LocalWordCnt = 0;
    int LocalPicCnt = 0;
    int PixColCnt = 0;
    int ArtCnt = -1; // Citac odstavcu
    
    double PixLineCnt;
    
    if (DrawBackground != 0) {
    void *ptr;
    char buf[20];
    sprintf(buf, "%ibriefbk", iniResolution - 0x0100);
    ptr = GraphicsDF->get(buf);
    MouseHide();
    DrawPicture(ptr);
    MouseShow();
    memfree(ptr);
  }
  
  for (i = 0; i <= NumOfLines; i++) {
    WordLineCnt = -1;
    PixLineCnt = 0;
    
    do {
      WordLineCnt++;
        LocalWordCnt++;
        
      if (WordsTypes[LocalWordCnt] == 1) {
            if (LocalWordCnt != 1) i++;
            ArtCnt++;
        PixLineCnt = GetStrWidth("  ", NormalFont);
        if (LocalWordCnt != 1) PixColCnt += ArticleSpace*2;
        WordLineCnt = 0;
        }
      if (Links[LocalWordCnt] == 0) 
        PutStr (BigDrawBuffer, LinePixels, PixLineCnt, PixColCnt,
                Words[LocalWordCnt], NormalFont, clrWhite, clrBlack);    
      else {
        PutStr (BigDrawBuffer, LinePixels, PixLineCnt, PixColCnt,
                Words[LocalWordCnt], NormalFont, clrRed, clrBlack);    
        LastLink++;
        LinksXY[LastLink].x1 = PixLineCnt;
        LinksXY[LastLink].x2 = PixLineCnt+GetStrWidth(Words[LocalWordCnt],NormalFont);
        LinksXY[LastLink].y1 = PixColCnt;
        LinksXY[LastLink].y2 = PixColCnt + 16;
        Links2[LastLink] = Links[LocalWordCnt];
      }
      
      if (WordsTypes[LocalWordCnt] == 4) {
        // Obrazky (jee...)
        LocalPicCnt++;
        
        void *ptr;
        ptr = GraphicsDF->get(PicFiles[LocalPicCnt]);
        PixColCnt += 32;
        CopyBmpNZ(BigDrawBuffer, LinePixels, (LinePixels-PicSize[LocalPicCnt].x)/2, 
                  PixColCnt, ptr, PicSize[LocalPicCnt].x, PicSize[LocalPicCnt].y);
        PixColCnt += (PicSize[LocalPicCnt].y-16);
        
        memfree(ptr);
      }
            
        PixLineCnt += (double)GetStrWidth(Words[LocalWordCnt], NormalFont);
        PixLineCnt += LineSpace[i];
        
        if (WordsTypes[LocalWordCnt] == 3) goto LastWord;
                
    } while (WordsTypes[LocalWordCnt] != 2);
    PixColCnt += 16;
    
  }

  LastWord:
  
  BriefRedrawScr();
  
}


void BriefRedrawScr()
{
    PutBitmap32(LeftSpace, UpSpace, (void*)((long)BigDrawBuffer+(LinePixels*Ofset)), LinePixels, MinNumOfYPixels);
    
}


void BriefScrollUp(int Num)
{
    int OldOfs = Ofset;
  if (Ofset-Num < 0) Ofset = 0;
  else Ofset -= Num;
    if (OldOfs != Ofset) {
    MouseHide();
    BriefRedrawScr();
    MouseShow();
  }
}
void BriefScrollDown(int Num)
{
    int OldOfs = Ofset;
  if (Ofset+Num > BigBufLines-MinNumOfYPixels) Ofset = BigBufLines-MinNumOfYPixels;
  else Ofset += Num;
    
    if (OldOfs != Ofset) {
    MouseHide();
    BriefRedrawScr();
    MouseShow();
  }
}






void BriefDone()
{
  memfree(BigDrawBuffer);
    for (int i = 0; i < WordCnt; i++) memfree(Words[i]);
}
