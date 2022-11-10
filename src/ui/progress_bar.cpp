namespace fnx
{
void progress_bar::render( camera_handle camera, matrix4x4 parent_matrix )
{
    auto [win, _0] = singleton<fnx::window>::acquire();
    auto [renderer, _1] = singleton<fnx::renderer>::acquire();
    auto [properties, _2] = singleton<fnx::property_manager>::acquire();
    auto [models, _3] = singleton<asset_manager<model>>::acquire();
    auto [materials, _4] = singleton<asset_manager<material>>::acquire();
    auto [fonts, _5] = singleton<asset_manager<font>>::acquire();
    auto [textures, _6] = singleton<asset_manager<texture>>::acquire();
    auto [shaders, _7] = singleton<asset_manager<shader>>::acquire();

    _shader = shaders.get( "ui_progress_bar.shader" );
    _model = models.get( "quad", *raw_model_quad() );
    _material = materials.get( "ui_block.material" );

    /*
        Design & Implementation

        Model
            left = -1 opengl x axis
            top = 1 opengl y axis
            width = 2.0
            height = 2.0

        Widget Constraint
            left = x pixel
            top = y pixel
            width = n pixels
            height = m pixels

        Scale
            coord_width/screen_width = x/2.0
            coord_height/screen_height = x/2.0
    */

    auto state = _state;
    if ( state == state::normal && _checked )
    {
        state = state::checked;
    }

    auto color = _colors[static_cast<size_t>( state )];
    auto background_color = _background;
    auto outline_color = _outline_colors[static_cast<size_t>( state )];
    auto parent_alpha = has_parent() ? _parent->get_alpha() : 1.f;
    auto alpha = parent_alpha * _current_alpha;
    color.w *= alpha;
    outline_color.w *= alpha;
    background_color.w *= alpha;

    auto scale = vector3{ get_width() / 2.f, get_height() / 2.f, 1.f } * properties.get_property<float>
                 ( PROPERTY_UI_SCALE );

    // widget coordinates and size are measured in world units from -1 to 1
    // scaling will only need to be done when the UI as a whole is scaled, otherwise the viewport will take care of scaling
    // 1. start with scaling the model to our size
    auto mat_scale = matrix_scale( matrix4x4::identity(), scale );
    // 2. translate to the parent aligned matrix
    auto mat_translate = fnx::matrix_translate( parent_matrix, get_x(), get_y(), 0.f );
    // 3. get the center coordinate for the glsl shader to do it's calculations per pixel
    auto center = vector3{ get_width() / 2.f, get_height() / 2.f, 0.f };
    mat_translate = matrix_translate( mat_translate, center );
    // 4. translate to the center of the widget
    auto mat = mat_scale * matrix_translate( mat_translate, get_width() / 2.f, get_height() / 2.f, 0.f );

    _material->add_vector4( UNIFORM_BACKGROUND_COLOR, background_color );
    _material->add_float( UNIFORM_PROGRESS, _progress );
    _material->add_int( UNIFORM_DIRECTION,
                        static_cast<int>( _fill_direction ) );	// this is the progress fill direction, not the gradient direction
    _material->add_vector4( UNIFORM_COLOR, color );
    _material->add_vector2( UNIFORM_SIZE, fnx::vector2{ get_width(), get_height() } );
    _material->add_vector2( UNIFORM_CENTER, fnx::vector2{center.x, center.y} );
    _material->add_vector4( UNIFORM_RADIUS, _corner_radius );
    _material->add_vector2( UNIFORM_RESOLUTION, fnx::vector2{ static_cast<decimal>( win.width() ), static_cast<decimal>( win.height() ) } );
    _material->add_float( UNIFORM_OUTLINE_THICKNESS, _outline_thickness[static_cast<size_t>( state )] );
    _material->add_vector4( UNIFORM_OUTLINE_COLOR, outline_color );
    _material->add_int( UNIFORM_NUM_GRADIENT,
                        static_cast<int>( _gradients[static_cast<size_t>( state )].get_values().size() ) );
    _material->add_int( UNIFORM_GRADIENT_DIRECTION,
                        static_cast<int>( _gradient_directions[static_cast<size_t>( state )] ) );
    auto gradient = _gradients[static_cast<size_t>( state )].get_values();
    std::for_each( std::begin( gradient ), std::end( gradient ), [this]( fnx::vector4 & v )
    {
        v.w *= _current_alpha;
    } );	// apply any transition alpha to the gradient
    _material->add_array_vector4s( UNIFORM_GRADIENT, gradient );

    renderer.apply_shader( _shader );
    renderer.apply_model( _model );
    renderer.apply_material( _material );
    renderer.apply_camera( camera );

    // could use a transform component but this isn't an entity in the traditional sense
    // could set this on the material, but this way we avoid any reuse issues with the material and other buttons
    _shader->apply_uniform( UNIFORM_MODEL_VIEW_MATRIX, mat );

    renderer.draw_current();

    // TODO : render background with outline if requested
    // TODO : render progress with rounded edges to the left and rounded edges to the right if 100%
}
}