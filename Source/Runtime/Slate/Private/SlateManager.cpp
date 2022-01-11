#include "SlateManager.h"
#include "GenericPlatform/GameFramework.h"
#include "Misc/Helpers.h"
#include <memory>
SlateManager::~SlateManager()
{
}

SlateManager::SlateManager()
{
}

void SlateManager::AddToView(std::shared_ptr<SWindow> window)
{
	auto app = GameFramework::Get().GetGenericApplication();
	std::shared_ptr <GenericWindow> platformWindow= app->MakeWindow();

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

	app->InitializeWindow(platformWindow, Definition, nullptr, false);
	window->SetPlatformWindow(platformWindow);

	Windows.push_back(window);
}

void SlateManager::OnOSPaint(const std::shared_ptr<GenericWindow>& Window)
{
	const std::shared_ptr < SWindow > window= FindWindowByNative(Window);

	// Delta and total time will be filled in by the Window.
	UpdateEventArgs updateEventArgs(0.0, 0.0);
	auto& Timer=GameFramework::Get().GetGenericApplication()->GetTimer();
	updateEventArgs.DeltaTime = Timer.ElapsedSeconds();
	updateEventArgs.TotalTime = Timer.TotalSeconds();
	window->OnUpdate(updateEventArgs);
}

void SlateManager::HandleDPIScaleChanged(const std::shared_ptr<GenericWindow>& Window)
{
}

const std::shared_ptr<SWindow> SlateManager::FindWindowByNative(const std::shared_ptr<GenericWindow>& Window)
{
	for (auto window : Windows) {
		std::shared_ptr<const GenericWindow> nativewindow = window->GetNativeWindow();
		if (nativewindow == Window) {
			return window;
		}
	}
	return std::shared_ptr<SWindow>();
}
