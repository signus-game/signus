#include "engine.h"

static const int Res = 800;
static const int Mbd = 168;

inline void _movsb(uintptr_t& edi, uintptr_t& esi, int& ecx)
{
	__builtin_memcpy((void*)edi, (void*)esi, 1);
	edi += 1;
	esi += 1;
}

inline void _movsw(uintptr_t& edi, uintptr_t& esi, int& ecx)
{
	__builtin_memcpy((void*)edi, (void*)esi, 2);
	edi += 2;
	esi += 2;
}

inline void _movsd(uintptr_t& edi, uintptr_t& esi, int& ecx)
{
	__builtin_memcpy((void*)edi, (void*)esi, 4);
	edi += 4;
	esi += 4;
}

inline void _rep_movsd(uintptr_t& edi, uintptr_t& esi, int& ecx)
{
	__builtin_memcpy((void*)edi, (void*)esi, ecx * 4);
	edi += ecx * 4;
	esi += ecx * 4;
	ecx = 0;
}

inline void _stosb(uintptr_t& edi, uint32_t value)
{
	__builtin_memset((void*)edi, value & 0xff, 1);
}

inline void _stosw(uintptr_t& edi, uint32_t value)
{
	__builtin_memset((void*)edi, value & 0xff, 2);
}

inline void _stosd(uintptr_t& edi, uint32_t value)
{
	__builtin_memset((void*)edi, value & 0xff, 4);
}

inline void _rep_stosd(uintptr_t& edi, uint32_t value, int& ecx)
{
	__builtin_memset((void*)edi, value & 0xff, 4*ecx);
}

extern uint8_t DarkingTable[];

void DrawTerrA(int x, int y, void* bmp)
{
	uintptr_t edi = x;
	int eax = y;
	uintptr_t esi = (uintptr_t) bmp;
	int edx = 560+Mbd;
	eax *= edx;
	edi += eax;
	edi += (uintptr_t) MapBuf;
	int ecx;
	
			edi += 26;
			_movsd(edi, esi, ecx);
			edi += Mbd+554;
			ecx = 2;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+550;
			ecx = 3;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+546;
			ecx = 4;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+542;
			ecx = 5;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+538;
			ecx = 6;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+534;
			ecx = 7;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+530;
			ecx = 8;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+526;
			ecx = 9;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+522;
			ecx = 10;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+518;
			ecx = 11;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+514;
			ecx = 12;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+510;
			ecx = 13;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+506;
			ecx = 14;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+506;
			ecx = 13;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+510;
			ecx = 12;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+514;
			ecx = 11;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+518;
			ecx = 10;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+522;
			ecx = 9;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+526;
			ecx = 8;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+530;
			ecx = 7;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+534;
			ecx = 6;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+538;
			ecx = 5;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+542;
			ecx = 4;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+546;
			ecx = 3;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+550;
			ecx = 2;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+554;
			_movsd(edi, esi, ecx);
}



void DrawTerrB(int x, int y, void* bmp)
{
	uintptr_t edi = x;
	int eax = y;
	uintptr_t esi = (uintptr_t) bmp;
	int edx = 560+Mbd;
	eax *= edx;
	edi += eax;
	edi += (uintptr_t) MapBuf;
	int ecx;

			edi += 26;
			_movsd(edi, esi, ecx);
			edi += Mbd+555;
			_movsd(edi, esi, ecx);
			_movsw(edi, esi, ecx);
			_movsb(edi, esi, ecx);
			edi += Mbd+552;
			ecx = 2;
			_rep_movsd(edi, esi, ecx);
			_movsw(edi, esi, ecx);
			edi += Mbd+549;
			ecx = 3;
			_rep_movsd(edi, esi, ecx);
			_movsb(edi, esi, ecx);
			edi += Mbd+546;
			ecx = 4;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+543;
			ecx = 4;
			_rep_movsd(edi, esi, ecx);
			_movsw(edi, esi, ecx);
			_movsb(edi, esi, ecx);
			edi += Mbd+540;
			ecx = 5;
			_rep_movsd(edi, esi, ecx);
			_movsw(edi, esi, ecx);
			edi += Mbd+537;
			ecx = 6;
			_rep_movsd(edi, esi, ecx);
			_movsb(edi, esi, ecx);
			edi += Mbd+534;
			ecx = 7;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+531;
			ecx = 7;
			_rep_movsd(edi, esi, ecx);
			_movsw(edi, esi, ecx);
			_movsb(edi, esi, ecx);
			edi += Mbd+528;
			ecx = 8;
			_rep_movsd(edi, esi, ecx);
			_movsw(edi, esi, ecx);
			edi += Mbd+525;
			ecx = 9;
			_rep_movsd(edi, esi, ecx);
			_movsb(edi, esi, ecx);
			edi += Mbd+522;
			ecx = 10;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+519;
			ecx = 10;
			_rep_movsd(edi, esi, ecx);
			_movsw(edi, esi, ecx);
			_movsb(edi, esi, ecx);
			edi += Mbd+516;
			ecx = 10;
			_rep_movsd(edi, esi, ecx);
			_movsw(edi, esi, ecx);
			edi += Mbd+517;
			ecx = 10;
			_rep_movsd(edi, esi, ecx);
			_movsw(edi, esi, ecx);
			edi += Mbd+517;
			ecx = 10;
			_rep_movsd(edi, esi, ecx);
			_movsw(edi, esi, ecx);
			edi += Mbd+517;
			ecx = 10;
			_rep_movsd(edi, esi, ecx);
			_movsw(edi, esi, ecx);
			edi += Mbd+517;
			ecx = 10;
			_rep_movsd(edi, esi, ecx);
			_movsw(edi, esi, ecx);
			edi += Mbd+517;
			ecx = 10;
			_rep_movsd(edi, esi, ecx);
			_movsw(edi, esi, ecx);
			edi += Mbd+517;
			ecx = 10;
			_rep_movsd(edi, esi, ecx);
			_movsw(edi, esi, ecx);
			edi += Mbd+517;
			ecx = 10;
			_rep_movsd(edi, esi, ecx);
			_movsw(edi, esi, ecx);
			edi += Mbd+517;
			ecx = 10;
			_rep_movsd(edi, esi, ecx);
			_movsw(edi, esi, ecx);
			edi += Mbd+517;
			ecx = 10;
			_rep_movsd(edi, esi, ecx);
			_movsw(edi, esi, ecx);
			edi += Mbd+517;
			ecx = 10;
			_rep_movsd(edi, esi, ecx);
			_movsw(edi, esi, ecx);
			edi += Mbd+517;
			ecx = 10;
			_rep_movsd(edi, esi, ecx);
			_movsw(edi, esi, ecx);
			edi += Mbd+517;
			ecx = 10;
			_rep_movsd(edi, esi, ecx);
			_movsw(edi, esi, ecx);
			edi += Mbd+518;
			ecx = 10;
			_rep_movsd(edi, esi, ecx);
			_movsb(edi, esi, ecx);
			edi += Mbd+521;
			ecx = 9;
			_rep_movsd(edi, esi, ecx);
			_movsw(edi, esi, ecx);
			edi += Mbd+524;
			ecx = 8;
			_rep_movsd(edi, esi, ecx);
			_movsw(edi, esi, ecx);
			_movsb(edi, esi, ecx);
			edi += Mbd+527;
			ecx = 8;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+530;
			ecx = 7;
			_rep_movsd(edi, esi, ecx);
			_movsb(edi, esi, ecx);
			edi += Mbd+533;
			ecx = 6;
			_rep_movsd(edi, esi, ecx);
			_movsw(edi, esi, ecx);
			edi += Mbd+536;
			ecx = 5;
			_rep_movsd(edi, esi, ecx);
			_movsw(edi, esi, ecx);
			_movsb(edi, esi, ecx);
			edi += Mbd+539;
			ecx = 5;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+542;
			ecx = 4;
			_rep_movsd(edi, esi, ecx);
			_movsb(edi, esi, ecx);
			edi += Mbd+545;
			ecx = 3;
			_rep_movsd(edi, esi, ecx);
			_movsw(edi, esi, ecx);
			edi += Mbd+548;
			ecx = 2;
			_rep_movsd(edi, esi, ecx);
			_movsw(edi, esi, ecx);
			_movsb(edi, esi, ecx);
			edi += Mbd+551;
			ecx = 2;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+554;
			_movsd(edi, esi, ecx);
			_movsb(edi, esi, ecx);
			edi += Mbd+557;
			_movsw(edi, esi, ecx);
}






