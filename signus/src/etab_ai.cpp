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

#include "etables.h"


// Postupove tabulky zkusenosti:

int TabUnitsExperience[][10] = {
/*Nejednotka  */ {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
/*PerseusAlfa */ {0,  25,  50,  75, 100, 125, 150, 175, 200, 225}, //OK
/*PerseusBeta */ {0,  30,  60,  90, 120, 150, 180, 210, 240, 270}, //OK
/*PerseusGama */ {0,  50, 100, 150, 200, 250, 300, 350, 400, 450}, //OK
/*Dirrac      */ {0,  50, 100, 150, 200, 250, 300, 350, 400, 450}, //OK
/*Agni        */ {0,  50, 100, 150, 200, 250, 300, 350, 400, 450}, //OK
/*Skorpion    */ {0,  60, 120, 180, 240, 300, 360, 420, 480, 560}, //OK
/*Herkules    */ {0, 150, 300, 450, 600, 750, 900,1050,1200,1350}, //OK
/*Falanga     */ {0,  25,  50,  75, 100, 125, 150, 175, 200, 225}, //OK
/*Golias      */ {0, 200, 400, 600, 800,1000,1200,1400,1600,1800}, //OK
/*Patriot     */ {0,  50, 100, 150, 200, 250, 300, 350, 400, 450}, //OK
/*Bumerang    */ {0,  10,  20,  30,  40,  50,  60,  70,  80,  90}, //OK
/*Skiatron    */ {0, 100, 200, 300, 400, 500, 600, 700, 800, 900}, //OK
/*Xenon       */ {0, 100, 200, 300, 400, 500, 600, 700, 800, 900}, //OK
/*Uran        */ {0, 100, 200, 300, 400, 500, 600, 700, 800, 900}, //OK
/*Gargantua   */ {0, 100, 200, 300, 400, 500, 600, 700, 800, 900}, //OK
/*Medea       */ {0, 100, 200, 300, 400, 500, 600, 700, 800, 900}, //OK
/*Ceres       */ {0, 100, 200, 300, 400, 500, 600, 700, 800, 900}, //OK
/*Gnom        */ {0, 100, 200, 300, 400, 500, 600, 700, 800, 900}, //OK
/*Iris        */ {0, 100, 200, 300, 400, 500, 600, 700, 800, 900}, //OK
/*Horizont    */ {0,  30,  60,  90, 120, 150, 180, 210, 240, 270}, //OK
/*Pagoda      */ {0,  25,  50,  75, 100, 125, 150, 175, 200, 225}, //OK
/*Minotaurus  */ {0,  60, 120, 180, 240, 300, 360, 420, 480, 560}, //OK
/*Thor        */ {0,  60, 120, 180, 240, 300, 360, 420, 480, 560}, //OK
/*Spektrum    */ {0,  30,  60,  90, 120, 150, 180, 210, 240, 270}, //OK
/*Neptun      */ {0, 100, 200, 300, 400, 500, 600, 700, 800, 900}, //OK
/*Helios      */ {0,  30,  60,  90, 120, 150, 180, 210, 240, 270}, //OK
/*Olymp       */ {0, 100, 200, 300, 400, 500, 600, 700, 800, 900}, //OK
/*Mohykan     */ {0,  30,  60,  90, 120, 150, 180, 210, 240, 270}, //OK
/*Mesias      */ {0, 150, 300, 450, 600, 750, 900,1050,1200,1350}, //OK
/*Ganymedes   */ {0, 100, 200, 300, 400, 500, 600, 700, 800, 900}, //OK
/*Flint       */ {0,  25,  50,  75, 100, 125, 150, 175, 200, 225}, //OK
/*Viking      */ {0,  30,  60,  90, 120, 150, 180, 210, 240, 270}, //OK
/*Poseidon    */ {0,  50, 100, 150, 200, 250, 300, 350, 400, 450}, //OK
/*Kraken      */ {0, 100, 200, 300, 400, 500, 600, 700, 800, 900}, //OK
/*Oasa        */ {0, 100, 200, 300, 400, 500, 600, 700, 800, 900}, //OK
/*Laguna      */ {0, 100, 200, 300, 400, 500, 600, 700, 800, 900}, //OK
/*Barracuda   */ {0,  50, 100, 150, 200, 250, 300, 350, 400, 450}, //OK
/*Ikaros      */ {0,  30,  60,  90, 120, 150, 180, 210, 240, 270}, //OK
/*Kobra       */ {0,  30,  60,  90, 120, 150, 180, 210, 240, 270}, //OK
/*Saturn      */ {0, 200, 400, 600, 800,1000,1200,1400,1600,1800}, //OK
/*Mystik      */ {0, 150, 300, 450, 600, 750, 900,1050,1200,1350}, //OK
/*Caesar      */ {0, 100, 200, 300, 400, 500, 600, 700, 800, 900}, //OK
/*Proton      */ {0, 100, 200, 300, 400, 500, 600, 700, 800, 900}, //OK
/*Rex         */ {0, 150, 300, 450, 600, 750, 900,1050,1200,1350}, //OK
/*Satan       */ {0, 100, 200, 300, 400, 500, 600, 700, 800, 900}  //OK
};



// ceny jednotek:
int TabUnitsCost[55] = {
/*Nejednotka  */       1,
/*PerseusAlfa */       100,
/*PerseusBeta */       200,
/*PerseusGama */       600,
/*Dirrac      */       550,
/*Agni        */       650,
/*Skorpion    */       350,
/*Herkules    */       600,
/*Falanga     */       200,
/*Golias      */       800,
/*Patriot     */       250,
/*Bumerang    */       50,
/*Skiatron    */       800,
/*Xenon       */       650,
/*Uran        */       300,
/*Gargantua   */       1000,
/*Medea       */       450,
/*Ceres       */       300,
/*Gnom        */       650,
/*Iris        */       950,
/*Horizont    */       1,
/*Pagoda      */       1,
/*Minotaurus  */       1,
/*Thor        */       1,
/*Spektrum    */       1,
/*Neptun      */       800,
/*Helios      */       900,
/*Olymp       */       300,
/*Mohykan     */       400,
/*Mesias      */       750,
/*Ganymedes   */       1000,
/*Flint       */       750,
/*Viking      */       1000,
/*Poseidon    */       1500,
/*Kraken      */       2000,
/*Oasa        */       1200,
/*Laguna      */       1750,
/*Barracuda   */       800,
/*Ikaros      */       350,
/*Kobra       */       500,
/*Saturn      */       650,
/*Mystik      */       1100,
/*Caesar      */       550,
/*Proton      */       450,
/*Rex         */       700,
/*Satan       */       400,
/*Nejednotka  */       1,
/*Nejednotka  */       1,
/*Nejednotka  */       1,
/*Nejednotka  */       1,
/*unRadar     */       1,
/*unBase      */       1,
/*unHeliport  */       1,
/*unAirport   */       1,
/*unWarehouse */       1
};

// vyrobni casy jednotek:
int TabUnitsProducTime[55] = {
/*Nejednotka  */       0,
/*PerseusAlfa */       1,
/*PerseusBeta */       1,
/*PerseusGama */       2,
/*Dirrac      */       2,
/*Agni        */       2,
/*Skorpion    */       2,
/*Herkules    */       3,
/*Falanga     */       1,
/*Golias      */       3,
/*Patriot     */       2,
/*Bumerang    */       1,
/*Skiatron    */       1,
/*Xenon       */       2,
/*Uran        */       2,
/*Gargantua   */       3,
/*Medea       */       2,
/*Ceres       */       1,
/*Gnom        */       2,
/*Iris        */       3,
/*Horizont    */       1,
/*Pagoda      */       1,
/*Minotaurus  */       1,
/*Thor        */       1,
/*Spektrum    */       1,
/*Neptun      */       2,
/*Helios      */       2,
/*Olymp       */       2,
/*Mohykan     */       2,
/*Mesias      */       3,
/*Ganymedes   */       3,
/*Flint       */       2,
/*Viking      */       3,
/*Poseidon    */       3,
/*Kraken      */       3,
/*Oasa        */       3,
/*Laguna      */       3,
/*Barracuda   */       2,
/*Ikaros      */       1,
/*Kobra       */       2,
/*Saturn      */       3,
/*Mystik      */       2,
/*Caesar      */       2,
/*Proton      */       2,
/*Rex         */       2,
/*Satan       */       2,
/*Nejednotka  */       1,
/*Nejednotka  */       1,
/*Nejednotka  */       1,
/*Nejednotka  */       1,
/*unRadar     */       1,
/*unBase      */       1,
/*unHeliport  */       1,
/*unAirport   */       1,
/*unWarehouse */       1
};






// Opatrnost jednotek
double TabCareful[55] = {
/*Nejednotka  */       0,
/*PerseusAlfa */       0.6,
/*PerseusBeta */       0.8,
/*PerseusGama */       0.8,
/*Dirrac      */       0.7,
/*Agni        */       0.7,
/*Skorpion    */       2,
/*Herkules    */       3,
/*Falanga     */       2,
/*Golias      */       3,
/*Patriot     */       2,
/*Bumerang    */       0.6,
/*Skiatron    */       3,
/*Xenon       */       2,
/*Uran        */       5,
/*Gargantua   */       5,
/*Medea       */       4,
/*Ceres       */       2,
/*Gnom        */       2,
/*Iris        */       1,
/*Horizont    */       1,
/*Pagoda      */       1,
/*Minotaurus  */       1,
/*Thor        */       1,
/*Spektrum    */       1,
/*Neptun      */       5,
/*Helios      */       1,
/*Olymp       */       5,
/*Mohykan     */       0.8,
/*Mesias      */       3,
/*Ganymedes   */       3,
/*Flint       */       0.8,
/*Viking      */       0.8,
/*Poseidon    */       1,
/*Kraken      */       1,
/*Oasa        */       5,
/*Laguna      */       5,
/*Barracuda   */       1,
/*Ikaros      */       0.6,
/*Kobra       */       1,
/*Saturn      */       1,
/*Mystik      */       0.8,
/*Caesar      */       2,
/*Proton      */       2,
/*Rex         */       0.8,
/*Satan       */       0,
/*Nejednotka  */       0,
/*Nejednotka  */       0,
/*Nejednotka  */       0,
/*Nejednotka  */       0,
/*unRadar     */       0,
/*unBase      */       0,
/*unHeliport  */       0,
/*unAirport   */       0,
/*unWarehouse */       0

};

// Nebezpecnost jednotek
double TabDanger[55] = {
/*Nejednotka  */       0,
/*PerseusAlfa */       0.8,
/*PerseusBeta */       1.3,
/*PerseusGama */       1.7,
/*Dirrac      */       1.6,
/*Agni        */       1.6,
/*Skorpion    */       1.4,
/*Herkules    */       2,
/*Falanga     */       1.7,
/*Golias      */       1.5,
/*Patriot     */       1.3,
/*Bumerang    */       1,
/*Skiatron    */       1.2,
/*Xenon       */       1.1,
/*Uran        */       2,
/*Gargantua   */       3,
/*Medea       */       1.3,
/*Ceres       */       1,
/*Gnom        */       1,
/*Iris        */       1.2,
/*Horizont    */       1.3,
/*Pagoda      */       1.3,
/*Minotaurus  */       1.5,
/*Thor        */       1.8,
/*Spektrum    */       1.7,
/*Neptun      */       2,
/*Helios      */       1.7,
/*Olymp       */       2,
/*Mohykan     */       1.3,
/*Mesias      */       1.8,
/*Ganymedes   */       1,
/*Flint       */       0.7,
/*Viking      */       1,
/*Poseidon    */       1.5,
/*Kraken      */       3,
/*Oasa        */       3,
/*Laguna      */       3,
/*Barracuda   */       2,
/*Ikaros      */       1.2,
/*Kobra       */       1.3,
/*Saturn      */       1.8,
/*Mystik      */       2.5,
/*Caesar      */       1,
/*Proton      */       1,
/*Rex         */       1.5,
/*Satan       */       5,
/*Nejednotka  */       0,
/*Nejednotka  */       0,
/*Nejednotka  */       0,
/*Nejednotka  */       0,
/*unRadar     */       0.5,
/*unBase      */       0.5,
/*unHeliport  */       0.5,
/*unAirport   */       0.5,
/*unWarehouse */       0.5

};


