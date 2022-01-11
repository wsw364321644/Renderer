#pragma once
#include<memory>
#include "GenericPlatform/GenericApplicationMessageHandler.h"
#include "SWindow.h"
class SlateManager:public GenericApplicationMessageHandler {

	
public:
	SlateManager();

	virtual ~SlateManager();
	void AddToView(std::shared_ptr<SWindow> window);

	virtual void OnOSPaint(const std::shared_ptr<GenericWindow>& Window) override;
	virtual void HandleDPIScaleChanged(const std::shared_ptr< GenericWindow >& Window) override;

	const std::shared_ptr<SWindow> FindWindowByNative(const std::shared_ptr< GenericWindow >& Window);
protected:
	std::vector<std::shared_ptr<SWindow>> Windows;

};