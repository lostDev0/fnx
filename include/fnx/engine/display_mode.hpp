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

        display_mode() = default;
        ~display_mode() = default;

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
}
