/*
 * This is a "sandbox" project with a basic example of how to use
 * the fnx engine in your project. 
 */

// Libraries
#include "fnx/fnx.hpp"
#include <iostream>

using namespace reactphysics3d;
using namespace fnx;
using namespace std::chrono;

// Main function
int main(int argc, char** argv) {

    // First you need to create the PhysicsCommon object. This is a factory module
    // that you can use to create physics world and other objects. It is also responsible
    // for logging and memory management
    PhysicsWorld* world{nullptr};
    DefaultLogger* logger{nullptr};

    fnx::world::init("fnx engine sandbox");

    {
        // Allow the event manager to execute once to ensure the systems initialize properly such as audio manager
        auto [events,_3] = singleton<event_manager>::acquire();
        events.update(0.f);
    }

    // GOAL : Create a simple 3d fire with audio playing
    // 1. Get some audio
    {
        auto [audio,_] = singleton<audio_manager>::acquire();
        audio.set_master_volume(1.f,1.f);
        audio.play_sound("fire.wav");
    }

    fnx::world::run();

    return 0;
}