#include <AppCore/App.h>
#include <AppCore/Window.h>
#include <AppCore/Overlay.h>
#include <AppCore/JSHelpers.h>
#include  <iostream>
using namespace ultralight;

#include <windows.h>
#include "FmodWrapperLibrary.h"

// Returns an empty string if dialog is canceled
std::string openfilename(const char* filter = "All Files (*.*)\0*.*\0", HWND owner = NULL) {
	OPENFILENAME ofn;
	char fileName[MAX_PATH] = "";
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = owner;
	ofn.lpstrFilter = filter;
	ofn.lpstrFile = fileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	ofn.lpstrDefExt = "";
	std::string fileNameStr;
	if (GetOpenFileName(&ofn))
		fileNameStr = fileName;
	return fileNameStr;
}

class MyApp : public LoadListener {
	RefPtr<Overlay> overlay_;
public:
	MyApp(Ref<Window> win) {
		///
		/// Create an Overlay using the same dimensions as our Window.
		///
		overlay_ = Overlay::Create(win, win->width(), win->height(), 0, 0);

		///
		/// Register our MyApp instance as a load listener so we can handle the
		/// View's OnDOMReady event below.
		///
		overlay_->view()->set_load_listener(this);

		///
		/// Load a string of HTML (we're using a C++11 Raw String Literal)
		///
		overlay_->view()->LoadURL("file:///app.html");
	}

	virtual ~MyApp() {}

	///
	/// Our native JavaScript callback. This function will be called from
	/// JavaScript by calling GetMessage(). We bind the callback within
	/// the DOMReady callback defined below.
	///
	void LoadTrack(const JSObject& thisObject, const JSArgs& args)
	{
		std::cout << "Not implemented yet\n";
	}
	void Stop(const JSObject& thisObject, const JSArgs& args)
	{
		std::cout << "Stop button pressed\n";
		FmodWrapperLibrary::FmodWrapper::getInstance().Stop();
	}
	void Repeat(const JSObject& thisObject, const JSArgs& args)
	{
		String resultString = overlay_->view()->EvaluateScript("document.getElementById('loop').checked;");
		std::string result_str = std::string(resultString.utf8().data());
		if(result_str.compare("false"))
		{
			FmodWrapperLibrary::FmodWrapper::getInstance().Repeat(true);
			std::cout << "Repeat on\n";
		}
		else
		{
			FmodWrapperLibrary::FmodWrapper::getInstance().Repeat(false);
			std::cout << "Repeat off\n";
		}
	}
	void PlayPause(const JSObject& thisObject, const JSArgs& args)
	{
		String resultString = overlay_->view()->EvaluateScript("document.getElementById('play-pause').checked;");
		FmodWrapperLibrary::FmodWrapper::getInstance().PlayPause();
		/*std::string result_str = std::string(resultString.utf8().data());
		if(result_str.compare("true"))
		{
			FmodWrapperLibrary::FmodWrapper::getInstance().Pause();
			std::cout << "Pause\n";
		}
		else
		{
			FmodWrapperLibrary::FmodWrapper::getInstance().Play();
			std::cout << "Play\n";
		}*/
	}
	JSValue OnOpenFileDialog(const JSObject& thisObject, const JSArgs& args) {
		std::string filename = openfilename();
		FmodWrapperLibrary::FmodWrapper::getInstance().LoadSound(filename.c_str());
		return JSValue(filename.c_str());
	}

	void ChangeVolume(const JSObject& thisObject, const JSArgs& args) {
		String resultString = overlay_->view()->EvaluateScript("document.getElementById('volume-slider').value;");
		int volume = std::atoi(resultString.utf8().data());
		std::cout << "Volume: " << volume << std::endl;
		FmodWrapperLibrary::FmodWrapper::getInstance().SetVolume(volume);
	}

	///
	/// Inherited from LoadListener, called when the page has finished parsing
	/// the document.
	///
	/// We perform all our JavaScript initialization here.
	///
	virtual void OnDOMReady(ultralight::View* caller,
		uint64_t frame_id,
		bool is_main_frame,
		const String& url) override {
		///
		/// Set our View's JSContext as the one to use in subsequent JSHelper calls
		///
		Ref<JSContext> context = caller->LockJSContext();
		SetJSContext(context.get());

		///
		/// Get the global object (this would be the "window" object in JS)
		///
		JSObject global = JSGlobalObject();

		///
		/// Bind MyApp::GetMessage to the JavaScript function named "GetMessage".
		///
		/// You can get/set properties of JSObjects by using the [] operator with
		/// the following types as potential property values:
		///  - JSValue
		///      Represents a JavaScript value, eg String, Object, Function, etc.
		///  - JSCallback 
		///      Typedef of std::function<void(const JSObject&, const JSArgs&)>)
		///  - JSCallbackWithRetval 
		///      Typedef of std::function<JSValue(const JSObject&, const JSArgs&)>)
		///
		/// We use the BindJSCallbackWithRetval macro to bind our C++ class member
		/// function to our JavaScript callback.
		///
		global["OnOpenFileDialog"] = BindJSCallbackWithRetval(&MyApp::OnOpenFileDialog);
		global["LoadTrack"] = BindJSCallback(&MyApp::LoadTrack);
		global["PlayPause"] = BindJSCallback(&MyApp::PlayPause);
		global["Stop"] = BindJSCallback(&MyApp::Stop);
		global["Loop"] = BindJSCallback(&MyApp::Repeat);
		global["OnVolumeChange"] = BindJSCallback(&MyApp::ChangeVolume);
	}
};

int main() {
	FmodWrapperLibrary::FmodWrapper::SayHello();
	///
	/// Create our main App instance.
	///
	auto app = App::Create();

	///
	/// Create our Window using default window flags.
	///
	auto window = Window::Create(app->main_monitor(), 300, 300, false, kWindowFlags_Titled);

	///
	/// Set our window title.
	///
	window->SetTitle("Ultralight Sample 4 - JavaScript");

	///
	/// Bind our App's main window.
	///
	/// @note This MUST be done before creating any overlays or calling App::Run
	///
	app->set_window(window);

	///
	/// Create our MyApp instance (creates overlays and handles all logic).
	///
	MyApp my_app(window);

	///
	/// Run our main loop.
	///
	app->Run();

	return 0;
}