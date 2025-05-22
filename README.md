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

## Installation <a id="installation"></a>

### Arch-Based Distributions <a id="installation-arch-based"></a>

Arch-Based users can directly install the
[AUR package](https://aur.archlinux.org/packages/polycat)
and follow 2-3 installation steps in
[Other Distributions](#installation-other) section.

### Other Distributions <a id="installation-other"></a>

1. Install polycat

```bash
sudo make clean install
```

2. Add polycat module to your polybar config:

```ini
[module/polycat]
type = custom/script
exec = <path-to-polycat-executable>
tail = true
```

3. Place the module and add the font

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

Polycat is configured with file placed in `$HOME/.config/polycat-config` (config path is specified using [command-line arguments](#features-arguments)).

If the config file doesn't exist, polycat uses the default one located at `/usr/local/share/polycat/polycat-config`.

The default configuration path can be different depending on install prefix used.

To create default config for manual installation:

```bash
mkdir -p $HOME/.config/
cp /usr/local/share/polycat/polycat-config $HOME/.config/polycat-config
```

To create default config for package manager installation:

```bash
mkdir -p $HOME/.config/
cp /usr/share/polycat/polycat-config $HOME/.config/polycat-config
```

#### Default config

```ini
frames = ""
high_rate = 30
low_rate = 2
poll_period = 1000
smoothing_enabled = true
smoothing_value = 2000
sleeping_enabled = true
sleeping_threshold = 8
wakeup_threshold = 12
sleeping_frames = ""
sleeping_rate = 4
format_enabled = false
format = "$frame $lcpu"
```

- `frames` (non-empty string)
  sets the frames to loop through.
- `high_rate` (integer [1-255] inclusive)
  sets the FPS maximum.
- `low_rate` (integer [1-255] inclusive)
  sets the FPS minimum.
- `poll_period` (integer >1)
  sets the period of polling the CPU (stat file).
- `smoothing_enabled` (boolean)
  enables smooth transition between CPU load values.
- `smoothing_value` (integer [1-10000] inclusive)
  number of milliseconds required to transition from 0% to 100%.
- `sleeping_enabled` (boolean)
  enables sleeping.
- `sleeping_threshold` (integer [1-100] inclusive)
  sets the maximum CPU load needed to transition from active mode to sleeping mode.
- `wakeup_threshold` (integer [1-100] inclusive, >sleeping_threshold)
  sets the minimum CPU load value needed to transition from sleeping mode to active mode
- `sleeping_frames` (non-empty string)
  sets the frames to loop through in sleeping mode.
- `sleeping_rate` (integer [1-255] inclusive)
  sets the FPS for sleeping animation.
- `format_enabled` (boolean)
  enables output formatting.
- `format` (string)
  sets the output format. `$frame` - animation, `$lcpu` - left-aligned CPU load value, `$rcpu` - right-aligned CPU load value.
  Note: always escape `$` characters with `$` (like this: `$$`), otherwise an error will be thrown.

#### Sleeping

![polycat sleeping demo animation](res/polycat-sleeping-demo.gif)

#### Output formatting

![polycat formatting demo animation](res/polycat-formatting-demo.gif)

format string here: `"$rcpu $frame"`

### Command-line arguments <a id="features-arguments"></a>

By default, polycat uses `/proc/stat` file for CPU polling and `/usr/share/polycat/polycat-config` as a configuration file if `$HOME/.config/polycat-config` does not exist.
Command-line arguments allow you to set the location of configuration file as well as the location of stat file

- `-c` or `--config-path` sets the path for configuration file
- `-s` or `--stat-path` sets the path for stat file

#### Example

```bash
./polycat --config-path ./config-files/config --stat-path /proc/stat
```

In this example, the config file path would be **~/config-files/config** and stat path would be **/proc/stat**
