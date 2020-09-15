#include <cstddef>
#include <array>
#include <random>

#include <stm32f4xx.h>

void delay (const size_t ticks)
{
	for (size_t i = 0; i < ticks; i++);
}

void basic_led_test()
{
	constexpr size_t led_pin_index = 13; // LED is connected to this pin.

	// Enable GPIOD clocking:
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN; // or: "RCC->AHB1ENR |= (1 << 3);" (because this 3rd bit)

	// Set output mode for 12th pin of the gpiod port (because the led is connected to it):
	GPIOD->MODER |= (1 << (led_pin_index * 2 + 0)); // or: GPIOD->MODER |= GPIO_MODER_MODER12_0;

	// Infinnite loop:
	while (true)
	{
		GPIOD->ODR ^= (1 << led_pin_index); // Invert 12th bit of GPIO port D
		delay(1000000); // Wait 1000000 tacts
	}
}

void test_std_array()
{
	std::array<size_t, 4> led_pins = {
		12, 13, 14, 15
	};


	// Enable GPIOD clocking:
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;

	// Set pin output modes:
	for (auto pin_number : led_pins) GPIOD->MODER |= (1 << (2 * pin_number + 0));

	// Mainloop:
	size_t current_led_index = 0;
	while (true)
	{
		GPIOD->ODR &= ~(1 << led_pins[current_led_index]); // Reset the old pin

		++current_led_index;
		if (current_led_index == led_pins.size())
			current_led_index = 0;

		GPIOD->ODR |= (1 << led_pins[current_led_index]); // Set the new pin

		delay(1000000); // Wait 1000000 tacts
	}
}


void test_std_random()
{
	std::array<size_t, 4> led_pins = {
		12, 13, 14, 15
	};

	std::mt19937 random_number_generator{};
	std::uniform_int_distribution<unsigned int> uniform_distribution(0, led_pins.size() - 1);
	// ^^^ Because this generates number from [start, end], but not from [start, end)

	// Enable GPIOD clocking:
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;

	// Set pin output modes:
	for (auto pin_number : led_pins) GPIOD->MODER |= (1 << (2 * pin_number + 0));

	// Mainloop:
	size_t current_led_index = 0;

	while (true)
	{
		GPIOD->ODR &= ~(1 << led_pins[current_led_index]); // Reset the old pin

		current_led_index = uniform_distribution.operator()(random_number_generator); // Just for demonstration reasons, DON`T DO IT...

		GPIOD->ODR |= (1 << led_pins[current_led_index]); // Set the new pin

		for (size_t tact_counter = 0; tact_counter < 1000000; tact_counter++); // Wait 1000000 tacts
	}
}

///////////////////////////////////
///								///
///								///
//			Main function		///
///								///
///								///
///////////////////////////////////
///
int main()
{
	basic_led_test();
	// test_std_array();
	// test_std_random();
}








/*
void Delay()
{
	int i;
	for (i = 0; i < 1000000; i++)
		asm("nop");
}

int main()
{
  GPIO_InitTypeDef GPIO_InitStructure;
  
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

  for (;;)
  {
	  GPIO_WriteBit(GPIOD, GPIO_Pin_12, Bit_SET);
	  Delay();
	  GPIO_WriteBit(GPIOD, GPIO_Pin_12, Bit_RESET);
	  Delay();
  }
}
*/
