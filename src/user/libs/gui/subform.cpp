#include <gui/subform.hpp>

namespace btos_api{
namespace gui{

gds::Surface &SubForm::GetSurface(){
	if(!surf) surf.reset(new gds::Surface(gds_SurfaceType::Vector, rect.w, rect.h, 100, gds_ColourType::True));
	return *surf;
}

gds::Rect SubForm::GetBoundingRect(){
	return rect;
}

void SubForm::Update(const gds::Rect &r){
	auto rt = r;
	rt.x += rect.x;
	rt.y += rect.y;
	updateRects.push_back(rt);
}

void SubForm::Update(){
	updateRects.push_back(rect);
}

void SubForm::SetSubscribed(uint32_t s){
	subs = s;
}

bool SubForm::OnLastControlFocus(bool reverse){
	GetContainer().FocusNext(reverse);
	return GetContainer().GetFocus().get() != this;
}

SubForm::SubForm(const gds::Rect &r): rect(r) {}

EventResponse SubForm::HandleEvent(const wm_Event &e){
	return {Container::HandleEvent(e)};
}


//IControl::Paint
void SubForm::Paint(gds::Surface &s){	
	s.Blit(*surf, {0, 0, rect.w, rect.h}, rect);
}

gds::Rect SubForm::GetPaintRect(){
	return rect;
}

gds::Rect SubForm::GetInteractRect(){
	return rect;
}

uint32_t SubForm::GetSubscribed(){
	return subs;
}

void SubForm::Focus(){
	auto fcs = GetFocus();
	if(fcs) fcs->Focus();
	else FocusNext(false);
}

void SubForm::Blur(){
	auto fcs = GetFocus();
	if(fcs) fcs->Blur();
}

uint32_t SubForm::GetFlags(){
	return 0;
}

//Container::Paint
void SubForm::Paint(const std::vector<gds::Rect> &rects){
	Container::Paint(rects);
	for(const auto &uR : updateRects) GetContainer().Paint(uR);
	updateRects.clear();
}
	
}
}