#if defined(_WIN32)
#include <windows.h>
#endif
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdint.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stbimage/stb_image.h"

struct key_map
{
    std::unordered_map<fnx::FNX_KEY, bool, fnx::FNX_KEY_HASH> _keyboard_state_map;
    auto& operator[](const fnx::FNX_KEY& key) { return _keyboard_state_map[key]; }
};

std::mutex _glfw_event_mutex;
GLFWwindow* _glfw_win{ nullptr };
double _cursor_position_x = 0.0;
double _cursor_position_y = 0.0;
key_map _key_map;

namespace fnx
{
    void opengl_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        std::lock_guard<std::mutex> guard(_glfw_event_mutex);

        if (action == GLFW_PRESS)
        {
            keyboard_press_evt e;
            e._key = static_cast<FNX_KEY>(key);
            _key_map[e._key] = true;
            // fire event within the active world
            FNX_EMIT(e);
        }
        else if (action == GLFW_RELEASE)
        {
            keyboard_release_evt e;
            e._key = static_cast<FNX_KEY>(key);
            _key_map[e._key] = false;
            FNX_EMIT(e);
        }
        else if (action == GLFW_REPEAT)
        {
            keyboard_repeat_evt e;
            e._key = static_cast<FNX_KEY>(key);
            FNX_EMIT(e);
        }
    }

    void opengl_error_callback(int error, const char* description)
    {
        FNX_WARN(std::string(__func__) + std::string(" ") + std::string(description));
    }

    void opengl_window_size_callback(GLFWwindow* window, int width, int height)
    {
        FNX_WARN(fnx::format_string("%s %d %d", __func__, width, height));
        std::lock_guard<std::mutex> guard(_glfw_event_mutex);

        /*
        const float aspectRatio = ((float)width) / height;
        float xSpan = 1;
        float ySpan = 1;

        if (aspectRatio > 1) {
            // Width > Height, so scale xSpan accordingly.
            //xSpan *= aspectRatio;
        }
        else {
            // Height >= Width, so scale ySpan accordingly.
            //ySpan = xSpan / aspectRatio;
        }

        glOrtho(-1 * xSpan, xSpan, -1 * ySpan, ySpan, -1, 1);

        // Use the entire window for rendering.
        glViewport(0, 0, width, height);
        */

        window_resize_evt e;
        e._width = width;
        e._height = height;
        glfwGetWindowPos(window, &e._x, &e._y);
        glViewport(0, 0, width, height);
        FNX_EMIT(e);
    }

    void opengl_window_pos_callback(GLFWwindow* window, int x, int y)
    {
        FNX_INFO(fnx::format_string("%s %d %d", __func__, x, y));

        std::lock_guard<std::mutex> guard(_glfw_event_mutex);
        window_move_evt e;
        e._x = x;
        e._y = y;
        glfwGetWindowSize(window, &e._width, &e._height);
        FNX_EMIT(e);
    }

    void opengl_window_focus_callback(GLFWwindow* window, int focused)
    {
        FNX_INFO(fnx::format_string("%s %d", __func__, focused));

        std::lock_guard<std::mutex> guard(_glfw_event_mutex);

        if (focused)
        {
            window_gain_focus_evt e;
            FNX_EMIT(e);
        }
        else
        {
            window_lose_focus_evt e;
            FNX_EMIT(e);
        }
    }

    void opengl_cursor_pos_callback(GLFWwindow* window, double x, double y)
    {
        auto pos = fnx::vector2();
        {
            auto [win,_] = singleton<fnx::window>::acquire();
            pos = win.screen_to_opengl(x,y);
        }
        std::lock_guard<std::mutex> guard(_glfw_event_mutex);

        _cursor_position_x = x;
        _cursor_position_y = y;

        mouse_move_evt e;
        e._x = x;
        e._y = y;
        e._gl_x = pos.x;
        e._gl_y = pos.y;
        FNX_EMIT(e);
    }

    void opengl_cursor_enter_callback(GLFWwindow* window, int entered)
    {
        FNX_INFO(fnx::format_string("%s %d", __func__, entered));

        std::lock_guard<std::mutex> guard(_glfw_event_mutex);

        if (entered)
        {
            mouse_enter_evt e;
            FNX_EMIT(e);
        }
        else
        {
            mouse_exit_evt e;
            FNX_EMIT(e);
        }
    }

    void opengl_mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
    {
        double x, y;
        glfwGetCursorPos(window, &x, &y);
        auto pos = fnx::vector2();
        {
            auto [win,_] = singleton<fnx::window>::acquire();
            pos = win.screen_to_opengl(x,y);
        }

        std::lock_guard<std::mutex> guard(_glfw_event_mutex);

        if (action == GLFW_PRESS)
        {
            mouse_press_evt e;
            e._btn = static_cast<FNX_BUTTON>(button);
            e._x = x;
            e._y = y;
            e._gl_x = pos.x;
            e._gl_y = pos.y;
            FNX_EMIT(e);
        }
        else
        {
            mouse_release_evt e;
            e._btn = static_cast<FNX_BUTTON>(button);
            e._x = x;
            e._y = y;
            e._gl_x = pos.x;
            e._gl_y = pos.y;
            FNX_EMIT(e);
        }
    }

    void opengl_mouse_scroll_callback(GLFWwindow* window, double x_offset, double y_offset)
    {
        std::lock_guard<std::mutex> guard(_glfw_event_mutex);
        mouse_scroll_evt e;
        e._x = x_offset;
        e._y = y_offset;
        FNX_EMIT(e);
    }

    void opengl_disable_cursor_window_limit()
    {
        FNX_INFO(fnx::format_string("%s", __func__));
        glfwSetInputMode(_glfw_win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }

    void window::show_default_cursor()
    {
        FNX_INFO(fnx::format_string("%s", __func__));
        glfwSetInputMode(_glfw_win, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }

    void window::hide_default_cursor()
    {
        FNX_INFO(fnx::format_string("%s", __func__));
        glfwSetInputMode(_glfw_win, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    }

    display_mode window::get_resolution()
    {
        display_mode mode;
        const GLFWvidmode* current= glfwGetVideoMode(glfwGetPrimaryMonitor());

        mode._width = current->width;
        mode._height = current->height;
        mode._refresh_rate = current->refreshRate;
        mode._depth = current->redBits + current->blueBits + current->greenBits;
        return mode;
    }

    std::vector<display_mode> window::get_supported_display_modes()
    {
        std::vector<display_mode> ret_modes;
        int count;
        GLFWmonitor* primary = glfwGetPrimaryMonitor();

        if (nullptr == primary)
        {
            FNX_ERROR("glfw unable to get primary monitor");
        }
        else
        {
            const GLFWvidmode* modes = glfwGetVideoModes(primary, &count);

            if (nullptr != modes && count > 0)
            {
                for (int i = count - 1; i >= 0; i--)
                {
                    display_mode dm;
                    dm._width = modes[i].width;
                    dm._height = modes[i].height;
                    dm._refresh_rate = modes[i].refreshRate;
                    dm._depth = modes[i].blueBits + modes[i].greenBits + modes[i].redBits;
                    ret_modes.emplace_back(dm);
                }
            }
            else
            {
                FNX_ERROR("glfw unable to get display modes");
            }
        }

        return ret_modes;
    }

    bool window::is_display_supported(const display_mode& mode)
    {
        const auto& modes = get_supported_display_modes();

        for (const auto& m : modes)
        {
            if (mode == m)
            {
                return true;
            }
        }

        return false;
    }

    void window::set_display_mode(const display_mode& display_mode)
    {
        _display_mode = display_mode;
    }

    void window::set_dimensions(int x, int y, int width, int height)
    {
        glfwSetWindowPos(_glfw_win, x, y);
        glfwSetWindowSize(_glfw_win, width, height);
        _dirty_cache.store(true);
        update_dimensions();
    }

    fnx::display_mode window::get_display_mode()
    {
        return _display_mode;
    }

    bool window::create(const std::string& title)
    {
        _title = title;

        // create our primary display window
        glfwWindowHint(GLFW_VISIBLE, GL_TRUE);
        glfwWindowHint(GLFW_SAMPLES, static_cast<int>(_display_mode._anti_alias)); // antialiasing
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL
        glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
        glfwWindowHint(GLFW_RED_BITS, _display_mode._depth / 3);
        glfwWindowHint(GLFW_GREEN_BITS, _display_mode._depth / 3);
        glfwWindowHint(GLFW_BLUE_BITS, _display_mode._depth / 3);
        glfwWindowHint(GLFW_REFRESH_RATE, _display_mode._refresh_rate);
        _glfw_win = glfwCreateWindow(_display_mode._width, _display_mode._height, title.c_str(), NULL, NULL);  // normal window
        //g_window = glfwCreateWindow( _display_mode._width, _display_mode._height, title.c_str(), glfwGetPrimaryMonitor(),
        //                             NULL ); // full screen

        if (nullptr == _glfw_win)
        {
            //glfwTerminate();
            return false;
        }
        else
        {
            // setup callbacks for mouse, keyboard, and window inputs
            glfwSetKeyCallback(_glfw_win, opengl_key_callback);               // detect keyboard events
            glfwSetWindowSizeCallback(_glfw_win, opengl_window_size_callback); // detect window resize
            glfwSetWindowPosCallback(_glfw_win, opengl_window_pos_callback);  // detect window move
            glfwSetWindowFocusCallback(_glfw_win, opengl_window_focus_callback); // detect focus change
            glfwSetCursorPosCallback(_glfw_win, opengl_cursor_pos_callback);  // detect cursor position change
            glfwSetCursorEnterCallback(_glfw_win, opengl_cursor_enter_callback); // detect cursor enter and exit
            glfwSetMouseButtonCallback(_glfw_win, opengl_mouse_button_callback); // detect mouse button press
            glfwSetScrollCallback(_glfw_win, opengl_mouse_scroll_callback);   // detect mouse scroll change
            // make the window's context current
            glfwMakeContextCurrent(_glfw_win);
            // start GLEW extension handler
            glewExperimental = GL_TRUE;
            glewInit();
            // get version info
            const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
            const GLubyte* version = glGetString(GL_VERSION); // version as a string
            if (_display_mode._anti_alias != display_mode::anti_alias::None)
            {
                // may be redundant as this is usually enabled by default but recommended
                glEnable(GL_MULTISAMPLE);
            }
            glEnable(GL_DEPTH_TEST);  // enable depth-testing
            glCullFace(GL_BACK);      // remove the back of objects
            glEnable(GL_CULL_FACE);   // don't render back face
            glDepthFunc(GL_LESS);     // depth-testing interprets a smaller value as "closer"
            glfwSwapInterval(0);      // disable vsync, 1 vsync
            GLFWmonitor* mon = glfwGetPrimaryMonitor();

            if (nullptr != mon)
            {
                const auto* video_mode = glfwGetVideoMode(mon);

                if (nullptr != video_mode)
                {
                    glfwGetMonitorPhysicalSize(mon, &_screen_dim_x, &_screen_dim_y);
                }
            }

            update_dimensions();
        }

        {
            void* hwnd = NULL;
#if defined(_WIN32)
    #ifndef _FNX_WINDOW
            hwnd = GetConsoleWindow();
    #else
            hwnd = GetActiveWindow();
    #endif
#endif
            window_init_evt e;
            e._id = hwnd;
            glfwGetWindowPos(_glfw_win, &e._x, &e._y);
            glfwGetWindowSize(_glfw_win, &e._width, &e._height);
            FNX_EMIT(e);
        }

        {
            window_gain_focus_evt e;
            FNX_EMIT(e);
        }

        // subscribe for window resize event
        {
            auto [events,_] = singleton<event_manager>::acquire();
            events.subscribe<window_resize_evt>(fnx::bind(*this, &window::on_window_resize));
        }

        return true;
    }

    bool window::on_window_resize(const window_resize_evt& evt)
    {
        _window_position_x = evt._x;
        _window_position_y = evt._y;
        _dimensions_x = evt._width;
        _dimensions_y = evt._height;
        _dirty_cache.store(true);
        return false;
    }

    void window::update()
    {
        glfwPollEvents(); // get all events from opengl window

        if (glfwWindowShouldClose(_glfw_win))
        {
            window_close_evt e;
            FNX_EMIT(e);
        }
    }

    void window::swap()
    {
        glfwSwapBuffers(_glfw_win);
    }

    void window::dpi(float& x, float& y)
    {
        x = _dpi_x;
        y = _dpi_y;
    }

    int window::get_pos_x()
    {
        if (_dirty_cache.load()) update_dimensions();
        return _window_position_x;
    }

    int window::get_pos_y()
    { 
        if (_dirty_cache.load()) update_dimensions();
        return _window_position_y;
    }

    void window::get_pos(int& x, int& y)
    {
        if (_dirty_cache.load()) update_dimensions();
        x = _window_position_x;
        y = _window_position_y;
    }

    int window::width()
    {
        if (_dirty_cache.load())
        {
            update_dimensions();
        }

        return _dimensions_x;
    }

    int window::height()
    {
        if (_dirty_cache.load())
        {
            update_dimensions();
        }

        return _dimensions_y;
    }

    int window::screen_width()
    {
        return _screen_dim_x;
    }

    int window::screen_height()
    {
        return _screen_dim_y;
    }

    float window::get_aspect_ratio()
    {
        return static_cast<float>(_dimensions_x) / static_cast<float>(_dimensions_y);
    }

    void window::set_fullscreen(bool enable)
    {
        if (enable)
        {
            // save the old position for restoration when the window is no longer full screen
            glfwGetWindowPos(_glfw_win, &_window_position_x, &_window_position_y);
            // TODO: Issue with the rate of cursor view deltas in full screen mode.
            // TODO: Issue with an empty border at top of screen in fullscreen mode.
            // TODO: Issue with text becoming distorted in fullscreen mode.
            glfwSetWindowMonitor(_glfw_win, glfwGetPrimaryMonitor(), 0, 0, _display_mode._width, _display_mode._height,
                _display_mode._refresh_rate);
            glViewport(0, 0, _display_mode._width, _display_mode._height);
        }
        else
        {
            glfwDefaultWindowHints();
            glfwWindowHint(GLFW_VISIBLE, GL_TRUE);
            glfwWindowHint(GLFW_SAMPLES, static_cast<int>(_display_mode._anti_alias)); // antialiasing
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL
            glfwWindowHint(GLFW_RED_BITS, _display_mode._depth / 3);
            glfwWindowHint(GLFW_GREEN_BITS, _display_mode._depth / 3);
            glfwWindowHint(GLFW_BLUE_BITS, _display_mode._depth / 3);
            glfwWindowHint(GLFW_REFRESH_RATE, _display_mode._refresh_rate);
            glfwSetWindowMonitor(_glfw_win, nullptr, _window_position_x, _window_position_y, _display_mode._width,
                _display_mode._height, _display_mode._refresh_rate);
            glfwGetWindowSize(_glfw_win, &_dimensions_x, &_dimensions_y);  // move the viewport below the title bar
            glViewport(0, 0, _dimensions_x, _dimensions_y);
        }

        update_dimensions();
    }

    void window::update_dimensions()
    {
        if (nullptr != _glfw_win)
        {
            glfwGetWindowSize(_glfw_win, &_dimensions_x, &_dimensions_y);
            GLFWmonitor* mon = glfwGetPrimaryMonitor();

            if (nullptr != mon)
            {
                int monitor_dim_x, monitor_dim_y;
                const auto* video_mode = glfwGetVideoMode(mon);

                if (nullptr != video_mode)
                {
                    glfwGetMonitorPhysicalSize(mon, &monitor_dim_x, &monitor_dim_y);
                    // take mm and convert to inch then take video resolution divided by that measurement to get pixels per inch in either direction
                    _dpi_x = static_cast<float>(video_mode->width) / (static_cast<float>(monitor_dim_x) * 0.0393701f);
                    _dpi_y = static_cast<float>(video_mode->height) / (static_cast<float>(monitor_dim_y) * 0.0393701f);
                }
                else
                {
                    FNX_ERROR("unable to get the default monitor's video mode");
                }
            }
            else
            {
                FNX_ERROR("unable to get the default monitor");
            }

            int x, y, w, h;
            glfwGetWindowPos(_glfw_win, &x, &y);
            glfwGetWindowSize(_glfw_win, &w, &h);

            _dimensions_x = w;
            _dimensions_y = h;
            _window_position_x = x;
            _window_position_y = y;
        
            // reset the viewport so the opengl coordinate system is realigned to the actual window dimensions
            glViewport(0, 0, _dimensions_x, _dimensions_y);

            FNX_INFO(fnx::format_string("%s %d %d %d %d", __func__, x, y, w, h));
        }	

        _dirty_cache.store(false);
    }

    window::window()
    {
        glfwSetErrorCallback(opengl_error_callback); // detect error events
    }

    window::~window()
    {
        if (nullptr != _glfw_win)
        {
            glfwDestroyWindow(_glfw_win);
        }
    }

    bool window::is_key_pressed(FNX_KEY keyboard_key)
    {
        if (FNX_KEY::FK_FIRST <= keyboard_key && FNX_KEY::FK_LAST >= keyboard_key)
        {
            return _key_map[keyboard_key];
        }
        else
            return false;
    }

    void window::get_cursor_position(double &pos_x, double &pos_y)
    {
        pos_x = _cursor_position_x;
        pos_y = _cursor_position_y;
    }

    fnx::vector2 window::screen_to_opengl(double screen_x, double screen_y)
    {
        auto x = (2.0 * screen_x / static_cast<double>(width())) - 1.0;
        auto y = -(2.0 * screen_y / static_cast<double>(height())) + 1.0;
        return fnx::vector2(x, y);
    }

    fnx::vector2 window::opengl_to_screen(double gl_x, double gl_y)
    {
        auto x = ((gl_x + 1.0) / 2.0) * static_cast<double>(width());
        auto y = ((gl_y - 1.0) / -2.0) * static_cast<double>(height());
        return fnx::vector2(x, y);
    }

    fnx::vector2 window::opengl_to_cartesian(double gl_x, double gl_y)
    {
        auto x = ((gl_x + 1.0) / 2.0) * static_cast<double>(width());
        auto y = ((-gl_y - 1.0) / -2.0) * static_cast<double>(height());
        return fnx::vector2(x, y);
    }

    void window::set_icon(const char* image, const char* thumbnail)
    {
        auto count = 0;
        GLFWimage images[2];
        if (image != nullptr)
        {
            images[0].pixels = stbi_load(image, &images[0].width, &images[0].height, 0, 4);
            count++;
        }
        if (image != nullptr && thumbnail != nullptr)
        {
            images[1].pixels = stbi_load(image, &images[1].width, &images[1].height, 0, 4);
            count++;
        }
        glfwSetWindowIcon(_glfw_win, count, images);

        stbi_image_free(images[0].pixels);
        stbi_image_free(images[1].pixels);
    }
}