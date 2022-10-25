#pragma once

namespace fnx
{
	/// @brief Format for data storage in opengl textures.
	/// @note Mimics OpenGL constants in order to obfuscate opengl complexity.
	enum format : int
	{
		DepthComponent = 0x1902,
		DepthComponent16 = 0x81A5,
		DepthComponent24 = 0x81A6,
		DepthComponent32 = 0x81A7,
		Red = 0x1903,
		Alpha = 0x1906,
		RGB = 0x1907,
		RGBA = 0x1908
	};

	/// @brief Attachment for data storage in opengl textures.
	/// @note Mimics OpenGL constants in order to obfuscate opengl complexity.
	enum attachment : int
	{
		None = 0,
		Color_0 = 0x8CE0,
		Color_1 = 0x8CE1,
		Color_2 = 0x8CE2,
		Color_3 = 0x8CE3,
		Color_4 = 0x8CE4,
		Color_5 = 0x8CE5,
		Color_6 = 0x8CE6,
		Color_7 = 0x8CE7,
		Color_8 = 0x8CE8,
		Color_9 = 0x8CE9,
		Color_10 = 0x8CEA,
		Color_11 = 0x8CEB,
		Color_12 = 0x8CEC,
		Color_13 = 0x8CED,
		Color_14 = 0x8CEE,
		Color_15 = 0x8CEF,
		Depth_Attachment = 0x8D00
	};

	/// @brief Filter for data storage in opengl textures.
	/// @note Mimics OpenGL constants in order to obfuscate opengl complexity.
	enum filter : int
	{
		Nearest = 0x2600,
		Linear = 0x2601
	};

	constexpr auto UNIFORM_CAMERA_VIEW_MATRIX = "u_CameraViewMatrix";
	constexpr auto UNIFORM_CAMERA_PROJECTION_MATRIX = "u_CameraProjectionMatrix";
	constexpr auto UNIFORM_CAMERA_PROJECTION_VIEW_MATRIX = "u_CameraProjectionViewMatrix";
	constexpr auto UNIFORM_CAMERA_POSITION = "u_CameraPosition";
	constexpr auto UNIFORM_MODEL_VIEW_MATRIX = "u_ModelViewMatrix";
	constexpr auto UNIFORM_MODEL_TRANSPOSE_VIEW_MATRIX = "u_ModelTransposeViewMatrix";
	constexpr auto UNIFORM_LIGHT_SPACE_MATRIX = "u_LightSpaceMatrix";
	constexpr auto UNIFORM_TEXTURE_SAMPLER = "u_TextureSampler";
	constexpr auto UNIFORM_TEXTURE_ATLAS_MAP = "u_TextureAtlasMap";
	constexpr auto UNIFORM_TEXTURE_ATLAS_OFFSET = "u_TextureAtlasOffset";
	constexpr auto UNIFORM_COLOR = "u_Color";
	constexpr auto UNIFORM_BACKGROUND_COLOR = "u_BackgroundColor";
	constexpr auto UNIFORM_BORDER_COLOR = "u_BorderColor";
	constexpr auto UNIFORM_GAMMA = "u_Gamma";
	constexpr auto UNIFORM_FOG_COLOR = "u_FogColor";
	constexpr auto UNIFORM_FOG_DENSITY = "u_FogDensity";
	constexpr auto UNIFORM_FOG_GRADIENT = "u_FogGradient";
	constexpr auto UNIFORM_DEPTH_MAP = "u_DepthMap";
	constexpr auto UNIFORM_NEAR_PLANE = "u_NearPlane";
	constexpr auto UNIFORM_FAR_PLANE = "u_FarPlane";
	constexpr auto UNIFORM_SHADOW_MAP = "u_ShadowMap";

	constexpr auto UNIFORM_SIZE = "u_Size";
	constexpr auto UNIFORM_WIDTH = "u_Width";
	constexpr auto UNIFORM_EDGE = "u_Edge";
	constexpr auto UNIFORM_BORDER_WIDTH = "u_BorderWidth";
	constexpr auto UNIFORM_BORDER_EDGE = "u_BorderEdge";
	constexpr auto UNIFORM_BORDER_OFFSET = "u_BorderOffset";
	constexpr auto UNIFORM_CENTER = "u_Center";
	constexpr auto UNIFORM_RADIUS = "u_Radius";
	constexpr auto UNIFORM_RESOLUTION = "u_Resolution";
	constexpr auto UNIFORM_OUTLINE_THICKNESS = "u_OutlineThickness";
	constexpr auto UNIFORM_OUTLINE_COLOR = "u_OutlineColor";
	constexpr auto UNIFORM_NUM_GRADIENT = "u_NumGradient";
	constexpr auto UNIFORM_GRADIENT_DIRECTION = "u_GradientDirection";
	constexpr auto UNIFORM_GRADIENT = "u_Gradient";
	constexpr auto UNIFORM_OVERLAY = "u_Overlay";
	constexpr auto UNIFORM_PROGRESS = "u_Progress";
	constexpr auto UNIFORM_DIRECTION = "u_Direction";

