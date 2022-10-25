using namespace std;

namespace fnx
{
	font::font(const std::string& font_family, fnx::asset_manager<fnx::texture>& textures)
		: fnx::asset(font_family)
		, _texture_manager{ textures }
	{
		init(_texture_manager, font_family + ".png", font_family + ".fnt");
	}

	void font::init(fnx::asset_manager<fnx::texture>& textures, const std::string& texture_file_path, const std::string& char_map_file_path)
	{
		// disable mipmapping
		// mipmapping can be enabled if text is drawn at various depths and may gain some quality improvements
		texture::config cfg;
		cfg._enable_mip = false;
		_texture = textures.get(texture_file_path, cfg); //, 4, Format::RGBA, Format::RGBA, Filter::Linear, Attachment::None, true )
		_line_height = { 0 };
		_base = { 0 };
		ifstream in(char_map_file_path);

		if (in.good())
		{
			string line;

			while (getline(in, line))
			{
				auto words = split(line, " =", true);

				if (words.size() > 0 && words[0] == "char")
				{
					// start of char information, assuming every line is the same format, so no robustness
					//char id=32   x=0     y=0     width=0     height=0     xoffset=0     yoffset=71    xadvance=27     page=0  chnl=0
					_character_info.emplace(std::piecewise_construct,
						std::forward_as_tuple(static_cast<char>(stol(words[2]))),
						std::forward_as_tuple(static_cast<short>(stol(words[4])),
							static_cast<short>(stol(words[6])),
							static_cast<short>(stol(words[8])),
							static_cast<short>(stol(words[10])),
							static_cast<short>(stol(words[12])),
							static_cast<short>(stol(words[14])),
							static_cast<short>(stol(words[16]))));
				}
				else if (words.size() > 0 && words[0] == "common")
				{
					//common lineHeight=95 base=71 scaleW=512 scaleH=512 pages=1 packed=0
					_line_height = static_cast<short>(stol(words[2]));
					_base = static_cast<short>(stol(words[4]));
				}
			}

			FNX_DEBUG(fnx::format_string("loaded font from file %s", char_map_file_path.c_str()));
		}
		else
		{
			FNX_ERROR(fnx::format_string("unable to load font file %s", char_map_file_path.c_str()));
		}
	}

	font::~font()
	{
	}

	float font::calculate_line_height(float font_size_in_pixels, float window_height)
	{
		auto& a_char = _character_info[0x41];   // A

		// font height in pixels divided by texture height in pixels gives half scale opengl height
		// equivalent to the number of times the texture height needs scaled to be the correct height in pixels
		float font_scale = font_size_in_pixels / static_cast<float>(a_char.height);
		return ((a_char.height * 2.f) / window_height) * font_scale;
	}

