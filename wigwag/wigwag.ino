#include "Patterns.h"
#include "Pins.h"

PatternSelector pattern_selector;
PatternFlasher pattern_flasher;

void setup() 
{
    Serial.begin(9600);
    pinMode(L1, OUTPUT);
    pinMode(L2, OUTPUT);
    pinMode(L3, OUTPUT);
    pinMode(L4, OUTPUT);

    ResetOutputs();
}

// reset all outputs to OFF
void ResetOutputs()
{
    digitalWrite(L1, LOW);
    digitalWrite(L2, LOW);
    digitalWrite(L3, LOW);
    digitalWrite(L4, LOW);
}

void FlashCurrentPattern() 
{
    unsigned long currentTime = millis();
    const unsigned int num_flashes = 3; // the pattern will flash [num_flashes] times per side
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

        /*/
        // need to switch to different pins depending on what the current pattern is
        switch( pattern_selector.CurrentPattern() )
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
                ResetOutputs();
                break;
        }
        //*/
        PrintCurrentFlashingPattern();
    }

    // send the appropriate state to the current selected pins
    //digitalWrite( flashing_pins[0], state );
    //digitalWrite( flashing_pins[1], state );
    pattern_flasher.Flash( state );
}

void PrintCurrentFlashingPattern()
{
    switch( pattern_flasher.CurrentPattern() )
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
        // We've registered a button press
        if(buttonState == LOW) {
            ResetOutputs();
            pattern_selector.CyclePattern();
            PrintCurrentFlashingPattern();
        }
        // went from ON to OFF, button has been released
        else {

        }
    }
    skip:
    
    lastButtonState = buttonState;
    FlashCurrentPattern();
    /*
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