	constexpr auto UNIFORM_MATERIAL_KA = "u_MaterialKa";
	constexpr auto UNIFORM_MATERIAL_KD = "u_MaterialKd";
	constexpr auto UNIFORM_MATERIAL_KS = "u_MaterialKs";
	constexpr auto UNIFORM_MATERIAL_KE = "u_MaterialKe";
	constexpr auto UNIFORM_MATERIAL_NS = "u_MaterialNs";
	constexpr auto UNIFORM_MATERIAL_NI = "u_MaterialNi";
	constexpr auto UNIFORM_MATERIAL_D = "u_MaterialD";
	constexpr auto UNIFORM_MATERIAL_ILLUM = "u_MaterialIllum";

	constexpr auto UNIFORM_MATERIAL_MAP_KA = "u_MaterialMapKa";
	constexpr auto UNIFORM_MATERIAL_MAP_KD = "u_MaterialMapKd";
	constexpr auto UNIFORM_MATERIAL_MAP_KS = "u_MaterialMapKs";
	constexpr auto UNIFORM_MATERIAL_MAP_KE = "u_MaterialMapKe";
	constexpr auto UNIFORM_MATERIAL_MAP_NS = "u_MaterialMapNs";
	constexpr auto UNIFORM_MATERIAL_MAP_NI = "u_MaterialMapNi";
	constexpr auto UNIFORM_MATERIAL_MAP_D = "u_MaterialMapD";

	constexpr auto UNIFORM_LIGHTS_NUM_SPOT = "u_NumSpotLights";
	constexpr auto UNIFORM_LIGHTS_NUM_POINT = "u_NumPointLights";
	constexpr auto UNIFORM_LIGHTS_SPOT_ARRAY= "u_SpotLights";
	constexpr auto UNIFORM_LIGHTS_POINT_ARRAY = "u_PointLights";
	constexpr auto UNIFORM_LIGHT_DIRECTIONAL= "u_DirectionalLight";
	constexpr auto UNIFORM_LIGHT_DIRECTION = "direction";
	constexpr auto UNIFORM_LIGHT_AMBIENT= "ka";
	constexpr auto UNIFORM_LIGHT_DIFFUSE = "kd";
	constexpr auto UNIFORM_LIGHT_SPECULAR = "ks";
	constexpr auto UNIFORM_LIGHT_POSITION = "position";
	constexpr auto UNIFORM_LIGHT_CONSTANT = "constant";
	constexpr auto UNIFORM_LIGHT_LINEAR = "linear";
	constexpr auto UNIFORM_LIGHT_QUADRATIC = "quadratic";
	constexpr auto UNIFORM_LIGHT_CUTOFF = "cutOff";
	constexpr auto UNIFORM_LIGHT_OUTER_CUTOFF = "outerCutOff";

	constexpr auto UNIFORM_SKYBOX_MAP = "u_SkyboxMap";
	constexpr auto UNIFORM_SKYBOX_MAP_NIGHT = "u_SkyboxMapNight";
	constexpr auto UNIFORM_SKYBOX_BLEND_FACTOR = "u_SkyboxBlendFactor";

	constexpr auto SHADER_DEFAULT = "default.shader";
	constexpr auto SHADER_SIMPLE = "simple.shader";
	constexpr auto SHADER_MATERIAL = "material.shader";
	constexpr auto SHADER_MATERIAL_WITH_SHADOW = "material_shadow.shader";
	constexpr auto SHADER_MATERIAL_WITH_BLOOM = "material_bloom.shader";
	constexpr auto SHADER_TERRAIN = "terrain.shader";
	constexpr auto SHADER_SHADOWS = "shadows.shader";
	constexpr auto SHADER_SKYBOX = "skybox.shader";

	constexpr auto DEFAULT_NEAR_PLANE = .1f;
	constexpr auto DEFAULT_FAR_PLANE = 50.f;

	constexpr auto SKYBOX_MODEL_ASSET_NAME = "skybox_model";
}
