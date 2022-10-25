#pragma once

namespace fnx
{
	class texture : public fnx::asset
	{
		struct impl;
	public:

		struct config
		{
			struct mip_t
			{
				bool _enable{ false };
				explicit mip_t(bool enable=false) noexcept : _enable(enable){}
				~mip_t() = default;
			};

			config(mip_t enable) : _enable_mip{ enable._enable } {};
			config() = default;
			~config() = default;

			int _components{ 0u };
			unsigned int _channels{ 4u };
			fnx::format _format{ fnx::format::RGBA };
			fnx::format _internal_format{ fnx::format::RGBA };
			fnx::filter _filter{ fnx::filter::Linear };
			fnx::attachment _attachment{ fnx::attachment::None };
			bool _clamp{ false };
			bool _enable_mip{ true };
			float _mip_bias{ -.4f };
			unsigned char _rows{ 1 };
			unsigned char _cols{ 1 };
		};

		texture(const std::string& file_path);
		texture(const std::string& file_path, uint32_t target);
		texture(const std::string& file_path, const fnx::texture::config& config);
		texture(const std::string& file_path, unsigned int channels,
			fnx::format format, fnx::format internal_format, fnx::filter filter,
			fnx::attachment attachment, bool clamp, bool enable_mip_mapping, float mip_bias, unsigned char rows, unsigned char cols);
		texture(unsigned int width, unsigned int height, unsigned int channels, 
			fnx::format format, fnx::format internal_format, fnx::filter filter, 
			fnx::attachment attachment, bool clamp, bool enable_mip_mapping, float mip_bias, unsigned char rows, unsigned char cols);
		~texture();

		/// @brief Return with width of the texture in pixels.
		inline auto width() const { return _image.get_info()._width; }

		/// @brief Return the height of the texture in pixels.
		inline auto height() const { return _image.get_info()._height; }
		
		/// @brief Return the number of channels.
		inline auto channels() const { return _image.get_info()._channels; }
		
		/// @brief Return the width in pixels of a single tile within the texture map.
		inline auto atlas_tile_width() const { return _tile_width; }
		
		/// @brief Return the height in pixels of a single tile within the texture map.
		inline auto atlas_tile_height() const { return _tile_height; }

		/// @brief Return the number of rows within the texture map.
		inline auto atlas_num_rows() const { return _rows; }

		/// @brief Return the number of columns within the texture map.
		inline auto atlas_num_cols() const { return _cols; }

		/// @brief Return the position within the texture map for the given index.
		reactphysics3d::Vector2 calc_atlas_offset(unsigned char index);

		/// @brief Return the pixel at an row,col coordinate.
		void pixel(unsigned int row, unsigned int col, char& r, char& g, char& b, char& a) const;
		
		/// @brief Bind the current texture to the context.
		void bind(unsigned int unit) const;
		
		/// @brief Tell the target to accept subsequent opengl calls.
		void set_render_target();

	private:
		fnx::raw_image _image;
		fnx::format _format{ fnx::format::RGBA };
		fnx::format _internal_format{ fnx::format::RGBA };
		fnx::filter _filter{ fnx::filter::Linear };
		fnx::attachment _attachment{ fnx::attachment::None };
		bool _clamp{ false };
		bool _enable_mip{ false };
		float _mip_bias{ -.4f };
		unsigned char _rows{ 1 };
		unsigned char _cols{ 1 };
		unsigned int _tile_width{ 0u };
		unsigned int _tile_height{ 0u };

		impl* _impl{ nullptr };
		void init();
	};

	using texture_handle = fnx::asset_handle<fnx::texture>;
}