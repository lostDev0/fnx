#pragma once

namespace fnx
{
class collapsable : public fnx::widget
{
public:
    /// @brief Create a collapsable container with no title or icon.
    collapsable( fnx::widget_handle content, fnx::widget_handle icon = nullptr, bool collapsed = true,
                 const std::string& name = create_type_name<fnx::collapsable>() )
        : widget( name )
        , _icon( icon )
        , _content( content )
        , _collapsed( collapsed )
    {
        auto [events, _] = singleton<event_manager>::acquire();
        events.subscribe<widget_release_evt>( fnx::bind( *this, &collapsable::on_widget_release ) );
        if ( collapsed && _content )
        {
            _content->set_visibility( false );
            if ( _icon )
            {
                _icon->check();
            }
        }
        else if ( !collapsed && _content )
        {
            _content->set_visibility( true );
            if ( _icon )
            {
                _icon->uncheck();
            }
        }
    }

    virtual ~collapsable()
    {
        auto [events, _] = singleton<event_manager>::acquire();
        events.unsubscribe<widget_release_evt>( fnx::bind( *this, &collapsable::on_widget_release ) );
    }

    /// @brief Expands the widget.
    /// @note Unchecks icon and shows content container.
    void expand()
    {
        if ( _content )
        {
            _content->show();
        }
        _collapsed = false;
    }

    /// @brief Collapses the widget.
    /// @note Checks icon and hides content container.
    void collapse()
    {
        if ( _content )
        {
            _content->hide();
        }
        _collapsed = true;
    }

protected:
    fnx::widget_handle _icon{ nullptr };
    fnx::widget_handle _content{ nullptr };
    bool _collapsed{ false };

    bool on_widget_release( const widget_release_evt& evt )
    {
        if ( _icon && evt._src == _icon->get_id() )
        {
            if ( _collapsed )
            {
                expand();
            }
            else
            {
                collapse();
            }
        }
        return false;
    }
private:
};
}