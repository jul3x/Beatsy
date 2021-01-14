# Beatsy

![preview](/preview.gif "Preview of Beatsy")

Easily configurable, self-describing 3D music visualizer written in C++ and OpenGL.

-----

## Usage

To run visualization simply type `./Beatsy config/default.json`. Repositiory contains `test.wav` file which is used by *Beatsy*.
`config` directory consists of three files which show few types of visualizers. Every config file can be easily modified.

*Beatsy* does not emit any sounds by itself, so if you want to hear visualized music you can type:
```bash
./Beatsy config/default.json & ffplay test.wav
``` 

Test song: `VAREN - shameless`

-----

## Build

Package uses **C++17** language, so you need a compatible compiler. Tested on **g++9**.
    
* Dependencies:
    * OpenGL
    * GLEW
    * GLFW3
    * GLM

* Third-party libraries provided in repository:
    * Gist
    * kissfft
    * AudioFile
    * jsoncpp 
  

~jul3x