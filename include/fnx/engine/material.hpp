#pragma once

namespace fnx
{
class material : public fnx::asset
{
public:
    material() = delete;
    ~material() = default;
    material( const std::string& name ) : asset( name )
    {
        FNX_DEBUG( fnx::format_string( "initializing material %s", get_name() ) );
    }

    void add_texture( const std::string& name, fnx::texture_handle value )
    {
        _textures[name] = value;
    }
    void add_float( const std::string& name, float value )
    {
        _floats[name] = value;
    }
    void add_int( const std::string& name, int value )
    {
        _ints[name] = value;
    }
    void add_vector2( const std::string& name, const fnx::vector2& value )
    {
        _vector2s[name] = value;
    }
    void add_vector3( const std::string& name, const fnx::vector3& value )
    {
        _vector3s[name] = value;
    }
    void add_vector4( const std::string& name, const fnx::vector4& value )
    {
        _vector4s[name] = value;
    }
    void add_matrix4x4( const std::string& name, const fnx::matrix4x4& value )
    {
        _matrix4x4s[name] = value;
    }

    const auto& get_textures() const
    {
        return _textures;
    }
    const auto& get_floats() const
    {
        return _floats;
    }
    const auto& get_ints() const
    {
        return _ints;
    }
    const auto& get_vector2s() const
    {
        return _vector2s;
    }
    const auto& get_vector3s() const
    {
        return _vector3s;
    }
    const auto& get_vector4s() const
    {
        return _vector4s;
    }
    const auto& get_matrix4x4s() const
    {
        return _matrix4x4s;
    }

    bool get( const std::string& name, fnx::texture_handle& out )
    {
        return get( name, _textures, out );
    }
    bool get( const std::string& name, float& out )
    {
        return get( name, _floats, out );
    }
    bool get( const std::string& name, int& out )
    {
        return get( name, _ints, out );
    }
    bool get( const std::string& name, fnx::vector2& out )
    {
        return get( name, _vector2s, out );
    }
    bool get( const std::string& name, fnx::vector3& out )
    {
        return get( name, _vector3s, out );
    }
    bool get( const std::string& name, fnx::vector4& out )
    {
        return get( name, _vector4s, out );
    }
    bool get( const std::string& name, fnx::matrix4x4& out )
    {
        return get( name, _matrix4x4s, out );
    }

    void add_array_vector4s( const std::string& name, const std::vector<fnx::vector4>& src )
    {
        _arr_vector4s[name] = src;
    }
    const auto& get_array_vector4s() const
    {
        return _arr_vector4s;
    }

private:
    std::unordered_map<std::string, fnx::texture_handle> _textures;
    std::unordered_map<std::string, float> _floats;
    std::unordered_map<std::string, int> _ints;
    std::unordered_map<std::string, fnx::vector2> _vector2s;
    std::unordered_map<std::string, fnx::vector3> _vector3s;
    std::unordered_map<std::string, fnx::vector4> _vector4s;
    std::unordered_map<std::string, fnx::matrix4x4> _matrix4x4s;

    std::unordered_map<std::string, std::vector<fnx::vector4>> _arr_vector4s;

    template<typename ReturnType, typename SourceType>
    const bool get( const std::string& name, const SourceType& lookup, ReturnType& out )
    {
        auto it = lookup.find( name );
        if ( it != std::end( lookup ) )
        {
            out = it->second;
            return true;
        }
        return false;
    }
};

using material_handle = fnx::asset_handle<fnx::material>;

/// @brief Parses a file and puts all of the material assets into the asset manager.
/// @param[in] file_path : local file system path
extern void parse_material_file( const std::string& file_path );
}
