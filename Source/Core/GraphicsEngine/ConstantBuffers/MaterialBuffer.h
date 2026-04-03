#pragma once

struct MaterialBuffer
{
	float AlbedoTint[4];
	float EmissiveColor[3];
	float EmissionIntensity;
	float Roughness;
	float Metalness;
	unsigned int Flags;

	unsigned int _padding[1];
};