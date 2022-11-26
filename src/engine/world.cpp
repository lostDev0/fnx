#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <ostream>

using namespace reactphysics3d;
using namespace std;
using namespace std::chrono;

namespace fnx
{
namespace world
{
namespace detail
{
std::atomic<bool> _engine_running{true};
}

void init()
{
    {
        auto [physicsCommon, _] = singleton<PhysicsCommon>::acquire();
        // Create a physics world
        auto* world = physicsCommon.createPhysicsWorld();
        auto* logger = physicsCommon.createDefaultLogger();
        auto logLevel = static_cast<uint>( static_cast<uint>( Logger::Level::Warning ) | static_cast<uint>
                                           ( Logger::Level::Error ) | static_cast<uint>( Logger::Level::Information ) );
        logger->addStreamDestination( std::cout, logLevel, DefaultLogger::Format::Text );
        physicsCommon.setLogger( logger );
    }
}

void create_window( const char* window_title, fnx::display_mode& display )
{
    {
        auto [events, _] = singleton<event_manager>::acquire();
        events.subscribe<fnx::window_close_evt>( fnx::bind( &fnx::world::stop ) );
    }
    {
        singleton<audio_manager>::acquire();
    }
    {
        // initialize GLFW
        if ( !glfwInit() )
        {
            FNX_ERROR( "Unable to initialize window framework" );
            throw std::runtime_error( "unable to initialize window framework" );
        }
        auto [win, _] = singleton<window>::acquire();
        auto mode = display;

        // verify the gpu supports this display mode
        if ( !window::is_display_supported( mode ) )
        {
            FNX_WARN( "Display mode provided is not supported, loading maximum supported resolution" );
            // configuration is not supported, try a currently set level
            const auto& modes = window::get_supported_display_modes();
            auto current_mode = window::get_resolution();
            bool found_current{ false };
            for ( const auto& supported_mode : modes )
            {
                if ( supported_mode == current_mode )
                {
                    mode = current_mode;
                    found_current = true;
                    break;
                }
            }
            if ( !found_current && modes.size() > 0 )
            {
                mode = modes[0];
            }
        }

        win.set_display_mode( mode );
        if ( win.create( window_title ) )
        {
            win.set_fullscreen( mode._fullscreen );
        }
    }
}

void run()
{
    try
    {
        double fps;
        {
            // render engine initialization
            auto [renderer, _] = singleton<fnx::renderer>::acquire();
            renderer.init_depth_map();
            renderer.init_post_processing();
        }
        {
            // ensure that any systems relying on the window dimensions has accurate information
            auto [win, _] = singleton<window>::acquire();
            window_resize_evt e;
            e._x = win.get_pos_x();
            e._y = win.get_pos_y();
            e._width = win.width();
            e._height = win.height();
            FNX_EMIT( e );
            fps = 1.0 / static_cast<double>( win.get_display_mode()._refresh_rate );
        }

        high_resolution_clock::time_point start_time = high_resolution_clock::now();
        fnx::decimal delta = 0.0;
        fnx::decimal cycle_accumulator = 0.0;
        const double rolling_val = 0.9;
        constexpr auto max_samples = 16;
        auto num_samples = 0;
        auto current_sample = 0;
        fnx::decimal second_accumulator{ 0.0 };
        fnx::decimal fps_samples[max_samples] { 0.f };

        fnx::decimal fps_now{ 0.0 };
        fnx::decimal fps_avg{ 0.0 };
        fnx::decimal fps_min{ 0.0 };
        fnx::decimal fps_max{ 0.0 };

        while ( detail::_engine_running )
        {
            high_resolution_clock::time_point now = high_resolution_clock::now();
            delta = static_cast<fnx::decimal>( duration_cast<nanoseconds>( now - start_time ).count() ) / 1E9;
            start_time = now;
            cycle_accumulator += delta;
            second_accumulator += delta;	// for fps calculation

            {
                // process io events
                auto [win, _] = singleton<window>::acquire();
                win.update();
            }
            {
                // process any io events
                singleton<event_manager>::acquire().data.update( delta );
                // process systems
                FNX_EMIT_NOW( fnx::update_evt{fnx::update_evt::action_t::start, delta} );
                FNX_EMIT_NOW( fnx::update_evt{fnx::update_evt::action_t::end} );
            }
            if ( cycle_accumulator >= fps )
            {
                num_samples++;
                while ( second_accumulator > 1.0 )
                {
                    // calcuate average fps, min fps and max fps
                    // first current_fps should be legit, after that it's 0
                    auto current_fps = static_cast<fnx::decimal>( num_samples ) / second_accumulator;
                    fps_samples[current_sample++] = current_fps;
                    if ( current_sample == max_samples )
                    {
                        current_sample = 0;    // roll over
                    }
                    second_accumulator -= 1.0;	// remove a second
                    fnx::decimal avg = 0.0;
                    fnx::decimal min = 10000.0;
                    fnx::decimal max = -10000.0;
                    for ( auto i = 0; i < max_samples; i++ )
                    {
                        auto val = fps_samples[i];
                        avg += val;
                        if ( val < min )
                        {
                            min = val;
                        }
                        if ( val > max )
                        {
                            max = val;
                        }
                    }
                    fps_now = current_fps;
                    fps_avg = avg / max_samples;
                    fps_min = min;
                    fps_max = max;
                    num_samples = 0;
                }

                {
                    auto [properties, _1] = singleton<fnx::property_manager>::acquire();
                    auto [renderer, _2] = singleton<fnx::renderer>::acquire();
                    renderer.clear( properties.get_property<vector3>( fnx::PROPERTY_WORLD_BACKGROUND ) );
                }
                FNX_EMIT_NOW( fnx::render_evt{ render_evt::action_t::start, fps_now, fps_avg, fps_min, fps_max} );
                FNX_EMIT_NOW( fnx::render_evt{ render_evt::action_t::end } );
                FNX_EMIT_NOW( fnx::render_shadows_evt{ render_shadows_evt::action_t::start } );
                FNX_EMIT_NOW( fnx::render_shadows_evt{ render_shadows_evt::action_t::end } );
                FNX_EMIT_NOW( fnx::render_post_processing_evt{ render_post_processing_evt::action_t::start } );
                FNX_EMIT_NOW( fnx::render_post_processing_evt{ render_post_processing_evt::action_t::end } );
                FNX_EMIT_NOW( fnx::render_user_interface_evt{ render_user_interface_evt::action_t::start } );
                FNX_EMIT_NOW( fnx::render_user_interface_evt{ render_user_interface_evt::action_t::end } );
                auto [win, _] = singleton<window>::acquire();
                win.swap();
                cycle_accumulator = 0.0;
            }
        }
    }
    catch ( ... ) {}
}

bool stop( const window_close_evt& )
{
    detail::_engine_running = false;
    auto [events, _] = singleton<event_manager>::acquire();
    events.unsubscribe<fnx::window_close_evt>( fnx::bind( &fnx::world::stop ) );
    return false;
}

void terminate()
{
    auto [audio, _] = singleton<audio_manager>::acquire();
    audio.stop();
    glfwTerminate();
}

void save_display_configuration( const std::string& file_path, fnx::display_mode& mode )
{
    ofstream out( file_path );
    if ( !out.good() )
    {
        FNX_ERROR( fnx::format_string( "Unable to save display configuration file %s", file_path ) );
        return;
    }
    std::string str;
    serializer<display_mode>::to_yaml( str, mode );
    out << str;
}

fnx::display_mode load_display_configuration( const std::string& file_path )
{
    fnx::display_mode mode;
    ifstream in( file_path );
    if ( !in.good() )
    {
        FNX_ERROR( fnx::format_string( "Unable to load display configuration file %s", file_path ) );
        return mode;
    }
    ostringstream sout;
    copy( istreambuf_iterator<char>( in ), istreambuf_iterator<char>(), ostreambuf_iterator<char>( sout ) );
    serializer<display_mode>::from_yaml( sout.str(), mode );
    return mode;
}
}
}