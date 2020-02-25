#pragma once

const int L1 = 1;
const int L2 = 2;
const int L3 = 3;
const int L4 = 4;

unsigned int red_pins[2] = {1,3};
unsigned int blue_pins[2] = {2,4};

unsigned int wigwag_pins_init[2] = { L2, L3 };
unsigned int xstrobe_pins_init[2] = { L1, L3 };
unsigned int upper_lower_pins_init[2] = { L1, L2 };

const unsigned int buttonPin = 6;