void DrawTerrC(int x, int y, void* bmp)
{

	uintptr_t edi = x;
	int eax = y;
	uintptr_t esi = (uintptr_t) bmp;
	int edx = 560+Mbd;
	eax *= edx;
	edi += eax;
	edi += (uintptr_t) MapBuf;
	int ecx;


			edi += 26;
			_movsd(edi, esi, ecx);
			edi += Mbd+554;
			_movsd(edi, esi, ecx);
			_movsw(edi, esi, ecx);
			_movsb(edi, esi, ecx);
			edi += Mbd+551;
			ecx = 2;
			_rep_movsd(edi, esi, ecx);
			_movsw(edi, esi, ecx);
			edi += Mbd+548;
			ecx = 3;
			_rep_movsd(edi, esi, ecx);
			_movsb(edi, esi, ecx);
			edi += Mbd+545;
			ecx = 4;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+542;
			ecx = 4;
			_rep_movsd(edi, esi, ecx);
			_movsw(edi, esi, ecx);
			_movsb(edi, esi, ecx);
			edi += Mbd+539;
			ecx = 5;
			_rep_movsd(edi, esi, ecx);
			_movsw(edi, esi, ecx);
			edi += Mbd+536;
			ecx = 6;
			_rep_movsd(edi, esi, ecx);
			_movsb(edi, esi, ecx);
			edi += Mbd+533;
			ecx = 7;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+530;
			ecx = 7;
			_rep_movsd(edi, esi, ecx);
			_movsw(edi, esi, ecx);
			_movsb(edi, esi, ecx);
			edi += Mbd+527;
			ecx = 8;
			_rep_movsd(edi, esi, ecx);
			_movsw(edi, esi, ecx);
			edi += Mbd+524;
			ecx = 9;
			_rep_movsd(edi, esi, ecx);
			_movsb(edi, esi, ecx);
			edi += Mbd+521;
			ecx = 10;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+518;
			ecx = 10;
			_rep_movsd(edi, esi, ecx);
			_movsw(edi, esi, ecx);
			_movsb(edi, esi, ecx);
			edi += Mbd+519;
			ecx = 10;
			_rep_movsd(edi, esi, ecx);
			_movsw(edi, esi, ecx);
			edi += Mbd+519;
			ecx = 10;
			_rep_movsd(edi, esi, ecx);
			_movsw(edi, esi, ecx);
			edi += Mbd+519;
			ecx = 10;
			_rep_movsd(edi, esi, ecx);
			_movsw(edi, esi, ecx);
			edi += Mbd+519;
			ecx = 10;
			_rep_movsd(edi, esi, ecx);
			_movsw(edi, esi, ecx);
			edi += Mbd+519;
			ecx = 10;
			_rep_movsd(edi, esi, ecx);
			_movsw(edi, esi, ecx);
			edi += Mbd+519;
			ecx = 10;
			_rep_movsd(edi, esi, ecx);
			_movsw(edi, esi, ecx);
			edi += Mbd+519;
			ecx = 10;
			_rep_movsd(edi, esi, ecx);
			_movsw(edi, esi, ecx);
			edi += Mbd+519;
			ecx = 10;
			_rep_movsd(edi, esi, ecx);
			_movsw(edi, esi, ecx);
			edi += Mbd+519;
			ecx = 10;
			_rep_movsd(edi, esi, ecx);
			_movsw(edi, esi, ecx);
			edi += Mbd+519;
			ecx = 10;
			_rep_movsd(edi, esi, ecx);
			_movsw(edi, esi, ecx);
			edi += Mbd+519;
			ecx = 10;
			_rep_movsd(edi, esi, ecx);
			_movsw(edi, esi, ecx);
			edi += Mbd+519;
			ecx = 10;
			_rep_movsd(edi, esi, ecx);
			_movsw(edi, esi, ecx);
			edi += Mbd+519;
			ecx = 10;
			_rep_movsd(edi, esi, ecx);
			_movsw(edi, esi, ecx);
			edi += Mbd+519;
			ecx = 10;
			_rep_movsd(edi, esi, ecx);
			_movsb(edi, esi, ecx);
			edi += Mbd+520;
			ecx = 9;
			_rep_movsd(edi, esi, ecx);
			_movsw(edi, esi, ecx);
			edi += Mbd+523;
			ecx = 8;
			_rep_movsd(edi, esi, ecx);
			_movsw(edi, esi, ecx);
			_movsb(edi, esi, ecx);
			edi += Mbd+526;
			ecx = 8;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+529;
			ecx = 7;
			_rep_movsd(edi, esi, ecx);
			_movsb(edi, esi, ecx);
			edi += Mbd+532;
			ecx = 6;
			_rep_movsd(edi, esi, ecx);
			_movsw(edi, esi, ecx);
			edi += Mbd+535;
			ecx = 5;
			_rep_movsd(edi, esi, ecx);
			_movsw(edi, esi, ecx);
			_movsb(edi, esi, ecx);
			edi += Mbd+538;
			ecx = 5;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+541;
			ecx = 4;
			_rep_movsd(edi, esi, ecx);
			_movsb(edi, esi, ecx);
			edi += Mbd+544;
			ecx = 3;
			_rep_movsd(edi, esi, ecx);
			_movsw(edi, esi, ecx);
			edi += Mbd+547;
			ecx = 2;
			_rep_movsd(edi, esi, ecx);
			_movsw(edi, esi, ecx);
			_movsb(edi, esi, ecx);
			edi += Mbd+550;
			ecx = 2;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+553;
			_movsd(edi, esi, ecx);
			_movsb(edi, esi, ecx);
			edi += Mbd+556;
			_movsw(edi, esi, ecx);
}






void DrawTerrD(int x, int y, void* bmp)
{

	uintptr_t edi = x;
	int eax = y;
	uintptr_t esi = (uintptr_t) bmp;
	int edx = 560+Mbd;
	eax *= edx;
	edi += eax;
	edi += (uintptr_t) MapBuf;
	int ecx;

			edi += 14*(560+Mbd)+2;
			ecx = 7;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+534;
			ecx = 7;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+534;
			ecx = 7;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+534;
			ecx = 7;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+534;
			ecx = 7;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+534;
			ecx = 7;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+534;
			ecx = 7;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+534;
			ecx = 7;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+534;
			ecx = 7;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+534;
			ecx = 7;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+534;
			ecx = 7;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+534;
			ecx = 7;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+534;
			ecx = 7;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+534;
			ecx = 7;
			_rep_movsd(edi, esi, ecx);
}






void DrawTerrE(int x, int y, void* bmp)
{

	uintptr_t edi = x;
	int eax = y;
	uintptr_t esi = (uintptr_t) bmp;
	int edx = 560+Mbd;
	eax *= edx;
	edi += eax;
	edi += (uintptr_t) MapBuf;
	int ecx;
	
			
			
			edi += 14*(560+Mbd)+26;
			ecx = 7;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+530;
			ecx = 7;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+530;
			ecx = 7;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+530;
			ecx = 7;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+530;
			ecx = 7;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+530;
			ecx = 7;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+530;
			ecx = 7;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+530;
			ecx = 7;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+530;
			ecx = 7;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+530;
			ecx = 7;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+530;
			ecx = 7;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+530;
			ecx = 7;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+530;
			ecx = 7;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+530;
			ecx = 7;
			_rep_movsd(edi, esi, ecx);
}






