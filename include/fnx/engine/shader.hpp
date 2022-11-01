#pragma once

namespace fnx
{
    enum shader_index
    {
        shader_vertex = 0,
        shader_fragment,
        shader_max
    };

    class shader_impl;
    class shader : public fnx::asset
    {
    public:
        /// @brief Constructs shader and initializes it.
        shader(const std::string& path);
        shader(const std::string& name, const std::string& content);
        ~shader();

        void bind() const;

        void apply_all_uniforms() const;

        /* apply_uniform sets the uniform without error checking */
        void apply_uniform(const char* uniform_name, int val) const;
        void apply_uniform(const char* uniform_name, unsigned int val) const;
        void apply_uniform(const char* uniform_name, float val) const;
        void apply_uniform(const char* uniform_name, double val) const;

        /* directions (vectors) */
        void apply_uniform(const char* uniform_name, int x, int y) const;
        void apply_uniform(const char* uniform_name, int x, int y, int z) const;
        void apply_uniform(const char* uniform_name, int x, int y, int z, int w) const;
        void apply_uniform(const char* uniform_name, const fnx::vector2& val) const;
        void apply_uniform(const char* uniform_name, const fnx::vector3& val) const;
        void apply_uniform(const char* uniform_name, const fnx::vector4& val) const;
        void apply_uniform(const char* uniform_name, const fnx::matrix4x4& val) const;

        static std::string create_array_name(const char* uniform_name, unsigned int index);
        static std::string create_struct_name(const char* uniform_name, const char* member_name);
        static std::string create_struct_array_name(const char* uniform_name, const char* member_name, unsigned int index);

        /* array */
        void apply_uniform(const char* uniform_name, unsigned int index, int val) const;
        void apply_uniform(const char* uniform_name, unsigned int index, unsigned int val) const;
        void apply_uniform(const char* uniform_name, unsigned int index, float val) const;
        void apply_uniform(const char* uniform_name, unsigned int index, double val) const;
        void apply_uniform(const char* uniform_name, unsigned int index, int x, int y) const;
        void apply_uniform(const char* uniform_name, unsigned int index, int x, int y, int z) const;
        void apply_uniform(const char* uniform_name, unsigned int index, int x, int y, int z, int w) const;
        void apply_uniform(const char* uniform_name, unsigned int index, const fnx::vector2& val) const;
        void apply_uniform(const char* uniform_name, unsigned int index, const fnx::vector3& val) const;
        void apply_uniform(const char* uniform_name, unsigned int index, const fnx::vector4& val) const;
        void apply_uniform(const char* uniform_name, unsigned int index, const fnx::matrix4x4& val) const;

        /* structs */
        void apply_uniform(const char* uniform_name, const char* member_name, int val) const;
        void apply_uniform(const char* uniform_name, const char* member_name, unsigned int val) const;
        void apply_uniform(const char* uniform_name, const char* member_name, float val) const;
        void apply_uniform(const char* uniform_name, const char* member_name, double val) const;
        void apply_uniform(const char* uniform_name, const char* member_name, int x, int y) const;
        void apply_uniform(const char* uniform_name, const char* member_name, int x, int y, int z) const;
        void apply_uniform(const char* uniform_name, const char* member_name, int x, int y, int z, int w) const;
        void apply_uniform(const char* uniform_name, const char* member_name, const fnx::vector2& val) const;
        void apply_uniform(const char* uniform_name, const char* member_name, const fnx::vector3& val) const;
        void apply_uniform(const char* uniform_name, const char* member_name, const fnx::vector4& val) const;
        void apply_uniform(const char* uniform_name, const char* member_name, const fnx::matrix4x4& val) const;

