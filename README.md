# Terraria basic clone in raylib and rlImgui

## Prerequisites
- CMake
- Raylib, Cmake will automatically install it if you don't have it locally
## How to run
This is a cmake project, so as always
```bash
git clone https://github.com/fantasyAlgo/TerrariaCloneRefactor.git
mkdir build
cmake ..
build
./TerrariaFake
```
## Features
- Lighting, this is made on another thread
- Block placing, you can place and collect blocks, including torches (there are not a lot really)
- Zombies at night

TODO:
- [x] Solve the lighting system
- [x] Finish the UI, rotate the tiles, add the empty one (e.g make a rect that points to nothing ), add hearts
- [ ] Style the trees and make the map less dead
- [ ] Add world saving
- [x] Make an inventory system
- [x] Add the possibility to zoom (i guess that's the whole point of this refactor)
- [x] Make a GUI
- [ ] World saving
- [x] Add other entities
- [ ] Add crafting
- [ ] Cool water

