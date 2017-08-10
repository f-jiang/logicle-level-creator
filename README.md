# Logicle Level Creator

This is a rewritten, improved version of the command-line program used to procedurally generate level packs for my mobile puzzle game [Logicle](https://play.google.com/store/apps/details?id=air.com.rampantmuffin.Logicle).
The program takes as input a JSON parameter file containing the level specifications, such as board dimensions and the number of colours to use,
and outputs a single JSON file containing the level pack and the levels' solutions. The level pack contents are sorted in order of increasing difficulty.

## Build and Dependencies

Place the following libraries' headers inside `lib/`:

- [Catch](https://github.com/philsquared/Catch) for unit testing
- [JSON for Modern C++](https://github.com/nlohmann/json)

Build with `make`. Alternatively, build tests with `make test`. Compilation confirmed to be working with gcc and g++ 5.4.0 and GNU Make 4.1.

## Usage

### Command usage

```
logicle-level-creator [-h] [-v] [-i indent-level] PARAM_FILES...
```

### JSON parameter file formatting

```
{
    "name": "example",
    "categories": [
        {
            "name": "example category",
            "groups": [
                {
                    "gameboard-height": 3,
                    "gameboard-width": 3,
                    "colors": ["0x111111", "0x222222"],
                    "color-dist": "uniform",    // "uniform" or "random"; default value of "uniform" if key left unspecified
                    "num-levels": 1000          // default value of 1 if key left unspecified
                }
            ]
        }
    ]
}
```

#### Alternate key names

- `"gameboard-height"`: `"height"`
- `"gameboard-width"`: `"width"`
- `"colors"`: `"colours"`
- `"color-dist"`: `"colour-dist"`, `"color-distribution"`, `"colour-distribution"`, `"cdist"`
- `"num-levels"`: `"n-levels"`, `"level-count"`

## Authors

- Feilan Jiang ([f-jiang](https://github.com/f-jiang))

