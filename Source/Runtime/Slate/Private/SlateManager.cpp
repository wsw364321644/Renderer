#include "SlateManager.h"
#include "GameFramework.h"
SlateManager::~SlateManager()
{
}

SlateManager::SlateManager()
{
}

void SlateManager::AddToView(std::shared_ptr<SWindow> window)
{
	std::shared_ptr <Window> platformWindow=GameFramework::Get().CreateWindow(window->WindowTitle(), window->ClientSize().x, window->ClientSize().y);
	window->SetPlatformWindow(platformWindow);
}
