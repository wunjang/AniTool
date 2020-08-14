#ifndef SHADERTABLE_H__
#define SHADERTABLE_H__

namespace SHADER_TAG
{
	class Shader_Base_Public
	{
		public:
			const D3DXHANDLE BaseTexture = "g_BaseTexture";

			const D3DXHANDLE matView = "g_matView";
			const D3DXHANDLE matProj = "g_matProj";
			const D3DXHANDLE matWorld = "g_matWorld";

			const D3DXHANDLE matOldWorld = "g_matOldWorld";
			const D3DXHANDLE matOldView = "g_matOldView";
			const D3DXHANDLE matOldProj = "g_matOldProj";
	};

	//Public
	class Shader_Mesh_Public : public Shader_Base_Public
	{
	public:
		const D3DXHANDLE NormalTexture = "g_NormalTexture";
		const D3DXHANDLE vec4RimLightColor = "g_RimLightColor";
	};


	////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////
	//Default
	class Shader_Mesh_Default : public Shader_Mesh_Public
	{
	public:
		const D3DXHANDLE Technique = "Default_Device";
	};

	////////////////////////////////////////////////////////////////////////////////////
	//Default
	class Shader_Mesh_HardWare_Skin: public Shader_Mesh_Public
	{
	public:
		const D3DXHANDLE Technique = "Hardware_Skin";

		const D3DXHANDLE matPaletteArray = "MatrixPalette";

		const D3DXHANDLE iNumBoneInfluences = "NumBoneInfluences";
	};




}



#endif