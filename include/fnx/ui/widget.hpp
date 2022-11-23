#pragma once

namespace fnx
{
class layer_serializer;
enum class widget_type
{
    widget = 0,
    block,
    button,
    clipping_container,
    image,
    label,
    line,
    mutually_exclusive_container,
    panel,
    progress_bar,
    radio,
    scroll_view,
    sizer,
    slider,
    tabbed_notebook,
    text_box,
    text_input
};


class widget;
template<typename T>
using widget_handle_t = fnx::reference_ptr<T>;
using widget_handle = fnx::widget_handle_t<fnx::widget>;
using widget_id = unsigned int;

/// @brief User Interface main interface to rendering layers.
class widget
{
public:
    enum class state
    {
        normal = 0,
        hover,
        press,
        checked,
        max
    };

    enum class fill_direction
    {
        top_to_bottom = 0,
        left_to_right = 1,
        outer_to_inner = 2,	// not applicable to progress bar direction
        bottom_to_top = 3,
        right_to_left = 4,	// not applicable to progress bar direction
        circle = 5,	// TODO
        max
    };

    /// @brief Create a widget with an optional name.
    widget( const std::string& name = fnx::create_type_name<fnx::widget>(), widget_type type = widget_type::widget );
    virtual ~widget() = default;

    /// @brief Get the widget local x coordinate in world space (opengl).
    auto get_x() const
    {
        return _current_bounds.x;
    }

    /// @brief Get the widget local y coordinate in world space (opengl).
    auto get_y() const
    {
        return _current_bounds.y;
    }

    /// @brief Get the widget width in world space (opengl).
    auto get_width() const
    {
        return _current_bounds.z;
    }

    /// @brief Get the widget height coordinate in world space (opengl).
    auto get_height() const
    {
        return _current_bounds.w;
    }

    /// @brief Get the widget name.
    const std::string& get_name() const
    {
        return _name;
    }
    // TODO set_name should probably be a friend class to serialize namespace
    void set_name( const std::string& name )
    {
        _name = name;
    }

    /// @brief Get the widget global x coordinate in world space (opengl).
    auto get_abs_x() -> reactphysics3d::decimal
    {
        return nullptr == _parent ? get_x() : ( _parent->get_abs_x() + get_x() );
    }

    /// @brief Get the widget global y coordinate in world space (opengl).
    auto get_abs_y() -> reactphysics3d::decimal
    {
        return nullptr == _parent ? get_y() : ( _parent->get_abs_y() + get_y() );
    }

    /// @brief Set the widget local x,y coordinate.
    void set_position( const fnx::vector2& position )
    {
        _current_bounds.x = position.x;
        _current_bounds.y = position.y;
    }

    /// @brief Set the widget local x,y coordinate.
    void set_position( reactphysics3d::decimal x, reactphysics3d::decimal y )
    {
        _current_bounds.x = x;
        _current_bounds.y = y;
    }

    /// @brief Set the widget width and height.
    void set_size( const fnx::vector2& size )
    {
        _current_bounds.z = size.x;
        _current_bounds.w = size.y;
    }

    /// @brief Set the widget width and height.
    void set_size( reactphysics3d::decimal width, reactphysics3d::decimal height )
    {
        _current_bounds.z = width;
        _current_bounds.w = height;
    }

    /// @brief Set the strict size.
    /// @note This is used for sizers to determine the expected size.
    void set_strict_size( reactphysics3d::decimal width, reactphysics3d::decimal height )
    {
        _strict_size = fnx::vector2{ width, height };
    }

    /// @brief Return the strict size.
    /// @note This is used for sizers to determine if something is auto sizeable or has a preferred size.
    const auto& get_strict_size()
    {
        return _strict_size;
    }

    /// @brief Set the widget's alpha transparency.
    /// @note Primarily used for transitions.
    void set_alpha( reactphysics3d::decimal alpha )
    {
        _current_alpha = alpha;
    }

    /// @brief Get this widget's current alpha.
    auto get_alpha()
    {
        return _current_alpha;
    }

    /// @brief Gets the position without animator manipulation
    auto get_original_position() const
    {
        return _original_bounds.xy();
    }

    /// @brief Gets the size without animator manipulation
    auto get_original_size() const
    {
        return _original_bounds.zw();
    }

    /// @brief Gets the position with animator manipulation (slide)
    auto get_current_position() const
    {
        return _current_bounds.xy();
    }

    /// @brief Gets the size with animator manipulation (scale)
    auto get_current_size() const
    {
        return _current_bounds.zw();
    }

    /// @brief Gets the alpha value with animator manipulation (fade)
    auto get_original_alpha() const
    {
        return _original_alpha;
    }

    /// @brief Calculates the farthest boundary of all children widgets.
    void get_children_bounds( fnx::vector4& bounds ) const;

    /// @brief Update the widget as needed during the cycle.
    /// @param[in] delta : delta time in seconds between calls to update
    virtual void update( double delta );

