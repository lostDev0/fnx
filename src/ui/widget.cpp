namespace fnx
{
/*
#define SEND_TO_CHILDREN(_func_, ...) bool result = false; \
auto it_end = _children.rend(); \
for (auto it = _children.rbegin(); it != it_end; ++it) \
{ \
	auto w = *it; \
	if (w && w->is_visible() && w->is_interactive()) \
	{ \
		result |= w->_func_(__VA_ARGS__); \
		if (result) \
		{ \
			break; \
		} \
	} \
} \
return result
*/

#define SEND_TO_CHILDREN(_func_, ...) bool child_absorbed = false; \
    widget_handle notify_child; \
    auto it_end = _children.rend(); \
    for (auto it = _children.rbegin(); _send_events_to_children && it != it_end; ++it) \
    { \
        auto w = *it; \
        if (w && w->is_visible() && w->is_interactive()) \
        { \
            child_absorbed |= w->_func_(__VA_ARGS__); \
            if (child_absorbed) \
            { \
                notify_child = w; \
                break; \
            } \
        } \
    } \

widget::widget( const std::string& name, widget_type type )
    : _type { type}
    , _name{ name }
{
    static unsigned int handle = 0u;
    _id = handle++;
    set_constraints( fill_parent );
}

bool widget::on_event( const mouse_move_evt& event )
{
    bool result = false;
    vector2 cursor_pos{ event._gl_x, event._gl_y };
    // TODO : each widget should store the parent's matrix or inverse matrix to do hit checks
    auto bounds = rect{ get_abs_x(), get_abs_y(), get_abs_x() + get_width(), get_abs_y() + get_height() };
    if ( _visible && _interactive )
    {
        if ( within( bounds, cursor_pos ) )
        {
            if ( !_mouse_over )
            {
                FNX_DEBUG( fnx::format_string( "entered: %s", _name ) );
                // transitioned to hover state
                _mouse_over = true;
                result |= do_mouse_enter();

                // apply any transitions
                _animator.on_mouse_enter();
            }
        }
        else
        {
            if ( _mouse_over )
            {
                FNX_DEBUG( fnx::format_string( "exited: %s", _name ) );
                // transitioned out of hover state
                _mouse_over = false;
                result |= do_mouse_exit();
                _animator.on_mouse_exit();
            }
        }
        result |= do_mouse_move( event._gl_x, event._gl_y );
        if ( !result && _send_events_to_children )
        {
            // process children for the move after the parent to know if the parent is visible and interactive, etc
            SEND_TO_CHILDREN( on_event, event );
            result = child_absorbed;
            if ( result )
            {
                notify_mouse_move_absorbed( notify_child );
            }
        }
    }
    return result;
}

bool widget::on_event( const mouse_press_evt& event )
{
    bool result = false;
    SEND_TO_CHILDREN( on_event, event );
    result = child_absorbed;
    if ( !result )
    {
        // no child handled it and we are over this widget
        // TODO do i need to check mouse over?
        result = do_mouse_press( event._btn, event._gl_x, event._gl_y );
        if ( result )
        {
            FNX_DEBUG( fnx::format_string( "pressed: %s", _name ) );
            _animator.do_mouse_press();
        }
    }
    else
    {
        notify_mouse_press_absorbed( notify_child );
    }
    return result;
}

bool widget::on_event( const mouse_release_evt& event )
{
    bool result = false;
    SEND_TO_CHILDREN( on_event, event );
    result = child_absorbed;
    if ( !result )
    {

        result = do_mouse_release( event._btn, event._gl_x, event._gl_y );
        if ( result )
        {
            FNX_DEBUG( fnx::format_string( "released: %s", _name ) );
            _animator.do_mouse_release();
        }
    }
    else
    {
        notify_mouse_release_absorbed( notify_child );
    }
    return result;
}

bool widget::on_event( const keyboard_press_evt& event )
{
    bool result = false;
    SEND_TO_CHILDREN( on_event, event );
    result = child_absorbed;
    if ( !result )
    {
        result = do_key_press( event._key );
        if ( result )
        {
            FNX_DEBUG( fnx::format_string( "keyboard press absorbed: %s", _name ) );
        }
    }
    else
    {
        notify_key_press_absorbed( notify_child );
    }
    return result;
}

bool widget::on_event( const keyboard_release_evt& event )
{
    bool result = false;
    SEND_TO_CHILDREN( on_event, event );
    result = child_absorbed;
    if ( !result )
    {
        result = do_key_release( event._key );
        if ( result )
        {
            FNX_DEBUG( fnx::format_string( "keyboard release absorbed: %s", _name ) );
        }
    }
    else
    {
        notify_key_release_absorbed( notify_child );
    }
    return result;
}

bool widget::on_event( const keyboard_repeat_evt& event )
{
    bool result = false;
    SEND_TO_CHILDREN( on_event, event );
    result = child_absorbed;
    if ( !result )
    {
        result = do_key_repeat( event._key );
        if ( result )
        {
            FNX_DEBUG( fnx::format_string( "keyboard repeat absorbed: %s", _name ) );
        }
    }
    else
    {
        notify_key_repeat_absorbed( notify_child );
    }
    return result;
}

bool widget::on_event( const mouse_scroll_evt& event )
{
    bool result = false;
    SEND_TO_CHILDREN( on_event, event );
    result = child_absorbed;
    if ( !result )
    {
        result = do_scroll( event._x, event._y );
        if ( result )
        {
            FNX_DEBUG( fnx::format_string( "mouse scroll absorbed: %s", _name ) );
        }
    }
    else
    {
        notify_scroll_absorbed( notify_child );
    }
    return result;
}

void widget::on_parent_change( float parent_x, float parent_y, float parent_width, float parent_height )
{
    _original_bounds = _constraints.apply( vector4{ parent_x, parent_y, parent_width, parent_height }, _current_bounds );
    _current_bounds = _original_bounds;

    _animator.update( 0, *this );

    // update self then tell children to update
    for ( auto& c : _children )
    {
        c->on_parent_change( get_x(), get_y(), get_width(), get_height() );
    }
}

/// @brief Returns the children's furthest coordinates on the xy axis.
/// @note 0 = left, 1 = top, 2 = right, 3 = bottom
void widget::get_children_bounds( vector4& bounds ) const
{
    for ( const auto& c : _children )
    {
        auto left = c->get_x();
        auto bottom = c->get_y();
        auto right = left + c->get_width();
        auto top = bottom + c->get_height();
        if ( left < bounds.x )
        {
            // left
            bounds.x = ( left );
        }
        if ( right > bounds.z )
        {
            // right
            bounds.z = ( right );
        }
        if ( top > bounds.y )
        {
            // top
            bounds.y = ( top );
        }
        if ( bottom < bounds.w )
        {
            bounds.w = ( bottom );
        }

        if ( c->has_children() )
        {
            c->get_children_bounds( bounds );
        }
    }
}

void widget::update( double delta )
{
    _animator.update( delta, *this );
    for ( auto& w : _children )
    {
        if ( w->is_visible() )
        {
            w->update( delta );
        }
    }
}

void widget::render( camera_handle camera, matrix4x4 parent_matrix )
{
    if ( is_visible() )
    {
        parent_matrix = matrix_translate( parent_matrix, get_x(), get_y(), 0.f );
        for ( auto& w : _children )
        {
            if ( w->is_visible() )
            {
                w->render( camera, parent_matrix );
            }
        }
    }
}

void widget::add_widget( fnx::widget_handle widget )
{
    if ( widget )
    {
        _children.emplace_back( widget );
        widget->set_parent( *this );
    }
}

std::unordered_map<widget_id, widget_handle>& get_widget_map()
{
    static std::unordered_map<widget_id, widget_handle> map;
    return map;
}

widget_handle get_widget_by_id( widget_id id )
{
    return get_widget_map()[id];
}

void widget::uncheck()
{
    if ( is_checkable() && is_checked() )
    {
        _state = state::normal;
        _checked = false;
    }
};

void widget::check()
{
    if ( is_checkable() && !is_checked() )
    {
        _state = state::checked;
        _checked = true;
    }
};

void widget::activate()
{
    if ( !is_active() )
    {
        _active = true;
        widget_active_evt e{ _id };
        FNX_EMIT( e );
        _animator.do_active();
        do_active();
    }
}

void widget::inactivate()
{
    if ( is_active() )
    {
        _active = false;
        widget_inactive_evt e{ _id };
        FNX_EMIT( e );
        _animator.do_inactive();
        do_inactive();
    }
}

widget_handle widget::get_widget( const std::string& name )
{
    for ( auto child : _children )
    {
        if ( child->get_name() == name )
        {
            return child;
        }
        else if ( child->has_children() )
        {
            auto ret = child->get_widget( name );
            if ( ret )
            {
                return ret;
            }
        }
    }
    return nullptr;
}
}
