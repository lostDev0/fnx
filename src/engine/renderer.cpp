#include <GL/glew.h>
#include <GLFW/glfw3.h>
using namespace reactphysics3d;

bool fnx::renderer::_is_wireframe{ false };

namespace fnx
{
    renderer::renderer()
    {
        // TODO render game objects
        //auto [events,_] = singleton<event_manager>::acquire();
        //events.subscribe<render_evt>(fnx::bind(*this, &renderer::render_game_objects));
    }

    renderer::~renderer()
    {
        // TODO unsubscribe from game object rendering
        //auto [events,_] = singleton<event_manager>::acquire();
        //events.unsubscribe<render_evt>(fnx::bind(*this, &renderer::render_game_objects));
        //disable_shadows();
    }

    void renderer::enable_shadows()
    {
        // TODO
        //auto [events,_] = singleton<event_manager>::acquire();
        //events.subscribe<render_shadows_evt>(fnx::bind(*this, &renderer::render_game_object_shadows));
        //_enable_shadows = true;
    }
    
    void renderer::disable_shadows()
    {
        /*
        // TODO
        if (_enable_shadows)
        {
            auto [events,_] = singleton<event_manager>::acquire();
            events.unsubscribe<render_shadows_evt>(fnx::bind(*this, &renderer::render_game_object_shadows));
        }
        _enable_shadows = false;
        */
    }

    /*
    // TODO add renderables if needed
    void renderer::draw(camera_handle camera, const fnx::renderable& renderable, const fnx::transform& transform)
    {
        renderable._shader->bind();
        apply_camera(renderable._shader, camera);
        apply_transformation(renderable._shader, transform);
        apply_material(renderable._shader, renderable._material);

        renderable._model->bind();
        renderable._model->render();
        renderable._model->unbind();
    }
    */

    void renderer::draw_current(const fnx::matrix4x4& transform)
    {
        if (_model && _shader && _camera && _material)
        {
            _shader->set_uniform(UNIFORM_MODEL_VIEW_MATRIX, transform);
            apply_material(_material);
            _model->render();
        }
    }

    void renderer::draw_current()
    {
        if (_model) { _model->render(); }
    }

    void renderer::apply_transformation(const matrix4x4& transform) const
    {
        apply_transformation(_shader, transform);
    }

    void renderer::apply_transformation(shader_handle shader, const fnx::matrix4x4& transform)
    {
        shader->set_uniform(UNIFORM_MODEL_VIEW_MATRIX, transform);
    }

    void renderer::apply_shader(shader_handle shader)
    {
        shader->bind();
        _shader = shader;
        _current_texture_index = 0;
    }

    void renderer::apply_model(model_handle model)
    {
        model->bind();
        _model = model;
    }

    void renderer::apply_camera(camera_handle camera)
    {
        _camera = camera;
        apply_camera(_shader, _camera);
    }

    void renderer::apply_camera(shader_handle shader, camera_handle camera)
    {
        shader->set_uniform(UNIFORM_CAMERA_PROJECTION_MATRIX, camera->get_projection_matrix());
        shader->set_uniform(UNIFORM_CAMERA_VIEW_MATRIX, camera->get_view_matrix());
        shader->set_uniform(UNIFORM_CAMERA_POSITION, camera->get_position());
    }

    void renderer::apply_fog(const vector4& color, float density, float gradient) const
    {
        apply_fog(_shader, color, density, gradient);
    }

    void renderer::apply_fog(shader_handle shader, const vector4& color, float density, float gradient)
    {
        shader->set_uniform(UNIFORM_FOG_COLOR, color);
        shader->set_uniform(UNIFORM_FOG_DENSITY, density);
        shader->set_uniform(UNIFORM_FOG_GRADIENT, gradient);
    }

    void renderer::apply_lights(const vector3& gamma, const std::vector<material>& lights) const
    {
        apply_lights(_shader, gamma, lights);
    }

