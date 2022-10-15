#pragma once

#include <type_traits>
#include <set>
#include <queue>
#include <unordered_set>
#include <stack>
#include <forward_list>
#include <map>
#include <unordered_map>

namespace fnx
{
	template <typename, typename = void>
	struct extract_signature;

	template <typename Ret, typename... Args>
	struct extract_signature<Ret(Args...), void> {
		using type = Ret(Args...);
	};
	template <typename Ret, typename... Args>
	struct extract_signature<Ret(*)(Args...), void> {
		using type = Ret(Args...);
	};
	template <typename Ret, typename... Args>
	struct extract_signature<Ret(&)(Args...), void> {
		using type = Ret(Args...);
	};

	template <typename Ret, typename C, typename... Args>
	struct extract_signature<Ret(C::*)(Args...), void> {
		using type = Ret(Args...);
	};
	template <typename Ret, typename C, typename... Args>
	struct extract_signature<Ret(C::*)(Args...) const, void> {
		using type = Ret(Args...);
	};
	template <typename Ret, typename C, typename... Args>
	struct extract_signature<Ret(C::*)(Args...)&, void> {
		using type = Ret(Args...);
	};
	template <typename Ret, typename C, typename... Args>
	struct extract_signature<Ret(C::*)(Args...) const&, void> {
		using type = Ret(Args...);
	};
	template <typename Ret, typename C, typename... Args>
	struct extract_signature<Ret(C::*)(Args...)&&, void> {
		using type = Ret(Args...);
	};
	template <typename Ret, typename C, typename... Args>
	struct extract_signature<Ret(C::*)(Args...) const&&, void> {
		using type = Ret(Args...);
	};

	template <typename Func>
	struct extract_signature<Func, std::void_t<decltype(&Func::operator())>> {
		using type = typename extract_signature<decltype(&Func::operator())>::type;
	};

	/// @brief Used to break up function protype information into compile time accessible fields.
	template <typename F>
	struct ArgType;

	template <typename Ret, typename... Args>
	struct ArgType<Ret(*)(Args...)> {
		static constexpr auto nargs = sizeof...(Args);
		using result_type = Ret;
		using argument_types = std::tuple<Args...>;
		using signature = Ret(Args...);

		template <size_t N>
		struct get_arg {
			using type = typename std::tuple_element<N, std::tuple<Args...>>::type;
		};
	};

	template <typename Class, typename Ret, typename... Args>
	struct ArgType<Ret(Class::*)(Args...)> {
		static constexpr auto nargs = sizeof...(Args);
		using result_type = Ret;
		using argument_types = std::tuple<Args...>;
		using signature = Ret(Args...);
		using class_type = Class;

		template <size_t N>
		struct get_arg {
			using type = typename std::tuple_element<N, std::tuple<Args...>>::type;
		};
	};

	template <typename Class, typename Ret, typename... Args>
	struct ArgType<Ret(Class::*)(Args...) const> {
		static constexpr auto nargs = sizeof...(Args);
		using result_type = Ret;
		using argument_types = std::tuple<Args...>;
		using signature = Ret(Args...);
		using class_type = Class;
		template <size_t N>
		struct get_arg {
			using type = typename std::tuple_element<N, std::tuple<Args...>>::type;
		};
	};

	template <typename Base, typename T, typename... Ts>
	struct are_base_of :
		std::conditional<std::is_base_of<Base, T>::value, are_base_of<Base, Ts...>,
		std::false_type>::type
	{};

	template <typename Base, typename T>
	struct are_base_of<Base, T> : std::is_base_of<Base, T> {};

	/// @brief Executes a function on all arguments.
	template <typename Func, typename... Args>
	void for_each_arg(Func&& f, Args&& ... args)
	{
		((void)f(args), ...);
	}

	/// @brief Executes a function on all tuple members.
	template <typename Func, typename TupleType>
	void for_tuple(Func&& f, TupleType&& tuple)
	{
		apply(
			[&f](auto&& ... elems) {
				for_each_arg(f,
					std::forward<decltype(elems)>(elems)...);
			},
			std::forward<TupleType>(tuple));
	}

