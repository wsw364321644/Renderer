#include "GenericPlatform/GenericWindow.h"


GenericWindow::GenericWindow():
	bIsFullscreen(false)
, bIsMinimized(false)
, bIsMaximized(false)
{

}

GenericWindow::~GenericWindow()
{

}

void GenericWindow::ReshapeWindow( int32_t X, int32_t Y, int32_t Width, int32_t Height )
{
	// empty default functionality
}

bool GenericWindow::GetFullScreenInfo( int32_t& X, int32_t& Y, int32_t& Width, int32_t& Height ) const
{
	// this function cannot return valid results, and should not be needed on consoles, etc
	//UE_LOG(LogGenericPlatformWindow, Fatal, TEXT("GetFullScreenInfo is not expected to be called on this platform"));
	return false;
}

void GenericWindow::MoveWindowTo ( int32_t X, int32_t Y )
{
	// empty default functionality
}

void GenericWindow::BringToFront( bool bForce )
{
	// empty default functionality
}

void GenericWindow::HACK_ForceToFront()
{
	// empty default functionality
}

void GenericWindow::Destroy()
{
	// empty default functionality
}

void GenericWindow::Minimize()
{
	// empty default functionality
}

void GenericWindow::Maximize()
{
	// empty default functionality
}

void GenericWindow::Restore()
{
	// empty default functionality
}

void GenericWindow::Show()
{
	// empty default functionality
}

void GenericWindow::Hide()
{
	// empty default functionality
}

void GenericWindow::SetWindowMode( EWindowMode InNewWindowMode )
{
	// empty default functionality
}

EWindowMode GenericWindow::GetWindowMode() const
{
	// default functionality
	return EWindowMode::Windowed;
}

bool GenericWindow::IsMaximized() const
{
	return bIsMaximized;
}

bool GenericWindow::IsMinimized() const
{
	return bIsMinimized;
}


bool GenericWindow::IsVisible() const
{
	// empty default functionality
	return true;
}

bool GenericWindow::GetRestoredDimensions(int32_t& X, int32_t& Y, int32_t& Width, int32_t& Height)
{
	// this function cannot return valid results, and should not be needed on consoles, etc
	//UE_LOG(LogGenericPlatformWindow, Fatal, TEXT("GetRestoredDimensions is not expected to be called on this platform"));
	return false;
}

void GenericWindow::SetWindowFocus()
{
	// empty default functionality
}

void GenericWindow::SetOpacity( const float InOpacity )
{
	// empty default functionality
}

void GenericWindow::Enable( bool bEnable )
{
	// empty default functionality
}

bool GenericWindow::IsPointInWindow( int32_t X, int32_t Y ) const
{
	// empty default functionality
	return true;
}
	
int32_t GenericWindow::GetWindowBorderSize() const
{
	// empty default functionality
	return 0;
}

int32_t GenericWindow::GetWindowTitleBarSize() const
{
	// empty default functionality
	return 0;
}

void* GenericWindow::GetOSWindowHandle() const
{
	return nullptr;
}

bool GenericWindow::IsForegroundWindow() const 
{
	// empty default functionality
	return true;
}

void GenericWindow::SetText(const char* const Text)
{
	// empty default functionality
}

const GenericWindowDefinition& GenericWindow::GetDefinition() const
{
	return *Definition.get();
}


void GenericWindow::AdjustCachedSize(glm::vec2& Size ) const
{
}


void GenericWindow::OnDPIScaleChanged(DPIScaleEventArgs& e)
{
	DPIScaling = e.DPIScale;
}

void GenericWindow::OnResize(ResizeEventArgs& e)
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