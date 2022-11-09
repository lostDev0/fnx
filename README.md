# FNX C++ Game Engine
## Getting Started
### Dependencies
Perform a git clone on ReachPhysics3d and build the library for static inclusion.
Install yaml-cppd
```
$ cd ./dependencies
$ git submodule add https://github.com/jbeder/yaml-cpp.git ./yaml-cpp/
```
### Creating an Application
Basic application startup is fairly simple.
```cpp
// initialize the fnx engine with the highest available resolution
fnx::world::init();
// executing the main loop
fnx::world::run();
// loop has finished, perform cleanup
fnx::world::terminate();
```
Complex applicaiton startup allows for setting up windows with specific configuration files.
```cpp
fnx::world::init();
// Get any preconfigured display mode settings and attempt to restore the window using those values
auto display = fnx::world::load_display_configuration("display.cfg");
fnx::world::create_window("My Window Title", display);
// Whatever the engine was able to create, save those for the next time
auto [win, _1] = fnx::singleton<fnx::window>::acquire();
fnx::world::save_display_configuration("display.cfg", win.get_display_mode());
fnx::world::run();
fnx::world::terminate();
```
Notice the use of the singleton class. This is how to obtain global objects with the fnx engine.
## LICENSE
Copyright (c) <year> <copyright holders>

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
   claim that you wrote the original software. If you use this software
   in a product, an acknowledgment in the product documentation would be
   appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.