    void renderer::apply_lights(shader_handle shader, const Vector3& gamma, const std::vector<material>& lights)
    {
        shader->set_uniform(UNIFORM_GAMMA, gamma);
        for (const auto& light : lights)
        {
            apply_material(shader, light);
        }
    }

    void renderer::apply_material(material_handle mat)
    {
        apply_material(_shader, *mat);
    }

    void renderer::apply_material(const fnx::material& mat)
    {
        apply_material(_shader, mat);
    }

    void renderer::apply_material(shader_handle shader, material_handle mat)
    {
        if (mat)
        {
            apply_material(shader, *mat);
        }
    }
    void renderer::apply_material(shader_handle shader, const fnx::material& mat)
    {
        // apply all uniforms
        auto cnt = 0;
        for (const auto& t : mat.get_textures())
        {
            t.second->bind(cnt);
            shader->set_uniform(t.first.c_str(), cnt++);
        }
        for (const auto& f : mat.get_floats())
        {
            shader->set_uniform(f.first.c_str(), f.second);
        }
        for (const auto& i : mat.get_ints())
        {
            shader->set_uniform(i.first.c_str(), i.second);
        }
        for (const auto& m : mat.get_matrix4x4s())
        {
            shader->set_uniform(m.first.c_str(), m.second);
        }
        for (const auto& v : mat.get_vector2s())
        {
            shader->set_uniform(v.first.c_str(), v.second);
        }
        for (const auto& v : mat.get_vector3s())
        {
            shader->set_uniform(v.first.c_str(), v.second);
        }
        for (const auto& v : mat.get_vector4s())
        {
            shader->set_uniform(v.first.c_str(), v.second);
        }
        for (const auto& pair : mat.get_array_vector4s())
        {
            auto i = 0u;
            for (const auto& arr : pair.second)
            {
                shader->set_uniform(UNIFORM_GRADIENT, i, arr);
                ++i;
            }
        }
    }

