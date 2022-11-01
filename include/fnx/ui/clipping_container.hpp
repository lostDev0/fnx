#pragma once

namespace fnx
{
	class clipping_container : public fnx::widget
	{
	public:
		clipping_container(const std::string& name = create_type_name<clipping_container>())
			: widget(name)
		{}
		virtual ~clipping_container() {}

		virtual void render(camera_handle camera, matrix4x4 parent_matrix) override
		{
			auto [win,_1] = singleton<window>::acquire();
			auto origin = fnx::vector3{ get_x(), get_y(), 0.f };
			parent_matrix = fnx::matrix_translate(parent_matrix, origin);
			// origin now has the global position in opengl coordinates but the clipping region is in screen pixels
			auto origin_screen = win.opengl_to_cartesian(origin.x, origin.y);
			auto [renderer,_2] = singleton<renderer>::acquire();

			renderer.set_clipping(static_cast<int>(origin_screen.x), 
				static_cast<int>(origin_screen.y), 
				static_cast<int>(get_width() * (win.width() / 2.f)), 
				static_cast<int>(get_height() * (win.height() / 2.f)));
			widget::render(camera, parent_matrix);
			renderer.reset_clipping();
		}
	};
}