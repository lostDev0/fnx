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
    struct serializer<fnx::display_mode>
    {
        static inline void to_yaml(std::string& content, const fnx::display_mode& obj)
        {
            YAML::Emitter out;
            out << YAML::BeginMap;
            out << YAML::Key << "width" << YAML::Value << obj.get_width();
            out << YAML::Key << "height" << YAML::Value << obj.get_height();
            out << YAML::Key << "refresh_rate" << YAML::Value << obj.get_refresh_rate();
            out << YAML::Key << "depth" << YAML::Value << obj.get_depth();
            out << YAML::Key << "fullscreen" << YAML::Value << obj.get_fullscreen();
            out << YAML::Key << "anti_alias" << YAML::Value << obj.get_anti_alias();
            out << YAML::EndMap;
            content = out.c_str();
        }

        static inline void from_yaml(const std::string& content, fnx::display_mode& obj)
        {
            YAML::Node data = YAML::Load(content);
            if(data["width"])
                obj.set_width(data["width"].as<decltype(obj._width)>());
            if(data["height"])
                obj.set_height(data["height"].as<decltype(obj._height)>());
            if(data["refresh_rate"])
                obj.set_refresh_rate(data["refresh_rate"].as<decltype(obj._refresh_rate)>());
            if(data["depth"])
                obj.set_depth(data["depth"].as<decltype(obj._depth)>());
            if(data["fullscreen"])
                obj.set_fullscreen(data["fullscreen"].as<decltype(obj._fullscreen)>());
            if(data["anti_aias"])
                obj.set_anti_alias(static_cast<display_mode::anti_alias>(data["anti_alias"].as<int>()));
        }
    };
}
