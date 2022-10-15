#pragma once

namespace fnx
{
	template<typename T>
	struct acquire_i
	{
		T& data;
		std::lock_guard<std::mutex> lock;
	};

	template<typename T>
	/// @brief Get a heap singleton that is thread safe.
	/// @usage auto [data, _] = singleton<Class>::acquire();
	class singleton
	{
	public:
		static acquire_i<T> acquire()
		{
			static std::unique_ptr<T> _instance = std::make_unique<T>();
			static std::mutex _mutex;
			return { *_instance, 
					 std::lock_guard<std::mutex>(_mutex) };
		}
	};
}