	reactphysics3d::Vector2 font::calculate_string_size(const reactphysics3d::Vector2& size_limits, const std::string& text, float font_height_in_pixels, 
		float window_width, float window_height, std::vector<std::pair<float, std::string>>& lines)
	{
		auto& a_char = _character_info[0x41];   // A
		auto& dot_char = _character_info[0x2E];   // .
		float font_scale = font_height_in_pixels / static_cast<float>(a_char.height);
		auto line_height_in_opengl = calculate_line_height(font_height_in_pixels, window_height);
		float width_to_opengl = 2.f / window_width;		/// conversion factor for width
		float width_of_small_char = static_cast<float>(dot_char.xadvance) * width_to_opengl * font_scale;	/// opengl width of the . character

		// lines = width,string
		reactphysics3d::Vector2 ret{ 0.f,0.f };
		auto word_start_index = 0u;
		auto current_line_whole_words_start = 0u;		// start of the current line
		auto current_line_whole_words_end = 0u;		// end of the current line
		float current_line_whole_words_length = 0.f;	// only the whole words that are known to fit on the line
		float current_width = 0.f;
		bool save_line = false;
		bool at_start_of_line = true;
		bool overrun = false;

		for(auto idx = 0u; idx < text.size(); ++idx)
		{
			auto c_str = text[idx];
			const auto& c = _character_info[c_str];

			if (at_start_of_line)
			{
				if (0x20 == c_str)	// space
				{
					continue;
				}
				current_line_whole_words_start = idx;
				word_start_index = idx;
				current_line_whole_words_length = 0.f;
				current_width = 0.f;
			}
			else
			{
				if (0x20 == c_str)	// space
				{
					if (overrun)
					{
						// can only put a single word on the line
						overrun = false;
						current_line_whole_words_end = word_start_index - 1;
						//idx = current_line_whole_words_end;

						save_line = true;
						at_start_of_line = true;
					}
					else
					{
						current_line_whole_words_end = idx;
						word_start_index = idx + 1;
						current_line_whole_words_length = current_width;
					}
				}
			}
	
			if (!save_line && c.xadvance > 0)
			{
				// char, advance x right
				current_width += static_cast<float>(c.xadvance) * width_to_opengl * font_scale;
				at_start_of_line = false;
			}
			else if (!save_line && 0x0a == c_str)	// new line
			{
				save_line = true;
				at_start_of_line = true;
				current_line_whole_words_end = idx;
			}

			// handle wrapping of the line
			if (!save_line && size_limits.x > 0.0f && ((current_width + width_of_small_char) >= size_limits.x))
			{
				// backup and find the word
				if (overrun)
				{
					// just keep adding letters until we find a space
				}
				else if (current_line_whole_words_start == word_start_index)
				{
					// couldn't fit this word on the line
					overrun = true;
				}
				else
				{
					overrun = false;
					current_line_whole_words_end = word_start_index - 1;
					idx = current_line_whole_words_end;

					save_line = true;
					at_start_of_line = true;
				}				
			}
			else if (idx == text.size() - 1)
			{
				// at the end of a line or only one line exists
				save_line = true;
				current_line_whole_words_end = idx + 1;
				current_line_whole_words_length = current_width;
			}

			if (save_line)
			{
				if (current_line_whole_words_end > current_line_whole_words_start)
				{
					auto str = text.substr(current_line_whole_words_start, current_line_whole_words_end - current_line_whole_words_start);
					lines.emplace_back(std::make_pair(current_line_whole_words_length, str));
					current_line_whole_words_start = current_line_whole_words_end;	// advance the next string to the remaining character string
				}
			}

			save_line = false;
		}

		if (current_line_whole_words_end - current_line_whole_words_start > 0)
		{
			if (current_line_whole_words_end > current_line_whole_words_start)
			{
				auto str = text.substr(current_line_whole_words_start, current_line_whole_words_end - current_line_whole_words_start);
				lines.emplace_back(std::make_pair(current_line_whole_words_length, str));
			}
		}

		for(auto i = 0u;i<lines.size();++i)
		{
			ret.x = fnx::maximum(ret.x, lines[i].first);
			if(i > 0)	// Skip one line to ensure that the starting y value is correct due to the origin location
				ret.y += line_height_in_opengl;
		}

		return ret;
	}