void DrawTerrF(int x, int y, void* bmp)
{

	uintptr_t edi = x;
	int eax = y;
	uintptr_t esi = (uintptr_t) bmp;
	int edx = 560+Mbd;
	eax *= edx;
	edi += eax;
	edi += (uintptr_t) MapBuf;
	int ecx;
	

			edi += 26;
			_movsd(edi, esi, ecx);
			edi += Mbd+555;
			_movsd(edi, esi, ecx);
			_movsw(edi, esi, ecx);
			edi += Mbd+553;
			ecx = 2;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+551;
			ecx = 2;
			_rep_movsd(edi, esi, ecx);
			_movsw(edi, esi, ecx);
			edi += Mbd+549;
			ecx = 3;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+547;
			ecx = 3;
			_rep_movsd(edi, esi, ecx);
			_movsw(edi, esi, ecx);
			edi += Mbd+545;
			ecx = 4;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+543;
			ecx = 4;
			_rep_movsd(edi, esi, ecx);
			_movsw(edi, esi, ecx);
			edi += Mbd+541;
			ecx = 5;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+539;
			ecx = 5;
			_rep_movsd(edi, esi, ecx);
			_movsw(edi, esi, ecx);
			edi += Mbd+537;
			ecx = 6;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+535;
			ecx = 6;
			_rep_movsd(edi, esi, ecx);
			_movsw(edi, esi, ecx);
			edi += Mbd+533;
			ecx = 7;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+531;
			ecx = 7;
			_rep_movsd(edi, esi, ecx);
			_movsw(edi, esi, ecx);
			edi += Mbd+529;
			ecx = 8;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+527;
			ecx = 8;
			_rep_movsd(edi, esi, ecx);
			_movsw(edi, esi, ecx);
			edi += Mbd+525;
			ecx = 9;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+523;
			ecx = 9;
			_rep_movsd(edi, esi, ecx);
			_movsw(edi, esi, ecx);
			edi += Mbd+521;
			ecx = 10;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+519;
			ecx = 10;
			_rep_movsd(edi, esi, ecx);
			_movsw(edi, esi, ecx);
			edi += Mbd+517;
			ecx = 11;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+515;
			ecx = 11;
			_rep_movsd(edi, esi, ecx);
			_movsw(edi, esi, ecx);
			edi += Mbd+513;
			ecx = 12;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+511;
			ecx = 12;
			_rep_movsd(edi, esi, ecx);
			_movsw(edi, esi, ecx);
			edi += Mbd+509;
			ecx = 13;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+507;
			ecx = 13;
			_rep_movsd(edi, esi, ecx);
			_movsw(edi, esi, ecx);
			edi += Mbd+505;
			ecx = 14;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+504;
			ecx = 14;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+506;
			ecx = 13;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+510;
			ecx = 12;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+514;
			ecx = 11;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+518;
			ecx = 10;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+522;
			ecx = 9;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+526;
			ecx = 8;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+530;
			ecx = 7;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+534;
			ecx = 6;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+538;
			ecx = 5;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+542;
			ecx = 4;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+546;
			ecx = 3;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+550;
			ecx = 2;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+554;
			_movsd(edi, esi, ecx);
}






void DrawTerrG(int x, int y, void* bmp)
{

	uintptr_t edi = x;
	int eax = y;
	uintptr_t esi = (uintptr_t) bmp;
	int edx = 560+Mbd;
	eax *= edx;
	edi += eax;
	edi += (uintptr_t) MapBuf;
	int ecx;

			edi +=  14*(560+Mbd)+2;
			ecx = 7;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+533;
			ecx = 7;
			_rep_movsd(edi, esi, ecx);
			_movsb(edi, esi, ecx);
			edi += Mbd+532;
			ecx = 7;
			_rep_movsd(edi, esi, ecx);
			_movsw(edi, esi, ecx);
			edi += Mbd+531;
			ecx = 7;
			_rep_movsd(edi, esi, ecx);
			_movsw(edi, esi, ecx);
			_movsb(edi, esi, ecx);
			edi += Mbd+530;
			ecx = 8;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+529;
			ecx = 8;
			_rep_movsd(edi, esi, ecx);
			_movsb(edi, esi, ecx);
			edi += Mbd+528;
			ecx = 8;
			_rep_movsd(edi, esi, ecx);
			_movsw(edi, esi, ecx);
			edi += Mbd+527;
			ecx = 8;
			_rep_movsd(edi, esi, ecx);
			_movsw(edi, esi, ecx);
			_movsb(edi, esi, ecx);
			edi += Mbd+526;
			ecx = 9;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+525;
			ecx = 9;
			_rep_movsd(edi, esi, ecx);
			_movsb(edi, esi, ecx);
			edi += Mbd+524;
			ecx = 9;
			_rep_movsd(edi, esi, ecx);
			_movsw(edi, esi, ecx);
			edi += Mbd+523;
			ecx = 9;
			_rep_movsd(edi, esi, ecx);
			_movsw(edi, esi, ecx);
			_movsb(edi, esi, ecx);
			edi += Mbd+522;
			ecx = 10;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+521;
			ecx = 10;
			_rep_movsd(edi, esi, ecx);
			_movsb(edi, esi, ecx);
			edi += Mbd+520;
			ecx = 9;
			_rep_movsd(edi, esi, ecx);
			_movsw(edi, esi, ecx);
			edi += Mbd+523;
			ecx = 8;
			_rep_movsd(edi, esi, ecx);
			_movsw(edi, esi, ecx);
			_movsb(edi, esi, ecx);
			edi += Mbd+526;
			ecx = 8;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+529;
			ecx = 7;
			_rep_movsd(edi, esi, ecx);
			_movsb(edi, esi, ecx);
			edi += Mbd+532;
			ecx = 6;
			_rep_movsd(edi, esi, ecx);
			_movsw(edi, esi, ecx);
			edi += Mbd+535;
			ecx = 5;
			_rep_movsd(edi, esi, ecx);
			_movsw(edi, esi, ecx);
			_movsb(edi, esi, ecx);
			edi += Mbd+538;
			ecx = 5;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+541;
			ecx = 4;
			_rep_movsd(edi, esi, ecx);
			_movsb(edi, esi, ecx);
			edi += Mbd+544;
			ecx = 3;
			_rep_movsd(edi, esi, ecx);
			_movsw(edi, esi, ecx);
			edi += Mbd+547;
			ecx = 2;
			_rep_movsd(edi, esi, ecx);
			_movsw(edi, esi, ecx);
			_movsb(edi, esi, ecx);
			edi += Mbd+550;
			ecx = 2;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+553;
			_movsd(edi, esi, ecx);
			_movsb(edi, esi, ecx);
			edi += Mbd+556;
			_movsw(edi, esi, ecx);

}





void DrawTerrH(int x, int y, void* bmp)
{

	uintptr_t edi = x;
	int eax = y;
	uintptr_t esi = (uintptr_t) bmp;
	int edx = 560+Mbd;
	eax *= edx;
	edi += eax;
	edi += (uintptr_t) MapBuf;
	int ecx;

			edi += 14*(560+Mbd)+26;
			_movsd(edi, esi, ecx);
			edi += Mbd+554;
			ecx = 2;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+550;
			ecx = 3;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+546;
			ecx = 4;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+542;
			ecx = 5;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+538;
			ecx = 6;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+534;
			ecx = 7;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+530;
			ecx = 8;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+526;
			ecx = 9;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+522;
			ecx = 10;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+518;
			ecx = 11;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+514;
			ecx = 12;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+510;
			ecx = 13;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+506;
			ecx = 14;
			_rep_movsd(edi, esi, ecx);
}





