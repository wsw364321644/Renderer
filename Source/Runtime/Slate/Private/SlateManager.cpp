#include "SlateManager.h"
#include "GenericPlatform/GameFramework.h"
#include "Misc/Helpers.h"
#include "GenericPlatform/PlatformApplicationMisc.h"
#include "Misc/App.h"
#include "imgui.h"
#include <std_ext.h>
#include <memory>
std::shared_ptr<GenericApplication> SlateManager::m_GenericApplication;
SlateManager::~SlateManager()
{
}

std::shared_ptr<SlateManager> SlateManager::Create()
{
	auto sm=std::make_shared<SlateManager>();
	m_GenericApplication.reset(FPlatformApplicationMisc::CreateApplication());
	m_GenericApplication->SetMessageHandler(sm);
	
	return sm;
}

SlateManager::SlateManager()
{
}

void SlateManager::AddToView(std::shared_ptr<SWindow> window)
{

	std::shared_ptr <FGenericWindow> platformWindow= m_GenericApplication->MakeWindow();

	std::shared_ptr< GenericWindowDefinition > Definition = std::make_shared< GenericWindowDefinition>();

	//Definition->Type = window->GetType();

	//const FVector2D Size = window->GetInitialDesiredSizeInScreen();
	Definition->WidthDesiredOnScreen = window->ClientSize.x;
	Definition->HeightDesiredOnScreen = window->ClientSize.y;

	//const FVector2D Position = window->GetInitialDesiredPositionInScreen();
	Definition->XDesiredPositionOnScreen = window->ScreenPosition.x;
	Definition->YDesiredPositionOnScreen = window->ScreenPosition.y;

	//Definition->HasOSWindowBorder = InSlateWindow->HasOSWindowBorder();
	//Definition->TransparencySupport = InSlateWindow->GetTransparencySupport();
	//Definition->AppearsInTaskbar = InSlateWindow->AppearsInTaskbar();
	//Definition->IsTopmostWindow = InSlateWindow->IsTopmostWindow();
	//Definition->AcceptsInput = InSlateWindow->AcceptsInput();
	//Definition->ActivationPolicy = InSlateWindow->ActivationPolicy();
	//Definition->FocusWhenFirstShown = InSlateWindow->IsFocusedInitially();

	//Definition->HasCloseButton = InSlateWindow->HasCloseBox();
	//Definition->SupportsMinimize = InSlateWindow->HasMinimizeBox();
	//Definition->SupportsMaximize = InSlateWindow->HasMaximizeBox();

	//Definition->IsModalWindow = InSlateWindow->IsModalWindow();
	//Definition->IsRegularWindow = InSlateWindow->IsRegularWindow();
	//Definition->HasSizingFrame = InSlateWindow->HasSizingFrame();
	//Definition->SizeWillChangeOften = InSlateWindow->SizeWillChangeOften();
	//Definition->ShouldPreserveAspectRatio = InSlateWindow->ShouldPreserveAspectRatio();
	//Definition->ExpectedMaxWidth = InSlateWindow->GetExpectedMaxWidth();
	//Definition->ExpectedMaxHeight = InSlateWindow->GetExpectedMaxHeight();

	Definition->Title = window->WindowTitle;
	//Definition->Opacity = InSlateWindow->GetOpacity();
	//Definition->CornerRadius = InSlateWindow->GetCornerRadius();

	//Definition->SizeLimits = InSlateWindow->GetSizeLimits();

	m_GenericApplication->InitializeWindow(platformWindow, Definition, nullptr, false);
	window->SetPlatformWindow(platformWindow);

	Windows.push_back(window);
}

bool SlateManager::OnKeyChar(const char Character, const bool IsRepeat)
{
	if (ImGui::GetCurrentContext() != NULL) {
		ImGuiIO& io = ImGui::GetIO();
		io.AddInputCharacterUTF16((unsigned short)Character);
	}
	return false;
}

bool SlateManager::OnKeyDown(const int32_t KeyCode, const uint32_t CharacterCode, const bool IsRepeat)
{
	if (ImGui::GetCurrentContext() != NULL) {
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[KeyCode] = true;
	}

	
	return false;
}

