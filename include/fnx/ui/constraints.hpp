#pragma once

namespace fnx
{
class widget;
/// @brief Constrain a value based on input.
class constraint
{
public:
    constraint() {};
    virtual ~constraint() {};
    constraint( const constraint& other ) = default;
    constraint( constraint&& other ) = default;
    constraint& operator=( const constraint& other ) = default;
    constraint& operator=( constraint&& other ) = default;

    virtual fnx::vector4 apply( const fnx::vector4& parent, const fnx::vector4& child )
    {
        return child;
    }

    virtual std::unique_ptr<constraint> clone()
    {
        return std::make_unique<constraint>( *this );
    };
};

/// @brief Collection of constraint objects for position and sizing a child widget within a parent.
class constraints
{
public:
    constraints() = default;
    ~constraints() = default;

    template <typename... Ts>
    constraints( const Ts& ... ts )
    {
        ( add( ts ), ... );
    }
    constraints( const constraints& other )
    {
        _constraints.reserve( other._constraints.size() );
        for ( const auto& e : other._constraints )
        {
            _constraints.push_back( e->clone() );
        }
    }

    constraints& operator=( const constraints& other )
    {
        _constraints.clear();
        _constraints.reserve( other._constraints.size() );
        for ( const auto& e : other._constraints )
        {
            _constraints.push_back( e->clone() );
        }
        return *this;
    }

    template<typename T, typename std::enable_if<std::is_base_of<fnx::constraint, T>::value, T>::type* = nullptr>
    void add( const T& constraint )
    {
        _constraints.push_back( std::make_unique<T>( constraint ) );
    }

    template<typename T, typename std::enable_if<std::is_base_of<fnx::constraint, T>::value, T>::type* = nullptr>
    void add( std::unique_ptr<T>& constraint )
    {
        _constraints.push_back( std::move( constraint ) );
    }

    template<typename T, typename std::enable_if<std::is_base_of<fnx::constraint, T>::value, T>::type* = nullptr>
    void add_post_process( const T& constraint )
    {
        _post_process_constraints.push_back( std::make_unique<T>( constraint ) );
    }

    template<typename T, typename std::enable_if<std::is_base_of<fnx::constraint, T>::value, T>::type* = nullptr>
    void add_post_process( std::unique_ptr<T>& constraint )
    {
        _post_process_constraints.push_back( std::move( constraint ) );
    }

    void clear()
    {
        _constraints.clear();
    }

    void clear_post_process()
    {
        _post_process_constraints.clear();
    }

    void clear_all()
    {
        clear();
        clear_post_process();
    }

    vector4 apply( const vector4& parent, const vector4& child )
    {
        auto n_child = child;
        for ( const auto& c : _constraints )
        {
            n_child = c->apply( parent, n_child );
        }
        for ( const auto& c : _post_process_constraints )
        {
            n_child = c->apply( parent, n_child );
        }
        return n_child;
    }

    auto clone_all() const
    {
        std::vector<std::unique_ptr<fnx::constraint>> ret;
        for ( const auto& ptr : _constraints )
        {
            ret.push_back( ptr->clone() );
        }
        return ret;
    }

protected:
    std::vector<std::unique_ptr<fnx::constraint>> _constraints;
    std::vector<std::unique_ptr<fnx::constraint>>
            _post_process_constraints;	/// additional constraints that are applied after the base constraints
};

class relative_horz_constraint : public constraint
{
public:
    relative_horz_constraint( float f ) : constraint(), _ratio{ f } {}
    virtual ~relative_horz_constraint() {};
    relative_horz_constraint( const relative_horz_constraint& other ) = default;
    relative_horz_constraint( relative_horz_constraint&& other ) = default;
    relative_horz_constraint& operator=( const relative_horz_constraint& other ) = default;
    relative_horz_constraint& operator=( relative_horz_constraint&& other ) = default;

    virtual std::unique_ptr<constraint> clone() override
    {
        return std::make_unique<relative_horz_constraint>( *this );
    };

    virtual vector4 apply( const vector4& parent, const vector4& child )
    {
        auto n_child = child;
        auto left = 0.f;
        auto right = parent.z;
        auto child_width = child.z;
        auto x = fnx::calc_value( left, right, _ratio );
        n_child.x = fnx::minimum( right - child_width, x );	// don't exceed right
        n_child.x = fnx::maximum( n_child.x, left );	// don't exceed left
        return n_child;
    }
protected:
    float _ratio{ 0.f };
};

class relative_vert_constraint : public constraint
{
public:
    relative_vert_constraint( float f ) : constraint(), _ratio{ f } {}
    virtual ~relative_vert_constraint() {};
    relative_vert_constraint( const relative_vert_constraint& other ) = default;
    relative_vert_constraint( relative_vert_constraint&& other ) = default;
    relative_vert_constraint& operator=( const relative_vert_constraint& other ) = default;
    relative_vert_constraint& operator=( relative_vert_constraint&& other ) = default;

