/* This file is part of the Spring engine (GPL v2 or later), see LICENSE.html */

#ifndef SHADOW_HANDLER_H
#define SHADOW_HANDLER_H

#include <array>

#include "Rendering/GL/FBO.h"
#include "System/float4.h"
#include "System/Matrix44f.h"

namespace Shader {
	struct IProgramObject;
}

class CCamera;
class CShadowHandler
{
public:
	CShadowHandler(): shadowMapFBO(true) {}

	void Init();
	void Kill();
	void Reload(const char* argv);
	void Update();

	void SetupShadowTexSampler(uint32_t texUnit, bool enable = false) const;
	void SetupShadowTexSamplerRaw() const;
	void ResetShadowTexSampler(uint32_t texUnit, bool disable = false) const;
	void ResetShadowTexSamplerRaw() const;
	void CreateShadows();

	enum ShadowGenerationBits {
		SHADOWGEN_BIT_NONE  = 0,
		SHADOWGEN_BIT_MAP   = 2,
		SHADOWGEN_BIT_MODEL = 4,
		SHADOWGEN_BIT_PROJ  = 8,
		SHADOWGEN_BIT_TREE  = 16,
		SHADOWGEN_BIT_COLOR = 128,
	};
	enum ShadowProjectionMode {
		SHADOWPROMODE_MAP_CENTER = 0, // use center of map-geometry as projection target (constant res.)
		SHADOWPROMODE_CAM_CENTER = 1, // use center of camera-frustum as projection target (variable res.)
		SHADOWPROMODE_MIX_CAMMAP = 2, // use whichever mode maximizes resolution this frame
	};
	enum ShadowMapSizes {
		MIN_SHADOWMAP_SIZE =   512,
		DEF_SHADOWMAP_SIZE =  2048,
		MAX_SHADOWMAP_SIZE = 16384,
	};

	enum ShadowGenProgram {
		SHADOWGEN_PROGRAM_MODEL      = 0,
		SHADOWGEN_PROGRAM_MODEL_GL4  = 1,
		SHADOWGEN_PROGRAM_MAP        = 2,
		SHADOWGEN_PROGRAM_TREE_NEAR  = 3,
		SHADOWGEN_PROGRAM_TREE_FAR   = 4,
		SHADOWGEN_PROGRAM_PROJECTILE = 5,
		SHADOWGEN_PROGRAM_LAST       = 6,
	};

	enum ShadowMatrixType {
		SHADOWMAT_TYPE_CULLING = 0,
		SHADOWMAT_TYPE_DRAWING = 1,
	};

	Shader::IProgramObject* GetShadowGenProg(ShadowGenProgram p) {
		return shadowGenProgs[p];
	}

	const CMatrix44f& GetShadowMatrix   (uint32_t idx = SHADOWMAT_TYPE_DRAWING) const { return  viewMatrix[idx];      }
	const      float* GetShadowMatrixRaw(uint32_t idx = SHADOWMAT_TYPE_DRAWING) const { return &viewMatrix[idx].m[0]; }

	const CMatrix44f& GetShadowViewMatrix(uint32_t idx = SHADOWMAT_TYPE_DRAWING) const { return  viewMatrix[idx]; }
	const CMatrix44f& GetShadowProjMatrix(uint32_t idx = SHADOWMAT_TYPE_DRAWING) const { return  projMatrix[idx]; }
	const      float* GetShadowViewMatrixRaw(uint32_t idx = SHADOWMAT_TYPE_DRAWING) const { return &viewMatrix[idx].m[0]; }
	const      float* GetShadowProjMatrixRaw(uint32_t idx = SHADOWMAT_TYPE_DRAWING) const { return &projMatrix[idx].m[0]; }

	uint32_t GetShadowTextureID() const { return shadowDepthTexture; }
	uint32_t GetColorTextureID() const { return shadowColorTexture; }

	bool GetAttachColor() const { return attachColor; }

	static bool ShadowsInitialized() { return firstInit; }
	static bool ShadowsSupported() { return shadowsSupported; }

	bool ShadowsLoaded() const { return shadowsLoaded; }
	bool InShadowPass() const { return inShadowPass; }

	static const float4& GetShadowParams() { return shadowTexProjCenter; }
private:
	void FreeFBOAndTextures();
	bool InitFBOAndTextures();

	void DrawShadowPasses();
	void LoadProjectionMatrix(const CCamera* shadowCam);
	void LoadShadowGenShaders();

	void SetShadowMatrix(CCamera* playerCam, CCamera* shadowCam);
	void SetShadowCamera(CCamera* shadowCam);

	float4 GetShadowProjectionScales(CCamera*, const CMatrix44f&);
	float3 CalcShadowProjectionPos(CCamera*, float3*);

	float GetOrthoProjectedMapRadius(const float3&, float3&);
	float GetOrthoProjectedFrustumRadius(CCamera*, const CMatrix44f&, float3&);

public:
	int shadowConfig;
	int shadowMapSize;
	int shadowGenBits;
	int shadowProMode;

private:
	uint32_t shadowDepthTexture;
	uint32_t shadowColorTexture;

	bool shadowsLoaded = false;
	bool inShadowPass = false;

	bool attachColor = false;

	inline static bool firstInit = true;
	inline static bool shadowsSupported = false;

	// these project geometry into light-space
	// to write the (FBO) depth-buffer texture
	std::array<Shader::IProgramObject*, SHADOWGEN_PROGRAM_LAST> shadowGenProgs;

	float3 projMidPos[2 + 1];
	float3 sunProjDir;

	float4 shadowProjScales;
	/// frustum bounding-rectangle corners; x1, x2, y1, y2
	float4 shadowProjMinMax;
	/// xmid, ymid, p17, p18

	// culling and drawing versions of both matrices
	CMatrix44f projMatrix[2];
	CMatrix44f viewMatrix[2];

	FBO shadowMapFBO;

	static inline float4 shadowTexProjCenter = {
		0.5f,
		0.5f,
		FLT_MAX,
		1.0f
	};
};

extern CShadowHandler shadowHandler;

#endif /* SHADOW_HANDLER_H */
