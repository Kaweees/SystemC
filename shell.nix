{
  pkgs ? import <nixpkgs> { },
}:

pkgs.mkShell {
  buildInputs = with pkgs; [
    clang # C++ compiler
    systemc # SystemC library & headers
    pkg-config # Dependency discovery
    cmake # CMake build system
    cmake-format # CMake format tool
    nixfmt-classic # Nix formatter
    uv # Convert VCD to WaveDrom
    just # Just runner
  ];

  # Shell hook to set up environment
  shellHook = ''
    export SYSTEMC_HOME=${pkgs.systemc}
    echo "SystemC available at: $SYSTEMC_HOME"
  '';
}
