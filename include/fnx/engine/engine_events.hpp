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

    template<typename T>
    inline std::string to_string(const T&)
    {
        return "unknown type";
    }

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
    struct window_resize_evt
    { 
        int _x{ 0 }; 
        int _y{ 0 }; 
        int _width{ 0 }; 
        int _height{ 0 };
    };
    struct window_move_evt
    { 
        int _x{ 0 }; 
        int _y{ 0 }; 
        int _width{ 0 }; 
        int _height{ 0 };
    };
    struct window_minimize_evt {};
    struct window_maximize_evt {};
    struct window_fullscreen_evt {};
    struct window_lose_focus_evt {};
    struct window_gain_focus_evt {};
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
    struct keyboard_release_evt { FNX_KEY _key{ FNX_KEY::FK_UNKNOWN }; };
    struct keyboard_repeat_evt { FNX_KEY _key{ FNX_KEY::FK_UNKNOWN }; };

    struct mouse_enter_evt {};
    struct mouse_exit_evt {};
    struct mouse_move_evt { double _x{ 0.0 }; double _y{ 0.0 }; double _gl_x{ 0.0 }; double _gl_y{ 0.0 }; };
    struct mouse_press_evt { FNX_BUTTON _btn{ FNX_BUTTON::BT_UNKNOWN }; double _x{ 0.0 }; double _y{ 0.0 }; double _gl_x{ 0.0 }; double _gl_y{ 0.0 }; };
    struct mouse_release_evt { FNX_BUTTON _btn{ FNX_BUTTON::BT_UNKNOWN }; double _x{ 0.0 }; double _y{ 0.0 }; double _gl_x{ 0.0 }; double _gl_y{ 0.0 }; };
    struct mouse_scroll_evt { double _x{ 0.0 }; double _y{ 0.0 }; };

    struct widget_active_evt { unsigned int _src; };
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