#pragma once

namespace fnx
{
/// <summary>
/// Designates a range of vertices as needing a particular material map.
/// </summary>
struct material_info
{
    std::string _material_name;
    int _start{ 0u };
    int _end{ 0u };

    material_info() {};
    ~material_info() {};
    material_info( const std::string& material_name, int start, int end )
        : _material_name{ material_name }
        , _start{ start }
        , _end{ end }
    {}
};

/// <summary>
/// A collection of Material ranges.
/// </summary>
class material_map
{
    std::vector<material_info> _materials;
public:
    material_map() = default;
    ~material_map() = default;

    auto& add_material_range( const std::string& material_name, int start, int end )
    {
        return _materials.emplace_back( material_name, start, end );
    }

    /// @brief Return all materials.
    const auto& get_all() const
    {
        return _materials;
    }

    /// @brief Return all materials.
    auto& get_all_mutable()
    {
        return _materials;
    }

    /// @brief Return the number of materials.
    inline auto size() const
    {
        return _materials.size();
    }
};
}