#pragma once
#include "json/json.hpp"

namespace fnx
{
	namespace serialize
	{
		template <typename Class, typename T>
		using member_ptr_t = T Class::*;

		template <typename T>
		using setter_func_t = function_ref<void(const T&)>;

		// disregard the const here as function ref doesn't yet know to use a member function pointer
		template <typename T>
		using getter_func_t = function_ref<const T & ()>;

		template <typename T>
		using getter_val_func_t = function_ref<T ()>;

		template <typename Class, typename T>
		class member {
		public:
			using class_type = Class;
			using member_type = T;

			using member_ptr_t = T Class::*;

			using setter_func_t = function_ref<void(const T&)>;

			// disregard the const here as function ref doesn't yet know to use a member function pointer
			using getter_func_t = function_ref<const T & ()>;

			using getter_val_func_t = function_ref<T ()>;

			member() = delete;
			~member() = default;
			member(const char* name, member_ptr_t ptr) : _name{ name }, _ptr{ ptr } {}
			member(const char* name, getter_func_t getter_ptr, setter_func_t setter_ptr) : _name{ name }, _getter{ getter_ptr }, _setter{ setter_ptr } {}
			member(const char* name, getter_val_func_t getter_ptr, setter_func_t setter_ptr) : _name{ name }, _getter_val{ getter_ptr }, _setter{ setter_ptr } {}

			/*
			const T& get(Class& obj) const
			{
				if (nullptr != _getter)
					return _getter(obj);
				else if (has_ptr())
					return obj.*_ptr;
				throw std::runtime_error("no getter provided");
			}
			*/

			T get(Class& obj) const
			{
				if (_getter)
					return _getter(obj);
				else if (_getter_val)
					return _getter_val(obj);
				else if (has_ptr())
					return obj.*_ptr;
				throw std::runtime_error("no getter provided");
			}

			template <typename V, typename = std::enable_if_t<std::is_constructible<T, V>::value>>
			void set(Class& obj, V&& value) const
			{
				if (has_setter())
					_setter(obj, value);
				else if (has_ptr())
					obj.*_ptr = value;
				else
					throw std::runtime_error("no setter provided");
			}

			auto get_name() const { return _name; }
			bool has_ptr() const { return nullptr != _ptr; }
			bool has_getter() const { return _getter || _getter_val; }
			bool has_setter() const { return _setter; }
		private:
			const char* _name;
			member_ptr_t _ptr{ nullptr };
			setter_func_t _setter;
			getter_func_t _getter;
			getter_val_func_t _getter_val;
		};

		template <typename Class, typename T>
		auto make_member(const char* name, T Class::* member)
		{
			return serialize::member<Class, T>(name, member);
		}

		template <typename Class, typename ParamType>
		auto make_member(const char* name, const ParamType& (Class::* getter_func)() const,
			void (Class::* setter_func)(const ParamType&))
		{
			function_ref<const ParamType & ()> getter(getter_func);
			function_ref<void(const ParamType&)> setter(setter_func);
			return serialize::member<Class, ParamType>(name, getter, setter);
		}

		template <typename Class, typename ParamType>
		auto make_member(const char* name, ParamType (Class::* getter_func)() const,
			void (Class::* setter_func)(const ParamType&))
		{
			function_ref<ParamType ()> getter(getter_func);
			function_ref<void(const ParamType&)> setter(setter_func);
			return serialize::member<Class, ParamType>(name, getter, setter);
		}

		template<typename... Ts>
		auto make_members(Ts&&... args)
		{
			return std::make_tuple(std::forward<Ts>(args)...);
		}

		template<typename T>
		inline auto register_members() { return make_members(); }

		template<typename T>
		inline auto register_name() { return ""; }

		template<typename T, typename TupleType>
		struct member_holder
		{
			static TupleType members;
			static auto name() { return serialize::register_name<T>(); }
		};

		template<typename T, typename TupleType>
		TupleType member_holder<T, TupleType>::members = register_members<T>();

		template <typename T>
		constexpr auto get_name()
		{
			return member_holder<T, decltype(serialize::register_members<T>())>::name();
		}

