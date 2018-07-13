/* System Includes */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Arduino Includes */

#include <TaskAction.h>
#include <Adafruit_NeoPixel.h>

/* Local Includes */

#include "settings.h"
#include "security.h"
#include "game.h"
#include "log.h"

/* Private Data */

#define NUMLINES 3
#define PIXELS_PER_LINE 7
#define NUMPIXELS (NUMLINES * PIXELS_PER_LINE)

static const uint8_t RELAY_PINS[2] = {8,9};

static uint16_t s_timeouts[2];
static Adafruit_NeoPixel s_security_pixels[2] = {
	Adafruit_NeoPixel(NUMPIXELS, 2, NEO_GRB + NEO_KHZ800),
	Adafruit_NeoPixel(NUMPIXELS, 3, NEO_GRB + NEO_KHZ800),
};

/* Private Functions */

static void set_panel(uint8_t team, uint8_t const * const pcolors)
{
	for(uint8_t p=0; p<NUMPIXELS; p++)
	{
		s_security_pixels[team].setPixelColor(p, pcolors[0], pcolors[1], pcolors[2]);
	}
	s_security_pixels[team].show();
}

static void set_line(uint8_t team, uint8_t line, uint8_t const * const pcolors)
{
	uint8_t start = line*PIXELS_PER_LINE;
	uint8_t end = start+PIXELS_PER_LINE;

	s_security_pixels[team].clear();
	for(uint8_t p=start; p<end; p++)
	{
		s_security_pixels[team].setPixelColor(p, pcolors[0], pcolors[1], pcolors[2]);
	}
	s_security_pixels[team].show();
}

static void security_task_fn(TaskAction* task)
{
	(void)task;

	for(uint8_t k=0; k<2; k++)
	{
		if (s_timeouts[k])
		{
			s_timeouts[k]--;
			if (s_timeouts[k] == 0)
			{
				security_set_level(k, SECURITY_LEVEL_LOW);
				game_security_timeout_end(k);
			}
		}
	}
}
static TaskAction s_security_task(security_task_fn, 1000, INFINITE_TICKS);

/* Public Functions */

void security_set_level(uint8_t team, eSecurityLevel level)
{
	s_timeouts[team] = 0;
	switch(level)
	{
	case SECURITY_LEVEL_OFF:
		set_line(team, 0, SECURITY_LEVEL_OFF_COLOUR);
		digitalWrite(RELAY_PINS[team], HIGH);
		break;
	case SECURITY_LEVEL_LOW:
		set_line(team, 1, SECURITY_LEVEL_LOW_COLOUR);
		break;
	case SECURITY_LEVEL_HIGH:
		s_timeouts[team] = SECURITY_TIMEOUT_IN_SECONDS;
		set_line(team, 2, SECURITY_LEVEL_HIGH_COLOUR);
		break;
	case SECURITY_LEVEL_FAIL:
		set_panel(team, SECURITY_LEVEL_HIGH_COLOUR);

	}
}

void security_setup()
{
	s_security_pixels[0].begin();
	s_security_pixels[1].begin();

	pinMode(RELAY_PINS[0], OUTPUT);
	pinMode(RELAY_PINS[1], OUTPUT);
}

void security_tick()
{
	s_security_task.tick();
}
