#pragma once

#include <type_traits>

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
}