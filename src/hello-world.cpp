#include "bsp_core.hpp"
#include "bsp_display.hpp"
#include "bsp_init.hpp"
#include "bsp_leds.hpp"

#include <cstdint>

void toggle_ld3()
{
	static uint32_t counter = 0;
	static bool ld3_state = false;

	counter++;

	if (counter == 1000)
	{
		ld3_state = !ld3_state;
		bsp::set_ld3(ld3_state);

		counter = 0;
	}
}

int main()
{
	bsp::init();

	bsp::register_1ms_callback(toggle_ld3);

	bsp::set_ld1(true);

	bsp::Display display;

	display.power_on();

	while (1)
	{
		;
	}
}
