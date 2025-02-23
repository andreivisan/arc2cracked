#include "gates.h"

bool nand(bool a, bool b) {
    return !(a && b);
}

bool not_gate(bool a) {
    return nand(a, a);
}
bool and_gate(bool a, bool b) {
    return nand(nand(a, b), nand(a, b));
}

bool or_gate(bool a, bool b) {
    return not_gate(and_gate(not_gate(a), not_gate(b)));
}

bool xor_gate(bool a, bool b) {
    return or_gate(and_gate(a, not_gate(b)), and_gate(not_gate(a), b));
}

bool mux_gate(bool a, bool b, bool sel) {
    return or_gate(and_gate(a, not_gate(sel)), and_gate(b, sel));
}

void demux_gate(bool input, bool sel, bool *out0, bool *out1) {
    *out0 = and_gate(input, not_gate(sel));
    *out1 = and_gate(input, sel);
}
