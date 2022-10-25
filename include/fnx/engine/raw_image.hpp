#pragma once

namespace fnx
{
	class raw_image
	{
	public:

		struct info
		{
			uint32_t _width{};
			uint32_t _height{};
			uint32_t _components{};
			uint32_t _channels{ 4 };
			size_t _size{};
			unsigned char* _stb_data{ nullptr };
			void* _data{ nullptr };
			bool _is_ok{ false };
			bool _is_stb{ false };
		};

		raw_image() = default;
		raw_image(void* data, size_t size, int32_t width, int32_t height, int channels = 4);
		raw_image(const std::string& file_path, int32_t channels = 4);
		~raw_image();

		/// @brief Load the image from a file path with a partiular number of channels.
		/// @param[in] file_path : file system location
		/// @param[in] channels : number of RGBA channels.
		/// @note channels of 0 means grab only the channels present in the file. If the image only has RGB channels,
		///		and channels value is 4, the Alpha channel will be added to the output buffer.
		bool load_from_file(const std::string& file_path, int32_t channels = 4);

		/// @brief Load the image from a file path with a partiular number of channels.
		/// @param[in] data : buffer
		/// @param[in] size : size of buffer
		/// @param[in] width : width in pixels
		/// @param[in] height : height in pixels
		/// @param[in] channels : number of RGBA channels.
		/// @note channels of 0 means grab only the channels present in the file. If the image only has RGB channels,
		///		and channels value is 4, the Alpha channel will be added to the output buffer.
		bool load_from_buffer(void* data, size_t size, int32_t width, int32_t height, int channels = 4);

		/// @brief Free memory
		/// @note is_ok will become false
		void unload();

		auto operator=(const raw_image& other) = delete;
		raw_image(const raw_image& other) = delete;
		raw_image(raw_image&& other) = default;

		const auto& get_info() const
		{
			return _info;
		}
	private:
		fnx::raw_image::info _info;
	};
}
