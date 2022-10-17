#pragma once

#include <string>

namespace fnx
{
    struct sound_evt
    {
        enum type
        {
            invalid,
            play,
            loop,
            pause,
            stop,
            volume,
            master_volume
        };

        sound_evt::type _type{ sound_evt::type::invalid };
        float _left_volume{ 1.f };
        float _right_volume{ 1.f };
        std::string _resource_name;
    };

    template<>
    inline std::string to_string(const sound_evt& evt)
    {
        switch(evt._type)
        {
            case sound_evt::type::play:
                return std::string("play ") + evt._resource_name;
            case sound_evt::type::loop:
                return std::string("loop ") + evt._resource_name;
            case sound_evt::type::pause:
                return std::string("pause ") + evt._resource_name;
            case sound_evt::type::stop:
                return std::string("stop ") + evt._resource_name;
            case sound_evt::type::volume:
                return std::string("volume ") + std::to_string(evt._left_volume) + std::string(" ") + std::to_string(evt._right_volume);
            case sound_evt::type::master_volume:
                return std::string("master volume ") + std::to_string(evt._left_volume) + std::string(" ") + std::to_string(evt._right_volume);
            case sound_evt::type::invalid:
            default:
                return std::string("invalid ") + evt._resource_name;
        }
    }

    struct window_close_evt {};

    template<>
    inline std::string to_string(const window_close_evt& evt)
    {
        return "window closed";
    }

    struct window_resize_evt
    { 
        int _x{ 0 }; 
        int _y{ 0 }; 
        int _width{ 0 }; 
        int _height{ 0 };
    };

    template<>
    inline std::string to_string(const window_resize_evt& evt)
    {
        return fnx::format_string("window resize %d, %d, %d, %d", evt._x, evt._y, evt._width, evt._height);
    }

    struct window_move_evt
    { 
        int _x{ 0 }; 
        int _y{ 0 }; 
        int _width{ 0 }; 
        int _height{ 0 };
    };

    template<>
    inline std::string to_string(const window_move_evt& evt)
    {
        return fnx::format_string("window move %d, %d, %d, %d", evt._x, evt._y, evt._width, evt._height);
    }

    struct window_minimize_evt {};
    
    template<>
    inline std::string to_string(const window_minimize_evt& evt)
    {
        return "window minimized";
    }

    struct window_maximize_evt {};

    template<>
    inline std::string to_string(const window_maximize_evt& evt)
    {
        return "window maximized";
    }

    struct window_fullscreen_evt {};
    template<>
    inline std::string to_string(const window_fullscreen_evt& evt)
    {
        return "window fullscreen";
    }

    struct window_lose_focus_evt {};

    template<>
    inline std::string to_string(const window_lose_focus_evt& evt)
    {
        return "window lost focus";
    }

    struct window_gain_focus_evt {};

    template<>
    inline std::string to_string(const window_gain_focus_evt& evt)
    {
        return "window gained focus";
    }

    struct window_init_evt {
        void* _id;	/// used for any threads that need to know the active window
        int _x{ 0 }; int _y{ 0 }; int _width{ 0 }; int _height{ 0 };
    };

    template<>
    inline std::string to_string(const window_init_evt& evt)
    {
        return std::string("window initialized ") + std::to_string(evt._width) + std::string(" ") + std::to_string(evt._height);
    }

    struct keyboard_press_evt { FNX_KEY _key{ FNX_KEY::FK_UNKNOWN }; };

    template<>
    inline std::string to_string(const keyboard_press_evt& evt)
    {
        return fnx::format_string("%c", key_to_ascii(evt._key));
    }

    struct keyboard_release_evt { FNX_KEY _key{ FNX_KEY::FK_UNKNOWN }; };

    template<>
    inline std::string to_string(const keyboard_release_evt& evt)
    {
        return fnx::format_string("%c", key_to_ascii(evt._key));
    }

    struct keyboard_repeat_evt { FNX_KEY _key{ FNX_KEY::FK_UNKNOWN }; };