    virtual std::unique_ptr<constraint> clone() override
    {
        return std::make_unique<relative_vert_constraint>( *this );
    };

    virtual vector4 apply( const vector4& parent, const vector4& child )
    {
        auto n_child = child;
        auto top = parent.w;
        auto bottom = 0.f;
        auto child_height = child.w;
        auto y = fnx::calc_value( bottom, top, _ratio );
        n_child.y = fnx::minimum( top - child_height, y );	// don't exceed top
        n_child.y = fnx::maximum( n_child.y, bottom );	// don't exceed bottom
        return n_child;
    }
protected:
    float _ratio{ 0.f };
};

class fill_horz_constraint : public constraint
{
public:
    fill_horz_constraint() = default;
    fill_horz_constraint( float f ) : constraint(), _ratio( f ) {}
    virtual ~fill_horz_constraint() {};
    fill_horz_constraint( const fill_horz_constraint& other ) = default;
    fill_horz_constraint( fill_horz_constraint&& other ) = default;
    fill_horz_constraint& operator=( const fill_horz_constraint& other ) = default;
    fill_horz_constraint& operator=( fill_horz_constraint&& other ) = default;

    virtual std::unique_ptr<constraint> clone() override
    {
        return std::make_unique<fill_horz_constraint>( *this );
    };

    virtual vector4 apply( const vector4& parent, const vector4& child )
    {
        auto n_child = child;
        // z = width
        n_child.z = parent.z * _ratio;
        return n_child;
    }
protected:
    float _ratio{ 1.f };
};

class fill_vert_constraint : public constraint
{
public:
    fill_vert_constraint() = default;
    fill_vert_constraint( float f ) : constraint(), _ratio( f ) {}
    virtual ~fill_vert_constraint() {};
    fill_vert_constraint( const fill_vert_constraint& other ) = default;
    fill_vert_constraint( fill_vert_constraint&& other ) = default;
    fill_vert_constraint& operator=( const fill_vert_constraint& other ) = default;
    fill_vert_constraint& operator=( fill_vert_constraint&& other ) = default;

    virtual std::unique_ptr<constraint> clone() override
    {
        return std::make_unique<fill_vert_constraint>( *this );
    };

    virtual vector4 apply( const vector4& parent, const vector4& child )
    {
        auto n_child = child;
        // w = height
        n_child.w = parent.w * _ratio;
        return n_child;
    }
protected:
    float _ratio{ 1.f };
};

class center_horz_constraint : public relative_horz_constraint
{
public:
    center_horz_constraint() : relative_horz_constraint( .5f ) {}
    virtual ~center_horz_constraint() {};
    center_horz_constraint( const center_horz_constraint& other ) = default;
    center_horz_constraint( center_horz_constraint&& other ) = default;
    center_horz_constraint& operator=( const center_horz_constraint& other ) = default;
    center_horz_constraint& operator=( center_horz_constraint&& other ) = default;

    virtual std::unique_ptr<constraint> clone() override
    {
        return std::make_unique<center_horz_constraint>( *this );
    };

    virtual vector4 apply( const vector4& parent, const vector4& child ) override
    {
        auto n_child = child;
        auto child_width = child.z;
        auto parent_width = parent.z;
        n_child.x = ( parent_width / 2.f ) - ( child_width / 2.f );
        return n_child;
    }
};

class center_vert_constraint : public relative_vert_constraint
{
public:
    center_vert_constraint() : relative_vert_constraint( .5f ) {}
    virtual ~center_vert_constraint() {};
    center_vert_constraint( const center_vert_constraint& other ) = default;
    center_vert_constraint( center_vert_constraint&& other ) = default;
    center_vert_constraint& operator=( const center_vert_constraint& other ) = default;
    center_vert_constraint& operator=( center_vert_constraint&& other ) = default;

    virtual std::unique_ptr<constraint> clone() override
    {
        return std::make_unique<center_vert_constraint>( *this );
    };

    virtual vector4 apply( const vector4& parent, const vector4& child ) override
    {
        auto n_child = child;
        auto child_height = child.w;
        auto parent_height = parent.w;
        n_child.y = ( parent_height / 2.f ) - ( child_height / 2.f );
        return n_child;
    }
};

class left_align_constraint : public relative_horz_constraint
{
public:
    left_align_constraint() : relative_horz_constraint( 0.f ) {}
    virtual ~left_align_constraint() {};
    left_align_constraint( const left_align_constraint& other ) = default;
    left_align_constraint( left_align_constraint&& other ) = default;
    left_align_constraint& operator=( const left_align_constraint& other ) = default;
    left_align_constraint& operator=( left_align_constraint&& other ) = default;

    virtual std::unique_ptr<constraint> clone() override
    {
        return std::make_unique<left_align_constraint>( *this );
    };
};

class right_align_constraint : public relative_horz_constraint
{
public:
    right_align_constraint() : relative_horz_constraint( 1.f ) {}
    virtual ~right_align_constraint() {};
    right_align_constraint( const right_align_constraint& other ) = default;
    right_align_constraint( right_align_constraint&& other ) = default;
    right_align_constraint& operator=( const right_align_constraint& other ) = default;
    right_align_constraint& operator=( right_align_constraint&& other ) = default;

