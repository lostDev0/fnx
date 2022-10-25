#pragma once

namespace fnx
{
	class material : public fnx::asset
	{
	public:
		material() = delete;
		~material() = default;
		material(const std::string& name) : asset(name)
		{
			FNX_DEBUG(fnx::format_string("initializing material %s", get_name()));
		}
		
		void add_texture(const std::string& name, fnx::texture_handle value) { _textures[name] = value; }
		void add_float(const std::string& name, float value) { _floats[name] = value; }
		void add_int(const std::string& name, int value) { _ints[name] = value; }
		void add_vec2f(const std::string& name, const reactphysics3d::Vector2& value) { _vec2fs[name] = value; }
		void add_vec3f(const std::string& name, const reactphysics3d::Vector3& value) { _vec3fs[name] = value; }
		void add_vec4f(const std::string& name, const reactphysics3d::Quaternion& value) { _vec4fs[name] = value; }
		void add_matrix4x4(const std::string& name, const fnx::matrix4x4& value) { _matrix4x4s[name] = value; }

		const auto& get_textures() const { return _textures; }
		const auto& get_floats() const { return _floats; }
		const auto& get_ints() const { return _ints; }
		const auto& get_vec2fs() const { return _vec2fs; }
		const auto& get_vec3fs() const { return _vec3fs; }
		const auto& get_vec4fs() const { return _vec4fs; }
		const auto& get_matrix4x4s() const { return _matrix4x4s; }

		bool get(const std::string& name, fnx::texture_handle& out) { return get(name, _textures, out); }
		bool get(const std::string& name, float& out) { return get(name, _floats, out); }
		bool get(const std::string& name, int& out) { return get(name, _ints, out); }
		bool get(const std::string& name, reactphysics3d::Vector2& out) { return get(name, _vec2fs, out); }
		bool get(const std::string& name, reactphysics3d::Vector3& out) { return get(name, _vec3fs, out); }
		bool get(const std::string& name, reactphysics3d::Quaternion& out) { return get(name, _vec4fs, out); }
		bool get(const std::string& name, fnx::matrix4x4& out) { return get(name, _matrix4x4s, out); }

		void add_array_vec4fs(const std::string& name, const std::vector<reactphysics3d::Quaternion>& src) { _arr_vec4fs[name] = src; }
		const auto& get_array_vec4fs() const { return _arr_vec4fs; }

	private:
		std::unordered_map<std::string, fnx::texture_handle> _textures;
		std::unordered_map<std::string, float> _floats;
		std::unordered_map<std::string, int> _ints;
		std::unordered_map<std::string, reactphysics3d::Vector2> _vec2fs;
		std::unordered_map<std::string, reactphysics3d::Vector3> _vec3fs;
		std::unordered_map<std::string, reactphysics3d::Quaternion> _vec4fs;
		std::unordered_map<std::string, fnx::matrix4x4> _matrix4x4s;

		std::unordered_map<std::string, std::vector<reactphysics3d::Quaternion>> _arr_vec4fs;

		template<typename ReturnType, typename SourceType>
		const bool get(const std::string& name, const SourceType& lookup, ReturnType& out)
		{
			auto it = lookup.find(name);
			if (it != std::end(lookup))
			{
				out = it->second;
				return true;
			}
			return false;
		}
	};

	using material_handle = fnx::asset_handle<fnx::material>;

	/// @brief Parses a file and puts all of the material assets into the asset manager.
	/// @param[in] file_path : local file system path
	extern void parse_material_file(const std::string& file_path);
}