    /// @brief Render this widget.
    /// @param[in] camera : camera to provide render frustrum
    /// @param[in] parent_matrix : transformation matrix provided by the parent
    virtual void render( camera_handle camera, fnx::matrix4x4 parent_matrix );

    /// @brief Set the parent widget making this a child.
    void set_parent( widget& parent )
    {
        _parent = &parent;
    }

    /// @brief Returns whether this widget has a parent.
    auto has_parent()
    {
        return nullptr != _parent;
    }

    auto get_parent()
    {
        return _parent;
    }

    /// @brief Set size and position constraints.
    void set_constraints( const constraints& constraints )
    {
        _constraints = constraints;
    }

    /// @brief Add a constraint to the size and position constraints.
    void add_constraints( const constraints& constraints )
    {
        for ( auto& c : constraints.clone_all() )
        {
            _constraints.add( c );
        }
    }

    /// @brief Reset constraints to be whatever the set position and size are.
    void reset_constraints()
    {
        set_constraints( fnx::constraints{} );
    }

    auto& get_constraints()
    {
        return _constraints;
    }

    /// @brief Access the widget animator.
    auto& get_animator()
    {
        return _animator;
    }

    /// @brief Notify the widget that it's parent dimensions changed.
    virtual void on_parent_change( reactphysics3d::decimal parent_x, reactphysics3d::decimal parent_y,
                                   reactphysics3d::decimal parent_width, reactphysics3d::decimal parent_height );

    /// @brief Starts the on_show behavior
    virtual void show()
    {
        set_visibility( true );
        _animator.on_show();
        for ( auto child : _children )
        {
            if ( child->is_visible() || child->is_hidden() )
            {
                child->set_visibility( true );
                child->get_animator().on_show();
            }
        }
    }

    /// @brief Starts the on_hide behavior
    /// @note doesn't turn visibility off so the hide animation can be shown
    virtual void hide()
    {
        auto wait = 0.0;
        for ( auto child : _children )
        {
            if ( child->is_visible() )
            {
                auto duration = child->get_animator().on_hide();
                wait = fnx::maximum( wait, duration );
            }
        }
        _animator.on_hide( wait );
        // hide animation will turn the widget invisible upon completion
    }

    /// @brief Widget is normally visible, but is currently hidden due to an animation
    auto is_hidden() -> bool
    {
        return _animator_hidden;
    }

    /// @brief Return the widget's visibility.
    /// @note this may not accurately reflect on_show transitions
    auto is_visible() -> bool
    {
        return _visible && !is_hidden();
    }

    /// @brief Enable events to be processed by the widget.
    void enable_events()
    {
        _interactive = true;
    }

    // @brief Prevent events from being handled by the widget.
    void disable_events()
    {
        _interactive = false;
    }

    /// @brief Returns whether the widget is processing events.
    auto is_interactive() -> bool
    {
        return _interactive;
    }

    /// @brief Sets the widget visibility without transitions or events.
    void set_visibility( bool visible )
    {
        _visible = visible;
        if ( _visible )
        {
            _animator_hidden = false;
        }
        if ( !_visible )
        {
            _model.reset();
            _shader.reset();
            _texture.reset();
            _material.reset();
        }
    }

    /// @brief Returns the unique identifier of this widget.
    auto get_id() const
    {
        return _id;
    }

    /// @brief Is the widget currently checked.
    const bool& is_checked() const
    {
        return _checked;
    }
    void set_checked( const bool& val )
    {
        _checked = val;
    }

    /// @brief Can the widget be checked.
    const bool& is_checkable() const
    {
        return _checkable;
    }

    void set_checkable( const bool& checkable )
    {
        _checkable = checkable;
    }

    /// @brief Uncheck the widget.
    virtual void uncheck();

    /// @brief Check the widget.
    virtual void check();

    /// @brief Toggle the checked state if the widget can be checked
    void toggle()
    {
        if ( is_checkable() )
        {
            _checked ? uncheck() : check();
        }
    }

    /// @brief Does the widget have children.
    bool has_children() const
    {
        return !_children.empty();
    }

    /// @brief Get the children of the widget.
    const std::vector<fnx::widget_handle>& get_children() const
    {
        return _children;
    }

    /// @brief Get the number of children.
    auto num_children() const
    {
        return _children.size();
    }

    /// @brief Find a child in the heirarchy by it's name.
    /// @return nullptr if no widget is found, else the widget handle
    fnx::widget_handle get_widget( const std::string& name );

    /// @brief Get the current widget state.
    auto get_state()
    {
        return _state;
    }

    /// @brief Is the cursor currently over the widget
    auto is_cursor_over()
    {
        return _mouse_over;
    }

    /// @brief Is the widget currently undergoing manipulation.
    auto is_active() -> bool
    {
        return _active;
    }

    /// @brief Activate the widget.
    /// @note emits an event and calls do_active()
    void activate();

    /// @brief Inactivate the widget.
    /// @note emits an event and calls do_inactive()
    void inactivate();

