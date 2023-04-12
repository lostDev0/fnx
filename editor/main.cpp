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
        // TODO win.set_icon("icon.png", "");
    }

    {
        // Allow the event manager to execute once to ensure the systems initialize properly such as audio manager
        auto [events, _] = singleton<event_manager>::acquire();
        events.update( 0.f );
    }

    fnx::ui::init();
    //fnx::ui::parse_yaml_file( "user_interface.yaml" );
    {
        auto [layers, _] = fnx::singleton<fnx::layer_stack>::acquire();
        auto layer = fnx::make_shared_ref<fnx::layer>();
        //auto sizer = fnx::make_shared_ref<fnx::vert_sizer>();
        auto raise_land = fnx::make_shared_ref<fnx::button>( fnx::colors::blue, fnx::colors::red, fnx::colors::white,
                          fnx::colors::cyan, "raise_land" );
        auto lower_land = fnx::make_shared_ref<fnx::button>( fnx::colors::grey_0, fnx::colors::grey_1, fnx::colors::grey_2,
                          fnx::colors::grey_3, "lower_land" );
        raise_land->set_constraints( constraints( fnx::fill_horz_constraint( .1f ), fnx::fill_vert_constraint( .1f ),
                                     center_horz_constraint{}, center_vert_constraint{} ) );
        //sizer->set_constraints( fnx::constraints( fill_vert_constraint{.3f}, fill_horz_constraint{.1f} ) );
        //sizer->set_constraints( fill_parent );

        //auto font = fnx::make_shared_ref<fnx::font>( "Arial",
        //            fnx::singleton<fnx::asset_manager<fnx::texture>>::acquire().data );
        //auto raise_land_label = fnx::make_shared_ref<fnx::label>( font, "Raise" );
        //auto lower_land_label = fnx::make_shared_ref<fnx::label>( font, "Lower" );
        //raise_land->set_label( raise_land_label );
        //lower_land->set_label( lower_land_label );
        //sizer->add_widgets( raise_land );
        //layer->add_widget( sizer );
        //raise_land->set_constraints( fnx::fill_parent );
        //lower_land->set_constraints( fnx::fill_parent );

        layer->add_widget( raise_land );
        layer->show();
        layers.add_layer( layer );
    }

    {
        auto ui_cam = fnx::make_shared_ref<fnx::ortho_camera>();
        auto dflt_cam = fnx::make_shared_ref<fnx::perspective_camera>( fnx::angle( fnx::Degree( 70.0 ) ), 16.0 / 9.0, .1,
                        1000.0 );
        auto [cam_mgr, _] = singleton<camera_manager>::acquire();
        cam_mgr.add( ui_cam, camera_manager::ui );
        cam_mgr.add( dflt_cam, camera_manager::default );
    }

    fnx::world::run();
    fnx::world::terminate();

    return 0;
}