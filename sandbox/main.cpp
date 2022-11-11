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
using namespace std;

// Main function
int main( int argc, char** argv )
{
    fnx::world::init();
    // Get any preconfigured display mode settings and attempt to restore the window using those values
    auto display = fnx::world::load_display_configuration( "display.yaml" );
    fnx::world::create_window( "fnx engine sandbox", display );
    // Whatever the engine was able to create, save those for the next time
    {
        auto [win, _] = fnx::singleton<fnx::window>::acquire();
        fnx::world::save_display_configuration( "display.yaml", win.get_display_mode() );
    }

    {
        // Allow the event manager to execute once to ensure the systems initialize properly such as audio manager
        auto [events, _] = singleton<event_manager>::acquire();
        events.update( 0.f );
    }

    // GOAL : Create a simple 3d fire with audio playing
    // 1. Get some audio (done)
    // 2. Create a window (done)
    // 3. Add ground
    // 4. Add logs
    // 5. Add fire emitter
    // 6. Add user interface
    // 7. Add user interface behavior scripting
    {
        auto [audio, _] = singleton<audio_manager>::acquire();
        audio.set_master_volume( 1.f, 1.f );
        audio.play_sound( "fire.wav" );
    }

    fnx::ui::init();
    fnx::ui::parse_yaml_file( "user_interface.yaml" );

    auto [layers, _1] = singleton<layer_stack>::acquire();
    for ( auto i = 0; i < 5; i++ )
    {
        auto layer = make_shared_ref<fnx::layer>();
        layers.add_layer( layer );
        layer->add_widget( create_widget<fnx::block>() );
    }

    fnx::world::run();
    fnx::world::terminate();

    std::string out;
    serializer<layer_stack>::to_yaml( out, layers );
    cout << out << endl;

    return 0;
}