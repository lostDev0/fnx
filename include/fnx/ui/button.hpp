#pragma once

namespace fnx
{
	class button : public fnx::block
	{
	public:
		/// @brief Define an interactive block with colors that change due to mouse interactivety.
		button(const fnx::colors::rgba& color,
			const fnx::colors::rgba& highlight_color,
			const fnx::colors::rgba& press_color,
			const fnx::colors::rgba& checked_color,
			const std::string& name = create_type_name<fnx::button>())
			: block(color, highlight_color, press_color, checked_color, name)
		{
		}

		/// @brief Define a block where all states are the same color.
		button(const fnx::colors::rgba& color, const std::string& name = create_type_name<fnx::button>())
			: block(color, color, color, color, name)
		{
		}

		void set_icon(fnx::widget_handle_t<fnx::image> icon)
		{
			if (icon) add_widget(icon);
		}

		void set_label(fnx::widget_handle_t<fnx::label> label)
		{
			if (label) add_widget(label);
		}

		virtual ~button() = default;

		virtual bool do_mouse_release(FNX_BUTTON button, double x, double y) override;
	};
}