void DrawTerrI(int x, int y, void* bmp)
{

	uintptr_t edi = x;
	int eax = y;
	uintptr_t esi = (uintptr_t) bmp;
	int edx = 560+Mbd;
	eax *= edx;
	edi += eax;
	edi += (uintptr_t) MapBuf;
	int ecx;

			edi += 14*(560+Mbd)+26;
			ecx = 7;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+530;
			ecx = 7;
			_rep_movsd(edi, esi, ecx);
			_movsb(edi, esi, ecx);
			edi += Mbd+529;
			ecx = 7;
			_rep_movsd(edi, esi, ecx);
			_movsw(edi, esi, ecx);
			edi += Mbd+528;
			ecx = 7;
			_rep_movsd(edi, esi, ecx);
			_movsw(edi, esi, ecx);
			_movsb(edi, esi, ecx);
			edi += Mbd+527;
			ecx = 8;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+526;
			ecx = 8;
			_rep_movsd(edi, esi, ecx);
			_movsb(edi, esi, ecx);
			edi += Mbd+525;
			ecx = 8;
			_rep_movsd(edi, esi, ecx);
			_movsw(edi, esi, ecx);
			edi += Mbd+524;
			ecx = 8;
			_rep_movsd(edi, esi, ecx);
			_movsw(edi, esi, ecx);
			_movsb(edi, esi, ecx);
			edi += Mbd+523;
			ecx = 9;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+522;
			ecx = 9;
			_rep_movsd(edi, esi, ecx);
			_movsb(edi, esi, ecx);
			edi += Mbd+521;
			ecx = 9;
			_rep_movsd(edi, esi, ecx);
			_movsw(edi, esi, ecx);
			edi += Mbd+520;
			ecx = 9;
			_rep_movsd(edi, esi, ecx);
			_movsw(edi, esi, ecx);
			_movsb(edi, esi, ecx);
			edi += Mbd+519;
			ecx = 10;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+518;
			ecx = 10;
			_rep_movsd(edi, esi, ecx);
			_movsb(edi, esi, ecx);
			edi += Mbd+521;
			ecx = 9;
			_rep_movsd(edi, esi, ecx);
			_movsw(edi, esi, ecx);
			edi += Mbd+524;
			ecx = 8;
			_rep_movsd(edi, esi, ecx);
			_movsw(edi, esi, ecx);
			_movsb(edi, esi, ecx);
			edi += Mbd+527;
			ecx = 8;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+530;
			ecx = 7;
			_rep_movsd(edi, esi, ecx);
			_movsb(edi, esi, ecx);
			edi += Mbd+533;
			ecx = 6;
			_rep_movsd(edi, esi, ecx);
			_movsw(edi, esi, ecx);
			edi += Mbd+536;
			ecx = 5;
			_rep_movsd(edi, esi, ecx);
			_movsw(edi, esi, ecx);
			_movsb(edi, esi, ecx);
			edi += Mbd+539;
			ecx = 5;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+542;
			ecx = 4;
			_rep_movsd(edi, esi, ecx);
			_movsb(edi, esi, ecx);
			edi += Mbd+545;
			ecx = 3;
			_rep_movsd(edi, esi, ecx);
			_movsw(edi, esi, ecx);
			edi += Mbd+548;
			ecx = 2;
			_rep_movsd(edi, esi, ecx);
			_movsw(edi, esi, ecx);
			_movsb(edi, esi, ecx);
			edi += Mbd+551;
			ecx = 2;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+554;
			_movsd(edi, esi, ecx);
			_movsb(edi, esi, ecx);
			edi += Mbd+557;
			_movsw(edi, esi, ecx);

}






void DrawTerrJ(int x, int y, void* bmp)
{

	uintptr_t edi = x;
	int eax = y;
	uintptr_t esi = (uintptr_t) bmp;
	int edx = 560+Mbd;
	eax *= edx;
	edi += eax;
	edi += (uintptr_t) MapBuf;
	int ecx;

			edi += 26;
			_movsd(edi, esi, ecx);
			edi += Mbd+554;
			ecx = 2;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+550;
			ecx = 3;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+546;
			ecx = 4;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+542;
			ecx = 5;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+538;
			ecx = 6;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+534;
			ecx = 7;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+530;
			ecx = 8;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+526;
			ecx = 9;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+522;
			ecx = 10;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+518;
			ecx = 11;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+514;
			ecx = 12;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+510;
			ecx = 13;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+506;
			ecx = 14;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+506;
			ecx = 13;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+509;
			ecx = 12;
			_rep_movsd(edi, esi, ecx);
			_movsw(edi, esi, ecx);
			edi += Mbd+511;
			ecx = 12;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+513;
			ecx = 11;
			_rep_movsd(edi, esi, ecx);
			_movsw(edi, esi, ecx);
			edi += Mbd+515;
			ecx = 11;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+517;
			ecx = 10;
			_rep_movsd(edi, esi, ecx);
			_movsw(edi, esi, ecx);
			edi += Mbd+519;
			ecx = 10;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+521;
			ecx = 9;
			_rep_movsd(edi, esi, ecx);
			_movsw(edi, esi, ecx);
			edi += Mbd+523;
			ecx = 9;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+525;
			ecx = 8;
			_rep_movsd(edi, esi, ecx);
			_movsw(edi, esi, ecx);
			edi += Mbd+527;
			ecx = 8;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+529;
			ecx = 7;
			_rep_movsd(edi, esi, ecx);
			_movsw(edi, esi, ecx);
			edi += Mbd+531;
			ecx = 7;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+533;
			ecx = 6;
			_rep_movsd(edi, esi, ecx);
			_movsw(edi, esi, ecx);
			edi += Mbd+535;
			ecx = 6;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+537;
			ecx = 5;
			_rep_movsd(edi, esi, ecx);
			_movsw(edi, esi, ecx);
			edi += Mbd+539;
			ecx = 5;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+541;
			ecx = 4;
			_rep_movsd(edi, esi, ecx);
			_movsw(edi, esi, ecx);
			edi += Mbd+543;
			ecx = 4;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+545;
			ecx = 3;
			_rep_movsd(edi, esi, ecx);
			_movsw(edi, esi, ecx);
			edi += Mbd+547;
			ecx = 3;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+549;
			ecx = 2;
			_rep_movsd(edi, esi, ecx);
			_movsw(edi, esi, ecx);
			edi += Mbd+551;
			ecx = 2;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+553;
			_movsd(edi, esi, ecx);
			_movsw(edi, esi, ecx);
			edi += Mbd+555;
			_movsd(edi, esi, ecx);
			edi += Mbd+557;
			_movsw(edi, esi, ecx);
}






void DrawTerrK(int x, int y, void* bmp)
{

	uintptr_t edi = x;
	int eax = y;
	uintptr_t esi = (uintptr_t) bmp;
	int edx = 560+Mbd;
	eax *= edx;
	edi += eax;
	edi += (uintptr_t) MapBuf;
	int ecx;

			edi += 26;
			_movsd(edi, esi, ecx);
			edi += Mbd+554;
			_movsd(edi, esi, ecx);
			_movsw(edi, esi, ecx);
			_movsb(edi, esi, ecx);
			edi += Mbd+551;
			ecx = 2;
			_rep_movsd(edi, esi, ecx);
			_movsw(edi, esi, ecx);
			edi += Mbd+548;
			ecx = 3;
			_rep_movsd(edi, esi, ecx);
			_movsb(edi, esi, ecx);
			edi += Mbd+545;
			ecx = 4;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+542;
			ecx = 4;
			_rep_movsd(edi, esi, ecx);
			_movsw(edi, esi, ecx);
			_movsb(edi, esi, ecx);
			edi += Mbd+539;
			ecx = 5;
			_rep_movsd(edi, esi, ecx);
			_movsw(edi, esi, ecx);
			edi += Mbd+536;
			ecx = 6;
			_rep_movsd(edi, esi, ecx);
			_movsb(edi, esi, ecx);
			edi += Mbd+533;
			ecx = 7;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+530;
			ecx = 7;
			_rep_movsd(edi, esi, ecx);
			_movsw(edi, esi, ecx);
			_movsb(edi, esi, ecx);
			edi += Mbd+527;
			ecx = 8;
			_rep_movsd(edi, esi, ecx);
			_movsw(edi, esi, ecx);
			edi += Mbd+524;
			ecx = 9;
			_rep_movsd(edi, esi, ecx);
			_movsb(edi, esi, ecx);
			edi += Mbd+521;
			ecx = 10;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+518;
			ecx = 10;
			_rep_movsd(edi, esi, ecx);
			_movsw(edi, esi, ecx);
			_movsb(edi, esi, ecx);
			edi += Mbd+519;
			ecx = 10;
			_rep_movsd(edi, esi, ecx);
			_movsw(edi, esi, ecx);
			edi += Mbd+520;
			ecx = 10;
			_rep_movsd(edi, esi, ecx);
			_movsb(edi, esi, ecx);
			edi += Mbd+521;
			ecx = 10;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+522;
			ecx = 9;
			_rep_movsd(edi, esi, ecx);
			_movsw(edi, esi, ecx);
			_movsb(edi, esi, ecx);
			edi += Mbd+523;
			ecx = 9;
			_rep_movsd(edi, esi, ecx);
			_movsw(edi, esi, ecx);
			edi += Mbd+524;
			ecx = 9;
			_rep_movsd(edi, esi, ecx);
			_movsb(edi, esi, ecx);
			edi += Mbd+525;
			ecx = 9;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+526;
			ecx = 8;
			_rep_movsd(edi, esi, ecx);
			_movsw(edi, esi, ecx);
			_movsb(edi, esi, ecx);
			edi += Mbd+527;
			ecx = 8;
			_rep_movsd(edi, esi, ecx);
			_movsw(edi, esi, ecx);
			edi += Mbd+528;
			ecx = 8;
			_rep_movsd(edi, esi, ecx);
			_movsb(edi, esi, ecx);
			edi += Mbd+529;
			ecx = 8;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+530;
			ecx = 7;
			_rep_movsd(edi, esi, ecx);
			_movsw(edi, esi, ecx);
			_movsb(edi, esi, ecx);
			edi += Mbd+531;
			ecx = 7;
			_rep_movsd(edi, esi, ecx);
			_movsw(edi, esi, ecx);
			edi += Mbd+532;
			ecx = 7;
			_rep_movsd(edi, esi, ecx);
}






