#pragma once

#include <systemc>
#include <iomanip>
#include <vector>
#include <random>
#include <kiwimodel/full_adder.hpp>

namespace kiwimodel {

// Test Vector Structure
struct test_vector {
    bool a, b, cin;
    bool expected_sum, expected_cout;

    test_vector(bool _a, bool _b, bool _cin, bool _sum, bool _cout)
      : a(_a),
        b(_b),
        cin(_cin),
        expected_sum(_sum),
        expected_cout(_cout) {}
};

// Stimulus Generator Module
SC_MODULE(stimulus_gen) {
  // Output ports to drive DUT
  sc_core::sc_out<bool> a_out;
  sc_core::sc_out<bool> b_out;
  sc_core::sc_out<bool> cin_out;

  // Control signals
  sc_core::sc_out<bool> test_complete;

  // Clock
  sc_core::sc_in<bool> clk;

  // Test vectors
  std::vector<test_vector> test_vectors;
  unsigned int vector_index;

  // Stimulus generation process
  void stimulus_process();

  // Constructor
  SC_CTOR(stimulus_gen)
    : vector_index(0) {
    // Initialize test vectors with all combinations
    init_test_vectors();

    SC_THREAD(stimulus_process);
    sensitive << clk.pos();
  }
private:
  void init_test_vectors();
  void add_random_vectors(int count);
};

// Monitor Module
SC_MODULE(monitor) {
  // Input ports to monitor DUT
  sc_core::sc_in<bool> a_in;
  sc_core::sc_in<bool> b_in;
  sc_core::sc_in<bool> cin_in;
  sc_core::sc_in<bool> sum_in;
  sc_core::sc_in<bool> cout_in;

  // Control signals
  sc_core::sc_in<bool> test_complete;

  // Clock
  sc_core::sc_in<bool> clk;

  // Statistics
  unsigned int total_tests;
  unsigned int passed_tests;
  unsigned int failed_tests;

  // Monitoring process
  void monitor_process();

  // Constructor
  SC_CTOR(monitor)
    : total_tests(0),
      passed_tests(0),
      failed_tests(0) {
    SC_THREAD(monitor_process);
    sensitive << clk.pos();
  }
private:
  bool verify_result(bool a, bool b, bool cin, bool sum, bool cout);
  void print_header();
  void print_result(bool a, bool b, bool cin, bool sum, bool cout, bool expected_sum, bool expected_cout, bool pass);
  void print_summary();
};

// Clock Generator Module
SC_MODULE(clock_gen) {
  sc_core::sc_out<bool> clk_out;

  void clock_process();

  SC_CTOR(clock_gen) { SC_THREAD(clock_process); }
};

// Top-level Testbench Module
SC_MODULE(full_adder_testbench) {
  // Clock signal
  sc_core::sc_signal<bool> clk_sig;

  // DUT signals
  sc_core::sc_signal<bool> a_sig, b_sig, cin_sig;
  sc_core::sc_signal<bool> sum_sig, cout_sig;

  // Control signals
  sc_core::sc_signal<bool> test_complete_sig;

  // Module instances
  full_adder* dut;
  stimulus_gen* stim;
  monitor* mon;
  clock_gen* clk_gen;

  // Constructor
  SC_CTOR(full_adder_testbench) {
    // Create module instances
    dut = new full_adder("DUT");
    stim = new stimulus_gen("STIMULUS");
    mon = new monitor("MONITOR");
    clk_gen = new clock_gen("CLOCK_GEN");

    // Connect DUT
    dut->a(a_sig);
    dut->b(b_sig);
    dut->cin(cin_sig);
    dut->sum(sum_sig);
    dut->cout(cout_sig);

    // Connect stimulus generator
    stim->a_out(a_sig);
    stim->b_out(b_sig);
    stim->cin_out(cin_sig);
    stim->test_complete(test_complete_sig);
    stim->clk(clk_sig);

    // Connect monitor
    mon->a_in(a_sig);
    mon->b_in(b_sig);
    mon->cin_in(cin_sig);
    mon->sum_in(sum_sig);
    mon->cout_in(cout_sig);
    mon->test_complete(test_complete_sig);
    mon->clk(clk_sig);

    // Connect clock generator
    clk_gen->clk_out(clk_sig);
  }

  // Destructor
  ~full_adder_testbench() {
    delete dut;
    delete stim;
    delete mon;
    delete clk_gen;
  }
};

// Implementation of stimulus_gen methods
void stimulus_gen::init_test_vectors() {
  // Add all 8 possible combinations
  test_vectors.push_back(test_vector(0, 0, 0, 0, 0)); // 0 + 0 + 0 = 0, carry = 0
  test_vectors.push_back(test_vector(0, 0, 1, 1, 0)); // 0 + 0 + 1 = 1, carry = 0
  test_vectors.push_back(test_vector(0, 1, 0, 1, 0)); // 0 + 1 + 0 = 1, carry = 0
  test_vectors.push_back(test_vector(0, 1, 1, 0, 1)); // 0 + 1 + 1 = 0, carry = 1
  test_vectors.push_back(test_vector(1, 0, 0, 1, 0)); // 1 + 0 + 0 = 1, carry = 0
  test_vectors.push_back(test_vector(1, 0, 1, 0, 1)); // 1 + 0 + 1 = 0, carry = 1
  test_vectors.push_back(test_vector(1, 1, 0, 0, 1)); // 1 + 1 + 0 = 0, carry = 1
  test_vectors.push_back(test_vector(1, 1, 1, 1, 1)); // 1 + 1 + 1 = 1, carry = 1

  // Add some random test vectors for robustness
  add_random_vectors(10);
}

void stimulus_gen::add_random_vectors(int count) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(0, 1);

