#pragma once

struct FrameBuffer
{
	float View[4][4];
	float Projection[4][4];
	float CameraPosition[4];
};