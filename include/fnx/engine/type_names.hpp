#pragma once

namespace fnx
{
class entity;
class material;
class model;
class font;
class shader;
class texture;
class raw_model;
class layer;

template<typename T>
inline const char* get_type_name()
{
    static const auto name = typeid( T ).name();
    return name;
}

template<>
inline const char* get_type_name<fnx::shader>()
{
    static const auto name = "shader";
    return name;
}

template<>
inline const char* get_type_name<fnx::model>()
{
    static const auto name = "model";
    return name;
}

template<>
inline const char* get_type_name<fnx::entity>()
{
    static const auto name = "entity";
    return name;
}

template<>
inline const char* get_type_name<fnx::material>()
{
    static const auto name = "material";
    return name;
}

template<>
inline const char* get_type_name<fnx::font>()
{
    static const auto name = "font";
    return name;
}

template<>
inline const char* get_type_name<fnx::layer>()
{
    static const auto name = "layer";
    return name;
}

template<typename T>
struct type_name
{
    static const char* name;
};

template<typename T>
const char* type_name<T>::name = fnx::get_type_name<T>();

template<typename T>
std::string create_type_name()
{
    static unsigned int id = 0;
    static const std::string name = ( std::string( fnx::type_name<T>::name ) + std::string( "_" ) );
    return name + std::to_string( id++ );
}
}