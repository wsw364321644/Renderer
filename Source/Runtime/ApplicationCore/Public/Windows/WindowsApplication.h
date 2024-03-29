#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "Windows/ReadDirectoryChanges.h"
#include "GenericPlatform/GenericApplication.h"
#include "Windows/WindowsDeviceInputManager.h"

class FWindowsWindow;
class FWindowsApplication: public GenericApplication
{
public:
	FWindowsApplication(const HINSTANCE inInstance, const HICON IconHandle);
	virtual ~FWindowsApplication();

	/**
* Create a render window.
*
* @param windowName The name of the window instance. This will also be the
* name that appears in the title of the Window.
* @param clientWidth The width (in pixels) of the window's client area.
* @param clientHeight The height (in pixels) of the window's client area.
* @returns The created window instance.
*/
	virtual std::shared_ptr< FGenericWindow > MakeWindow() override;
	virtual void InitializeWindow(const std::shared_ptr< FGenericWindow >& InWindow, const std::shared_ptr< GenericWindowDefinition >& InDefinition, const std::shared_ptr< FGenericWindow >& InParent, const bool bShowImmediately) override;
	// Handle to application instance.
	HINSTANCE GetWindowsHandle() {
		return m_hInstance;
	}

	/**
 * Invoked when a file is modified on disk.
 */
	FileChangeEvent FileChanged;

	/**
 * To support hot-loading of modified files, you can register a directory
 * path for listening for file change notifications. File change
 * notifications are set through the Application::FileChange event.
 *
 * @param dir The directory to listen for file changes.
 * @param recursive Whether to listen for file changes in sub-folders.
 */
	void RegisterDirectoryChangeListener(const std::wstring& dir,
		bool recursive = true);

	int32_t ProcessMessage(HWND hwnd, uint32_t msg, WPARAM wParam, LPARAM lParam);
	virtual void ProcessDeferredEvents(const float TimeDelta)override;
	std::weak_ptr<FGenericWindow> GetWindowByName(const std::string& windowName) const;
	std::weak_ptr<FGenericWindow> GetWindowByHandle(HWND handle) const;
	virtual void SetMessageHandler(const std::shared_ptr<GenericApplicationMessageHandler >& InMessageHandler) override;
	virtual FDeviceInputManager* GetDeviceInputManager() override { return  m_WindowsDeviceInputManager.get(); }

	void UpdateWindowTitle(FWindowsWindow* Window,const char* const Text);
private:


	// A file modification was detected.
	virtual void OnFileChange(FileChangedEventArgs& e);

	// Directory change listener thread entry point function.
	void CheckFileChanges();

	HINSTANCE m_hInstance;


	// Directory change listener.
	CReadDirectoryChanges m_DirectoryChanges;
	// Thread to run directory change listener.
	std::thread m_DirectoryChangeListenerThread;
	std::mutex  m_DirectoryChangeMutex;
	// Flag to terminate directory change thread.
	std::atomic_bool m_bTerminateDirectoryChangeThread;


	std::shared_ptr<WindowsDeviceInputManager> m_WindowsDeviceInputManager;
};
