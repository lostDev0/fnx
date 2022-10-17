#include <GL/glew.h>
#include <GLFW/glfw3.h>

using namespace reactphysics3d;
namespace fnx
{
    namespace world
    {
        namespace detail
        {
            std::atomic<bool> _engine_running{true};
        }

        void init(const char* window_title)
        {
            { 
                auto [physicsCommon,_] = singleton<PhysicsCommon>::acquire();
                // Create a physics world
                auto* world = physicsCommon.createPhysicsWorld();
                auto* logger = physicsCommon.createDefaultLogger();
                auto logLevel = static_cast<uint>(static_cast<uint>(Logger::Level::Warning) | static_cast<uint>(Logger::Level::Error) | static_cast<uint>(Logger::Level::Information));
                logger->addStreamDestination(std::cout, logLevel, DefaultLogger::Format::Text);
                physicsCommon.setLogger(logger);
            }
            { 
                auto [events,_] = singleton<event_manager>::acquire();
                events.subscribe<fnx::window_close_evt>(fnx::bind(&fnx::world::stop));
            }
            { singleton<audio_manager>::acquire(); }
            {
                // initialize GLFW
                if (!glfwInit())
                {
                    FNX_ERROR("Unable to initialize window framework")
                    throw "unable to initialize window framework";
                }
                auto [win,_] = singleton<window>::acquire();
                const auto& modes = window::get_supported_display_modes();

                /*
                TODO : add configured display mode
                auto current_mode = window::get_resolution();
                bool found_current{ false };
                for (const auto& supported_mode : modes)
                {
                    if(supported_mode == current_mode)
                    {
                        mode = current_mode;
                        found_current = true;
                        break;
                    }
                }
                if (!found_current && modes.size() > 0)
                {
                    mode = modes[0];
                }
                */

                win.set_display_mode(modes[0]);
                if (win.create(window_title))
                {
                    win.set_fullscreen(modes[0]._fullscreen);
                }
            }
        }

        void run()
        {
            while(detail::_engine_running)
            {
                {
                    auto [win,_] = singleton<window>::acquire();
                    win.update();
                }
                {
                    auto [event,_] = singleton<event_manager>::acquire();
                    event.update(0);
                }
                {
                    auto [win,_] = singleton<window>::acquire();
                    win.swap();
                }
            }
        }

        bool stop(const window_close_evt&)
        {
            detail::_engine_running = false;
            auto [events,_] = singleton<event_manager>::acquire();
            events.unsubscribe<fnx::window_close_evt>(fnx::bind(&fnx::world::stop));
            return false;
        }

        void terminate()
        {
            glfwTerminate();
        }
    }
}