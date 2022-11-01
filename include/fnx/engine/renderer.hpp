#pragma once

namespace fnx
{
	enum class blend
	{
		ZERO = 0x0,
		ONE = 0x1,
		SRC = 0x0300,
		ONE_MINUS_SRC = 0x0301,
		SRC_ALPHA,
		ONE_MINUS_SRC_ALPHA,
		DST_ALPHA,
		ONE_MINUS_DST_ALPHA,
		DST,
		ONE_MINUS_DST
	};

	class renderer
	{
	public:

		/// @brief Render the current model with the provided transformation.
		void draw_current(const fnx::matrix4x4& transform);
		/// @brief Render the current model assuming the shader and material have been applied.
		void draw_current();
		void apply_shader(shader_handle shader);
		void apply_model(model_handle model);
		void apply_camera(camera_handle camera);
		void apply_material(material_handle mat);
		void apply_material(const fnx::material& mat);
		void apply_transformation(const matrix4x4& transform) const;
		void apply_fog(const vector4& color, float density, float gradient) const;
		void apply_lights(const vector3& gamma, const std::vector<material>& lights) const;

		//static void draw(camera_handle camera, const fnx::renderable& renderable, const fnx::matrix4x4& transform);
		static void apply_transformation(shader_handle shader, const matrix4x4& transform);
		static void apply_camera(shader_handle shader, camera_handle camera);
		static void apply_fog(shader_handle shader, const vector4& color, float density, float gradient);
		static void apply_lights(shader_handle shader, const vector3& gamma, const std::vector<material>& lights);
		static void apply_material(shader_handle shader, material_handle mat);
		static void apply_material(shader_handle shader, const fnx::material& mat);

		static void clear(float red, float green, float blue);
		static void clear(const fnx::colors::rgb& rgb);

		static void enable_hardware_depth_test();
		static void set_provoking_vertex_first();
		static void render_to_screen();
		static void set_wireframe_mode();
		static void set_fill_mode();
		static void set_line_width(float thickness);
		static bool is_wireframe();
		static void enable_blend();
		static void disable_blend();
		static void blend_src_alpha_one_minus_src_alpha();
		static void blend_src_alpha_one_minus_dst_alpha();
		static void blend_src_alpha_one_minus_src_color();
		static void blend_src_alpha_one_minus_dst_color();
		static void blend_dst_alpha_one_minus_src_alpha();
		static void blend_dst_alpha_one_minus_dst_alpha();
		static void blend_dst_alpha_one_minus_src_color();
		static void blend_dst_alpha_one_minus_dst_color();
		static void blend_one_one();
		static void blend_zero_zero();
		static void blend(fnx::blend a, fnx::blend b);
		static void disable_depth_test();
		static void enable_depth_test();
		static void enable_face_cull();
		static void disable_face_cull();
		static void clear_depth_bit();
		static void set_clipping(int x, int y, int width, int height);
		static void reset_clipping();
		//static void draw(const fnx::rect2df& rect, const fnx::transform& transform);
		//static void draw(const reactphysics3d::AABB& aabb, const fnx::matrix4x4& transform);

		renderer();
		~renderer();

		void enable_shadows();
		void disable_shadows();
		bool is_shadow_enabled() { return _enable_shadows; }

		/*
		void render_world_objects(fnx::camera_handle camera, const fnx::vector4& fog_color, float fog_density,
			float fog_gradient, const fnx::vector3& gamma, const std::vector<material>& lights);

		/// @param[in] near_plane : how close to the light to start shading things
		/// @param[in] far_plane : how far from the light to stop shading things
		/// @param[in] shader : shader used for generating the shadow(depth) map
		void generate_shadows(const std::vector<material>& lights, float near_plane = 1.f, float far_plane = 15.f,
			shader_handle shader = nullptr);
			*/

	private:
		static bool _is_wireframe;
		bool _enable_shadows{ false };

		shader_handle _shader{};
		model_handle _model{};
		material_handle _material{};
		camera_handle _camera{};
		//frustrum _frustrum{};
		int32_t _current_texture_index{ 0 };

		uint32_t _depth_map_fbo{ 0u };
		uint32_t _depth_map_texture{ 0u };

		uint32_t _post_processing_fbo{ 0u };
		uint32_t _post_processing_texture{ 0u };

		/// @brief Creates the depth map FBO for doing depth testing and shadows
		void init_depth_map();
		/// @brief Creates the post processing FBO for doing effects upon the rendered scene
		void init_post_processing();

		//bool render_game_objects(const fnx::render_evt& evt);
		//bool render_game_object_shadows(const fnx::render_shadows_evt& evt);
	};
}