  for (int i = 0; i < count; i++) {
    bool a = dis(gen);
    bool b = dis(gen);
    bool cin = dis(gen);

    // Calculate expected results
    bool expected_sum = a ^ b ^ cin;
    bool expected_cout = (a & b) | (cin & (a ^ b));

    test_vectors.push_back(test_vector(a, b, cin, expected_sum, expected_cout));
  }
}

void stimulus_gen::stimulus_process() {
  // Wait for initial settling
  wait(1, sc_core::SC_NS);

  std::cout << "=== Full Adder Testbench Started ===" << std::endl;

  // Apply all test vectors
  for (vector_index = 0; vector_index < test_vectors.size(); vector_index++) {
    const test_vector& tv = test_vectors[vector_index];

    // Apply inputs
    a_out.write(tv.a);
    b_out.write(tv.b);
    cin_out.write(tv.cin);

    // Wait for propagation
    wait(2, sc_core::SC_NS);
  }

  // Signal test completion
  test_complete.write(true);
  wait(5, sc_core::SC_NS);

  std::cout << "=== Full Adder Testbench Completed ===" << std::endl;
  sc_core::sc_stop();
}

// Implementation of monitor methods
void monitor::monitor_process() {
  print_header();

  while (!test_complete.read()) {
    wait();

    if (!test_complete.read()) {
      // Wait a bit for DUT to settle after inputs change
      wait(1, sc_core::SC_NS);

      bool a = a_in.read();
      bool b = b_in.read();
      bool cin = cin_in.read();
      bool sum = sum_in.read();
      bool cout = cout_in.read();

      // Calculate expected results
      bool expected_sum = a ^ b ^ cin;
      bool expected_cout = (a & b) | (cin & (a ^ b));

      // Verify result
      bool pass = verify_result(a, b, cin, sum, cout);

      // Print result
      print_result(a, b, cin, sum, cout, expected_sum, expected_cout, pass);

      // Update statistics
      total_tests++;
      if (pass) {
        passed_tests++;
      } else {
        failed_tests++;
      }
    }
  }

  print_summary();
}

bool monitor::verify_result(bool a, bool b, bool cin, bool sum, bool cout) {
  bool expected_sum = a ^ b ^ cin;
  bool expected_cout = (a & b) | (cin & (a ^ b));

  return (sum == expected_sum) && (cout == expected_cout);
}

void monitor::print_header() {
  std::cout << std::endl;
  std::cout << "Time(ns) | A | B | Cin | Sum | Cout | Expected Sum | Expected Cout | Result" << std::endl;
  std::cout << "---------|---|---|-----|-----|------|--------------|---------------|-------" << std::endl;
}

void monitor::print_result(bool a, bool b, bool cin, bool sum, bool cout, bool expected_sum, bool expected_cout,
                           bool pass) {
  std::cout << std::setw(8) << sc_core::sc_time_stamp() << " | " << a << " | " << b << " | " << cin << "   | " << sum
            << "   | " << cout << "    | " << expected_sum << "            | " << expected_cout << "             | "
            << (pass ? "PASS" : "FAIL") << std::endl;
}

void monitor::print_summary() {
  std::cout << std::endl;
  std::cout << "=== Test Summary ===" << std::endl;
  std::cout << "Total Tests:  " << total_tests << std::endl;
  std::cout << "Passed Tests: " << passed_tests << std::endl;
  std::cout << "Failed Tests: " << failed_tests << std::endl;
  std::cout << "Success Rate: " << std::fixed << std::setprecision(2)
            << (total_tests > 0 ? (double)passed_tests / total_tests * 100.0 : 0.0) << "%" << std::endl;

  if (failed_tests == 0) {
    std::cout << "✓ All tests PASSED!" << std::endl;
  } else {
    std::cout << "✗ Some tests FAILED!" << std::endl;
  }
}

// Implementation of clock_gen methods
void clock_gen::clock_process() {
  while (true) {
    clk_out.write(false);
    wait(5, sc_core::SC_NS);
    clk_out.write(true);
    wait(5, sc_core::SC_NS);
  }
}

} // namespace kiwimodel
