/* System Includes */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Arduino Includes */

#include <TaskAction.h>
#include <Adafruit_NeoPixel.h>

/* Local Includes */

#include "maglock.h"
#include "log.h"

/* Defines, Typedefs, Constants */

static uint8_t MAGLOCK_PINS[] = {8, 9};

/* Public Functions */

void maglock_setup()
{
	pinMode(MAGLOCK_PINS[0], OUTPUT);
	pinMode(MAGLOCK_PINS[1], OUTPUT);
}

void maglock_control(uint8_t maglock, bool enable)
{
	digitalWrite(MAGLOCK_PINS[maglock], enable ? LOW: HIGH);
}
