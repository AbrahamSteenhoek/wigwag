#pragma once

typedef uint64_t uint;
typedef unsigned long ulong;

const uint L_turn = 2;
const uint R_turn = 3;

// pins 5&7 are toast
const uint L1 = 8;
const uint L2 = 9;
const uint L3 = 4;
const uint L4 = 6;

uint xstrobe_pins_init[2] = { L1, L3 };
uint upper_lower_pins_init[2] = { L1, L2 };

const uint buttonPin = 11;
const uint strobe_switch_pin = 12;
