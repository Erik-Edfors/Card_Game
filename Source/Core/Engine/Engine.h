#pragma once
#define WIN32_LEAN_AND_MEAN
#define NOGDICAPMASKS     // CC_*, LC_*, PC_*, CP_*, TC_*, RC_
#define NOVIRTUALKEYCODES // VK_*
#define NOWINMESSAGES     // WM_*, EM_*, LB_*, CB_*
#define NOWINSTYLES       // WS_*, CS_*, ES_*, LBS_*, SBS_*, CBS_*
#define NOSYSMETRICS      // SM_*
#define NOMENUS           // MF_*
#define NOICONS           // IDI_*
#define NOKEYSTATES       // MK_*
#define NOSYSCOMMANDS     // SC_*
#define NORASTEROPS       // Binary and Tertiary raster ops
#define NOSHOWWINDOW      // SW_*
#define OEMRESOURCE       // OEM Resource values
#define NOATOM            // Atom Manager routines
#define NOCLIPBOARD       // Clipboard routines
#define NOCOLOR           // Screen colors
#define NOCTLMGR          // Control and Dialog routines
#define NODRAWTEXT        // DrawText() and DT_*
#define NOGDI             // All GDI defines and routines
#define NOKERNEL          // All KERNEL defines and routines
#define NOUSER            // All USER defines and routines
#define NONLS             // All NLS defines and routines
#define NOMB              // MB_* and MessageBox()
#define NOMEMMGR          // GMEM_*, LMEM_*, GHND, LHND, associated routines
#define NOMETAFILE        // typedef METAFILEPICT
#define NOMINMAX          // Macros min(a,b) and max(a,b)
#define NOMSG             // typedef MSG and associated routines
#define NOOPENFILE        // OpenFile(), OemToAnsi, AnsiToOem, and OF_*
#define NOSCROLL          // SB_* and scrolling routines
#define NOSERVICE         // All Service Controller routines, SERVICE_ equates, etc.
#define NOSOUND           // Sound driver routines
#define NOTEXTMETRIC      // typedef TEXTMETRIC and associated routines
#define NOWH              // SetWindowsHook and WH_*
#define NOWINOFFSETS      // GWL_*, GCL_*, associated routines
#define NOCOMM            // COMM driver routines
#define NOKANJI           // Kanji support stuff.
#define NOHELP            // Help engine interface.
#define NOPROFILER        // Profiler interface.
#define NODEFERWINDOWPOS  // DeferWindowPos routines
#define NOMCX             // Modem Configuration Extensions
#include <Windows.h>

#include <memory>
#include <vector>

#include <Core/RenderHardwareInterface/PImpl.hpp>

namespace CommonUtilities
{
	class InputManager;
	class TimeManager;
}

class GraphicsEngine;
class Scene;
class Console;
class CommandParser;

namespace graphics
{
	class Window;
}

class Engine
{
public:
	Engine();
	~Engine();

	bool Initialize(HWND aWindow, const wchar_t* aAssetPath);
	bool Shutdown();

	bool ProcessWindowsMessage(_In_ HWND hWnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam);

	void SetConsoleEnabled(bool aEnabled) noexcept;
	bool IsConsoleEnabled() const noexcept;

	bool BeginFrame();
	void Update();
	void EndFrame();

	Console& GetConsole() noexcept;
	CommandParser& GetCommandParser() noexcept;
	GraphicsEngine& GetGraphicsEngine() noexcept;

	const Console& GetConsole() const noexcept;
	const CommandParser& GetCommandParser() const noexcept;
	const GraphicsEngine& GetGraphicsEngine() const noexcept;
	const CommonUtilities::InputManager& GetInputManager() const noexcept;
	const CommonUtilities::TimeManager& GetTimeManager() const noexcept;

	Scene* GetScene() noexcept;

	template<class _SceneTy, class... _Args>
	void PushScene(_Args&&... aArgs);

	template<class _SceneTy>
	void ChangeScene();

	void PopScene();

private:
	bool myConsoleEnabled;

	graphics::PImpl<graphics::Window> myWindow;

	std::unique_ptr<Console> myConsole;
	std::unique_ptr<CommandParser> myCommandParser;

	std::unique_ptr<CommonUtilities::InputManager> myInputManager;

	std::unique_ptr<GraphicsEngine> myGraphicsEngine;

	std::unique_ptr<CommonUtilities::TimeManager> myTimeManager;

	std::vector<std::unique_ptr<Scene>> mySceneStack;
};

template<class _SceneTy, class... _Args>
inline void Engine::PushScene(_Args&&... aArgs)
{
	mySceneStack.push_back(std::make_unique<_SceneTy>(std::forward<_Args>(aArgs)...));
	mySceneStack.back()->Initialize(this);
}

template<class _SceneTy>
inline void Engine::ChangeScene()
{
	PopScene();
	PushScene<_SceneTy>();
}
