# A demo featuring box2d and SDL

The demo allows you to take control of a small physics-based creature that can point it's arms at the location of the mouse.

Controls:
- **WASD** - movement 
- **LMB** - hold to drag objects around the scene. Also makes the controlled creature point at the mouse.

## Running the program

The game is built using C++ and CMake and follows a standart build process

```sh
git clone https://github.com/max7im-prog/GameWithSDL
cd ./GameWithSdl
mkdir build
cd ./build
cmake ..
cmake --build .
```
The game's binary is located in **/build/bin**

## Testing

The program uses google tests to facilitate testing. After the program is built, testing binary can be found as **build/bin/Game_tests**

## Documentation

The program has doxygen configured. In order to generate documentation, go to the project's root and run

```sh
doxygen ./docs/Doxyfile
```

The documentation will be generated in **/docs**
