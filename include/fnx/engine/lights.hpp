#pragma once

namespace fnx
{
	struct light
	{
		enum type
		{
			directional,
			spot,
			point
		};

		fnx::vector3 position;
		fnx::vector3 direction;
		fnx::vector3 ambient;
		fnx::vector3 diffuse;
		fnx::vector3 specular; 
		float cut_off;
		float outer_cut_off;
		float constant;
		float linear;
		float quadratic;
		light::type type{ directional };		
		
		light() = default;

		/// @brief Create a directional light.
		light(const fnx::vector3& direction, const fnx::vector3& ambient, const fnx::vector3& diffuse, const fnx::vector3& specular)
		{
			this->type = fnx::light::type::directional;
			this->direction = direction;
			this->ambient = ambient;
			this->diffuse = diffuse;
			this->specular = specular;
		};

		/// @brief Create a point light.
		light(const fnx::vector3& position, const fnx::vector3& ambient, const fnx::vector3& diffuse, const fnx::vector3& specular, float constant, float linear, float quadratic)
		{
			this->type = fnx::light::type::point;
			this->position = position;
			this->ambient = ambient;
			this->diffuse = diffuse;
			this->specular = specular;
			this->constant = constant;
			this->linear = linear;
			this->quadratic = quadratic;
		};

		/// @brief Create a spot light.
		light(const fnx::vector3& position, const fnx::vector3& direction, const fnx::vector3& ambient, const fnx::vector3& diffuse, const fnx::vector3& specular, float constant, float linear, float quadratic, float cutoff, float outer_cutoff)
		{
			this->type = fnx::light::type::spot;
			this->direction = direction;
			this->position = position;
			this->ambient = ambient;
			this->diffuse = diffuse;
			this->specular = specular;
			this->constant = constant;
			this->linear = linear;
			this->quadratic = quadratic;
			this->cut_off = cutoff;
			this->outer_cut_off = outer_cutoff;
		};

		~light() = default;
	};
}