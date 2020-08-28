#ifndef ENGINE_ENUM_H__
#define ENGINE_ENUM_H__

namespace ENGINE
{
	// ID_DYNAMIC : 실시간 업데이트가 필요한 컴포넌트
	
	enum SCENEID { SCENE_STATIC, SCENE_TITLE, SCENE_STAGE_WFW_HOME, SCENE_STAGE_SW_HOME, SCENE_STAGE_SW_SHOP, SCENE_STAGE_SW_PYRAMID, SCENE_STAGE_SW_UNDERWORLD, SCENE_STAGE_SW_UNDERWORLD_BOSS, SCENE_STAGE_SW_SLOT, SCENE_END };

	enum WINMODE { MODE_FULL, MODE_WIN };

	enum INFO { INFO_RIGHT, INFO_UP, INFO_LOOK, INFO_POS, INFO_END };
	
	enum ROTATION { ROT_X, ROT_Y, ROT_Z, ROT_END };
	namespace TEXTURE
	{
		enum TYPE { TYPE_GENERAL, TYPE_CUBE, TYPE_END };
		enum SHADER { SHADER_ALBEDO, SHADER_NORMAL, SHADER_EMISSIVE, SHADER_ROUGHNESS, SHADER_METALIC, SHADER_END };
	}
	namespace TIMER
	{
		enum TYPE { IMMEDIATE, GENERAL, DISCREET, TYPE_END };
	}

	namespace SUBSET
	{
		enum RENDER { RENDER_ON, RENDER_OFF, RENDER_ALPHA, RENDEREND };
	}

	enum RESOURCETYPE {	RESOURCE_STATIC, RESOURCE_STAGE, RESOURCE_END	};

	enum BUFFERID { BUFFER_TRICOL, BUFFER_RCTEX, BUFFER_DYNAMICRCTEX, BUFFER_TERRAINTEX, BUFFER_CUBETEX };
	
	enum TEXTURETYPE { TEX_NORMAL, TEX_CUBE, TEX_END };
	
