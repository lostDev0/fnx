#pragma once

namespace fnx
{
    class block : public fnx::widget
    {
    public:

        static material_handle k_material;
        
        /// @brief Define an interactive block with colors that change due to mouse interactivety.
        block(const fnx::colors::rgba& color, 
            const fnx::colors::rgba& highlight_color, 
            const fnx::colors::rgba& press_color, 
            const fnx::colors::rgba& checked_color, 
            const std::string& name = create_type_name<fnx::block>())
            : widget(name)
        {
            _colors[static_cast<size_t>(state::normal)] = color;
            _colors[static_cast<size_t>(state::hover)] = highlight_color;
            _colors[static_cast<size_t>(state::press)] = press_color;
            _colors[static_cast<size_t>(state::checked)] = checked_color;
        }

        /// @brief Define a block where all states are the same color.
        block(const fnx::colors::rgba& color, const std::string& name = create_type_name<fnx::block>())
            : block(color,color,color,color,name)
        {
        }

        block(const std::string& name = fnx::create_type_name<fnx::block>())
            : block(fnx::colors::white, fnx::colors::white, fnx::colors::white, fnx::colors::white, name)
        {}


        virtual ~block() {}

        /// @brief Set the corner radius of all radii to the same value
        void set_corner_radius(const float& val)
        {
            _corner_radius = vector4{ val,val,val,val };
        }

        /// @brief Set the corner radius of all radii
        /// @note indice order is bottom_left, top_left, top_right, bottom_right
        void set_corner_radii(const vector4& radii)
        {
            _corner_radius = radii;
        }

        void set_corner_radius_bottom_left(const float& val)
        {
            _corner_radius[0] = val;
        }

        void set_corner_radius_top_left(const float& val)
        {
            _corner_radius[1] = val;
        }

        void set_corner_radius_top_right(const float& val)
        {
            _corner_radius[2] = val;
        }

        void set_corner_radius_bottom_right(const float& val)
        {
            _corner_radius[3] = val;
        }

        const float& get_corner_radii() const
        {
            return _corner_radius[0];
        }

        const float& get_corner_radius_bottom_left() const
        {
            return _corner_radius[0];
        }

        const float& get_corner_radius_top_left() const
        {
            return _corner_radius[1];
        }

        const float& get_corner_radius_top_right() const
        {
            return _corner_radius[2];
        }

        const float& get_corner_radius_bottom_right() const
        {
            return _corner_radius[3];
        }

        /// @brief Set the corner radius of all radii
        /// @note indice order is bottom_left, top_left, top_right, bottom_right
        void set_corner_radii(float bottom_left, float top_left, float top_right, float bottom_right)
        {
            _corner_radius = vector4{bottom_left, top_left, top_right, bottom_right};
        }

        void set_color(state state, const fnx::colors::rgba& color)
        {
            _colors[static_cast<size_t>(state)] = color;
        }

        void set_color(const fnx::colors::rgba& color)
        {
            for (auto i = 0u; i < static_cast<size_t>(state::max); ++i)
            {
                _colors[static_cast<size_t>(i)] = color;
            }
        }

        static auto convert_to_color(const unsigned int& val) -> fnx::colors::rgba
        {
            fnx::colors::rgba color;
            color.x = static_cast<float>((val & 0xff000000) >> 24) / 255.f;
            color.y = static_cast<float>((val & 0xff0000) >> 16) / 255.f;
            color.z = static_cast<float>((val & 0xff00) >> 8) / 255.f;
            color.w = static_cast<float>((val & 0xff)) / 255.f;
            return color;
        }

        static auto convert_from_color(const fnx::colors::rgba& val) -> unsigned int
        {
            unsigned int color;
            float r = val.r() * 255.f;
            float g = val.g() * 255.f;
            float b = val.b() * 255.f;
            float a = val.a() * 255.f;
            color = ((static_cast<unsigned int>(r) << 24) & 0xff000000) |
                ((static_cast<unsigned int>(g) << 16) & 0xff0000) |
                ((static_cast<unsigned int>(b) << 8) & 0xff00) |
                ((static_cast<unsigned int>(a)) & 0xff);
            return color;
        }

        void set_hex_color(const unsigned int& val)
        {
            fnx::colors::rgba color = convert_to_color(val);
            _colors[static_cast<size_t>(state::normal)] = color;
            _colors[static_cast<size_t>(state::hover)] = color;
            _colors[static_cast<size_t>(state::press)] = color;
            _colors[static_cast<size_t>(state::checked)] = color;
        }

        unsigned int get_hex_color() const
        {
            unsigned int color;
            float r = _colors[static_cast<size_t>(state::normal)].r() * 255.f;
            float g = _colors[static_cast<size_t>(state::normal)].g() * 255.f;
            float b = _colors[static_cast<size_t>(state::normal)].b() * 255.f;
            float a = _colors[static_cast<size_t>(state::normal)].a() * 255.f;
            color = ((static_cast<unsigned int>(r) << 24) & 0xff000000) |
                ((static_cast<unsigned int>(g) << 16) & 0xff0000) |
                ((static_cast<unsigned int>(b) << 8) & 0xff00) |
                ((static_cast<unsigned int>(a)) & 0xff);
            return color;
        }

