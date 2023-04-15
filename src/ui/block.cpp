namespace fnx
{
namespace detail
{
const std::string ui_block_shader{ R"(#vertex shader
#version 330 core
layout(location = 0) in vec3 vert;

uniform mat4 u_ModelViewMatrix;
out vec4 FragPosition;

void main()
{
    FragPosition = u_ModelViewMatrix * vec4(vert, 1.0);
    gl_Position = FragPosition;
}

#fragment shader
#version 330 core
out vec4 OutColor;

in vec4 FragPosition;

uniform vec4 u_Radius;
uniform vec2 u_Center;
uniform vec2 u_Size;
uniform vec2 u_Resolution;

uniform vec4 u_Color;
uniform vec4 u_Gradient[10];
uniform int u_GradientDirection;
uniform int u_NumGradient;
uniform float u_OutlineThickness;
uniform vec4 u_OutlineColor;

//const float threshold = 1.5;
const float threshold = .001;

vec3 additive_blend(vec3 bg, vec3 fg)
{
    return min(bg + fg, 1.f);
}

vec3 average_blend(vec3 bg, vec3 fg)
{
    return (bg + fg) / 2.0;
}

bool point_in_triangle(vec2 p, vec2 p0, vec2 p1, vec2 p2)
{
    float A = .5 * (-p1.y * p2.x + p0.y * (-p1.x + p2.x) + p0.x * (p1.y - p2.y) + p1.x * p2.y);
    float sign = A < 0.0 ? -1.0 : 1.0;
    float s = (p0.y * p2.x - p0.x * p2.y + (p2.y - p0.y) * p.x + (p0.x - p2.x) * p.y) * sign;
    float t = (p0.x * p1.y - p0.y * p1.x + (p0.y - p1.y) * p.x + (p1.x - p0.x) * p.y) * sign;

    return s > 0.0 && t > 0.0 && (s + t) < 2.0 * A * sign;
}

vec2 convert_to_pixel(vec2 model, vec2 resolution)
{
    return vec2(((model.x + 1.0) / 2.0) * resolution.x, ((1.0 - model.y) / 2.0) * resolution.y);
}

float square(float val)
{
    return (val * val);
}

float distance_squared(vec2 p1, vec2 p2)
{
    vec2 vector = p2 - p1;
    return (vector.x * vector.x) + (vector.y * vector.y);
}

/// @brief Calculate a circular gradient with cut off edges
/// @param pixel_position : opengl coordindate of the pixel
/// @param max_size : do not apply gradient past this boundary
/// @param center : center of the circle
/// @param radius : radius of the circle
/// @param threshold : how far out from the radius to allow gradient
float calc_corner_alpha(vec2 pixel_position, vec2 center, float radius, float threshold)
{
    if (radius <= 0.0)
    {
        return u_Color.a;
    }

    float lower_squared = square(radius);
    float dist_squared = distance_squared(pixel_position, center);
    float upper_squared = square(radius + threshold);
    return min(u_Color.a, smoothstep(upper_squared, lower_squared, dist_squared));
}

vec4 find_gradient_color(float t)
{
    /*
    
    c0......c1......c2.......c3
    t0..........t............t1
           left....right
    
    */
    // find out where in the tween we are
    // find out point between the two scales
    t = min(t, 1.0);
    vec4 ret_val;

    if (u_NumGradient > 1)
    {
        float color_dist = 1.0 / float(u_NumGradient - 1);
        int left_idx = int(t / color_dist);
        int right_idx = left_idx;
        float left_bound = left_idx * color_dist;
        float right_bound = left_bound;
        vec4 left = u_Gradient[left_idx];
        vec4 right = left;
        
        if (left_idx < u_NumGradient - 1)
        {
            right_idx = left_idx + 1;
            right_bound = right_idx * color_dist;
            right = u_Gradient[right_idx];
        }

        t = (t - left_bound) / color_dist;		
        ret_val = (right - left) * t + left;
    }
    else if (u_NumGradient == 1)
    {
        ret_val = u_Gradient[0];
    }

    return ret_val;
}

void main()
{
    // TODO send aspect ratio as uniform
    float aspect_ratio = u_Resolution.x / u_Resolution.y;
    float a = 0.0;
    float half_width = (u_Size.x / 2.0);
    float half_height = (u_Size.y / 2.0);

    vec2 top_left = vec2(u_Center.x - (half_width - u_Radius[1]), u_Center.y + (half_height - u_Radius[1]));
    vec2 top_right = vec2(u_Center.x + (half_width - u_Radius[2]), u_Center.y + (half_height - u_Radius[2]));
    vec2 bottom_left = vec2(u_Center.x - (half_width - u_Radius[0]), u_Center.y - (half_height - u_Radius[0]));
    vec2 bottom_right = vec2(u_Center.x + (half_width - u_Radius[3]), u_Center.y - (half_height - u_Radius[3]));

    top_left.x = min(u_Center.x, top_left.x);
    top_left.y = max(u_Center.y, top_left.y); 
    
    bottom_left.y = min(u_Center.y, bottom_left.y);
    bottom_left.x = min(u_Center.x, bottom_left.x);

    top_right.x = max(u_Center.x, top_right.x);
    top_right.y = max(u_Center.y, top_right.y);

    bottom_right.x = max(u_Center.x, bottom_right.x);
    bottom_right.y = min(u_Center.y, bottom_right.y);

    vec2 curPosition = convert_to_pixel(FragPosition.xy, u_Resolution);
    //vec2 curPosition = FragPosition.xy;

    vec4 color = u_Color;
    if (u_NumGradient > 0)
    {
        float t_left = u_Center.x - half_width;
        float t_right = u_Center.x + half_width;
        float t_top = u_Center.y - half_height;
        float t_bottom = u_Center.y + half_height;
        if (0 == u_GradientDirection)
        {
            // top to bottom
            color = find_gradient_color((curPosition.y - t_top) / (t_bottom - t_top));
        }
        else if(1 == u_GradientDirection)
        {
            // left to right
            color = find_gradient_color((curPosition.x - t_left) / (t_right - t_left));
        }
        else if (2 == u_GradientDirection)
        {
            // outer to inner (square)
            if (point_in_triangle(curPosition, u_Center, vec2(t_left, t_top), vec2(t_right, t_top)))
            {
                // top
                color = find_gradient_color((curPosition.y - t_top) / (half_height));
            }
            else if (point_in_triangle(curPosition, u_Center, vec2(t_left, t_top), vec2(t_left, t_bottom)))
            {
                // left
                color = find_gradient_color((curPosition.x - t_left) / (half_width));
            }
            else if (point_in_triangle(curPosition, u_Center, vec2(t_right, t_top), vec2(t_right, t_bottom)))
            {
                // right
                color = find_gradient_color((t_right - curPosition.x) / (half_width));
            }
            else
            {
                // bottom
                color = find_gradient_color((t_bottom - curPosition.y) / (half_height));
            }
        }
        else if (3 == u_GradientDirection)
        {
            // TODO
            // bottom to top
            color = find_gradient_color((curPosition.y - t_top) / (t_bottom - t_top));
        }
        else if (4 == u_GradientDirection)
        {
            // TODO
            // right to left
            color = find_gradient_color((curPosition.x - t_left) / (t_right - t_left));
        }
        else if (5 == u_GradientDirection)
        {
            // TODO
            // outer to inner (circle)
            color = find_gradient_color((curPosition.x - t_left) / (t_right - t_left));
        }
    }

    // handle the block outline
    if(u_OutlineThickness > 0.0)
    {
        float t_left = u_Center.x - half_width + u_OutlineThickness;
        float t_right = u_Center.x + half_width - u_OutlineThickness;
        float t_top = u_Center.y - half_height + (u_OutlineThickness);
        float t_bottom = u_Center.y + half_height - (u_OutlineThickness);
        // thickness is in pixels
        if (curPosition.x < t_left || curPosition.x > t_right || curPosition.y < t_top || curPosition.y > t_bottom)
        {
            // apply border color to rectangular box
            color = u_OutlineColor;
        }
        else
        {
            float rad_squared_bottom_left = square(u_Radius[0] - (u_OutlineThickness));
            float rad_squared_top_left = square(u_Radius[1] - (u_OutlineThickness));
            float rad_squared_top_right = square(u_Radius[2] - (u_OutlineThickness));
            float rad_squared_bottom_right = square(u_Radius[3] - (u_OutlineThickness));
            if (curPosition.x < top_left.x && curPosition.y > top_left.y && (distance_squared(curPosition, top_left) > rad_squared_top_left))
            {
                color = u_OutlineColor;
            }
            else if (curPosition.x > top_right.x&& curPosition.y > top_right.y && (distance_squared(curPosition, top_right) > rad_squared_top_right))
            {
                color = u_OutlineColor;
            }
            else if (curPosition.x < bottom_left.x && curPosition.y < bottom_left.y && (distance_squared(curPosition, bottom_left) > rad_squared_bottom_left))
            {
                color = u_OutlineColor;
            }
            else if (curPosition.x > bottom_right.x&& curPosition.y < bottom_right.y && (distance_squared(curPosition, bottom_right) > rad_squared_bottom_right))
            {
                color = u_OutlineColor;
            }
        }
    }

    if (curPosition.x < top_left.x && curPosition.y > top_left.y)
    {
        // top left corner
        a = calc_corner_alpha(curPosition.xy, top_left, u_Radius[1], threshold);
    }
    else if (curPosition.x < bottom_left.x && curPosition.y < bottom_left.y)
    {
        // bottom left corner
        a = calc_corner_alpha(curPosition.xy, bottom_left, u_Radius[0], threshold);
    }
    else if (curPosition.x > bottom_right.x && curPosition.y < bottom_right.y)
    {
        // bottom right corner
        a = calc_corner_alpha(curPosition.xy, bottom_right, u_Radius[3], threshold);
    }
    else if (curPosition.x > top_right.x && curPosition.y > top_right.y)
    {
        // top right corner
        a = calc_corner_alpha(curPosition.xy, top_right, u_Radius[2], threshold);
    }
    else
    {
        a = color.a;
    }

    //OutColor = vec4(color.rgb, a);
    OutColor = vec4(color.xyz, min(a, color.a));
}
)" };
    }
    void block::render(camera_handle camera, matrix4x4 parent_matrix)
    {
        auto [win, _0] = singleton<fnx::window>::acquire();
        auto [renderer, _1] = singleton<fnx::renderer>::acquire();
        {
            auto [manager, _2] = singleton<asset_manager<shader>>::acquire();
            _shader = manager.get("fnx_ui_block.shader", fnx::detail::ui_block_shader);
        }
        {
            auto [manager, _2] = singleton<asset_manager<model>>::acquire();
            _model = manager.get("quad", raw_model_quad());
        }
        {
            auto [manager, _2] = singleton<asset_manager<material>>::acquire();
            _material = manager.get("ui_block.material");
        }

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
        if (state == state::normal && _checked)
        {
            state = state::checked;
        }

        auto color = _colors[static_cast<size_t>(state)];
        auto outline_color = _outline_colors[static_cast<size_t>(state)];
        auto parent_alpha = has_parent() ? _parent->get_alpha() : 1.f; 
        auto alpha = parent_alpha * _current_alpha;
        color.w *= alpha;
        outline_color.w *= alpha;

        auto half_width = get_width() / 2.f;
        auto half_height = get_height() / 2.f;
        auto [properties, _2] = singleton<property_manager>::acquire();
        auto scale = vector3{half_width, half_height, 1.f} * properties.get_property<float>(PROPERTY_UI_SCALE);

        // widget coordinates and size are measured in world units from -1 to 1
        // scaling will only need to be done when the UI as a whole is scaled, otherwise the viewport will take care of scaling
        // 1. start with scaling the model to our size
        auto mat_scale = matrix_scale(matrix4x4::identity(), scale);
        // 2. translate to the parent aligned matrix
        auto mat_translate = fnx::matrix_translate(parent_matrix, get_x(), get_y(), 0.f);
        // 3. get the center coordinate for the glsl shader to do it's calculations per pixel
        auto center = vector3{ half_width, half_height, 0.f }; 
        //mat_translate.transform(center);
        mat_translate = matrix_translate(mat_translate, center);
        // 4. translate to the center of the widget
        auto mat = mat_scale * matrix_translate(mat_translate, half_width, half_height, 0.f);
        _material->add_vector4(UNIFORM_COLOR, color);

        // opengl based 
        //_material->add_vector2(UNIFORM_SIZE, vector2{ get_width(), get_height() });
        //_material->add_vector2(UNIFORM_CENTER, vector2{center.x, center.y});
        //_material->add_vector4(UNIFORM_RADIUS, _corner_radius); // @todo Handle screen proportions

        auto pixel_size = fnx::vector2{ get_width() * (win.width()/2.f), get_height() * (win.height() / 2.f) };
        auto pixel_center = win.opengl_to_screen(center.x, center.y);
        
        _material->add_vector2(UNIFORM_SIZE, pixel_size);
        _material->add_vector2(UNIFORM_CENTER, pixel_center);
        _material->add_vector4(UNIFORM_RADIUS, _corner_radius); // @todo Handle screen proportions

        _material->add_vector2(UNIFORM_RESOLUTION, fnx::vector2{ static_cast<decimal>(win.width()), static_cast<decimal>(win.height()) });
        _material->add_float(UNIFORM_OUTLINE_THICKNESS, _outline_thickness[static_cast<size_t>(state)]);
        _material->add_vector4(UNIFORM_OUTLINE_COLOR, outline_color);
        _material->add_int(UNIFORM_NUM_GRADIENT, static_cast<int>(_gradients[static_cast<size_t>(state)].get_values().size()));
        _material->add_int(UNIFORM_GRADIENT_DIRECTION, static_cast<int>(_gradient_directions[static_cast<size_t>(state)]));
        auto gradient = _gradients[static_cast<size_t>(state)].get_values();
        std::for_each(std::begin(gradient), std::end(gradient), [&](fnx::vector4& v) { v.w *= alpha; });	// apply any transition alpha to the gradient
        _material->add_array_vector4s(UNIFORM_GRADIENT, gradient);

        renderer.apply_shader(_shader);
        renderer.apply_model(_model);
        renderer.apply_material(_material);
        renderer.apply_camera(camera);

        // could use a transform component but this isn't an entity in the traditional sense
        // could set this on the material, but this way we avoid any reuse issues with the material and other buttons
        _shader->apply_uniform(UNIFORM_MODEL_VIEW_MATRIX, mat);

        renderer.draw_current();

        // Render children
        widget::render(camera, parent_matrix);
    }

    bool block::do_mouse_enter()
    {
        _mouse_over = true;
        _state = state::hover;
        return false;
    }

    bool block::do_mouse_press(FNX_BUTTON key, double x, double y)
    {
        if (_mouse_over)
        {
            _state = state::press;
            widget_press_evt e{ _id };
            FNX_EMIT(e);
            activate();
            return true;
        }
        return false;
    }

    bool block::do_mouse_release(FNX_BUTTON key, double x, double y)
    {
        if (_mouse_over)
        {
            if (!is_active())
                // the press event did not originate from this widget
                return false;

            // press originated from this widget
            if (is_checkable())
            {
                toggle();
            }

            widget_release_evt e{ _id };
            FNX_EMIT(e);
            
            // cursor is still over, transition to hover state
            _state = state::hover;

            // no press is pending, inactivate the widget
            inactivate();
            return true;
        }

        _state = state::normal;
        return false;
    }

    bool block::do_mouse_exit()
    {
        if (_checked)
        {
            _state = state::checked;
        }
        else
        {
            _state = state::normal;
        }
        _mouse_over = false;
        return false;
    }
}
