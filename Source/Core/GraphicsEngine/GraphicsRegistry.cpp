#include "GraphicsEngine.pch.h"
#include "GraphicsRegistry.h"
#include "GraphicsEngine.h"
#include <algorithm>

GraphicsRegistry::GraphicsRegistry() :
	myNextID(0)
{
}
GraphicsRegistry::~GraphicsRegistry() = default;

//void GraphicsRegistry::SetCamera(const gengine::Camera& aCamera)
//{
//	myCamera = aCamera;
//	myCameraView = myCamera.Transform.GetInverse();
//}

RenderEntryID GraphicsRegistry::AddEntry(const RenderEntry& aRenderEntry)
{
	RenderEntryID id = _genID();

	myRegistry.insert(id, aRenderEntry);

	return id;
}

RenderEntryID GraphicsRegistry::AddEntry(RenderEntry&& aRenderEntry)
{
	RenderEntryID id = _genID();

	myRegistry.insert(id, std::move(aRenderEntry));

	return id;
}

void GraphicsRegistry::UpdateEntry(RenderEntryID aID, const RenderEntry& aRenderEntry)
{
	myRegistry[aID] = aRenderEntry;
}

void GraphicsRegistry::UpdateEntry(RenderEntryID aID, RenderEntry&& aRenderEntry)
{
	myRegistry[aID] = std::move(aRenderEntry);
}

bool GraphicsRegistry::RemoveEntry(RenderEntryID aID)
{
	return myRegistry.erase(aID);
}

RenderEntry& GraphicsRegistry::GetEntry(RenderEntryID aID)
{
	return myRegistry[aID];
}

const RenderEntry& GraphicsRegistry::GetEntry(RenderEntryID aID) const
{
	return myRegistry[aID];
}

void GraphicsRegistry::Draw(GraphicsEngine* aGraphicsEngine, const gengine::Camera& aCamera, GRAPHICS_REGISTRY_DRAW_FLAG aFlags, bool aEnableBlend)
{
	std::vector<RenderEntry*> sorted;

	const bool shouldDrawStatic = (aFlags & GRAPHICS_REGISTRY_DRAW_STATIC);
	const bool shouldDrawNonStatic = (aFlags & GRAPHICS_REGISTRY_DRAW_NON_STATIC);
	const bool shouldDrawBlending = (aFlags & GRAPHICS_REGISTRY_DRAW_BLENDING);
	const bool shouldDrawNonBlending = (aFlags & GRAPHICS_REGISTRY_DRAW_NON_BLENDING);
	const bool shouldDrawAnimated = (aFlags & GRAPHICS_REGISTRY_DRAW_ANIMATED);
	const bool shouldDrawNonAnimated = (aFlags & GRAPHICS_REGISTRY_DRAW_NON_ANIMATED);

	_renderEntryComp comp = {};
	comp.CameraPosition = aCamera.Transform.Row(4).xyz();
	sorted.reserve(myRegistry.size());
	for (auto& entry : myRegistry)
	{
		const bool isStatic = (entry.GetFlags() & RENDER_ENTRY_FLAGS_STATIC) != 0;
		const bool isBlending = (entry.GetFlags() & RENDER_ENTRY_FLAGS_BLENDING) != 0;
		const bool isAnimated = (entry.GetFlags() & RENDER_ENTRY_FLAGS_ANIMATED) != 0;

		if (
			(isStatic && shouldDrawStatic || not isStatic && shouldDrawNonStatic)
			&& (isBlending && shouldDrawBlending || not isBlending && shouldDrawNonBlending)
			&& (isAnimated && shouldDrawAnimated || not isAnimated && shouldDrawNonAnimated)
			)
		{
			sorted.push_back(&entry);
		}

		//if ((entry.GetFlags() & RENDER_ENTRY_FLAGS_STATIC) != 0 && (aFlags & GRAPHICS_REGISTRY_DRAW_STATIC) == 0)
		//{
		//	continue;
		//}
		//if ((entry.GetFlags() & RENDER_ENTRY_FLAGS_STATIC) == 0 && (aFlags & GRAPHICS_REGISTRY_DRAW_NON_STATIC) == 0)
		//{
		//	continue;
		//}
		//if ((entry.GetFlags() & RENDER_ENTRY_FLAGS_BLENDING) != 0 && (aFlags & GRAPHICS_REGISTRY_DRAW_BLENDING) == 0)
		//{
		//	continue;
		//}
		//if ((entry.GetFlags() & RENDER_ENTRY_FLAGS_BLENDING) == 0 && (aFlags & GRAPHICS_REGISTRY_DRAW_NON_BLENDING) == 0)
		//{
		//	continue;
		//}
		//if ((entry.GetFlags() & RENDER_ENTRY_FLAGS_ANIMATED) != 0 && (aFlags & GRAPHICS_REGISTRY_DRAW_ANIMATED) == 0)
		//{
		//	continue;
		//}
		//if ((entry.GetFlags() & RENDER_ENTRY_FLAGS_ANIMATED) == 0 && (aFlags & GRAPHICS_REGISTRY_DRAW_NON_ANIMATED) == 0)
		//{
		//	continue;
		//}
		//sorted.push_back(&entry);
	}
	std::sort(sorted.begin(), sorted.end(), comp);

	std::vector<RenderEntry*> forward;
	std::vector<RenderEntry*> backward;
	for (auto entry : sorted)
	{
		if (entry->GetFlags() & RENDER_ENTRY_FLAGS_BLENDING)
		{
			forward.push_back(entry);
		}
		else
		{
			backward.push_back(entry);
		}
	}

	std::reverse(forward.begin(), forward.end());

	for (RenderEntry* entry : backward)
	{
		entry->Draw(aGraphicsEngine, aEnableBlend);
	}
	for (RenderEntry* entry : forward)
	{
		entry->Draw(aGraphicsEngine, aEnableBlend);
	}
}

