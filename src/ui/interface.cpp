#include "json/json.hpp"

namespace fnx
{
    namespace ui
    {
        bool init()
        {
            auto [properties, _] = singleton<property_manager>::acquire();
            properties.set_property(fnx::PROPERTY_UI_SCALE, 1.f);

            //serialize::register_members<fnx::widget>();
            //serialize::register_members<fnx::block>();
            //serialize::register_members<fnx::button>();
            //serialize::register_members<fnx::horz_sizer>();
            //serialize::register_members<fnx::vert_sizer>();
            //serialize::register_members<fnx::horz_fit_sizer>();
            //serialize::register_members<fnx::vert_fit_sizer>();
            //register_deserializer<fnx::block>(serialize::get_name<fnx::block>());
            //register_deserializer<fnx::button>(serialize::get_name<fnx::button>());
            //register_deserializer<fnx::horz_sizer>(serialize::get_name<fnx::horz_sizer>());
            //register_deserializer<fnx::vert_sizer>(serialize::get_name<fnx::vert_sizer>());
            //register_deserializer<fnx::horz_fit_sizer>(serialize::get_name<fnx::horz_fit_sizer>());
            //register_deserializer<fnx::vert_fit_sizer>(serialize::get_name<fnx::vert_fit_sizer>());
            return true;
        }

        fnx::layer_handle parse_file(const std::string& file_path, bool add_to_layer_stack)
        {
            fnx::layer_handle ret;
            /*
            std::ifstream ifs(file_path);
            auto json = nlohmann::json::parse(ifs);
            ret = ui::factory::from_json(json);
            if(add_to_layer_stack)
                auto [stack, _] = singleton<layer_stack>::acquire();
                stack.add_layer(ret);
            */
           return ret;
        }

        fnx::layer_handle parse_string(const std::string& content, bool add_to_layer_stack)
        {

            fnx::layer_handle ret;
            /*
            auto json = nlohmann::json::parse(content);
            ret = ui::factory::from_json(json);
            if (add_to_layer_stack)
                auto [stack, _] = singleton<layer_stack>::acquire();
                stack.add_layer(ret);
            */
            return ret;
        }
    }
}