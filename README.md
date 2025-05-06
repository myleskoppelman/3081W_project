# Newton's Simulator
 >A 2D gravity simulator based on Newtonian physics

Newton’s Simulator is a real-time interactive physics simulation engine that visualizes gravitational interactions between celestial bodies in a two-dimensional plane. Designed for performance and clarity, it features object creation, collision modeling, data I/O, and real-time graphics using OpenGL.

Installation Instructions
Dependencies:

- C++17
- OpenGL
- GLFW
- GLM
- makefile

Installation Guide:

```
git clone https://github.com/yourusername/newtons-simulator.git
```


How to Run:
```
make
./newtons_simulator
```

Reinstall:
```
make clean
make
./newtons_simulator
```

Use GUI buttons to start, pause, reset, speed up, slow down, show commands, or show stats.

Use keyboard spawn and load objects using commands.
Commands:
- 'load [filename]': Load new object file. File must be a .csv file formatted like { mass(base), mass(power), radius, vx, vy, x, y }
- 'clear': Clear objects from screen
- 'spawn [mass(base),mass(power),radius,vx,vy,x,y]': Spawns new object
- 'quit': Quit program 

