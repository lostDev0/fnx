#if !defined _WIN32
#define CUTE_SOUND_FORCE_SDL
#endif
#define CUTE_SOUND_IMPLEMENTATION
#include <cutesound/cute_sound.h>

namespace fnx
{
    struct sound_impl
    {
        cs_audio_source_t _loaded_sound;
        cs_playing_sound_t _playing_sound;
    };

    struct audio_context
    {
        bool _isWindow{ false };

        audio_context()
        {
            void* hwnd = NULL;
#ifndef _FNX_WINDOW
        #ifndef CUTE_SOUND_FORCE_SDL
            hwnd = GetConsoleWindow();
        #endif
#else
            hwnd = GetActiveWindow();
            _isWindow = true;
#endif
            // the hwnd is null if you construct the context on its own thread
            // you have to get the hwnd from the active window of the main thread

            auto error = cs_init(hwnd, 44100, 8192, nullptr);
            if(error != cs_error_t::CUTE_SOUND_ERROR_NONE)
            {
                FNX_WARN(fnx::format_string("Error occurred initializing audio: %d", error));
            }
        }

        ~audio_context()
        {
            cs_shutdown();
        }
    };

    sound::sound(const std::string& file_path)
        : fnx::asset(file_path)
        , _impl(std::make_unique<sound_impl>())
    {
        auto [ctx, _] = singleton<audio_context>::acquire();
        cs_error_t error;
        _impl->_loaded_sound = *cs_load_wav(file_path.c_str(), &error);
        if (error != CUTE_SOUND_ERROR_NONE)
        {
            FNX_ERROR(fnx::format_string("failed to load %s %s", file_path, error));
            cs_free_audio_source(&_impl->_loaded_sound);
            _impl->_playing_sound = cs_playing_sound_t{};
            unload();
        }
        else
        {
            _impl->_playing_sound = cs_play_sound(&_impl->_loaded_sound, cs_sound_params_default());
        }
    }

    void sound::play()
    {
        if (is_loaded())
        {
            auto [ctx, _] = singleton<audio_context>::acquire();
            cs_play_sound(&_impl->_loaded_sound, cs_sound_params_default());
            //cs_insert_sound(ctx._context, &_impl->_playing_sound);
        }
    }

    void sound::loop()
    {
        // loop an already playing sound file
        cs_sound_set_is_looped(_impl->_playing_sound, true);
    }

    void sound::pause()
    {
        // pause a currently playing sound file
        cs_sound_set_is_paused(_impl->_playing_sound, true);
    }

    void sound::stop()
    {
        // pause a currently playing sound file
        // TODO
        //cs_stop_sound(&_impl->_playing_sound);
    }

    void sound::set_volume(float left, float right)
    {
        _volume_left =left;
        _volume_right = right;
        cs_sound_set_volume(_impl->_playing_sound, fnx::maximum(left, right));
        // TODO: different channel volumes
        //cs_set_volume(&_impl->_playing_sound, left, right);
    }

    void sound::apply_master_volume(float left, float right)
    {
        cs_set_playing_sounds_volume(fnx::maximum(left, right));
        // TODO: different channel volumes
        //cs_sound_set_volume(&_impl->_playing_sound, left * _volume_left, right * _volume_right);
    }

    void sound::mix()
    {
        //auto [ctx, _] = singleton<audio_context>::acquire();
        //if(ctx._context) cs_mix(ctx._context);
        cs_mix();
    }

    void sound::initialize_sound_context()
    {
        auto [ctx, _] = singleton<audio_context>::acquire();
    }

    sound::~sound()
    {
        cs_free_audio_source(&_impl->_loaded_sound);
        _impl->_playing_sound = cs_playing_sound_t{};
    }
}