using namespace fnx;

void perspective_camera::update_view_matrix()
{
    _view_matrix = fnx::matrix_look_at( _position, _position + _front, _up );
    _projection_matrix = fnx::matrix_perspective( _field_of_view, _aspect_ratio, _near, _far );
    _view_projection_matrix = _projection_matrix * _view_matrix;
}

void perspective_camera::zoom( double delta )
{
    _position += _front * delta;
    update_view_matrix();
}

void perspective_camera::pan( double delta )
{
    _position += _up.cross( _front ) * delta;
    update_view_matrix();
}

void perspective_camera::pitch( const fnx::angle& angle )
{
    if ( angle.get_as_degree() != 0.0f )
    {
        /*
        fnx::vector3 right = (_up.cross(_front)).get_normal();
        auto rotation = fnx::matrix_to_vector(fnx::matrix_rotate(fnx::matrix_identity(), angle, right) * fnx::matrix<float,4,1>(fnx::vec4f(_front, 0.f)));
        _front = fnx::vector3(rotation.x, rotation.y, rotation.z).get_normal();
        _up = (_front.cross(right)).get_normal();
        update_view_matrix();
        */
    }
}

void perspective_camera::rotate_y( const fnx::angle& angle )
{
    if ( angle.get_as_degree() != 0.0f )
    {
        /*
        static const fnx::vector3 up(0.0f, 1.0f, 0.0f);
        auto rotation = fnx::matrix_rotate(fnx::matrix4x4::identity(), angle, up);
        auto front_rotation_vector = fnx::matrix_to_vector(rotation * fnx::matrix<float, 4, 1>(fnx::vec4f(_front, 0.f)));
        auto up_rotation_vector = fnx::matrix_to_vector(rotation * fnx::matrix<float, 4, 1>(fnx::vec4f(_up, 0.f)));
        _front = fnx::vector3(front_rotation_vector.x, front_rotation_vector.y, front_rotation_vector.z).get_normal();
        _up = fnx::vector3(up_rotation_vector.x, up_rotation_vector.y, up_rotation_vector.z).get_normal();
        update_view_matrix();
        */
    }
}

void perspective_camera::rotate_z( const fnx::angle& angle )
{
    if ( angle.get_as_degree() != 0.0f )
    {
        /*
        static const fnx::vector3 up(0.0f, 0.0f, 1.0f);
        auto rotation = fnx::matrix_rotate(fnx::matrix4x4::identity(), angle, up);
        auto front_rotation_vector = fnx::matrix_to_vector(rotation * fnx::matrix<float, 4, 1>(fnx::vec4f(_front, 0.f)));
        auto up_rotation_vector = fnx::matrix_to_vector(rotation * fnx::matrix<float, 4, 1>(fnx::vec4f(_up, 0.f)));
        _front = fnx::vector3(front_rotation_vector.x, front_rotation_vector.y, front_rotation_vector.z).get_normal();
        _up = fnx::vector3(up_rotation_vector.x, up_rotation_vector.y, up_rotation_vector.z).get_normal();
        update_view_matrix();
        */
    }
}
