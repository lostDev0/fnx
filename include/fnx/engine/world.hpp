#pragma once

namespace fnx
{
    namespace world
    {
        /// @brief Initialize the fnx engine world and any dependencies.
        void init(const char* window_title);

        /// @brief Start the fnx engine.
        void run();

        /// @brief Finish fnx engine execution cycle and return.
        /// @return Always returns false.
        bool stop(const window_close_evt&);

        /// @brief Destroy the window.
        void terminate();
    }
}