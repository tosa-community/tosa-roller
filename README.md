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

```bash
# If build files don't already exist
aclocal
autoconf
automake --add-missing

./configure
make
```

## Usage

1. Run the program with a link to the roles data file:

```bash
./tosa-roller -d /path/to/data/file
# Example:
./tosa-roller -d data/roles.json
```

2. Enter the role slots you would like to add to the list. Leave a line blank to continue to the next step. Alternatively, you can specify the role list from a flag using the `-f` option.
3. For every player, enter the scrolls they are using as a comma separated list. Prefix the roll with a `-` to enter as a cursed scroll. For example, to add a `Jailor` blessed scroll and a `Coroner` cursed scroll to a player (This stage is skipped when using the `-r` option):

```
1. jailor,-coroner
```

For more information on usage, see [Options](#options).

## Options

- `-b`/`--bans <text>`: Comma-separated list of roles that should be prevented from rolling, e. g. `-b conversus,sorcerer` will prevent `Conversus` and `Sorcerer` from rolling.
- `-d`/`--data <path>`: Path to the roles data JSON.
- `-f`/`--from-file <path>`: Path to the file to generate the rolelist from.
- `--no-color`: Display output without colour.
- `-o`/`--to-file <path>`: Path of the file to write the output to.
- `-r`/`--skip-scrolls`: Skip scrolls prompt.
- `-V`/`--verbose`: Show extra information in the output.
