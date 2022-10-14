#pragma once
#pragma warning ( suppress : 4716 )

#include <cassert>
#include <type_traits>

namespace fnx
{
    /*
    // https://stackoverflow.com/questions/38288042/c11-14-invoke-workaround
    template <typename Fn, typename... Args,
        typename = std::enable_if_t<std::is_member_pointer<std::decay_t<Fn>>::value>,
        int = 0>
        constexpr auto invoke(Fn&& f, Args&&... args) noexcept(
            noexcept(std::mem_fn(f)(std::forward<Args>(args)...)))
        -> decltype(std::mem_fn(f)(std::forward<Args>(args)...)) {
        return std::mem_fn(f)(std::forward<Args>(args)...);
    }

    template <typename Fn, typename... Args,
        typename = std::enable_if_t < !std::is_member_pointer<std::decay_t<Fn>>{} >>
        constexpr auto invoke(Fn&& f, Args&&... args) noexcept(
            noexcept(std::forward<Fn>(f)(std::forward<Args>(args)...)))
        -> decltype(std::forward<Fn>(f)(std::forward<Args>(args)...)) {
        return std::forward<Fn>(f)(std::forward<Args>(args)...);
    }
    */

    template <typename F>
    class function_ref;

    template <typename rtype, typename... args>
    class function_ref<rtype(args...)> {
    public:
        using Functor = rtype(*)(args...);
        using MyType = function_ref<rtype(args...)>;

        function_ref() : _obj{ nullptr }, _func{ nullptr } {
            _callback = [](void* obj, void* fn, args... xs) -> rtype { assert(false); return rtype{}; };
        }

        constexpr function_ref(const function_ref& other) noexcept
        {
            _obj = other._obj;
            _func = (other._func == other._buf) ? _buf : other._func;
            _callback = other._callback;
            std::memcpy(_buf, other._buf, sizeof _buf);
        }

        constexpr function_ref& operator=(const function_ref& other) noexcept
        {
            _obj = other._obj;
            _func = (other._func == other._buf) ? _buf : other._func;
            _callback = other._callback;
            std::memcpy(_buf, other._buf, sizeof _buf);
            return *this;
        };

        function_ref(Functor func)
            : _obj{ nullptr }, _func{ reinterpret_cast<void*>(func) }
        {
            _callback = [](void*, void* fn, args... xs) -> rtype {
                return std::invoke(reinterpret_cast<Functor>(fn), std::forward<args>(xs)...);
            };
        }

        template <typename Func, std::enable_if_t<std::is_class_v<std::decay_t<Func>> &&
            !std::is_same_v<function_ref, std::decay_t<Func>>> * = nullptr>
        function_ref(Func&& f) : _obj{ nullptr }, _func{ std::addressof(f) }
        {
            _callback = [](void* obj, void* fn, args... xs) -> rtype {
                return std::invoke(
                    *reinterpret_cast<typename std::add_pointer<Func>::type>(fn),
                    std::forward<args>(xs)...);
            };
        }

        template <typename Func, typename C = typename ArgType<Func>::class_type,
            std::enable_if_t<std::is_class<C>::value &&
            std::is_member_function_pointer<Func>::value> * = nullptr>
        function_ref(Func f) : _obj{ nullptr }, _func{ _buf }
        {
            static_assert(sizeof(f) <= sizeof(_buf));
            std::memcpy(_buf, &f, sizeof(f));
            _callback = [](void* obj, void* fn, args... xs) -> rtype {
                return std::invoke(
                    *reinterpret_cast<typename std::add_pointer<Func>::type>(fn),
                    *static_cast<C*>(obj), std::forward<args>(xs)...);
            };
        }

        template <typename C, typename Func,
            std::enable_if_t<std::is_class<C>::value &&
            std::is_member_function_pointer<Func>::value> * = nullptr>
        function_ref(C& o, Func f)
            : _obj{ std::addressof(o) }, _func{ _buf }
        {
            static_assert(sizeof(f) <= sizeof(_buf));
            std::memcpy(_buf, &f, sizeof(f));
            _callback = [](void* obj, void* fn, args... xs) -> rtype {
                return std::invoke(
                    *reinterpret_cast<typename std::add_pointer<Func>::type>(fn),
                    *static_cast<C*>(obj), std::forward<args>(xs)...);
            };
        }

        template <typename C, typename Func,
            std::enable_if_t<std::is_class<C>::value &&
            std::is_member_function_pointer<Func>::value> * = nullptr>
        void bind(C& o, Func f) {
            _obj = std::addressof(o);
            _func = _buf;
            static_assert(sizeof(f) <= sizeof(_buf));
            std::memcpy(_buf, &f, sizeof(f));
            _callback = [](void* obj, void* fn, args... xs) -> rtype {
                return std::invoke(
                    *reinterpret_cast<typename std::add_pointer<Func>::type>(fn),
                    *static_cast<C*>(obj), std::forward<args>(xs)...);
            };
        }

        void bind(Functor func)
        {
            _obj = nullptr;
            _func = reinterpret_cast<void*>(func);
            _callback = [](void*, void* fn, args... xs) -> rtype {
                return std::invoke(reinterpret_cast<Functor>(fn), std::forward<args>(xs)...);
            };
        }

        template <typename Func, typename = std::enable_if_t<!std::is_member_function_pointer<std::decay_t<Func>>{} >>
        void bind(Func&& f)
        {
            _obj = nullptr;
            _func = std::addressof(f);
            _callback = [](void* obj, void* fn, args... xs) -> rtype {
                return std::invoke(
                    *reinterpret_cast<typename std::add_pointer<Func>::type>(fn),
                    std::forward<args>(xs)...);
            };
        }

        template <typename C, typename Func>
        void bind(Func f)
        {
            _obj = nullptr;
            _func = _buf;
            static_assert(sizeof(f) <= sizeof(_buf));
            std::memcpy(_buf, &f, sizeof(f));
            _callback = [](void* obj, void* fn, args... xs) -> rtype {
                return std::invoke(
                    *reinterpret_cast<typename std::add_pointer<Func>::type>(fn),
                    *static_cast<C*>(obj), std::forward<args>(xs)...);
            };
        }

        rtype operator()(args... xs) const {
            return _callback(_obj, _func, std::forward<args>(xs)...);
        }

        template <typename C>
        rtype operator()(C& obj, args... xs) const {
            return _callback(std::addressof(obj), _func, std::forward<args>(xs)...);
        }
        
        template <typename C>
        rtype operator()(const C& obj, args... xs) const {
            return _callback(std::addressof(obj), _func, std::forward<args>(xs)...);
        }
        
        auto operator==(const MyType& other) const {
            if (_obj == nullptr && other._obj == nullptr)
                return _func == other._func;	// function
            else if (_obj == other._obj)
                return std::equal(std::begin(_buf), std::end(_buf), std::begin(other._buf), std::end(other._buf));	// member
            return false;
        }

        auto operator!=(const MyType& other) const {
            return !(*this == other);
        }

        operator bool() const { return _func != nullptr; }

    private:
        void* _obj = nullptr;
        void* _func = nullptr;
        rtype(*_callback)(void*, void*, args...) = nullptr;  // object, function, arguments
        unsigned char _buf[4 * sizeof(void*)]{};
    };

    template <typename Func>
    auto bind(Func&& f) {
        return fnx::function_ref<typename fnx::extract_signature<Func>::type>(f);
    }
    template <typename Obj, typename Func>
    auto bind(Obj& o, Func f) {
        return fnx::function_ref<typename fnx::extract_signature<Func>::type>(o, f);
    }
}