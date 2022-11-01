#pragma once

namespace fnx
{
	/// @brief Used to provide widget look and feel attributes.
	struct widget_config
	{
		fnx::colors::rgba _colors[static_cast<size_t>(fnx::widget::state::max)]{ vector4{0.f,0.f,0.f,1.f} };	/// color for each state
		fnx::colors::rgba _outline_colors[static_cast<size_t>(fnx::widget::state::max)];	/// color for each state
		float _outline_thickness[static_cast<size_t>(fnx::widget::state::max)];	/// border thickness for each state
		fnx::tween<vector4> _gradients[static_cast<size_t>(fnx::widget::state::max)];	/// gradient for each state
		fnx::widget::fill_direction _gradient_directions[static_cast<size_t>(fnx::widget::state::max)]{ fnx::widget::fill_direction::left_to_right };	/// gradient direction for each state
	};
}