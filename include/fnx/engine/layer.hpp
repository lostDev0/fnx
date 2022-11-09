#pragma once

namespace fnx
{
    class layer_stack;
    /// @brief Layers act as the main interaction with game objects and widgets.
    /// @note these should be used like an interface
    class layer
    {
    protected:

        friend serializer<fnx::layer>;
        friend serializer<fnx::layer_stack>;
        bool _visible{ false };
        std::string _name;  /// identifier of this layer
    public:
        layer(const std::string& name = fnx::create_type_name<fnx::layer>())
            : _name(name)
        {};
        virtual ~layer() {};

        /// @brief Process an update cycle from the engine.
        /// @param[in] delta : difference between cycles in seconds
        virtual void update(double delta) { }

        /// @brief Process a render cycle from the engine.
        virtual void render(camera_handle camera) {}

        /// @brief Get the name of this layer.
        /// @return string : name of the layer
        const auto& get_name() const { return _name; }

        /// @brief Make the layer visible.
        virtual void show() { _visible = true; }

        /// @brief Make the layer invisible.
        virtual void hide() { _visible = false; }

        /// @brief Returns whether the layer is visible or not.
        /// @return bool : true if visible
        auto is_visible() const { return _visible; }

        virtual bool on_event(const window_init_evt& event) { return false; }
        virtual bool on_event(const window_close_evt& event) { return false; }
        virtual bool on_event(const window_resize_evt& event) { return false; }
        virtual bool on_event(const window_move_evt& event) { return false; }
        virtual bool on_event(const window_minimize_evt& event) { return false; }
        virtual bool on_event(const window_maximize_evt& event) { return false; }
        virtual bool on_event(const window_fullscreen_evt& event) { return false; }
        virtual bool on_event(const window_lose_focus_evt& event) { return false; }
        virtual bool on_event(const window_gain_focus_evt& event) { return false; }
        virtual bool on_event(const keyboard_press_evt& event) { return false; }
        virtual bool on_event(const keyboard_release_evt& event) { return false; }
        virtual bool on_event(const keyboard_repeat_evt& event) { return false; }
        virtual bool on_event(const mouse_enter_evt& event) { return false; }
        virtual bool on_event(const mouse_exit_evt& event) { return false; }
        virtual bool on_event(const mouse_move_evt& event) { return false; }
        virtual bool on_event(const mouse_press_evt& event) { return false; }
        virtual bool on_event(const mouse_release_evt& event) { return false; }
        virtual bool on_event(const mouse_scroll_evt& event) { return false; }
    };

    using layer_handle = fnx::reference_ptr<fnx::layer>;

    template<>
    struct serializer<fnx::layer>
    {
        static inline void to_yaml(std::string& content, const fnx::layer& obj)
        {
            YAML::Emitter out;
            out << YAML::BeginMap;
            out << YAML::Key << "name" << YAML::Value << obj.get_name();
            out << YAML::Key << "visible" << YAML::Value << obj.is_visible();
            out << YAML::EndMap;
            content = out.c_str();
        }

        static inline void from_yaml(const std::string& content, fnx::layer& obj)
        {
            YAML::Node data = YAML::Load(content);
            obj._name = data["name"].as<std::string>();
            obj._visible = data["visible"].as<bool>();
        }
    };
}
