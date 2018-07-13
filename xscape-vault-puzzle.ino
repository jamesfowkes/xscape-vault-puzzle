/* System Includes */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Local Includes */

#include "game-keypad.h"
#include "combination.h"
#include "game.h"
#include "security.h"
#include "maglock.h"
#include "sm.h"
#include "log.h"

void setup()
{
	Serial.begin(115200);
	keypad_setup();
	combination_setup();
	security_setup();
	maglock_setup();

	maglock_control(0, false);
	maglock_control(0, false);

	pln("Bank Vault Security Puzzle");

	security_set_level(0, SECURITY_LEVEL_OFF);
	security_set_level(1, SECURITY_LEVEL_OFF);

	delay(250);
	security_set_level(0, SECURITY_LEVEL_LOW);
	security_set_level(1, SECURITY_LEVEL_LOW);
	delay(250);
	security_set_level(0, SECURITY_LEVEL_HIGH);
	security_set_level(1, SECURITY_LEVEL_HIGH);
	delay(250);
	security_set_level(0, SECURITY_LEVEL_FAIL);
	security_set_level(1, SECURITY_LEVEL_FAIL);
	delay(250);

	security_set_level(0, SECURITY_LEVEL_LOW);
	security_set_level(1, SECURITY_LEVEL_LOW);
}

void loop()
{
	keypad_tick();
	combination_tick();
	security_tick();
}
