#pragma once

namespace fnx
{
/// @brief Children are clipped to a viewport and scrolling bars are provided for moving the content
/// @note The scroll view widget is not meant to provide a background of it's own.
class scroll_view : public fnx::widget
{
public:
    scroll_view( std::shared_ptr<fnx::slider> horz, std::shared_ptr<fnx::slider> vert,
                 const std::string& name = create_type_name<scroll_view>() )
        : fnx::widget( name )
        , _vert_scroll{vert}
        , _horz_scroll{horz}
    {
        // TODO register for the progress update from the slider
    }
    virtual ~scroll_view() = default;

    /// @brief Show the scroll bars even when there is no scroll allowed.
    void auto_hide_scroll_bars( double timeout_in_seconds )
    {
        _always_show_bars = false;
        _timeout = timeout_in_seconds;
    }

    /// @brief Render all children clipped to the viewable region.
    /// @note This will not render the scroll bars.
    virtual void render( camera_handle camera, matrix4x4 parent_matrix ) override;

    virtual void update( double delta ) override;

protected:
    bool _always_show_bars{ true };				/// never timeout the scroll bars
    bool _triggered_hide{ false };				/// flag indicating that a hide was requested
    double _timeout{ 0.0 };						/// how many seconds until auto fade for scroll bars
    double _accumulator{ -1.0 };				/// how many seconds have passed since the cursor exited the widget
    fnx::constraints _viewport_constraints;		/// constraints that determine how the viewport will be aligned
    vector4 _children_bounds;					/// furthest points the children span.
    matrix4x4 _child_translation_matrix;		/// tranlation matrix for the children when rendering
    std::shared_ptr<fnx::slider> _vert_scroll{ nullptr };
    std::shared_ptr<fnx::slider> _horz_scroll{ nullptr };

    virtual bool do_mouse_enter() override
    {
        _send_events_to_children = true;
        return false;
    }

    virtual bool do_mouse_exit() override
    {
        _send_events_to_children = false;
        // tell the children that may consider the cursor over them, to be their normal state
        for ( auto& c : _children )
        {
            if ( c->is_active() )
            {
                c->inactivate();
            }
            if ( c->is_cursor_over() )
            {
                c->do_mouse_exit();
            }
        }
        return false;
    }

private:
};
}