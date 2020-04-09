//#include <EEPROM.h> // only Arduino Uno has EEPROM

#include "Flash.h"
#include "Patterns.h"
#include "PatternSelector.h"
#include "PatternFlasher.h"
#include "Pins.h"

// TODO: can I make these static?
int buttonState = HIGH;
int lastButtonState = HIGH;


// initialize the current active pattern as the one stored in flash
// TODO: Make these Singletons (ewwwww)
PatternSelector pattern_selector( ReadPatternFromROM() );
//PatternSelector pattern_selector( Pattern::WIGWAG );
PatternFlasher pattern_flasher( pattern_selector );

void setup()
{
    Serial.begin(9600);
    pinMode(L1, OUTPUT);
    pinMode(L2, OUTPUT);
    pinMode(L3, OUTPUT);
    pinMode(L4, OUTPUT);
    pinMode(LED_BUILTIN, OUTPUT);

    pinMode( buttonPin, INPUT );

    pinMode( L_turn, INPUT );
    pinMode( R_turn, INPUT );
    attachInterrupt( digitalPinToInterrupt( L_turn ), TripTurnSignal, RISING );
    attachInterrupt( digitalPinToInterrupt( R_turn ), TripTurnSignal, RISING );

    ResetOutputs();
}

void TripTurnSignal()
{
    ResetOutputs();
    ts_trigger = HIGH; // turn signal has been tripped
}

// reset all outputs to OFF
void ResetOutputs()
{
    digitalWrite(L1, LOW);
    digitalWrite(L2, LOW);
    digitalWrite(L3, LOW);
    digitalWrite(L4, LOW);
}

bool TurnSignalOn()
{
    return digitalRead( L_turn ) || digitalRead( R_turn );
}

// TODO: make this a function inside the PatternFlasher class
void FlashCurrentPattern() 
{
    ulong currentTime = millis();
    const uint num_flashes = 3; // the pattern will flash [num_flashes] times per side
    //unsigned int flashing_pins[2] = { pattern_selector.init_pins()[0], pattern_selector.init_pins()[1] };

    // if the pattern has changed, update the flasher
    if( pattern_flasher.CurrentPattern() != pattern_selector.CurrentPattern() )
    {
        pattern_flasher.UpdatePattern( pattern_selector );
        flash_count = 0;
    }

    // change the state of the machine every time interval
    if (currentTime - startTime >= interval)
    {
        startTime = millis();
        if ( state == LOW )
            flash_count++;
        state = !state; // switch from OFF to ON, or vice versa
    }

    // switches sides once it's flashed [num_flashes] times
    if ( flash_count > num_flashes )
    {
        // reset state and flash count
        flash_count = 0;
        state = LOW;

        pattern_flasher.Switch();
    }

    // send the appropriate state to the current selected pins
    pattern_flasher.UpdateLights( state );
}

// Debug Method
void PrintCurrentFlashingPattern()
{
    //Serial.print( "Button check: " );
    //Serial.println( EEPROM.isValid() );
    switch( pattern_selector.CurrentPattern() )
    {
        case Pattern::WIGWAG:
            Serial.println("WIGWAG");
            break;
        case Pattern::UPPER_LOWER:
            Serial.println("UPPER_LOWER");
            break;
        case Pattern::XSTROBE:
            Serial.println("XSTROBE");
            break;
        case Pattern::LOWER:
            Serial.println("LOWER");
            break;
        default:
            Serial.println("ERROR");
            break;                
    }
}

bool ButtonStateChanged( const bool buttonState, const bool lastButtonState )
{
    if(buttonState != lastButtonState) {
        // need the same reading for 30ms to count as a button state change
        int delay_count = 0;
        while ( delay_count++ < 3)
        {
            delay(10);
            if ( digitalRead(buttonPin) != buttonState ) // if there is a different reading within 30 ms, then we ditch the reading
            {
                return false;
            }
        }
    }
    else
    {
        return false;
    }

    return true;
}

bool TurnSignalDone()
{
    // still holding turn signal
    if( TurnSignalOn() )
    {
        ts_trigger = millis();
        return false;
    }

    return abs( millis() - ts_trigger_time ) > ts_wait;
}

void loop()
{
    if ( ts_trigger == HIGH )
    {
        ts_trigger = false; // reset the turn signal trigger
        ts_trigger_time = millis();
    }

    if ( TurnSignalDone() )
    {
        buttonState = digitalRead(buttonPin);
        // only want to cycle the pattern when the user can see the pattern change (strobe is on)
        // if the buttonState has been changed
        // should trigger on button release
        if( ButtonStateChanged( buttonState, lastButtonState ) ) {
            // if we have gone from PRESSED to RELEASED (using reverse logic)
            if ( buttonState == HIGH ) {
                ResetOutputs();
                pattern_selector.CyclePattern();
                digitalWrite(LED_BUILTIN, LOW);
                PrintCurrentFlashingPattern(); // debug
            }
            // if we have gone from RELEASED to PRESSED
            else {
                digitalWrite( LED_BUILTIN, HIGH );
            }
        }
        skip:

        FlashCurrentPattern();
        lastButtonState = buttonState;
    }
    else
    {
        ResetOutputs();
    }
}
