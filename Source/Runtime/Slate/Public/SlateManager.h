#pragma once
#include<memory>
#include "GenericPlatform/GenericApplicationMessageHandler.h"
#include "SWindow.h"
#include "GenericPlatform/GenericApplication.h"
class SlateManager:public GenericApplicationMessageHandler {

	
public:
	SlateManager();
	virtual ~SlateManager();
	static std::shared_ptr< SlateManager> Create();

	void AddToView(std::shared_ptr<SWindow> window);

	virtual void OnOSPaint(const std::shared_ptr<GenericWindow>& Window) override;
	virtual void HandleDPIScaleChanged(const std::shared_ptr< GenericWindow >& Window) override;
	/// Polls game devices for input 
	void PollGameDeviceState();
	const std::shared_ptr<SWindow> FindWindowByNative(const std::shared_ptr< GenericWindow >& Window);
	static GenericApplication* GetGenericApplication(){
		return m_GenericApplication.get();
	}
protected:
	std::vector<std::shared_ptr<SWindow>> Windows;
	static std::shared_ptr<GenericApplication> m_GenericApplication;
};