bool SlateManager::OnKeyUp(const int32_t KeyCode, const uint32_t CharacterCode, const bool IsRepeat)
{
	if (ImGui::GetCurrentContext() != NULL) {
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[KeyCode] = false;
	}

	return false;
}

bool SlateManager::OnMouseDown(const std::shared_ptr<FGenericWindow>& Window, const EMouseButton Button)
{
	return OnMouseDown(Window, Button, GetGenericApplication()->GetDeviceInputManager()->GetCursorPosition());
}

bool SlateManager::OnMouseDown(const std::shared_ptr<FGenericWindow>& Window, const EMouseButton Button, const glm::vec2 CursorPos)
{
	if (ImGui::GetCurrentContext() != NULL) {
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[std::to_underlying<EMouseButton>(Button)] = true;
	}
	

	return false;
}

bool SlateManager::OnMouseUp(const EMouseButton Button)
{
	return OnMouseUp(Button, GetGenericApplication()->GetDeviceInputManager()->GetCursorPosition());
}

bool SlateManager::OnMouseUp(const EMouseButton Button, const glm::vec2 CursorPos)
{
	if (ImGui::GetCurrentContext() != NULL) {
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[std::to_underlying<EMouseButton>(Button)] = false;
	}
	return false;
}

bool SlateManager::OnMouseDoubleClick(const std::shared_ptr<FGenericWindow>& Window, const EMouseButton Button)
{
	return OnMouseDoubleClick(Window, Button, GetGenericApplication()->GetDeviceInputManager()->GetCursorPosition());
}

bool SlateManager::OnMouseDoubleClick(const std::shared_ptr<FGenericWindow>& Window, const EMouseButton Button, const glm::vec2 CursorPos)
{
	if (ImGui::GetCurrentContext() != NULL) {
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[std::to_underlying<EMouseButton>(Button)] = true;
	}
	return false;
}

bool SlateManager::OnMouseWheel(const float Delta)
{
	return OnMouseWheel(Delta, GetGenericApplication()->GetDeviceInputManager()->GetCursorPosition());
}

bool SlateManager::OnMouseWheel(const float Delta, const glm::vec2 CursorPos)
{
	if (ImGui::GetCurrentContext() != NULL) {
		ImGuiIO& io = ImGui::GetIO();
		io.MouseWheel += Delta;
	}
	return false;
}

bool SlateManager::OnMouseMove()
{
	auto pos = GetGenericApplication()->GetDeviceInputManager()->GetCursorPosition();
	return OnRawMouseMove(pos.x, pos.y);
}

bool SlateManager::OnRawMouseMove(const int32_t X, const int32_t Y)
{
	return false;
}

bool SlateManager::OnCursorSet()
{
	return false;
}

void SlateManager::OnOSPaint(const std::shared_ptr<FGenericWindow>& Window)
{
	const std::shared_ptr < SWindow > window= FindWindowByNative(Window);

	// Delta and total time will be filled in by the Window.
	UpdateEventArgs updateEventArgs(0.0, 0.0);
	auto& Timer= m_GenericApplication->GetTimer();
	updateEventArgs.DeltaTime = Timer.ElapsedSeconds();
	updateEventArgs.TotalTime = Timer.TotalSeconds();
	window->OnUpdate(updateEventArgs);
}

void SlateManager::HandleDPIScaleChanged(const std::shared_ptr<FGenericWindow>& Window)
{
	ImGuiIO& io = ImGui::GetIO();
	io.FontGlobalScale = Window->GetDPIScaling();
}

void SlateManager::PollGameDeviceState()
{
	m_GenericApplication->PollGameDeviceState(FApp::GetDeltaTime());
}

const std::shared_ptr<SWindow> SlateManager::FindWindowByNative(const std::shared_ptr<FGenericWindow>& Window)
{
	for (auto window : Windows) {
		std::shared_ptr<const FGenericWindow> nativewindow = window->GetNativeWindow();
		if (nativewindow == Window) {
			return window;
		}
	}
	return std::shared_ptr<SWindow>();
}
