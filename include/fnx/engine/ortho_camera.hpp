#pragma once

namespace fnx
{
class ortho_camera : public fnx::camera
{
public:
    ortho_camera( float aspect_ratio = 1.f, float left = -1.f, float right = 1.f, float bottom = -1.f, float top = 1.f )
        : fnx::camera()
        , _left( left )
        , _right( right )
        , _bottom( bottom )
        , _top( top )
    {
        set_aspect_ratio( aspect_ratio );
    }
    virtual ~ortho_camera() = default;

    virtual void rotate_z( const fnx::angle& rotation ) override
    {
        _rotation = rotation;
        update_view_matrix();
    }

    virtual void set_aspect_ratio( float ratio ) override
    {
        _projection_matrix = fnx::matrix_ortho( _left * ratio, _right * ratio, _bottom, _top );
        camera::set_aspect_ratio( ratio );	// will update matrix
    }

protected:
    virtual void update_view_matrix() override;

    float _left{};
    float _right{};
    float _bottom{};
    float _top{};
};
}
