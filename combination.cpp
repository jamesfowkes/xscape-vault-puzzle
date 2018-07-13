/* System Includes */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Arduino Includes */

#include <TaskAction.h>

/* Local Includes */

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
	}
}
static TaskAction s_combination_task(combination_task_fn, 100, INFINITE_TICKS);

void combination_setup()
{

}

void combination_tick()
{
	s_combination_task.tick();
}