		template<typename T>
		constexpr bool is_registered()
		{
			return !std::is_same<std::tuple<>, decltype(serialize::register_members<T>())>::value;
		}

		template <typename MemberType>
		using get_member_type = typename std::decay_t<MemberType>::member_type;

		template<typename T>
		const auto& get_members()
		{
			return member_holder<T, decltype(serialize::register_members<T>())>::members;
		}

		template<typename T, typename F, typename = std::enable_if_t<serialize::is_registered<T>()>>
		void for_all_members(F&& f)
		{
			for_tuple(std::forward<F>(f), serialize::get_members<T>());
		}

		template <typename T, typename F, 
			typename = std::enable_if_t<!serialize::is_registered<T>()>,
			typename = void>
		void for_all_members(F&&)
		{
			// for unregistered
		}

		template<typename T, 
			     typename = std::enable_if_t<serialize::is_registered<T>()>>
		nlohmann::json serialize(T& obj)
		{
			nlohmann::json value;
			serialize::for_all_members<T>(
				[&obj, &value](auto& member) 
				{
					// create a json entry with the member's name
					auto& json_entry = value[member.get_name()];
					// get the member's value
					auto val = member.get(obj);
					//json_entry = val;
					if constexpr (serialize::is_registered<decltype(val)>())
						json_entry = serialize::serialize(val);
					else
						json_entry = serialize::serialize_basic(val);
				});
			return value;
		}

		template <typename T, 
			      typename = std::enable_if_t<!serialize::is_registered<T>()>, 
			      typename = void>
		nlohmann::json serialize(T& obj)
		{
			return serialize_basic(obj);
		}

		template <typename T>
		nlohmann::json serialize_basic(T& obj)
		{
			return nlohmann::json(obj);
		}

		// specialization for std::vector
		template <typename T>
		nlohmann::json serialize_basic(std::vector<T>& obj)
		{
			nlohmann::json value;
			int i = 0;

			if constexpr (serialize::is_registered<T>())
			{
				for (T& elem : obj) 
				{
					value[i] = serialize::serialize(elem);
					++i;
				}
			}
			else
			{
				for (T& elem : obj)
				{
					value[i] = elem;
					++i;
				}
			}
			return value;
		}

		// specialization for std::unordered_map
		template <typename K, typename V>
		nlohmann::json serialize_basic(std::unordered_map<K, V>& obj)
		{
			nlohmann::json value;
			for (auto& pair : obj) {
				value.emplace(fnx::to_string(pair.first), pair.second);
			}
			return value;
		}

		template <typename K, typename V>
		nlohmann::json serialize_basic(std::map<K, V>& obj)
		{
			nlohmann::json value;
			for (auto& pair : obj) {
				value.emplace(fnx::to_string(pair.first), pair.second);
			}
			return value;
		}

		template<typename T, typename = std::enable_if_t<serialize::is_registered<T>()>>
		void deserialize(const nlohmann::json& j, T& obj)
		{
			if (j.is_object())
			{
				serialize::for_all_members<T>(
					[&obj, &j](auto& member)
					{
						auto name = member.get_name();
						if (j.contains(name))
						{
							auto& json_entry = j[name];
							using member_t = get_member_type<decltype(member)>;
							if constexpr (!serialize::is_registered<member_t>())
							{
								//member.set(obj, json_entry.template get<member_t>());
								member_t tmp;
								serialize::deserialize_basic(json_entry, tmp);
								member.set(obj, tmp);
							}
							else
							{
								member_t tmp;
								serialize::deserialize(json_entry, tmp);
								member.set(obj, tmp);
							}
						}
					});
			}
		}
		
		template <typename T,
			typename = std::enable_if_t<!serialize::is_registered<T>()>, 
			typename = void>
		void deserialize(const nlohmann::json& object, T& obj)
		{
			deserialize_basic(object, obj);
		}

		template<typename T, typename = std::enable_if_t<!std::is_stl_container<T>::value>>
		void deserialize_basic(const nlohmann::json& object, T& obj)
		{
			obj = object.get<T>();
		}

