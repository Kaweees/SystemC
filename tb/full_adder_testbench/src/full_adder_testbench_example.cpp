#include <systemc>
#include <kiwimodel/kiwimodel.hpp>

using namespace kiwimodel;

int sc_main(int argc, char* argv[]) {
  std::cout << "Full Adder Testbench Example" << std::endl;
  std::cout << "============================" << std::endl;

  // Create the testbench
  full_adder_testbench tb("TESTBENCH");

  // Enable VCD tracing (optional)
  sc_core::sc_trace_file* tf = sc_core::sc_create_vcd_trace_file("full_adder_traces");
  if (tf) {
    sc_core::sc_trace(tf, tb.clk_sig, "clk");
    sc_core::sc_trace(tf, tb.a_sig, "a");
    sc_core::sc_trace(tf, tb.b_sig, "b");
    sc_core::sc_trace(tf, tb.cin_sig, "cin");
    sc_core::sc_trace(tf, tb.sum_sig, "sum");
    sc_core::sc_trace(tf, tb.cout_sig, "cout");
    sc_core::sc_trace(tf, tb.test_complete_sig, "test_complete");
  }

  // Run the simulation
  sc_core::sc_start();

  // Close trace file
  if (tf) {
    sc_core::sc_close_vcd_trace_file(tf);
    std::cout << std::endl << "VCD trace file 'full_adder_traces.vcd' generated." << std::endl;
  }

  return 0;
}
