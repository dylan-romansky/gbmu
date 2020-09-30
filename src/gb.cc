#include "gb.hpp"
#include <SDL2/SDL.h>

#define WIN_WIDTH 160
#define WIN_HEIGHT 144
#define CPU_FREQ 4194304
#define FRAME_TIME 70224


gb::gb()
{
	_cycles = 0;
	_mmu = std::make_shared<mmu>(dmg);
	_ppu = std::make_shared<ppu>(_mmu, dmg);
	_cpu = std::make_unique<cpu>(_mmu, _ppu);
}

gb::gb(sys_type type)
{
	_cycles = 0;
	_mmu = std::make_shared<mmu>(type);
	_ppu = std::make_shared<ppu>(_mmu, type);
	_cpu = std::make_unique<cpu>(_mmu, _ppu);
}

void	gb::load_cart(char *name)
{
	_mmu->loadCart(name);
	_cpu->reset();
}

void	gb::frame_advance()
{
	unsigned framecount = 0;
	unsigned cyc;
	while (framecount < FRAME_TIME)
	{
		_mmu->pollInput();
		cyc = _cpu->opcode_parse();
//		printf("cyc = %u\n\n", cyc);
//		_mmu->timerInc(cyc);
		_cycles += cyc;
		framecount += cyc;
		if (_cycles >= CPU_FREQ)
			_cycles -= CPU_FREQ;
	}
//	for (unsigned i = 0; i < 23040; i++)
//		printf("0x%08X\n", _ppu->pixels[23040]);
}
