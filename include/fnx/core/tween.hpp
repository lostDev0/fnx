#pragma once

namespace fnx
{
	template<typename T>
	/// @brief Creates a gradient of values that allows weighting certain values more than others. 
	class tween
	{
	public:
		tween() = default;
		
		template<typename... TArgs>
		tween(TArgs... args)
			: _vectors{args...}
		{
		}
		~tween() = default;

		void add(const T& val)
		{
			_vectors.emplace_back(val);
		}

		virtual T get(double t) const
		{
			// find out where in the tween we are
			// find out point between the two scales
			std::size_t total = _vectors.size();
			t = fnx::minimum(t, 1.0);
			T ret_val{0};	// if used as a scale tween, you need to add 1 scale

			if (total > 1)
			{
				double color_dist = 1.0 / static_cast<double>(total - 1);
				std::size_t left_idx = static_cast<int>(t / color_dist);
				std::size_t right_idx = left_idx;
				double left_bound = left_idx * color_dist;
				double right_bound = left_bound;
				auto left = _vectors[left_idx];
				auto right = left;

				if (left_idx < total - 1)
				{
					right_idx = left_idx + 1;
					right_bound = right_idx * color_dist;
					right = _vectors[right_idx];
				}

				t = (t - left_bound) / color_dist;
				ret_val = static_cast<T>((right - left) * t + left);
			}
			else if (total == 1)
			{
				ret_val = _vectors[0];
			}

			return ret_val;
		}

		T get_pivot(unsigned int idx) const
		{
			if (idx < _vectors.size())
			{
				return _vectors[idx];
			}

			return T();
		}

		const auto& get_values() const
		{
			return _vectors;
		}
	private:
		std::vector<T> _vectors;
	};
}
