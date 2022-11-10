#pragma once

namespace fnx
{
/// @brief Interface for positioning and sizing children.
class sizer : public fnx::block
{
public:
    sizer( const std::string& name = create_type_name<fnx::sizer>() )
        : block( fnx::colors::transparent, name ) {}
    virtual ~sizer() = default;

    /// @brief Resize the children of the sizer
    virtual void resize() {};

    virtual void on_parent_change( float parent_x, float parent_y, float parent_width, float parent_height ) override;

    void set_proportions( const std::vector<size_t>& values )
    {
        _proportions.clear();
        for ( auto v : values )
        {
            _proportions.emplace_back( static_cast<float>( v ) );
        }
        // fill any left over rows with a standard proportion
        for ( auto i = _proportions.size(); i < _children.size(); ++i )
        {
            _proportions.emplace_back( float{ 1.f } );
        }
    }

    void set_spacing( float pad )
    {
        _left_pad = pad;
        _right_pad = pad;
        _top_pad = pad;
        _bottom_pad = pad;
        _center_pad = pad / 2.f;
    }

    void set_spacing( float left, float right, float top, float bottom, float center )
    {
        _left_pad = left;
        _right_pad = right;
        _top_pad = top;
        _bottom_pad = bottom;
        _center_pad = center;
    }

protected:
    std::vector<float> _proportions;
    float _left_pad{ 0.f };
    float _right_pad{ 0.f };
    float _top_pad{ 0.f };
    float _bottom_pad{ 0.f };
    float _center_pad{ 0.f };
};

/// @brief 2D grid of children who have different sizes.
class box_sizer : public fnx::sizer
{
public:
    box_sizer( size_t rows, size_t cols, const std::string& name = create_type_name<fnx::box_sizer>() )
        : sizer( name ), _rows{ rows }, _cols{ cols } {}
    virtual ~box_sizer() = default;

    /// @brief Resize the children of the sizer
    // TODO: loop through all children and calculate row and col sizes
    virtual void resize() override {};

    virtual void add_row()
    {
        ++_rows;
    }

    virtual void add_col()
    {
        ++_cols;
    }

protected:
    size_t _rows{ 0u };
    size_t _cols{ 0u };
};

/// @brief 2D grid of children who all share the same size.
class grid_sizer : public fnx::box_sizer
{
public:
    grid_sizer( size_t rows, size_t cols, const std::string& name = create_type_name<fnx::grid_sizer>() )
        : box_sizer( rows, cols, name ) {}
    virtual ~grid_sizer() = default;

    virtual void resize() override;
};

class vert_sizer : public fnx::box_sizer
{
public:
    enum class direction
    {
        bottom_to_top,
        top_to_bottom
    };
    vert_sizer( size_t rows = 0, vert_sizer::direction direction = vert_sizer::direction::top_to_bottom,
                const std::string& name = create_type_name<fnx::widget>() )
        : box_sizer( rows, 1, name ), _fill_direction{ direction } {}
    virtual ~vert_sizer() = default;

    virtual void add_col() override {};
    virtual void resize() override;
    virtual void add_widget( widget_handle widget ) override
    {
        sizer::add_widget( widget );
        widget->set_strict_size( widget->get_width(), widget->get_height() );
        if ( num_children() > _rows )
        {
            ++_rows;
        }
        if ( _proportions.size() < num_children() )
        {
            _proportions.push_back( 1.f );
        }
    }

protected:
    vert_sizer::direction _fill_direction{ vert_sizer::direction::bottom_to_top };
};

class vert_fit_sizer : public fnx::vert_sizer
{
public:
    vert_fit_sizer( size_t rows = 0, vert_sizer::direction direction = vert_sizer::direction::top_to_bottom,
                    const std::string& name = create_type_name<fnx::vert_fit_sizer>() )
        : vert_sizer( rows, direction, name ) {}
    virtual ~vert_fit_sizer() = default;

    virtual void resize() override;
};

class horz_sizer : public fnx::box_sizer
{
public:
    enum class direction
    {
        left_to_right,
        right_to_left
    };
    horz_sizer( size_t cols = 0, horz_sizer::direction direction = horz_sizer::direction::left_to_right,
                const std::string& name = create_type_name<fnx::horz_sizer>() )
        : box_sizer( 1, cols, name ), _fill_direction{ direction } {}
    virtual ~horz_sizer() = default;

    virtual void add_row() override {};
    virtual void resize() override;

    virtual void add_widget( widget_handle widget ) override
    {
        sizer::add_widget( widget );
        if ( num_children() > _cols )
        {
            ++_cols;
        }
        widget->set_strict_size( widget->get_width(), widget->get_height() );
        if ( _proportions.size() < num_children() )
        {
            _proportions.push_back( 1.f );
        }
    }

protected:
    horz_sizer::direction _fill_direction{ horz_sizer::direction::left_to_right };
};

class horz_fit_sizer : public fnx::horz_sizer
{
public:
    horz_fit_sizer( size_t cols, horz_sizer::direction direction,
                    const std::string& name = create_type_name<fnx::horz_fit_sizer>() )
        : horz_sizer( cols = 0, direction, name ) {}
    virtual ~horz_fit_sizer() = default;

    virtual void resize() override;
};
}