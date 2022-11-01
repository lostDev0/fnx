#pragma once

namespace fnx
{
    struct display_mode
    {
        enum anti_alias
        {
            None = 0,
            AA_2x = 2,
            AA_4x = 4,
            AA_6x = 6,
            AA_8x = 8
        };

        bool operator==(const display_mode& other) const
        {
            return _width == other._width && 
                   _height == other._height && 
                   _depth == other._depth;
        }

        CREATE_ACCESSORS_WITH_DEFAULT(int, _width, 0);
        CREATE_ACCESSORS_WITH_DEFAULT(int, _height, 0);
        CREATE_ACCESSORS_WITH_DEFAULT(int, _refresh_rate, 0);
        CREATE_ACCESSORS_WITH_DEFAULT(int, _depth, 0);
        CREATE_ACCESSORS_WITH_DEFAULT(int, _fullscreen, 0);
        CREATE_ACCESSORS_WITH_DEFAULT(anti_alias, _anti_alias, anti_alias::None);
    };

    template<>
    inline auto serialize::register_members<display_mode>()
    {
        // TODO anti_alias
        return make_members(make_member("width", &display_mode::get_width, &display_mode::set_width),
            make_member("height", &display_mode::get_height, &display_mode::set_height),
            make_member("refresh_rate", &display_mode::get_refresh_rate, &display_mode::set_refresh_rate),
            make_member("depth", &display_mode::get_depth, &display_mode::set_depth),
            make_member("fullscreen", &display_mode::get_fullscreen, &display_mode::set_fullscreen));
    }
    
    template<>
    inline auto serialize::register_name<display_mode>() { return "display_mode"; }
}
