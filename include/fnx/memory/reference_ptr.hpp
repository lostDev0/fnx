#pragma once

namespace fnx
{
    template<typename T>
    /// @brief Constructs memory in a contiguous memory block and acts like a shared_ptr keeping references.
    ///     This class manages memory itself, therefor you must construct objects in place like make_shared<>.
    class reference_ptr
    {
    public:

        reference_ptr() {}
        reference_ptr(nullptr_t) {}

        //template <typename U, typename = typename std::enable_if<std::is_base_of<T, U>::value>::type>
        template<typename U>
        reference_ptr(const fnx::reference_ptr<U>& other)
        {
            if (nullptr != _asset_ptr)
            {
                reset();
            }

            _asset_ptr = static_cast<T*>(std::remove_const_t<U*>(other._asset_ptr));
            _ref_count = other._ref_count;
            _ptr_type_allocator = other._ptr_type_allocator;
            _ptr_counter_allocator = other._ptr_counter_allocator;

            if (nullptr != other._ref_count && nullptr != other._asset_ptr)
            {
                (*_ref_count)++;
            }
        }

        template<typename ...TArgs>
        reference_ptr<T>& create(TArgs&& ... args)
        {
            _asset_ptr = _type_allocator.create(std::forward<TArgs>(args)...);
            _ref_count = _counter_allocator.create();

            if (nullptr == _asset_ptr || nullptr == _ref_count)
            {
                throw;
            }

            *_ref_count = 1;
            return *this;
        }

        reference_ptr(const reference_ptr<T>& other)
        {
            if (nullptr != _asset_ptr)
            {
                reset();
            }

            _asset_ptr = other._asset_ptr;
            _ref_count = other._ref_count;
            _ptr_type_allocator = other._ptr_type_allocator;
            _ptr_counter_allocator = other._ptr_counter_allocator;

            if (nullptr != other._ref_count && nullptr != other._asset_ptr)
            {
                (*_ref_count)++;
            }
        }

        reference_ptr(reference_ptr<T>&& other)
        {
            if (nullptr != _asset_ptr)
            {
                reset();
            }

            _asset_ptr = other._asset_ptr;
            _ref_count = other._ref_count;
            _ptr_type_allocator = other._ptr_type_allocator;
            _ptr_counter_allocator = other._ptr_counter_allocator;

            if (nullptr != other._ref_count && nullptr != other._asset_ptr)
            {
                (*_ref_count)++;
            }

            // decrement the other asset so that the count stays the same
            other.reset();
        }

        ~reference_ptr()
        {
            reset();
        }

        reference_ptr<T>& operator=(const reference_ptr<T>& other)
        {
            reset();

            _asset_ptr = other._asset_ptr;
            _ref_count = other._ref_count;
            _ptr_type_allocator = other._ptr_type_allocator;
            _ptr_counter_allocator = other._ptr_counter_allocator;

            if (nullptr != other._ref_count && nullptr != other._asset_ptr)
            {
                (*_ref_count)++;
            }

            return *this;
        }

        reference_ptr<T>& operator=(reference_ptr<T>&& other)
        {
            reset();

            _asset_ptr = other._asset_ptr;
            _ref_count = other._ref_count;
            _ptr_type_allocator = other._ptr_type_allocator;
            _ptr_counter_allocator = other._ptr_counter_allocator;

            if (nullptr != other._ref_count && nullptr != other._asset_ptr)
            {
                (*_ref_count)++;
            }

            // decrement the other asset so that the count stays the same
            other.reset();

            return *this;
        }

        template <typename U, typename = typename std::enable_if<std::is_base_of<T, U>::value>::type>
        reference_ptr<T>& operator=(const reference_ptr<U>& other)
        {
            reset();

            _asset_ptr = other._asset_ptr;
            _ref_count = other._ref_count;
            _ptr_type_allocator = other._ptr_type_allocator;
            _ptr_counter_allocator = other._ptr_counter_allocator;

            if (nullptr != other._ref_count && nullptr != other._asset_ptr)
            {
                (*_ref_count)++;
            }

            return *this;
        }

        template <typename U, typename = typename std::enable_if<std::is_base_of<T, U>::value>::type>
        reference_ptr<T>& operator=(reference_ptr<U>&& other)
        {
            reset();

            _asset_ptr = other._asset_ptr;
            _ref_count = other._ref_count;
            _ptr_type_allocator = other._ptr_type_allocator;
            _ptr_counter_allocator = other._ptr_counter_allocator;

            if (nullptr != other._ref_count && nullptr != other._asset_ptr)
            {
                (*_ref_count)++;
            }

            // decrement the other asset so that the count stays the same
            other.reset();

            return *this;
        }

        reference_ptr<T>& operator=(nullptr_t ptr)
        {
            reset();
            return *this;
        }

        bool operator==(const reference_ptr<T>& other) const
        {
            return _asset_ptr == other._asset_ptr;
        }

        bool operator!=(const reference_ptr<T>& other) const
        {
            return !(*this == other);
        }

        explicit operator bool() const
        {
            return _asset_ptr != nullptr;
        }
        
        T& operator*() const
        {
            return *_asset_ptr;
        }

        T* operator->() const
        {
            return _asset_ptr;
        }

        T* get() const
        {
            return _asset_ptr;
        }

        unsigned int ref_count()
        {
            return nullptr == _ref_count ? 0 : *_ref_count;
        }

        /// @brief Releases a reference or destroy memory if this is the last reference.
        void reset()
        {
            if (nullptr != _asset_ptr)
            {
                // decrement the ref count and make sure the returned values are nulled so that
                // if this isn't the last reference, using this structure would still be invalid
                if (*_ref_count > 0)
                {
                    (*_ref_count)--;
                }

                if (0 == *_ref_count)
                {
                    _ptr_type_allocator->destroy(_asset_ptr);
                    _ptr_counter_allocator->destroy(_ref_count);
                }

                // disallow accessing the memory but keep the count
                _asset_ptr = nullptr;
            }
        }
    protected:
        static heap_pool_allocator<T> _type_allocator;
        static heap_pool_allocator<unsigned int> _counter_allocator;

        template<typename U>
        friend class fnx::reference_ptr;

        unsigned int* _ref_count{ nullptr };
        T* _asset_ptr{ nullptr };
        heap_pool_allocator_base* _ptr_type_allocator{ &_type_allocator };
        heap_pool_allocator_base* _ptr_counter_allocator{ &_counter_allocator };
    };

    template<typename T>
    heap_pool_allocator<T> reference_ptr<T>::_type_allocator;

    template<typename T>
    heap_pool_allocator<unsigned int> reference_ptr<T>::_counter_allocator;

    template<typename T, typename ...TArgs>
    /// @brief Create reference_ptr and send arguments to it.
    /// @return reference_ptr object
    /// @note Better performance if you just call create() on a reference_ptr.
    reference_ptr<T> make_shared_ref(TArgs&& ... args)
    {
        return reference_ptr<T>().create(std::forward<TArgs>(args)...);
    }

    template<typename T, typename U>
    fnx::reference_ptr<T> static_pointer_cast(const fnx::reference_ptr<U>& other)
    {
        fnx::reference_ptr<T> ret(other);
        /*
        ret._asset_ptr = static_cast<T*>(other.get());
        ret._ref_count = other._ref_count;
        ret._ptr_type_allocator = other._ptr_type_allocator;
        ret._ptr_counter_allocator = other._ptr_counter_allocator;

        if (0 < other.ref_count() && nullptr != other)
        {
            (*ret._ref_count)++;
        }
        */
        return ret;
    }
}