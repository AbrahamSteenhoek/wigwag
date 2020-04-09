#pragma once

typedef uint64_t uint;
typedef unsigned long ulong;

const uint L_turn = 2;
const uint R_turn = 3;

const uint L1 = 8;
const uint L2 = 7;
const uint L3 = 6;
const uint L4 = 5;

uint xstrobe_pins_init[2] = { L1, L3 };
uint upper_lower_pins_init[2] = { L1, L2 };

const uint buttonPin = 4;
