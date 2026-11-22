#pragma once
#include "RenderHardwareInterface.pch.h"
#include "../imgui.h"

#include "Window_DX11Impl.h"
#include "Device_DX11Impl.h"

#include <Core/imgui/imgui.h>
#include <Core/imgui/imgui_impl_dx11.h>
#include <Core/imgui/imgui_impl_win32.h>

#define FILE_CATEGORY "ImGui_DX11Impl.cpp"

namespace graphics
{
	bool ImGuiInitialize(Device* aDevice)
	{
#define LOG_CATEGORY FILE_CATEGORY"->InitializeImGui"

		GRAPHICS_ASSERT(aDevice != nullptr, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(aDevice) " cannot be nullptr!");

		Device_DX11Impl* device = dynamic_cast<Device_DX11Impl*>(aDevice);
		GRAPHICS_ASSERT(device != nullptr, LOG_CATEGORY, result::INVALID_ARGUMENT, STRINGIFY(aDevice) " was not a " STRINGIFY(Device_DX11Impl*) "!");

		Window_DX11Impl* window = device->GetViewport();

		if (not IMGUI_CHECKVERSION())
		{
			LOG_ERROR(LOG_CATEGORY, "Failed to initialize ImGui! Failed version check.");
			return false;
		}

		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

		if (not ImGui_ImplWin32_Init(window->GetWindowHandle()))
		{
			LOG_ERROR(LOG_CATEGORY, "Failed to initialize ImGui! ImGui Win32 backend failed to initialize.");
			ImGuiShutdown();
			return false;
		}
		if (not ImGui_ImplDX11_Init(device->GetDevice(), device->GetDeviceContext()))
		{
			LOG_ERROR(LOG_CATEGORY, "Failed to initialize ImGui! ImGui DX11 backend failed to initialize.");
			ImGuiShutdown();
			return false;
		}

		window->SetImGuiEnabled(true);

		return true;

#undef LOG_CATEGORY
	}

	bool ImGuiShutdown()
	{
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();

		return true;
	}

	bool ImGuiNewFrame()
	{
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		//ImGui::ShowDemoWindow();

		return true;
	}
	bool ImGuiRender()
	{
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		return true;
	}
}

#undef FILE_CATEGORY