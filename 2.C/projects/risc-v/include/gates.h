#pragma once

#include <stdlib.h>
#include <stdbool.h>

bool nand(bool a, bool b);
bool not_gate(bool a);
bool and_gate(bool a, bool b);
bool or_gate(bool a, bool b);
bool xor_gate(bool a, bool b);
bool mux_gate(bool a, bool b, bool sel);
void demux_gate(bool input, bool sel, bool *out0, bool *out1);
