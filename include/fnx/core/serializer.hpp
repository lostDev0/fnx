#pragma once

namespace fnx
{
template<typename T>
struct serializer
{
    static inline void to_yaml( std::string& content, const T& obj ) {}
    static inline void from_yaml( const std::string& content, T& obj ) {}
    static inline void to_yaml( YAML::Emitter& content, const T& obj ) {}
    static inline void from_yaml( const YAML::Node& content, T& obj ) {}
};
}