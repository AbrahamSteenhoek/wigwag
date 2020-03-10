#pragma once
#include "Flash.h"
#include "Patterns.h"
#include "Pins.h"

class PatternSelector
{
    const uint wigwag_pins_init[2] = { L1, L4 };
    const uint xstrobe_pins_init[2] = { L1, L3 };
    const uint upper_lower_pins_init[2] = { L1, L2 };
    const uint lower_pins_init[2] = { L4, L4 };

    Pattern current_pattern;
public:
    PatternSelector(): current_pattern( Pattern::WIGWAG ) {}
    PatternSelector( Pattern pattern ):  current_pattern( pattern ) {}

    const uint* init_pins()
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

        // Do this instead! It's more verbose and primitive, but for the limited pattern options we have, it works
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

        // change the current active pattern in the Flash Memory
        
        StorePatternToROM( current_pattern );

        return current_pattern;
    }
};