	/// @brief Executes a function on all arguments if the static template analysis is true.
	template <bool Test, typename Func, typename... Args>
	void call_if(Func&& f, Args&& ... args)
	{
		f(std::forward<Args>(args)...);
	}

	template<typename T, typename R, typename... As>
	void* void_cast(R(T::* f)(As...))
	{
		union
		{
			R(T::* pf)(As...);
			void* p;
		};
		pf = f;
		return p;
	}
}

namespace std
{
	template<typename T>
	bool remove_if_contains(std::vector<T>& v, const T& i)
	{
		auto it_end = std::end(v);
		auto it = std::find(std::begin(v), std::end(v), i);
		if (it != it_end)
		{
			v.erase(it);
			return true;
		}
		return false;
	}

	template<typename T>
	bool add_if_missing(std::vector<T>& v, const T& i)
	{
		auto it_end = std::end(v);
		auto it = std::find(std::begin(v), std::end(v), i);
		if (it == it_end)
		{
			v.emplace_back(i);
			return true;
		}
		return false;
	}

	//specialize a type for all of the STL containers.
	namespace is_stl_container_impl {
		template <typename T>       struct is_stl_container :std::false_type {};		
		template <typename... Args> struct is_stl_container<std::vector            <Args...>> :std::true_type {};
		template <typename... Args> struct is_stl_container<std::deque             <Args...>> :std::true_type {};
		template <typename... Args> struct is_stl_container<std::list              <Args...>> :std::true_type {};
		template <typename... Args> struct is_stl_container<std::forward_list      <Args...>> :std::true_type {};
		template <typename... Args> struct is_stl_container<std::set               <Args...>> :std::true_type {};
		template <typename... Args> struct is_stl_container<std::multiset          <Args...>> :std::true_type {};
		template <typename... Args> struct is_stl_container<std::map               <Args...>> :std::true_type {};
		template <typename... Args> struct is_stl_container<std::multimap          <Args...>> :std::true_type {};
		template <typename... Args> struct is_stl_container<std::unordered_set     <Args...>> :std::true_type {};
		template <typename... Args> struct is_stl_container<std::unordered_multiset<Args...>> :std::true_type {};
		template <typename... Args> struct is_stl_container<std::unordered_map     <Args...>> :std::true_type {};
		template <typename... Args> struct is_stl_container<std::unordered_multimap<Args...>> :std::true_type {};
		template <typename... Args> struct is_stl_container<std::stack             <Args...>> :std::true_type {};
		template <typename... Args> struct is_stl_container<std::queue             <Args...>> :std::true_type {};
		template <typename... Args> struct is_stl_container<std::priority_queue    <Args...>> :std::true_type {};

		template <typename T>       struct is_stl_map :std::false_type {};
		template <typename... Args> struct is_stl_map<std::map               <Args...>> :std::true_type {};
		template <typename... Args> struct is_stl_map<std::multimap          <Args...>> :std::true_type {};
		template <typename... Args> struct is_stl_map<std::unordered_map     <Args...>> :std::true_type {};
		template <typename... Args> struct is_stl_map<std::unordered_multimap<Args...>> :std::true_type {};

		template <typename T>       struct is_stl_array :std::false_type {};
		template <typename T, std::size_t N> struct is_stl_array<std::array    <T, N>> :std::true_type {};
	}

	//type trait to utilize the implementation type traits as well as decay the type
	template <typename T> struct is_stl_container {
		static constexpr bool const value = is_stl_container_impl::is_stl_container<std::decay_t<T>>::value;
	};

	template <typename T> struct is_stl_map {
		static constexpr bool const value = is_stl_container_impl::is_stl_map<std::decay_t<T>>::value;
	};

	template <typename T> struct is_stl_array {
		static constexpr bool const value = is_stl_container_impl::is_stl_array<std::decay_t<T>>::value;
	};
}