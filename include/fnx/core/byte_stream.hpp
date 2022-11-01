#pragma once
#include <cassert>

namespace fnx
{
	class byte_stream
	{
	public:

		byte_stream();
		byte_stream(const std::string& file);
		virtual ~byte_stream();

		template<typename T>
		byte_stream& operator<<(const T& val)
		{
			if (_stream.size() + sizeof(T) >= _stream.capacity())
			{
				_stream.reserve(static_cast<size_t>(static_cast<float>(_stream.size() + sizeof(T))* g_reserve_multilier));
			}

			_stream.insert(_stream.end(),
				reinterpret_cast<const char*>(&val),
				reinterpret_cast<const char*>(&val) + sizeof(T));
			/*
			for ( auto i = 0u; i < sizeof( T ); i++ )
			{
				_stream.emplace_back( 0 );
			}

			T* val_ptr = reinterpret_cast<T*>( &_stream[_stream.size() - sizeof( T )] );
			*val_ptr = val;
			*/
			return *this;
		}

		template<typename T>
		byte_stream& operator>>(T& val)
		{
			assert(_idx + sizeof(T) <= _stream.size());
			std::memcpy(&val, &_stream[_idx], sizeof(T));
			/*
			T* val_ptr = reinterpret_cast<T*>( &_stream[_idx] );
			val = *val_ptr;
			*/
			_idx += sizeof(val);
			return (*this);
		}

		byte_stream& operator<<(const char* str)
		{
			return *this << std::string(str);
		}

		byte_stream& operator<<(const std::string& val)
		{
			if (_stream.size() + val.size() + sizeof(unsigned int) >= _stream.capacity())
			{
				_stream.reserve(static_cast<size_t>(static_cast<float>(_stream.size() + val.size() + sizeof(
					unsigned int))* g_reserve_multilier));
			}

			*this << static_cast<unsigned int>(val.size());

			for (const auto& c : val)
			{
				_stream.emplace_back(c);
			}

			return (*this);
		}

		template<typename T>
		byte_stream& operator<<(const std::vector<T>& array)
		{
			// unsigned int : num_vals
			// T * n : data
			if (_stream.size() + array.size() * sizeof(T) + sizeof(unsigned int) >= _stream.capacity())
			{
				_stream.reserve(static_cast<size_t>(static_cast<float>(_stream.size() + array.size() * sizeof(T) + sizeof(
					unsigned int))* g_reserve_multilier));
			}

			*this << static_cast<unsigned int>(array.size());

			for (const auto& c : array)
			{
				*this << c;
			}

			return (*this);
		}

		template<>
		byte_stream& operator>>(std::string& val)
		{
			assert(_idx + sizeof(unsigned int) <= _stream.size());
			unsigned int size{ 0u };
			(*this) >> size;
			assert(_idx + size <= _stream.size());
			val.assign(&_stream[_idx], size);
			_idx += size;
			return (*this);
		}

		template<typename T>
		byte_stream& operator>>(std::vector<T>& array)
		{
			// unsigned int : num_vals
			// T * n : data
			unsigned int num_vals;
			T val;
			*this >> num_vals;
			assert(_idx + (sizeof(T) * num_vals) <= _stream.size());
			array.reserve(num_vals);

			for (auto i = 0u; i < num_vals; i++)
			{
				*this >> val;
				array.emplace_back(val);
			}

			return (*this);
		}

		/// @brief Reset the stream index pointer to the first element.
		void reset()
		{
			_idx = 0;
		}

		auto begin()
		{
			return std::begin(_stream);
		}

		auto end()
		{
			return std::end(_stream);
		}

		template<typename T>
		/// @brief Look at the upcoming value.
		/// @return returns false if there is not enough data in the stream.
		bool peek(T& val)
		{
			if (_idx + sizeof(T) > _stream.size())
			{
				return false;
			}

			*this >> val;
			_idx -= sizeof(T);
			return true;
		}

		template<typename T>
		/// @brief Look at the previous value.
		/// @return returns false if there is not enough data in the stream.
		bool peek_back(T& val)
		{
			if (_idx < sizeof(T))
			{
				return false;
			}

			_idx -= sizeof(T);
			*this >> val;
			return true;
		}

		/// @brief Remove all data within the stream.
		void clear()
		{
			_stream.clear();
		}

		auto size()
		{
			return _stream.size();
		}

		auto position()
		{
			return _idx;
		}

		const auto& data()
		{
			return _stream;
		}

		/// @brief Clears the stream and repopulates data from that of a file path.
		/// @note This is primarily for re-use of the stream.
		byte_stream& from_file(const std::string& file_path);

		/// @brief Construct a stream from char array.
		/// @note This will clear the Stream if it has any data.
		byte_stream& from_byte_array(const char* data, unsigned int size);

	protected:
	private:

		std::vector<char> _stream;
		unsigned int _idx{ 0u };

		const float g_reserve_multilier{ 1.2f };
		const unsigned int g_min_stream_size{ 1024u };
	};
}