void DrawTerrL(int x, int y, void* bmp)
{

	uintptr_t edi = x;
	int eax = y;
	uintptr_t esi = (uintptr_t) bmp;
	int edx = 560+Mbd;
	eax *= edx;
	edi += eax;
	edi += (uintptr_t) MapBuf;
	int ecx;

			edi += 14*(560+Mbd)+2;
			ecx = 13;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+510;
			ecx = 12;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+514;
			ecx = 11;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+518;
			ecx = 10;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+522;
			ecx = 9;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+526;
			ecx = 8;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+530;
			ecx = 7;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+534;
			ecx = 6;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+538;
			ecx = 5;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+542;
			ecx = 4;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+546;
			ecx = 3;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+550;
			ecx = 2;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+554;
			_movsd(edi, esi, ecx);
}






void DrawTerrM(int x, int y, void* bmp)
{

	uintptr_t edi = x;
	int eax = y;
	uintptr_t esi = (uintptr_t) bmp;
	int edx = 560+Mbd;
	eax *= edx;
	edi += eax;
	edi += (uintptr_t) MapBuf;
	int ecx;

			edi += 26;
			_movsd(edi, esi, ecx);
			edi += Mbd+555;
			_movsd(edi, esi, ecx);
			_movsw(edi, esi, ecx);
			_movsb(edi, esi, ecx);
			edi += Mbd+552;
			ecx = 2;
			_rep_movsd(edi, esi, ecx);
			_movsw(edi, esi, ecx);
			edi += Mbd+549;
			ecx = 3;
			_rep_movsd(edi, esi, ecx);
			_movsb(edi, esi, ecx);
			edi += Mbd+546;
			ecx = 4;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+543;
			ecx = 4;
			_rep_movsd(edi, esi, ecx);
			_movsw(edi, esi, ecx);
			_movsb(edi, esi, ecx);
			edi += Mbd+540;
			ecx = 5;
			_rep_movsd(edi, esi, ecx);
			_movsw(edi, esi, ecx);
			edi += Mbd+537;
			ecx = 6;
			_rep_movsd(edi, esi, ecx);
			_movsb(edi, esi, ecx);
			edi += Mbd+534;
			ecx = 7;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+531;
			ecx = 7;
			_rep_movsd(edi, esi, ecx);
			_movsw(edi, esi, ecx);
			_movsb(edi, esi, ecx);
			edi += Mbd+528;
			ecx = 8;
			_rep_movsd(edi, esi, ecx);
			_movsw(edi, esi, ecx);
			edi += Mbd+525;
			ecx = 9;
			_rep_movsd(edi, esi, ecx);
			_movsb(edi, esi, ecx);
			edi += Mbd+522;
			ecx = 10;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+519;
			ecx = 10;
			_rep_movsd(edi, esi, ecx);
			_movsw(edi, esi, ecx);
			_movsb(edi, esi, ecx);
			edi += Mbd+516;
			ecx = 10;
			_rep_movsd(edi, esi, ecx);
			_movsw(edi, esi, ecx);
			edi += Mbd+517;
			ecx = 10;
			_rep_movsd(edi, esi, ecx);
			_movsb(edi, esi, ecx);
			edi += Mbd+518;
			ecx = 10;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+519;
			ecx = 9;
			_rep_movsd(edi, esi, ecx);
			_movsw(edi, esi, ecx);
			_movsb(edi, esi, ecx);
			edi += Mbd+520;
			ecx = 9;
			_rep_movsd(edi, esi, ecx);
			_movsw(edi, esi, ecx);
			edi += Mbd+521;
			ecx = 9;
			_rep_movsd(edi, esi, ecx);
			_movsb(edi, esi, ecx);
			edi += Mbd+522;
			ecx = 9;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+523;
			ecx = 8;
			_rep_movsd(edi, esi, ecx);
			_movsw(edi, esi, ecx);
			_movsb(edi, esi, ecx);
			edi += Mbd+524;
			ecx = 8;
			_rep_movsd(edi, esi, ecx);
			_movsw(edi, esi, ecx);
			edi += Mbd+525;
			ecx = 8;
			_rep_movsd(edi, esi, ecx);
			_movsb(edi, esi, ecx);
			edi += Mbd+526;
			ecx = 8;
			_rep_movsd(edi, esi, ecx);
			edi += Mbd+527;
			ecx = 7;
			_rep_movsd(edi, esi, ecx);
			_movsw(edi, esi, ecx);
			_movsb(edi, esi, ecx);
			edi += Mbd+528;
			ecx = 7;
			_rep_movsd(edi, esi, ecx);
			_movsw(edi, esi, ecx);
			edi += Mbd+530;
			ecx = 7;
			_rep_movsd(edi, esi, ecx);
}


void DrawLocalA(void *LocBuf, int x, int y, int color)
{

	uintptr_t esi = (uintptr_t) LocBuf;
	uintptr_t edi = x;
	int eax = y;

	int edx = 280 + (Mbd/2);
	eax *= edx;
	edi += eax;
	
	eax = color;
	int ecx;

			edi += esi;

			edi += 13;
			_stosw(edi, eax);
			edi += (Mbd/2)+276;
			_stosd(edi, eax);
			_stosw(edi, eax);
			edi += (Mbd/2)+272;
			ecx = 2;
			_rep_stosd(edi, eax, ecx);
			_stosw(edi, eax);
			edi += (Mbd/2)+268;
			ecx = 3;
			_rep_stosd(edi, eax, ecx);
			_stosw(edi, eax);
			edi += (Mbd/2)+264;
			ecx = 4;
			_rep_stosd(edi, eax, ecx);
			_stosw(edi, eax);
			edi += (Mbd/2)+260;
			ecx = 5;
			_rep_stosd(edi, eax, ecx);
			_stosw(edi, eax);
			edi += (Mbd/2)+256;
			ecx = 6;
			_rep_stosd(edi, eax, ecx);
			_stosw(edi, eax);
			edi += (Mbd/2)+254;
			ecx = 6;
			_rep_stosd(edi, eax, ecx);
			_stosw(edi, eax);
			edi += (Mbd/2)+256;
			ecx = 5;
			_rep_stosd(edi, eax, ecx);
			_stosw(edi, eax);
			edi += (Mbd/2)+260;
			ecx = 4;
			_rep_stosd(edi, eax, ecx);
			_stosw(edi, eax);
			edi += (Mbd/2)+264;
			ecx = 3;
			_rep_stosd(edi, eax, ecx);
			_stosw(edi, eax);
			edi += (Mbd/2)+268;
			ecx = 2;
			_rep_stosd(edi, eax, ecx);
			_stosw(edi, eax);
			edi += (Mbd/2)+272;
			_stosd(edi, eax);
			_stosw(edi, eax);
			edi += (Mbd/2)+276;
			_stosw(edi, eax);
}




