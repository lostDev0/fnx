#pragma once

namespace fnx
{
    /// @brief raw_model represents the raw mesh of a model.
    /// @note Model class is reponsible for holding the material, skin, texture, etc. and applying it to the raw_model.
    class raw_model : public fnx::asset
    {
    public:

        enum data_bit
        {
            texture = 0x01,
            normal = 0x02,
            color = 0x04,
            hints = 0x08,
            line = 0x10
        };

        using vbo_data_arr_t = std::vector<float>;
        using ibo_data_arr_t = std::vector<unsigned short>;

        raw_model(const std::string& name);
        raw_model(const std::string& name, const vbo_data_arr_t& vbo_data);
        raw_model(const std::string& name, const float * vbo_data, size_t size);
        raw_model(const std::string& name, const vbo_data_arr_t& vbo_data, const ibo_data_arr_t& ibo_data);

        /// @brief creates a quad from two triangles
        raw_model(const std::string& name, float left, float top, float width, float height);
        ~raw_model();

        std::vector<float> get_position_data() const;
        const auto& get_vertices() const { return _vbo_data; }
        const auto& get_indices() const { return _ibo_data; }
        const auto& get_material_map() const { return _material_map; }
        const auto& get_aabb() const { return _aabb; }
        auto& get_mutable_vertices() { return _vbo_data; }
        auto& get_mutable_indices() { return _ibo_data; }
        auto& get_mutable_material_map() { return _material_map; }
        auto& get_mutable_aabb() { return _aabb; }
        auto get_num_vertices() const
        {
            auto size = _vbo_data.size();
            auto divisor = get_stride();
            return size / divisor; 
        }

        auto get_stride() const -> uint32_t
        {
            auto stride = 3;	// vertex xyz
            if (has_texture_data())
            {
                stride += 2;	// uv
            }
            if (has_normal_data())
            {
                stride += 3;	// xyz
            }
            if (has_color_data())
            {
                stride += 4;	// rgba
            }
            return stride;
        }

        bool has_texture_data() const { return _flags & data_bit::texture; }
        bool has_normal_data() const { return _flags & data_bit::normal; }
        bool has_color_data() const { return _flags & data_bit::color; }
        bool is_line() const { return _flags & data_bit::line; }
        bool has_material_map() const { return _material_map.size(); }

        void set_texture_data_true() { _flags |= data_bit::texture; }
        void set_normal_data_true() { _flags |= data_bit::normal; }
        void set_color_data_true() { _flags |= data_bit::color; }
        void set_is_line_true() { _flags |= data_bit::line; }

        void set_texture_data_false() { _flags &= ~(1 << data_bit::texture); }
        void set_normal_data_false() { _flags &= ~(1 << data_bit::normal); }
        void set_color_data_false() { _flags &= ~(1 << data_bit::color); }
        void set_is_line_false() { _flags &= ~(1 << data_bit::line); }

        /// @todo Hints

    private:
        vbo_data_arr_t _vbo_data;	/// raw vertex data
        ibo_data_arr_t _ibo_data;	/// vertex draw order
        int _flags{ 0 };
        material_map _material_map;
        reactphysics3d::AABB _aabb; /// min and maxes in each axis
    };

    using raw_model_handle = fnx::asset_handle<fnx::raw_model>;

    extern std::shared_ptr<fnx::raw_model> raw_model_quad();
    extern std::vector<float> create_vbo_data_for_line(const reactphysics3d::Vector3& start, const reactphysics3d::Vector3& end, const reactphysics3d::Quaternion& start_color, const reactphysics3d::Quaternion& end_color);
    extern raw_model_handle raw_model_line(const reactphysics3d::Vector3& start, const reactphysics3d::Vector3& end, const reactphysics3d::Quaternion& start_color, const reactphysics3d::Quaternion& end_color);
    extern void parse_model_file(const std::string& file_path);
}
