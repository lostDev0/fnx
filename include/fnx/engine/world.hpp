#pragma once

namespace fnx
{
    namespace world
    {
        /// @brief Initialize the fnx engine world and any dependencies.
        extern void init();

        // TODO make display a const
        /// @brief Create the opengl window.
        extern void create_window(const char* window_title, fnx::display_mode& display);

        /// @brief Start the fnx engine.
        extern void run();

        /// @brief Finish fnx engine execution cycle and return.
        /// @return Always returns false.
        extern bool stop(const window_close_evt&);

        /// @brief Destroy the window.
        extern void terminate();

        /// @brief Save a display mode object to a file.
        /// @param file_path local file system path
        /// @param mode display mode object
        extern void save_display_configuration(const std::string& file_path, fnx::display_mode& mode);

        /// @brief Load a display mode object from a file.
        /// @param file_path local file system path
        /// @return display_mode object
        extern fnx::display_mode load_display_configuration(const std::string& file_path);
    }
}