void DrawLocalB(void *LocBuf, int x, int y, int color)
{
	uintptr_t esi = (uintptr_t) LocBuf;
	uintptr_t edi = x;
	int eax = y;

	int edx = 280 + (Mbd/2);
	eax *= edx;
	edi += eax;
	
	int ecx;
	eax = color;

	
			edi += esi;

			edi += 13;
			_stosw(edi, eax);
			edi += (Mbd/2)+277;
			_stosd(edi, eax);
			_stosb(edi, eax);
			edi += (Mbd/2)+274;
			ecx = 2;
			_rep_stosd(edi, eax, ecx);
			edi += (Mbd/2)+271;
			ecx = 2;
			_rep_stosd(edi, eax, ecx);
			_stosw(edi, eax);
			_stosb(edi, eax);
			edi += (Mbd/2)+268;
			ecx = 3;
			_rep_stosd(edi, eax, ecx);
			_stosw(edi, eax);
			edi += (Mbd/2)+265;
			ecx = 4;
			_rep_stosd(edi, eax, ecx);
			_stosb(edi, eax);
			edi += (Mbd/2)+262;
			ecx = 5;
			_rep_stosd(edi, eax, ecx);
			edi += (Mbd/2)+259;
			ecx = 5;
			_rep_stosd(edi, eax, ecx);
			_stosb(edi, eax);
			edi += (Mbd/2)+258;
			ecx = 5;
			_rep_stosd(edi, eax, ecx);
			_stosb(edi, eax);
			edi += (Mbd/2)+258;
			ecx = 5;
			_rep_stosd(edi, eax, ecx);
			_stosb(edi, eax);
			edi += (Mbd/2)+258;
			ecx = 5;
			_rep_stosd(edi, eax, ecx);
			_stosb(edi, eax);
			edi += (Mbd/2)+258;
			ecx = 5;
			_rep_stosd(edi, eax, ecx);
			_stosb(edi, eax);
			edi += (Mbd/2)+258;
			ecx = 5;
			_rep_stosd(edi, eax, ecx);
			_stosb(edi, eax);
			edi += (Mbd/2)+258;
			ecx = 5;
			_rep_stosd(edi, eax, ecx);
			_stosb(edi, eax);
			edi += (Mbd/2)+260;
			ecx = 4;
			_rep_stosd(edi, eax, ecx);
			_stosw(edi, eax);
			_stosb(edi, eax);
			edi += (Mbd/2)+263;
			ecx = 4;
			_rep_stosd(edi, eax, ecx);
			edi += (Mbd/2)+266;
			ecx = 3;
			_rep_stosd(edi, eax, ecx);
			_stosb(edi, eax);
			edi += (Mbd/2)+269;
			ecx = 2;
			_rep_stosd(edi, eax, ecx);
			_stosw(edi, eax);
			edi += (Mbd/2)+272;
			_stosd(edi, eax);
			_stosw(edi, eax);
			_stosb(edi, eax);
			edi += (Mbd/2)+275;
			_stosd(edi, eax);
			edi += (Mbd/2)+278;
			_stosb(edi, eax);
}



void DrawLocalC(void *LocBuf, int x, int y, int color)
{
	
	uintptr_t esi = (uintptr_t) LocBuf;
	uintptr_t edi = x;
	int eax = y;

	int edx = 280 + (Mbd/2);
	eax *= edx;
	edi += eax;
	
	int ecx;
	eax = color;

			edi += esi;

			edi += 13;
			_stosw(edi, eax);
			edi += (Mbd/2)+276;
			_stosd(edi, eax);
			_stosb(edi, eax);
			edi += (Mbd/2)+273;
			ecx = 2;
			_rep_stosd(edi, eax, ecx);
			edi += (Mbd/2)+270;
			ecx = 2;
			_rep_stosd(edi, eax, ecx);
			_stosw(edi, eax);
			_stosb(edi, eax);
			edi += (Mbd/2)+267;
			ecx = 3;
			_rep_stosd(edi, eax, ecx);
			_stosw(edi, eax);
			edi += (Mbd/2)+264;
			ecx = 4;
			_rep_stosd(edi, eax, ecx);
			_stosb(edi, eax);
			edi += (Mbd/2)+261;
			ecx = 5;
			_rep_stosd(edi, eax, ecx);
			edi += (Mbd/2)+260;
			ecx = 5;
			_rep_stosd(edi, eax, ecx);
			_stosb(edi, eax);
			edi += (Mbd/2)+260;
			ecx = 5;
			_rep_stosd(edi, eax, ecx);
			_stosb(edi, eax);
			edi += (Mbd/2)+260;
			ecx = 5;
			_rep_stosd(edi, eax, ecx);
			_stosb(edi, eax);
			edi += (Mbd/2)+260;
			ecx = 5;
			_rep_stosd(edi, eax, ecx);
			_stosb(edi, eax);
			edi += (Mbd/2)+260;
			ecx = 5;
			_rep_stosd(edi, eax, ecx);
			_stosb(edi, eax);
			edi += (Mbd/2)+260;
			ecx = 5;
			_rep_stosd(edi, eax, ecx);
			_stosb(edi, eax);
			edi += (Mbd/2)+260;
			ecx = 5;
			_rep_stosd(edi, eax, ecx);
			_stosb(edi, eax);
			edi += (Mbd/2)+260;
			ecx = 4;
			_rep_stosd(edi, eax, ecx);
			_stosw(edi, eax);
			_stosb(edi, eax);
			edi += (Mbd/2)+262;
			ecx = 4;
			_rep_stosd(edi, eax, ecx);
			edi += (Mbd/2)+265;
			ecx = 3;
			_rep_stosd(edi, eax, ecx);
			_stosb(edi, eax);
			edi += (Mbd/2)+268;
			ecx = 2;
			_rep_stosd(edi, eax, ecx);
			_stosw(edi, eax);
			edi += (Mbd/2)+271;
			_stosd(edi, eax);
			_stosw(edi, eax);
			_stosb(edi, eax);
			edi += (Mbd/2)+274;
			_stosd(edi, eax);
			edi += (Mbd/2)+277;
			_stosb(edi, eax);
}



void DrawLocalD(void *LocBuf, int x, int y, int color)
{
	uintptr_t esi = (uintptr_t) LocBuf;
	uintptr_t edi = x;
	int eax = y;

	int edx = 280 + (Mbd/2);
	eax *= edx;
	edi += eax;
	
	int ecx;
	eax = color;

			edi += esi;

			edi += 7*(280+(Mbd/2))+1;
			ecx = 3;
			_rep_stosd(edi, eax, ecx);
			_stosw(edi, eax);
			edi += (Mbd/2)+268;
			ecx = 3;
			_rep_stosd(edi, eax, ecx);
			_stosw(edi, eax);
			edi += (Mbd/2)+268;
			ecx = 3;
			_rep_stosd(edi, eax, ecx);
			_stosw(edi, eax);
			edi += (Mbd/2)+268;
			ecx = 3;
			_rep_stosd(edi, eax, ecx);
			_stosw(edi, eax);
			edi += (Mbd/2)+268;
			ecx = 3;
			_rep_stosd(edi, eax, ecx);
			_stosw(edi, eax);
			edi += (Mbd/2)+268;
			ecx = 3;
			_rep_stosd(edi, eax, ecx);
			_stosw(edi, eax);
			edi += (Mbd/2)+268;
			ecx = 3;
			_rep_stosd(edi, eax, ecx);
			_stosw(edi, eax);
}



void DrawLocalE(void *LocBuf, int x, int y, int color)
{
	uintptr_t esi = (uintptr_t) LocBuf;
	uintptr_t edi = x;
	int eax = y;

	int edx = 280 + (Mbd/2);
	eax *= edx;
	edi += eax;
	
	int ecx;
	eax = color;

			edi += esi;

			edi += 14+7*(280+(Mbd/2))-1;
			ecx = 3;
			_rep_stosd(edi, eax, ecx);
			_stosw(edi, eax);
			edi += (Mbd/2)+264;
			ecx = 3;
			_rep_stosd(edi, eax, ecx);
			_stosw(edi, eax);
			edi += (Mbd/2)+264;
			ecx = 3;
			_rep_stosd(edi, eax, ecx);
			_stosw(edi, eax);
			edi += (Mbd/2)+264;
			ecx = 3;
			_rep_stosd(edi, eax, ecx);
			_stosw(edi, eax);
			edi += (Mbd/2)+264;
			ecx = 3;
			_rep_stosd(edi, eax, ecx);
			_stosw(edi, eax);
			edi += (Mbd/2)+264;
			ecx = 3;
			_rep_stosd(edi, eax, ecx);
			_stosw(edi, eax);
			edi += (Mbd/2)+264;
			ecx = 3;
			_rep_stosd(edi, eax, ecx);
			_stosw(edi, eax);
}



