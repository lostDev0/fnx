#pragma once

namespace fnx
{
    /// @brief A single hierarchy of widgets to be rendered and processed in the order they were added.
    /// @note It is the expectation that entities are never removed and only destroyed when the layer is destroyed.
    class widget_layer : public layer
    {
        friend serializer<fnx::widget_layer>;
        fnx::widget_handle _root{fnx::create_widget<fnx::widget>()};		/// top level widget container that has no visual representation
        bool _dirty_cache{ true };	/// flag indicating the widgets need to be redrawn
    public:
        widget_layer(const std::string& name = create_type_name<fnx::widget_layer>());
        virtual ~widget_layer();

        virtual void update(double delta) override;
        virtual void render(camera_handle camera) override;

        virtual void show() override
        {
            //layer::show();
            // pass event to all children
            _root->activate();
            _root->show();
        }

        virtual void hide() override
        {
            if (_root->is_visible())
            {
                // pass event to all children
                _root->hide();
                _root->inactivate();
            }
        }

        /// @brief Add a widget to the layer.
        /// @note cannot remove a widget from the layer
        widget_handle add_widget(widget_handle w);

        /// @brief Returns nullptr or a widget handle to the given widget name.
        widget_handle get_widget(const std::string& widget_name);

        auto get_root() const { return _root; }

        virtual bool on_event(const window_init_evt& event) override;
        virtual bool on_event(const window_close_evt& event) override;
        virtual bool on_event(const window_resize_evt& event) override;
        virtual bool on_event(const window_move_evt& event) override;
        virtual bool on_event(const window_minimize_evt& event) override;
        virtual bool on_event(const window_maximize_evt& event) override;
        virtual bool on_event(const window_fullscreen_evt& event) override;
        virtual bool on_event(const window_lose_focus_evt& event) override;
        virtual bool on_event(const window_gain_focus_evt& event) override;
        virtual bool on_event(const keyboard_press_evt& event) override;
        virtual bool on_event(const keyboard_release_evt& event) override;
        virtual bool on_event(const keyboard_repeat_evt& event) override;
        virtual bool on_event(const mouse_enter_evt& event) override;
        virtual bool on_event(const mouse_exit_evt& event) override;
        virtual bool on_event(const mouse_move_evt& event) override;
        virtual bool on_event(const mouse_press_evt& event) override;
        virtual bool on_event(const mouse_release_evt& event) override;
        virtual bool on_event(const mouse_scroll_evt& event) override;

    protected:

        template<typename event_type>
        bool send_event_to_widgets(const event_type& evt)
        {
            bool ret = false;
            auto reverse_it = _root->get_children().rbegin();
            auto reverse_end = _root->get_children().rend();
            for (; reverse_it != reverse_end; ++reverse_it)
            {
                // if you need an "invisible" widget, make the appearance invisible but don't set the visible flag to false
                if ((*reverse_it)->is_interactive() && (*reverse_it)->is_visible())
                {
                    ret |= (*reverse_it)->on_event(evt);
                    if (ret)
                    {
                        break;
                    }
                }
            }
            return ret;
        }

        bool on_widget_active(const widget_active_evt&);
        bool on_widget_inactive(const widget_inactive_evt&);

        /// TODO this may need to create widget ids that are only ever increasing
        unsigned int _active_widget{ 0xffffffff };		
    };

    template<>
    struct serializer<fnx::widget_layer>
    {
        static inline void to_yaml(std::string& content, const fnx::widget_layer& obj)
        {
            YAML::Emitter out;
            to_yaml(out, obj);
            content = out.c_str();
        }

        static inline void from_yaml(const std::string& content, fnx::widget_layer& obj)
        {
            YAML::Node data = YAML::Load(content);
            from_yaml(data, obj);
        }

        static inline void to_yaml(YAML::Emitter& out, const fnx::widget_layer& obj)
        {
            serializer<fnx::layer>::to_yaml(out, obj);
            out << YAML::Key << "dirty_cache" << YAML::Value << true;
            out << YAML::Key << "active_widget" << YAML::Value << obj._active_widget;
            serializer<fnx::widget>::to_yaml(out, *obj.get_root());
        }

        static inline void from_yaml(const YAML::Node& data, fnx::widget_layer& obj)
        {
            serializer<fnx::layer>::from_yaml(data, obj);
            obj._dirty_cache = data["dirty_cache"].as<decltype(obj._dirty_cache)>();
            obj._active_widget = data["active_widget"].as<decltype(obj._active_widget)>();
            serializer<fnx::widget>::from_yaml(data, *obj._root);
        }
    };
}