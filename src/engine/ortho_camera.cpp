namespace fnx
{
void ortho_camera::update_view_matrix()
{
    auto transform = fnx::matrix_translate( matrix4x4::identity(), _position );
    auto rotate = fnx::matrix_rotate( matrix4x4::identity(), _rotation, fnx::vector3{0, 0, 1} );
    _view_matrix = fnx::matrix_inverse( transform * rotate );
    _view_projection_matrix = _projection_matrix * _view_matrix;
}
}