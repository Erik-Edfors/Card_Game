#pragma once

struct ObjectBuffer
{
	float Transform[4][4];
	unsigned int ID;
	unsigned int _padding[3];
};