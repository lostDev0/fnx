#pragma once
#include <string>
#include <vector>

namespace fnx
{
	enum VBO_Index
	{
		VBO_Invalid = -1,
		VBO_Data = 0,
		VBO_Max_Assigned
	};

	using Attribute_Index = int;
	constexpr auto Attribute_Index_Position = 0;
	constexpr auto Attribute_Index_Texture = 1;
	constexpr auto Attribute_Index_Normal = 2;
	constexpr auto Attribute_Index_Color = 3;


	class model_impl;
	class raw_model;
	class model : public fnx::asset
	{
	public:
		/// @brief Create a model with no vertex data.
		model(const std::string& name, bool has_texture = false, bool has_normals = false, bool has_colors = false);

		/// @brief Create a model from an existing raw model.
		model(const std::string& name, fnx::raw_model& raw_model);

		/// @brief Create a model from an existing raw model.
		model(const std::string& name, fnx::raw_model_handle raw_model);

		virtual ~model();

		void render_as_lines() { _render_as_lines = true; }

		/// @brief Start using the model for rendering.
		virtual void bind() const;

		/// @brief Draw the model.
		virtual void render() const;

		/// @brief Draw part of the model.
		virtual void render_partial(int start, int end) const;

		/// @brief Stop using the model for rendering.
		virtual void unbind() const;

		/// @brief Start using the model for initialization.
		/// @note Call this before setting up VBOs and IBOs.
		void bind_to_vao();

		/// @brief Stop using the model for initialization.
		/// @note Call this when doing setting up VBOs and IBOs.
		void unbind_vao();

		/// @brief Returns the VAO id that was given during construction.
		uint32_t get_vao() const;

		/// @brief Set the current vertex buffer to be used for subsequent calls.
		void bind_to_vbo(VBO_Index vbo_idx);

		/// @brief Set the current vertex buffer's data.
		void set_vbo_data(const std::vector<float>& elements);

		/// @brief Enable an attribute to be used in shaders.
		void enable_attrib(Attribute_Index attr_idx);

		/// @brief Enable an attribute to be used in shaders.
		void enable_attrib_array(Attribute_Index attr_idx);

		/// @brief Tell the shader how to interpret the VBO_Data.
		void setup_attrib_array(Attribute_Index attr_idx, int num_values, int bytes_offset);

		void unbind_vbo();

		/// @brief Initialize a VBO buffer.
		/// @note Initial call to populate a VBO buffer. This will tell opengl how to interpret the data.
		///     Updates to the VBO's buffer content should be done by calling update_vbo().
		void load_to_vbo(VBO_Index vbo_idx, unsigned int num_components, const std::vector<float>& elements);
		/// @brief Initialize a VBO buffer.
		/// @note Initial call to populate a VBO buffer. This will tell opengl how to interpret the data.
		///     Updates to the VBO's buffer content should be done by calling update_vbo().
		void load_to_vbo(VBO_Index vbo_idx, unsigned int num_components, const std::vector<unsigned int>& elements);
		/// @brief Initialize a VBO buffer.
		/// @note Initial call to populate a VBO buffer. This will tell opengl how to interpret the data.
		///     Updates to the VBO's buffer content should be done by calling update_vbo().
		void load_to_vbo(VBO_Index vbo_idx, unsigned int num_components, const std::vector<int>& elements);
		/// @brief Initialize a VBO buffer.
		/// @note Initial call to populate a VBO buffer. This will tell opengl how to interpret the data.
		///     Updates to the VBO's buffer content should be done by calling update_vbo().
		void load_to_vbo(VBO_Index vbo_idx, unsigned int num_components, const std::vector<unsigned short>& elements);
		/// @brief Initialize a VBO buffer.
		/// @note Initial call to populate a VBO buffer. This will tell opengl how to interpret the data.
		///     Updates to the VBO's buffer content should be done by calling update_vbo().
		void load_to_vbo(VBO_Index vbo_idx, unsigned int num_components, const std::vector<short>& elements);
		/// @brief Initialize a VBO buffer.
		/// @note Initial call to populate a VBO buffer. This will tell opengl how to interpret the data.
		///     Updates to the VBO's buffer content should be done by calling update_vbo().
		void load_to_vbo(VBO_Index vbo_idx, unsigned int num_components, const std::vector<char>& elements);
		/// @brief Initialize a VBO buffer.
		/// @note Initial call to populate a VBO buffer. This will tell opengl how to interpret the data.
		///     Updates to the VBO's buffer content should be done by calling update_vbo().
		void load_to_vbo(VBO_Index vbo_idx, unsigned int num_components, const std::vector<unsigned char>& elements);

		/// @brief Loads an attribute to an already initialized VBO.
		void load_to_attribute(VBO_Index vbo_idx, Attribute_Index attr_idx, unsigned int num_components, unsigned int stride_in_num_components, unsigned int offset_in_num_components, const std::vector<float>& elements);

		/// @brief Loads an attribute to an already initialized and active VBO.
		void load_to_attribute(Attribute_Index attr_idx, unsigned int num_components, unsigned int stride_in_num_components, unsigned int offset_in_num_components);

		void load_to_ibo(const std::vector<unsigned char>& indices);
		void load_to_ibo(const std::vector<unsigned short>& indices);
		void load_to_ibo(const std::vector<unsigned int>& indices);

		/// @brief Update an already initialized VBO float buffer's content.
		void update_vbo(VBO_Index vbo_idx, const std::vector<float>& elements);
		/// @brief Update an already initialized VBO unsigned int buffer's content.
		void update_vbo(VBO_Index vbo_idx, const std::vector<unsigned int>& elements);
		/// @brief Update an already initialized VBO int buffer's content.
		void update_vbo(VBO_Index vbo_idx, const std::vector<int>& elements);
		/// @brief Update an already initialized VBO unsigned short buffer's content.
		void update_vbo(VBO_Index vbo_idx, const std::vector<unsigned short>& elements);
		/// @brief Update an already initialized VBO short buffer's content.
		void update_vbo(VBO_Index vbo_idx, const std::vector<short>& elements);
		/// @brief Update an already initialized VBO char buffer's content.
		void update_vbo(VBO_Index vbo_idx, const std::vector<char>& elements);
		/// @brief Update an already initialized VBO unsigned char buffer's content.
		void update_vbo(VBO_Index vbo_idx, const std::vector<unsigned char>& elements);

		auto has_raw_model() const { return _raw_model != nullptr; }
		const auto& get_raw_model() const { return _raw_model; }

	private:
		model_impl* _impl{ nullptr };
		fnx::raw_model_handle _raw_model{ nullptr };
		bool _render_as_lines{ false };

		model(const model& other) = delete;
		model(model&& other) = delete;

		void init();
	};

	using model_handle = fnx::asset_handle<fnx::model>;
}