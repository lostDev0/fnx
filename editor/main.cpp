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
    auto aspect_ratio = 1.f;
    // Whatever the engine was able to create, save those for the next time
    {
        auto [win, _] = fnx::singleton<fnx::window>::acquire();
        fnx::world::save_display_configuration( "display.yaml", win.get_display_mode() );
        aspect_ratio = win.get_aspect_ratio();
        // TODO win.set_icon("icon.png", "");
    }

    {
        // Allow the event manager to execute once to ensure the systems initialize properly such as audio manager
        auto [events, _] = singleton<event_manager>::acquire();
        events.update( 0.f );
    }
    {
        auto [property_manager, _] = fnx::singleton<fnx::property_manager>::acquire();
        property_manager.set_property(fnx::PROPERTY_WORLD_BACKGROUND, fnx::vector4(.2,.2,.2,1));
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
        raise_land->set_outline_thickness(1);
        raise_land->set_outline_color(fnx::colors::cyan);
        auto teal_transparent = fnx::colors::rgba(fnx::colors::teal);
	    teal_transparent.w = 0.0;
        raise_land->set_gradient({ fnx::colors::teal , teal_transparent }, widget::fill_direction::left_to_right);
        raise_land->set_corner_radius(12);
        //fnx::slide_transition t{ fnx::vector2{-1,0}, fnx::vector2{0,0}, .4 };
        fnx::fade_transition t{ fnx::tween<float>{1.f, 0.f, 1.f}, .5 };
        fnx::transitions ts{t};
        //raise_land->get_animator().add_on_show_transitions(ts);
        raise_land->get_animator().add_on_mouse_enter_transitions(ts);
        //raise_land->set_constraints(fill_parent);
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
        auto ui_cam = fnx::make_shared_ref<fnx::ortho_camera>(aspect_ratio, -1.f, 1.f, -1.f, 1.f);
        ui_cam->set_position(0,0,0);
        ui_cam->look_at(fnx::vector3(0,0,0), fnx::vector3(0,1,0));
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