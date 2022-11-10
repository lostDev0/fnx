#pragma once

namespace fnx
{
class line : public fnx::widget
{
public:
    line( const std::string& name = create_type_name<fnx::line>() ) : widget( name ) {}
    virtual ~line() {}

    void set_thickness( float thickness_in_pixels )
    {
        _thickness = thickness_in_pixels;
    }
    void add_point( const fnx::vector3& position, const colors::rgba& color )
    {
        _points.insert( _points.end(), { position.x, position.y, position.z, color.r(), color.g(), color.b(), color.a() } );
        _dirty_cache = true;
    }

    virtual void render( camera_handle camera, matrix4x4 parent_matrix ) override;

protected:
    bool _dirty_cache;
    float _thickness;				/// line thickness in pixels
    std::vector<float> _points;		/// stored data of vertices and colors
};
}