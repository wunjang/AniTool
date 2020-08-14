#ifndef ENGINE_ENUM_H__
#define ENGINE_ENUM_H__

namespace ENGINE
{
	// ID_DYNAMIC : 실시간 업데이트가 필요한 컴포넌트
	

	enum WINMODE { MODE_FULL, MODE_WIN };

	enum INFO { INFO_RIGHT, INFO_UP, INFO_LOOK, INFO_POS, INFO_END };
	
	enum ROTATION { ROT_X, ROT_Y, ROT_Z, ROT_END };

	enum RESOURCETYPE {	RESOURCE_STATIC, RESOURCE_STAGE, RESOURCE_END	};

	enum BUFFERID { BUFFER_TRICOL, BUFFER_RCTEX, BUFFER_DYNAMICRCTEX, BUFFER_TERRAINTEX, BUFFER_CUBETEX };
	
	enum TEXTURETYPE { TEX_NORMAL, TEX_CUBE, TEX_END };
	
	enum RENDERID { RENDER_PRIORITY, RENDER_NONALPHA, RENDER_ALPHA,RENDER_REMAIN,RENDER_AO, RENDER_UI, RENDER_END };
	
	
	enum MESHTYPE { TYPE_STATIC, TYPE_DYNAMIC, TYPE_NAVI, TYPE_END };

	enum MOUSEKEYSTATE { DIM_LB, DIM_RB, DIM_MB, DIM_END };

	enum MOUSEMOVESTATE { DIMS_X, DIMS_Y, DIMS_Z, DIMS_END };

	enum USEAGE { DISABLE, ENABLE };

	enum GAME_STATE
	{
		ERROR_EXIT = -1,
		NO_EVENT,
		OBJ_POOL,
		DEAD_OBJ,
		CHANGE_SCENE,
		GAME_STATE_END
	};

	enum LAYER_TYPE
	{
		LAYER_ENVIRONMENT,
		LAYER_PLAYER,
		LAYER_GAMEOBJECT,
		LAYER_UI,
		LAYER_END
	};
	enum DIRECT_MESH
	{
		DIRECT_MESH_BOX,
		DIRECT_MESH_SPHERE,
		DIRECT_MESH_END
	};

	enum HURT_TYPE
	{
		HURT_BASIC,
		HURT_RISED,
		HURT_HIGHRISED,
		HURT_STURN,
		HURT_DOWN,
		HURT_KNOCK_BACK,
		HURT_TYPE_END
	};

	enum UNIT_STATE
	{
		UNIT_ONGROUND,
		UNIT_ONAERIAL,
		UNIT_END
	};

	enum OBJTYPE
	{
		OBJTYPE_PLAYER, OBJTYPE_SOLIDER_SPEAR, OBJTYPE_SOLDIER_BOW, OBJTYPE_SOLDIER_SHIELD, OBJTYPE_BOSS, OBJTYPE_END
	};

	//렌더타겟
	enum RenderTargetTag {
		RenderTargetTag_Albedo,
		RenderTargetTag_Normal,
		RenderTargetTag_Shade,
		RenderTargetTag_Depth,
		RenderTargetTag_Roughness,
		RenderTargetTag_Metallic,
		RenderTargetTag_Specular,
		RenderTargetTag_Emissive,
		RenderTargetTag_Blend,
		RenderTargetTag_Bloom,
		RenderTargetTag_Distortion,
		RenderTargetTag_Bright,
		RenderTargetTag_Bright_DownSample,
		RenderTargetTag_Vertical,
		RenderTargetTag_Horizontal,
		RenderTargetTag_Alpha,
		RenderTargetTag_Velocity,
		RenderTargetTag_SSAO,
		RenderTargetTag_Luminance1x1,
		RenderTargetTag_Luminance3x3,
		RenderTargetTag_Luminance9x9,
		RenderTargetTag_Luminance27x27,
		RenderTargetTag_Luminance81x81,
		RenderTargetTag_Luminance243x243,
		RenderTargetTag_Final,
		RenderTargetTag_End
	};
	enum MRTTag {
		MRTTag_BackBuffer,
		MRTTag_Defferd,
		MRTTag_PBR,
		MRTTag_POSTPROCESS,
		MRTTag_Light,
		MRTTag_SSAO,
		MRTTag_Alpha,
		MRTTag_Blend,
		MRTTag_Velocity,
		MRTTag_Luminance,
		MRTTag_Bright,
		MRTTag_Blur,
		MRTTag_Final,
		MRTTag_END
	};

	enum OBJ_ID
	{
		UI,
		UI_MAP,
		UI_COIN,
		UI_HP,
		ITEM_COIN,
		FLAG,
		CAMERA = 100,
		//
		TERRAIN = 200,
		MAP_WFW_GROUND,
		MAP_WFW_WATER000,
		MAP_WFW_BREAKABLE,
		MAP_WFW_BUSH,
		MAP_WFW_TRAMPOLINE,
		MAP_WFW_STONEBRIGE,
		MAP_WFW_BACKGROUND,
		NAV = 500,
		// Terrain End
		PLAYER = 1000,
		PLAYERCAP,
		MONSTER_TYPE = 1500,
		MONSTER_KURIBO,
		MONSTER_TREX,
		MONSTER_WANWAN,
		MONSTER_BREEDAWANWAN,
		MONSTER_BREEDA,
		//
		MARIO2D,
		KURIBO2D,
		NOKONOKO2D,
		//
		OBJ_ID_END
	};

	namespace COMPONENT
	{
		enum TAG
		{
			TAG_TRANSFORM,
			TAG_MESH,
			TAG_NAVIMESH,
			TAG_RENDERER,
			TAG_TEXTURE,
			TAG_VIBUFFER,
			TAG_SHADER,
			TAG_CONTROLLER,

			TAG_WANWAN_AI_CONTROL,
			TAG_WANWAN_CAPTURE_CONTROL,
			TAG_KURIBO_AI_CONTROL,
			TAG_KURIBO_CAPTURE_CONTROL,
			TAG_TREX_AI_CONTROL,
			TAG_TREX_CAPTURE_CONTROL,
			TAG_BREEDA_AI_CONTROL,
			TAG_BREEDA_CAPTURE_CONTROL,
			TAG_BREEDAWANWAN_AI_CONTROL,
			TAG_BREEDAWANWAN_CAPTURE_CONTROL
		};
		enum ID
		{
			ID_DYNAMIC,
			ID_STATIC,
			ID_END
		};
	}
	enum AXIS { AXIS_X, AXIS_Y, AXIS_Z, AXIS_END };
	

	namespace COLLIDER
	{
		enum TYPE { TYPE_BOX, TYPE_SPHERE, TYPE_END };
		enum COL { COL_FALSE, COL_TRUE, COL_END };
	}

	enum on_Type { on_Ground, on_Air, on_TypeEnd };
	enum StateByWall { BlockedByWall, CatchByWall, StateByWallEND };

	namespace SHADER_MESH_CONSTANT
	{
		enum PassNum {Pass_Default,Pass_PostProcess,Pass_AlphaBlend,Pass_AlphaBlend_Black,Pass_Render_Water,Pass_End};
	}
	namespace SHADER_BLEND_CONSTANT
	{
		enum PassNum { Pass_Blend,Pass_Blend_Alpha,Pass_RenderBackBuffer, Pass_End };
	}
}

#endif // ENGINE_Enum_h__
