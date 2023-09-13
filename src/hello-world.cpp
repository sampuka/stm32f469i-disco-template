#include "bsp_core.hpp"
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

void toggle_ld3()
{
	static uint32_t counter = 0;

	counter++;

	if (counter == 1000)
	{
		state3 = !state3;
		bsp::set_ld3(state3);

		counter = 0;
	}
}

int main()
{
	bsp::init();

	bsp::register_1ms_callback(toggle_ld3);

	bsp::set_ld1(true);

	while (1)
	{
		;
	}
}
