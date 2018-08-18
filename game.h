#ifndef _GAME_STATE_H_
#define _GAME_STATE_H_

/* Defines, typedefs, consts */

static const uint8_t TEAM1 = 0;
static const uint8_t TEAM2 = 1;

enum game_event_enum
{
	EVENT_KEYPAD_ENTRY,
	EVENT_ENABLE_SECURITY,
	EVENT_DISABLE_SECURITY,
	EVENT_COMBINATION_CHANGE,
	EVENT_UNLOCK,
	EVENT_LOCKOUT,
	NUMBER_OF_EVENTS
};
typedef enum game_event_enum GAME_EVENT_ENUM;

enum game_state_enum
{
	STATE_NORMAL,
	STATE_SECURE,
	STATE_WON,
	STATE_LOST
};
typedef enum game_state_enum GAME_STATE_ENUM;

struct game_data
{
	uint8_t team;
	uint8_t keypad[4];
	uint8_t combination[5];
	bool locked_out_once;
};
typedef struct game_data GAME_DATA;

typedef void (*SM_HANDLER)(GAME_DATA& data);

void game_setup();
void game_new_keypad_entry(uint8_t team, char key);
void game_set_combination(uint8_t team, uint8_t * combination);
void game_security_timeout_end(uint8_t team);

#endif
