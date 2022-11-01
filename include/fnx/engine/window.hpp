#pragma once

#include <atomic>
#include <string>
#include <vector>

namespace fnx
{
	using window_handle = int;

	class window
	{
		std::atomic<bool> _dirty_cache{ true };
		fnx::display_mode _display_mode;
		int _dimensions_x{ 0 };
		int _dimensions_y{ 0 };
		float _dpi_x{ 0.f };
		float _dpi_y{ 0.f };
		int _screen_dim_x{ 0 };
		int _screen_dim_y{ 0 };
		int _window_position_x{ 0 };
		int _window_position_y{ 0 };
		std::string _title{};
	public:
		window();
		~window();

		window(const window& other) = default;

		/// @brief Force an update of stored dimension information.
		void update_dimensions();

		/// @brief Create and initialize the window.
		bool create(const std::string& title);

		/// @brief Get all supported video card display modes from the GPU.
		static std::vector<fnx::display_mode> get_supported_display_modes();

		/// @brief Get current display resolution.
		static fnx::display_mode get_resolution();

		/// @brief Check if a display mode is supported by the video card.
		static bool is_display_supported(const fnx::display_mode& mode);

		/// @brief Set the display mode of the window.
		/// @note a display mode must be set prior to initialization
		void set_display_mode(const fnx::display_mode& display_mode);

		void set_dimensions(int x, int y, int width, int height);

		/// @brief Returns this window's display mode.
		fnx::display_mode get_display_mode();

		int get_pos_x();
		int get_pos_y();
		void get_pos(int& x, int& y);

		const std::string get_title() const { return _title; }

		/// @brief Returns the window width if it has been initialized.
		int width();

		/// @brief Returns the window height if it has been initialized.
		int height();

		/// @brief Returns the screen width in pixels.
		int screen_width();

		/// @brief Returns the screen height in pixels.
		int screen_height();

		/// @brief Processes the window events since last call.
		void update();

		/// @brief Swap the frame buffer.
		/// @note called at the end of the render cycle
		void swap();

		/// @brief Returns the pixels per inch of display.
		/// @note This call is performance intensive.
		void dpi(float& x, float& y);

		/// @brief Set the window to full screen or normal mode.
		void set_fullscreen(bool enable);

		/// @brief Returns the windows width/height.
		float get_aspect_ratio();

		/// @brief Remove the system cursor graphic.
		static void hide_default_cursor();

		/// @brief Show the system cursor graphic.
		static void show_default_cursor();

		/// @brief Return the pressed state of the key.
		static bool is_key_pressed(fnx::FNX_KEY key);

		/// @brief Returns the position in global coordinates of the cursor.
		static void get_cursor_position(double& pos_x, double& pos_y);

		/// @brief Returns the position opengl coordinates.
		fnx::vector2 screen_to_opengl(double screen_x, double screen_y);

		/// @brief Returns the position in screen coordinates where 0,0 is the top left corner of the screen.
		fnx::vector2 opengl_to_screen(double gl_x, double gl_y);

		/// @brief Returns the position in screen coordinates where 0,0 is the bottom left corner of the screen.
		fnx::vector2 opengl_to_cartesian(double gl_x, double gl_y);

		void set_icon(const char* image_path, const char* small_image_path);

	private:
		bool on_window_resize(const fnx::window_resize_evt& evt);
	};
}