float GraphicsRegistry::_distanceToCamera(const RenderEntry* aRenderEntry, const CommonUtilities::Vector3<float>& aCameraPosition)
{
	const CommonUtilities::Matrix4x4f& transform = aRenderEntry->GetTransform();
	const CommonUtilities::Sphere<float>& sphere = aRenderEntry->GetMesh().EncompassingSphere;
	const float greatestScale = std::sqrtf(
		std::max(
			std::max(
				transform.Row(1).LengthSqr(),
				transform.Row(2).LengthSqr()
			),
			transform.Row(3).LengthSqr()
		)
	);

	CommonUtilities::Sphere<float> globalSphere(
		(sphere.Center.xyz1() * transform).xyz(),
		sphere.Radius * greatestScale
	);

	return (globalSphere.Center - aCameraPosition).Length() - globalSphere.Radius;

	//const Mesh& mesh = aRenderEntry->GetMesh();

	//const CommonUtilities::Matrix4x4<float>& transform = aRenderEntry->GetTransform();
	//const CommonUtilities::Sphere<float> sphere = mesh.EncompassingSphere;

	//float maxScaleAxisLength = (std::max)(
	//	transform.Row(1).LengthSqr(),
	//	(std::max)(
	//		transform.Row(2).LengthSqr(),
	//		transform.Row(3).LengthSqr()
	//		)
	//	);
	//maxScaleAxisLength = sqrtf(maxScaleAxisLength);

	//CommonUtilities::Sphere<float> transformedSphere(
	//	(sphere.Center.xyz1() * transform).xyz(),
	//	sphere.Radius * maxScaleAxisLength
	//);

	//return (transformedSphere.Center - aCameraPosition).Length() - transformedSphere.Radius;
}

RenderEntryID GraphicsRegistry::_genID() noexcept
{
	return myNextID++;
}

bool GraphicsRegistry::_renderEntryComp::operator()(const RenderEntry* aLHS, const RenderEntry* aRHS) const
{
	return _distanceToCamera(aLHS, CameraPosition) < _distanceToCamera(aRHS, CameraPosition);
}
