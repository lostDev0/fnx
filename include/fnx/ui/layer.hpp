#pragma once

namespace fnx
{
class layer_stack;
/// @brief A single hierarchy of widgets to be rendered and processed in the order they were added.
/// @note It is the expectation that entities are never removed and only destroyed when the layer is destroyed.
class layer
{
    friend layer_serializer;
    bool _visible{ false };
    std::string _name;  /// identifier of this layer
    fnx::widget_handle _root{fnx::create_widget<fnx::widget>()};		/// top level widget container that has no visual representation
    bool _dirty_cache{ true };	/// flag indicating the widgets need to be redrawn
public:
    layer( const std::string& name = create_type_name<fnx::layer>() );
    virtual ~layer();

    /// @brief Process an update cycle from the engine.
    /// @param[in] delta : difference between cycles in seconds
    virtual void update( double delta );

    /// @brief Process a render cycle from the engine.
    virtual void render( camera_handle camera );

    /// @brief Returns the name of this layer.
    /// @return std::string
    inline auto get_name() const
    {
        return _name;
    }

    /// @brief Returns whether the layer is visible or not.
    /// @return bool : true if visible
    inline auto is_visible() const
    {
        return _visible;
    }

    /// @brief Make the layer visible.
    inline void show()
    {
        //layer::show();
        // pass event to all children
        _root->activate();
        _root->show();
    }

    /// @brief Make the layer invisible.
    inline void hide()
    {
        if ( _root->is_visible() )
        {
            // pass event to all children
            _root->hide();
            _root->inactivate();
        }
    }

    /// @brief Add a widget to the layer.
    /// @note cannot remove a widget from the layer
    widget_handle add_widget( widget_handle w );

    /// @brief Returns nullptr or a widget handle to the given widget name.
    widget_handle get_widget( const std::string& widget_name );

    auto get_root() const
    {
        return _root;
    }

    virtual bool on_event( const window_init_evt& event );
    virtual bool on_event( const window_close_evt& event );
    virtual bool on_event( const window_resize_evt& event );
    virtual bool on_event( const window_move_evt& event );
    virtual bool on_event( const window_minimize_evt& event );
    virtual bool on_event( const window_maximize_evt& event );
    virtual bool on_event( const window_fullscreen_evt& event );
    virtual bool on_event( const window_lose_focus_evt& event );
    virtual bool on_event( const window_gain_focus_evt& event );
    virtual bool on_event( const keyboard_press_evt& event );
    virtual bool on_event( const keyboard_release_evt& event );
    virtual bool on_event( const keyboard_repeat_evt& event );
    virtual bool on_event( const mouse_enter_evt& event );
    virtual bool on_event( const mouse_exit_evt& event );
    virtual bool on_event( const mouse_move_evt& event );
    virtual bool on_event( const mouse_press_evt& event );
    virtual bool on_event( const mouse_release_evt& event );
    virtual bool on_event( const mouse_scroll_evt& event );

protected:

    template<typename event_type>
    bool send_event_to_widgets( const event_type& evt )
    {
        bool ret = false;
        auto reverse_it = _root->get_children().rbegin();
        auto reverse_end = _root->get_children().rend();
        for ( ; reverse_it != reverse_end; ++reverse_it )
        {
            // if you need an "invisible" widget, make the appearance invisible but don't set the visible flag to false
            if ( ( *reverse_it )->is_interactive() && ( *reverse_it )->is_visible() )
            {
                ret |= ( *reverse_it )->on_event( evt );
                if ( ret )
                {
                    break;
                }
            }
        }
        return ret;
    }

    bool on_widget_active( const widget_active_evt& );
    bool on_widget_inactive( const widget_inactive_evt& );

    /// TODO this may need to create widget ids that are only ever increasing
    unsigned int _active_widget{ 0xffffffff };
};

using layer_handle = fnx::reference_ptr<fnx::layer>;
}