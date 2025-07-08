{ pkgs ? import <nixpkgs> { } }:

pkgs.mkShell {
  buildInputs = with pkgs; [
    cmake # CMake build system
    cmake-format # CMake format tool
    nixfmt-classic # Nix formatter
    clang # C++ compiler
    just # Just runner
    systemc # SystemC library & headers
    pkg-config # Dependency discovery
  ];

  # Shell hook to set up environment
  shellHook = ''
    export SYSTEMC_HOME=${pkgs.systemc}
    echo "SystemC available at: $SYSTEMC_HOME"
  '';
}
