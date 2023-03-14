#pragma once
#include "SWidget.h"
#include "GenericPlatform/GenericWindow.h"
#include "Misc/CoreMiscDefines.h"
#include <glm/glm.hpp>
#include <string>
struct ImGuiContext;

class SImguiContent :public SWidget{

	
public:
    SImguiContent();
	virtual ~SImguiContent();

private:
	ImGuiContext* m_pImGuiCtx;
};