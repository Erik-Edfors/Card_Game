#pragma once
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>

#include <exception>

#include <Core/Logger/Logger.h>

#include <Core/RenderHardwareInterface/DX11/Graphics_DX11.h>
#include <Core/RenderHardwareInterface/DDSLoad.h>
#include <Core/RenderHardwareInterface/Result.h>

#define __STRINGIFY_INTERNAL(Expr) #Expr
#define STRINGIFY(Expr) __STRINGIFY_INTERNAL(Expr)

