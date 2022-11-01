#pragma once

namespace fnx
{
	class tabbed_page : public fnx::widget
	{
	public:
		tabbed_page(const std::string& name = fnx::create_type_name<fnx::tabbed_page>())
			: fnx::widget(name) {}
		~tabbed_page() = default;
	};

	/// @brief Size the sizer according to the children it was provided
	class tabbed_notebook : public fnx::widget
	{
	public:
		enum class tab_location
		{
			top,
			left
		};
		tabbed_notebook(tab_location loc, const std::string& name = fnx::create_type_name<fnx::tabbed_notebook>())
			: fnx::widget(name)
		{
			if (tab_location::top == loc)
			{
				// row 0 = tabs
				// row 1 = pages
				auto notebook_sizer = fnx::create_widget<fnx::vert_fit_sizer>(2);	/// Aligns the tabs and pages vertically
				auto radio = fnx::create_widget<fnx::radio>();						/// Ensures only one page is selected at a time
				auto radio_sizer = fnx::create_widget<fnx::horz_sizer>(0);			/// Aligns the tabs horizontally
				auto mec = fnx::create_widget<fnx::mutually_exclusive_container>();
				fnx::widget::add_widget(notebook_sizer);	// _children[0]
				notebook_sizer->add_widget(radio);	// _children[0][0]
				notebook_sizer->add_widget(mec);
				radio->add_widget(radio_sizer);	// _children[0][0][0]

				radio_sizer->set_constraints(fnx::fill_parent);
				notebook_sizer->set_constraints(fnx::fill_parent);
				mec->set_constraints(fnx::fill_parent);

				_tab_container = radio_sizer;
				_mec_container = mec;
			}
			else
			{
				auto w = fnx::create_widget<fnx::vert_sizer>(0);
				fnx::widget::add_widget(w);
			}
		}
		~tabbed_notebook() = default;

		virtual void add_page(fnx::widget_handle_t<fnx::tabbed_page> page)
		{
			if (page && _tab)
			{
				// add a tab for this new child using the copy constructor
				auto w = fnx::create_widget<fnx::block>(*_tab.get());
				_tab_container->add_widget(w);
				// add this page as a mutually exclusive child to the notebook
				_mec_container->add_widget(page);
			}
		}

		virtual void add_widget(fnx::widget_handle wid) override {}

		void set_tab_block(fnx::widget_handle_t<fnx::block> block)
		{
			if (block)
			{
				_tab = block;
			}
		}

		void set_visible_child(size_t index)
		{
			_mec_container->set_visible_child(index);	// will bound check index
			if (index < _tab_container->num_children())
			{
				_current_visible_child = index + 1;
				// TODO: set the tab to checked state for the particular notebook page we are on
			}
		}

		// TODO: text with the tab name
		// TODO: on parent change
		// TODO: click on other tabs
		// TODO: add a tab when adding a widget
		// TODO : render tab as selected

	protected:
		size_t _current_visible_child{ 0 };
		fnx::widget_handle_t<fnx::block> _tab;
		fnx::widget_handle_t<fnx::sizer> _tab_container;
		fnx::widget_handle_t<fnx::mutually_exclusive_container> _mec_container;
	private:
	};
}