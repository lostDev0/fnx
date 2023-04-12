#pragma once
using namespace reactphysics3d;

namespace fnx
{
class perspective_camera : public fnx::camera
{
public:
    perspective_camera( const fnx::angle& fov_y, decimal aspect, decimal front, decimal back )
        : fnx::camera()
        , _field_of_view( fov_y )
        , _near( front )
        , _far( back )
    {
        _up = Vector3( 0.f, 0.f, 1.f );
        _front = Vector3( 0.f, 1.f, 0.f );
        _aspect_ratio = aspect;
        update_view_matrix();
    }
    virtual ~perspective_camera() = default;

    virtual void zoom( double delta ) override;
    virtual void pan( double delta ) override;
    virtual void pitch( const fnx::angle& angle ) override;
    virtual void rotate_y( const fnx::angle& angle ) override;
    virtual void rotate_z( const fnx::angle& angle ) override;

    void set_params( const fnx::angle& fov_y, float front, float back )
    {
        _field_of_view = fov_y;
        _near = front;
        _far = back;
        update_view_matrix();
    }
protected:
    friend fnx::camera_serializer;
    virtual void update_view_matrix() override;

    fnx::angle _field_of_view;
    float _near;
    float _far;
};
}
