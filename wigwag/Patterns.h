#pragma once
#include "Pins.h"

ulong startTime;
const ulong interval = 80;
//const long switch_interval = 200; // TODO

uint flash_count;
bool state = LOW;

bool turn_signal_on = LOW;
const ulong ts_wait = 3000; // amount of time [ms] that we wait to resume our pattern
volatile bool ts_trigger = LOW; // Arduino Reference recommends setting variables changed by ISR as volatile
ulong ts_trigger_time = 0;

enum class Pattern
{
    WIGWAG = 0,
    XSTROBE = 1,
    UPPER_LOWER = 2,
    LOWER = 3,
    LAST = LOWER,
    FIRST = WIGWAG
};