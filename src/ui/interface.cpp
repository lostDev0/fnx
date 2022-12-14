using namespace std;
namespace fnx
{
namespace ui
{
bool init()
{
    {
        auto [stack, _1] = singleton<layer_stack>::acquire();
    }
    {
        auto [properties, _2] = singleton<property_manager>::acquire();
        properties.set_property( fnx::PROPERTY_UI_SCALE, 1.f );
    }
    return true;
}

void parse_yaml_file( const std::string& file_path )
{
    auto [stack, _] = singleton<layer_stack>::acquire();
    ifstream in( file_path );
    if ( !in.good() )
    {
        FNX_ERROR( fnx::format_string( "Unable to load ui configuration file %s", file_path ) );
        return;
    }
    ostringstream sout;
    copy( istreambuf_iterator<char>( in ), istreambuf_iterator<char>(), ostreambuf_iterator<char>( sout ) );
    layer_serializer().deserialize( stack, sout.str() );
}
}
}