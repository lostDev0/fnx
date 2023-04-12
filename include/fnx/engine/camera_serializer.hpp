#pragma once
namespace fnx
{
class ortho_camera;
class perspective_camera;
class camera_serializer
{
public:
    std::string serialize( const fnx::ortho_camera& camera );
    std::string serialize( const fnx::perspective_camera& camera );
    void deserialize( fnx::camera_handle camera, const std::string& content );
protected:
private:
    void serialize_camera( YAML::Emitter& out, const fnx::ortho_camera& camera );
    void serialize_camera( YAML::Emitter& out, const fnx::perspective_camera& camera );
    void deserialize_camera( const YAML::Node& in, fnx::ortho_camera& camera );
    void deserialize_camera( const YAML::Node& in, fnx::perspective_camera& camera );
    fnx::camera_handle deserialize_camera_type( const YAML::Node& data );
};
}