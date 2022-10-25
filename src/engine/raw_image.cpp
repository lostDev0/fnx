//#define STB_IMAGE_IMPLEMENTATION
#include "stbimage/stb_image.h"

namespace fnx
{
	raw_image::raw_image(const std::string& file_path, int32_t channels)
	{
		load_from_file(file_path, channels);
	}

	bool raw_image::load_from_file(const std::string& file_path, int32_t channels)
	{
		int width, height, components;
		_info._stb_data = stbi_load(file_path.c_str(), &width, &height, &components, channels);

		if (nullptr != _info._stb_data)
		{
			_info._components = components;
			_info._channels = channels;
			_info._width = width;
			_info._height = height;
			_info._size = _info._width * _info._height * _info._channels;
			FNX_DEBUG(fnx::format_string("loaded image from file %s", file_path.c_str()));
			_info._is_ok = true;
			_info._is_stb = true;
			_info._data = _info._stb_data;
		}
		else
		{
			_info._is_ok = false;
			FNX_ERROR(fnx::format_string("unable to load image %s", file_path.c_str()));
		}

		return _info._is_ok;
	}

	raw_image::raw_image(void* data, size_t size, int32_t width, int32_t height, int channels)
	{
		load_from_buffer(data, size, width, height, channels);
	}

	bool raw_image::load_from_buffer(void* data, size_t size, int32_t width, int32_t height, int channels)
	{
		_info._data = data;
		_info._size = size;
		_info._width = width;
		_info._height = height;
		_info._channels = channels;
		_info._is_ok = _info._stb_data != nullptr && _info._size == (_info._width * _info._height * 8);
		
		if(!_info._is_ok)
		{
			FNX_ERROR("unable to load image from data bufffer");
		}

		return _info._is_ok;
	}

	raw_image::~raw_image()
	{
		unload();
	}

	void raw_image::unload()
	{
		if (_info._is_ok && _info._stb_data != nullptr && _info._is_stb)
		{
			stbi_image_free(_info._stb_data);
		}
		else if (_info._is_ok && _info._data != nullptr && !_info._is_stb)
		{
			free(_info._data);
		}

		_info._data = nullptr;
		_info._stb_data = nullptr;
	}
}