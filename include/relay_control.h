#ifndef RELAY_CONTROL_H
#define RELAY_CONTROL_H

#include <stdbool.h>

void relay_init(void);
void relay_set(bool turn_on);

#endif