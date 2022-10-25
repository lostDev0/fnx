#include <GL/glew.h>
#include <GLFW/glfw3.h>

using namespace std;
using namespace fnx;

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

namespace fnx
{
	class model_impl
	{
	public:
		GLuint _vao;
		GLuint _vbos[VBO_Index::VBO_Max_Assigned]{ static_cast<GLuint>(VBO_Index::VBO_Invalid) };
		unsigned int _vbo_num_elements[VBO_Index::VBO_Max_Assigned]{ 0u };
		unsigned int _vbo_num_components[VBO_Index::VBO_Max_Assigned]{ 0u };
		GLuint _ibo;
		unsigned int _num_indices{ 0u };
	};

	void model::init()
	{
		_impl = new model_impl();
		/*** Creation of the VAO and VBO ***/
		glGenVertexArrays(1, &_impl->_vao);
		glBindVertexArray(_impl->_vao);
		glGenBuffers(VBO_Max_Assigned, &_impl->_vbos[VBO_Data]);
		/*** Done ***/
		glBindVertexArray(0); // un-bind
	}

	model::model(const std::string& name, bool has_texture, bool has_normals, bool has_colors)
		: fnx::asset(name)
	{
		init();
		bind_to_vao();
		auto stride = static_cast<int>(sizeof(float)) * 3;	// takes care of the vertex xyz data
		auto offset = 0;
		glBindBuffer(GL_ARRAY_BUFFER, _impl->_vbos[VBO_Data]);

		// enable the attributes
		glEnableVertexAttribArray(Attribute_Index_Position);
		if (has_texture)
		{
			glEnableVertexAttribArray(Attribute_Index_Texture);
			stride += sizeof(float) * 2;	// takes care of the texure uv data
		}
		if (has_normals)
		{
			glEnableVertexAttribArray(Attribute_Index_Normal);
			stride += sizeof(float) * 3;	// takes care of the normal xyz direction data
		}
		if (has_colors)
		{
			glEnableVertexAttribArray(Attribute_Index_Color);
			stride += sizeof(float) * 4;	// takes care of the normal xyz direction data
		}

		// tell opengl how to format the vbo array
		glVertexAttribPointer(Attribute_Index_Position, 3, GL_FLOAT, GL_FALSE, stride, BUFFER_OFFSET(static_cast<int>(sizeof(float))* offset));
		offset += 3;
		if (has_texture)
		{
			glVertexAttribPointer(Attribute_Index_Texture, 2, GL_FLOAT, GL_FALSE, stride, BUFFER_OFFSET(static_cast<int>(sizeof(float))* offset));
			offset += 2;
		}
		if (has_normals)
		{
			glVertexAttribPointer(Attribute_Index_Normal, 3, GL_FLOAT, GL_FALSE, stride, BUFFER_OFFSET(static_cast<int>(sizeof(float))* offset));
			offset += 3;
		}
		if (has_colors)
		{
			glVertexAttribPointer(Attribute_Index_Color, 4, GL_FLOAT, GL_FALSE, stride, BUFFER_OFFSET(static_cast<int>(sizeof(float))* offset));
			offset += 4;
		}

		_impl->_vbo_num_elements[VBO_Data] = 0;
		_impl->_vbo_num_components[VBO_Data] = offset;
		unbind_vao();
	}

	model::model(const std::string& name, fnx::raw_model& raw)
		: fnx::asset(name)
	{
		init();
		bind_to_vao();
		const auto& verts = raw.get_vertices();
		auto stride = static_cast<int>(sizeof(float)) * 3;	// takes care of the vertex xyz data
		auto offset = 0;
		glBindBuffer(GL_ARRAY_BUFFER, _impl->_vbos[VBO_Data]);
		glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(float), verts.data(), GL_STATIC_DRAW);

		//FNX_DEBUG("raw model %s has %d vertices", raw.get_name(), raw.get_num_vertices());

		if (raw.is_line())
		{
			render_as_lines();
		}
		
