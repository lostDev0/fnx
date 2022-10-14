#pragma once

#include <ostream>

namespace fnx
{
	template<typename T, size_t default_size = 32u>
	class unordered_vector
	{
	public:
		class iter
		{
		public:
			iter(unordered_vector& v, size_t pos)
				: _vector{ v }
				, _pos{ pos }
			{}

			auto& operator++()
			{
				++_pos;
				return *this;
			}

			auto operator==(const unordered_vector<T, default_size>::iter& other) const -> bool { return _vector == other._vector && _pos == other._pos; }
			auto operator!=(const unordered_vector<T, default_size>::iter& other) const -> bool { return _vector != other._vector || _pos != other._pos; }
			T& operator*() { return _vector[_pos]; }
			const T& operator*() const { return _vector[_pos]; }
		private:

			friend unordered_vector;
			unordered_vector<T>& _vector;
			size_t _pos{ 0u };
		};

		unordered_vector()
			: _head{ std::malloc(default_size * sizeof(T)) }
			, _capacity{ default_size }
		{
			static_assert(default_size > 0);
		};

		~unordered_vector()
		{
			if (nullptr != _head)
			{
				for (auto i = 0u; i < _size; ++i)
				{
					(&_head[i])->~T();
				}
				delete[] _head;
			}
			_size = 0u;
		};

		unordered_vector(const std::initializer_list<T>& lst)
		{
			static_assert(default_size > 0);
			reserve(lst.size());
			for (const auto& l : lst)
			{
				emplace_back(l);
			}
		}

		unordered_vector(size_t capacity)
		{
			static_assert(default_size > 0);
			reserve(capacity);
		}

		void reserve(size_t size)
		{
			if (size > _capacity)
			{
				size > default_size ? size : _capacity + default_size;
				T* head = new T[size];
				_capacity = size;
				std::copy(_head, _head + _size, head);
				delete[] _head;
				_head = head;
			}
		}

		auto& operator[](size_t index)
		{
			assert(index < _size);
			return _head[index];
		}

		const auto& operator[](size_t index) const
		{
			assert(index < _size);
			return _head[index];
		}

		template<typename... Ts>
		auto& emplace_back(Ts&&... args)
		{
			reserve(_size + 1);
			new (&_head[_size]) T(std::forward<Ts>(args)...);
			return _head[_size++];
		}

		auto find(const T& key)
		{
			auto it = begin();
			auto it_end = end();
			while (it != it_end)
			{
				if (*it == key)
				{
					break;
				}
				++it;
			}
			return it;
		}

		auto size() const { return _size; }
		auto capacity() const { return _capacity; }

		auto begin() { return iter(*this, 0); }
		auto end() { return iter(*this, _size); }

		auto erase(const iter& it)
		{
			if (it._vector == *this)
			{
				if (it._pos < _size)
				{
					remove(it._pos);
					return iter(*this, it._pos);
				}
			}
			return end();
		}

		auto operator==(const unordered_vector<T, default_size>& other) const -> bool
		{
			if (size() != other.size())
			{
				return false;
			}
			for (auto j = 0u; j < size(); ++j)
			{
				if ((*this)[j] != other[j])
				{
					return false;
				}
			}
			return true;
		}

		auto operator!=(const unordered_vector<T, default_size>& other) const -> bool
		{
			return !((*this) == other);
		}

	private:
		T* _head{ nullptr };
		size_t _capacity{ 0u };
		size_t _size{ 0u };

		void remove(size_t idx)
		{
			if (idx < _size)
			{
				(&_head[idx])->~T();
				_head[idx] = _head[_size - 1];
				--_size;
			}
		}
	};

	template<typename CharT, typename Traits, typename T = float, size_t Size = 32u>
	inline std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& out, const fnx::unordered_vector<T, Size>& val)
	{
		out << std::string("fnx::unordered_vector{");
		for (auto i : val)
		{
			out << std::to_string(i) << std::string(",");
		}
		out << std::string("}");
		return out;
	}
}