#include <AppCore/App.h>
#include <AppCore/Window.h>
#include <AppCore/Overlay.h>
#include <AppCore/JSHelpers.h>
#include  <iostream>
using namespace ultralight;

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
	JSValue OnOpenFileDialog(const JSObject& thisObject, const JSArgs& args) {
		///
		/// Return our message to JavaScript as a JSValue.
		///
		///
		std::cout << "OnClick!\n";
		return JSValue("C:\Qualcosa.mp3");
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
	}
};

int main() {
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