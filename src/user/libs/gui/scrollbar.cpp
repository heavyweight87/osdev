#include <gui/scrollbar.hpp>
#include <gui/defaults.hpp>
#include <gui/drawing.hpp>
#include <dev/mouse.h>
#include <dev/keyboard.h>
#include <wm/libwm.h>

namespace btos_api{
namespace gui{

static inline gds::Point swapXY(bool swap, gds::Point p){
	if(swap) std::swap(p.x, p.y);
	return p;
}

static inline gds::Rect swapXY(bool swap, gds::Rect r){
	if(swap){
		std::swap(r.x, r.y);
		std::swap(r.w, r.h);
	}
	return r;
}

template<typename T>
static inline void swapXY(bool swap, T &a, T &b){
	if(swap) std::swap(a, b);
}

Scrollbar::Scrollbar(const gds::Rect &r, uint32_t l, uint32_t s, uint32_t p, uint32_t v, bool h) : rect(r), lines(l), step(s), page(p), value(v), horiz(h) {}

EventResponse Scrollbar::HandleEvent(const wm_Event &e){
	bool handled = false;
	bool update = false;
	uint32_t oldValue = value;
	
	if(e.type & wm_PointerEvents){
		int32_t p = horiz ? e.Pointer.x - rect.x : e.Pointer.y - rect.y;
		auto h = horiz ? rect.w : rect.h;
		auto w = horiz ? rect.h : rect.w;
		
		int32_t sqSize = w;
		double scale = (double)(h - (3 * sqSize)) / (double)lines;
		int32_t pos = ((double)value * scale) + sqSize;
		uint32_t possValue = (double)((double)p - ((double)sqSize * 1.5)) * (1.0 / scale);
		if(sqSize * 1.5 > p) possValue = 0;
		if(possValue > lines) possValue = lines;
		
		enum class Area{
			TopButton, UpperSpace, Box, LowerSpace, BottomButton
		} over;
		
		if(p < sqSize) over = Area::TopButton;
		else if(p < pos) over = Area::UpperSpace;
		else if(p < pos + sqSize) over = Area::Box;
		else if(p < (int32_t)h - sqSize) over = Area::LowerSpace;
		else over = Area::BottomButton;
		
		if(e.type == wm_EventType::PointerButtonUp && e.Pointer.button == 1){
			handled = true;
			if(grabbed){
				value = possValue;
			}else{
				switch(over){
					case Area::TopButton:
						value = std::max<int32_t>(0, value - step);
						break;
					case Area::UpperSpace:
						value = std::max<int32_t>(0, value - page);
						break;
					case Area::Box:
						// Handled by "grabbed" case
						break;
					case Area::LowerSpace:
						value = std::min(lines, value + page);
						break;
					case Area::BottomButton:
						value = std::min(lines, value + step);
						break;
				}
			}
			if(topBtnDown || btmBtnDown || grabbed) update = true;
			topBtnDown = btmBtnDown = grabbed = false;
		}else if(e.type == wm_EventType::PointerButtonDown && e.Pointer.button == 1){
			handled = true;
			topBtnDown = btmBtnDown = grabbed = false;
			switch(over){
				case Area::TopButton:
					topBtnDown = true;
					update = true;
					break;
				case Area::BottomButton:
					btmBtnDown = true;
					update = true;
					break;
				case Area::Box:
					grabbed = true;
				default:
					break;
			}
		}else if(e.type == wm_EventType::PointerMove || e.type == wm_EventType::PointerEnter){
			auto pinfo = WM_GetPointerInfo();
			if(pinfo.flags & MouseFlags::Button1){
				handled = true;
				if(grabbed) value = possValue;
				else{
					if(topBtnDown | btmBtnDown) update = true;
					topBtnDown = btmBtnDown = false;
					switch(over){
						case Area::TopButton:
							topBtnDown = true;
							update = true;
							break;
						case Area::BottomButton:
							btmBtnDown = true;
							update = true;
							break;
						default:
							break;
					}
				}
			}else grabbed = false;
		}else if(e.type == wm_EventType::PointerLeave){
			handled = true;
			topBtnDown = btmBtnDown = false;
			update = true;
		}
	}else if(e.type == wm_EventType::Keyboard && !(e.Key.code & KeyFlags::KeyUp)){
		uint16_t code = KB_code(e.Key.code);
		if(code == (KeyFlags::NonASCII | KeyCodes::LeftArrow) || code == (KeyFlags::NonASCII | KeyCodes::UpArrow)){
			value = std::max<int32_t>(0, value - step);
			update = true;
			handled = true;
		}else if(code == (KeyFlags::NonASCII | KeyCodes::RightArrow) || code == (KeyFlags::NonASCII | KeyCodes::DownArrow)){
			value = std::min(lines, value + step); 
			update = true;
			handled = true;
		}else if(code == (KeyFlags::NonASCII | KeyCodes::PageUp)){
			value = std::max<int32_t>(0, value - page);
			update = true;
			handled = true;
		}else if(code == (KeyFlags::NonASCII | KeyCodes::PageDown)){
			value = std::min(lines, value + page);
			update = true;
			handled = true;
		}
	}
	
	if(value != oldValue){
		RaiseChangeEvent();
		update = true;
	}
	if(update){
		IControl::Paint(rect);
	}
	return {handled};
}

void Scrollbar::Paint(gds::Surface &s){
	uint32_t w = rect.w;
	uint32_t h = rect.h;
	swapXY(horiz, w, h);
	int32_t x = rect.x;
	int32_t y = rect.y;
	swapXY(horiz, x, y);
	uint32_t inW = rect.w - 1;
	uint32_t inH = rect.h - 1;
	swapXY(horiz, inW, inH);
	int32_t sqSize = w;
	
	if(!bkSurf){
		bkSurf.reset(new gds::Surface(gds_SurfaceType::Vector, rect.w, rect.h, 100, gds_ColourType::True));
		bkSurf->BeginQueue();
		
		auto bkgCol = colours::GetBackground().Fix(*bkSurf);
		bkSurf->Box(swapXY(horiz, {0, 0, w, h}), bkgCol, bkgCol, 1, gds_LineStyle::Solid, gds_FillStyle::Filled);
		
		auto scrCol = colours::GetScrollbarBackground().Fix(*bkSurf);
		bkSurf->Box(swapXY(horiz, {0, sqSize - 1, w, inH - (sqSize * 2) + 3}), scrCol, scrCol, 1, gds_LineStyle::Solid, gds_FillStyle::Filled);
		
		auto btn = colours::GetScrollbarButton().Fix(*bkSurf);
		bkSurf->Box(swapXY(horiz, {0, 1, (uint32_t)sqSize, (uint32_t)sqSize - 2}), btn, btn, 1, gds_LineStyle::Solid, gds_FillStyle::Filled);
		bkSurf->Box(swapXY(horiz, {0, (int32_t)h - sqSize + 2, (uint32_t)sqSize, (uint32_t)sqSize - 3}), btn, btn, 1, gds_LineStyle::Solid, gds_FillStyle::Filled);
		
		auto bdr = colours::GetBorder().Fix(*bkSurf);
		drawing::Border(*bkSurf, swapXY(horiz, {0, 0, (uint32_t)sqSize - 1, (uint32_t)sqSize - 1}), bdr);
		drawing::Border(*bkSurf, swapXY(horiz, {0, (int32_t)h - sqSize, (uint32_t)sqSize - 1, (uint32_t)sqSize - 1}), bdr);
		
		auto arw = colours::GetScrollbarArrow().Fix(*bkSurf);
		int32_t half = sqSize / 2;
		int32_t third = sqSize / 3;
		std::vector<gds::Point> topArrow = {swapXY(horiz, {half, third}), swapXY(horiz, {2 * third + 1, 2 * third}), swapXY(horiz, {third, 2 * third})};
		bkSurf->Polygon(topArrow, true, arw, arw, 1, gds_LineStyle::Solid, gds_FillStyle::Filled);

		int32_t btmTop = h - sqSize;
		std::vector<gds::Point> btmArrow = {swapXY(horiz, {half, btmTop + (2 * third) + 1}), swapXY(horiz, {2 * third + 1, btmTop + third + 1}), swapXY(horiz, {third, btmTop + third + 1})};
		bkSurf->Polygon(btmArrow, true, arw, arw, 1, gds_LineStyle::Solid, gds_FillStyle::Filled);
		
		bkSurf->CommitQueue();
		bkSurf->Compress();
	}
	
	s.Blit(*bkSurf, {0, 0, rect.w, rect.h}, rect);
	
	auto topLeft = colours::GetScrollbarHiLight().Fix(s);
	auto bottomRight = colours::GetScrollbarLowLight().Fix(s);
	
	auto topBtnTL = topLeft;
	auto topBtnBR = bottomRight;
	if(topBtnDown) std::swap(topBtnTL, topBtnBR);
	drawing::BevelBox(s, swapXY(horiz, {x + 1,  y + 1, (uint32_t)sqSize - 3, (uint32_t)sqSize - 3}), topBtnTL, topBtnBR);
	
	auto btmBtnTL = topLeft;
	auto btmBtnBR = bottomRight;
	if(btmBtnDown) std::swap(btmBtnTL, btmBtnBR);
	drawing::BevelBox(s, swapXY(horiz, {x+ 1, y + (int32_t)(h - sqSize) + 1, (uint32_t)sqSize - 3, (uint32_t)sqSize - 3}), btmBtnTL, btmBtnBR);
	
	double scale = (double)(h - (3 * sqSize)) / (double)lines;
	auto v = std::min(value, lines);
	int32_t pos = ((double)v * scale) + sqSize;
	
	auto bdr = colours::GetBorder().Fix(s);
	auto btn = colours::GetScrollbarButton().Fix(s);
	s.Box(swapXY(horiz, {x + 1, y + pos + 1, (uint32_t)sqSize - 2, (uint32_t)sqSize - 2}), btn, btn, 1, gds_LineStyle::Solid, gds_FillStyle::Filled);
	drawing::Border(s, swapXY(horiz, {x, y + pos, (uint32_t)sqSize - 1, (uint32_t)sqSize - 1}), bdr);
	drawing::BevelBox(s, swapXY(horiz, {x + 1, y + pos + 1, (uint32_t)sqSize - 3, (uint32_t)sqSize - 3}), topLeft, bottomRight);
	if(focus){
		auto fcs = colours::GetScrollbarFocus().Fix(s);
		s.Box(swapXY(horiz, {x + 3, y + pos + 3, (uint32_t)sqSize - 6, (uint32_t)sqSize - 6}), fcs, fcs);
	}
	
	if(!enabled){
		auto cast = colours::GetDisabledCast().Fix(s);
		s.Box(rect, cast, cast, 1, gds_LineStyle::Solid, gds_FillStyle::Filled);
	}
}

gds::Rect Scrollbar::GetPaintRect(){
	return rect;
}

gds::Rect Scrollbar::GetInteractRect(){
	return rect;
}

uint32_t Scrollbar::GetSubscribed(){
	return wm_EventType::PointerMove | wm_EventType::PointerButtonDown | wm_EventType::PointerButtonUp | wm_EventType::Keyboard | wm_EventType::PointerLeave | wm_EventType::PointerEnter;
}

void Scrollbar::Focus(){
	if(!focus){
		focus = true;
		IControl::Paint(rect);
	}
}

void Scrollbar::Blur(){
	if(focus){
		focus = false;
		IControl::Paint(rect);
	}
}
	
void Scrollbar::SetLines(uint32_t l){
	if(l != lines){
		lines = l;
		IControl::Paint(rect);
	}
}

void Scrollbar::SetStep(uint32_t s){
	if(s != step){
		step = s;
		IControl::Paint(rect);
	}
}

void Scrollbar::SetPage(uint32_t p){
	if(p != page){
		page = p;
		IControl::Paint(rect);
	}
}

void Scrollbar::SetValue(uint32_t v){
	if(v != value){
		value = v;
		IControl::Paint(rect);
	}
}

uint32_t Scrollbar::GetValue(){
	return value;
}

uint32_t Scrollbar::GetFlags(){
	return 0;
}

void Scrollbar::Enable(){
	if(!enabled){
		enabled = true;
		IControl::Paint(rect);
	}
}

void Scrollbar::Disable(){
	if(enabled){
		enabled = false;
		IControl::Paint(rect);
	}
}

bool Scrollbar::IsEnabled(){
	return enabled;
}

void Scrollbar::Refresh(){
	IControl::Paint(rect);
}

void Scrollbar::SetPosition(const gds::Rect &r){
	rect = r;
	bkSurf.reset();
	IControl::Paint(rect);
}

}
}
