#include <gui/label.hpp>
#include <gui/defaults.hpp>

namespace btos_api{
namespace gui{

Label::Label(const gds::Rect &r, const std::string &t, Label::Justification j) : rect(r), text(t), just(j) {}
	
EventResponse Label::HandleEvent(const wm_Event&){
	return {false};
}

void Label::Paint(gds::Surface &s){
	if(!surf){
		surf.reset(new gds::Surface(gds_SurfaceType::Vector, rect.w, rect.h, 100, gds_ColourType::True));
		
		auto textMeasures = surf->MeasureText(text, fonts::GetLabelFont(), fonts::GetLabelTextSize());
		auto bkgCol = colours::GetBackground().Fix(*surf);
		auto txtCol = colours::GetLabelText().Fix(*surf);
			
		int32_t textX;
		switch(just){
			case Label::Justification::Left:
				textX = 0;
				break;
			case Label::Justification::Right:
				textX = rect.w - textMeasures.w;
				break;
			case Label::Justification::Center:
				textX = std::max<int32_t>(((rect.w - textMeasures.w) / 2), 0);
				break;
		}
		
		int32_t textY = std::max<int32_t>(((rect.h + textMeasures.h) / 2), 0);
		
		surf->Box({0, 0, rect.w, rect.h}, bkgCol, bkgCol, 1, gds_LineStyle::Solid, gds_FillStyle::Filled);
		surf->Text({textX, textY}, text, fonts::GetLabelFont(), fonts::GetLabelTextSize(), txtCol);
		surf->Compress();
	}
	
	s.Blit(*surf, {0, 0, rect.w, rect.h}, rect);
}

gds::Rect Label::GetPaintRect(){
	return rect;
}

gds::Rect Label::GetInteractRect(){
	return {0, 0, 0, 0};
}

uint32_t Label::GetSubscribed(){
	return 0;
}
	
void Label::SetText(const std::string &t){
	text = t;
	surf.reset();
	IControl::Paint(rect);
}

void Label::SetPosition(const gds::Rect &r){
	rect = r;
	surf.reset();
}

}
}
