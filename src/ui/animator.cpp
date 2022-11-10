using namespace std;
using namespace std::chrono;

namespace fnx
{
double animator::start( type index, double wait )
{
    auto duration = 0.0;
    switch ( index )
    {
        case mouse_enter:
            duration = start( mouse_enter, mouse_exit, _rewind_on_exit, false, false, wait );
            break;
        case mouse_exit:
            duration = start( mouse_exit, mouse_enter, _rewind_on_exit, false, false, wait );
            break;
        case mouse_press:
            duration = start( mouse_press, mouse_release, _rewind_on_release, false, false, wait );
            break;
        case mouse_release:
            duration = start( mouse_release, mouse_press, _rewind_on_release, false, false, wait );
            break;
        case show:
            duration = start( show, hide, _rewind_on_hide, true, false, wait );
            break;
        case hide:
            duration = start( hide, show, _rewind_on_hide, true, true, wait );
            break;
        case activate:
            duration = start( activate, inactivate, _rewind_on_inactive, false, false, wait );
            break;
        case inactivate:
            duration = start( inactivate, activate, _rewind_on_inactive, false, false, wait );
            break;
        case max:
        default:
            break;
    }
    return duration;
}

void animator::finish( type index, widget& w )
{
    auto anim = &_transition_map[index];
    if ( anim->_hide_widget_on_finish )
    {
        w.do_made_invisible();
    }
    anim->_ignore_mouse_input = false;
    anim->_wait_to_finish = 0.0;
    anim->_hide_widget_on_finish = false;
    anim->_has_finished = true;
}

void animator::update( double delta, widget& w )
{
    bool ignore_input = false;
    for ( const auto& t : _transition_map )
    {
        ignore_input |= t._ignore_mouse_input;
    }

    for ( auto i = 0; i < type::max; ++i )
    {
        if ( ignore_input && ( i != type::show && i != type::hide ) )
        {
            continue;
        }

        auto& t = _transition_map[i];
        if ( t._has_finished )
        {
            continue;
        }

        t._wait_to_finish -= delta;
        t._transitions.apply( delta, w );

        auto wait = t._wait_to_finish > 0.0;
        auto done = t._transitions.is_done();

        // if done and wait to finish has expired, finish
        if ( done && !wait && !t._has_finished )
        {
            finish( static_cast<animator::type>( i ), w );
        }
    }
}

double animator::start( animator::type start, animator::type stop, bool rewind, bool ignore_io, bool hide_on_finish,
                        double wait )
{
    auto& begin = _transition_map[start];
    auto& end = _transition_map[stop];
    auto& begin_transitions = begin._transitions;
    auto& end_transitions = end._transitions;
    auto end_in_progress = !end_transitions.is_empty() && !end_transitions.is_done();
    double duration = 0.0;

    if ( end_transitions.is_empty() && begin_transitions.is_empty() )
    {
        return 0.0;
    }

    if ( end_transitions.is_empty() && !begin_transitions.is_empty() )
    {
        // only working with the start here
        begin_transitions.play( false, begin._has_finished );
        if ( begin._has_finished )
        {
            begin_transitions.seek_to_start();
        }
        begin._ignore_mouse_input = ignore_io;
        begin._hide_widget_on_finish = hide_on_finish;
        begin._wait_to_finish = wait;
        begin._has_finished = false;
        duration = begin_transitions.get_remainder();
    }
    else if ( begin_transitions.is_empty() && !end_transitions.is_empty() )
    {
        // only working with the end here
        // reverse the partial or complete end transition
        end_transitions.play( true, end._has_finished );
        end._ignore_mouse_input = ignore_io;
        end._hide_widget_on_finish = hide_on_finish;
        end._wait_to_finish = wait;
        end._has_finished = false;
        duration = end_transitions.get_remainder();
    }
    else
    {
        // both of the transitions exist
        if ( !end_transitions.is_done() )
        {
            // end is still in progress, just reverse it
            end_transitions.play( true, end_transitions.is_done() );
            end._hide_widget_on_finish = hide_on_finish;
            end._has_finished = false;
            duration = end_transitions.get_remainder();
        }
        else
        {
            // end is not in progress, perform the start
            begin_transitions.play( false, begin_transitions.is_done() );
            begin._ignore_mouse_input = ignore_io;
            begin._hide_widget_on_finish = hide_on_finish;
            begin._wait_to_finish = wait;
            begin._has_finished = false;
            duration = begin_transitions.get_remainder();
        }
    }

    return duration;
}
}