	enum RENDERID { RENDER_PRIORITY, RENDER_NONALPHA, RENDER_ALPHA,RENDER_POSTPROCESS, RENDER_SHADOW, RENDER_OBJECT_SHADOW,RENDER_AO, RENDER_REFRACTION, RENDER_VELOCITY, RENDER_FORWARD, RENDER_PARTICLE,RENDER_UI, RENDER_SCREEN, RENDER_END };
	enum SCREEN_RENDER_ID {SCREEN_EFFECT,SCREEN_UI,SCREEN_GLOBAL_EFFECT,SCREEN_END};
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
		RenderTargetTag_Refraction_Diffuse,
		RenderTargetTag_Refraction,
		RenderTargetTag_Roughness,
		RenderTargetTag_Metallic,
		RenderTargetTag_Specular,
		RenderTargetTag_GodRayMask,
		RenderTargetTag_GodRay,
		RenderTargetTag_Blend,
		RenderTargetTag_Bloom,
		RenderTargetTag_Distortion,
		RenderTargetTag_DOF_Vertical,
		RenderTargetTag_DOF_Horizontal,
		RenderTargetTag_Bright,
		RenderTargetTag_Bright_DownSample,
		RenderTargetTag_Vertical,
		RenderTargetTag_Horizontal,
		RenderTargetTag_Alpha,
		RenderTargetTag_Velocity,
		RenderTargetTag_SSAO,
		RenderTargetTag_Smoke,
		RenderTargetTag_Old_Smoke,
		RenderTargetTag_Fog_Density,
		RenderTargetTag_Fog,
		RenderTargetTag_ForwardLight_Alpha,
		RenderTargetTag_MotionBlur,
		RenderTargetTag_Luminance1x1,
		RenderTargetTag_Luminance3x3,
		RenderTargetTag_Luminance9x9,
		RenderTargetTag_Luminance27x27,
		RenderTargetTag_Luminance81x81,
		RenderTargetTag_Luminance243x243,
		RenderTargetTag_Shadow,
		RenderTargetTag_ObjectShadow,
		RenderTargetTag_ScreenCtrl,
		RenderTargetTag_DepthProjection,
		RenderTargetTag_Final,
		RenderTargetTag_End
	};
	enum MRTTag {
		MRTTag_BackBuffer,
		MRTTag_Defferd,
		MRTTag_Refraction,
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
		// UI
		UI,
		CAPSLOTUNIT,
		SHOPMENU,
		UI_MEASUREMAP,
		UI_MAP,
		UI_COIN,
		UI_SHINE,
		UI_GETMOON,
		MINIMAPQUEST,
		MENU,
		WORLD_WATERFALL,
		WORLD_SAND,
		INVEN,
		UI_HP,
		CAMERA = 100,

		// Item
		ITEM_COIN,
		FLAG,
		SHINE,
		LIFEUPITEM,
		NPC,
		SHOPDOOR,
		DESERTMAN,

		// Map
		TERRAIN = 200,
		MAP_WFW_GROUND,
		MAP_WFW_WATER000,
		MAP_WFW_BREAKABLE,
		MAP_WFW_BUSH,
		MAP_WFW_TRAMPOLINE,
		MAP_WFW_STONEBRIGE,
		MAP_WFW_BACKGROUND,
		CAPAPPEAR_TARGETSTEP_A,
		CAPAPPEAR_TARGETSTEP_B,
		CAPAPPEAR_TARGETSTEP_C,
		MAP_WFW_UNBREAK,
		ShineTowerMeterDirt,
		PIPE,
		MAP_WFW_SHADOW,
		MAP_BLOCK_QUESTION,
		MAP_BLOCK_BRICK,

		MAP_SW_GROUND = 300,
		MAP_SW_ICE,
		MAP_SW_POISON_WATER,
		MAP_SW_WATER,
		MAP_SW_BREAKABLE,
		MAP_SW_UNBREAK,
		MAP_SW_PLANT,
		MAP_SW_MIDDLE_VIEW,
		MAP_SW_ELECTRIC_WIRE,
		MAP_SW_ONLY_RENDER,
		MAP_SW_UNBREAK_MOVE,
		MAP_BARRIOR,
		MAP_SW_UNBREAK_INVISIBLE,
		MAP_SW_QUICK_SAND,
		MAP_SW_RISE_PARTS,
		MAP_SW_WATER_FOUNTAIN,
		MAP_SW_LIFT_RAIL,
		CAP_ACCELERATOR,
		CAP_BEAMER,
		CAP_BEAMERBEAM,
		MAP_SW_2D_COVER,
		MAP_SW_CACTUS_MINI,
		CAP_TRAMPOLINE,
		FRAILBOX,
		MAP_SW_PYRAMID_UP,



		NAV = 500,

		// Terrain End
		PLAYER = 1000,
		PLAYERCAP,
		JAXI,
		// Monster
		MONSTER_TYPE = 1500,
		MONSTER_KURIBO,
		MONSTER_TREX,
		MONSTER_WANWAN,
		MONSTER_WANWANPILE,
		MONSTER_BREEDAWANWAN,
		MONSTER_BREEDA,
		MONSTER_POPN,
		MONSTER_KURIBOMINI,
		MONSTER_MUMMY,
		MONSTER_BOMBTAIL,
		MONSTER_KILLER,
		MONSTER_KILLERLUNCHER,
		MONSTER_MEGANE,
		MONSTER_BOSSKNUCKLE_BODY,
		MONSTER_BOSSKNUCKLE_KNUCKLE,
		MONSTER_END,

		MONSTEROBJ_TYPE = 2000,
		MONSTEROBJ_BREEDAWANWANHAT,
		MONSTEROBJ_BOMBTAILHAT,
		MONSTEROBJ_BOMBTAILTAIL,
		MONSTEROBJ_BOMBTAILBOMB,
		MONSTEROBJ_MEGANESUNGLASSES,
		MONSTEROBJ_BOSSKNUCKLEHEAD,
		MONSTEROBJ_BOSSKNUCKLETHROWICE,
		MONSTEROBJ_BOSSKNUCKLEGROUNDICE,
		MONSTEROBJ_END,
		//
		PLACE2D,
		MARIO2D,
		KURIBO2D,
		NOKONOKO2D,
		DOKAN2D,
		COIN2D,
		BRICK2D,
		BRICK_EMPTY2D,
		BRICK_QUESTION2D,
		BRICK_BREAK2D,
		SHINE2D,
		CORNER,
		KILLER2D,
		KILLERLAUNCHER,
		//
		OBJ_ID_EFFECT = 100000,
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
			TAG_TEXTURE1,
			TAG_TEXTURE2,
			TAG_TEXTURE3,
			TAG_TEXTURE4,
			TAG_TEXTURE5,
			TAG_TEXTURE6,
			TAG_VIBUFFER,
			TAG_SHADER,
			TAG_CONTROLLER,

			TAG_ADDEDMESH_0,
			TAG_ADDEDMESH_1,
			TAG_ADDEDMESH_2,

			TAG_WANWAN_AI_CONTROL,
			TAG_WANWAN_CAPTURE_CONTROL,
			TAG_KURIBO_AI_CONTROL,
			TAG_KURIBO_CAPTURE_CONTROL,
			TAG_TREX_AI_CONTROL,
			TAG_TREX_CAPTURE_CONTROL,
			TAG_BREEDA_AI_CONTROL,
			TAG_BREEDA_CAPTURE_CONTROL,
			TAG_BREEDAWANWAN_AI_CONTROL,
			TAG_BREEDAWANWAN_CAPTURE_CONTROL,
			TAG_KURIBOMINI_AI_CONTROL
		};
		enum ID
		{
			ID_DYNAMIC,
			ID_STATIC,
			ID_END
		};
	}
	enum AXIS { AXIS_X, AXIS_Y, AXIS_Z, AXIS_END };
	
	enum PlaneColData { Plane_On, Plane_Under, Plane_Cross };

	namespace COLLIDER
	{
		enum TYPE { TYPE_BOX, TYPE_SPHERE, TYPE_END };
		enum COL { COL_FALSE, COL_TRUE, COL_END };
	}

	enum on_Type { on_Ground, on_Air, on_TypeEnd };
	enum StateByWall { BlockedByWall, CatchByWall, StateByWallEND };

	namespace SHADER_MESH_CONSTANT
	{
		enum PassNum {Pass_Default,
			Pass_PostProcess,
			Pass_AlphaBlend,
			Pass_AlphaBlend_Black,
			Pass_Render_Water,
			Pass_Render_2D,
			Pass_Render_BackgroundWater,
			Pass_PBR,Pass_Alpha_Test,
			Pass_Make_Dark_BackGround,
			Pass_Velocity,
			Pass_Depth_Projection,
			Pass_Draw_Albedo,
			Pass_Draw_Albedo_Alpha,
			Pass_End};

		enum EffectPass {
			EffectPass_NonAlpha,
			EffectPass_OpacityBlend,
			EffectPass_AdditiveBlend,
			EffectPass_PostProcess,
			EffectPass_LUT,
			EffectPass_Alpha,
			EffectPass_Mask,
			EffectPass_Inv_Mask,
			EffectPass_Draw_Screen,
			EffectPass_Fire,
			EffectPass_End
		};
	}
	namespace SHADER_BLEND_CONSTANT
	{
		enum PassNum { Pass_Blend,Pass_Blend_Alpha,Pass_RenderBackBuffer, Pass_End };
	}
	
	namespace SHADER_POSTPROCESS_CONSTANT
	{
		enum LuminancePass { Luminance_GrayScaleDownSample, Luminance_DownSample, Luminance_End };
		enum BlurPass { Blur_Bright, Blur_DownSample, Blur_Horizontal, Blur_Vertical, Blur_End };
		enum GodRay {GodRay_ComputePass,GodRay_End};
		enum MotionBlur { MotionBlur_ComputePass,MotionBlur_End};
		enum Smoke{ Smoke_ComputePass, Smoke_End };

	}
	namespace STAGE_ID
	{
		enum MAP { MAP_WFW, MAP_SW, MAP_END };
	}

	namespace NAV_INPUT_TYPE
	{
		enum INPUT_TYPE
		{
			INPUT_DYNAMIC,
			INPUT_STATIC,
			INPUT_WORLD,
			INPUT_2D,
			INPUT_TYPE_END
		};
	}

	namespace DIMENSION2_STAGE
	{
		enum DIMENSION2_STAGE
		{
			STAGE1,
			STAGE2_0,
			STAGE2_1,
			STAGE2_2,
			STAGE2_3,
			DIMENSION2_STAGE_END
		};
	}
}

#endif // ENGINE_Enum_h__
