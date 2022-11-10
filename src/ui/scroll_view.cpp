namespace fnx
{
void scroll_view::render( camera_handle camera, matrix4x4 parent_matrix )
{
    auto [win, _0] = singleton<fnx::window>::acquire();
    auto [renderer, _1] = singleton<fnx::renderer>::acquire();

    auto origin = vector3{ get_x(), get_y(), 0.f };
    parent_matrix = matrix_translate( parent_matrix, origin );
    // origin now has the global position in opengl coordinates but the clipping region is in screen pixels
    auto origin_screen = win.opengl_to_cartesian( origin.x, origin.y );

    renderer.set_clipping( static_cast<int>( origin_screen.x ),
                           static_cast<int>( origin_screen.y ),
                           static_cast<int>( get_width() * ( win.width() / 2.f ) ),
                           static_cast<int>( get_height() * ( win.height() / 2.f ) ) );
    widget::render( camera, parent_matrix * _child_translation_matrix );
    renderer.reset_clipping();
}

void scroll_view::update( double delta )
{
    fnx::widget::update( delta );
    vector3 translate;
    matrix4x4 mat;

    // calculate the bounds of all widgets
    _children_bounds = vector4{};	// reset before sending to children
    // @note 0 = left, 1 = top, 2 = right, 3 = bottom
    get_children_bounds( _children_bounds );	// now updated with child boundaries

    auto child_height = _children_bounds.y - _children_bounds.w;
    auto child_width = _children_bounds.z - _children_bounds.x;

    if ( !_mouse_over )
    {
        _accumulator += delta;
        if ( !_always_show_bars && _accumulator > _timeout && !_triggered_hide )
        {
            _triggered_hide = true;
            if ( _horz_scroll && _horz_scroll->is_visible() )
            {
                _horz_scroll->hide();
            }
            if ( _vert_scroll && _vert_scroll->is_visible() )
            {
                _vert_scroll->hide();
            }
        }
    }
    else
    {
        _triggered_hide = false;
        _accumulator = 0.0;	// start the hide timer
        if ( _horz_scroll && child_width > get_width() )
        {
            _horz_scroll->show();
        }
        if ( _vert_scroll && child_height > get_height() )
        {
            _vert_scroll->show();
        }
    }

    if ( child_width <= get_width() )
    {
        if ( !_always_show_bars )
        {
            if ( _horz_scroll )
            {
                _horz_scroll->set_visibility( false );
            }
        }
        else if ( _horz_scroll )
        {
            _horz_scroll->disable_events();
        }
    }

    if ( child_height <= get_height() )
    {
        if ( !_always_show_bars )
        {
            if ( _vert_scroll )
            {
                _vert_scroll->set_visibility( false );
            }
        }
        else if ( _vert_scroll )
        {
            _vert_scroll->disable_events();
        }
    }

    if ( _vert_scroll && fill_direction::top_to_bottom == _vert_scroll->get_fill_direction() )
    {
        translate.y = ( _children_bounds.w - child_height + get_height() );
        // calculate the vertical progress
        auto progress = _vert_scroll->get_progress();
        auto dy = ( child_height - get_height() ) * progress;
        translate.y = ( translate.y + dy );
    }
    else if ( _vert_scroll && fill_direction::bottom_to_top == _vert_scroll->get_fill_direction() )
    {
        translate.y = ( _children_bounds.w );
        // calculate the vertical progress
        auto progress = _vert_scroll->get_progress();
        auto dy = ( child_height - get_height() ) * progress;
        translate.y = ( translate.y - dy );
    }
    if ( _horz_scroll && fill_direction::left_to_right == _horz_scroll->get_fill_direction() )
    {
        translate.x = ( _children_bounds.x );	// left
        // calculate the horizontal progress
        auto progress = _horz_scroll->get_progress();
        auto dx = ( child_width - get_width() ) * progress;
        translate.x = ( translate.x - dx );
    }
    else if ( _horz_scroll && fill_direction::right_to_left == _horz_scroll->get_fill_direction() )
    {
        translate.x = ( _children_bounds.x - child_width + get_width() );
        // calculate the horizontal progress
        auto progress = _horz_scroll->get_progress();
        auto dx = ( child_width - get_width() ) * progress;
        translate.x = ( translate.x + dx );
    }

    _child_translation_matrix = fnx::matrix_translate( mat, translate );
}
}