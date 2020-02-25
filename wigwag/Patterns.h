#pragma once
#include "Pins.h"

unsigned long startTime;
const long interval = 100;
const long switch_interval = 200;

unsigned int flash_count;
bool state = LOW;

bool pattern_state = true;
unsigned int pattern_select = 0;

enum class Patterns
{
    WIGWAG,
    XSTROBE,
    UPPER_LOWER,
    LOWER,
    FIRST=WIGWAG,
    LAST=LOWER
};

class PatternSelector
{
    const unsigned int wigwag_pins_init[2] = { light2, light3 };
    const unsigned int xstrobe_pins_init[2] = { light1, light3 };
    const unsigned int upper_lower_pins_init[2] = { light1, light2 };
    const unsigned int lower_pins_init[2] = { light3, light4 };

    Patterns current_pattern;
public:
    PatternSelector() {}

    const unsigned int* init_pins( Patterns pattern_type )
    {
        switch ( pattern_type )
        {
            case Patterns::WIGWAG:
                return wigwag_pins_init;
                break;
            case Patterns::XSTROBE:
                return xstrobe_pins_init;
                break;
            case Patterns::UPPER_LOWER:
                return upper_lower_pins_init;
                break;
            case Patterns::LOWER:
                return lower_pins_init;
                break;
            default:
                return wigwag_pins_init;
                break;
        }
    }

    const Patterns CurrentPattern() { return current_pattern; }

    Patterns CyclePattern()
    {
        //TODO
        switch( current_pattern )
        {
        }
    }

};