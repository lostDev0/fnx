namespace fnx
{
//bool panel::on_event(const fnx::mouse_move_evt& event)
bool panel::do_mouse_move( double x, double y )
{
    _current_cursor_position.x = x;
    _current_cursor_position.y = y;
    if ( _drag )
    {
        auto widget_position = fnx::vector2{ get_x(), get_y() };
        widget_position.x = ( widget_position.x + static_cast<float>( _current_cursor_position.x - _prev_cursor_position.x ) );
        widget_position.y = ( widget_position.y + static_cast<float>( _current_cursor_position.y - _prev_cursor_position.y ) );

        auto [win, _] = singleton<window>::acquire();

        // keep it on the screen
        widget_position.x = ( fnx::maximum( widget_position.x, 0.f ) );
        widget_position.x = ( fnx::minimum( widget_position.x, 2.f - get_width() ) );

        widget_position.y = ( fnx::maximum( widget_position.y, 0.f ) );
        widget_position.y = ( fnx::minimum( widget_position.y, 2.f - get_height() ) );

        set_position( widget_position );

        // we've changed the panel's characteristics, overwrite the constraints
        if ( has_parent() )
        {
            fnx::constraints c;
            // position relative to the parent to maintain position on window resize
            c.add( fill_vert_constraint{ get_height() / _parent->get_height() } );
            c.add( fill_horz_constraint{ get_width() / _parent->get_width() } );
            c.add( relative_vert_constraint{ get_y() / _parent->get_height() } );
            c.add( relative_horz_constraint{ get_x() / _parent->get_width() } );
            set_constraints( c );
            on_parent_change( _parent->get_x(), _parent->get_y(), _parent->get_width(), _parent->get_height() );
        }

        for ( auto& c : _children )
        {
            c->on_parent_change( get_x(), get_y(), get_width(), get_height() );
        }

        _prev_cursor_position = _current_cursor_position;
        return true;
    }
    else if ( _resize )
    {
        auto dx = static_cast<float>( x - _prev_cursor_position.x );
        auto dy = static_cast<float>( y - _prev_cursor_position.y );
        auto width = get_width();
        auto height = get_height();
        auto min_width = fnx::maximum( _resize_btn->get_width(), _icon->get_width() );
        auto min_height = fnx::maximum( _resize_btn->get_height(), _icon->get_width() );

        // determine where the anchor is in regards to the widget
        // we'll assume the anchor isn't in outside of the widget
        auto abs_x = get_abs_x();
        auto abs_y = get_abs_y();
        auto local_x = get_x();
        auto local_y = get_y();

        auto left = _resize_btn->get_abs_x() + _resize_btn->get_width() / 2.f < ( abs_x + ( width / 2.f ) );
        auto lower = _resize_btn->get_abs_y() + _resize_btn->get_height() / 2.f < ( abs_y + ( height / 2.f ) );

        if ( left )
        {
            // resize needs to move position to the right
            if ( local_x + dx > 0 )
            {
                local_x += dx;

                width -= dx;
            }
        }
        else
        {
            if ( local_x + dx > 0 )
            {
                width += dx;
            }
        }

        if ( lower )
        {
            local_y += dy;
            height -= dy;
        }
        else
        {
            height += dy;
        }

        width = fnx::maximum( width, min_width );
        height = fnx::maximum( height, min_height );

        set_size( width, height );
        set_position( local_x, local_y );

        // we've changed the panel's characteristics, overwrite the constraints
        // TODO: this may need to occur within the on_parent_change so that if it's added to another widget during execution,
        //       the placement will be relative on first render
        if ( has_parent() )
        {
            fnx::constraints c;
            // position relative to the parent to maintain position on window resize
            c.add( fill_vert_constraint{ height / _parent->get_height() } );
            c.add( fill_horz_constraint{ width / _parent->get_width() } );
            c.add( relative_vert_constraint{ get_y() / _parent->get_height() } );
            c.add( relative_horz_constraint{ get_x() / _parent->get_width() } );
            set_constraints( c );
            widget::on_parent_change( _parent->get_x(), _parent->get_y(), _parent->get_width(), _parent->get_height() );
        }

        for ( auto& c : _children )
        {
            c->on_parent_change( get_x(), get_y(), get_width(), get_height() );
        }

        _prev_cursor_position = _current_cursor_position;
        return true;
    }
    return widget::do_mouse_move( x, y ) || _modal;
}

bool panel::on_widget_press( const fnx::widget_press_evt& evt )
{
    if ( _resize_btn && evt._src == _resize_btn->get_id() )
    {
        _resize = true;
    }
    else if ( _drag_btn && evt._src == _drag_btn->get_id() )
    {
        _drag = true;
    }
    _prev_cursor_position = _current_cursor_position;
    return false;	// let anyone else handle if the need to
}

bool panel::on_widget_release( const widget_release_evt& evt )
{
    if ( _close_btn && evt._src == _close_btn->get_id() )
    {
        hide();
    }
    else if ( _resize_btn && evt._src == _resize_btn->get_id() )
    {
        _resize = false;
    }
    else if ( _drag_btn && evt._src == _drag_btn->get_id() )
    {
        _drag = false;
    }
    return false;	// let anyone else handle if the need to
}

bool panel::do_mouse_exit()
{
    if ( _auto_close && !_modal )
    {
        hide();
        return true;
    }
    return fnx::widget::do_mouse_exit();
};

bool panel::do_key_release( FNX_KEY key )
{
    return _modal || widget::do_key_release( key );
}

bool panel::do_mouse_press( FNX_BUTTON btn, double x, double y )
{
    return _modal || widget::do_mouse_press( btn, x, y );
}

bool panel::do_mouse_release( FNX_BUTTON btn, double x, double y )
{
    return _modal || widget::do_mouse_release( btn, x, y );
}
}