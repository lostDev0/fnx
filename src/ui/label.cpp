using namespace fnx::detail;

namespace fnx
{
    void label::render(camera_handle camera, matrix4x4 parent_matrix)
    {
        // TODO render background (block parent)
        if (_label.size() > 0)
        {
            auto [win, _0] = singleton<fnx::window>::acquire();
            auto [renderer, _1] = singleton<fnx::renderer>::acquire();
            auto [properties, _2] = singleton<fnx::property_manager>::acquire();

            if (_dirty_cache)
            {
                update_label_model();
            }

            auto color = _label_color;
            auto border_color = _border_color;
            auto parent_alpha = has_parent() ? _parent->get_alpha() : 1.f;
            auto alpha = parent_alpha * _current_alpha;
            color.w *= alpha;
            border_color.w *= alpha;

            // widgets are always aligned with 0,0 at top left corner
            // text is aligned with 0,0 at bottom left corner
            // adjust the y dimension for rendering the model
        
            auto scale = vector3{1.f, 1.f ,1.f } * properties.get_property<float>(PROPERTY_UI_SCALE);
            auto mat_translate = fnx::matrix_translate(parent_matrix, get_x(), get_y(), 0.f);
            
            // widget coordinates and size are measured in world units from -1 to 1
            // scaling will only need to be done when the UI as a whole is scaled, otherwise the viewport will take care of scaling
            auto mat_scale = matrix_scale(matrix4x4::identity(), scale);
            auto mat = mat_scale * mat_translate;

            auto mod = _bold ? 1.25f : .94f;
            auto font_edge_tween = fnx::tween<float>{ .43f * mod, .4f * mod, .19f * mod, .04f * mod };
            auto font_width_tween = fnx::tween<float>{ .35f * mod, .37f * mod, .49f * mod, .55f * mod };
            auto ratio = get_height() / win.height();
            auto width = font_width_tween.get(ratio);
            auto edge = font_edge_tween.get(ratio);

            _material->add_vector4(UNIFORM_COLOR, color);
            _material->add_vector4(UNIFORM_BORDER_COLOR, border_color);
            _material->add_float(UNIFORM_WIDTH, width);
            _material->add_float(UNIFORM_EDGE, edge);
            _material->add_float(UNIFORM_BORDER_WIDTH, _border_width);
            _material->add_float(UNIFORM_BORDER_EDGE, _border_edge);
            _material->add_vector2(UNIFORM_BORDER_OFFSET, _border_offset);
            _material->add_texture(UNIFORM_TEXTURE_SAMPLER, _texture);

            renderer.apply_shader(_shader);
            renderer.apply_model(_model);
            renderer.apply_material(_material);
            renderer.apply_camera(camera);

            // could use a transform component but this isn't an entity in the traditional sense
            // could set this on the material, but this way we avoid any reuse issues with the material and other buttons
            _shader->apply_uniform(UNIFORM_MODEL_VIEW_MATRIX, mat);

            renderer.draw_current();
        }
    }

    void label::update_label_model()
    {
        // update all aspects of the label and recreate the raw model
        auto [win, _0] = singleton<fnx::window>::acquire();
        auto [renderer, _1] = singleton<fnx::renderer>::acquire();
        auto [properties, _2] = singleton<fnx::property_manager>::acquire();
        auto [models, _3] = singleton<asset_manager<model>>::acquire();
        auto [materials, _4] = singleton<asset_manager<material>>::acquire();
        auto [fonts, _5] = singleton<asset_manager<font>>::acquire();
        auto [textures, _6] = singleton<asset_manager<texture>>::acquire();
        auto [shaders, _7] = singleton<asset_manager<shader>>::acquire();

        vector2 cursor{ 0.f, 0.f };

        if (win.width() > 0 && win.height() > 0)
        {
            std::vector<fnx::decimal> data;
            _label_font = fonts.get(_label_font_name, textures);
            _model = models.get(_name, true, false);
            _shader = shaders.get("ui_label.shader");
            _material = materials.get("ui_label.material");
            _texture = _label_font->get_texture();

            // this is the model and texture map information to be saved in the vbo buffers
            auto height_in_opengl = _font_size > 0.f ? _font_size : get_height();
            auto height_in_px = height_in_opengl * 2.f * static_cast<reactphysics3d::decimal>(win.height());

            // TODO : save line widths for cursor click calculations on text input widget
            _label_size = _label_font->calculate_texture_model_info(_text_alignment, 
                vector2{ get_width(), get_height() }, 
                height_in_px, _label, data, data, static_cast<reactphysics3d::decimal>(win.width()), static_cast<reactphysics3d::decimal>(win.height()), cursor, _lines);

            _model->bind_to_vao();
            _model->update_vbo(VBO_Data, data);	// packed data
            _model->unbind_vao();

            /*
                The label widget itself will need to have a size provided like a block widget.
                The label will also contain the measurements of the text always starting at 0,0.
            */
            
            if (has_parent())
            {
                auto* parent = get_parent();
                widget::on_parent_change(parent->get_x(), parent->get_y(), parent->get_width(), parent->get_height());
            }
            _dirty_cache = false;
        }
    }

    void label::set_text(const std::string& label)
    {
        _label = label;
        _dirty_cache = true;
    }

    void label::set_text_size(reactphysics3d::decimal height)
    {
        _font_size = height;
    }

    void label::set_text_size_in_pixels(size_t px)
    {
        auto [win, _] = singleton<window>::acquire();
        auto opengl_height = (static_cast<fnx::decimal>(px) / win.height()) / 2.f;
        _font_size = opengl_height;
    }

    void label::set_text_size_in_points(size_t pt)
    {
        fnx::decimal x, y;
        auto [win, _] = singleton<window>::acquire();
        win.dpi(x,y);
        auto dpi_scale = y / 72.f;
        // y holds the dots per inch, so for example 72 for y is 1 dpi_ratio
        auto pixel_height = (pt / 72.f) * y;
        auto opengl_height = (pixel_height / win.height()) / 2.f;
        _font_size = opengl_height;
    }

    void label::set_text_color(const fnx::colors::rgba& color)
    {
        _label_color = color;
    }

    void label::set_border_color(const fnx::colors::rgba& color)
    {
        _border_color = color;
    }

    void label::set_border_width(reactphysics3d::decimal width)
    {
        _border_width = width;
    }

    void label::set_border_edge(reactphysics3d::decimal edge)
    {
        _border_edge = edge;
    }

    void label::set_border_offset(reactphysics3d::decimal x, reactphysics3d::decimal y)
    {
        _border_offset = { x,y };
    }

    void label::on_parent_change(reactphysics3d::decimal parent_x, reactphysics3d::decimal parent_y, reactphysics3d::decimal parent_width, reactphysics3d::decimal parent_height)
    {
        widget::on_parent_change(parent_x, parent_y, parent_width, parent_height);
        _dirty_cache = true;
    }
}