void DrawLocalF(void *LocBuf, int x, int y, int color)
{
	uintptr_t esi = (uintptr_t) LocBuf;
	uintptr_t edi = x;
	int eax = y;

	int edx = 280 + (Mbd/2);
	eax *= edx;
	edi += eax;
	
	int ecx;
	eax = color;

			edi += esi;

			edi += 13;
			_stosw(edi, eax);
			edi += (Mbd/2)+277;
			_stosd(edi, eax);
			edi += (Mbd/2)+275;
			_stosd(edi, eax);
			_stosw(edi, eax);
			edi += (Mbd/2)+273;
			ecx = 2;
			_rep_stosd(edi, eax, ecx);
			edi += (Mbd/2)+271;
			ecx = 2;
			_rep_stosd(edi, eax, ecx);
			_stosw(edi, eax);
			edi += (Mbd/2)+269;
			ecx = 3;
			_rep_stosd(edi, eax, ecx);
			edi += (Mbd/2)+267;
			ecx = 3;
			_rep_stosd(edi, eax, ecx);
			_stosw(edi, eax);
			edi += (Mbd/2)+265;
			ecx = 4;
			_rep_stosd(edi, eax, ecx);
			edi += (Mbd/2)+263;
			ecx = 4;
			_rep_stosd(edi, eax, ecx);
			_stosw(edi, eax);
			edi += (Mbd/2)+261;
			ecx = 5;
			_rep_stosd(edi, eax, ecx);
			edi += (Mbd/2)+259;
			ecx = 5;
			_rep_stosd(edi, eax, ecx);
			_stosw(edi, eax);
			edi += (Mbd/2)+257;
			ecx = 6;
			_rep_stosd(edi, eax, ecx);
			edi += (Mbd/2)+255;
			ecx = 6;
			_rep_stosd(edi, eax, ecx);
			_stosw(edi, eax);
			edi += (Mbd/2)+253;
			ecx = 7;
			_rep_stosd(edi, eax, ecx);
			edi += (Mbd/2)+253;
			ecx = 6;
			_rep_stosd(edi, eax, ecx);
			_stosw(edi, eax);
			edi += (Mbd/2)+256;
			ecx = 5;
			_rep_stosd(edi, eax, ecx);
			_stosw(edi, eax);
			edi += (Mbd/2)+260;
			ecx = 4;
			_rep_stosd(edi, eax, ecx);
			_stosw(edi, eax);
			edi += (Mbd/2)+264;
			ecx = 3;
			_rep_stosd(edi, eax, ecx);
			_stosw(edi, eax);
			edi += (Mbd/2)+268;
			ecx = 2;
			_rep_stosd(edi, eax, ecx);
			_stosw(edi, eax);
			edi += (Mbd/2)+272;
			_stosd(edi, eax);
			_stosw(edi, eax);
			edi += (Mbd/2)+276;
			_stosw(edi, eax);
}



void DrawLocalG(void *LocBuf, int x, int y, int color)
{
	uintptr_t esi = (uintptr_t) LocBuf;
	uintptr_t edi = x;
	int eax = y;

	int edx = 280 + (Mbd/2);
	eax *= edx;
	edi += eax;
	
	int ecx;
	eax = color;

			edi += esi;

			edi += 1+7*(280+(Mbd/2));
			ecx = 3;
			_rep_stosd(edi, eax, ecx);
			_stosw(edi, eax);
			edi += (Mbd/2)+267;
			ecx = 3;
			_rep_stosd(edi, eax, ecx);
			_stosw(edi, eax);
			_stosb(edi, eax);
			edi += (Mbd/2)+266;
			ecx = 4;
			_rep_stosd(edi, eax, ecx);
			edi += (Mbd/2)+265;
			ecx = 4;
			_rep_stosd(edi, eax, ecx);
			_stosb(edi, eax);
			edi += (Mbd/2)+264;
			ecx = 4;
			_rep_stosd(edi, eax, ecx);
			_stosw(edi, eax);
			edi += (Mbd/2)+263;
			ecx = 4;
			_rep_stosd(edi, eax, ecx);
			_stosw(edi, eax);
			_stosb(edi, eax);
			edi += (Mbd/2)+262;
			ecx = 5;
			_rep_stosd(edi, eax, ecx);
			edi += (Mbd/2)+261;
			ecx = 4;
			_rep_stosd(edi, eax, ecx);
			_stosw(edi, eax);
			_stosb(edi, eax);
			edi += (Mbd/2)+262;
			ecx = 4;
			_rep_stosd(edi, eax, ecx);
			edi += (Mbd/2)+265;
			ecx = 3;
			_rep_stosd(edi, eax, ecx);
			_stosb(edi, eax);
			edi += (Mbd/2)+268;
			ecx = 2;
			_rep_stosd(edi, eax, ecx);
			_stosw(edi, eax);
			edi += (Mbd/2)+271;
			_stosd(edi, eax);
			_stosw(edi, eax);
			_stosb(edi, eax);
			edi += (Mbd/2)+274;
			_stosd(edi, eax);
			edi += (Mbd/2)+277;
			_stosb(edi, eax);
}



void DrawLocalH(void *LocBuf, int x, int y, int color)
{
	uintptr_t esi = (uintptr_t) LocBuf;
	uintptr_t edi = x;
	int eax = y;

	int edx = 280 + (Mbd/2);
	eax *= edx;
	edi += eax;
	
	int ecx;
	eax = color;

			edi += esi;

			edi += 13+7*(280+(Mbd/2));
			_stosw(edi, eax);
			edi += (Mbd/2)+276;
			_stosd(edi, eax);
			_stosw(edi, eax);
			edi += (Mbd/2)+272;
			ecx = 2;
			_rep_stosd(edi, eax, ecx);
			_stosw(edi, eax);
			edi += (Mbd/2)+268;
			ecx = 3;
			_rep_stosd(edi, eax, ecx);
			_stosw(edi, eax);
			edi += (Mbd/2)+264;
			ecx = 4;
			_rep_stosd(edi, eax, ecx);
			_stosw(edi, eax);
			edi += (Mbd/2)+260;
			ecx = 5;
			_rep_stosd(edi, eax, ecx);
			_stosw(edi, eax);
			edi += (Mbd/2)+256;
			ecx = 6;
			_rep_stosd(edi, eax, ecx);
			_stosw(edi, eax);
}



void DrawLocalI(void *LocBuf, int x, int y, int color)
{
	uintptr_t esi = (uintptr_t) LocBuf;
	uintptr_t edi = x;
	int eax = y;

	int edx = 280 + (Mbd/2);
	eax *= edx;
	edi += eax;
	
	int ecx;
	eax = color;

			edi += esi;

			edi += 13+7*(280+(Mbd/2));
			ecx = 3;
			_rep_stosd(edi, eax, ecx);
			_stosw(edi, eax);
			edi += (Mbd/2)+264;
			ecx = 3;
			_rep_stosd(edi, eax, ecx);
			_stosw(edi, eax);
			_stosb(edi, eax);
			edi += (Mbd/2)+263;
			ecx = 4;
			_rep_stosd(edi, eax, ecx);
			edi += (Mbd/2)+262;
			ecx = 4;
			_rep_stosd(edi, eax, ecx);
			_stosb(edi, eax);
			edi += (Mbd/2)+261;
			ecx = 4;
			_rep_stosd(edi, eax, ecx);
			_stosw(edi, eax);
			edi += (Mbd/2)+260;
			ecx = 4;
			_rep_stosd(edi, eax, ecx);
			_stosw(edi, eax);
			_stosb(edi, eax);
			edi += (Mbd/2)+259;
			ecx = 5;
			_rep_stosd(edi, eax, ecx);
			edi += (Mbd/2)+260;
			ecx = 4;
			_rep_stosd(edi, eax, ecx);
			_stosw(edi, eax);
			_stosb(edi, eax);
			edi += (Mbd/2)+263;
			ecx = 4;
			_rep_stosd(edi, eax, ecx);
			edi += (Mbd/2)+266;
			ecx = 3;
			_rep_stosd(edi, eax, ecx);
			_stosb(edi, eax);
			edi += (Mbd/2)+269;
			ecx = 2;
			_rep_stosd(edi, eax, ecx);
			_stosw(edi, eax);
			edi += (Mbd/2)+272;
			_stosd(edi, eax);
			_stosw(edi, eax);
			_stosb(edi, eax);
			edi += (Mbd/2)+275;
			_stosd(edi, eax);
			edi += (Mbd/2)+278;
			_stosb(edi, eax);
}



