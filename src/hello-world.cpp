#include "bsp_devtemp.hpp"
#include "bsp_init.hpp"
#include "bsp_leds.hpp"

#include <cstdint>

volatile uint32_t var = 0;
volatile uint32_t var2 = 0;
volatile uint32_t var3 = 0;
volatile bool state = false;
volatile bool state2 = false;
volatile bool state3 = false;

int main()
{
	bsp::init();

	bsp::set_ld1(true);

	while (1)
	{
		var = var + 1;

		var2 = bsp::get_tim2_counter();

		if ((var % 100000) == 0)
		{
			state = !state;
			bsp::set_ld4(state);

			state2 = (var2 % 2) == 0;
			bsp::set_ld2(state2);
		}
	}
}

extern "C" void TIM2_Handler()
{
	state3 = !state3;
	bsp::set_ld3(state3);

	bsp::reset_tim2_interrupt();
}
