
# Alex's Macro

A program made in C with the use of the WindowsAPI, that records the keys and mouse clicks performed, and replays them back in the same timeframe as you pressed them.


## User guide

By default:
- the recording process is started using _*F2*_ 
- and playback is started by using _*F1*_. 
At the time of me writing this, these keybinds can be modified only if you do so directly in the code, and then recompile the project.


## Building

To build this project, make sure you have installed:

- make
- gcc

Then build the project using:

```bash
  make
```

And run it using

```bash
  make run
```


## Extra

At this point, the project uses SDL3 version 3.2.0 to open up a window. This window is not currently in use, but in the future I'll be working on a GUI interface for this program.

Also, there is currently no indicator for when the script is recording or playing your keybinds other than some logs in the console.


## Authors

- [Materniuc Alex](https://github.com/MaterniucAlex/)

