#include "Patterns.h"
#include "Pins.h"

PatternSelector pattern_selector;

void setup() 
{
    Serial.begin(9600);
    pinMode(light1, OUTPUT);
    pinMode(light2, OUTPUT);
    pinMode(light3, OUTPUT);
    pinMode(light4, OUTPUT);

    ResetOutputs();
}

// reset all outputs to OFF
void ResetOutputs()
{
    digitalWrite(light2, LOW);
    digitalWrite(light3, LOW);
    digitalWrite(light1, LOW);
    digitalWrite(light4, LOW);
}

void FlashPattern() 
{
    unsigned long currentTime = millis();
    const unsigned int num_flashes = 3; // the pattern will flash [num_flashes] times per side
    unsigned int* wigwag_pins = wigwag_pins_init;
    if (currentTime - startTime >= interval) // change the state of the machine every time interval
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

        // if current selected pins were on the left side, then switch to right side
        wigwag_pins[0] = (wigwag_pins[0] == light1)? light2 : light1;
        wigwag_pins[1] = (wigwag_pins[1] == light4)? light3 : light4;
    }

    // send the appropriate state to the current selected pins
    digitalWrite ( wigwag_pins[0], state );
    digitalWrite ( wigwag_pins[1], state );
}

void Wigwag() 
{
    unsigned long currentTime = millis();
    const unsigned int num_flashes = 3; // the pattern will flash [num_flashes] times per side
    unsigned int* wigwag_pins = wigwag_pins_init;
    if (currentTime - startTime >= interval) // change the state of the machine every time interval
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

        // if current selected pins were on the left side, then switch to right side
        wigwag_pins[0] = (wigwag_pins[0] == light1)? light2 : light1;
        wigwag_pins[1] = (wigwag_pins[1] == light4)? light3 : light4;
    }

    // send the appropriate state to the current selected pins
    digitalWrite ( wigwag_pins[0], state );
    digitalWrite ( wigwag_pins[1], state );
}

void XStrobe()
{
    const unsigned int num_flashes = 3;
    unsigned long currentTime = millis();
    unsigned int* xstrobe_pins = xstrobe_pins_init;
    if (currentTime - startTime >= interval) // change the state of the current LED(side) every time interval
    {
        startTime = millis();
        if ( state == LOW )
            flash_count++;
        state = !state;
    }

    // switches sides once it's flashed [num_flashes] times
    if ( flash_count > num_flashes )
    {
    // reset state and flash count
    flash_count = 0;
    state = LOW;

    xstrobe_pins[0] = (xstrobe_pins[0] == light1)? light4 : light1;
    xstrobe_pins[1] = (xstrobe_pins[1] == light3)? light2 : light3;
    }

    digitalWrite ( xstrobe_pins[0], state );
    digitalWrite ( xstrobe_pins[1], state );
}

int buttonState = 0;
int lastButtonState = 0;

void loop()
{
    buttonState = digitalRead(buttonPin);
    
    // if the buttonState has been changed
    if(buttonState != lastButtonState) {
        // went from OFF to ON, button has been pushed
        // triggers on button release (LOW)
        delay(10);

        // check the reading three times in a row with 10ms delays in betweem to verify readings
        if(buttonState == lastButtonState) {
        goto skip;
        }
        delay(10);
        if(buttonState == lastButtonState) {
        goto skip;
        }
        if(buttonState == LOW) {
            ResetOutputs();
            //Serial.println(pattern_select);
            pattern_state = (pattern_state == true)? false : true;
            pattern_select++;
            if( pattern_select > 1 )
            {
                pattern_select = 0;
            }
        }
        // went from ON to OFF, button has been released
        else {

        }
    }
    skip:
    
    lastButtonState = buttonState;

    //*
    switch( pattern_select )
    {
        case 0:
            Wigwag();
            break;
        case 1:
            XStrobe();
            break;
        default:
            Wigwag();
            break;
    }
    //*/
}