	reactphysics3d::Vector2 font::calculate_texture_model_info(fnx::alignment align, const reactphysics3d::Vector2& size_limits, float font_height_in_pixels, 
		const std::string& text, std::vector<float>& model_coords, std::vector<float>& text_coords, 
		float window_width, float window_height, reactphysics3d::Vector2& cursor_in_opengl_coords, std::vector<std::pair<float, std::string>>& line_map)
	{
		assert(window_width > 0);
		assert(window_height > 0);
		// prepare vectors for memory population
		model_coords.reserve(text.size() * 6 * 3);
		text_coords.reserve(text.size() * 6 * 2);
		// find the ratio to the font size to be used for the raw model info
		auto& a_char = _character_info[0x41];   // A
		auto& dot_char = _character_info[0x2E];   // .
		float font_scale = font_height_in_pixels / static_cast<float>(a_char.height);
		reactphysics3d::Vector2 texture_dim(static_cast<float>(_texture->width()), static_cast<float>(_texture->height()));

		auto lines = 0u;
		auto line_height_in_pixels = font_height_in_pixels;
		auto line_height_in_opengl = calculate_line_height(font_height_in_pixels, window_height);

		float width_to_opengl = 2.f / window_width;		/// conversion factor for width
		float height_to_opengl = 2.f / window_height;	/// conversion factor for height (does not honor aspect ratio)
		float width_of_small_char = static_cast<float>(dot_char.xadvance)* width_to_opengl* font_scale;	/// opengl width of the . character
		float max_width = size_limits.x;				/// maximum opengl size for calculating horizontal positioning (cannot overrun)
		float max_height = size_limits.y;				/// maximum opengl height for calculating vertical positioning (can overrun)
		float current_width = 0.f;						/// track the current width no matter the alignment
		float x_start = 0;

		size_t line_num = 0;
		
		line_map.clear();
		auto size = calculate_string_size(size_limits, text, font_height_in_pixels, window_width, window_height, line_map);

		if (line_map.empty())
			return size;

		if (align == fnx::alignment::bottom ||
			align == fnx::alignment::bottom_left ||
			align == fnx::alignment::bottom_right)
		{
			cursor_in_opengl_coords.y = size.y;
		}
		else if (align == fnx::alignment::center ||
			align == fnx::alignment::left ||
			align == fnx::alignment::right)
		{
			cursor_in_opengl_coords.y = (size_limits.y/2.f + (size.y - line_height_in_opengl/2.f)/2.f);
		}
		else if (align == fnx::alignment::top ||
			align == fnx::alignment::top_left ||
			align == fnx::alignment::top_right)
		{
			// 0,0 of the model is the bottom left corner of the character, the line needs moved down by 1 height unit
			cursor_in_opengl_coords.y = (size_limits.y - line_height_in_opengl);
		}

		if (align == fnx::alignment::right ||
			align == fnx::alignment::top_right ||
			align == fnx::alignment::bottom_right)
		{
			x_start = size_limits.x - line_map[line_num].first;
			cursor_in_opengl_coords.x = x_start;
		}
		else if (align == fnx::alignment::center || 
			align == fnx::alignment::bottom || 
			align == fnx::alignment::top)
		{
			x_start = size_limits.x / 2.f - line_map[line_num].first / 2.f;
			cursor_in_opengl_coords.x = x_start;
		}

		for (const auto& p : line_map)
		{
			for (const auto& c_str : p.second)
			{
				const auto& c = _character_info[c_str];

				/*
				Model                   Texture from character position
				d                 c     d(0,0)            c
				+-------------+         +-------------+
				|             |         |             |
				|             |         |             |
				|             |         |             |
				|             |         |             |
				|             |         |             |
				|             |         |             |
				+-------------+         +-------------+
				a(0,0)            b     a(0,1)           b
				*/
				if (c.width > 0)
				{
					reactphysics3d::Vector2 char_in_opengl_coords(static_cast<float>(c.width)* width_to_opengl,
						static_cast<float>(c.height)* height_to_opengl);
					reactphysics3d::Vector2 offset_in_opengl_coords(static_cast<float>(c.xoffset)* width_to_opengl,
						static_cast<float>(c.yoffset)* height_to_opengl);

					// upscale coordinates to the provided font size
					char_in_opengl_coords *= font_scale;
					offset_in_opengl_coords *= font_scale;

					reactphysics3d::Vector2 texture_coords(calc_ratio(0.f, texture_dim.x, static_cast<float>(c.x)),
						calc_ratio(0.f, texture_dim.y, static_cast<float>(c.y)));
					reactphysics3d::Vector2 char_text_dim(calc_ratio(0.f, texture_dim.x, static_cast<float>(c.width)),
						calc_ratio(0.f, texture_dim.y, static_cast<float>(c.height)));
					float a_x, a_y, b_x, b_y, c_x, c_y, d_x, d_y; // char location within texture
					float m_a_x, m_a_y, m_b_x, m_b_y, m_c_x, m_c_y, m_d_x, m_d_y, m_z;    // char location on screen
					float y_offset = static_cast<float>(_base - c.yoffset - c.height)* height_to_opengl* font_scale;

					// uv coords
					a_x = texture_coords.x;
					a_y = texture_coords.y + char_text_dim.y;
					b_x = texture_coords.x + char_text_dim.x;
					b_y = texture_coords.y + char_text_dim.y;
					c_x = texture_coords.x + char_text_dim.x;
					c_y = texture_coords.y;
					d_x = texture_coords.x;
					d_y = texture_coords.y;

					// opengl xyz coords
					m_a_x = cursor_in_opengl_coords.x + offset_in_opengl_coords.x;
					m_a_y = cursor_in_opengl_coords.y + y_offset;
					m_b_x = cursor_in_opengl_coords.x + char_in_opengl_coords.x + offset_in_opengl_coords.x;
					m_b_y = cursor_in_opengl_coords.y + y_offset;
					m_c_x = cursor_in_opengl_coords.x + char_in_opengl_coords.x + offset_in_opengl_coords.x;
					m_c_y = cursor_in_opengl_coords.y + char_in_opengl_coords.y + y_offset;
					m_d_x = cursor_in_opengl_coords.x + offset_in_opengl_coords.x;
					m_d_y = cursor_in_opengl_coords.y + char_in_opengl_coords.y + y_offset;
					m_z = 0.0f;

					model_coords.emplace_back(m_a_x);
					model_coords.emplace_back(m_a_y);
					model_coords.emplace_back(m_z);
					text_coords.emplace_back(a_x);
					text_coords.emplace_back(a_y);
					model_coords.emplace_back(m_b_x);
					model_coords.emplace_back(m_b_y);
					model_coords.emplace_back(m_z);
					text_coords.emplace_back(b_x);
					text_coords.emplace_back(b_y);
					model_coords.emplace_back(m_c_x);
					model_coords.emplace_back(m_c_y);
					model_coords.emplace_back(m_z);
					text_coords.emplace_back(c_x);
					text_coords.emplace_back(c_y);
					model_coords.emplace_back(m_c_x);
					model_coords.emplace_back(m_c_y);
					model_coords.emplace_back(m_z);
					text_coords.emplace_back(c_x);
					text_coords.emplace_back(c_y);
					model_coords.emplace_back(m_d_x);
					model_coords.emplace_back(m_d_y);
					model_coords.emplace_back(m_z);
					text_coords.emplace_back(d_x);
					text_coords.emplace_back(d_y);
					model_coords.emplace_back(m_a_x);
					model_coords.emplace_back(m_a_y);
					model_coords.emplace_back(m_z);
					text_coords.emplace_back(a_x);
					text_coords.emplace_back(a_y);
				}

				auto char_width = static_cast<float>(c.xadvance)* width_to_opengl* font_scale;
				current_width += char_width;
				cursor_in_opengl_coords.x += char_width;
			}

			// handle wrapping of the line
			++line_num;
			if (align == fnx::alignment::right ||
				align == fnx::alignment::top_right ||
				align == fnx::alignment::bottom_right)
			{
				x_start = size_limits.x - (line_num < line_map.size() ? line_map[line_num].first : 0.f);
			}
			else if (align == fnx::alignment::center ||
				align == fnx::alignment::bottom ||
				align == fnx::alignment::top)
			{
				x_start = size_limits.x / 2.f - line_map[line_num].first / 2.f;
			}

			cursor_in_opengl_coords.x = x_start;
			cursor_in_opengl_coords.y -= (line_height_in_opengl);
			current_width = 0.f;
		}
		return size;
	}
}