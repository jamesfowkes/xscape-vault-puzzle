/* System Includes */

#include <stdint.h>
#include <stdlib.h>

/* Local Includes */

#include "game.h"
#include "sm.h"
#include "log.h"

/* Private Functions */

#ifndef ARDUINO
void sei() {}
void cli() {} 
#endif

static bool match_entry(const STATE_MACHINE_ENTRY& entry, GAME_STATE_ENUM state, GAME_EVENT_ENUM event)
{
	return (entry.from == state) && (entry.event == event);
}

static STATE_MACHINE_ENTRY const * find_entry_by_current_state_and_event(STATE_MACHINE *psm, GAME_EVENT_ENUM event)
{
	if (!psm) { return NULL; }

	for(uint8_t i = 0; i < psm->count; i++)
	{
		if (match_entry(psm->entries[i], psm->current_state, event)) { return &psm->entries[i]; }
	}
	return NULL;
}

static void push_event(STATE_MACHINE *psm, GAME_EVENT_ENUM event)
{
	if (psm->n_events < MAX_EVENTS)
	{
		psm->events[psm->n_events++] = event;
	}
}

static GAME_EVENT_ENUM pop_event(STATE_MACHINE *psm)
{
	GAME_EVENT_ENUM next_event = NUMBER_OF_EVENTS;
	if (psm->n_events)
	{
		next_event = psm->events[0];
		for (uint8_t i = 0; i < MAX_EVENTS-1; i++)
		{
			psm->events[i] = psm->events[i+1];
		}
		psm->events[MAX_EVENTS-1] = NUMBER_OF_EVENTS;
		psm->n_events--;
	}
	return next_event;
}

/* Public Functions */

void sm_run(STATE_MACHINE *psm)
{
	if (!psm) { return; }
	if (psm->n_events == 0) { return; }

	GAME_EVENT_ENUM next_event = pop_event(psm);

	STATE_MACHINE_ENTRY const * entry = find_entry_by_current_state_and_event(psm, next_event);
	if (entry)
	{
		psm->current_state = entry->to;
		if (entry->handler)
		{
			entry->handler(psm->data);
		}
	}
}

void sm_push_event(STATE_MACHINE *psm, GAME_EVENT_ENUM event)
{
	if (!psm) { return; }
	if (psm->n_events == MAX_EVENTS) { return; }

	cli();
	push_event(psm, event);
	sm_run(psm);
	sei();
}
