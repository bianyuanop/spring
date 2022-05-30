/* This file is part of the Spring engine (GPL v2 or later), see LICENSE.html */

#ifndef SMOOTH_HEIGHT_MESH_H
#define SMOOTH_HEIGHT_MESH_H

#include <memory_resource>
#include <queue>
#include <vector>

#include "System/type2.h"

class CGround;

struct DamageMesh {
	std::vector<bool> damageMap;
	std::queue<int> damageQueue[2];
	std::queue<int> horizontalBlurQueue;
	std::queue<int> verticalBlurQueue;
	int width = 0;
	int height = 0;
	int queueReleaseOnFrame = 0;
	bool activeBuffer = 0;
};

/**
 * Provides a GetHeight(x, y) of its own that smooths the mesh.
 */
class SmoothHeightMesh
{
public:
	void Init(int2 max, int res, int smoothRad);
	void Kill();

	float GetHeight(float x, float y);
	float GetHeightAboveWater(float x, float y);
	float SetHeight(int index, float h);
	float AddHeight(int index, float h);
	float SetMaxHeight(int index, float h);

	int GetMaxX() const { return maxx; }
	int GetMaxY() const { return maxy; }
	float GetFMaxX() const { return fmaxx; }
	float GetFMaxY() const { return fmaxy; }
	float GetResolution() const { return fresolution; }

	const float* GetMeshData() const { return &mesh[0]; }
	const float* GetOriginalMeshData() const { return &origMesh[0]; }

	void UpdateSmoothMesh();

	void OnMapDamage(int x1, int z1, int x2, int z2);

private:
	void MakeSmoothMesh();

	int maxx = 0;
	int maxy = 0;
	float fmaxx = 0.0f;
	float fmaxy = 0.0f;
	float fresolution = 0.f;
	int resolution = 0;
	int smoothRadius = 0;

	std::vector<float> maximaMesh;
	std::vector<float> mesh;
	std::vector<float> tempMesh;
	std::vector<float> origMesh;

	std::vector<float> colsMaxima;
	std::vector<int> maximaRows;

	DamageMesh meshDamageTrack;

	void UpdateMapMaximaGrid();
	void BuildNewMapMaximaGrid();
};

extern SmoothHeightMesh smoothGround;

#endif
