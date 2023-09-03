#ifndef HAL_HAL_GPIO_HPP
#define HAL_HAL_GPIO_HPP

#include "hal_core_types.hpp"
#include <type_traits>

#include <cstddef>

namespace hal
{

class __attribute__((packed)) Gpio
{
public:
	enum GpioNumber
	{
		GPIOA = 0,
		GPIOB,
		GPIOC,
		GPIOD,
		GPIOE,
		GPIOF,
		GPIOG,
		GPIOH,
		GPIOI,
		GPIOJ,
		GPIOK
	};

	enum class Mode : uint32_t
	{
		INPUT = 0b00,
		OUTPUT = 0b01,
		ALTFUNC = 0b10,
		ANALOG = 0b11
	};

	enum class OutputType : uint32_t
	{
		PUSHPULL = 0b0,
		OPENDRAIN = 0b1
	};

	enum class Speed : uint32_t
	{
		LOW = 0b00,
		MEDIUM = 0b01,
		HIGH = 0b10,
		VERYHIGH = 0b11
	};

	enum class Pull : uint32_t
	{
		NOPULLUP_PULLDOWN = 0b00,
		PULLUP = 0b01,
		PULLDOWN = 0b10
	};

	Gpio()
	{
		static_assert(offsetof(Gpio, AFRH) == 0x24, "Invalid GPIO memory map");
		static_assert(std::is_standard_layout<Gpio>::value, "GPIO is not standard layout");
	}

	void* operator new(size_t, GpioNumber number)
	{
		return reinterpret_cast<void*>(0x40020000 + number * 0x400);
	}

	void set_mode(uint8_t pin, Mode mode)
	{
		MODER &= ~(0b11 << (pin * 2));
		MODER |= (static_cast<uint32_t>(mode) << (pin * 2));
	}

	void set_output_type(uint8_t pin, OutputType type)
	{
		OTYPER &= ~(1 << pin);
		OTYPER |= (static_cast<uint32_t>(type) << pin);
	}

	void set_speed(uint8_t pin, Speed speed)
	{
		OSPEEDR &= ~(0b11 << (pin * 2));
		OSPEEDR |= (static_cast<uint32_t>(speed) << (pin * 2));
	}

	void set_pull(uint8_t pin, Pull pull)
	{
		PUPDR &= ~(0b11 << (pin * 2));
		PUPDR |= (static_cast<uint32_t>(pull) << (pin * 2));
	}

	void set_output_high(uint8_t pin)
	{
		BSRR = (1 << pin);
	}

	void set_output_low(uint8_t pin)
	{
		BSRR = (1 << (pin + 16));
	}

private:
	device_register MODER;
	device_register OTYPER;
	device_register OSPEEDR;
	device_register PUPDR;
	device_register IDR;
	device_register ODR;
	device_register BSRR;
	device_register LCKR;
	device_register AFRL;
	device_register AFRH;
};

class GpioPin
{
public:
	GpioPin(Gpio& gpio, uint8_t pin) : gpio_(gpio), pin_(pin)
	{
	}

	void setup_for_output()
	{
		gpio_.set_mode(pin_, Gpio::Mode::OUTPUT);
		gpio_.set_output_type(pin_, Gpio::OutputType::PUSHPULL);
		gpio_.set_speed(pin_, Gpio::Speed::HIGH);
		gpio_.set_pull(pin_, Gpio::Pull::NOPULLUP_PULLDOWN);
	}

	void set_output_high()
	{
		gpio_.set_output_high(pin_);
	}

	void set_output_low()
	{
		gpio_.set_output_low(pin_);
	}

private:
	Gpio& gpio_;
	const uint8_t pin_;
};

}  // namespace hal

#endif  // HAL_HAL_GPIO_HPP