void DrawLocalJ(void *LocBuf, int x, int y, int color)
{
	uintptr_t esi = (uintptr_t) LocBuf;
	uintptr_t edi = x;
	int eax = y;

	int edx = 280 + (Mbd/2);
	eax *= edx;
	edi += eax;
	
	int ecx;
	eax = color;

			edi += esi;

			edi += 13;
			_stosw(edi, eax);
			edi += (Mbd/2)+276;
			_stosd(edi, eax);
			_stosw(edi, eax);
			edi += (Mbd/2)+272;
			ecx = 2;
			_rep_stosd(edi, eax, ecx);
			_stosw(edi, eax);
			edi += (Mbd/2)+268;
			ecx = 3;
			_rep_stosd(edi, eax, ecx);
			_stosw(edi, eax);
			edi += (Mbd/2)+264;
			ecx = 4;
			_rep_stosd(edi, eax, ecx);
			_stosw(edi, eax);
			edi += (Mbd/2)+260;
			ecx = 5;
			_rep_stosd(edi, eax, ecx);
			_stosw(edi, eax);
			edi += (Mbd/2)+256;
			ecx = 6;
			_rep_stosd(edi, eax, ecx);
			_stosw(edi, eax);
			edi += (Mbd/2)+254;
			ecx = 6;
			_rep_stosd(edi, eax, ecx);
			_stosw(edi, eax);
			edi += (Mbd/2)+255;
			ecx = 6;
			_rep_stosd(edi, eax, ecx);
			edi += (Mbd/2)+257;
			ecx = 5;
			_rep_stosd(edi, eax, ecx);
			_stosw(edi, eax);
			edi += (Mbd/2)+259;
			ecx = 5;
			_rep_stosd(edi, eax, ecx);
			edi += (Mbd/2)+261;
			ecx = 4;
			_rep_stosd(edi, eax, ecx);
			_stosw(edi, eax);
			edi += (Mbd/2)+263;
			ecx = 4;
			_rep_stosd(edi, eax, ecx);
			edi += (Mbd/2)+265;
			ecx = 3;
			_rep_stosd(edi, eax, ecx);
			_stosw(edi, eax);
			edi += (Mbd/2)+267;
			ecx = 3;
			_rep_stosd(edi, eax, ecx);
			edi += (Mbd/2)+269;
			ecx = 2;
			_rep_stosd(edi, eax, ecx);
			_stosw(edi, eax);
			edi += (Mbd/2)+271;
			ecx = 2;
			_rep_stosd(edi, eax, ecx);
			edi += (Mbd/2)+273;
			_stosd(edi, eax);
			_stosw(edi, eax);
			edi += (Mbd/2)+275;
			_stosd(edi, eax);
			edi += (Mbd/2)+277;
			_stosw(edi, eax);
}


void DrawLocalK(void *LocBuf, int x, int y, int color)
{
	uintptr_t esi = (uintptr_t) LocBuf;
	uintptr_t edi = x;
	int eax = y;

	int edx = 280 + (Mbd/2);
	eax *= edx;
	edi += eax;
	
	int ecx;
	eax = color;

			edi += esi;

			edi += 13;
			_stosw(edi, eax);
			edi += (Mbd/2)+276;
			_stosd(edi, eax);
			_stosb(edi, eax);
			edi += (Mbd/2)+273;
			ecx = 2;
			_rep_stosd(edi, eax, ecx);
			edi += (Mbd/2)+270;
			ecx = 2;
			_rep_stosd(edi, eax, ecx);
			_stosw(edi, eax);
			_stosb(edi, eax);
			edi += (Mbd/2)+267;
			ecx = 3;
			_rep_stosd(edi, eax, ecx);
			_stosw(edi, eax);
			edi += (Mbd/2)+264;
			ecx = 4;
			_rep_stosd(edi, eax, ecx);
			_stosb(edi, eax);
			edi += (Mbd/2)+261;
			ecx = 5;
			_rep_stosd(edi, eax, ecx);
			edi += (Mbd/2)+260;
			ecx = 5;
			_rep_stosd(edi, eax, ecx);
			_stosb(edi, eax);
			edi += (Mbd/2)+261;
			ecx = 5;
			_rep_stosd(edi, eax, ecx);
			edi += (Mbd/2)+262;
			ecx = 4;
			_rep_stosd(edi, eax, ecx);
			_stosw(edi, eax);
			_stosb(edi, eax);
			edi += (Mbd/2)+263;
			ecx = 4;
			_rep_stosd(edi, eax, ecx);
			_stosw(edi, eax);
			edi += (Mbd/2)+264;
			ecx = 4;
			_rep_stosd(edi, eax, ecx);
			_stosb(edi, eax);
			edi += (Mbd/2)+265;
			ecx = 4;
			_rep_stosd(edi, eax, ecx);
			edi += (Mbd/2)+266;
			ecx = 3;
			_rep_stosd(edi, eax, ecx);
			_stosw(edi, eax);
			_stosb(edi, eax);
}



void DrawLocalL(void *LocBuf, int x, int y, int color)
{
	uintptr_t esi = (uintptr_t) LocBuf;
	uintptr_t edi = x;
	int eax = y;

	int edx = 280 + (Mbd/2);
	eax *= edx;
	edi += eax;
	
	int ecx;
	eax = color;

			edi += esi;

			edi += 1+7*(280+(Mbd/2));
			ecx = 6;
			_rep_stosd(edi, eax, ecx);
			_stosw(edi, eax);
			edi += (Mbd/2)+256;
			ecx = 5;
			_rep_stosd(edi, eax, ecx);
			_stosw(edi, eax);
			edi += (Mbd/2)+260;
			ecx = 4;
			_rep_stosd(edi, eax, ecx);
			_stosw(edi, eax);
			edi += (Mbd/2)+264;
			ecx = 3;
			_rep_stosd(edi, eax, ecx);
			_stosw(edi, eax);
			edi += (Mbd/2)+268;
			ecx = 2;
			_rep_stosd(edi, eax, ecx);
			_stosw(edi, eax);
			edi += (Mbd/2)+272;
			_stosd(edi, eax);
			_stosw(edi, eax);
			edi += (Mbd/2)+276;
			_stosw(edi, eax);
}



void DrawLocalM(void *LocBuf, int x, int y, int color)
{
	uintptr_t esi = (uintptr_t) LocBuf;
	uintptr_t edi = x;
	int eax = y;

	int edx = 280 + (Mbd/2);
	eax *= edx;
	edi += eax;
	
	int ecx;
	eax = color;

			edi += esi;

			edi += 13;
			_stosw(edi, eax);
			edi += (Mbd/2)+277;
			_stosd(edi, eax);
			_stosb(edi, eax);
			edi += (Mbd/2)+274;
			ecx = 2;
			_rep_stosd(edi, eax, ecx);
			edi += (Mbd/2)+271;
			ecx = 2;
			_rep_stosd(edi, eax, ecx);
			_stosw(edi, eax);
			_stosb(edi, eax);
			edi += (Mbd/2)+268;
			ecx = 3;
			_rep_stosd(edi, eax, ecx);
			_stosw(edi, eax);
			edi += (Mbd/2)+265;
			ecx = 4;
			_rep_stosd(edi, eax, ecx);
			_stosb(edi, eax);
			edi += (Mbd/2)+262;
			ecx = 5;
			_rep_stosd(edi, eax, ecx);
			edi += (Mbd/2)+259;
			ecx = 5;
			_rep_stosd(edi, eax, ecx);
			_stosb(edi, eax);
			edi += (Mbd/2)+258;
			ecx = 5;
			_rep_stosd(edi, eax, ecx);
			edi += (Mbd/2)+259;
			ecx = 4;
			_rep_stosd(edi, eax, ecx);
			_stosw(edi, eax);
			_stosb(edi, eax);
			edi += (Mbd/2)+260;
			ecx = 4;
			_rep_stosd(edi, eax, ecx);
			_stosw(edi, eax);
			edi += (Mbd/2)+261;
			ecx = 4;
			_rep_stosd(edi, eax, ecx);
			_stosb(edi, eax);
			edi += (Mbd/2)+262;
			ecx = 4;
			_rep_stosd(edi, eax, ecx);
			edi += (Mbd/2)+263;
			ecx = 3;
			_rep_stosd(edi, eax, ecx);
			_stosw(edi, eax);
			_stosb(edi, eax);
}



void PutSpritePart1(uint8_t *screen, int sizes, uint8_t *data, int adding)
{	
	int size_low = sizes & 0xff;
	int size_high = sizes >> 8;
	int cl = size_low;
	int ch = size_high;
	
	while (true) {
			if (*data != 0) {
				if (*data == 63) {
					*screen = DarkingTable[*screen];
				} else {
					*screen = *data;
				}
			}
			
			screen++;
			data++;
			
			cl--;
			if (cl != 0) {
				continue;
			}
			
			cl = size_low;
			data += adding;
			screen += 560 + Mbd - size_low;
			
			ch--;
			if (ch != 0) {
				continue;
			}
			
			return;
	}
}

void PutSpritePart(void *screen, int sizes, void *data, int adding)
{
	PutSpritePart1((uint8_t*)screen, sizes, (uint8_t*)data, adding);
}


