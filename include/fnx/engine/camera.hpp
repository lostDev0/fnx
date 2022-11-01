#pragma once

namespace fnx
{
	class camera
	{
	public:
		camera();
		~camera();
		camera(const camera& other) = default;

		virtual void set_position(const fnx::vector3& position)
		{
			_position = position;
			update_view_matrix();
		}

		virtual void set_position(float x, float y, float z)
		{
			_position.x = x;
			_position.y = y;
			_position.z = z;
			update_view_matrix();
		}

		void look_at(const fnx::vector3& target, const fnx::vector3& up)
		{
			_target = target;
			_front = _target - _position;
			_up = up;
			update_view_matrix();
		}

		const auto& get_projection_matrix() { return _projection_matrix; }
		const auto& get_view_matrix() { return _view_matrix; }
		const auto& get_view_projection_matrix() { return _view_projection_matrix; }
		const auto& get_position() { return _position; }
		const auto& get_front() { return _front; }
		const auto& get_up() { return _up; }
		const auto& get_target() { return _target; }
		auto get_right() const { return _up.cross(_front); }// TODO, use normal? get_cross_product(_front).get_normal(); }
		auto get_aspect_ratio() { return _aspect_ratio; }

		virtual void zoom(double delta) {};
		virtual void pan(double delta) {};
		virtual void pitch(const fnx::angle& angle) {};
		virtual void rotate_y(const fnx::angle& angle) {};
		virtual void rotate_z(const fnx::angle& angle) {};
		virtual void move(const fnx::vector3& delta)
		{ 
			set_position(_position + delta);
		};
		virtual void set_up(const fnx::vector3& up)
		{ 
			_up = up;
			update_view_matrix();
		}
		virtual void set_front(const fnx::vector3& front)
		{ 
			_front = front;
			update_view_matrix();
		}
		virtual void set_aspect_ratio(float ratio)
		{
			_aspect_ratio = ratio;
			update_view_matrix();
		}

	protected:
		virtual void update_view_matrix();
		fnx::matrix4x4 _projection_matrix{};
		fnx::matrix4x4 _view_matrix{};
		fnx::matrix4x4 _view_projection_matrix{};
		fnx::vector3 _position{ 0.f,0.f,0.f };
		fnx::angle _rotation{ fnx::Degree(0.f) };
		fnx::vector3 _up{ 0.f,1.f,0.f };
		fnx::vector3 _front{ 0.f,0.f,-1.f };
		fnx::vector3 _target{ 0.f,0.f,0.f };
		float _aspect_ratio{ 1.f };
	};

	using camera_handle = std::shared_ptr<camera>;

	/// @brief Camera component to render the game world objects from a given camera's perspective.
	struct camera_component
	{
		camera_handle _camera;
	};
}
