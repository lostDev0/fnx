#pragma once

namespace fnx
{
	struct sound_impl;
	/// @brief Asset class to handle low level sound implementations.
	/// @note This class should only be used internally to the fnx engine.
	class sound : public fnx::asset
	{
	public:
		sound(const std::string& file_path);
		~sound();

		void play();
		void loop();
		void pause();
		void stop();
		void set_volume(float left, float right);
		void apply_master_volume(float left, float right);

		/// @brief Call every cycle to merge sounds into audio output.
		static void mix();

		/// @brief Call this when mixing occurs on a thread. The context must be initialized within the window context.
		static void initialize_sound_context();

	private:
		float _volume_left{ 1.f };
		float _volume_right{ 1.f };
		std::unique_ptr<sound_impl> _impl;
	};

	using sound_handle = std::shared_ptr<fnx::sound>;
}
