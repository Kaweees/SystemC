# Like GNU `make`, but `just` rustier.
# https://just.systems/
# run `just` from this directory to see available commands

alias b := build
alias r := run
alias c := clean
alias ch := check
alias f := format
alias d := docs

# Default command when 'just' is run without arguments
default:
  @just --list

# Get the number of cores
CORES := if os() == "macos" { `sysctl -n hw.ncpu` } else if os() == "linux" { `nproc` } else { "1" }

# Build the project
build *build_type='Release':
  @mkdir -p build
  @echo "Configuring the build system..."
  @cd build && cmake -S .. -B . -DCMAKE_BUILD_TYPE={{build_type}} -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
  @echo "Building the project..."
  @cd build && cmake --build . -j{{CORES}}

# Run a package
run package='full_adder_testbench' *args='':
  @./target/release/{{package}} {{args}}
  @uvx --from git+https://github.com/Toroid-io/vcd2wavedrom python -m vcd2wavedrom.vcd2wavedrom -i {{package}}.vcd -o {{package}}.json

# Remove build artifacts and non-essential files
clean:
  @echo "Cleaning..."
  @find . -type d -name "build" -exec rm -rf {} +
  @find . -type d -name "target" -exec rm -rf {} +

# Run code quality tools
check:
  @echo "Running code quality tools..."
  @cppcheck --error-exitcode=1 --project=build/compile_commands.json -i build/_deps/

# Format the project
format:
  @echo "Formatting..."
  @chmod +x ./scripts/format.sh
  @./scripts/format.sh format
  @cmake-format -i $(find . -name "CMakeLists.txt")
  @find . -name "*.nix" -type f -exec nixfmt {} \;

# Generate documentation
docs:
  @echo "Generating documentation..."
