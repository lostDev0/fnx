#pragma once
#include <map>

namespace fnx
{
class layer_stack
{
    std::map<size_t, layer_handle> _layers;
public:
    layer_stack();
    ~layer_stack();

    /// @brief Return the number of layers.
    auto count()
    {
        return _layers.size();
    }

    /// @brief Insert a layer at the end of the list
    bool add_layer( layer_handle layer );

    /// @brief Insert a layer if it doesn't already exist
    /// @return bool : true if the layer was added
    bool insert_layer( size_t loc, layer_handle layer );

    /// @brief Returns the layer at the given position if it exists.
    /// @return layer_handle : nullptr if the layer doesn't exist at the position
    auto get_layer_at_position_event( size_t index )
    {
        return _layers[index];
    }

    /// @brief Returns every layer.
    const auto& get_layers() const
    {
        return _layers;
    }

    /// @brief Returns the opengl entity layer.
    auto get_entity_layer()
    {
        return _layers[0u];
    }

    /// @brief Removes a layer if it exists.
    bool remove_layer( const std::string& layer_name );

private:
    template<typename event_type>
    bool send_event_to_layers( const event_type& evt )
    {
        bool ret = false;
        auto reverse_it = _layers.rbegin();
        auto reverse_end = _layers.rend();
        for ( ; reverse_it != reverse_end; ++reverse_it )
        {
            ret |= reverse_it->second->on_event( evt );
            if ( ret )
            {
                // one of the widgets within the layer handled the
                // event so we don't pass it through
                break;
            }
        }
        return ret;
    }

    /// @brief Call update on all layers in order.
    bool update_layers( const fnx::update_evt& evt );

    /// @brief Call render on all layers in order.
    bool render_layers( const fnx::render_user_interface_evt& evt );
};

template<>
struct serializer<fnx::layer_stack>
{
    static inline void to_yaml( std::string& content, const fnx::layer_stack& obj )
    {
        YAML::Emitter out;
        out << YAML::BeginSeq << "layers";
        for ( auto layer : obj.get_layers() )
        {
            // TODO handle widget_layer vs layer
            serializer<fnx::layer>::to_yaml( out, *( layer.second ) );
        }
        out << YAML::EndSeq;
        content = out.c_str();
    }

    static inline void from_yaml( const std::string& content, fnx::layer_stack& obj )
    {
        YAML::Node data = YAML::Load( content );
        if ( data["layers"] )
        {
            for ( auto layer : data["layers"] )
            {
                // TODO need to handle layer vs widge_layer
                auto handle = make_shared_ref<fnx::layer>();
                serializer<fnx::layer>::from_yaml( layer, *handle );
                obj.add_layer( handle );
            }
        }
    }
};
}
