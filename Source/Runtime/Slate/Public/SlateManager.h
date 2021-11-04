#include<memory>
#include "SWindow.h"
class SlateManager {

	~SlateManager();
public:
	SlateManager();


	void AddToView(std::shared_ptr<SWindow> window);

protected:
	std::vector<std::shared_ptr<Window>> m_windows;

};