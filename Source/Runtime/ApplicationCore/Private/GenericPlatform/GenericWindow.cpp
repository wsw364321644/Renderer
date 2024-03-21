#include "GenericPlatform/GenericWindow.h"


FGenericWindow::FGenericWindow():
	bIsFullscreen(false)
, bIsMinimized(false)
, bIsMaximized(false)
{

}

FGenericWindow::~FGenericWindow()
{

}

void FGenericWindow::ReshapeWindow( int32_t X, int32_t Y, int32_t Width, int32_t Height )
{
	// empty default functionality
}

bool FGenericWindow::GetFullScreenInfo( int32_t& X, int32_t& Y, int32_t& Width, int32_t& Height ) const
{
	// this function cannot return valid results, and should not be needed on consoles, etc
	//UE_LOG(LogGenericPlatformWindow, Fatal, TEXT("GetFullScreenInfo is not expected to be called on this platform"));
	return false;
}

void FGenericWindow::MoveWindowTo ( int32_t X, int32_t Y )
{
	// empty default functionality
}

void FGenericWindow::BringToFront( bool bForce )
{
	// empty default functionality
}

void FGenericWindow::HACK_ForceToFront()
{
	// empty default functionality
}

void FGenericWindow::Destroy()
{
	// empty default functionality
}

void FGenericWindow::Minimize()
{
	// empty default functionality
}

void FGenericWindow::Maximize()
{
	// empty default functionality
}

void FGenericWindow::Restore()
{
	// empty default functionality
}

void FGenericWindow::Show()
{
	// empty default functionality
}

void FGenericWindow::Hide()
{
	// empty default functionality
}

void FGenericWindow::SetWindowMode( EWindowMode InNewWindowMode )
{
	// empty default functionality
}

EWindowMode FGenericWindow::GetWindowMode() const
{
	// default functionality
	return EWindowMode::Windowed;
}

bool FGenericWindow::IsMaximized() const
{
	return bIsMaximized;
}

bool FGenericWindow::IsMinimized() const
{
	return bIsMinimized;
}


bool FGenericWindow::IsVisible() const
{
	// empty default functionality
	return true;
}

bool FGenericWindow::GetRestoredDimensions(int32_t& X, int32_t& Y, int32_t& Width, int32_t& Height)
{
	// this function cannot return valid results, and should not be needed on consoles, etc
	//UE_LOG(LogGenericPlatformWindow, Fatal, TEXT("GetRestoredDimensions is not expected to be called on this platform"));
	return false;
}

void FGenericWindow::SetWindowFocus()
{
	// empty default functionality
}

void FGenericWindow::SetOpacity( const float InOpacity )
{
	// empty default functionality
}

void FGenericWindow::Enable( bool bEnable )
{
	// empty default functionality
}

bool FGenericWindow::IsPointInWindow( int32_t X, int32_t Y ) const
{
	// empty default functionality
	return true;
}
	
int32_t FGenericWindow::GetWindowBorderSize() const
{
	// empty default functionality
	return 0;
}

int32_t FGenericWindow::GetWindowTitleBarSize() const
{
	// empty default functionality
	return 0;
}

void* FGenericWindow::GetOSWindowHandle() const
{
	return nullptr;
}

bool FGenericWindow::IsForegroundWindow() const 
{
	// empty default functionality
	return true;
}

void FGenericWindow::SetText(const char* const Text)
{
	// empty default functionality
}

const GenericWindowDefinition& FGenericWindow::GetDefinition() const
{
	return *Definition.get();
}


void FGenericWindow::AdjustCachedSize(glm::vec2& Size ) const
{
}


void FGenericWindow::OnDPIScaleChanged(DPIScaleEventArgs& e)
{
	DPIScaling = e.DPIScale;
}

void FGenericWindow::OnResize(ResizeEventArgs& e)
{
	if ((bIsMinimized || bIsMaximized) && e.State == EWindowState::Restored)
	{
		bIsMaximized = false;
		bIsMinimized = false;
	}
	if (!bIsMinimized && e.State == EWindowState::Minimized)
	{
		bIsMinimized = true;
		bIsMaximized = false;
	}
	if (!bIsMaximized && e.State == EWindowState::Maximized)
	{
		bIsMaximized = true;
		bIsMinimized = false;
	}
}