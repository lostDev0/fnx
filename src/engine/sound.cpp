#define CUTE_SOUND_IMPLEMENTATION
#include <cutesound/cute_sound.h>

namespace fnx
{
    struct sound_impl
    {
        cs_loaded_sound_t _loaded_sound{};
        cs_playing_sound_t _playing_sound{};
    };

    struct audio_context
    {
        HWND _hwnd{};
        bool _isWindow{ false };
        cs_context_t* _context{ nullptr };

        audio_context()
        {
#ifndef _FNX_WINDOW
            _hwnd = GetConsoleWindow();
#else
            _hwnd = GetActiveWindow();
            _isWindow = true;
#endif
            // the hwnd is null if you construct the context on its own thread
            // you have to get the hwnd from the active window of the main thread

            _context = cs_make_context(_hwnd, 44100, 8192, 0, nullptr);
            if (nullptr == _context)
            {
                FNX_ERROR(std::string("Audio context cannot be created within a thread"));
            }
        }

        ~audio_context()
        {
            cs_shutdown_context(_context);
        }
    };

    sound::sound(const std::string& file_path)
        : fnx::asset(file_path)
        , _impl(std::make_unique<sound_impl>())
    {
        auto [ctx, _] = singleton<audio_context>::acquire();
        _impl->_loaded_sound = cs_load_wav(file_path.c_str());
        if (cs_error_reason != nullptr)
        {
            //FNX_ERROR("failed to load %s %s", file_path, cs_error_reason);
            cs_free_sound(&_impl->_loaded_sound);
            _impl->_playing_sound = cs_playing_sound_t{};
            unload();
        }
        else
        {
            _impl->_playing_sound = cs_make_playing_sound(&_impl->_loaded_sound);
        }
    }

    void sound::play()
    {
        if (is_loaded())
        {
            auto [ctx, _] = singleton<audio_context>::acquire();
            cs_insert_sound(ctx._context, &_impl->_playing_sound);
        }
    }

    void sound::loop()
    {
        // loop an already playing sound file
        cs_loop_sound(&_impl->_playing_sound, 1);
    }

    void sound::pause()
    {
        // pause a currently playing sound file
        cs_pause_sound(&_impl->_playing_sound, 1);
    }

    void sound::stop()
    {
        // pause a currently playing sound file
        cs_stop_sound(&_impl->_playing_sound);
    }

    void sound::set_volume(float left, float right)
    {
        _volume_left =left;
        _volume_right = right;
        cs_set_volume(&_impl->_playing_sound, left, right);
    }

    void sound::apply_master_volume(float left, float right)
    {
        cs_set_volume(&_impl->_playing_sound, left * _volume_left, right * _volume_right);
    }

    void sound::mix()
    {
        auto [ctx, _] = singleton<audio_context>::acquire();
        if(ctx._context) cs_mix(ctx._context);
    }

    void sound::initialize_sound_context()
    {
        auto [ctx, _] = singleton<audio_context>::acquire();
    }

    sound::~sound()
    {
        cs_free_sound(&_impl->_loaded_sound);
        _impl->_playing_sound = cs_playing_sound_t{};
    }
}