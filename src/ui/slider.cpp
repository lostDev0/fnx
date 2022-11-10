namespace fnx
{
bool slider::do_mouse_press( FNX_BUTTON button, double x, double y )
{
    // someone absorbed it
    if ( _foreground && _anchor && _mouse_over )
    {
        auto direction = _foreground->get_fill_direction();
        // background absorbed it, jump progress to point
        if ( direction == fill_direction::left_to_right )
        {
            auto relative_x = x - _foreground->get_abs_x();
            auto progress = ( relative_x / _foreground->get_width() );
            fnx::maximum( progress, 0.0 );
            fnx::minimum( 1.0, progress );
            _foreground->set_progress( static_cast<float>( progress ) );
        }
        else if ( direction == fill_direction::bottom_to_top )
        {
            auto relative_y = y - _foreground->get_abs_y();
            auto progress = ( relative_y / _foreground->get_height() );
            fnx::maximum( progress, 0.0 );
            fnx::minimum( 1.0, progress );
            _foreground->set_progress( static_cast<float>( progress ) );
        }
        else if ( direction == fill_direction::right_to_left )
        {
            auto relative_x = x - _foreground->get_abs_x();
            auto progress = 1.f - ( relative_x / _foreground->get_width() );
            fnx::maximum( progress, 0.0 );
            fnx::minimum( 1.0, progress );
            _foreground->set_progress( static_cast<float>( progress ) );
        }
        else if ( direction == fill_direction::top_to_bottom )
        {
            auto relative_y = y - _foreground->get_abs_y();
            auto progress = 1.f - ( relative_y / _foreground->get_height() );
            fnx::maximum( progress, 0.0 );
            fnx::minimum( 1.0, progress );
            _foreground->set_progress( static_cast<float>( progress ) );
        }
        activate();
        return true;
    }
    return false;
}

bool slider::do_mouse_release( FNX_BUTTON button, double x, double y )
{
    if ( _foreground && is_active() )
    {
        widget_progress_evt evt;
        evt._progress = _foreground->get_progress();
        FNX_EMIT( evt );
        inactivate();
        return true;
    }
    return false;
}

bool slider::do_mouse_move( double x, double y )
{
    if ( _anchor && _foreground )
    {
        if ( _anchor->is_pressed() )
        {
            auto direction = _foreground->get_fill_direction();
            if ( direction == fill_direction::left_to_right )
            {
                auto relative_x = x - _foreground->get_abs_x();
                auto progress = ( relative_x / _foreground->get_width() );
                fnx::maximum( progress, 0.0 );
                fnx::minimum( 1.0, progress );
                _foreground->set_progress( static_cast<float>( progress ) );
            }
            else if ( direction == fill_direction::bottom_to_top )
            {
                auto relative_y = y - _foreground->get_abs_y();
                auto progress = ( relative_y / _foreground->get_height() );
                fnx::maximum( progress, 0.0 );
                fnx::minimum( 1.0, progress );
                _foreground->set_progress( static_cast<float>( progress ) );
            }
            else if ( direction == fill_direction::right_to_left )
            {
                auto relative_x = x - _foreground->get_abs_x();
                auto progress = 1.f - ( relative_x / _foreground->get_width() );
                fnx::maximum( progress, 0.0 );
                fnx::minimum( 1.0, progress );
                _foreground->set_progress( static_cast<float>( progress ) );
            }
            else if ( direction == fill_direction::top_to_bottom )
            {
                auto relative_y = y - _foreground->get_abs_y();
                auto progress = 1.f - ( relative_y / _foreground->get_height() );
                fnx::maximum( progress, 0.0 );
                fnx::minimum( 1.0, progress );
                _foreground->set_progress( static_cast<float>( progress ) );
            }
            return true;
        }
    }
    return widget::do_mouse_move( x, y );
}

void slider::update( double seconds )
{
    if ( _anchor && _foreground )
    {
        auto progress = _foreground->get_progress();
        auto direction = _foreground->get_fill_direction();
        constraints anchor_constraint;

        if ( direction == fill_direction::left_to_right )
        {
            anchor_constraint.add( fnx::center_vert_constraint() );
            anchor_constraint.add( fnx::relative_horz_constraint( progress - ( ( _anchor->get_width() / _foreground->get_width() ) /
                                   2.f ) ) );
        }
        else if ( direction == fill_direction::bottom_to_top )
        {
            anchor_constraint.add( fnx::center_horz_constraint() );
            anchor_constraint.add( fnx::relative_vert_constraint( progress - ( ( _anchor->get_height() /
                                   _foreground->get_height() ) / 2.f ) ) );
        }
        else if ( direction == fill_direction::right_to_left )
        {
            anchor_constraint.add( fnx::center_vert_constraint() );
            anchor_constraint.add( fnx::relative_horz_constraint( ( 1.f - progress ) - ( ( _anchor->get_width() /
                                   _foreground->get_width() ) / 2.f ) ) );
        }
        else if ( direction == fill_direction::top_to_bottom )
        {
            anchor_constraint.add( fnx::center_horz_constraint() );
            anchor_constraint.add( fnx::relative_vert_constraint( ( 1.f - progress ) - ( ( _anchor->get_height() /
                                   _foreground->get_height() ) / 2.f ) ) );
        }

        _anchor->set_constraints( anchor_constraint );
        _anchor->on_parent_change( get_x(), get_y(), get_width(), get_height() );
    }
    widget::update( seconds );
}
}