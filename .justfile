# Like GNU `make`, but `just` rustier.
# https://just.systems/
# run `just` from this directory to see available commands

alias b := build
alias r := run
alias c := clean
alias f := format
alias d := docs

# Default command when 'just' is run without arguments
default:
  @just --list

# Build the project
build *build_type='Release':
  @mkdir -p build
  @echo "Configuring the build system..."
  @cd build && cmake -S .. -B . -DCMAKE_BUILD_TYPE={{build_type}}
  @echo "Building the project..."
  @cd build && cmake --build . -j$(nproc)

# Run a package
run *package='full_adder':
  @./target/release/{{package}}

# Remove build artifacts and non-essential files
clean:
  @echo "Cleaning..."
  @rm -rf build
  @rm -rf target

# Format the project
format:
  @echo "Formatting..."
  @chmod +x ./scripts/format.sh
  @./scripts/format.sh format
  @cmake-format -i CMakeLists.txt
  @nixfmt .

# Generate documentation
docs:
  @echo "Generating documentation..."
