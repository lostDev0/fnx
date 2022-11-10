#pragma once

namespace fnx
{
class image : public fnx::block
{
public:
    /// @brief Creates a static image.
    image( const std::string& resource_name, const std::string& name = create_type_name<fnx::image>() )
        : block( fnx::colors::black, name )
        , _resource( resource_name )
        , _overlay( false )
    {
        // have to send an alpha channel higher than 0 so that it is displayed and alpha transitions are still applied
    }

    /// @brief Creates a static image with a color overlay.
    image( const std::string& resource_name, const fnx::colors::rgba& color,
           const std::string& name = create_type_name<fnx::block>() )
        : block( color, name )
        , _resource( resource_name )
    {

    }

    /// @brief Creates a static image with a dynamic color change effect for mouse events.
    image( const std::string& resource_name,
           const fnx::colors::rgba& color, const fnx::colors::rgba& highlight_color,
           const fnx::colors::rgba& press_color, const fnx::colors::rgba& checked_color,
           const std::string& name = create_type_name<fnx::block>() )
        : block( color, highlight_color, press_color, checked_color, name )
        , _resource( resource_name )
    {

    }

    image( const std::string& resource_name, const fnx::texture::config& resource_config,
           int normal_state_atlas_index, int highlight_state_atlas_index, int press_state_atlas_index,
           int checked_state_atlas_index, const std::string& name = create_type_name<fnx::block>() )
        : block( fnx::colors::black, name )
        , _resource( resource_name )
        , _resource_config( resource_config )
        , _overlay( false )
        , _atlas_index{normal_state_atlas_index, highlight_state_atlas_index, press_state_atlas_index, checked_state_atlas_index}
    {

    }

    virtual ~image() {};

    void render( camera_handle camera, matrix4x4 parent_matrix ) override;

    /// @brief Manually command the image to use a particular atlas index.
    void set_atlas_index( int index )
    {
        _auto_pick_atlas_index = false;
        _manual_atlas_index = index;
    }

    /// @brief Automatically chose the atlas index based on widget state.
    void reset_atlas_index()
    {
        _auto_pick_atlas_index = true;
        _manual_atlas_index = 0;
    }

protected:
    std::string _resource;
    fnx::texture::config _resource_config{};
    int _atlas_index[static_cast<int>( state::max )] { -1 };
    int _manual_atlas_index{ 0 };
    bool _auto_pick_atlas_index{ true };
    bool _overlay{ true };
};
}