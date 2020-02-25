#pragma once

const int light1 = 1;
const int light2 = 2;
const int light3 = 3;
const int light4 = 4;

unsigned int red_pins[2] = {1,3};
unsigned int blue_pins[2] = {2,4};

unsigned int wigwag_pins_init[2] = { light2, light3 };
unsigned int xstrobe_pins_init[2] = { light1, light3 };
unsigned int upper_lower_pins_init[2] = { light1, light2 };

const unsigned int buttonPin = 6;
