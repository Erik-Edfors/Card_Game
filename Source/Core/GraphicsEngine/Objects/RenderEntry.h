#pragma once
#include "../GraphicsEngineDefs.h"
#include "Objects/Mesh.h"
#include "Objects/Material.h"
#include "Objects/Skeleton.h"
#include "Objects/Animation.h"

class GraphicsEngine;

class RenderEntry
{
public:
	RenderEntry();
	~RenderEntry();

	void SetID(unsigned int aID) noexcept;
	unsigned int GetID() const noexcept;

	void SetFlags(RENDER_ENTRY_FLAGS aFlags) noexcept;
	RENDER_ENTRY_FLAGS GetFlags() const noexcept;

	void SetTransform(const CommonUtilities::Matrix4x4<float>& aTransform);
	const CommonUtilities::Matrix4x4<float>& GetTransform() const;

	void SetMesh(const Mesh& aMesh) noexcept;
	void SetMesh(Mesh&& aMesh) noexcept;
	const Mesh& GetMesh() const noexcept;
	Mesh& GetMesh() noexcept;

	void SetMaterial(const Material& aMaterial) noexcept;
	void SetMaterial(Material&& aMaterial) noexcept;
	const Material& GetMaterial() const noexcept;
	Material& GetMaterial() noexcept;

	void SetJoints(const CommonUtilities::Matrix4x4f* aJoints);
	const CommonUtilities::Matrix4x4f* GetJoints() const;

	void Draw(GraphicsEngine* aGraphicsEngine, bool aEnableBlend = true);

private:
	unsigned int myID;
	CommonUtilities::Matrix4x4<float> myTransform;
	const CommonUtilities::Matrix4x4f* myJoints;
	Mesh myMesh;
	Material myMaterial;
	RENDER_ENTRY_FLAGS myFlags;
};