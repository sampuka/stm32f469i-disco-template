#include "bsp/include/bsp_init.hpp"
#include "bsp/include/bsp_leds.hpp"

#include <cstdint>

volatile uint32_t var = 0;
volatile bool state = false;

int main()
{
	bsp::init();

	bsp::set_ld1(true);
	bsp::set_ld2(false);
	bsp::set_ld3(true);
	bsp::set_ld4(false);

	while (1)
	{
		var = var + 1;

		if ((var % 100000) == 0)
		{
			state = !state;
			bsp::set_ld4(state);
		}
	}
}
