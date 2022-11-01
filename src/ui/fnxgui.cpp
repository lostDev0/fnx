namespace fnx
{
    namespace
    {
        fnx::label_handle fps_label_widget = nullptr;
        bool update_debug_ui(const render_evt& msg)
        {
            if (fps_label_widget)
            {
                std::stringstream str;
                str << "fps (now): " << static_cast<int>(msg._fps_now);
                str << "\n\t(avg): " << static_cast<int>(msg._fps_avg);
                str << "\n\t(min): " << static_cast<int>(msg._fps_min);
                str << "\n\t(max): " << static_cast<int>(msg._fps_max);
                str << "\n";	//@todo ... label gets cut off if there isn't another character here
                fps_label_widget->set_text(str.str());
            }
            return false;
        }
    }

    void enable_debug_ui_output()
    {
        auto label = fnx::create_widget<fnx::label>("arial_sd", "", "fps_label");
        label->set_text_size_in_points(8);
        label->set_constraints(fnx::constraints(fnx::relative_vert_constraint(.9f), fnx::relative_horz_constraint(.008f)));
        auto layer = fnx::make_shared_ref<fnx::widget_layer>("debug_layer");
        layer->show();
        layer->add_widget(label);
        {
            auto [stack, _1] = singleton<layer_stack>::acquire();
            if (!stack.insert_layer(0, layer))
            {
                FNX_ERROR("unable to create widget layer");
            }
            else
            {
                fps_label_widget = label;

                std::stringstream str;
                str << "FPS (now): " << 0;
                str << "\nFPS (avg): " << 0;
                str << "\nFPS (min): " << 0;
                str << "\nFPS (max): " << 0;
                fps_label_widget->set_text(str.str());
                auto [events, _] = singleton<event_manager>::acquire();
                events.subscribe<fnx::render_evt>(fnx::bind(&update_debug_ui));
            }
        }
    }

    void disable_debug_ui_output()
    {
        auto [events, _] = singleton<event_manager>::acquire();
        auto [stack, _1] = singleton<layer_stack>::acquire();
        events.unsubscribe<fnx::render_evt>(fnx::bind(&update_debug_ui));
        // find the layer we added and remove it
        stack.remove_layer("debug_layer");
    }
}