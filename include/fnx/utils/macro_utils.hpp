#pragma once

namespace fnx
{
/// Creates get_"name" and set_"name"
#define CREATE_ACCESSORS(type, name)						\
    type name;												\
    const type& get##name() const { return name; }			\
    void set##name(const type& value) { name = value; }

/// Creates get_"name" and set_"name"
#define CREATE_VIRTUAL_ACCESSORS(type, name)						\
    type name;														\
    virtual const type& get##name() const { return name; }			\
    virtual void set##name(const type& value) { name = value; }

/// Creates get_"name" and set_"name"
#define CREATE_ACCESSORS_WITH_DEFAULT(type, name, default_value)	\
    type name{default_value};										\
    const type& get##name() const { return name; }					\
    void set##name(const type& value) { name = value; }

/// Creates get_"name" and set_"name" and get_mutable_"name"
#define CREATE_CONTAINER_ACCESSORS(container_type, type, name)				\
    container_type<type> name;												\
    const container_type<type>& get##name() const { return name; }			\
    container_type<type>& get_mutable##name() { return name; }				\
    void set##name(const container_type<type>& value) { name = value; }

/// Creates get_"name" and set_"name" and get_mutable_"name"
#define CREATE_MAP_ACCESSORS(container_type, key_type, value_type, name)					\
    container_type<key_type,value_type> name;												\
    const container_type<key_type,value_type>& get##name() const { return name; }			\
    container_type<key_type,value_type>& get_mutable##name() { return name; }				\
    void set##name(const container_type<key_type,value_type>& value) { name = value; }
}