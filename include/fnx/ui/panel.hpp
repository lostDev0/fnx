#pragma once

namespace fnx
{
class panel : public fnx::collapsable
{
public:
    panel( fnx::widget_handle content_container,
           fnx::widget_handle minimize_button = nullptr,
           fnx::widget_handle close_button = nullptr,
           fnx::widget_handle resize_button = nullptr,
           fnx::widget_handle drag_button = nullptr,
           bool collapsed = false,
           bool modal = false,
           bool auto_close = false,
           const std::string& name = create_type_name<fnx::panel>() )
        : fnx::collapsable( content_container, minimize_button, collapsed, name )
        , _modal{modal}
        , _auto_close{auto_close}
        , _close_btn{close_button}
        , _resize_btn{resize_button}
        , _drag_btn{drag_button}
    {
        auto [events, _] = singleton<event_manager>::acquire();
        events.subscribe<widget_release_evt>( fnx::bind( *this, &panel::on_widget_release ) );
        events.subscribe<widget_press_evt>( fnx::bind( *this, &panel::on_widget_press ) );
    };

    virtual ~panel()
    {
        auto [events, _] = singleton<event_manager>::acquire();
        events.unsubscribe<widget_release_evt>( fnx::bind( *this, &panel::on_widget_release ) );
        events.unsubscribe<widget_press_evt>( fnx::bind( *this, &panel::on_widget_press ) );
    };

    virtual void render( camera_handle camera, matrix4x4 parent_matrix ) override
    {
        auto [win, _1] = singleton<window>::acquire();
        auto [renderer, _2] = singleton<renderer>::acquire();
        auto origin = fnx::vector3{ get_x(), get_y(), 0.f };
        parent_matrix = fnx::matrix_translate( parent_matrix, origin );
        // origin now has the global position in opengl coordinates but the clipping region is in screen pixels
        auto origin_screen = win.opengl_to_cartesian( origin.x, origin.y );

        renderer.set_clipping( static_cast<int>( origin_screen.x ),
                               static_cast<int>( origin_screen.y ),
                               static_cast<int>( get_width() * ( win.width() / 2.f ) ),
                               static_cast<int>( get_height() * ( win.height() / 2.f ) ) );
        widget::render( camera, parent_matrix );
        renderer.reset_clipping();
    }

protected:

    /// @brief Checks to see if its drag button is pressed.
    bool on_widget_press( const fnx::widget_press_evt& evt );

    /// @brief Checks to see if its drag button is released.
    bool on_widget_release( const widget_release_evt& evt );

    virtual bool do_mouse_exit() override;
    virtual bool do_mouse_move( double x, double y ) override;
    virtual bool do_key_release( FNX_KEY key ) override;
    virtual bool do_mouse_press( FNX_BUTTON btn, double x, double y ) override;
    virtual bool do_mouse_release( FNX_BUTTON btn, double x, double y ) override;

    bool _drag{ false };				/// currently performing drag
    bool _resize{ false };				/// currently performing resize
    bool _modal{ false };				/// should all events be absorbed while open (only allow setting at construction)
    bool _auto_close{ false };			/// should the widget close when the cursor leaves
    widget_handle _close_btn;
    widget_handle _resize_btn;
    widget_handle _drag_btn;
    fnx::vector2 _current_cursor_position;
    fnx::vector2 _prev_cursor_position;
};
}