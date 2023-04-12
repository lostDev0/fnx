namespace fnx
{
std::string camera_serializer::serialize( const fnx::ortho_camera& camera )
{
    YAML::Emitter out;
    serialize_camera( out, camera );
    return out.c_str();
}

std::string camera_serializer::serialize( const fnx::perspective_camera& camera )
{
    YAML::Emitter out;
    serialize_camera( out, camera );
    return out.c_str();
}

void camera_serializer::deserialize( fnx::camera_handle camera, const std::string& content )
{
    YAML::Node data = YAML::Load( content );
    if ( data["cameras"] )
    {
        for ( auto camera : data["cameras"] )
        {
            if ( camera["type"] )
            {
                auto type = camera["type"].as<std::string>();
                if ( type == "ortho" )
                {
                    auto ortho_camera = make_shared_ref<fnx::ortho_camera>();
                    deserialize_camera( camera, *ortho_camera );
                }
                else if ( type == "perspective" )
                {
                    auto perspective_camera = make_shared_ref<fnx::perspective_camera>( fnx::angle( fnx::Degree( 70.0 ) ), 16.0 / 9.0, .1,
                                              1000.0 );
                    deserialize_camera( camera, *perspective_camera );
                }
            }
        }
    }
}

void camera_serializer::serialize_camera( YAML::Emitter& out, const fnx::ortho_camera& camera )
{
    out << YAML::BeginMap;
    out << YAML::Key << "type" << YAML::Value << "ortho";
    out << YAML::Key << "position" << YAML::Value << camera._position;
    out << YAML::Key << "rotation" << YAML::Value << camera._rotation.get_as_radian();
    out << YAML::Key << "up" << YAML::Value << camera._up;
    out << YAML::Key << "front" << YAML::Value << camera._front;
    out << YAML::Key << "target" << YAML::Value << camera._target;
    out << YAML::Key << "aspect" << YAML::Value << camera._aspect_ratio;
    out << YAML::Key << "left" << YAML::Value << camera._left;
    out << YAML::Key << "right" << YAML::Value << camera._right;
    out << YAML::Key << "bottom" << YAML::Value << camera._bottom;
    out << YAML::Key << "top" << YAML::Value << camera._top;
    out << YAML::EndMap;
}

void camera_serializer::serialize_camera( YAML::Emitter& out, const fnx::perspective_camera& camera )
{
    out << YAML::BeginMap;
    out << YAML::Key << "type" << YAML::Value << "perspective";
    out << YAML::Key << "position" << YAML::Value << camera._position;
    out << YAML::Key << "rotation" << YAML::Value << camera._rotation.get_as_radian();
    out << YAML::Key << "up" << YAML::Value << camera._up;
    out << YAML::Key << "front" << YAML::Value << camera._front;
    out << YAML::Key << "target" << YAML::Value << camera._target;
    out << YAML::Key << "aspect" << YAML::Value << camera._aspect_ratio;
    out << YAML::Key << "field_of_view" << YAML::Value << camera._field_of_view.get_as_radian();
    out << YAML::Key << "near" << YAML::Value << camera._near;
    out << YAML::Key << "far" << YAML::Value << camera._far;
    out << YAML::EndMap;
}

void camera_serializer::deserialize_camera( const YAML::Node& data, fnx::ortho_camera& camera )
{
    camera._position = data["position"].as<vector3>();
    camera._up = data["up"].as<vector3>();
    camera._front = data["front"].as<vector3>();
    camera._target = data["target"].as<vector3>();
    camera._aspect_ratio = data["aspect"].as<float>();
    camera._rotation = fnx::Radian( data["rotation"].as<float>() );

    camera._left = data["left"].as<float>();
    camera._right = data["right"].as<float>();
    camera._top = data["top"].as<float>();
    camera._bottom = data["bottom"].as<float>();
    camera.update_view_matrix();
}

void camera_serializer::deserialize_camera( const YAML::Node& data, fnx::perspective_camera& camera )
{
    camera._position = data["position"].as<vector3>();
    camera._up = data["up"].as<vector3>();
    camera._front = data["front"].as<vector3>();
    camera._target = data["target"].as<vector3>();
    camera._aspect_ratio = data["aspect"].as<float>();
    camera._rotation = fnx::Radian( data["rotation"].as<float>() );

    camera._field_of_view = fnx::Radian( data["field_of_view"].as<float>() );
    camera._near = data["near"].as<float>();
    camera._far = data["far"].as<float>();
    camera.update_view_matrix();
}

fnx::camera_handle camera_serializer::deserialize_camera_type( const YAML::Node& data )
{
    return fnx::camera_handle();
}
}