        void apply_uniform(const char* uniform_name, const char* member_name, unsigned int index, int val) const;
        void apply_uniform(const char* uniform_name, const char* member_name, unsigned int index, unsigned int val) const;
        void apply_uniform(const char* uniform_name, const char* member_name, unsigned int index, float val) const;
        void apply_uniform(const char* uniform_name, const char* member_name, unsigned int index, double val) const;
        void apply_uniform(const char* uniform_name, const char* member_name, unsigned int index, int x, int y) const;
        void apply_uniform(const char* uniform_name, const char* member_name, unsigned int index, int x, int y, int z) const;
        void apply_uniform(const char* uniform_name, const char* member_name, unsigned int index, int x, int y, int z, int w) const;
        void apply_uniform(const char* uniform_name, const char* member_name, unsigned int index, const fnx::vector2& val) const;
        void apply_uniform(const char* uniform_name, const char* member_name, unsigned int index, const fnx::vector3& val) const;
        void apply_uniform(const char* uniform_name, const char* member_name, unsigned int index, const fnx::vector4& val) const;
        void apply_uniform(const char* uniform_name, const char* member_name, unsigned int index, const fnx::matrix4x4& val) const;

        /* set_uniform sets the uniform with error checking */
        void set_uniform(const char* uniform_name, int val) const;
        void set_uniform(const char* uniform_name, unsigned int val) const;
        void set_uniform(const char* uniform_name, float val) const;
        void set_uniform(const char* uniform_name, double val) const;
        void set_uniform(const char* uniform_name, const fnx::vector2& val) const;
        void set_uniform(const char* uniform_name, const fnx::vector3& val) const;
        void set_uniform(const char* uniform_name, const fnx::vector4& val) const;
        void set_uniform(const char* uniform_name, const fnx::matrix4x4& val) const;
        void set_uniform(const char* uniform_name, unsigned int index, int value) const;
        void set_uniform(const char* uniform_name, unsigned int index, float value) const;
        void set_uniform(const char* uniform_name, unsigned int index, int x, int y) const;
        void set_uniform(const char* uniform_name, unsigned int index, int x, int y, int z) const;
        void set_uniform(const char* uniform_name, unsigned int index, int x, int y, int z, int w) const;
        void set_uniform(const char* uniform_name, unsigned int index, const fnx::vector2& value) const;
        void set_uniform(const char* uniform_name, unsigned int index, const fnx::vector3& value) const;
        void set_uniform(const char* uniform_name, unsigned int index, const fnx::vector4& value) const;
        void set_uniform(const char* uniform_name, const char* member_name, int val) const;
        void set_uniform(const char* uniform_name, const char* member_name, unsigned int val) const;
        void set_uniform(const char* uniform_name, const char* member_name, float val) const;
        void set_uniform(const char* uniform_name, const char* member_name, double val) const;
        void set_uniform(const char* uniform_name, const char* member_name, int x, int y) const;
        void set_uniform(const char* uniform_name, const char* member_name, int x, int y, int z) const;
        void set_uniform(const char* uniform_name, const char* member_name, int x, int y, int z, int w) const;
        void set_uniform(const char* uniform_name, const char* member_name, const fnx::vector2& val) const;
        void set_uniform(const char* uniform_name, const char* member_name, const fnx::vector3& val) const;
        void set_uniform(const char* uniform_name, const char* member_name, const fnx::vector4& val) const;
        void set_uniform(const char* uniform_name, const char* member_name, const fnx::matrix4x4& val) const;
        void set_uniform(const char* uniform_name, const char* member, unsigned int index, int value) const;
        void set_uniform(const char* uniform_name, const char* member, unsigned int index, float value) const;
        void set_uniform(const char* uniform_name, const char* member, unsigned int index, int x, int y) const;
        void set_uniform(const char* uniform_name, const char* member, unsigned int index, int x, int y, int z) const;
        void set_uniform(const char* uniform_name, const char* member, unsigned int index, const fnx::vector2& value) const;
        void set_uniform(const char* uniform_name, const char* member, unsigned int index, const fnx::vector3& value) const;
        void set_uniform(const char* uniform_name, const char* member, unsigned int index, const fnx::vector4& value) const;

    private:
        shader_impl* _impl{ nullptr };

        void init(const std::string& vert, const std::string& frag);
        void add_all_uniforms(const std::string& source);
    };

    using shader_handle = fnx::asset_handle<fnx::shader>;
}
