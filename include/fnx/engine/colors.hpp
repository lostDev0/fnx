#pragma once

namespace fnx
{
	namespace colors
	{
		using rgb = fnx::vector3;
		using rgba = fnx::vector4;
		static rgb black{ 0.f,0.f,0.f };
		static rgb white{ 1.f,1.f,1.f };
		static rgb red{ 1.f,0.f,0.f };
		static rgb green{ 0.f,1.f,0.f };
		static rgb blue{ 0.f,0.f,1.f };
		static rgb cyan{ 0.f,1.f,1.f };
		static rgb aquamarine{ 0.498f,1.f,0.831f };
		static rgb grey_0{ 0.1f,0.1f,0.1f };
		static rgb grey_1{ 0.2f,0.2f,0.2f };
		static rgb grey_2{ 0.3f,0.3f,0.3f };
		static rgb grey_3{ 0.4f,0.4f,0.4f };
		static rgb grey_4{ 0.5f,0.5f,0.5f };
		static rgb purple{ 0.5f,0.f,0.5f };
		static rgb lime{ 0.0f,1.f,0.0f };
		static rgb pink{ 1.0f,.41f,0.706f };
		static rgb transparent{ 0.f,0.f,0.f };
		static rgb teal{24.f/255.f, 188.f/255.f, 172.f/255.f};
	}
}
