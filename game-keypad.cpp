/* System Includes */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Arduino Includes */

#include "Keypad.h"

/* Local Includes */

#include "game-keypad.h"
#include "game.h"
#include "sm.h"
#include "log.h"

/* Private Variables */

static const uint8_t KEYPAD_ROWPINS[2][4] = {
	{29,27,25,23},
	{45,43,41,39}
};

static const uint8_t KEYPAD_COLPINS[2][3] = {
	{35,33,31},
	{51,49,47}
};

static char KEYPAD_KEYS[4][3] = {
	{ '1','2','3' },
	{ '4','5','6' },
	{ '7','8','9' },
	{ '*','0','#' },
};

static Keypad s_keypads[] = {
	Keypad(makeKeymap(KEYPAD_KEYS), (byte*)KEYPAD_ROWPINS[0], (byte*)KEYPAD_COLPINS[0], 4, 3),
	Keypad(makeKeymap(KEYPAD_KEYS), (byte*)KEYPAD_ROWPINS[1], (byte*)KEYPAD_COLPINS[1], 4, 3)
};


/* Public Functions */

void keypad_setup()
{
}

void keypad_tick()
{
	char key;

	for(uint8_t k=0; k <2; k++)
	{
		key = s_keypads[k].getKey();
		if (key)
		{
			game_new_keypad_entry(k, key);
		}
	}
}
