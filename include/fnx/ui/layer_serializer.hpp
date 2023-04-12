#pragma once
namespace fnx
{
class layer_serializer
{
public:
    std::string serialize( const layer_stack& stack );
    void deserialize( layer_stack& stack, const std::string& content );
private:
    void serialize_widget( YAML::Emitter& out, const widget& obj );
    void serialize_widget_type( YAML::Emitter& out, const widget& obj );

    void serialize_block( YAML::Emitter& out, const widget& obj );
    void deserialize_block( const YAML::Node& data, widget& obj );

    void deserialize_widget( const YAML::Node& in, widget& obj );
    fnx::widget_handle deserialize_widget_type( const YAML::Node& data );
};
}