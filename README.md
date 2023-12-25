# polycat <a id="polycat"></a>

runcat module for polybar (or waybar)

## Table of Contents <a id="table-of-contents"></a>

- [Polycat](#polycat)
- [Table of Contents](#table-of-contents)
- [Installation](#installation)
  - [Arch-Based Distributions](#installation-arch-based)
  - [Other Distributions](#installation-other)
- [Features](#features)
  - [Configuration](#features-configuration)
  - [Command-line arguments](#features-arguments)
- [Building](#building)

## Installation <a id="installation"></a>

### Arch-Based Distributions <a id="installation-arch-based"></a>

Arch-Based users can directly install the
[AUR package](https://aur.archlinux.org/packages/polycat)
and follow 3-4 installation steps in
[Other Distributions](#installation-other) section.

### Other Distributions <a id="installation-other"></a>

1. Build polycat (see [Building](#building))

2. Install the font

```console
$ cd <cloned-polycat-repo>/res
$ cp polycat.ttf ~/.local/share/fonts/polycat.ttf
```

3. Add polycat module to your polybar config:

```ini
[module/polycat]
type = custom/script
exec = <path-to-polycat-executable>
tail = true
```

4. Place the module and add the font

```ini
[bar/your-bar]
...
; font-0 = ...
font-1 = "polycat"
...
; modules-left = ... polycat ...
modules-right = ... polycat ...
...
```

Here you go

![polycat demo animation](res/polycat-demo.gif)

## Features <a id="features"></a>

### Configuration <a id="features-configuration"></a>

Polycat is configured with json file placed in `$HOME/.config/polycat-config.json` (config path is specified using [command-line arguments](#features-arguments)).

If config file doesn't exist, polycat automatically creates a config file with default values.

#### Default configuration values

```json
{
  "frames": "",
  "highRate": 30,
  "lowRate": 2,
  "pollingPeriod": 1000,
  "smoothEnabled": true,
  "smoothValue": 2000,
  "sleepEnabled": true,
  "sleepThreshold": 8,
  "wakeupThreshold": 12,
  "sleepFrames": "",
  "sleepRate": 4,
  "formatEnabled": false,
  "format": "$frame $lcpu"
}
```

- `frames` (non-empty string)
  sets the frames to loop through.
- `lowRate` (number [1-255] inclusive)
  sets the FPS minimum.
- `highRate` (number [1-255] inclusive)
  sets the FPS maximum.
- `pollingPeriod` (number >1)
  sets the period of polling the CPU (stat file).
- `smoothEnabled` (boolean)
  enables smooth transition between CPU load values.
- `smoothValue` (num [1-10000] inclusive)
  number of milliseconds required to transition from 0% to 100%.
- `sleepEnabled` (boolean)
  enables sleeping.
- `sleepThreshold` (number [1-100] inclusive)
  sets the maximum CPU load needed to transition from active mode to sleeping mode.
- `wakeupThreshold` (number [1-100] inclusive, >sleepThreshold)
  sets the minimum CPU load value needed to transition from sleeping mode to active mode
- `sleepFrames` (non-empty string)
  sets the frames to loop through in sleeping mode.
- `sleepRate` (number [1-255] inclusive)
  sets the FPS for sleeping animation.
- `formatEnabled` (boolean)
  enables output formatting.
- `format` (string)
  sets the output format. $frame - animation, $lcpu - left-aligned CPU load value, $rcpu - right-aligned CPU load value.
  Note: always escape $ characters with $, otherwise an error will be thrown.

#### Sleeping

![polycat sleeping demo animation](res/polycat-sleeping-demo.gif)

#### Output formatting

![polycat formatting demo animation](res/polycat-formatting-demo.gif)

format string here: `"$rcpu $frame"`

### Command-line arguments <a id="features-arguments"></a>

By default, polycat uses `/proc/stat` file for CPU polling and `$HOME/.config/polycat-config.json` as a configuration file.
Command-line arguments allow you to set the location of configuration file as well as the location of stat file

- `-c` or `--config-path` sets the path for configuration file
- `-s` or `--stat-path` sets the path for stat file

#### Example

```console
$ ./polycat --config-path ./config-files/config.json --stat-path /proc/stat
```

In this example, the config file path would be **~/config-files/config.json** and stat path would be **/proc/stat**

## Building <a id="building"></a>

```console
$ git clone --recursive https://github.com/2IMT/polycat.git
$ cd polycat
$ cmake -DCMAKE_BUILD_TYPE=RELEASE .
$ cmake --build .
$ ./polycat # Run polycat
```
