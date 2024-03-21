#include "SWindow.h"
#include "GenericPlatform/GenericWindow.h"
#include "GenericPlatform/Events.h"
SWindow::~SWindow()
{
}

SWindow::SWindow()
{
}




void SWindow::SetPlatformWindow(std::shared_ptr<FGenericWindow> window)
{
	NativeWindow = window;
}

void SWindow::ShowWindow()
{
	if (NativeWindow) {
		NativeWindow->Show();
	}
}

std::string SWindow::GetTitle() const
{
	return WindowTitle;
}

void SWindow::SetTitle(const std::string& InTitle)
{
	WindowTitle = InTitle;
	NativeWindow->SetText(InTitle.c_str());
}

void SWindow::SetWindowMode(EWindowMode WindowMode)
{
	NativeWindow->SetWindowMode(WindowMode);
}

void SWindow::OnDPIScaleChanged(DPIScaleEventArgs& e)
{
	DPIScaleChanged(e);
}


void SWindow::OnClose(WindowCloseEventArgs& e)
{
    Close(e);
}

void SWindow::OnResize(ResizeEventArgs& e)
{
	if ( e.State == EWindowState::Restored)
	{
		OnRestored(e);
	}
	if (e.State == EWindowState::Minimized)
	{
		OnMinimized(e);
	}
	if (e.State == EWindowState::Maximized)
	{
		OnMaximized(e);
	}
}

void SWindow::OnMinimized(ResizeEventArgs& e)
{
	Minimized(e);
}

void SWindow::OnMaximized(ResizeEventArgs& e)
{
	Maximized(e);
}

void SWindow::OnRestored(ResizeEventArgs& e)
{
	Restored(e);
}

void SWindow::OnUpdate(UpdateEventArgs& e)
{
	Update(e);
}

