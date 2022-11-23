#include <algorithm>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

using namespace std;

namespace fnx
{
struct texture::impl
{
    GLuint _texture{};
    GLenum _texture_target{ GL_TEXTURE_2D };
    GLuint _render_buffer{ 0 };
    GLuint _frame_buffer{ 0 };

    impl() = default;
    impl( GLuint target ) : _texture_target{target} { }
    ~impl() = default;
};

texture::texture( const std::string& file_path )
    : fnx::asset( file_path )
    , _impl{ new texture::impl( GL_TEXTURE_2D ) }
{
    if ( !_image.load_from_file( file_path, 4 ) )
    {
        FNX_ERROR( fnx::format_string( "unable to load texture resource %s", file_path ) );
    }
    init();
}

texture::texture( const std::string& file_path, uint32_t target )
    : fnx::asset( file_path )
    , _impl{ new texture::impl( target ) }
{
    if ( !_image.load_from_file( file_path, 4 ) )
    {
        FNX_ERROR( fnx::format_string( "unable to load texture resource %s", file_path ) );
    }
    init();
}

texture::texture( const std::string& file_path, const fnx::texture::config& config )
    : texture( file_path, config._channels, config._format, config._internal_format,
               config._filter, config._attachment, config._clamp, config._enable_mip, config._mip_bias, config._rows, config._cols )
{

}

texture::texture( const std::string& file_path, unsigned int channels,
                  fnx::format format, fnx::format internal_format, fnx::filter filter,
                  fnx::attachment attachment, bool clamp, bool enable_mip_mapping, float mip_bias, unsigned char rows,
                  unsigned char cols )
    : fnx::asset( file_path )
    , _format{ format }
    , _internal_format{ internal_format }
    , _filter{ filter }
    , _attachment{ attachment }
    , _clamp{ clamp }
    , _enable_mip{ enable_mip_mapping }
    , _mip_bias{ mip_bias }
    , _rows{ rows }
    , _cols{ cols }
    , _impl( new texture::impl() )
{
    _image.load_from_file( file_path, channels );
    init();
}

texture::texture( unsigned int width, unsigned int height, unsigned int channels, fnx::format format,
                  fnx::format internal_format, fnx::filter filter, fnx::attachment attachment, bool clamp, bool enable_mip_mapping,
                  float mip_bias,
                  unsigned char rows, unsigned char cols )
    : fnx::asset( "" )
    , _format{ format }
    , _internal_format{ internal_format }
    , _filter{ filter }
    , _attachment{ attachment }
    , _clamp{ clamp }
    , _enable_mip{ enable_mip_mapping }
    , _mip_bias{ mip_bias }
    , _rows{ rows }
    , _cols{ cols }
    , _impl( new texture::impl() )
{
    auto size = width * height * channels;
    _image.load_from_buffer( std::calloc( width * height * channels, sizeof( char ) ), size, width, height, channels );
    init();
}

void texture::init()
{
    const auto& info = _image.get_info();
    if ( !info._is_ok )
    {
        // nothing was loaded for this asset, who did this?!
        throw std::runtime_error( "texture file missing" );
    }

    // - mip-mapped texture files (.mpc, .mps, .mpb)
    if ( ends_with( get_name(), ".mpc" ) ||	// color
            ends_with( get_name(), ".mps" ) ||	// scalar
            ends_with( get_name(), ".mpb" ) )		// bump
    {
        // according to the wavefront api, these file types are mipmapped
        _enable_mip = true;
        _mip_bias = -.4f;
    }

    _tile_width = info._width / _cols;
    _tile_height = info._height / _rows;

    glGenTextures( 1, &_impl->_texture );
    glBindTexture( _impl->_texture_target, _impl->_texture );
    glEnable( _impl->_texture_target );

    if ( _clamp )
    {
        glTexParameteri( _impl->_texture_target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
        glTexParameteri( _impl->_texture_target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
    }
    else
    {
        glTexParameteri( _impl->_texture_target, GL_TEXTURE_WRAP_S, GL_REPEAT );
        glTexParameteri( _impl->_texture_target, GL_TEXTURE_WRAP_T, GL_REPEAT );
    }

    if ( _enable_mip )
    {
        glTexParameteri( _impl->_texture_target, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
        glTexParameteri( _impl->_texture_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
        glTexParameterf( _impl->_texture_target, GL_TEXTURE_LOD_BIAS, _mip_bias );
        glTexImage2D( _impl->_texture_target, 0, _internal_format, info._width, info._height, 0, _format, GL_UNSIGNED_BYTE,
                      info._data );

        GLfloat max;
        glGetFloatv( GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &max );
        glTexParameterf( _impl->_texture_target, GL_TEXTURE_MAX_ANISOTROPY_EXT, clamp( 0.f, 8.0f, max ) );
        glGenerateMipmap( _impl->_texture_target );
    }
    else
    {
        glTexParameteri( _impl->_texture_target, GL_TEXTURE_MIN_FILTER, _filter );
        glTexParameteri( _impl->_texture_target, GL_TEXTURE_MAG_FILTER, _filter );

        glTexImage2D( _impl->_texture_target, 0, _internal_format, info._width, info._height, 0, _format,
                      GL_UNSIGNED_BYTE, info._data );
    }

    glBindTexture( _impl->_texture_target, 0 ); // unbind
    GLenum draw_buffer;
    bool has_depth = false;

    if ( _attachment == GL_DEPTH_ATTACHMENT )
    {
        draw_buffer = GL_NONE;
        has_depth = true;
    }
    else if ( _attachment == GL_NONE )
    {
        return;
    }
    else
    {
        draw_buffer = _attachment;
    }

    if ( 0 == _impl->_frame_buffer )
    {
        glGenFramebuffers( 1, &_impl->_frame_buffer );
        glBindFramebuffer( GL_FRAMEBUFFER, _impl->_frame_buffer );
    }

    glFramebufferTexture2D( GL_FRAMEBUFFER, _attachment, _impl->_texture_target, _impl->_texture, 0 );

    if ( glCheckFramebufferStatus( GL_FRAMEBUFFER ) != GL_FRAMEBUFFER_COMPLETE || 0 == _impl->_frame_buffer )
    {
        return;
    }

    if ( !has_depth )
    {
        glGenRenderbuffers( 1, &_impl->_render_buffer );
        glBindRenderbuffer( GL_RENDERBUFFER, _impl->_render_buffer );
        glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH_COMPONENT, info._width, info._height );
        glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _impl->_render_buffer );
    }

    glDrawBuffer( draw_buffer );

    if ( glCheckFramebufferStatus( GL_FRAMEBUFFER ) != GL_FRAMEBUFFER_COMPLETE )
    {
        FNX_ERROR( "frame buffer creation failed" );
    }

    glBindFramebuffer( GL_FRAMEBUFFER, 0 );
    //TODO: initialize the texture for mip mapping
}

texture::~texture()
{
    if ( nullptr != _impl )
    {
        delete _impl;
    }

    _impl = nullptr;
}

fnx::vector2 texture::calc_atlas_offset( unsigned char index )
{
    fnx::vector2 ret{ 0.f, 0.f };
    if ( index < _rows * _cols )
    {
        int col = index % _cols;
        int row = index / _cols;

        ret.x = ( float )col * ( float )_tile_width;
        ret.y = ( float )row * ( float )_tile_height;
    }
    return ret;
}

void texture::pixel( unsigned int row, unsigned int col, char& r, char& g, char& b, char& a ) const
{
    const auto& info = _image.get_info();
    if ( info._size > 0 && info._height > 0 && info._width > 0 && row < info._height && col < info._height )
    {
        row = min( row, info._height - 1 );
        col = max( col, info._width - 1 );
        char* pixelOffset = &static_cast<char*>( info._data )[( min( row,
                            info._width - 1 ) * info._width + col ) * info._channels];
        r = ( info._channels >= 1 ) ? pixelOffset[0] : 0x00;
        g = ( info._channels >= 2 ) ? pixelOffset[1] : 0x00;
        b = ( info._channels >= 3 ) ? pixelOffset[2] : 0x00;
        a = ( info._channels >= 4 ) ? pixelOffset[3] : 0xff;
    }
}

void texture::set_render_target()
{
    const auto& info = _image.get_info();
    glBindTexture( GL_TEXTURE_2D, 0 );
    glBindFramebuffer( GL_FRAMEBUFFER, _impl->_frame_buffer );
    glViewport( 0, 0, info._width, info._height );
}

void texture::bind( unsigned int unit ) const
{
    assert( unit >= 0 && unit < 32 );
    glActiveTexture( GL_TEXTURE0 + unit );
    glBindTexture( _impl->_texture_target, _impl->_texture );
}
}