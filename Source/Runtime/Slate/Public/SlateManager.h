#pragma once
#include <memory>
#include <vector>
#include "GenericPlatform/GenericApplicationMessageHandler.h"
#include "SWindow.h"
#include "GenericPlatform/GenericApplication.h"



class FSlateManager:public GenericApplicationMessageHandler {
public:
	FSlateManager();
	virtual ~FSlateManager();

	static std::shared_ptr< FSlateManager> Create();

	void AddToView(std::shared_ptr<SWindow> window);

	virtual bool OnKeyChar(const char Character, const bool IsRepeat);
	virtual bool OnKeyDown(const int32_t KeyCode, const uint32_t CharacterCode, const bool IsRepeat);
	virtual bool OnKeyUp(const int32_t KeyCode, const uint32_t CharacterCode, const bool IsRepeat);
	virtual bool OnMouseDown(const std::shared_ptr< FGenericWindow >& Window, const EMouseButton Button);
	virtual bool OnMouseDown(const std::shared_ptr< FGenericWindow >& Window, const EMouseButton, const glm::vec2 CursorPos);
	virtual bool OnMouseUp(const EMouseButton Button);
	virtual bool OnMouseUp(const EMouseButton Button, const glm::vec2 CursorPos);
	virtual bool OnMouseDoubleClick(const std::shared_ptr< FGenericWindow >& Window, const EMouseButton Button);
	virtual bool OnMouseDoubleClick(const std::shared_ptr< FGenericWindow >& Window, const EMouseButton Button, const glm::vec2 CursorPos);
	virtual bool OnMouseWheel(const float Delta);
	virtual bool OnMouseWheel(const float Delta, const glm::vec2 CursorPos);
	virtual bool OnMouseMove();
	virtual bool OnRawMouseMove(const int32_t X, const int32_t Y);
	virtual bool OnCursorSet();

	virtual void OnOSPaint(const std::shared_ptr<FGenericWindow>& Window) override;
	virtual void HandleDPIScaleChanged(const std::shared_ptr< FGenericWindow >& Window) override;
	/// Polls game devices for input 
	void PollGameDeviceState();
	const std::shared_ptr<SWindow> FindWindowByNative(const std::shared_ptr< FGenericWindow >& Window);
	GenericApplication* GetGenericApplication(){
		return m_GenericApplication.get();
	}
protected:
	std::vector<std::shared_ptr<SWindow>> Windows;
	std::shared_ptr<GenericApplication> m_GenericApplication;
};