    /// @brief Is the widget in the pressed state.
    auto is_pressed()
    {
        return _state == state::press;
    }

    /* handle events from the engine */
    bool on_event( const mouse_move_evt& event );
    bool on_event( const mouse_press_evt& event );
    bool on_event( const mouse_release_evt& event );
    bool on_event( const mouse_scroll_evt& event );
    bool on_event( const keyboard_press_evt& event );
    bool on_event( const keyboard_release_evt& event );
    bool on_event( const keyboard_repeat_evt& event );

    /* derived widgets should override these and should not usually be called directly */
    virtual bool do_mouse_move( double x, double y )
    {
        return false;
    }
    virtual bool do_mouse_enter()
    {
        return false;
    };
    virtual bool do_mouse_exit()
    {
        return false;
    };
    virtual bool do_mouse_press( FNX_BUTTON button, double x, double y )
    {
        return false;
    }
    virtual bool do_mouse_release( FNX_BUTTON button, double x, double y )
    {
        return false;
    }
    virtual bool do_key_press( FNX_KEY key )
    {
        return false;
    }
    virtual bool do_key_release( FNX_KEY key )
    {
        return false;
    }
    virtual bool do_key_repeat( FNX_KEY key )
    {
        return false;
    }
    virtual void do_active() {}
    virtual void do_inactive() {}
    virtual bool do_scroll( double x, double y )
    {
        return false;
    }

    virtual void notify_mouse_move_absorbed( fnx::widget_handle child ) {}
    virtual void notify_mouse_enter_absorbed( fnx::widget_handle child ) {};
    virtual void notify_mouse_exit_absorbed( fnx::widget_handle child ) {};
    virtual void notify_mouse_press_absorbed( fnx::widget_handle child ) {}
    virtual void notify_mouse_release_absorbed( fnx::widget_handle child ) {}
    virtual void notify_key_press_absorbed( fnx::widget_handle child ) {}
    virtual void notify_key_release_absorbed( fnx::widget_handle child ) {}
    virtual void notify_key_repeat_absorbed( fnx::widget_handle child ) {}
    virtual void notify_active_absorbed( fnx::widget_handle child ) {}
    virtual void notify_inactive_absorbed( fnx::widget_handle child ) {}
    virtual void notify_scroll_absorbed( fnx::widget_handle child ) {}

    /// @brief Add a widget to the container.
    /// @param[in] widget : widget to be added to the container
    virtual void add_widget( fnx::widget_handle widget );

    template<typename... Types>
    void add_widgets( Types... widget )
    {
        ( add_widget( widget ), ... );
    }

    /// @brief Release all children.
    void clear()
    {
        _children.clear();
    }

    inline auto get_type() const
    {
        return _type;
    }

protected:
    friend fnx::layer_serializer;
    widget_type _type{widget_type::widget};
    widget_id _id{ 0u };				    /// unique id
    widget* _parent{ nullptr };				/// parent widget
    constraints _constraints;				/// position and size constraints within parent
    animator _animator;						/// transition animator
    model_handle _model;					/// model used to render this widget
    shader_handle _shader;					/// shader used to render this widget
    texture_handle _texture;				/// texture used to render this widget
    material_handle _material;				/// material used to render this widget
    vector4 _current_bounds{};				/// the widget's current bounds (animator)
    vector4 _original_bounds{};				/// the widget's original bounds
    fnx::vector2 _strict_size{};			/// the widget's size
    bool _mouse_over{ false };				/// the mouse is currently over the widget
    bool _recalculate{ true };
    bool _visible{ true };					/// the widget is currently visible
    bool _animator_hidden{ false };			/// the widget is currently hidden due to the animator
    bool _interactive{ true };				/// the widget accepts user input
    bool _active{ false };					/// the widget is considered to be active
    bool _checked{ false };
    bool _checkable{ true };
    bool _send_events_to_children{ true };
    fnx::decimal _current_alpha{ 1.f };		/// the widget's current alpha state (animator)
    fnx::decimal _original_alpha{ 1.f };	/// the widget's normal alpha state
    std::string _name;						/// the widget's non unique name
    state _state{ state::normal };
    std::vector<fnx::widget_handle> _children;

    friend class animator;
    friend class widget_layer;

    /// @brief Trigger after animator completes the on_show behavior
    virtual void do_made_visible()
    {
        _visible = true;
        _animator_hidden = false;
    }

    /// @brief Trigger after animator completes the on_hide behavior
    virtual void do_made_invisible()
    {
        _animator_hidden = true;
    }
};

// TODO: These functions may need to be turned into a factory if they get more complex

extern std::unordered_map<widget_id, widget_handle>& get_widget_map();

template<typename T, typename... Args, typename = typename std::enable_if<std::is_base_of<fnx::widget, T>::value>::type>
fnx::widget_handle_t<T> create_widget( Args... args )
{
    auto handle = fnx::make_shared_ref<T>( args... );
    get_widget_map()[handle->get_id()] = handle;
    return handle;
}

extern widget_handle get_widget_by_id( widget_id id );
}
