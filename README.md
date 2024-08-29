# tosa-roller

A rolelist generator for Town of Salem: Anticipation

## Dependencies

- [CLIUtils/CLI11](https://github.com/CLIUtils/CLI11)
- [nlohmann/json](https://github.com/nlohmann/json)

## Compilation

### using CMake

```bash
cmake .
cmake --build .
```

### using Autotools

```
# If build files don't already exist
aclocal
autoconf
automake --add-missing

./configure
make
```

## Usage

```bash
./tosa-roller -d /path/to/data/file
# Example:
./tosa-roller -d data/roles.json
```

For more information on usage, run `./tosa-roller -h`.
