#ifndef _CODES_H_
#define _CODES_H_

// Keytpad codes for each team
static const uint8_t TEAM1_SECURITY_ENABLE_CODE[] = {'1', '1', '1', '1'};
static const uint8_t TEAM2_SECURITY_ENABLE_CODE[] = {'2', '2', '2', '2'};
static const uint8_t TEAM1_SECURITY_DISABLE_CODE[] = {'3', '3', '3', '3'};
static const uint8_t TEAM2_SECURITY_DISABLE_CODE[] = {'4', '4', '4', '4'};

// Combinations for each team
static const uint8_t TEAM1_WINNING_COMBINATION[] = {0, 1, 2, 3, 4};
static const uint8_t TEAM2_WINNING_COMBINATION[] = {5, 6, 7, 8, 9};

// Security system timeouts
static const uint16_t SECURITY_TIMEOUT_IN_SECONDS = 30;

// Security system colours
static const uint8_t SECURITY_LEVEL_OFF_COLOUR[]	= {0, 0, 150};
static const uint8_t SECURITY_LEVEL_LOW_COLOUR[]	= {0, 150, 0};
static const uint8_t SECURITY_LEVEL_HIGH_COLOUR[]	= {150, 0, 0};

// Combination display colours
static const uint8_t COMBINATION_DISPLAY_COLOUR[] = {64, 64, 32};

#endif
