#pragma once
#include "Patterns.h"
#include "Pins.h"

class PatternFlasher
{
    uint flashing_pins[2] = { L1, L4 };
    Pattern current_pattern;
public:
    PatternFlasher(): current_pattern( Pattern::WIGWAG ) {}
    PatternFlasher( PatternSelector &p_selector ) { UpdatePattern( p_selector ); }

    Pattern CurrentPattern() { return current_pattern; }

    void UpdatePattern( PatternSelector &p_selector )
    {
        current_pattern = p_selector.CurrentPattern();
        ReInitLights( p_selector.init_pins() );
    }

    void ReInitLights( const uint* init_pins )
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
                // same as WIGWAG, just only flashes the lower lights
                flashing_pins[1] = (flashing_pins[1] == L4)? L3 : L4;
                flashing_pins[0] = flashing_pins[1];
                break;
            default: // error case
                //TODO
                break;
        }
    }
};