		// enable the attributes
		glEnableVertexAttribArray(Attribute_Index_Position);
		if (raw.has_texture_data())
		{
			//FNX_DEBUG("raw model %s has texture data", raw.get_name());
			glEnableVertexAttribArray(Attribute_Index_Texture);
			stride += sizeof(float) * 2;	// takes care of the texure uv data
		}
		if (raw.has_normal_data())
		{
			//FNX_DEBUG("raw model %s has normal data", raw.get_name());
			glEnableVertexAttribArray(Attribute_Index_Normal);
			stride += sizeof(float) * 3;	// takes care of the normal xyz direction data
		}
		if (raw.has_color_data())
		{
			//FNX_DEBUG("raw model %s has color data", raw.get_name());
			glEnableVertexAttribArray(Attribute_Index_Color);
			stride += sizeof(float) * 4;	// takes care of the color rgba data
		}

		// tell opengl how to format the vbo array
		glVertexAttribPointer(Attribute_Index_Position, 3, GL_FLOAT, GL_FALSE, stride, BUFFER_OFFSET(static_cast<int>(sizeof(float)) * offset));
		offset += 3;
		if (raw.has_texture_data())
		{
			glVertexAttribPointer(Attribute_Index_Texture, 2, GL_FLOAT, GL_FALSE, stride, BUFFER_OFFSET(static_cast<int>(sizeof(float))* offset));
			offset += 2;
		}
		if (raw.has_normal_data())
		{
			glVertexAttribPointer(Attribute_Index_Normal, 3, GL_FLOAT, GL_FALSE, stride, BUFFER_OFFSET(static_cast<int>(sizeof(float))* offset));
			offset += 3;
		}
		if (raw.has_color_data())
		{
			glVertexAttribPointer(Attribute_Index_Color, 4, GL_FLOAT, GL_FALSE, stride, BUFFER_OFFSET(static_cast<int>(sizeof(float))* offset));
			offset += 4;
		}
		
		_impl->_vbo_num_elements[VBO_Data] = static_cast<unsigned int>(verts.size() *sizeof(float)) / stride;
		_impl->_vbo_num_components[VBO_Data] = offset;

		if (!raw.get_indices().empty())
		{
			FNX_DEBUG(fnx::format_string("raw model %s has index data (%d)", raw.get_name(), raw.get_indices().size()));
			load_to_ibo(raw.get_indices());
		}