    void renderer::clear(float r, float g, float b)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(r, g, b, 1.0f);
    }

    void renderer::clear(const vector3& rgb)
    {
        renderer::clear(rgb.x, rgb.y, rgb.z);
    }

    void renderer::enable_hardware_depth_test()
    {
        // Enable depth test
        glEnable(GL_DEPTH_TEST);
        // Accept fragment if it closer to the camera than the former one
        glDepthFunc(GL_LESS);
    }

    void renderer::set_provoking_vertex_first()
    {
        glProvokingVertex(GL_FIRST_VERTEX_CONVENTION);
    }

    void renderer::render_to_screen()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void renderer::set_wireframe_mode()
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        _is_wireframe = true;
    }

    void renderer::set_fill_mode()
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        _is_wireframe = false;
    }

    void renderer::set_line_width(float thickness)
    {
        glLineWidth(thickness);
    }

    bool renderer::is_wireframe()
    {
        return _is_wireframe;
    }

    void renderer::enable_blend()
    {
        glEnable(GL_BLEND);
    }

    void renderer::blend_src_alpha_one_minus_src_alpha()
    {
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    void renderer::blend_src_alpha_one_minus_dst_alpha()
    {
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_DST_ALPHA);
    }

    void renderer::blend_src_alpha_one_minus_src_color()
    {
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_COLOR);
    }

    void renderer::blend_src_alpha_one_minus_dst_color()
    {
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_DST_COLOR);
    }

    void renderer::blend_dst_alpha_one_minus_src_alpha()
    {
        glBlendFunc(GL_DST_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    void renderer::blend_dst_alpha_one_minus_dst_alpha()
    {
        glBlendFunc(GL_DST_ALPHA, GL_ONE_MINUS_DST_ALPHA);
    }

    void renderer::blend_dst_alpha_one_minus_src_color()
    {
        glBlendFunc(GL_DST_ALPHA, GL_ONE_MINUS_SRC_COLOR);
    }

    void renderer::blend_dst_alpha_one_minus_dst_color()
    {
        glBlendFunc(GL_DST_ALPHA, GL_ONE_MINUS_DST_COLOR);
    }

    void renderer::blend_one_one()
    {
        glBlendFunc(GL_ONE, GL_ONE);
    }

    void renderer::blend_zero_zero()
    {
        glBlendFunc(GL_ZERO, GL_ZERO);
    }

    void renderer::blend(fnx::blend a, fnx::blend b)
    {
        glBlendFunc(static_cast<int>(a), static_cast<int>(b));
    }

    void renderer::disable_depth_test()
    {
        glDisable(GL_DEPTH_TEST);
    }

    void renderer::enable_depth_test()
    {
        glEnable(GL_DEPTH_TEST);
    }

    void renderer::disable_blend()
    {
        glDisable(GL_BLEND);
    }

    void renderer::enable_face_cull()
    {
        glEnable(GL_CULL_FACE); // don't render back face
    }

    void renderer::disable_face_cull()
    {
        glDisable(GL_CULL_FACE);
    }

    void renderer::clear_depth_bit()
    {
        glClear(GL_DEPTH_BUFFER_BIT);
    }

    void renderer::set_clipping(int x, int y, int width, int height)
    {
        glEnable(GL_SCISSOR_TEST);
        glScissor(x,y,width,height);
    }

    void renderer::reset_clipping()
    {
        glDisable(GL_SCISSOR_TEST);
    }

    /*
    // TODO draw a 2d rectangle
    void renderer::draw(const fnx::rect2df& rect, const fnx::transform& matrix4x4)
    {
        auto shaders = fnx::locator<fnx::asset_manager<fnx::shader>>::get_shared();
        // retrieve the line shader for drawing aabb boxes
        auto line_shader = shaders->get("line.shader");

        // create a line model for drawing the aabb boxes
        auto line_model = fnx::make_shared_ref<fnx::model>("line", raw_model_line(vector3{ 0,0,0 }, vector3{ 0,0,0 }, fnx::colors::white, fnx::colors::white));

        line_shader->bind();
        line_model->bind();

        fnx::vector3 bottom_left{ rect._left, rect._bottom, 0.f };
        fnx::vector3 top_left{ rect._left, rect._top, 0.f };
        fnx::vector3 bottom_right{ rect._right, rect._bottom, 0.f };
        fnx::vector3 top_right{ rect._right, rect._top, 0.f };

        line_model->update_vbo(fnx::VBO_Data, create_vbo_data_for_line(top_left, top_right, fnx::colors::white, fnx::colors::white));
        fnx::renderer::apply_transformation(line_shader, transform);
        line_model->render();

        line_model->update_vbo(fnx::VBO_Data, create_vbo_data_for_line(bottom_left, bottom_right, fnx::colors::white, fnx::colors::white));
        fnx::renderer::apply_transformation(line_shader, transform);
        line_model->render();

        line_model->update_vbo(fnx::VBO_Data, create_vbo_data_for_line(bottom_left, top_left, fnx::colors::white, fnx::colors::white));
        fnx::renderer::apply_transformation(line_shader, transform);
        line_model->render();

        line_model->update_vbo(fnx::VBO_Data, create_vbo_data_for_line(bottom_right, top_right, fnx::colors::white, fnx::colors::white));
        fnx::renderer::apply_transformation(line_shader, transform);
        line_model->render();

        line_model->unbind();
    }

    // TODO draw a 3d AABB
    void renderer::draw(const fnx::AABB& aabb, const fnx::transform& transform)
    {
        auto shaders = fnx::locator<fnx::asset_manager<fnx::shader>>::get_shared();
        // retrieve the line shader for drawing aabb boxes
        auto line_shader = shaders->get("line.shader");
        // create a line model for drawing the aabb boxes
        auto line_model = fnx::make_shared_ref<fnx::model>("line", raw_model_line(vector3{ 0,0,0 }, vector3{ 0,0,0 }, fnx::colors::white, fnx::colors::white));
        auto corners = aabb.get_corners();
        if (!corners.empty())
        {
            line_shader->bind();
            line_model->bind();

            line_model->update_vbo(fnx::VBO_Data, create_vbo_data_for_line(corners[0], corners[1], fnx::colors::white, fnx::colors::white));
            fnx::renderer::apply_transformation(line_shader, transform);
            line_model->render();

            line_model->update_vbo(fnx::VBO_Data, create_vbo_data_for_line(corners[1], corners[2], fnx::colors::white, fnx::colors::white));
            fnx::renderer::apply_transformation(line_shader, transform);
            line_model->render();

            line_model->update_vbo(fnx::VBO_Data, create_vbo_data_for_line(corners[2], corners[3], fnx::colors::white, fnx::colors::white));
            fnx::renderer::apply_transformation(line_shader, transform);
            line_model->render();

            line_model->update_vbo(fnx::VBO_Data, create_vbo_data_for_line(corners[3], corners[0], fnx::colors::white, fnx::colors::white));
            fnx::renderer::apply_transformation(line_shader, transform);
            line_model->render();

            line_model->update_vbo(fnx::VBO_Data, create_vbo_data_for_line(corners[4], corners[5], fnx::colors::white, fnx::colors::white));
            fnx::renderer::apply_transformation(line_shader, transform);
            line_model->render();

            line_model->update_vbo(fnx::VBO_Data, create_vbo_data_for_line(corners[5], corners[6], fnx::colors::white, fnx::colors::white));
            fnx::renderer::apply_transformation(line_shader, transform);
            line_model->render();

            line_model->update_vbo(fnx::VBO_Data, create_vbo_data_for_line(corners[6], corners[7], fnx::colors::white, fnx::colors::white));
            fnx::renderer::apply_transformation(line_shader, transform);
            line_model->render();

            line_model->update_vbo(fnx::VBO_Data, create_vbo_data_for_line(corners[7], corners[4], fnx::colors::white, fnx::colors::white));
            fnx::renderer::apply_transformation(line_shader, transform);
            line_model->render();

            line_model->update_vbo(fnx::VBO_Data, create_vbo_data_for_line(corners[0], corners[6], fnx::colors::white, fnx::colors::white));
            fnx::renderer::apply_transformation(line_shader, transform);
            line_model->render();

            line_model->update_vbo(fnx::VBO_Data, create_vbo_data_for_line(corners[3], corners[5], fnx::colors::white, fnx::colors::white));
            fnx::renderer::apply_transformation(line_shader, transform);
            line_model->render();

            line_model->update_vbo(fnx::VBO_Data, create_vbo_data_for_line(corners[1], corners[7], fnx::colors::white, fnx::colors::white));
            fnx::renderer::apply_transformation(line_shader, transform);
            line_model->render();

            line_model->update_vbo(fnx::VBO_Data, create_vbo_data_for_line(corners[2], corners[4], fnx::colors::white, fnx::colors::white));
            fnx::renderer::apply_transformation(line_shader, transform);
            line_model->render();

            line_model->unbind();
        }
    }
    */

    /*
    bool renderer::render_game_objects(const fnx::render_evt& evt)
    {
        auto entities = locate<entity_manager>();
        auto shaders = locate<fnx::asset_manager<fnx::shader>>();
        auto materials = locate<fnx::asset_manager<fnx::material>>();
        const auto& directional_light = locate<light_system>()->get_mutable_directional_light();

        // @todo can this frustrum be calculated to be just bigger than the screen area?
        // @todo duplicate code here and in generate shadows
        float near_plane = .1f, far_plane = 50.f;
        matrix4x4 light_projection = matrix_ortho(-100.0f, 100.0f, -100.0f, 100.0f, near_plane, far_plane);
        // reverse the light direction to get the "position"
        matrix4x4 light_view_matrix = matrix_look_at(directional_light.direction * -1.0f, vector3(0.f, 0.f, 0.f), vector3(0.0f, 1.0f, 0.0f));
        matrix4x4 light_space_matrix = light_view_matrix * light_projection;	// WHY is this multiplication backwards???

        // retrieve the line shader for drawing aabb boxes
        //auto line_shader = shaders->get("line.shader");
        // create a line model for drawing the aabb boxes
        //auto line_model = fnx::make_shared_ref<fnx::model>("line", raw_model_line(vector3{ 0,0,0 }, vector3{ 0,0,0 }, fnx::colors::white, fnx::colors::white));

        glEnable(GL_LINE_SMOOTH);
        glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
        set_line_width(1.0f);	// reset width back to normal
        enable_blend();			// widget layers may disable this so re-enable
        blend_src_alpha_one_minus_src_alpha();

        auto fog_color = locate<property_manager>()->get_property<vec4f>(PROPERTY_FOG_COLOR);
        auto fog_density = locate<property_manager>()->get_property<float>(PROPERTY_FOG_DENSITY);
        auto fog_gradient = locate<property_manager>()->get_property<float>(PROPERTY_FOG_GRADIENT);
        auto gamma = locate<property_manager>()->get_property<float>(PROPERTY_GAMMA);
        const auto& lights = locate<light_system>()->get_active_lights();

        entities->each<fnx::transform, fnx::camera_component>([&](fnx::entity::id entity, fnx::transform& transform, fnx::camera_component& cam) {
            if (cam._camera != nullptr)
            {
                entities->each_with_component_subset<fnx::transform, fnx::renderable>([&](fnx::entity::id entity, fnx::transform& transform, fnx::renderable& renderable) {
                    if (renderable._shader && renderable._model)
                    {
                        renderable._shader->bind();
                        ////////////////////////// Start Shadow
                        renderable._shader->set_uniform(UNIFORM_LIGHT_SPACE_MATRIX, light_space_matrix);
                        renderable._shader->set_uniform(UNIFORM_SHADOW_MAP, 0);	// @TODO this can't stay ... need to check the shader or material for this
                        glActiveTexture(GL_TEXTURE0);
                        glBindTexture(GL_TEXTURE_2D, _depth_map_texture);
                        ////////////////////////// End Shadow
                        apply_camera(renderable._shader, cam._camera);
                        apply_transformation(renderable._shader, transform);
                        apply_fog(renderable._shader, fog_color, fog_density, fog_gradient);
                        apply_lights(renderable._shader, gamma, lights);
                        apply_material(renderable._shader, renderable._material);

                        auto raw = renderable._model->get_raw_model();

                        renderable._model->bind();
                        if (raw)
                        {
                            // render each material map
                            if (raw->has_material_map())
                            {
                                // loop through all materials
                                for (const auto& mat : raw->get_material_map().get_all())
                                {
                                    auto material = materials->get(mat._material_name);
                                    apply_material(renderable._shader, material);
                                    renderable._model->render_partial(mat._start, mat._end);
                                }
                            }
                            else
                            {
                                renderable._model->render();
                            }
                        }
                        else
                        {
                            renderable._model->render();
                        }
                        renderable._model->unbind();

                        /// @todo only do this in a debug build and state
                        if (raw)
                        {
                            fnx::renderer::draw(raw->get_aabb(), transform);
                        }
                    }
                    });
            }
            });

        return false;
    }

    // renderQuad() renders a 1x1 XY quad in NDC for debug
    // -----------------------------------------
    
    unsigned int quadVAO = 0;
    unsigned int quadVBO;
    void renderQuad()
    {
        if (quadVAO == 0)
        {
            float quadVertices[] = {
                // positions        // texture Coords
                -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
                -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
                 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
                 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
            };
            // setup plane VAO
            glGenVertexArrays(1, &quadVAO);
            glGenBuffers(1, &quadVBO);
            glBindVertexArray(quadVAO);
            glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        }
        glBindVertexArray(quadVAO);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glBindVertexArray(0);
    }
    
    bool renderer::render_game_object_shadows(const fnx::render_shadows_evt& evt)
    {
        auto entities = locate<entity_manager>();
        auto shader = locate<asset_manager<fnx::shader>>()->get(fnx::SHADER_SHADOWS);
        auto near_plane = locate<property_manager>()->get_property<float>(fnx::PROPERTY_NEAR_PLANE);
        auto far_plane = locate<property_manager>()->get_property<float>(fnx::PROPERTY_FAR_PLANE);
        if (!shader)
            return false;
        shader->bind();
        const auto& directional_light = fnx::locator<fnx::light_system>::get_shared()->get_mutable_directional_light();
        // @todo can this frustrum be calculated to be just bigger than the screen area?
        matrix4x4 light_projection = matrix_ortho(-100.0f, 100.0f, -100.0f, 100.0f, near_plane, far_plane);
        // reverse the light direction to get the "position"
        matrix4x4 light_view_matrix = matrix_look_at(directional_light.direction * -1.0f, vector3(0.f, 0.f, 0.f), vector3(0.0f, 1.0f, 0.0f));
        matrix4x4 light_space_matrix = light_view_matrix * light_projection;	// WHY is this multiplication backwards???

        glViewport(0, 0, 2048, 2048);

        glBindFramebuffer(GL_FRAMEBUFFER, _depth_map_fbo);
        glEnable(GL_DEPTH_TEST);
        glClear(GL_DEPTH_BUFFER_BIT);

        /// @todo this will need to be specific for each light source when rendering multiple
        shader->set_uniform(UNIFORM_LIGHT_SPACE_MATRIX, light_space_matrix);

        entities->each_with_component_subset<fnx::transform, fnx::renderable>([&](fnx::entity::id entity, fnx::transform& transform, fnx::renderable& renderable) {
            /// @todo draw from all light sources
            //for (const auto& light : lights)
            {
                if (renderable._model)
                {
                    apply_transformation(shader, transform);
                    renderable._model->bind();
                    renderable._model->render();
                    renderable._model->unbind();
                }
            }
            });
        glBindFramebuffer(GL_FRAMEBUFFER, 0);	// unbind
        glViewport(0, 0, 1920, 1080);	// @todo window size and move into the main render loop
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        return false;
    }
    */

    void renderer::init_depth_map()
    {
        // create a frame buffer to hold the scene's depth map
        glGenFramebuffers(1, &_depth_map_fbo);
        const unsigned int width = 2048, height = 2048;	// @todo : replace with whatever the window size is

        // create a texture to render the depth map to
        // @todo : use the texture helpers
        glGenTextures(1, &_depth_map_texture);
        glBindTexture(GL_TEXTURE_2D, _depth_map_texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        // Prevents darkness outside the frustrum
        float clampColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, clampColor);

        // attach the depth texture to the depth frame buffer
        glBindFramebuffer(GL_FRAMEBUFFER, _depth_map_fbo);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _depth_map_texture, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);

        // Error checking framebuffer
        auto status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if (status != GL_FRAMEBUFFER_COMPLETE)
            FNX_ERROR(fnx::format_string("Depth Map Framebuffer error: %d", status));

        glBindFramebuffer(GL_FRAMEBUFFER, 0);	// unbind
    }

    void renderer::init_post_processing()
    {
        //https://github.com/VictorGordan/opengl-tutorials/blob/main/YoutubeOpenGL%2025%20-%20Shadow%20Maps%20(Directional%20Lights)/Main.cpp
        glGenFramebuffers(1, &_post_processing_fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, _post_processing_fbo);

        // @todo window size
        int width = 1920, height = 1080;
        glGenTextures(1, &_post_processing_texture);
        glBindTexture(GL_TEXTURE_2D, _post_processing_texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _post_processing_texture, 0);

        // Error checking framebuffer
        auto status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if (status != GL_FRAMEBUFFER_COMPLETE)
            FNX_ERROR(fnx::format_string("Post-Processing Framebuffer error: %d", status));

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
}
