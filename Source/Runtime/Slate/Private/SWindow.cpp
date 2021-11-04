#include "SWindow.h"
#include "GameFramework.h"
SWindow::~SWindow()
{
}

SWindow::SWindow()
{
}




void SWindow::SetPlatformWindow(std::shared_ptr<Window> window)
{
	
	m_window = window;
}
