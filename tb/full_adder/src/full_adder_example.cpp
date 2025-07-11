#include <iostream>
#include <systemc>
#include <kiwimodel/kiwimodel.hpp>

using namespace kiwimodel;

int sc_main(int argc, char* argv[]) {
  // Create signals
  sc_core::sc_signal<bool> a_sig, b_sig, cin_sig;
  sc_core::sc_signal<bool> sum_sig, cout_sig;

  // Create full adder instance
  full_adder fa_inst("full_adder_instance");

  // Connect signals to ports
  fa_inst.a(a_sig);
  fa_inst.b(b_sig);
  fa_inst.cin(cin_sig);
  fa_inst.sum(sum_sig);
  fa_inst.cout(cout_sig);

  // Test all possible input combinations
  std::cout << "Full Adder Truth Table:" << std::endl;
  std::cout << "A | B | Cin | Sum | Cout" << std::endl;
  std::cout << "--|---|-----|-----|-----" << std::endl;

  for (int a_val = 0; a_val <= 1; a_val++) {
    for (int b_val = 0; b_val <= 1; b_val++) {
      for (int cin_val = 0; cin_val <= 1; cin_val++) {
        // Set input values
        a_sig.write(a_val != 0);
        b_sig.write(b_val != 0);
        cin_sig.write(cin_val != 0);

        // Wait for propagation
        sc_core::sc_start(1, sc_core::SC_NS);

        // Read outputs
        bool sum_val = sum_sig.read() != 0;
        bool cout_val = cout_sig.read() != 0;

        // Display results
        std::cout << a_val << " | " << b_val << " | " << cin_val << "   | " << sum_val << "   | " << cout_val
                  << std::endl;
      }
    }
  }

  std::cout << "\nFull Adder example completed successfully!" << std::endl;
  return 0;
}
