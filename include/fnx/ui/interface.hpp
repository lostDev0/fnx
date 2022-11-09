#pragma once

namespace fnx
{
    constexpr auto PROPERTY_UI_SCALE = "ui_scale";			/// defined within world properties as the scale factor of the user interface
    namespace ui
    {
        /// @brief Initialize the user interface
        extern bool init();

        /// @brief Create a widget layer from configuration file
        extern void parse_yaml_file(const std::string& file_path);
    }
}