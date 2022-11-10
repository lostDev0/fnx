using namespace fnx;
using namespace std;

std::string fnx::ltrim( const std::string& str )
{
    const std::string delims( " \t\r\n" );
    std::string ret;
    size_t pos = 0u;

    for ( auto c : str )
    {
        bool found = false;

        for ( auto d : delims )
        {
            if ( c == d )
            {
                found = true;
                break;
            }
        }

        if ( found )
        {
            // still white space
            pos++;
            continue;
        }
        else
        {
            break;
        }
    }

    // populate return string
    ret.assign( begin( str ) + pos, end( str ) );
    return ret;
}

std::string fnx::rtrim( const std::string& str )
{
    const std::string delims( " \t\r\n" );
    std::string ret;
    size_t pos = 0u;
    auto it = str.rbegin();
    auto it_end = str.rend();

    for ( ; it != it_end; ++it )
    {
        bool found = false;

        for ( auto d : delims )
        {
            if ( *it == d )
            {
                found = true;
                break;
            }
        }

        if ( found )
        {
            // still white space
            pos++;
            continue;
        }
        else
        {
            break;
        }
    }

    // populate return string
    ret.assign( begin( str ), end( str ) - pos );
    return ret;
}

std::string fnx::trim( const std::string& str )
{
    return ltrim( rtrim( str ) );
}

std::vector<std::string> fnx::split( const std::string& str, const std::string& delims, bool compress_delimeter )
{
    std::vector<std::string> ret;
    std::string current;
    bool last_char_is_delim = false;

    for ( auto c : str )
    {
        bool found = false;

        for ( auto d : delims )
        {
            if ( c == d )
            {
                // current char is a delimeter
                found = true;
                break;
            }
        }

        if ( !found )
        {
            current += c;
            last_char_is_delim = false;
        }
        else
        {
            // current char is a delimeter
            if ( ( !compress_delimeter ) || ( !last_char_is_delim && compress_delimeter ) )
            {
                ret.emplace_back( current );
            }

            // ignore character if compress_delimeter is true
            current.clear();
            last_char_is_delim = true;
        }
    }

    if ( current.size() > 0 )
    {
        ret.emplace_back( current );
    }

    return ret;
}

std::string fnx::get_date_time( const std::string& format )
{
    std::time_t now = std::chrono::system_clock::to_time_t( std::chrono::system_clock::now() );
    std::unique_ptr<char[]> buffer( new char[32] );
    std::strftime( &buffer.get()[0], 32, "%Y-%m-%d %H:%M:%S", std::localtime( &now ) );
    return std::string( buffer.get() );
}

bool fnx::starts_with( const std::string& source, const std::string& key )
{
    if ( source.find( key ) == 0 )
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool fnx::ends_with( const std::string& source, const std::string& key )
{
    if ( source.find( key ) == source.size() - key.size() )
    {
        return true;
    }
    else
    {
        return false;
    }
}
