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



// Obojzivelne jednotky:



#include "headers.h"
#include "uamphi.h"
#include "untables.h"

/////////////// THovercraft - obecny predek





////////////////////////////////// jednotky/ ////////////////////////////




// Helios:

void THelios::Setup()
{
	TToweredHovercraft::Setup();
	MaxFuel = utHL_FUEL;
	MaxHitPoints = utHL_HP;
	MaxTimeUnits = utHL_TU;
	Defense[0] = utHL_DN0;
	Defense[1] = utHL_DN1;
	Defense[2] = utHL_DN2;
	Visib = utHL_VIS;
	CurWpn = 0;
	WeaponsCnt = 2;
	Weapons[0] = new TCannon(WpnNames[WPNNM_CANNON], 
	                 utHL_AN, utHL_ANB, utHL_AT, utHL_MAXR, utHL_AMMO);
	Weapons[1] = new TCannonAntiAir(WpnNames[WPNNM_AIRCANNON], 
	                 utHL_2AN, utHL_2ANB, utHL_2AT, utHL_2MAXR, utHL_2AMMO);
	Velocity = utHL_VEL;
}



void THelios::IncLevel(int alevel)
{
	MaxHitPoints = utHL_HP + alevel * utHL_INC_HP;
	Defense[0] = utHL_DN0 + alevel * utHL_INC_DN;
	Defense[1] = utHL_DN1 + alevel * utHL_INC_DN;
	Defense[2] = utHL_DN2 + alevel * utHL_INC_DN;
	Weapons[0]->IncAN(utHL_AN + alevel * utHL_INC_AN - Weapons[0]->AttackNum);
	Weapons[1]->IncAN(utHL_2AN + alevel * utHL_INC_2AN - Weapons[1]->AttackNum);
}



unsigned THelios::GetSupportedActions()
{
	unsigned sa = uatMove | uatSelect | uatSelectAir | uatLoadIn;
	if (CurWpn == 0) sa |= uatAttack | uatAttackGood | uatAttackField;
	else if (CurWpn == 1) sa |= uatAttackAir | uatAttackAirG;
	return sa;
}
