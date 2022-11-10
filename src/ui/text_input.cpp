namespace fnx
{
text_input::text_input( const std::string& font, const std::string& name )
    : label( font, "", name )
    , _blinker{ 0.5, 0.5 }
    , _is_on{ false }
{
    _blinker.set_repeating();
    // label disables interactivety by default
    enable_events();
}

void text_input::update( double delta )
{
    _blinker.update( delta );

    if ( _blinker.is_on() )
    {
        if ( !_is_on || _dirty_cache )
        {
            std::string temp ( _current_label );
            const auto it_begin = temp.begin();
            const auto it = it_begin + _current_pos;
            temp.insert( it, 0 );	// 0x00 id must exist in the font file or this blinking indicator won't appear
            label::set_text( temp );
        }
        _is_on = true;
    }
    else
    {
        if ( _is_on || _dirty_cache )
        {
            label::set_text( _current_label );
        }
        _is_on = false;
    }
    label::update( delta );
}

void text_input::do_active()
{
    _blinker.start();
}

void text_input::do_inactive()
{
    _blinker.stop();
}

bool text_input::do_mouse_move( double x, double y )
{
    //_cursor_pos = vec2d{ x,y };
    return is_active();
}

bool text_input::do_mouse_press( FNX_BUTTON btn, double x, double y )
{
    // absorb the mouse press event
    if ( _mouse_over )
    {
        // activate on press as it is the intention of the user to manipulate this widget
        // TODO : Calculate the starting highlight position
        activate();
        return true;
    }
    return false;
}

bool text_input::do_mouse_release( FNX_BUTTON btn, double x, double y )
{
    // TODO : Handle highlight in progress
    // absorb the mouse release event
    if ( _mouse_over && is_active() )
    {
        auto [win, _] = singleton<window>::acquire();
        auto window_width = win.width();
        auto window_height = win.height();
        // calculate the cursor position within the string and place the cursor there
        // handle empty string or string not long enough
        auto dx = x - get_abs_x();	// horizontal position within widget
        auto dy = y - get_abs_y();	// vertical position within widget
        auto c = _label_font->get_char_info( 'A' );
        float height_in_opengl = _font_size > 0.f ? _font_size : get_height();
        float height_in_pixels = height_in_opengl * ( window_height ) * 2.f;
        float width_to_opengl = 2.f / window_width;		/// conversion factor for width
        float font_scale = height_in_pixels / static_cast<float>( c.height );

        // TODO : handle removed spaces from word wrap
        auto num_lines = static_cast<unsigned int>( dy / _label_font->calculate_line_height( height_in_pixels,
                         static_cast<float>( window_height ) ) );
        size_t delta_chars = 0u;
        for ( auto i = 0u; i < _lines.size() - num_lines; ++i )
        {
            delta_chars += _lines[i].second.size();
        }
        // TODO : handle center and right aligned text
        auto total = 0.f;
        for ( const auto& character : _lines[_lines.size() - num_lines - 1].second )
        {
            auto width = static_cast<float>( _label_font->get_char_info( character ).xadvance ) * width_to_opengl * font_scale;
            if ( total + ( width / 2.f ) > dx )	// divide by 2 here helps with clicking inside a char
            {
                break;
            }
            total += width;
            ++delta_chars;
        }

        _current_pos = delta_chars;
        return true;
    }
    else if ( is_active() )
    {
        // release occurred outside of the widget, no longer editting
        inactivate();
        return true;
    }
    return false;
}

bool text_input::do_key_press( FNX_KEY key )
{
    // just absorb the event, the repeat or release will trigger a text update
    return is_active();
}

void text_input::process_key( FNX_KEY key )
{
    if ( key == FNX_KEY::FK_BACKSPACE )
    {
        if ( _current_label.size() > _minimum )
        {
            if ( window::is_key_pressed( FNX_KEY::FK_LEFT_CONTROL ) || window::is_key_pressed( FNX_KEY::FK_RIGHT_CONTROL ) )
            {
                // delete everything before
                _current_label.erase( _current_label.begin() + ( _current_pos - 1 ), _current_label.begin() + ( _minimum ) );
            }
            else
            {
                // delete single char
                if ( _current_pos > 0 )
                {
                    _current_label.erase( _current_label.begin() + ( _current_pos - 1 ) );
                    --_current_pos;
                }
            }
        }

        text_update_evt e;
        e._src = _name;
        e._text = _current_label;
        FNX_EMIT( e );
    }
    else if ( key == FNX_KEY::FK_DELETE )
    {
        if ( window::is_key_pressed( FNX_KEY::FK_LEFT_CONTROL ) || window::is_key_pressed( FNX_KEY::FK_RIGHT_CONTROL ) )
        {
            // delete everything after
            _current_label.erase( _current_label.begin() + ( _current_pos - 1 ), _current_label.end() );
        }
        else
        {
            // delete a single char
            _current_label.erase( _current_label.begin() + _current_pos );
        }

        text_update_evt e;
        e._src = _name;
        e._text = _current_label;
        FNX_EMIT( e );
    }
    else if ( key == FNX_KEY::FK_ESCAPE )
    {
        do_inactive();
    }
    else if ( key == FNX_KEY::FK_LEFT )
    {
        --_current_pos;
        if ( _current_pos < _minimum )
        {
            _current_pos = _minimum;
        }
    }
    else if ( key == FNX_KEY::FK_RIGHT )
    {
        _current_pos++;
        if ( _current_pos > _current_label.size() )
        {
            _current_pos = _current_label.size();
        }
    }
    else if ( key == FNX_KEY::FK_HOME )
    {
        _current_pos = _minimum;
    }
    else if ( key == FNX_KEY::FK_END )
    {
        _current_pos = _current_label.size();
    }
    else if ( key == FNX_KEY::FK_ENTER )
    {
        text_submit_evt e;
        e._text = _current_label;
        e._src = _name;
        FNX_EMIT( e );
    }
    else if ( key == FNX_KEY::FK_C && ( window::is_key_pressed( FNX_KEY::FK_LEFT_CONTROL )
                                        || window::is_key_pressed( FNX_KEY::FK_RIGHT_CONTROL ) ) )
    {
        // TODO : Copy current highlight
    }
    else if ( key == FNX_KEY::FK_V && ( window::is_key_pressed( FNX_KEY::FK_LEFT_CONTROL )
                                        || window::is_key_pressed( FNX_KEY::FK_RIGHT_CONTROL ) ) )
    {
        // TODO : Past clipboard at current location
    }
    else if ( key == FNX_KEY::FK_A && ( window::is_key_pressed( FNX_KEY::FK_LEFT_CONTROL )
                                        || window::is_key_pressed( FNX_KEY::FK_RIGHT_CONTROL ) ) )
    {
        // TODO : Select All
    }
    else
    {
        auto c = key_to_ascii( key );
        if ( test_char( c ) && _current_label.size() < _maximum )
        {
            if ( c != 0x00 )
            {
                _current_label.insert( _current_label.begin() + _current_pos, c );
                ++_current_pos;
            }

            text_update_evt e;
            e._src = _name;
            e._text = _current_label;
            FNX_EMIT( e );
        }
    }

    _dirty_cache = true;
}

bool text_input::do_key_release( FNX_KEY key )
{
    if ( is_active() )
    {
        process_key( key );
        return true;
    }
    return false;
}

bool text_input::do_key_repeat( FNX_KEY key )
{
    if ( is_active() )
    {
        process_key( key );
        return true;
    }
    return false;
}

void text_input::set_text( const std::string& label )
{
    _current_label = label;
    _current_pos = label.size();
    label::set_text( label );
}

bool text_input::test_char( char c )
{
    if ( _all || ( _alpha_allowed && std::isalpha( c ) ) )
    {
        return true;
    }
    else if ( _numeric_allowed && std::isdigit( c ) )
    {
        return true;
    }
    else if ( _space_allowed && ' ' == c )
    {
        return true;
    }
    return false;
}
}