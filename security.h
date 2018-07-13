#ifndef _SECURITY_H_
#define _SECURITY_H_

typedef enum
{
	SECURITY_LEVEL_OFF,
	SECURITY_LEVEL_LOW,
	SECURITY_LEVEL_HIGH,
	SECURITY_LEVEL_FAIL
} eSecurityLevel;

void security_setup();
void security_tick();

void security_set_level(uint8_t team, eSecurityLevel level);

#endif