		unbind_vao();
	}

	model::model(const std::string& name, fnx::raw_model_handle raw)
		: model(name, *raw)
	{
		_raw_model = raw;
	}

	void model::bind_to_vao()
	{
		glBindVertexArray(_impl->_vao);
	}

	void model::unbind_vao()
	{
		glBindVertexArray(0); // un-bind
	}

	uint32_t model::get_vao() const
	{
		return _impl->_vao;
	}

	void model::bind_to_vbo(VBO_Index vbo_idx)
	{
		glBindBuffer(GL_ARRAY_BUFFER, _impl->_vbos[vbo_idx]);
	}

	void model::set_vbo_data(const std::vector<float>& elements)
	{
		glBufferData(GL_ARRAY_BUFFER, elements.size() * sizeof(float), elements.data(), GL_STATIC_DRAW);
	}

	void model::enable_attrib(Attribute_Index attr_idx)
	{
		glEnableVertexAttribArray(attr_idx);
	}

	void model::enable_attrib_array(Attribute_Index attr_idx)
	{
		glVertexArrayAttribBinding(_impl->_vao, attr_idx, _impl->_vbos[VBO_Data]);
	}

	void model::setup_attrib_array(Attribute_Index attr_idx, int num_values, int bytes_offset)
	{
		glVertexArrayAttribFormat(
			_impl->_vao,
			attr_idx,
			num_values,
			GL_FLOAT,
			GL_FALSE,			/// do not normalise the vectors
			bytes_offset);		// the offset (in bytes) from the start of the buffer
	}

	void model::unbind_vbo()
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	model::~model()
	{
		if (nullptr != _impl)
		{
			glDeleteBuffers(VBO_Max_Assigned, &_impl->_vbos[VBO_Data]);
			glDeleteVertexArrays(1, &_impl->_vao);
			delete _impl;
		}

		_impl = nullptr;
	}

	void model::bind() const
	{
		glBindVertexArray(_impl->_vao);

		if (_impl->_num_indices > 0)
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _impl->_ibo);
		}
		else
		{
			glBindBuffer(GL_ARRAY_BUFFER, _impl->_vbos[VBO_Data]);
		}
	}

	void model::render() const
	{
		// no default map, trust that the shader will take care of it
		if (_render_as_lines)
		{
			glDrawArrays(GL_LINE_STRIP, 0, _impl->_vbo_num_elements[VBO_Data]);
		}
		else if (_impl->_num_indices > 0)
		{
			glDrawElements(GL_TRIANGLES, _impl->_num_indices, GL_UNSIGNED_SHORT, 0);
		}
		else
		{
			glDrawArrays(GL_TRIANGLES, 0, _impl->_vbo_num_elements[VBO_Data]);
		}
	}

	void model::render_partial(int start, int end) const
	{
		if (_impl->_num_indices > 0)
		{
			glDrawElements(GL_TRIANGLES, end - start, GL_UNSIGNED_SHORT, BUFFER_OFFSET(start * sizeof(GLushort)));
		}
		else if (end - start > 0)
		{
			glDrawArrays(GL_TRIANGLES, start, end - start);
		}
		else
		{
			// if end and start are the, render all
			glDrawArrays(GL_TRIANGLES, 0, _impl->_vbo_num_elements[VBO_Data]);
		}
	}

	void model::unbind() const
	{
		glBindVertexArray(0); // un-bind first

		if (_impl->_num_indices > 0)
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);  // un-bind
		}
		else
		{
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
	}

	void model::load_to_vbo(VBO_Index vbo_idx, unsigned int num_components, const std::vector<float>& elements)
	{
		glBindBuffer(GL_ARRAY_BUFFER, _impl->_vbos[vbo_idx]);
		glBufferData(GL_ARRAY_BUFFER, elements.size() * sizeof(float), elements.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(vbo_idx, num_components, GL_FLOAT, GL_FALSE, 0, NULL); // 0 stride indicates tight packing
		glEnableVertexAttribArray(vbo_idx);
		_impl->_vbo_num_elements[vbo_idx] = static_cast<unsigned int>(elements.size()) / num_components;
		_impl->_vbo_num_components[vbo_idx] = num_components;
	}

	void model::load_to_vbo(VBO_Index vbo_idx, unsigned int num_components, const std::vector<int>& elements)
	{
		glBindBuffer(GL_ARRAY_BUFFER, _impl->_vbos[vbo_idx]);
		glBufferData(GL_ARRAY_BUFFER, elements.size() * sizeof(int), elements.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(vbo_idx, num_components, GL_INT, GL_FALSE, 0, NULL); // 0 stride indicates tight packing
		glEnableVertexAttribArray(vbo_idx);
		_impl->_vbo_num_elements[vbo_idx] = static_cast<unsigned int>(elements.size()) / num_components;
		_impl->_vbo_num_components[vbo_idx] = num_components;
	}

	void model::load_to_vbo(VBO_Index vbo_idx, unsigned int num_components, const std::vector<unsigned int>& elements)
	{
		glBindBuffer(GL_ARRAY_BUFFER, _impl->_vbos[vbo_idx]);
		glBufferData(GL_ARRAY_BUFFER, elements.size() * sizeof(unsigned int), elements.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(vbo_idx, num_components, GL_UNSIGNED_INT, GL_FALSE, 0, NULL); // 0 stride indicates tight packing
		glEnableVertexAttribArray(vbo_idx);
		_impl->_vbo_num_elements[vbo_idx] = static_cast<unsigned int>(elements.size()) / num_components;
		_impl->_vbo_num_components[vbo_idx] = num_components;
	}

	void model::load_to_vbo(VBO_Index vbo_idx, unsigned int num_components, const std::vector<short>& elements)
	{
		glBindBuffer(GL_ARRAY_BUFFER, _impl->_vbos[vbo_idx]);
		glBufferData(GL_ARRAY_BUFFER, elements.size() * sizeof(short), elements.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(vbo_idx, num_components, GL_SHORT, GL_FALSE, 0, NULL); // 0 stride indicates tight packing
		glEnableVertexAttribArray(vbo_idx);
		_impl->_vbo_num_elements[vbo_idx] = static_cast<unsigned int>(elements.size()) / num_components;
		_impl->_vbo_num_components[vbo_idx] = num_components;
	}

	void model::load_to_vbo(VBO_Index vbo_idx, unsigned int num_components, const std::vector<unsigned short>& elements)
	{
		glBindBuffer(GL_ARRAY_BUFFER, _impl->_vbos[vbo_idx]);
		glBufferData(GL_ARRAY_BUFFER, elements.size() * sizeof(unsigned short), elements.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(vbo_idx, num_components, GL_UNSIGNED_SHORT, GL_FALSE, 0, NULL); // 0 stride indicates tight packing
		glEnableVertexAttribArray(vbo_idx);
		_impl->_vbo_num_elements[vbo_idx] = static_cast<unsigned int>(elements.size()) / num_components;
		_impl->_vbo_num_components[vbo_idx] = num_components;
	}

	void model::load_to_vbo(VBO_Index vbo_idx, unsigned int num_components, const std::vector<char>& elements)
	{
		glBindBuffer(GL_ARRAY_BUFFER, _impl->_vbos[vbo_idx]);
		glBufferData(GL_ARRAY_BUFFER, elements.size() * sizeof(char), elements.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(vbo_idx, num_components, GL_BYTE, GL_FALSE, 0, NULL); // 0 stride indicates tight packing
		glEnableVertexAttribArray(vbo_idx);
		_impl->_vbo_num_elements[vbo_idx] = static_cast<unsigned int>(elements.size()) / num_components;
		_impl->_vbo_num_components[vbo_idx] = num_components;
	}

	void model::load_to_vbo(VBO_Index vbo_idx, unsigned int num_components, const std::vector<unsigned char>& elements)
	{
		glBindBuffer(GL_ARRAY_BUFFER, _impl->_vbos[vbo_idx]);
		glBufferData(GL_ARRAY_BUFFER, elements.size() * sizeof(unsigned char), elements.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(vbo_idx, num_components, GL_UNSIGNED_BYTE, GL_FALSE, 0, NULL); // 0 stride indicates tight packing
		glEnableVertexAttribArray(vbo_idx);
		_impl->_vbo_num_elements[vbo_idx] = static_cast<unsigned int>(elements.size()) / num_components;
		_impl->_vbo_num_components[vbo_idx] = num_components;
	}

	void model::load_to_attribute(VBO_Index vbo_idx, Attribute_Index attr_idx, unsigned int num_components, unsigned int stride_in_num_components, unsigned int offset_in_num_components, const std::vector<float>& elements)
	{
		glBindBuffer(GL_ARRAY_BUFFER, _impl->_vbos[vbo_idx]);
		glBufferData(GL_ARRAY_BUFFER, elements.size() * sizeof(float), elements.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(attr_idx, num_components, GL_FLOAT, GL_FALSE, stride_in_num_components * sizeof(float), (void*)(offset_in_num_components * sizeof(float)));
		glEnableVertexAttribArray(vbo_idx);
		_impl->_vbo_num_elements[vbo_idx] = static_cast<unsigned int>(elements.size()) / stride_in_num_components;
		_impl->_vbo_num_components[vbo_idx] = stride_in_num_components;
	}

	void model::load_to_attribute(Attribute_Index attr_idx, unsigned int num_components, unsigned int stride_in_num_components, unsigned int offset_in_num_components)
	{
		glVertexAttribPointer(attr_idx, num_components, GL_FLOAT, GL_FALSE, stride_in_num_components * sizeof(float), (void*)(offset_in_num_components * sizeof(float)));
	}

	void model::update_vbo(VBO_Index vbo_idx, const std::vector<float>& elements)
	{
		glBindBuffer(GL_ARRAY_BUFFER, _impl->_vbos[vbo_idx]);
		glBufferData(GL_ARRAY_BUFFER, elements.size() * sizeof(float), elements.data(), GL_STATIC_DRAW);
		_impl->_vbo_num_elements[vbo_idx] = static_cast<unsigned int>(elements.size()) / _impl->_vbo_num_components[vbo_idx];
	}

	void model::update_vbo(VBO_Index vbo_idx, const std::vector<int>& elements)
	{
		glBindBuffer(GL_ARRAY_BUFFER, _impl->_vbos[vbo_idx]);
		glBufferData(GL_ARRAY_BUFFER, elements.size() * sizeof(int), elements.data(), GL_STATIC_DRAW);
		_impl->_vbo_num_elements[vbo_idx] = static_cast<unsigned int>(elements.size()) / _impl->_vbo_num_components[vbo_idx];
	}

	void model::update_vbo(VBO_Index vbo_idx, const std::vector<unsigned int>& elements)
	{
		glBindBuffer(GL_ARRAY_BUFFER, _impl->_vbos[vbo_idx]);
		glBufferData(GL_ARRAY_BUFFER, elements.size() * sizeof(unsigned int), elements.data(), GL_STATIC_DRAW);
		_impl->_vbo_num_elements[vbo_idx] = static_cast<unsigned int>(elements.size()) / _impl->_vbo_num_components[vbo_idx];
	}

	void model::update_vbo(VBO_Index vbo_idx, const std::vector<short>& elements)
	{
		glBindBuffer(GL_ARRAY_BUFFER, _impl->_vbos[vbo_idx]);
		glBufferData(GL_ARRAY_BUFFER, elements.size() * sizeof(short), elements.data(), GL_STATIC_DRAW);
		_impl->_vbo_num_elements[vbo_idx] = static_cast<unsigned int>(elements.size()) / _impl->_vbo_num_components[vbo_idx];
	}

	void model::update_vbo(VBO_Index vbo_idx, const std::vector<unsigned short>& elements)
	{
		glBindBuffer(GL_ARRAY_BUFFER, _impl->_vbos[vbo_idx]);
		glBufferData(GL_ARRAY_BUFFER, elements.size() * sizeof(unsigned short), elements.data(), GL_STATIC_DRAW);
		_impl->_vbo_num_elements[vbo_idx] = static_cast<unsigned int>(elements.size()) / _impl->_vbo_num_components[vbo_idx];
	}

	void model::update_vbo(VBO_Index vbo_idx, const std::vector<char>& elements)
	{
		glBindBuffer(GL_ARRAY_BUFFER, _impl->_vbos[vbo_idx]);
		glBufferData(GL_ARRAY_BUFFER, elements.size() * sizeof(char), elements.data(), GL_STATIC_DRAW);
		_impl->_vbo_num_elements[vbo_idx] = static_cast<unsigned int>(elements.size()) / _impl->_vbo_num_components[vbo_idx];
	}

	void model::update_vbo(VBO_Index vbo_idx, const std::vector<unsigned char>& elements)
	{
		glBindBuffer(GL_ARRAY_BUFFER, _impl->_vbos[vbo_idx]);
		glBufferData(GL_ARRAY_BUFFER, elements.size() * sizeof(unsigned char), elements.data(), GL_STATIC_DRAW);
		_impl->_vbo_num_elements[vbo_idx] = static_cast<unsigned int>(elements.size()) / _impl->_vbo_num_components[vbo_idx];
	}

	void model::load_to_ibo(const std::vector<unsigned char>& indices)
	{
		_impl->_num_indices = static_cast<unsigned int>(indices.size());
		glGenBuffers(1, &_impl->_ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _impl->_ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned char), indices.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void model::load_to_ibo(const std::vector<unsigned short>& indices)
	{
		_impl->_num_indices = static_cast<unsigned int>(indices.size());
		glGenBuffers(1, &_impl->_ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _impl->_ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), indices.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void model::load_to_ibo(const std::vector<unsigned int>& indices)
	{
		_impl->_num_indices = static_cast<unsigned int>(indices.size());
		glGenBuffers(1, &_impl->_ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _impl->_ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}