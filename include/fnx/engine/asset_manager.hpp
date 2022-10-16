#pragma once

#include <type_traits>

namespace fnx
{
    template<typename T, typename std::enable_if<std::is_base_of<fnx::asset, T>::value, T>::type * = nullptr>
    /// @brief Manage assets of a particular type.
    class asset_manager
    {
        std::mutex _lock;
        std::unordered_map<std::string, fnx::reference_ptr<T>> _assets;
    public:
        asset_manager() = default;
        ~asset_manager() { release_all(); };
        
        /// @brief Returns the number of managed assets of this type.
        auto count() { return _assets.size(); }

        template<typename... TArgs>
        /// @brief Return an asset or create it if it doesn't exist.
        /// @param[in] asset_name : unique name of an asset of a given type
        /// @param[in] args : construction arguments
        /// @note args is ignored if the asset is already created
        fnx::asset_handle<T> get(const std::string& asset_name, TArgs&& ... args)
        {
            std::lock_guard<std::mutex> guard(_lock);
            auto& ref = _assets[asset_name];
            if (nullptr == ref.get())
            {
                // not yet constructed, construct a new asset
                ref = fnx::make_shared_ref<T>(asset_name, std::forward<TArgs>(args)...);
            }
            return ref;
        }

        /// @brief Returns an asset or copies a provided asset
        /// @param[in] asset_name : unique name of an asset of a given type
        /// @param[in] base : asset to be copied
        /// @note args is ignored if the asset is already created
        fnx::asset_handle<T> provide(const std::string& asset_name, const T& base)
        {
            std::lock_guard<std::mutex> guard(_lock);
            auto& ref = _assets[asset_name];
            if (nullptr == ref.get())
            {
                // copy construct a new object matching the provided struct
                ref = fnx::make_shared_ref<T>(asset_name, base);
            }
            return ref;
        }

        /// @brief Reclaim handles and memory for a single asset.
        /// @param[in] asset_name : unique name of an asset of a given type
        void release(const std::string& asset_name)
        {
            std::lock_guard<std::mutex> guard(_lock);
            _assets.erase(asset_name);
        }

        /// @brief Reclaim handles and memory for any unused assets.
        void release_not_used()
        {
            std::lock_guard<std::mutex> guard(_lock);
            auto it_end = _assets.end();
            auto it = _assets.begin();

            for (; it != it_end; )
            {
                if (it->second.use_count() == 1)
                {
                    it = _assets.erase(it);
                    it_end = _assets.end();
                    continue;
                }

                ++it;
            }
        }

        /// @brief Reclaim handles and memory for all assets.
        /// @warning This does not reset any asset handles used within the engine.
        /// @note this needs to be fixed!
        void release_all()
        {
            std::lock_guard<std::mutex> guard(_lock);
            _assets.clear();
        }

        /// @brief Return all assets.
        /// @note use this carefully
        auto& get_all()
        {
            return _assets;
        }
    };
}