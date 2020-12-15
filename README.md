# Term CA
Cellular Automata on the terminal.

![Game of Life example](https://masflam.com/static/termca-gol.gif)

# Features
Simulation and visualization of:
* **Von Neumann cellular automaton**
* John Conway's **Game of Life**
* Brian Silverman's **Wireworld**
* Brian Silverman's **Seeds**

# Usage
To select which automaton to simulate and visualize, edit [`states.h`](states.h) and [`transition.inc`](transition.inc) to `#include` the appropriate files from the [`automata`](autmata/) folder.
Then (re)compile the executable with `make` and run it, giving it the input file(s) as command line argument(s). Example input can be found in [`examples`](examples/).

When you've opened the program, press `Enter` to continue to the next frame of the simulation. `Ctrl+C` will exit the program and `Ctrl+D` will make it go supersonic. (Accidental feature; you can still exit with `Ctrl+C`)

# Contributing
To contribute, you can write [example inputs](examples/), or add support for more [cellular automata](automata/). I'll ask you to use tabs for indentation and spaces for alignment, as well as not to make obnoxiously long lines.
