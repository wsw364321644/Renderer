#include "Window.h"
#include "DirectXMath.h"
#include "Misc/CoreMiscDefines.h"
class SWindow {

	~SWindow();
public:
	SWindow();


	void SetPlatformWindow(std::shared_ptr<Window> window);



private:
	DirectX::XMFLOAT2 m_screenPosition;
	DirectX::XMFLOAT2 m_clientMaxSize;
	DirectX::XMFLOAT2 m_clientMinSize;



	std::shared_ptr<Window> m_window;



	RWProperty(std::wstring, WindowTitle)
	RWProperty(DirectX::XMFLOAT2, ClientSize)

};