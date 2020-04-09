#pragma once
//#include <FlashAsEEPROM.h>
//#include <FlashStorage.h> // Must install Flash Library using Arduino Library Manager to use this
#include <EEPROM.h>
#include "Patterns.h"

// chars are 8 bits long
typedef unsigned char byte;

int PATTERN_ADDRESS = 0; // this has to be an int because of the library for some reason...why tho

void StorePatternToROM( Pattern pattern )
{
    // EEPROM Implementation
    EEPROM.write( PATTERN_ADDRESS, (int)pattern );
    delay(10); // EEPROM write takes some time
}

Pattern ReadPatternFromROM()
{
    // EEPROM Implementation
    Pattern pattern = (Pattern)EEPROM.read( PATTERN_ADDRESS );
    
    if( pattern < Pattern::FIRST || pattern > Pattern::LAST ) // checking for bad reads from EEPROM
    {
        StorePatternToROM( Pattern::WIGWAG );
        return Pattern::WIGWAG;
    }

    return pattern;
}