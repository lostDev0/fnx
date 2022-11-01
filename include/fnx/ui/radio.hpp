#pragma once

namespace fnx
{
	class radio : public fnx::widget
	{
	public:
		radio(const std::string& name = fnx::create_type_name<fnx::radio>()) : fnx::widget(name) {}
		~radio() {}

		virtual void notify_mouse_release_absorbed(widget_handle child)
		{
			uncheck_all(get_children());
		}

	protected:

		bool uncheck_all(std::vector<fnx::widget_handle> children)
		{
			// if _checked and not in the hover state, uncheck
			bool result = false;
			auto it_end = children.rend();
			for (auto it = children.rbegin(); it != it_end; ++it)
			{
				auto w = *it;
				if (w->is_checked() && !w->is_cursor_over())
				{
					w->uncheck();
					return true;
				}
				
				if (w->has_children())
				{
					if (uncheck_all(w->get_children()))
					{
						return true;
					}
				}
			}

			return false;
		}
	};
}