        // @brief Set a gradient background for a given state
        void set_gradient(state state, const fnx::tween<vector4>& gradient, fill_direction dir)
        {
            assert(state < state::max);
            _gradients[static_cast<size_t>(state)] = gradient;
            _gradient_directions[static_cast<size_t>(state)] = dir;
        }

        void set_gradient(const fnx::tween<vector4>& gradient, fill_direction dir)
        {
            for (auto i = 0u; i < static_cast<size_t>(state::max); ++i)
            {
                _gradients[static_cast<size_t>(i)] = gradient;
                _gradient_directions[static_cast<size_t>(i)] = dir;
            }
        }

        /// @brief Set the thickness of a border for the block for all states
        void set_outline_thickness(int pixels)
        {
            for (auto i = 0u; i < static_cast<size_t>(state::max); ++i)
            {
                _outline_thickness[i] = static_cast<float>(pixels);
            }
        }
        
        /// @brief Set the thickness of a border for the block.
        void set_outline_thickness(state state, float thickness)
        { 
            _outline_thickness[static_cast<size_t>(state)] = thickness;
        }

        /// @brief Set a color for the border of the block.
        void set_outline_color(const colors::rgba& color)
        { 
            for (auto i = 0u; i < static_cast<size_t>(state::max); ++i)
            {
                _outline_colors[i] = color;
            }
        }

        /// @brief Set a color for the border of the block during a specific state.
        void set_outline_color(state state, const colors::rgba& color)
        {
            _outline_colors[static_cast<size_t>(state)] = color;
        }

        /// @brief Draw the block.
        /// @note User Interface does not currently use the camera that is provided.
        virtual void render(camera_handle camera, fnx::matrix4x4 parent_matrix) override;

        void set_outline_thickness_normal(const float& px) { _outline_thickness[static_cast<size_t>(state::normal)] = px; }
        void set_outline_thickness_hover(const float& px) { _outline_thickness[static_cast<size_t>(state::hover)] = px; }
        void set_outline_thickness_pressed(const float& px) { _outline_thickness[static_cast<size_t>(state::press)] = px; }
        void set_outline_thickness_checked(const float& px) { _outline_thickness[static_cast<size_t>(state::checked)] = px; }

        float get_outline_thickness_normal() const { return _outline_thickness[static_cast<size_t>(state::normal)]; }
        float get_outline_thickness_hover() const { return _outline_thickness[static_cast<size_t>(state::hover)]; }
        float get_outline_thickness_pressed() const { return _outline_thickness[static_cast<size_t>(state::press)]; }
        float get_outline_thickness_checked() const { return _outline_thickness[static_cast<size_t>(state::checked)]; }

        void set_outline_color_normal(const unsigned int& color) { _outline_colors[static_cast<size_t>(state::normal)] = convert_to_color(color); }
        void set_outline_color_hover(const unsigned int& color) { _outline_colors[static_cast<size_t>(state::hover)] = convert_to_color(color); }
        void set_outline_color_pressed(const unsigned int& color) { _outline_colors[static_cast<size_t>(state::press)] = convert_to_color(color); }
        void set_outline_color_checked(const unsigned int& color) { _outline_colors[static_cast<size_t>(state::checked)] = convert_to_color(color); }

        unsigned int get_outline_color_normal() const { return convert_from_color(_outline_colors[static_cast<size_t>(state::normal)]); }
        unsigned int get_outline_color_hover() const { return convert_from_color(_outline_colors[static_cast<size_t>(state::hover)]); }
        unsigned int get_outline_color_pressed() const { return convert_from_color(_outline_colors[static_cast<size_t>(state::press)]); }
        unsigned int get_outline_color_checked() const { return convert_from_color(_outline_colors[static_cast<size_t>(state::checked)]); }

    protected:

        virtual bool do_mouse_enter() override;
        virtual bool do_mouse_exit() override;
        virtual bool do_mouse_press(FNX_BUTTON key, double x, double y) override;
        virtual bool do_mouse_release(FNX_BUTTON key, double x, double y) override;

        fnx::colors::rgba _colors[static_cast<size_t>(state::max)]{ vector4{0.f,0.f,0.f,1.f} };	/// color for each state
        fnx::colors::rgba _outline_colors[static_cast<size_t>(state::max)];	/// color for each state
        float _outline_thickness[static_cast<size_t>(state::max)];	/// border thickness for each state
        fnx::tween<vector4> _gradients[static_cast<size_t>(state::max)];	/// gradient for each state
        fill_direction _gradient_directions[static_cast<size_t>(state::max)]{ fill_direction::left_to_right };	/// gradient direction for each state
        
        vector4 _corner_radius{ 0.f, 0.f, 0.f, 0.f };	/// provides a radius for each corner independently
    };
}