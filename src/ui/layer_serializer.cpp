namespace fnx
{
std::string layer_serializer::serialize( const layer_stack& stack )
{
    std::string content;
    YAML::Emitter out;
    out << YAML::BeginMap;
    out << YAML::Key << "layers" << YAML::Value;
    out << YAML::BeginSeq;
    for ( auto layer : stack.get_layers() )
    {
        auto obj = *layer.second;
        out << YAML::BeginMap;
        out << YAML::Key << "name" << YAML::Value << obj.get_name();
        out << YAML::Key << "visible" << YAML::Value << obj.is_visible();
        out << YAML::Key << "dirty_cache" << YAML::Value << true;
        out << YAML::Key << "active_widget" << YAML::Value << obj._active_widget;
        out << YAML::Key << "root";
        serialize_widget( out, *obj.get_root() );
        out << YAML::EndMap;
    }
    out << YAML::EndSeq;
    out << YAML::EndMap;
    content = out.c_str();
    return content;
}

void layer_serializer::serialize_widget( YAML::Emitter& out, const widget& obj )
{
    out << YAML::BeginMap;
    out << YAML::Key << "type" << YAML::Value << static_cast<int>( obj._type );
    out << YAML::Key << "id" << YAML::Value << obj._id;
    //out << YAML::Key << "constraints" << YAML::Value;
    //out << YAML::Key << "animator" << YAML::Value;
    out << YAML::Key << "model" << YAML::Value << ( obj._model ? obj._model->get_name() : "default.model" );
    out << YAML::Key << "shader" << YAML::Value << ( obj._shader ? obj._shader->get_name() : "default.shader" );
    out << YAML::Key << "texture" << YAML::Value << ( obj._texture ? obj._texture->get_name() : "default.texture" );
    out << YAML::Key << "material" << YAML::Value << ( obj._material ? obj._material->get_name() : "default.material" );
    out << YAML::Key << "current_bounds" << YAML::Value << obj._current_bounds;
    out << YAML::Key << "original_bounds" << YAML::Value << obj._original_bounds;
    out << YAML::Key << "strict_size" << YAML::Value << obj._strict_size;
    out << YAML::Key << "visible" << YAML::Value << obj._visible;
    out << YAML::Key << "animator_hidden" << YAML::Value << obj._animator_hidden;
    out << YAML::Key << "interactive" << YAML::Value << obj._interactive;
    out << YAML::Key << "checked" << YAML::Value << obj._checked;
    out << YAML::Key << "checkable" << YAML::Value << obj._checkable;
    out << YAML::Key << "send_events_to_children" << YAML::Value << obj._send_events_to_children;
    out << YAML::Key << "current_alpha" << YAML::Value << obj._current_alpha;
    out << YAML::Key << "original_alpha" << YAML::Value << obj._original_alpha;
    out << YAML::Key << "name" << YAML::Value << obj._name;
    out << YAML::Key << "state" << YAML::Value << static_cast<int>( obj._state );
    // parse all of the type specific information
    serialize_widget_type( out, obj );
    if ( obj.has_children() )
    {
        out << YAML::Key << "children";
        out << YAML::BeginSeq;
        for ( auto child : obj._children )
        {
            serialize_widget( out, *child );
        }
        out << YAML::EndSeq;
    }
    out << YAML::EndMap;
}

void layer_serializer::serialize_widget_type( YAML::Emitter& out, const widget& obj )
{
    switch ( obj.get_type() )
    {
        case widget_type::block:
            serialize_block( out, obj );
            break;
        default:    // widget
            break;
    }
}

void layer_serializer::serialize_block( YAML::Emitter& out, const widget& obj )
{
    auto& block = dynamic_cast<const fnx::block&>( obj );
    out << YAML::Key << "colors" << YAML::BeginMap;
    out << YAML::Key << "normal" << YAML::Value << block.get_color( widget::state::normal );
    out << YAML::Key << "hover" << YAML::Value << block.get_color( widget::state::hover );
    out << YAML::Key << "checked" << YAML::Value << block.get_color( widget::state::checked );
    out << YAML::Key << "press" << YAML::Value << block.get_color( widget::state::press );
    out << YAML::EndMap;
    out << YAML::Key << "outline_colors" << YAML::BeginMap;
    out << YAML::Key << "normal" << YAML::Value << block.get_outline_color( widget::state::normal );
    out << YAML::Key << "hover" << YAML::Value << block.get_outline_color( widget::state::hover );
    out << YAML::Key << "checked" << YAML::Value << block.get_outline_color( widget::state::checked );
    out << YAML::Key << "press" << YAML::Value << block.get_outline_color( widget::state::press );
    out << YAML::EndMap;
    out << YAML::Key << "outline_thickness" << YAML::BeginMap;
    out << YAML::Key << "normal" << YAML::Value << block.get_outline_thickness( widget::state::normal );
    out << YAML::Key << "hover" << YAML::Value << block.get_outline_thickness( widget::state::hover );
    out << YAML::Key << "checked" << YAML::Value << block.get_outline_thickness( widget::state::checked );
    out << YAML::Key << "press" << YAML::Value << block.get_outline_thickness( widget::state::press );
    out << YAML::EndMap;
    out << YAML::Key << "gradient" << YAML::BeginMap;
    out << YAML::Key << "normal" << YAML::Value << block.get_gradient( widget::state::normal ).get_values();
    out << YAML::Key << "hover" << YAML::Value << block.get_gradient( widget::state::hover ).get_values();
    out << YAML::Key << "checked" << YAML::Value << block.get_gradient( widget::state::checked ).get_values();
    out << YAML::Key << "press" << YAML::Value << block.get_gradient( widget::state::press ).get_values();
    out << YAML::EndMap;
    out << YAML::Key << "gradient_direction" << YAML::BeginMap;
    out << YAML::Key << "normal" << YAML::Value << static_cast<int>( block.get_gradient_direction(
                widget::state::normal ) );
    out << YAML::Key << "hover" << YAML::Value << static_cast<int>( block.get_gradient_direction( widget::state::hover ) );
    out << YAML::Key << "checked" << YAML::Value << static_cast<int>( block.get_gradient_direction(
                widget::state::checked ) );
    out << YAML::Key << "press" << YAML::Value << static_cast<int>( block.get_gradient_direction( widget::state::press ) );
    out << YAML::EndMap;
    out << YAML::Key << "corner_radius" << YAML::Value << block.get_corner_radii();
}

void layer_serializer::deserialize( layer_stack& stack, const std::string& content )
{
    YAML::Node data = YAML::Load( content );
    if ( data["layers"] )
    {
        for ( auto layer : data["layers"] )
        {
            auto handle = make_shared_ref<fnx::layer>( layer["name"].as<std::string>() );

            layer["visible"].as<bool>() ? handle->show() : handle->hide();
            handle->_active_widget = layer["active_widget"].as < decltype( handle->_active_widget ) > ();
            // assume dirty cache is always true
            if ( layer["root"] )
            {
                deserialize_widget( layer["root"], *handle->get_root() );
            }
            stack.add_layer( handle );
        }
    }
}

void layer_serializer::deserialize_widget( const YAML::Node& data, widget& obj )
{
    obj._type = static_cast<widget_type>( data["type"].as<int>() );
    obj._id = data["id"].as<decltype( obj._id )>();
    // TODO constraints
    // TODO animator
    obj._model = singleton<asset_manager<model>>::acquire().data.reserve( data["model"].as<std::string>() );
    obj._shader = singleton<asset_manager<shader>>::acquire().data.reserve( data["shader"].as<std::string>() );
    obj._texture = singleton<asset_manager<texture>>::acquire().data.reserve( data["texture"].as<std::string>() );
    obj._material = singleton<asset_manager<material>>::acquire().data.reserve( data["material"].as<std::string>() );
    obj._current_bounds = data["current_bounds"].as<fnx::vector4>();
    obj._original_bounds = data["original_bounds"].as<fnx::vector4>();
    obj._strict_size = data["strict_size"].as<fnx::vector2>();
    obj._visible = data["visible"].as<bool>();
    obj._interactive = data["interactive"].as<bool>();
    obj._checked = data["checked"].as<bool>();
    obj._checkable = data["checkable"].as<bool>();
    obj._current_alpha = data["current_alpha"].as<fnx::decimal>();
    obj._original_alpha = data["original_alpha"].as<fnx::decimal>();
    obj._name = data["name"].as<std::string>();
    obj._state = static_cast<widget::state>( data["state"].as<int>() );
    if ( data["children"] )
    {
        for ( const YAML::Node& node : data["children"] )
        {
            auto child = deserialize_widget_type( node );
            obj.add_widget( child );
        }
    }
}

fnx::widget_handle layer_serializer::deserialize_widget_type( const YAML::Node& data )
{
    auto type = static_cast<fnx::widget_type>( data["type"].as<int>() );
    fnx::widget_handle handle;
    switch ( type )
    {
        case widget_type::block:
            // TODO
            handle = create_widget<fnx::block>( data["name"].as<std::string>() );
            break;
        default:    // widget
            handle = create_widget<fnx::widget>( data["name"].as<std::string>() );
            break;
    }
    deserialize_widget( data, *handle );
    return handle;
}
}