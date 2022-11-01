namespace fnx
{
    void line::render(camera_handle camera, matrix4x4 parent_matrix)
    {
        auto [renderer, _1] = singleton<fnx::renderer>::acquire();
        auto [properties, _2] = singleton<fnx::property_manager>::acquire();

        _model->render_as_lines();
        
        // TODO : test graphic display with "smooth" lines
        //glEnable(GL_LINE_SMOOTH);
        //glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

        renderer.apply_shader(_shader);
        renderer.apply_model(_model);

        if (_dirty_cache)
        {
            // update the VBO with the new data
            _model->update_vbo(VBO_Data, _points);	// packed data
            _dirty_cache = false;
        }

        auto center_3d = vector3(0.f, 0.f, 0.f);
        auto scale_3d = vector3(1.0f, 1.0f, 0.f) * properties.get_property<float>(PROPERTY_UI_SCALE);
        auto mat = fnx::matrix_scale(matrix4x4::identity(), scale_3d);
        mat = fnx::matrix_translate(mat, center_3d);

        _shader->apply_uniform(UNIFORM_MODEL_VIEW_MATRIX, mat);
        renderer.set_line_width(_thickness);
        
        renderer.draw_current();
        renderer.set_line_width(1.0f);	// reset width back to normal
    }
}