    template<>
    inline std::string to_string(const keyboard_repeat_evt& evt)
    {
        return fnx::format_string("%c", key_to_ascii(evt._key));
    }

    struct mouse_enter_evt {};

    template<>
    inline std::string to_string(const mouse_enter_evt& evt)
    {
        return "mouse entered window";
    }
    
    struct mouse_exit_evt {};

    template<>
    inline std::string to_string(const mouse_exit_evt& evt)
    {
        return "mouse exited window";
    }

    struct mouse_move_evt { double _x{ 0.0 }; double _y{ 0.0 }; double _gl_x{ 0.0 }; double _gl_y{ 0.0 }; };

    template<>
    inline std::string to_string(const mouse_move_evt& evt)
    {
        return fnx::format_string("mouse moved %f %f %f %f", evt._x, evt._y, evt._gl_x, evt._gl_y);
    }

    struct mouse_press_evt { FNX_BUTTON _btn{ FNX_BUTTON::BT_UNKNOWN }; double _x{ 0.0 }; double _y{ 0.0 }; double _gl_x{ 0.0 }; double _gl_y{ 0.0 }; };

    template<>
    inline std::string to_string(const mouse_press_evt& evt)
    {
        return fnx::format_string("mouse pressed %f %f %f %f", evt._x, evt._y, evt._gl_x, evt._gl_y);
    }

    struct mouse_release_evt { FNX_BUTTON _btn{ FNX_BUTTON::BT_UNKNOWN }; double _x{ 0.0 }; double _y{ 0.0 }; double _gl_x{ 0.0 }; double _gl_y{ 0.0 }; };

    template<>
    inline std::string to_string(const mouse_release_evt& evt)
    {
        return fnx::format_string("mouse released %f %f %f %f", evt._x, evt._y, evt._gl_x, evt._gl_y);
    }

    struct mouse_scroll_evt { double _x{ 0.0 }; double _y{ 0.0 }; };

    template<>
    inline std::string to_string(const mouse_scroll_evt& evt)
    {
        return fnx::format_string("mouse scrolled %f %f", evt._x, evt._y);
    }

    struct widget_active_evt { unsigned int _src; };

    template<>
    inline std::string to_string(const widget_active_evt& evt)
    {
        return fnx::format_string("widget activated %d", evt._src);
    }

    struct widget_inactive_evt { unsigned int _src; };
    struct widget_press_evt { unsigned int _src; };
    struct widget_release_evt { unsigned int _src; };
    struct widget_progress_evt { unsigned int _src; float _progress{ 0.f }; };

    struct text_submit_evt { std::string _text; std::string _src; };
    struct text_update_evt { std::string _text; std::string _src; };

    /// @brief Update all systems.
    struct update_evt
    {
        enum class action_t
        {
            start,
            end
        };
        action_t _action{ action_t::start };
        double _delta_in_seconds{ 0.0 };
    };

    /// @brief Render game objects.
    struct render_evt
    { 
        enum class action_t
        {
            start,
            end
        };
        action_t _action{ action_t::start };
        double _fps_now{ 0.0 };  double _fps_avg{ 0.0 }; double _fps_min{ 0.0 }; double _fps_max{ 0.0 }; 
    };

    /// @brief Render game objects with shadows and produce the shadow map.
    struct render_shadows_evt
    {
        enum class action_t
        {
            start,
            end
        };

        action_t _action{ action_t::start };
    };

    /// @brief Perform any post process rending of the frame buffer.
    struct render_post_processing_evt
    {
        enum class action_t
        {
            start,
            end
        };

        action_t _action{ action_t::start };
    };

    /// @brief Render any user interface object.
    struct render_user_interface_evt
    {
        enum class action_t
        {
            start,
            end
        };

        action_t _action{ action_t::start };
    };

    /// @brief Dawn event from the day/night system.
    struct dawn_evt
    {
        double _sun_rise_duration{ 0.0 };
    };

    /// @brief Dusk event from the day/night system.
    struct dusk_evt
    {
        double _sun_set_duration{ 0.0 };
    };
}