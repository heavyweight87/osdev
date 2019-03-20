#include <gui/container.hpp>
#include <gui/defaults.hpp>
#include <wm/libwm.h>
#include <dev/keyboard.h>

#include <algorithm>

namespace btos_api{
namespace gui{

class NullContainer : public Container{
private: 
	std::unique_ptr<gds::Surface> surf;

	gds::Surface &GetSurface(){
		if(!surf) surf.reset(new gds::Surface(gds_SurfaceType::Vector, 1, 1, 100, gds_ColourType::True));
		return *surf;
	}
	gds::Rect GetBoundingRect(){
		return {0, 0, 1, 1};
	}
	
	void Update(const gds::Rect &){
	}
	
	void Update(){
	}
	
	void SetSubscribed(uint32_t){
	}
};

static std::unique_ptr<NullContainer> nullContainer;

Container &IControl::GetContainer(){
	if(getContainer) return getContainer();
	else{
		if(!nullContainer) nullContainer.reset(new NullContainer());
		return *nullContainer;
	}
}

template<typename T> static std::shared_ptr<IControl> FNFSearch(std::shared_ptr<IControl> focus, T begin, T end, std::function<bool()> onLastControl){
	auto i = begin;
	bool found = false;
	while(true){
		if(found && !((*i)->GetFlags() & ControlFlags::NoFocus)) return *i;
		if(found && *i == focus) return focus;
		if(*i == focus) found = true;
		++i;
		if(i == end){
			if(onLastControl()) return nullptr;
			i = begin;
		}
	}
}

std::shared_ptr<IControl> Container::FindNextFocus(bool reverse){
	if(reverse) return FNFSearch(focus, controls.rbegin(), controls.rend(), [this, reverse]{return OnLastControlFocus(reverse);});
	else return FNFSearch(focus, controls.begin(), controls.end(), [this, reverse]{return OnLastControlFocus(reverse);});
}

bool Container::HandleEvent(const wm_Event &evt){
	queuePaint = true;
	EventResponse response;
	if(focus && (evt.type & wm_KeyboardEvents) && (focus->GetSubscribed() & evt.type)){
		response = focus->HandleEvent(evt);
	}
	
	if(!controls.empty() && !response.IsFinishedProcessing() && evt.type == wm_EventType::Keyboard){
		int16_t code = KB_code(evt.Key.code);
		if(!(code & KeyFlags::NonASCII) && KB_char(evt.Key.code) == '\t'){
			if(!focus){
				focus = *controls.begin();
				if(focus->GetFlags() & ControlFlags::NoFocus) focus = FindNextFocus(false);
				if(focus) focus->Focus();
			}else{
				focus->Blur();
				focus = FindNextFocus(evt.Key.code & KeyFlags::Shift);
				if(focus) focus->Focus();
			}
		}
	}
	
	if(!response.IsFinishedProcessing() && (evt.type & wm_PointerEvents)){
		auto e = evt;
		auto br = GetBoundingRect();
		e.Pointer.x -= br.x;
		e.Pointer.y -= br.y;
		
		if(e.type == wm_EventType::PointerMove){
			if(mouseOver && !gds::InRect(e.Pointer.x, e.Pointer.y, mouseOver->GetInteractRect())){
				auto leaveEvt = e;
				leaveEvt.type = wm_EventType::PointerLeave;
				HandleEvent(leaveEvt);
				mouseOver.reset();
			}
		}
		
		if(mouseOver && e.type == wm_EventType::PointerLeave){
			response = mouseOver->HandleEvent(e);
		}
		
		if(!response.IsFinishedProcessing()){
			for(auto &c : controls){
				if(e.type == wm_EventType::PointerMove && mouseOver != c && gds::InRect(e.Pointer.x, e.Pointer.y, c->GetInteractRect()) && (c->GetSubscribed() & wm_EventType::PointerEnter)){
					mouseOver = c;
					auto enterEvt = evt;
					enterEvt.type = wm_EventType::PointerEnter;
					HandleEvent(enterEvt);
				}
				if((c->GetSubscribed() & e.type) && gds::InRect(e.Pointer.x, e.Pointer.y, c->GetInteractRect())){
					response = c->HandleEvent(e);
					if(response.IsFinishedProcessing()){
						if(e.type != wm_EventType::PointerMove && e.type != wm_EventType::PointerEnter && e.type != wm_EventType::PointerLeave){
							if(c != focus){
								if(focus){
									focus->Blur();
								}
								focus = c;
								focus->Focus();
							}
						}
						break;
					}
				}
			}
		}
	}
	queuePaint = false;
	Paint(gds::TileRects(paintQueue));
	paintQueue.clear();
	return true;
}
	
void Container::Paint(const std::vector<gds::Rect> &rects){
	if(rects.empty()) return;
	if(queuePaint){ 
		for(auto &r : rects) paintQueue.push_back(r);
		return;
	}
	
	auto &surface = GetSurface();
	auto br = GetBoundingRect();
	surface.Clear();
	surface.BeginQueue();
	auto bkg = colours::GetBackground().Fix(surface);
	surface.Box({0, 0, br.w, br.h}, bkg, bkg, 1, gds_LineStyle::Solid, gds_FillStyle::Filled);
	for(auto &c : controls){
		c->Paint(surface);
	}
	surface.CommitQueue();
	
	for(auto &r : rects){
		if(r.w && r.h) Update();
		else{
			Update();
			break;
		}
	}
	WM_Sync();
}

void Container::Paint(const gds::Rect &r){
	Paint(std::vector<gds::Rect>{r});
}

void Container::AddControl(std::shared_ptr<IControl> control){
	AddControls({control});
}

void Container::AddControls(std::vector<std::shared_ptr<IControl>> ncontrols){
	for(auto &c : ncontrols){
		BindControl(*c);
		controls.push_back(c);
	}
	
	uint32_t subs = wm_EventType::Keyboard;
	for(auto &c : controls){
		subs |= c->GetSubscribed();
	}
	if(subs & (wm_EventType::PointerEnter | wm_EventType::PointerLeave)) subs |= wm_EventType::PointerMove;
	
	SetSubscribed(subs);
	
	std::vector<gds::Rect> paintRects;
	
	for(auto &c : ncontrols){
		paintRects.push_back(c->GetPaintRect());
	}
	Paint(gds::TileRects(paintRects));
}

void Container::BindControl(IControl &control){
	control.getContainer = [this] () -> Container& {return *this;};
}

std::shared_ptr<IControl> &Container::GetFocus(){
	return focus;
}

void Container::FocusNext(bool reverse){
	if(!focus){
		focus = *controls.begin();
		if(focus->GetFlags() & ControlFlags::NoFocus) focus = FindNextFocus(false);
		if(focus) focus->Focus();
	}else{
		focus->Blur();
		focus = FindNextFocus(reverse);
		if(focus) focus->Focus();
	}
}

}
}