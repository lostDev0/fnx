#pragma once

namespace fnx
{
/// @brief Asynchronous system that handles all audio events for the engine.
/// @note Events are held in a queue so that they can be processed without blocking the engine.
class audio_manager : public fnx::async_task
{
public:
    const unsigned int max_events = 256u;
    audio_manager();
    ~audio_manager();

    /// @brief Queue a sound to start playing.
    void play_sound( const char* name, float left_channel_volume = 1.f, float right_channel_volume = 1.f );

    /// @brief Queue a sound to be played repeatedly.
    void loop_sound( const char* name, float left_channel_volume = 1.f, float right_channel_volume = 1.f );

    /// @brief Queue a sound to be paused.
    void pause_sound( const char* name );

    /// @brief Queue a sound to be stopped.
    void stop_sound( const char* name );

    /// @brief Queue a volume change to a current sound.
    void set_volume( const char* name, float left_channel, float right_channel );

    /// @brief Queue a master volume change to all sounds.
    void set_master_volume( float left_channel, float right_channel );

    /// @brief Copies all events triggered by the engine into the queue.
    bool on_event( const sound_evt& event );

    /// @brief Setup the audio context when a window is opened.
    bool on_window_init( const window_init_evt& event );

    /// @brief Ensure that if we lost our window, we invalidate the sound context.
    bool on_window_close( const window_close_evt& event );

protected:
    /// @brief Loop through all events and handle them in the order they were received.
    void run() override final;

    asset_manager<sound> _assets{};
    float _master_volume_left{ 1.f };		/// volume multiplier for all sounds
    float _master_volume_right{ 1.f };		/// volume multiplier for all sounds
    bool _initialized{ false };				/// flag telling the system that the window should play sound
    std::vector<sound_evt> _event_queue;	/// ordered list of all events to be processed
};
}