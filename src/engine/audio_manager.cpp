
namespace fnx
{
void audio_manager::play_sound( const char* file_path, float left, float right )
{
    sound_evt e;
    e._type = sound_evt::play;
    e._resource_name = file_path;
    e._left_volume = left;
    e._right_volume = right;
    FNX_EMIT( e );
}

void audio_manager::pause_sound( const char* file_path )
{
    sound_evt e;
    e._type = sound_evt::pause;
    e._resource_name = file_path;
    FNX_EMIT( e );
}

void audio_manager::loop_sound( const char* file_path, float left, float right )
{
    sound_evt e;
    e._type = sound_evt::loop;
    e._resource_name = file_path;
    e._left_volume = left;
    e._right_volume = right;
    FNX_EMIT( e );
}

void audio_manager::stop_sound( const char* file_path )
{
    sound_evt e;
    e._type = sound_evt::stop;
    e._resource_name = file_path;
    FNX_EMIT( e );
}

void audio_manager::set_volume( const char* file_path, float left, float right )
{
    sound_evt e;
    e._type = sound_evt::volume;
    e._resource_name = file_path;
    e._left_volume = left;
    e._right_volume = right;
    FNX_EMIT( e );
}

void audio_manager::set_master_volume( float left, float right )
{
    sound_evt e;
    e._type = sound_evt::master_volume;
    e._left_volume = left;
    e._right_volume = right;
    FNX_EMIT( e );
}

audio_manager::audio_manager()
    : async_task{}
{
    auto [emitter, _] = fnx::singleton<fnx::event_manager>::acquire();
    _event_queue.reserve( max_events );
    emitter.subscribe<sound_evt>( fnx::bind( *this, &audio_manager::on_event ) );
    emitter.subscribe<window_init_evt>( fnx::bind( *this, &audio_manager::on_window_init ) );
    emitter.subscribe<window_close_evt>( fnx::bind( *this, &audio_manager::on_window_close ) );
}

audio_manager::~audio_manager()
{
    auto [emitter, _] = fnx::singleton<fnx::event_manager>::acquire();
    emitter.unsubscribe<sound_evt>( fnx::bind( *this, &audio_manager::on_event ) );
    emitter.unsubscribe<window_init_evt>( fnx::bind( *this, &audio_manager::on_window_init ) );
    emitter.unsubscribe<window_close_evt>( fnx::bind( *this, &audio_manager::on_window_close ) );
}

bool audio_manager::on_event( const sound_evt& event )
{
    std::scoped_lock guard( _lock );
    if ( _event_queue.size() < audio_manager::max_events )
    {
        // to be processed on the audio thread
        _event_queue.emplace_back( event );
    }

    // who else would be processing sound events? we'll return false just in case
    return false;
}

bool audio_manager::on_window_init( const window_init_evt& event )
{
    FNX_INFO( std::string( "initializing sound context" ) );
    std::scoped_lock guard( _lock );
    // window initialization is required for the sound lib to get the hardware handle
    sound::initialize_sound_context();
    _initialized = true;
    return false;
}

bool audio_manager::on_window_close( const window_close_evt& event )
{
    std::scoped_lock guard( _lock );
    _initialized = false;
    stop();	// stop future calls to run()
    return false;
}

void audio_manager::run()
{
    if ( !_initialized || !_running )
    {
        return;
    }

    sound::mix();

    if ( _event_queue.size() == 0 )
    {
        return;
    }

    const auto it_end = end( _event_queue );

    for ( auto it = begin( _event_queue ); it != it_end; ++it )
    {
        const auto& e = ( *it );
        // handle one event at a time and exit the function to allow any system events to not block
        switch ( e._type )
        {
            case sound_evt::play:
                {
                    auto res = _assets.get( e._resource_name );
                    if ( res )
                    {
                        res->play();
                        res->set_volume( e._left_volume, e._right_volume );
                    }
                    else
                    {
                        FNX_ERROR( std::string( "unable to find asset " ) + std::string( e._resource_name ) );
                    }
                }
                break;
            case sound_evt::pause:
                {
                    auto res = _assets.get( e._resource_name );
                    if ( res )
                    {
                        res->pause();
                    }
                    else
                    {
                        FNX_ERROR( std::string( "unable to find asset " ) + e._resource_name );
                    }
                }
                break;
            case sound_evt::stop:
                {
                    auto res = _assets.get( e._resource_name );
                    if ( res )
                    {
                        res->stop();
                    }
                    else
                    {
                        FNX_ERROR( std::string( "unable to find asset " ) + e._resource_name );
                    }
                }
                break;
            case sound_evt::loop:
                {
                    auto res = _assets.get( e._resource_name );
                    if ( res && res->is_loaded() )
                    {
                        res->play();
                        res->loop();
                        res->set_volume( e._left_volume, e._right_volume );
                    }
                    else
                    {
                        FNX_ERROR( std::string( "unable to find asset " ) + e._resource_name );
                    }
                }
                break;
            case sound_evt::volume:
                {
                    auto res = _assets.get( e._resource_name );
                    if ( res && res->is_loaded() )
                    {
                        res->set_volume( e._left_volume, e._right_volume );
                        res->apply_master_volume( _master_volume_left, _master_volume_right );
                    }
                    else
                    {
                        FNX_ERROR( std::string( "unable to find asset " ) + e._resource_name );
                    }
                }
                break;
            case sound_evt::master_volume:
                {
                    _master_volume_left = e._left_volume;
                    _master_volume_right = e._right_volume;

                    // apply the master volume to all sounds
                    for ( auto& sound : _assets.get_all() )
                    {
                        sound.second->apply_master_volume( e._left_volume, e._right_volume );
                    }
                }
                break;
            default:
                break;
        }
    }
    _event_queue.clear();
}
}