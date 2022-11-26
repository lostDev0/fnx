#pragma once

namespace fnx
{
union property
{
#define MAX_PROP_STR_LEN 32

    property() : str_val()
    {
        str_val.reserve( MAX_PROP_STR_LEN );
    }
    ~property() {};
    char c_val;
    short s_val;
    int i_val;
    long l_val;
    unsigned char uc_val;
    unsigned short us_val;
    unsigned int ui_val;
    unsigned long ul_val;
    float f_val;
    double d_val;
    long long ll_val;
    unsigned long long ull_val;
    vector2 v2_val;
    vector3 v3_val;
    vector4 v4_val;
    std::string str_val;

    template<typename T>
    T get()
    {
        return *this;
    }

    template<>char get()
    {
        return c_val;
    }
    template<>short get()
    {
        return s_val;
    }
    template<>int get()
    {
        return i_val;
    }
    template<>long get()
    {
        return l_val;
    }
    template<>float get()
    {
        return f_val;
    }
    template<>double get()
    {
        return d_val;
    }
    template<>long long get()
    {
        return ll_val;
    }
    template<>unsigned char get()
    {
        return uc_val;
    }
    template<>unsigned short get()
    {
        return us_val;
    }
    template<>unsigned int get()
    {
        return ui_val;
    }
    template<>unsigned long get()
    {
        return ul_val;
    }
    template<>unsigned long long get()
    {
        return ull_val;
    }
    template<>vector2 get()
    {
        return v2_val;
    }
    template<>vector3 get()
    {
        return v3_val;
    }
    template<>vector4 get()
    {
        return v4_val;
    }

    template<>std::string get()
    {
        return str_val;
    }

    template<typename T>void set( const T& val )
    {
        throw std::runtime_error( "Invalid property type" );
    }

    template<>void set( const char& val )
    {
        c_val = val;
    }
    template<>void set( const short& val )
    {
        s_val = val;
    }
    template<>void set( const int& val )
    {
        i_val = val;
    }
    template<>void set( const long& val )
    {
        l_val = val;
    }
    template<>void set( const float& val )
    {
        f_val = val;
    }
    template<>void set( const double& val )
    {
        d_val = val;
    }
    template<>void set( const long long& val )
    {
        ll_val = val;
    }
    template<>void set( const unsigned char& val )
    {
        uc_val = val;
    }
    template<>void set( const unsigned short& val )
    {
        us_val = val;
    }
    template<>void set( const unsigned int& val )
    {
        ui_val = val;
    }
    template<>void set( const unsigned long& val )
    {
        ul_val = val;
    }
    template<>void set( const unsigned long long& val )
    {
        ull_val = val;
    }
    template<>void set( const vector2& val )
    {
        v2_val = val;
    }
    template<>void set( const vector3& val )
    {
        v3_val = val;
    }
    template<>void set( const vector4& val )
    {
        v4_val = val;
    }

    template<>void set( const std::string& val )
    {
        str_val = val;
    }

    template<typename T>
    void operator=( const T& other )
    {
        set<T>( other );
    }
};

constexpr auto PROPERTY_WORLD_BACKGROUND = "world_bg";
constexpr auto PROPERTY_FOG_COLOR_DAY = "fog_color_day";
constexpr auto PROPERTY_FOG_COLOR_NIGHT = "fog_color_night";
constexpr auto PROPERTY_FOG_COLOR = "fog_color";
constexpr auto PROPERTY_DAY_NIGHT_BLEND_FACTOR = "day_night_blend";
constexpr auto PROPERTY_FOG_DENSITY = "fog_density";
constexpr auto PROPERTY_FOG_GRADIENT = "fog_gradient";
constexpr auto PROPERTY_GAMMA = "gamma";
constexpr auto PROPERTY_NEAR_PLANE = "near_plane";
constexpr auto PROPERTY_FAR_PLANE = "far_plane";
constexpr auto PROPERTY_GRAVITY = "gravity";
constexpr auto PROPERTY_TIME_OF_DAY = "time";
}
