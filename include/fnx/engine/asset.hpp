#pragma once

#include <string>

namespace fnx
{
/// @brief Interface for all managed asset types.
/// @note assets are centered around opengl rendering and I/O but not limited to
class asset
{
protected:
    std::string _name;		/// non unique name of the asset
    bool _loaded{ false };	/// flag to signify this asset is allocated
public:
    asset( const std::string& _name ) : _name( _name ) {};
    virtual ~asset() {};

    /// @brief Return the asset name.
    auto get_name() const
    {
        return _name;
    }

    /// @brief Load the asset.
    /// @note deprecated
    virtual void load()
    {
        _loaded = true;
    }

    /// @brief Unload the asset.
    /// @note deprecated
    virtual void unload()
    {
        _loaded = false;
    }

    /// @brief Return whether the asset is loaded.
    /// @note deprecated
    auto is_loaded()
    {
        return _loaded;
    }
};

template<typename T>
using asset_handle = fnx::reference_ptr<T>;	/// type that the engine should use for assets
}
