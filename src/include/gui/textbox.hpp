#ifndef _TEXTBOX_HPP
#define _TEXTBOX_HPP

#include "icontrol.hpp"

namespace btos_api{
namespace gui{

class TextBox : public IValueControl<std::string>, public IActionControl<void>{
private:
	gds::Rect rect;
	std::string text;
	std::unique_ptr<gds::Surface> surf;
	
	std::function<bool(uint32_t)> onKeyPress;
	
	gds::TextMeasurements textMeasures;
	
	bool update = false;
	size_t textOffset = 0;
	size_t cursorPos = 0;
	bool hasFocus = false;
	bool enabled = true;
	int32_t textY = 0;
	
	void UpdateDisplayState();
public:
	TextBox(const gds::Rect &r, const std::string &t = "");
	
	EventResponse HandleEvent(const wm_Event&);
	void Paint(gds::Surface &surf);
	gds::Rect GetPaintRect();
	gds::Rect GetInteractRect();
	uint32_t GetSubscribed();
	void Focus();
	void Blur();
	uint32_t GetFlags();
	void Enable();
	void Disable();
	bool IsEnabled();
	void SetPosition(const gds::Rect&);
	
	void SetText(const std::string &t);
	std::string GetText();
	std::string GetValue() {return GetText();}
	
	void OnKeyPress(const std::function<bool(uint32_t)> &oKP);
};

}
}

#endif
