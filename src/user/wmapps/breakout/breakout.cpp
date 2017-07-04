#include <gds/libgds.h>
#include <wm/libwm.h>
#include <dev/rtc.h>
#include <iostream>
#include <btos/message.hpp>
#include "game.hpp"

using namespace std;

enum class gamestate{
	Title,
	GameStart,
	GamePlay,
	Finish,
};

int main(){
    uint64_t surface = GDS_NewSurface(gds_SurfaceType::Bitmap, 320, 240);
	/*uint64_t win =*/ WM_NewWindow(100, 100, wm_WindowOptions::Default, wm_EventType::Keyboard | wm_EventType::Close, surface, "Breakout");
	font = GDS_GetFontID("DejaVu Sans", gds_FontStyle::Bold);
	DrawBackground();
	DrawTitle();
	WM_Update();
	bt_handle_t timer = bt_rtc_create_timer(100);
	gamestate state = gamestate::Title;
	Message msg = Message::Recieve();
	while(true){
		bt_msg_header head = msg.Header();
		wm_Event e = WM_ParseMessage(&head);
		if(e.type == wm_EventType::Close) break;
		switch(state){
			case gamestate::Title:{
				if(e.type == wm_EventType::Keyboard) state = gamestate::GameStart;
				break;
			}
			case gamestate::GameStart:{
				InitGame();
				state = gamestate::GamePlay;
				bt_rtc_reset_timer(timer);
				break;
			}
			case gamestate::GamePlay:{
				if(msg.From() == 0 && msg.Source() == bt_rtc_ext_id){
					bt_rtc_reset_timer(timer);
					bool end = GameStep();
					GameDraw();
					if(end) state = gamestate::Finish;
				}else{
					GameEvent(e);
				}
				break;
			}
			case gamestate::Finish:{
				EndGame();
				DrawBackground();
				DrawEndScreen();
				WM_Update();
				bt_rtc_sleep(2000);
				DrawBackground();
				DrawTitle();
				WM_Update();
				state = gamestate::Title;
				break;
			}
		}
		msg.Next();
	}
    return 0;
}
