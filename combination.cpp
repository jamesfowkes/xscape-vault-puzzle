/* System Includes */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Arduino Includes */

#include <TaskAction.h>
#include <Adafruit_NeoPixel.h>

/* Local Includes */

#include "settings.h"
#include "combination.h"
#include "game.h"
#include "sm.h"
#include "log.h"

/* Defines, Typedefs, Constants */

static const uint8_t COMBINATION_PINS[2][5] = {
	{A4, A3, A2, A1, A0},
	{A12, A11, A10, A9, A8}
};

/* Private Variables */

static Adafruit_NeoPixel s_combination_pixels[2] = {
	Adafruit_NeoPixel(50, 4, NEO_GRB + NEO_KHZ800),
	Adafruit_NeoPixel(50, 5, NEO_GRB + NEO_KHZ800),
};

/* Private Functions */

static void set_column_pixels(Adafruit_NeoPixel& pixels, uint8_t column, uint8_t n)
{
	bool column_is_odd = column & 1;
	if (column_is_odd)
	{
		n = 9 - n;
	}

	bool set;
	for (uint8_t i=0; i<10; i++)
	{
		set =  column_is_odd ? (i > n) : (i < n);
		if (set)
		{
			pixels.setPixelColor((column*10)+i,
				COMBINATION_DISPLAY_COLOUR[0], COMBINATION_DISPLAY_COLOUR[1], COMBINATION_DISPLAY_COLOUR[2]);
		}
	}
}

static void display_combination(uint8_t team, uint8_t * combination)
{
	Adafruit_NeoPixel& pixels = s_combination_pixels[team];

	pixels.clear();
	for (uint8_t col=0; col<5; col++)
	{
		set_column_pixels(pixels, col, combination[col]);
	}
	pixels.show();
}

static void combination_task_fn(TaskAction * task)
{
	(void)task;
	uint16_t adc_reading;
	uint8_t combination[5];

	for(uint8_t k=0; k<2; k++)
	{
		for(uint8_t adc=0; adc<5; adc++)
		{
			adc_reading = 1023 - analogRead(COMBINATION_PINS[k][adc]);
			combination[adc] = adc_reading / 103;
		}
		game_set_combination(k, combination);
		display_combination(k, combination);
	}
}
static TaskAction s_combination_task(combination_task_fn, 100, INFINITE_TICKS);

/* Public Functions */

void combination_setup()
{
	s_combination_pixels[0].begin();
	s_combination_pixels[1].begin();

	s_combination_pixels[0].clear();
	s_combination_pixels[1].clear();

	for(uint8_t i=0; i<50; i++)
	{
		s_combination_pixels[0].setPixelColor(i,
			COMBINATION_DISPLAY_COLOUR[0], COMBINATION_DISPLAY_COLOUR[1], COMBINATION_DISPLAY_COLOUR[2]);
		s_combination_pixels[0].show();
		s_combination_pixels[1].setPixelColor(i,
			COMBINATION_DISPLAY_COLOUR[0], COMBINATION_DISPLAY_COLOUR[1], COMBINATION_DISPLAY_COLOUR[2]);
		s_combination_pixels[1].show();
		delay(20);
	}
	
}

void combination_tick()
{
	s_combination_task.tick();
}
