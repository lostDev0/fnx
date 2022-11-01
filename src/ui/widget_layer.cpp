namespace fnx
{
	widget_layer::widget_layer(const std::string& name)
		: layer(name)
	{
		// widget layer is considered always visible to the layer stack, however, the _root widget determines rendering and interactivity
		layer::show();
		_root->reset_constraints();
		_root->set_size(2.f, 2.f);
		_root->set_position(-1.f, -1.f);
		_root->inactivate();
		_root->set_visibility(false);
		auto [events, _] = singleton<event_manager>::acquire();
		events.subscribe<widget_active_evt>(fnx::bind(*this, &widget_layer::on_widget_active));
		events.subscribe<widget_inactive_evt>(fnx::bind(*this, &widget_layer::on_widget_inactive));
	}

	widget_layer::~widget_layer()
	{
	}

	bool widget_layer::on_widget_active(const widget_active_evt& evt)
	{
		auto handle = get_widget_by_id(evt._src);
		if (_active_widget != evt._src && _active_widget)
		{
			auto active = get_widget_by_id(_active_widget);
			if (active && active->is_active())
			{
				FNX_DEBUG(fnx::format_string("inactivate: %d, %s", active->get_id(), active->get_name()));
				active->inactivate();
			}
		}

		_active_widget = evt._src;
		return false;
	}

	bool widget_layer::on_widget_inactive(const widget_inactive_evt& evt)
	{
		FNX_DEBUG(fnx::format_string("inactivate: %d", evt._src));
		_active_widget = 0xffffffff;
		return false;
	}

	widget_handle widget_layer::add_widget(widget_handle w)
	{
		if (w)
		{
			_root->add_widget(w);
			_dirty_cache = true;
			return w;
		}
		return fnx::widget_handle{};
	}

	widget_handle widget_layer::get_widget(const std::string& widget_name)
	{
		for (auto child : _root->get_children())
		{
			if (child->get_name() == widget_name)
			{
				return child;
			}
			else if(child->has_children())
			{
				auto ret = child->get_widget(widget_name);
				if (ret)
					return child;
			}
		}
		return nullptr;
	}

	void widget_layer::update(double delta)
	{
		if (_visible)
		{
			_root->update(delta);
		}
	}

	void widget_layer::render(camera_handle camera)
	{
		if (_root->is_visible())
		{
			auto [renderer, _] = singleton<fnx::renderer>::acquire();
			auto mode = renderer.is_wireframe();
			renderer.set_fill_mode();
			renderer.disable_depth_test();
			renderer.enable_blend();
			renderer.blend_src_alpha_one_minus_src_alpha();
			_root->render(camera, matrix4x4::identity());
			renderer.disable_blend();
			renderer.enable_depth_test();
			if (true == mode)
			{
				renderer.set_wireframe_mode();
			}
		}
	}

	bool widget_layer::on_event(const window_init_evt& event)
	{
		_root->on_parent_change(0.f, 0.f, 2.f, 2.f);
		return false;
	}

	bool widget_layer::on_event(const window_close_evt& event)
	{
		_root->clear();
		return false;
	}

	bool widget_layer::on_event(const window_resize_evt& event)
	{
		_root->on_parent_change(0.f, 0.f, 2.f, 2.f);
		return false;
	}

	bool widget_layer::on_event(const window_move_evt& event)
	{
		_root->on_parent_change(0.f, 0.f, 2.f, 2.f);
		return false;
	}

	bool widget_layer::on_event(const window_minimize_evt& event) { return false; }
	bool widget_layer::on_event(const window_maximize_evt& event) { return false; }
	bool widget_layer::on_event(const window_fullscreen_evt& event) { return false; }
	bool widget_layer::on_event(const window_lose_focus_evt& event) { return false; }
	bool widget_layer::on_event(const window_gain_focus_evt& event)
	{
		return false;
	}

	bool widget_layer::on_event(const keyboard_press_evt& event)
	{
		if(_root->is_interactive())
			return send_event_to_widgets(event);
		return false;
	}
	
	bool widget_layer::on_event(const keyboard_release_evt& event)
	{
		if (_root->is_interactive())
			return send_event_to_widgets(event);
		return false;
	}

	bool widget_layer::on_event(const keyboard_repeat_evt& event)
	{
		if (_root->is_interactive())
			return send_event_to_widgets(event);
		return false;
	}

	bool widget_layer::on_event(const mouse_enter_evt& event)
	{
		return false;
	}

	bool widget_layer::on_event(const mouse_exit_evt& event) { return false; }
	bool widget_layer::on_event(const mouse_move_evt& event)
	{
		if (_root->is_interactive())
			return send_event_to_widgets(event);
		return false;
	}

	bool widget_layer::on_event(const mouse_press_evt& event)
	{
		if (_root->is_interactive())
			return send_event_to_widgets(event);
		return false;
	}

	bool widget_layer::on_event(const mouse_release_evt& event)
	{
		if (_root->is_interactive())
			return send_event_to_widgets(event);
		return false;
	}

	bool widget_layer::on_event(const mouse_scroll_evt& event)
	{
		if (_root->is_interactive())
			return send_event_to_widgets(event);
		return false;
	}
}
