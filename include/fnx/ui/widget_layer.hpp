#pragma once

namespace fnx
{
	/// @brief A single hierarchy of widgets to be rendered and processed in the order they were added.
	/// @note It is the expectation that entities are never removed and only destroyed when the layer is destroyed.
	class widget_layer : public layer
	{
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

		auto get_root() { return _root; }

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

		unsigned int _active_widget{ 0xffffffff };		
	};
}