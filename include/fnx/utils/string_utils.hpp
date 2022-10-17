#pragma once
#include <string>
#include <vector>

namespace fnx
{
	/// @brief Removes leading white space from the string.
	extern std::string ltrim(const std::string& str);

	/// @brief Removes the trailing white space from the string.
	extern std::string rtrim(const std::string& str);

	/// @brief Removes trailing and leading white space from the string.
	extern std::string trim(const std::string& str);

	/// @brief Split a string at delimeters.
	extern std::vector<std::string> split(const std::string& str, const std::string& delims = std::string(" "),
		bool compress_delimeter = false);

	/// @brief Return the current date time in the given format.
	extern std::string get_date_time(const std::string& format);

	/// @brief Returns true if a string begins with another string.
	extern bool starts_with(const std::string& source, const std::string& key);

	/// @brief Returns true if a string ends with another string.
	extern bool ends_with(const std::string& source, const std::string& key);

	template<typename T>
	inline std::string to_string(const T& in)
	{
		return std::to_string(in);
	}

	template<>
	inline std::string to_string(const std::string& in)
	{
		return in;
	}

	template<typename T>
	inline T from_string(const std::string& in)
	{
		return static_cast<T>(std::stoll(in));
	}

	template<>
	inline float from_string(const std::string& in)
	{
		return std::stof(in);
	}

	template<>
	inline double from_string(const std::string& in)
	{
		return std::stod(in);
	}

	template<>
	inline std::string from_string(const std::string& in)
	{
		return in;
	}

	namespace detail
	{
		template<typename Type> auto convert_string_internal(Type&& t)
		{
			if constexpr (std::is_same<std::remove_cv_t<std::remove_reference_t<Type>>, std::string>::value) {
				return std::forward<Type>(t).c_str();
			}
			else {
				return std::forward<Type>(t);
			}
		}

		template<typename... Args> auto format_string_internal(const std::string& format, Args&& ... args)
		{
			auto size = snprintf(nullptr, 0, format.c_str(), args...) + 1u;
			std::unique_ptr<char[]> buffer(new char[size]);
			(void)snprintf(buffer.get(), size, format.c_str(), args...);
			return std::string(buffer.get());
		}
	}

	template<typename... Args> auto format_string(const std::string& format, Args&& ... args)
	{
		return detail::format_string_internal(format, detail::convert_string_internal(std::forward<Args>(args))...);
	}
};