    virtual std::unique_ptr<constraint> clone() override
    {
        return std::make_unique<right_align_constraint>( *this );
    };
};

class top_align_constraint : public relative_vert_constraint
{
public:
    top_align_constraint() : relative_vert_constraint( 1.f ) {}
    virtual ~top_align_constraint() {};
    top_align_constraint( const top_align_constraint& other ) = default;
    top_align_constraint( top_align_constraint&& other ) = default;
    top_align_constraint& operator=( const top_align_constraint& other ) = default;
    top_align_constraint& operator=( top_align_constraint&& other ) = default;

    virtual std::unique_ptr<constraint> clone() override
    {
        return std::make_unique<top_align_constraint>( *this );
    };
};

class bottom_align_constraint : public relative_vert_constraint
{
public:
    bottom_align_constraint() : relative_vert_constraint( 0.f ) {}
    virtual ~bottom_align_constraint() {};
    bottom_align_constraint( const bottom_align_constraint& other ) = default;
    bottom_align_constraint( bottom_align_constraint&& other ) = default;
    bottom_align_constraint& operator=( const bottom_align_constraint& other ) = default;
    bottom_align_constraint& operator=( bottom_align_constraint&& other ) = default;

    virtual std::unique_ptr<constraint> clone() override
    {
        return std::make_unique<bottom_align_constraint>( *this );
    };
};

class padding_constraint : public constraint
{
public:
    padding_constraint() = default;
    padding_constraint( float pad )
        : constraint()
        , _pad{pad, pad, pad, pad} {}
    padding_constraint( float left, float top, float right, float bottom ) : constraint(), _pad{ left, top, right, bottom } {}
    virtual ~padding_constraint() {};
    padding_constraint( const padding_constraint& other ) = default;
    padding_constraint( padding_constraint&& other ) = default;
    padding_constraint& operator=( const padding_constraint& other ) = default;
    padding_constraint& operator=( padding_constraint&& other ) = default;

    virtual std::unique_ptr<constraint> clone() override
    {
        return std::make_unique<padding_constraint>( *this );
    };

    virtual vector4 apply( const vector4& parent, const vector4& child )
    {
        auto n_child = child;
        n_child.x = child.x + _pad[0];
        n_child.y = child.y + _pad[1];
        n_child.z = child.z - _pad[2] - _pad[0];	// account for the left and right
        n_child.w = child.w - _pad[3] - _pad[1];	// account for the top and bottom
        return n_child;
    }
protected:
    vector4 _pad;
};

/// @brief Sets the height of the widget to be visibly equivalent to the width.
class vert_aspect_constraint : public constraint
{
public:
    vert_aspect_constraint() = default;
    vert_aspect_constraint( float modifier ) : constraint(), _modifier{ modifier } {}
    virtual ~vert_aspect_constraint() {};
    vert_aspect_constraint( const vert_aspect_constraint& other ) = default;
    vert_aspect_constraint( vert_aspect_constraint&& other ) = default;
    vert_aspect_constraint& operator=( const vert_aspect_constraint& other ) = default;
    vert_aspect_constraint& operator=( vert_aspect_constraint&& other ) = default;

    virtual std::unique_ptr<constraint> clone() override
    {
        return std::make_unique<vert_aspect_constraint>( *this );
    };

    virtual vector4 apply( const vector4& parent, const vector4& child );
protected:
    float _modifier{ 1.f };
};

/// @brief Sets the width of the widget to be visibly equivalent to the height.
class horz_aspect_constraint : public constraint
{
public:
    horz_aspect_constraint() = default;
    horz_aspect_constraint( float modifier ) : constraint(), _modifier{ modifier } {}
    virtual ~horz_aspect_constraint() {};
    horz_aspect_constraint( const horz_aspect_constraint& other ) = default;
    horz_aspect_constraint( horz_aspect_constraint&& other ) = default;
    horz_aspect_constraint& operator=( const horz_aspect_constraint& other ) = default;
    horz_aspect_constraint& operator=( horz_aspect_constraint&& other ) = default;

    virtual std::unique_ptr<constraint> clone() override
    {
        return std::make_unique<horz_aspect_constraint>( *this );
    };

    virtual vector4 apply( const vector4& parent, const vector4& child );
protected:
    float _modifier{ 1.f };
};

/// @brief Labels have different origins and should be positioned using this method.
extern void apply_label_constraints( fnx::widget& widget, alignment align );

const fnx::constraints fill_parent{ fill_horz_constraint{}, fill_vert_constraint{}, relative_horz_constraint{ 0.f }, relative_vert_constraint{ 0.f } };

const fnx::constraints center_in_parent{ center_horz_constraint{}, center_vert_constraint{}};
}