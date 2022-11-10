namespace fnx
{
void transition::start( double delay )
{
    _delay = delay;
    _current_time = fnx::clamp( _current_time, 0.0, _duration );
    _paused = false;
    _rewind = false;
}

void transition::pause()
{
    _paused = true;
}

void transition::rewind( double delay )
{
    _delay = delay;
    _current_time = fnx::clamp( _current_time, 0.0, _duration );

    _paused = false;
    _rewind = true;
}

double transition::get_adjusted_delta( double delta )
{
    // are we waiting for the delay?
    if ( _delay > 0.0 )
    {
        _delay -= delta;
        if ( _delay < 0.0 )
        {
            // transitioned past delay time
            delta = 0.0 - _delay;
            _delay = 0.0;
        }
        // still waiting on the delay
        return 0.0;
    }

    if ( _rewind )
    {
        delta *= -1.0;
    }
    if ( _paused )
    {
        delta = 0.0;
    }

    if ( _current_time + delta < 0.0 )
    {
        delta = -_current_time;
    }
    return delta;
}

void transitions::apply( double delta_in_seconds, widget& w )
{
    for ( const auto& t : _transitions )
    {
        auto delta = t->get_adjusted_delta( delta_in_seconds );
        t->apply( delta, w );
    }
}

void transitions::seek_to_end()
{
    for ( const auto& t : _transitions )
    {
        t->seek_to( t->get_duration() );
    }
}

void transitions::seek_to_start()
{
    for ( const auto& t : _transitions )
    {
        t->seek_to( 0.0 );
    }
}

void transitions::play( bool rewind, bool delay )
{
    _rewind = rewind;
    if ( rewind )
    {
        for ( auto& t : _transitions )
        {
            t->rewind( delay ? _delay : 0.0 );
        }
    }
    else
    {
        for ( auto& t : _transitions )
        {
            t->start( delay ? _delay : 0.0 );
        }
    }
};

void transitions::pause()
{
    for ( auto& t : _transitions )
    {
        t->pause();
    }

};

bool transitions::is_done()
{
    bool result = true;
    for ( auto& t : _transitions )
    {
        result &= t->is_done();
    }
    return result;
}

bool transitions::is_paused()
{
    bool result = true;
    for ( auto& t : _transitions )
    {
        result &= t->is_paused();
    }
    return result;
}

void slide_transition::apply( double delta_in_seconds, widget& w )
{
    _current_time += delta_in_seconds;
    auto pos = _tween.get( _current_time / _duration ) * w.get_current_size();

    w.set_position( pos + w.get_original_position() );
};

void fade_transition::apply( double delta_in_seconds, widget& w )
{
    _current_time += delta_in_seconds;
    auto alpha = _tween.get( _current_time / _duration );

    w.set_alpha( alpha * w.get_original_alpha() );
};

void size_transition::apply( double delta_in_seconds, widget& w )
{
    _current_time += delta_in_seconds;
    auto size = _tween.get( _current_time / _duration );

    w.set_size( size * w.get_original_size() );
}
}