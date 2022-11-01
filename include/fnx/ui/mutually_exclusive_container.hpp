#pragma once

namespace fnx
{
	/// @brief Size the sizer according to the children it was provided
	class mutually_exclusive_container : public fnx::widget
	{
	public:
		mutually_exclusive_container(const std::string& name = fnx::create_type_name<fnx::mutually_exclusive_container>())
			: fnx::widget(name) {}
		~mutually_exclusive_container() = default;

		void set_visible_child(size_t index)
		{
			if (index < _children.size())
			{
				_current_visible_child = index;
			}
		}

		virtual void render(camera_handle camera, matrix4x4 parent_matrix) override
		{
			if (_current_visible_child < _children.size())
			{
				_children[_current_visible_child]->render(camera, matrix_translate(matrix4x4::identity(), get_x(), get_y(), 0.f) * parent_matrix);
			}
		};
	protected:
		size_t _current_visible_child{ 0 };
	private:
	};
}