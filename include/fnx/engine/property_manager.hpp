#pragma once

namespace fnx
{
	class property_manager
	{
	public:
		template<typename T> auto get_property(const std::string& key) { return _properties[key].get<T>(); }
		template<typename T> auto get_property(const char* key) { return _properties[key].get<T>(); }
		template<typename T> auto set_property(const std::string& key, const T& val) { _properties[key].set<T>(val); }
		template<typename T> auto set_property(const char* key, const T& val) { _properties[key].set<T>(val); }
	private:
		struct cmp_str
		{
			bool operator()(const char* a, const char* b) const
			{
				return std::strcmp(a, b) < 0;
			}
		};
		std::unordered_map<std::string, fnx::property> _properties;
	};
}