		// specialization for std::vector
		template <typename T>
		void deserialize_basic(const nlohmann::json& object, std::vector<T>& obj)
		{
			if constexpr (serialize::is_registered<T>())
			{
				obj.reserve(object.size()); // vector.resize() works only for default constructible types
				for (auto& elem : object)
				{
					obj.emplace_back();
					serialize::deserialize<T>(elem, obj.back());
				}
			}
			else
			{
				obj.reserve(object.size()); // vector.resize() works only for default constructible types
				for (auto& elem : object)
				{
					obj.push_back(elem); // push rvalue
				}
			}
		}

		// specialization for std::unodered_map
		template <typename K, typename V>
		void deserialize_basic(const nlohmann::json& object, std::unordered_map<K, V>& obj)
		{

			if constexpr (serialize::is_registered<V>())
			{
				for (auto it = object.begin(); it != object.end(); ++it)
				{
					V& target = obj[fnx::from_string<K>(it.key())];
					serialize::deserialize<V>(it.value(), target);
				}
			}
			else
			{
				for (auto it = object.begin(); it != object.end(); ++it) {
					obj.emplace(fnx::from_string<K>(it.key()), it.value());
				}
			}			
		}

		// specialization for std::map
		template <typename K, typename V>
		void deserialize_basic(const nlohmann::json& object, std::map<K, V>& obj)
		{

			if constexpr (serialize::is_registered<V>())
			{
				for (auto it = object.begin(); it != object.end(); ++it)
				{
					V& target = obj[fnx::from_string<K>(it.key())];
					serialize::deserialize<V>(it.value(), target);
				}
			}
			else
			{
				for (auto it = object.begin(); it != object.end(); ++it) {
					obj.emplace(fnx::from_string<K>(it.key()), it.value());
				}
			}
		}

		template <typename T, typename = std::enable_if_t<serialize::is_registered<T>()>>
		void to_json(nlohmann::json& j, T& obj)
		{
			j = serialize::serialize(obj);
		}

		template <typename T, typename = std::enable_if_t<serialize::is_registered<T>()>>
		nlohmann::json to_json(T& obj)
		{
			return serialize::serialize(obj);
		}

		template <typename T, typename = std::enable_if_t<serialize::is_registered<T>()>>
		void from_json(const nlohmann::json& j, T& obj)
		{
			serialize::deserialize<T>(j, obj);
		}

		template <typename T, typename = std::enable_if_t<!serialize::is_registered<T>()>, typename = void>
		void to_json(nlohmann::json& j, T& obj)
		{
			j = nlohmann::json();
		}

		template <typename T, typename = std::enable_if_t<!serialize::is_registered<T>()>, typename = void>
		nlohmann::json to_json(T& obj)
		{
			return nlohmann::json();
		}

		template <typename T, typename = std::enable_if_t<!serialize::is_registered<T>()>, typename = void>
		void from_json(const nlohmann::json& j, T& obj)
		{
		}
	}

#define SERIALIZE(type)	\
	template<typename type> \
	inline void from_json(nlohmann::json& data, type& obj) { serialize::from_json(data, obj); }	\
	inline nlohmann::json to_json(type& obj) const { return serialize::to_json(obj); }

// Define the serialization helper functions required for the command client implementation. This implementation
// must exist within the cpp file and not in the header as it would be defined multiple times.
#define SERIALIZE_DECLARE_MEMBERS()	\
	virtual const char* serialize_name();	\
	virtual void serialize_from_json(nlohmann::json& data);	\
	virtual nlohmann::json serialize_to_json();

// Declare the serialization helper functions required for the command client implementation.
#define SERIALIZE_DEFINE_MEMBERS(type)	\
	const char* type##::serialize_name() { return serialize::get_name<type>(); }	\
	void type##::serialize_from_json(nlohmann::json& data) { serialize::from_json(data, *this); }	\
	nlohmann::json type##::serialize_to_json() { return serialize::to_json(*this); }

	/*
		Example:

		// .hpp
		struct serializable
		{
			SERIALIZE_DECLARE_MEMBERS();
		};

		// .cpp
		SERIALIZE_DEFINE_MEMBERS(serializable);
	
	*/
}