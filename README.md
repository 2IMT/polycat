# polycat
runcat module for polybar (or waybar)

## Usage
- Install the font
```console
foo@bar:~$ git clone https://github.com/2IMT/polycat.git
foo@bar:~$ cd polycat/res
foo@bar:~$ cp runcat.ttf ~/.local/share/fonts/runcat.ttf
```
- Place the executable in **~/.config/polybar/modules** directory (or any other)
- Add the module to your polybar config:
```ini
[module/polycat]
type = custom/polycat
exec = ~/.config/polybar/modules/polycat
tail = true
```
- Place the module where you want
```ini
[bar/your-bar]
...
; modules-left = ... polycat ...
modules-right = ... polycat ...
...
```
Here you go

![polycat demo animation](res/polycat-demo.gif)

## Features
### Configuration
Polycat is configured with json file usually placed in the working directory of polycat (can be configured using command-line arguments)

#### Example
```json
{
    "frames": "",
    "lowRate": 3,
    "highRate": 60,
    "pollingPeriod": 500
}
```

- **frames** sets the frames to loop through
- **lowRate** sets the FPS minimum
- **highRate** sets the FPS maximum
- **pollingPeriod** sets the period of polling the CPU (stat file)

### Command-line arguments
By default, polycat uses /proc/stat file for CPU polling and polycat-config.json located in the working directory as a configuration file
Command-line arguments allow you to set the location of configuration file as well as the location of stat file
- **-c** or **--config-path** sets the path for configuration file
- **-s** or **--stat-path** sets the path for stat file

#### Example
```console
foo@bar:~$ ./polycat --config-path ./config-files/config.json --stat-path /proc/stat
```
In this example, the config file path would be **~/.config-files/config.json** and stat path would be **/proc/stat**

