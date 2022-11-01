#pragma once

namespace fnx
{
	class slider : public fnx::widget
	{
	public:

		slider(fnx::widget_handle_t<fnx::progress_bar> foreground, fnx::widget_handle_t<fnx::block> anchor, const std::string& name = create_type_name<slider>())
			: widget(name)
			, _foreground{ foreground }
			, _anchor{ anchor }
		{
			set_constraints(fnx::constraints{});	// container fills parent by default, reset it here
			if (foreground)
			{
				widget::add_widget(foreground);
			}
			if (anchor)
			{
				widget::add_widget(anchor);
			}
		}
		virtual ~slider() = default;

		/// @brief Not allowed.
		virtual void add_widget(widget_handle widget) override {};

		auto get_progress() { return _foreground ? _foreground->get_progress() : 0.f; }

		auto get_fill_direction() { return _foreground ? _foreground->get_fill_direction() : fill_direction::left_to_right; }

		virtual void update(double seconds) override;

	protected:

		virtual bool do_mouse_move(double x, double y) override;
		virtual bool do_mouse_press(FNX_BUTTON button, double x, double y) override;
		virtual bool do_mouse_release(FNX_BUTTON button, double x, double y) override;

		fnx::widget_handle_t<progress_bar> _foreground;
		fnx::widget_handle_t<block> _anchor;
	};
}