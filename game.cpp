/* System Includes */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Local Includes */

#include "settings.h"
#include "security.h"
#include "maglock.h"
#include "game.h"
#include "sm.h"
#include "log.h"

/* Private Data */

static GAME_DATA s_game_data[2] = {
	{0, {0,0,0,0}, {0,0,0,0,0}},
	{1, {0,0,0,0}, {0,0,0,0,0}},
};

/* Private Functions */

static bool match_keypad_codes(uint8_t const * const p1, uint8_t const * const p2)
{
	return memcmp(p1, p2, 4) == 0;
}

static bool match_combinations(uint8_t const * const p1, uint8_t const * const p2)
{
	return memcmp(p1, p2, 5) == 0;
}

/* Game State Machine */

static void on_keypad_entry(GAME_DATA& data);
static void on_combination_change(GAME_DATA& data);
static void on_security_enabled(GAME_DATA& data);
static void on_game_won(GAME_DATA& data);
static void on_game_lost(GAME_DATA& data);
static void on_security_disabled(GAME_DATA& data);

static const STATE_MACHINE_ENTRY s_state_machine_entries[] = {

	{STATE_NORMAL, EVENT_KEYPAD_ENTRY, STATE_NORMAL, on_keypad_entry},
	{STATE_NORMAL, EVENT_COMBINATION_CHANGE, STATE_NORMAL, on_combination_change},
	{STATE_NORMAL, EVENT_ENABLE_SECURITY, STATE_SECURE, on_security_enabled},
	{STATE_NORMAL, EVENT_UNLOCK, STATE_WON, on_game_won},
	{STATE_NORMAL, EVENT_LOCKOUT, STATE_LOST, on_game_lost},
	{STATE_NORMAL, EVENT_DISABLE_SECURITY, STATE_NORMAL, NULL},

	{STATE_SECURE, EVENT_KEYPAD_ENTRY, STATE_SECURE, on_keypad_entry},
	{STATE_SECURE, EVENT_COMBINATION_CHANGE, STATE_SECURE, NULL},
	{STATE_SECURE, EVENT_ENABLE_SECURITY, STATE_SECURE, NULL},
	{STATE_SECURE, EVENT_UNLOCK, STATE_SECURE, NULL},
	{STATE_SECURE, EVENT_LOCKOUT, STATE_LOST, on_game_lost},
	{STATE_SECURE, EVENT_DISABLE_SECURITY, STATE_NORMAL, on_security_disabled}
};

static STATE_MACHINE s_state_machines[] = {
	BUILD_SM(s_state_machine_entries, STATE_NORMAL, s_game_data[0]),
	BUILD_SM(s_state_machine_entries, STATE_NORMAL, s_game_data[1])
};

/* State Machine Handler Functions */

static void on_keypad_entry(GAME_DATA& data)
{
	bool match = true;
	pln("Team %d got keypad entry %d, %d, %d, %d", data.team+1, data.keypad[0], data.keypad[1], data.keypad[2], data.keypad[3]);
	if (data.team == TEAM1 && match_keypad_codes(data.keypad, TEAM1_SECURITY_ENABLE_CODE))
	{
		sm_push_event(&s_state_machines[TEAM2], EVENT_ENABLE_SECURITY);
	}
	else if (data.team == TEAM2 && match_keypad_codes(data.keypad, TEAM2_SECURITY_ENABLE_CODE))
	{
		sm_push_event(&s_state_machines[TEAM1], EVENT_ENABLE_SECURITY);
	}
	else if (data.team == TEAM1 && match_keypad_codes(data.keypad, TEAM1_SECURITY_DISABLE_CODE))
	{
		sm_push_event(&s_state_machines[TEAM1], EVENT_DISABLE_SECURITY);
	}
	else if (data.team == TEAM2 && match_keypad_codes(data.keypad, TEAM2_SECURITY_DISABLE_CODE))
	{
		sm_push_event(&s_state_machines[TEAM2], EVENT_DISABLE_SECURITY);
	}
	else
	{
		match = false;
	}

	if (match)
	{
		// Reset both keypad histories to force both teams to enter complete codes
		memset(s_game_data[0].keypad, 0, 4);
		memset(s_game_data[1].keypad, 0, 4);
	}
}

static void on_combination_change(GAME_DATA& data)
{
	pln("Team %d got combination %d, %d, %d, %d, %d", data.team+1, data.combination[0], data.combination[1], data.combination[2], data.combination[3], data.combination[4]);
	if (data.team == TEAM1 && match_combinations(data.combination, TEAM1_WINNING_COMBINATION))
	{
		sm_push_event(&s_state_machines[TEAM1], EVENT_UNLOCK);
		sm_push_event(&s_state_machines[TEAM2], EVENT_LOCKOUT);
	}
	else if (data.team == TEAM2 && match_combinations(data.combination, TEAM2_WINNING_COMBINATION))
	{
		sm_push_event(&s_state_machines[TEAM1], EVENT_LOCKOUT);
		sm_push_event(&s_state_machines[TEAM2], EVENT_UNLOCK);
	}
}

static void on_security_enabled(GAME_DATA& data)
{
	pln("Enabling security for team %d", data.team+1);
	security_set_level(data.team, SECURITY_LEVEL_HIGH);
}

