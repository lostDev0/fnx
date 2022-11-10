namespace fnx
{
void parse_material_file( const std::string& file_path )
{
    auto [materials, _1] = singleton<asset_manager<material>>::acquire();
    auto [textures, _2] = singleton<asset_manager<texture>>::acquire();

    std::ifstream in( file_path );
    if ( in.good() )
    {
        std::string line;
        fnx::material_handle mat;
        while ( getline( in, line ) )
        {
            std::stringstream str( line );
            std::string cmd;
            std::string word;
            float val, r, g, b, illum;
            str >> cmd;

            if ( "newmtl" == cmd )
            {
                str >> word;
                FNX_DEBUG( fnx::format_string( "found material %s", word ) );
                mat = materials.get( word );
            }
            else if ( "Ns" == cmd )
            {
                str >> val;
                mat->add_float( UNIFORM_MATERIAL_NS, val );
            }
            else if ( "Ni" == cmd )
            {
                str >> val;
                mat->add_float( UNIFORM_MATERIAL_NI, val );
            }
            else if ( "d" == cmd )
            {
                str >> val;
                mat->add_float( UNIFORM_MATERIAL_D, val );
            }
            else if ( "Ka" == cmd )
            {
                str >> r >> g >> b;
                mat->add_vector3( UNIFORM_MATERIAL_KA, fnx::vector3{ r, g, b } );
            }
            else if ( "Kd" == cmd )
            {
                str >> r >> g >> b;
                mat->add_vector3( UNIFORM_MATERIAL_KD, fnx::vector3{ r, g, b } );
            }
            else if ( "Ks" == cmd )
            {
                str >> r >> g >> b;
                mat->add_vector3( UNIFORM_MATERIAL_KS, fnx::vector3{ r, g, b } );
            }
            else if ( "Ke" == cmd )
            {
                str >> r >> g >> b;
                mat->add_vector3( UNIFORM_MATERIAL_KE, fnx::vector3{ r, g, b } );
            }
            else if ( "illum" == cmd )
            {
                str >> illum;
                mat->add_float( UNIFORM_MATERIAL_ILLUM, illum );
            }
            else if ( "map_Ka" == cmd )
            {
                str >> word;
                mat->add_texture( UNIFORM_MATERIAL_MAP_KA, textures.get( word ) );
            }
            else if ( "map_Kd" == cmd )
            {
                str >> word;
                mat->add_texture( UNIFORM_MATERIAL_MAP_KD, textures.get( word ) );
            }
            else if ( "map_Ks" == cmd )
            {
                str >> word;
                mat->add_texture( UNIFORM_MATERIAL_MAP_KS, textures.get( word ) );
            }
            else if ( "map_d" == cmd )
            {
                str >> word;
                mat->add_texture( UNIFORM_MATERIAL_MAP_D, textures.get( word ) );
            }
        }
    }
    else
    {
        in.close();
        FNX_ERROR( fnx::format_string( "unable to load material file %s", file_path.c_str() ) );
    }
}
}