#include <iostream>
#include <gds/libgds.h>
#include <cstdio>
#include "windows.hpp"
#include "window.hpp"

using namespace std;

void InitCursor(){
	uint32_t cursor = GDS_NewSurface(gds_SurfaceType::Bitmap, 12, 21);
	uint32_t cur_black = GDS_GetColour(0, 0, 0);
	uint32_t cur_transp = GDS_GetColour(0, 0, 0, 255);
	uint32_t cur_white = GDS_GetColour(255, 255, 255);
	gds_Point cur_points[] = {{0, 0}, {0, 13}, {3, 10}, {6, 15}, {8, 15}, {8, 14}, {6, 9}, {10, 9}, {1, 0}};
	GDS_Box(0, 0, 11, 20, cur_transp, cur_transp, 1, gds_LineStyle::Solid, gds_FillStyle::Filled);
	GDS_Polygon(9, cur_points, true, cur_black, cur_white, 1, gds_LineStyle::Solid, gds_FillStyle::Filled);
	GDS_SetCursor(cursor, 1, 1);
	GDS_CursorVisibility(true);
}

shared_ptr<Window> CreateTestWin(string title, uint32_t x, uint32_t y, uint32_t w, uint32_t h){
	uint64_t surface = GDS_NewSurface(gds_SurfaceType::Bitmap, w, h);
	GDS_Box(0, 0, 400, 250, GDS_GetColour(0, 0, 0), GDS_GetColour(255, 255, 255), 1, gds_LineStyle::Solid, gds_FillStyle::Filled);
	shared_ptr<Window> win(new Window(surface));
	win->SetPosition(x, y);
	win->SetTitle(title);
	return win;
}

int main(){
    cout << "BT/OS WM" << endl;
	bt_vidmode vidmode;
	vidmode.width = 640;
	vidmode.height = 480;
	vidmode.bpp = 4;
	GDS_SetScreenMode(vidmode);
	InitCursor();
	GDS_SelectScreen();
	shared_ptr<Window> win1 = CreateTestWin("Window 1", 50, 50, 200, 100);
	win1->SetZOrder(10);
	AddWindow(win1);
	DrawWindows();
	getchar();
	shared_ptr<Window> win2 = CreateTestWin("Window 2", 100, 100, 250, 150);
	win2->SetZOrder(20);
	AddWindow(win2);
	DrawWindows();
	getchar();
	win1->SetZOrder(30);
	DrawWindows();
	getchar();
    return 0;
}