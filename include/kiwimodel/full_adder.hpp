#pragma once

#include <systemc.h>

namespace kiwimodel {

// Full Adder SystemC Module
SC_MODULE(full_adder) {
  // Input ports
  sc_in<bool> a; // First input bit
  sc_in<bool> b; // Second input bit
  sc_in<bool> cin; // Carry input

  // Output ports
  sc_out<bool> sum; // Sum output
  sc_out<bool> cout; // Carry output

  // Constructor
  SC_CTOR(full_adder) {
    // Register the process
    SC_METHOD(compute);
    sensitive << a << b << cin;
  }

  // Process declaration
  void compute() {
    // Full adder logic
    // Sum = A XOR B XOR Cin
    // Cout = (A AND B) OR (Cin AND (A XOR B))

    bool a_val = a.read();
    bool b_val = b.read();
    bool cin_val = cin.read();

    bool sum_val = a_val ^ b_val ^ cin_val;
    bool cout_val = (a_val & b_val) | (cin_val & (a_val ^ b_val));

    sum.write(sum_val);
    cout.write(cout_val);
  }
};

} // namespace kiwimodel
