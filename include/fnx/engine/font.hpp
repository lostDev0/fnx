#pragma once

namespace fnx
{
	class font : public fnx::asset
	{
	public:
		struct CharInfo
		{
			//char id=32   x=0     y=0     width=0     height=0     xoffset=0     yoffset=71    xadvance=27     page=0  chnl=0
			short x, y, width, height, xoffset, yoffset, xadvance;
			CharInfo(short x, short y, short width, short height, short xoffset, short yoffset, short xadvance)
				: x(x)
				, y(y)
				, width(width)
				, height(height)
				, xoffset(xoffset)
				, yoffset(yoffset)
				, xadvance(xadvance)
			{
			}

			CharInfo()
				: x(0)
				, y(0)
				, width(0)
				, height(0)
				, xoffset(0)
				, yoffset(0)
				, xadvance(0)
			{}
			~CharInfo() = default;
		};

		font(const std::string& font_family, fnx::asset_manager<fnx::texture>& textures);
		~font();

		/// @brief Calculates the model and texture coordinates for use by a shader.
		/// @return Returns the dimensions of the string.
		fnx::vector2 calculate_texture_model_info(fnx::alignment align, const fnx::vector2& size_limits, float font_size_in_pixels, const std::string& text, std::vector<float>& model_coords,
			std::vector<float>& text_coords, float window_width, float window_height, fnx::vector2& cursor, std::vector<std::pair<float, std::string>>& lines);

		float calculate_line_height(float font_size, float window_height);

		auto get_texture() { return _texture; };
		auto get_char_info(const char c) { return _character_info[c]; }

	protected:
		fnx::asset_manager<fnx::texture>& _texture_manager;
		fnx::texture_handle _texture;
		short _line_height;
		short _base;
		std::unordered_map<char, CharInfo> _character_info;

		fnx::vector2 calculate_string_size(const fnx::vector2& size_limits, const std::string& text, float font_height_in_pixels, 
			float window_width, float window_height, std::vector<std::pair<float, std::string>>& lines);

		void init(fnx::asset_manager<fnx::texture>& textures, const std::string& texture_file_path, const std::string& char_map_file_path);
	};

	using font_handle = fnx::asset_handle<fnx::font>;
}