static void on_game_won(GAME_DATA& data)
{
	pln("Team %d won!", data.team+1);
	security_set_level(data.team, SECURITY_LEVEL_OFF);
	maglock_control(data.team, true);
}

static void on_game_lost(GAME_DATA& data)
{
	pln("Team %d lost!", data.team+1);
	security_set_level(data.team, SECURITY_LEVEL_FAIL);
}

static void on_security_disabled(GAME_DATA& data)
{
	pln("Disabling security for team %d", data.team+1);
	security_set_level(data.team, SECURITY_LEVEL_LOW);
}

static void game_state_keypad_update(GAME_DATA& data, char key)
{
	data.keypad[0] = data.keypad[1];
	data.keypad[1] = data.keypad[2];
	data.keypad[2] = data.keypad[3];
	data.keypad[3] = key;
}

static bool game_combination_update(GAME_DATA& data, uint8_t * combination)
{
	bool match = memcmp(data.combination, combination, 5) == 0;
	if (!match)
	{
		memcpy(data.combination, combination, 5);
	}
	return !match;
}

void game_new_keypad_entry(uint8_t team, char key)
{
	pln("Got keypress '%c' for team %d", key, team+1);
	game_state_keypad_update(s_game_data[team], key);
	sm_push_event(&s_state_machines[team], EVENT_KEYPAD_ENTRY);
}

void game_set_combination(uint8_t team, uint8_t * combination)
{
	if (game_combination_update(s_game_data[team], combination))
	{
		sm_push_event(&s_state_machines[team], EVENT_COMBINATION_CHANGE);
	}
}

void game_security_timeout_end(uint8_t team)
{
	pln("Security timeout team %d", team);
	sm_push_event(&s_state_machines[team], EVENT_DISABLE_SECURITY);
}

#ifndef ARDUINO

#include <iostream>
#include <assert.h>

static void set_combination(uint8_t team, uint8_t comb1, uint8_t comb2, uint8_t comb3, uint8_t comb4, uint8_t comb5)
{
	s_game_data[team].combination[0] = comb1;
	s_game_data[team].combination[1] = comb2;
	s_game_data[team].combination[2] = comb3;
	s_game_data[team].combination[3] = comb4;
	s_game_data[team].combination[4] = comb5;
}

static void set_keypad(uint8_t team, uint8_t key1, uint8_t key2, uint8_t key3, uint8_t key4)
{
	s_game_data[team].keypad[0] = key1;
	s_game_data[team].keypad[1] = key2;
	s_game_data[team].keypad[2] = key3;
	s_game_data[team].keypad[3] = key4;
}

static void reset_game()
{
	s_state_machines[TEAM1].current_state = STATE_NORMAL;
	s_state_machines[TEAM2].current_state = STATE_NORMAL;
}


int main(int argc, char * argv[])
{
	(void)argc; (void)argv;

	pln("Testing team 2 security...");
	set_keypad(TEAM1, '1', '1', '1', '1');
	sm_push_event(&s_state_machines[TEAM1], EVENT_KEYPAD_ENTRY);
	assert(s_state_machines[TEAM2].current_state == STATE_SECURE);

	set_combination(TEAM2, 0, 1, 2, 3, 4);
	sm_push_event(&s_state_machines[TEAM2], EVENT_COMBINATION_CHANGE);
	assert(s_state_machines[TEAM2].current_state == STATE_SECURE);

	set_keypad(TEAM2, '4', '4', '4', '4');
	sm_push_event(&s_state_machines[TEAM2], EVENT_KEYPAD_ENTRY);
	assert(s_state_machines[TEAM2].current_state == STATE_NORMAL);

	pln("Testing team 1 security...");
	set_keypad(TEAM2, '2', '2', '2', '2');
	sm_push_event(&s_state_machines[TEAM2], EVENT_KEYPAD_ENTRY);
	assert(s_state_machines[TEAM1].current_state == STATE_SECURE);

	set_combination(TEAM1, 0, 1, 2, 3, 4);
	sm_push_event(&s_state_machines[TEAM1], EVENT_COMBINATION_CHANGE);
	assert(s_state_machines[TEAM1].current_state == STATE_SECURE);

	set_keypad(TEAM1, '3', '3', '3', '3');
	sm_push_event(&s_state_machines[TEAM1], EVENT_KEYPAD_ENTRY);
	assert(s_state_machines[TEAM1].current_state == STATE_NORMAL);

	pln("Testing team 1 win...");
	set_combination(TEAM1, 0, 1, 2, 3, 4);
	sm_push_event(&s_state_machines[TEAM1], EVENT_COMBINATION_CHANGE);
	assert(s_state_machines[TEAM1].current_state == STATE_WON);
	assert(s_state_machines[TEAM2].current_state == STATE_LOST);

	reset_game();

	pln("Testing team 2 win...");
	set_combination(TEAM2, 5, 6, 7, 8, 9);
	sm_push_event(&s_state_machines[TEAM2], EVENT_COMBINATION_CHANGE);
	assert(s_state_machines[TEAM1].current_state == STATE_LOST);
	assert(s_state_machines[TEAM2].current_state == STATE_WON);

	return 0;
}

#endif
