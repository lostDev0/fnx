namespace fnx
{
layer_stack::layer_stack()
    : _layers()
{
    auto [events, _] = singleton<event_manager>::acquire();
    events.subscribe<fnx::window_init_evt>( fnx::bind( *this, &layer_stack::send_event_to_layers<fnx::window_init_evt> ) );
    events.subscribe<fnx::window_close_evt>( fnx::bind( *this,
            &layer_stack::send_event_to_layers<fnx::window_close_evt> ) );
    events.subscribe<fnx::window_resize_evt>( fnx::bind( *this,
            &layer_stack::send_event_to_layers<fnx::window_resize_evt> ) );
    events.subscribe<fnx::window_move_evt>( fnx::bind( *this, &layer_stack::send_event_to_layers<fnx::window_move_evt> ) );
    events.subscribe<fnx::window_minimize_evt>( fnx::bind( *this,
            &layer_stack::send_event_to_layers<fnx::window_minimize_evt> ) );
    events.subscribe<fnx::window_maximize_evt>( fnx::bind( *this,
            &layer_stack::send_event_to_layers<fnx::window_maximize_evt> ) );
    events.subscribe<fnx::window_fullscreen_evt>( fnx::bind( *this,
            &layer_stack::send_event_to_layers<fnx::window_fullscreen_evt> ) );
    events.subscribe<fnx::window_lose_focus_evt>( fnx::bind( *this,
            &layer_stack::send_event_to_layers<fnx::window_lose_focus_evt> ) );
    events.subscribe<fnx::window_gain_focus_evt>( fnx::bind( *this,
            &layer_stack::send_event_to_layers<fnx::window_gain_focus_evt> ) );
    events.subscribe<fnx::keyboard_press_evt>( fnx::bind( *this,
            &layer_stack::send_event_to_layers<fnx::keyboard_press_evt> ) );
    events.subscribe<fnx::keyboard_release_evt>( fnx::bind( *this,
            &layer_stack::send_event_to_layers<fnx::keyboard_release_evt> ) );
    events.subscribe<fnx::keyboard_repeat_evt>( fnx::bind( *this,
            &layer_stack::send_event_to_layers<fnx::keyboard_repeat_evt> ) );
    events.subscribe<fnx::mouse_enter_evt>( fnx::bind( *this, &layer_stack::send_event_to_layers<fnx::mouse_enter_evt> ) );
    events.subscribe<fnx::mouse_exit_evt>( fnx::bind( *this, &layer_stack::send_event_to_layers<fnx::mouse_exit_evt> ) );
    events.subscribe<fnx::mouse_move_evt>( fnx::bind( *this, &layer_stack::send_event_to_layers<fnx::mouse_move_evt> ) );
    events.subscribe<fnx::mouse_press_evt>( fnx::bind( *this, &layer_stack::send_event_to_layers<fnx::mouse_press_evt> ) );
    events.subscribe<fnx::mouse_release_evt>( fnx::bind( *this,
            &layer_stack::send_event_to_layers<fnx::mouse_release_evt> ) );
    events.subscribe<fnx::mouse_scroll_evt>( fnx::bind( *this,
            &layer_stack::send_event_to_layers<fnx::mouse_scroll_evt> ) );

    events.subscribe<fnx::update_evt>( fnx::bind( *this, &layer_stack::update_layers ) );
    events.subscribe<fnx::render_user_interface_evt>( fnx::bind( *this, &layer_stack::render_layers ) );
}

layer_stack::~layer_stack() {}

bool layer_stack::add_layer( layer_handle layer )
{
    if ( layer != nullptr )
    {
        _layers[_layers.size()] = layer;
        return true;
    }
    return false;
}

bool layer_stack::insert_layer( size_t loc, layer_handle layer )
{
    // only adding the layer if it is above the entity layer and not already added
    if ( layer != nullptr && _layers[loc] == nullptr )
    {
        _layers[loc] = layer;
        return true;
    }
    return false;
}

bool layer_stack::remove_layer( const std::string& layer_name )
{
    auto it = _layers.begin();
    auto it_end = _layers.end();
    for ( ; it != it_end; ++it )
    {
        if ( it->second->get_name() == layer_name )
        {
            _layers.erase( it );
            return true;
        }
    }
    return false;
}

bool layer_stack::update_layers( const fnx::update_evt& evt )
{
    if ( evt._action == update_evt::action_t::start )
    {
        for ( auto& layer_pair : _layers )
        {
            layer_pair.second->update( evt._delta_in_seconds );
        }
    }
    return false;
}

bool layer_stack::render_layers( const fnx::render_user_interface_evt& evt )
{
    if ( evt._action == render_user_interface_evt::action_t::start )
    {
        auto camera = singleton<camera_manager>::acquire().data.get( camera_manager::ui );
        for ( auto& layer_pair : _layers )
        {
            layer_pair.second->render( camera );
        }
    }
    return false;
}
}
