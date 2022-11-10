#pragma once

namespace fnx
{
/// @brief A block widget that will partially render itself based on a progress percentage.
/// @note A background color is also specified to show unfilled progress.
class progress_bar : public fnx::block
{
public:
    progress_bar( const fnx::colors::rgba& color,
                  const fnx::colors::rgba& highlight_color,
                  const fnx::colors::rgba& press_color,
                  const fnx::colors::rgba& checked_color,
                  const fnx::colors::rgba& background_color,
                  fill_direction direction = fill_direction::left_to_right,
                  const std::string& name = create_type_name<progress_bar>() )
        : block( color, highlight_color, press_color, checked_color, name )
        , _background( background_color )
    {
        // will be flagged as _checkable by the block constructor
        set_fill_direction( direction );
        disable_events();
    }

    progress_bar( const fnx::colors::rgba& color,
                  const fnx::colors::rgba& background_color,
                  fill_direction direction = fill_direction::left_to_right,
                  const std::string& name = create_type_name<progress_bar>() )
        : block( color, name )
        , _background( background_color )
    {
        // will NOT be flagged as _checkable by the block constructor
        set_fill_direction( direction );
        disable_events();
    }

    ~progress_bar() = default;

    void set_progress( float percentage )
    {
        _progress = fnx::maximum( 0.f, percentage );
        _progress = fnx::minimum( 1.f, _progress );
    }

    void set_fill_direction( fill_direction direction )
    {
        if ( direction != fill_direction::circle && direction != fill_direction::outer_to_inner )
        {
            _fill_direction = direction;
        }
    }

    auto get_progress()
    {
        return _progress;
    }
    auto get_fill_direction()
    {
        return _fill_direction;
    }

    virtual void render( camera_handle camera, matrix4x4 parent_matrix ) override;

protected:
    fnx::colors::rgba _background{};
    float _progress{ 0.f };
    fill_direction _fill_direction{ fill_direction::left_to_right };
};
}