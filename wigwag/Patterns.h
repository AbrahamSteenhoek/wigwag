#pragma once
#include "Pins.h"

unsigned long startTime;
const long interval = 100;
//const long switch_interval = 200; // TODO

unsigned int flash_count;
bool state = LOW;

enum class Pattern
{
    WIGWAG,
    XSTROBE,
    UPPER_LOWER,
    LOWER
};

class PatternSelector
{
    const unsigned int wigwag_pins_init[2] = { L1, L4 };
    const unsigned int xstrobe_pins_init[2] = { L1, L3 };
    const unsigned int upper_lower_pins_init[2] = { L1, L2 };
    const unsigned int lower_pins_init[2] = { L3, L4 };

    Pattern current_pattern;
public:
    PatternSelector(): current_pattern( Pattern::WIGWAG ) {}

    const unsigned int* init_pins()
    {
        switch ( current_pattern )
        {
            case Pattern::WIGWAG:
                return wigwag_pins_init;
                break;
            case Pattern::XSTROBE:
                return xstrobe_pins_init;
                break;
            case Pattern::UPPER_LOWER:
                return upper_lower_pins_init;
                break;
            case Pattern::LOWER:
                return lower_pins_init;
                break;
            default:
                return wigwag_pins_init;
                break;
        }
    }

    const Pattern CurrentPattern() { return current_pattern; }

    Pattern CyclePattern()
    {
        // don't ever do this. ew.
        // btw this is a way of incrementing over enums by casting the Pattern to int
        // if ( ++( *reinterpret_cast<int*>( &current_pattern )) == (int)Pattern::LAST )
        // {
        //     current_pattern = Pattern::FIRST;
        // }
        switch( current_pattern )
        {
            case Pattern::WIGWAG:
                current_pattern = Pattern::XSTROBE;
                break;
            case Pattern::XSTROBE:
                current_pattern = Pattern::UPPER_LOWER;
                break;
            case Pattern::UPPER_LOWER:
                current_pattern = Pattern::LOWER;
                break;
            case Pattern::LOWER:
                current_pattern = Pattern::WIGWAG; // goes back to the first pattern when we've reached the end of the list
                break;
        }
        return current_pattern;
    }

};

class PatternFlasher
{
    unsigned int flashing_pins[2] = { L1, L4 };
    Pattern current_pattern;
public:
    PatternFlasher(): current_pattern( Pattern::WIGWAG ) {}

    Pattern CurrentPattern() { return current_pattern; }

    void UpdatePattern( PatternSelector &pattern_selector )
    {
        current_pattern = pattern_selector.CurrentPattern();
        ReInitLights( pattern_selector.init_pins() );
//        flashing_pins[0] = pattern_selector.init_pins()[0];
//        flashing_pins[1] = pattern_selector.init_pins()[1];
    }

    void ReInitLights( const unsigned int* init_pins )
    {
        flashing_pins[0] = init_pins[0];
        flashing_pins[1] = init_pins[1];
    }

    void UpdateLights( bool state )
    {
        digitalWrite( flashing_pins[0], state );
        digitalWrite( flashing_pins[1], state );
    }

    void Switch()
    {
        // need to switch to different pins depending on what the current pattern is
        switch( current_pattern )
        {
            case Pattern::WIGWAG:
                flashing_pins[0] = (flashing_pins[0] == L1)? L2 : L1;
                flashing_pins[1] = (flashing_pins[1] == L4)? L3 : L4;
                break;
            case Pattern::XSTROBE:
                flashing_pins[0] = (flashing_pins[0] == L1)? L4 : L1;
                flashing_pins[1] = (flashing_pins[1] == L3)? L2 : L3;
                break;
            case Pattern::UPPER_LOWER:
                flashing_pins[0] = (flashing_pins[0] == L1)? L3 : L1;
                flashing_pins[1] = (flashing_pins[1] == L2)? L4 : L2;
                break;
            case Pattern::LOWER:
                // the lower pattern constantly flashes the lower lights
                break;
            default: // error case
                //TODO
                break;
        }
    }
};