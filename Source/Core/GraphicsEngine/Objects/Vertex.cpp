#include "GraphicsEngine.pch.h"
#include "Vertex.h"

Vertex::Vertex(float x, float y, float z, float w, float nx, float ny, float nz, float nw, float tx, float ty, float tz, float tw, int b1, int b2, int b3, int b4, float w1, float w2, float w3, float w4, float u, float v)
{
	Position[0] = x;
	Position[1] = y;
	Position[2] = z;
	Position[3] = w;

	Normal[0] = nx;
	Normal[1] = ny;
	Normal[2] = nz;
	Normal[3] = nw;

	Tangent[0] = tx;
	Tangent[1] = ty;
	Tangent[2] = tz;
	Tangent[3] = tw;

	BoneIDs[0] = b1;
	BoneIDs[1] = b2;
	BoneIDs[2] = b3;
	BoneIDs[3] = b4;

	SkinWeights[0] = w1;
	SkinWeights[1] = w2;
	SkinWeights[2] = w3;
	SkinWeights[3] = w4;

	UV[0] = u;
	UV[1] = v;
}
