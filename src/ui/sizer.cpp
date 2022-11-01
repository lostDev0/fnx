namespace fnx
{
	void grid_sizer::resize()
	{
		auto width_ratio = 1.f / static_cast<float>(_cols);
		auto height_ratio = 1.f / static_cast<float>(_rows);

		for (auto i = 0; i < _children.size(); ++i)
		{
			size_t row = i / _cols;
			size_t col = i % _cols;
			auto x_ratio = static_cast<float>(col)* (width_ratio);
			auto y_ratio = static_cast<float>(row)* (height_ratio);
			constraints limits;
			limits.add(fill_horz_constraint{ width_ratio });
			limits.add(fill_vert_constraint{ height_ratio });
			limits.add(relative_horz_constraint{ x_ratio });
			limits.add(relative_vert_constraint{ y_ratio });			

			auto left_pad = _center_pad;
			auto top_pad = _center_pad;
			auto right_pad = _center_pad;
			auto bottom_pad = _center_pad;

			if (row == 0)
			{
				top_pad = _top_pad;
			}
			if (row == _rows - 1)
			{
				bottom_pad = _bottom_pad;
			}
			if (col == 0)
			{
				left_pad = _left_pad;
			}
			if (col == _cols - 1)
			{
				right_pad = _right_pad;
			}			

			limits.add(padding_constraint{ left_pad, top_pad, right_pad, bottom_pad });
			_children[i]->set_constraints(limits);
		}
	}

	void vert_sizer::resize()
	{
		auto total = 0.f;
		auto unit_height = 0.f;
		if (_children.size() > 0)
		{
			unit_height = get_height() / _children.size();
		}

		if (_proportions.size() > 0)
		{
			for (auto v : _proportions)
			{
				total += v;
			}

			if (total > 0)
			{
				unit_height = get_height() / total;	// 1 proportional unit equals a fraction of the sizer's height
			}
		}	

		auto height_ratio = 1.f / static_cast<float>(_rows);
		auto y_ratio = 0.f;

		for (auto i = 0; i < _children.size(); ++i)
		{
			height_ratio = (_proportions[i] * unit_height) / get_height();
			
			constraints limits;
			limits.add(fill_horz_constraint{ 1.f });
			limits.add(fill_vert_constraint{ height_ratio }); 
			limits.add(relative_horz_constraint{ 0.f });
			limits.add(relative_vert_constraint{ vert_sizer::direction::top_to_bottom == _fill_direction ? 1.f - (y_ratio + height_ratio) : y_ratio });
			
			auto left_pad = _left_pad;
			auto top_pad = _center_pad;
			auto right_pad = _right_pad;
			auto bottom_pad = _center_pad;

			if (i == 0)
			{
				vert_sizer::direction::top_to_bottom == _fill_direction ? top_pad = _top_pad : bottom_pad = _bottom_pad;
			}
			if (i == _rows - 1)
			{
				vert_sizer::direction::top_to_bottom == _fill_direction ? bottom_pad = _bottom_pad : top_pad = _top_pad;
			}

			limits.add(padding_constraint{ left_pad, top_pad, right_pad, bottom_pad });
			_children[i]->set_constraints(limits);

			y_ratio += height_ratio;
		}
	}

	void horz_sizer::resize()
	{
		auto total = 0.f;
		auto unit_width = 0.f;
		if (_children.size() > 0)
		{
			unit_width = get_width() / _children.size();
		}

		if (_proportions.size() > 0)
		{
			for (auto v : _proportions)
			{
				total += v;
			}

			if (total > 0)
			{
				unit_width = get_width() / total;	// 1 proportional unit equals a fraction of the sizer's width
			}
		}

		auto width_ratio = 1.f / static_cast<float>(_cols);
		auto x_ratio = 0.f;

		for (auto i = 0; i < _children.size(); ++i)
		{
			width_ratio = (_proportions[i] * unit_width) / get_width();

			constraints limits;
			limits.add(fill_horz_constraint{ width_ratio });
			limits.add(fill_vert_constraint{ 1.f });
			limits.add(relative_vert_constraint{ 0.f });
			limits.add(relative_horz_constraint{ horz_sizer::direction::right_to_left == _fill_direction ? 1.f - (x_ratio + width_ratio) : x_ratio });

			auto left_pad = _center_pad;
			auto top_pad = _top_pad;
			auto right_pad = _center_pad;
			auto bottom_pad = _bottom_pad;

			if (i == 0)
			{
				horz_sizer::direction::right_to_left == _fill_direction ? right_pad = _right_pad : left_pad = _left_pad;
			}
			if (i == _cols - 1)
			{
				horz_sizer::direction::right_to_left == _fill_direction ? left_pad = _left_pad : right_pad = _right_pad;
			}

			limits.add(padding_constraint{ left_pad, top_pad, right_pad, bottom_pad });
			_children[i]->set_constraints(limits);

			x_ratio += width_ratio;
		}
	}

	void vert_fit_sizer::resize()
	{
		auto y_ratio = 0.f;
		for (auto i = 0; i < _children.size(); ++i)
		{
			auto height_ratio = _children[i]->get_height() / get_height();
			auto& limits = _children[i]->get_constraints();
			limits.clear_post_process();

			auto left_pad = _left_pad;
			auto top_pad = _center_pad;
			auto right_pad = _right_pad;
			auto bottom_pad = _center_pad;

			if (i == 0)
			{
				vert_sizer::direction::top_to_bottom == _fill_direction ? top_pad = _top_pad : bottom_pad = _bottom_pad;
			}
			if (i == _rows - 1)
			{
				vert_sizer::direction::top_to_bottom == _fill_direction ? bottom_pad = _bottom_pad : top_pad = _top_pad;
			}

			limits.add_post_process(relative_horz_constraint{ vert_sizer::direction::top_to_bottom == _fill_direction ? 1.f - (y_ratio + height_ratio) : y_ratio });
			limits.add_post_process(padding_constraint{ left_pad, top_pad, right_pad, bottom_pad });

			y_ratio += height_ratio;
		}
	}

	void horz_fit_sizer::resize()
	{
		auto x_ratio = 0.f;
		for (auto i = 0; i < _children.size(); ++i)
		{
			auto width_ratio = _children[i]->get_width() / get_width();
			auto& limits = _children[i]->get_constraints();
			limits.clear_post_process();
			
			auto left_pad = _center_pad;
			auto top_pad = _top_pad;
			auto right_pad = _center_pad;
			auto bottom_pad = _bottom_pad;

			if (i == 0)
			{
				horz_sizer::direction::right_to_left == _fill_direction ? right_pad = _right_pad : left_pad = _left_pad;
			}
			if (i == _cols - 1)
			{
				horz_sizer::direction::right_to_left == _fill_direction ? left_pad = _left_pad : right_pad = _right_pad;
			}

			limits.add_post_process(relative_horz_constraint{ horz_sizer::direction::right_to_left == _fill_direction ? 1.f - (x_ratio + width_ratio) : x_ratio });
			limits.add_post_process(padding_constraint{ left_pad, top_pad, right_pad, bottom_pad });

			x_ratio += width_ratio;
		}
	}

	void sizer::on_parent_change(float parent_x, float parent_y, float parent_width, float parent_height)
	{
		block::on_parent_change(parent_x, parent_y, parent_width, parent_height);
		resize();
		for (auto& c : _children)
		{
			c->on_parent_change(get_x(), get_y(), get_width(), get_height());
		}
	}
}
