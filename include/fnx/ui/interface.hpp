#pragma once

namespace fnx
{
	constexpr auto PROPERTY_UI_SCALE = "ui_scale";			/// defined within world properties as the scale factor of the user interface
	namespace ui
	{
		//extern void enable_debug_ui_output();
		//extern void disable_debug_ui_output();
		//extern void display_example_ui();

		/// @brief Initialize the user interface
		extern bool init();

		/// @brief Create a widget layer from configuration file
		extern fnx::layer_handle parse_file(const std::string& file_path, bool add_to_layer_stack = true);

		extern fnx::layer_handle parse_string(const std::string& content, bool add_to_layer_stack = true);
	}
}