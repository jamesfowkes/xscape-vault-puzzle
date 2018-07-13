#ifndef _SM_H_
#define _SM_H_

/* Defines, typedefs, consts */

#define MAX_EVENTS 4

#define BUILD_SM(entries, initial_state, data) { \
	entries, \
	initial_state, \
	{NUMBER_OF_EVENTS, NUMBER_OF_EVENTS, NUMBER_OF_EVENTS, NUMBER_OF_EVENTS}, \
	0, \
	data, \
	sizeof(entries) / sizeof(STATE_MACHINE_ENTRY) \
}

struct state_machine_entry
{
	GAME_STATE_ENUM from;
	GAME_EVENT_ENUM event;
	GAME_STATE_ENUM to;
	SM_HANDLER handler;
};
typedef struct state_machine_entry STATE_MACHINE_ENTRY;

struct state_machine
{
	STATE_MACHINE_ENTRY const * const entries;
	GAME_STATE_ENUM current_state;
	GAME_EVENT_ENUM events[MAX_EVENTS];
	uint8_t n_events;
	GAME_DATA& data;
	uint8_t count;
};
typedef struct state_machine STATE_MACHINE;

void sm_push_event(STATE_MACHINE *psm, GAME_EVENT_ENUM event);

#endif
