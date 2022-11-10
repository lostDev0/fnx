#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <ostream>

using namespace reactphysics3d;
using namespace std;

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
            FNX_ERROR( "Unable to initialize window framework" )
            throw "unable to initialize window framework";
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
    while ( detail::_engine_running )
    {
        {
            auto [win, _] = singleton<window>::acquire();
            win.update();
        }
        {
            auto [event, _] = singleton<event_manager>::acquire();
            event.update( 0 );
        }
        {
            auto [win, _] = singleton<window>::acquire();
            win.swap();
        }
    }
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