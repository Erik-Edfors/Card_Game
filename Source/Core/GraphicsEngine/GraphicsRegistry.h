#pragma once
#include "GraphicsEngineDefs.h"
#include "Objects/Camera.h"
#include "Objects/RenderEntry.h"
#include <unordered_map>
#include <Core/CommonUtilities/Registry.hpp>

typedef unsigned int GRAPHICS_REGISTRY_DRAW_FLAG;
enum
{
	GRAPHICS_REGISTRY_DRAW_NONE = 0,
	GRAPHICS_REGISTRY_DRAW_STATIC = 1,
	GRAPHICS_REGISTRY_DRAW_NON_STATIC = 2,
	GRAPHICS_REGISTRY_DRAW_BLENDING = 4,
	GRAPHICS_REGISTRY_DRAW_NON_BLENDING = 8,
	GRAPHICS_REGISTRY_DRAW_ANIMATED = 16,
	GRAPHICS_REGISTRY_DRAW_NON_ANIMATED = 32,
	GRAPHICS_REGISTRY_DRAW_ALL = GRAPHICS_REGISTRY_DRAW_STATIC | GRAPHICS_REGISTRY_DRAW_NON_STATIC | GRAPHICS_REGISTRY_DRAW_BLENDING | GRAPHICS_REGISTRY_DRAW_NON_BLENDING | GRAPHICS_REGISTRY_DRAW_ANIMATED | GRAPHICS_REGISTRY_DRAW_NON_ANIMATED
};

class GraphicsEngine;

class GraphicsRegistry
{
public:
	GraphicsRegistry();
	~GraphicsRegistry();

	//void SetCamera(const gengine::Camera& aCamera);

	RenderEntryID AddEntry(const RenderEntry& aRenderEntry);
	RenderEntryID AddEntry(RenderEntry&& aRenderEntry);
	void UpdateEntry(RenderEntryID aID, const RenderEntry& aRenderEntry);
	void UpdateEntry(RenderEntryID aID, RenderEntry&& aRenderEntry);
	bool RemoveEntry(RenderEntryID aID);

	RenderEntry& GetEntry(RenderEntryID aID);
	const RenderEntry& GetEntry(RenderEntryID aID) const;

	void Draw(GraphicsEngine* aGraphicsEngine, const gengine::Camera& aCamera, GRAPHICS_REGISTRY_DRAW_FLAG aFlags = GRAPHICS_REGISTRY_DRAW_ALL, bool aEnableBlend = true);

private:
	struct _renderEntryComp
	{
		CommonUtilities::Vector3<float> CameraPosition;
		bool operator()(const RenderEntry* aLHS, const RenderEntry* aRHS) const;
	};

	static float _distanceToCamera(const RenderEntry* aRenderEntry, const CommonUtilities::Vector3<float>& aCameraPosition);

	RenderEntryID _genID() noexcept;

	gengine::Camera myCamera;
	CommonUtilities::Matrix4x4<float> myCameraView;

	RenderEntryID myNextID;

	CommonUtilities::Registry<RenderEntryID, RenderEntry> myRegistry;
};