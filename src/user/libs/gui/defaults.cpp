#include <gui/defaults.hpp>
#include <gds/libgds.h>
#include <btos/resc.h>
#include <dev/rtc.h>
#include <unistd.h>

#include "gui_resc.tar.h"

namespace btos_api{
namespace gui{
	
namespace colours{
	namespace constants{
		const gds::Colour Black               {0, 0x00, 0x00, 0x00, 0};
		const gds::Colour Blue                {0, 0x00, 0x00, 0xA8, 0};
		const gds::Colour Green               {0, 0x00, 0xA8, 0x00, 0};
		const gds::Colour Cyan                {0, 0x00, 0xA8, 0xA8, 0};
		const gds::Colour Red                 {0, 0xA8, 0x00, 0x00, 0};
		const gds::Colour Purple              {0, 0xA8, 0x00, 0xA8, 0};
		const gds::Colour Orange              {0, 0xA8, 0x54, 0x00, 0};
		const gds::Colour Grey                {0, 0xA8, 0xA8, 0xA8, 0};
		const gds::Colour DarkGrey    		  {0, 0x54, 0x54, 0x54, 0};
		const gds::Colour LightBlue   		  {0, 0x54, 0x54, 0xFF, 0};
		const gds::Colour LightGreen  	  	  {0, 0x54, 0xFF, 0x54, 0};
		const gds::Colour LightCyan   		  {0, 0x54, 0xFF, 0xFF, 0};
		const gds::Colour Pink                {0, 0xFF, 0x54, 0x54, 0};
		const gds::Colour Magenta             {0, 0xFF, 0x54, 0xFF, 0};
		const gds::Colour Yellow              {0, 0xFF, 0xFF, 0x54, 0};
		const gds::Colour White               {0, 0xFF, 0xFF, 0xFF, 0};
	}
	
	gds::Colour GetBackground(){
		return constants::White;
	}
	
	gds::Colour GetSelection(){
		return constants::LightBlue;
	}
	
	gds::Colour GetSelectionFocus(){
		return constants::Blue;
	}
	
	gds::Colour GetDisabledCast(){
		auto c = constants::Grey;
		c.a = 128;
		return c;
	}
	
	gds::Colour GetBorder(){
		return constants::Black;
	}
	
	gds::Colour GetButtonColour(){
		return constants::Grey;
	}
	
	gds::Colour GetButtonHiLight(){
		return constants::White;
	}
	
	gds::Colour GetButtonLowLight(){
		return constants::DarkGrey;
	}
	
	gds::Colour GetButtonText(){
		return constants::Black;
	}
	
	gds::Colour GetButtonFocus(){
		return constants::DarkGrey;
	}
	
	gds::Colour GetImageButtonColour(){
		return constants::Grey;
	}
	
	gds::Colour GetImageButtonHiLight(){
		return constants::White;
	}
	
	gds::Colour GetImageButtonLowLight(){
		return constants::DarkGrey;
	}
	
	gds::Colour GetImageButtonFocus(){
		return constants::DarkGrey;
	}
	
	gds::Colour GetLabelText(){
		return constants::Black;
	}
	
	gds::Colour GetTextBoxText(){
		return constants::Black;
	}
	
	gds::Colour GetTextBoxHiLight(){
		return constants::Grey;
	}
	
	gds::Colour GetTextBoxLowLight(){
		return constants::DarkGrey;
	}
	
	gds::Colour GetTextCursor(){
		return constants::Black;
	}
	
	gds::Colour GetSliderLineHiLight(){
		return constants::Grey;
	}
	
	gds::Colour GetSliderLineLowLight(){
		return constants::DarkGrey;
	}
	
	gds::Colour GetSliderColour(){
		return constants::Grey;
	}
	
	gds::Colour GetSliderFocus(){
		return constants::DarkGrey;
	}
	
	gds::Colour GetSliderTick(){
		return constants::Black;
	}
	
	gds::Colour GetCheckboxText(){
		return constants::Black;
	}
	
	gds::Colour GetCheckboxHiLight(){
		return constants::Grey;
	}
	
	gds::Colour GetCheckboxLowLight(){
		return constants::DarkGrey;
	}
	
	gds::Colour GetCheckboxCheck(){
		return constants::Black;
	}
	
	gds::Colour GetCheckboxFocus(){
		return constants::DarkGrey;
	}
	
	gds::Colour GetRadioButtonText(){
		return constants::Black;
	}
	
	gds::Colour GetRadioButtonHiLight(){
		return constants::Grey;
	}
	
	gds::Colour GetRadioButtonLowLight(){
		return constants::DarkGrey;
	}
	
	gds::Colour GetRadioButtonCheck(){
		return constants::Black;
	}
	
	gds::Colour GetRadioButtonFocus(){
		return constants::DarkGrey;
	}
	
	gds::Colour GetScrollbarBackground(){
		return constants::DarkGrey;
	}
	
	gds::Colour GetScrollbarButton(){
		return constants::Grey;
	}
	
	gds::Colour GetScrollbarHiLight(){
		return constants::White;
	}
	
	gds::Colour GetScrollbarLowLight(){
		return constants::DarkGrey;
	}
	
	gds::Colour GetScrollbarArrow(){
		return constants::Black;
	}
	
	gds::Colour GetScrollbarFocus(){
		return constants::DarkGrey;
	}
	
	gds::Colour GetTextAreaText(){
		return constants::Black;
	}
	
	gds::Colour GetTextAreaHiLight(){
		return constants::Grey;
	}
	
	gds::Colour GetTextAreaLowLight(){
		return constants::DarkGrey;
	}
	
	gds::Colour GetGroupBoxText(){
		return constants::Black;
	}
	
	gds::Colour GetListBoxText(){
		return constants::Black;
	}
	
	gds::Colour GetListBoxHiLight(){
		return constants::Grey;
	}
	
	gds::Colour GetListBoxLowLight(){
		return constants::DarkGrey;
	}
	
	gds::Colour GetDetailListText(){
		return constants::Black;
	}
	
	gds::Colour GetDetailListHiLight(){
		return constants::Grey;
	}
	
	gds::Colour GetDetailListLowLight(){
		return constants::DarkGrey;
	}
	
	gds::Colour GetDetailListHeader(){
		return constants::Grey;
	}
	
	gds::Colour GetIconViewText(){
		return constants::Black;
	}
	
	gds::Colour GetIconViewHiLight(){
		return constants::Grey;
	}
	
	gds::Colour GetIconViewLowLight(){
		return constants::DarkGrey;
	}
	
	gds::Colour GetTreeViewText(){
		return constants::Black;
	}
	
	gds::Colour GetTreeViewHiLight(){
		return constants::Grey;
	}
	
	gds::Colour GetTreeViewLowLight(){
		return constants::DarkGrey;
	}
	
	gds::Colour GetToolbarBackground(){
		return constants::Grey;
	}
	
	gds::Colour GetToolbarButtonColour(){
		return constants::Grey;
	}
	
	gds::Colour GetToolbarButtonHiLight(){
		return constants::White;
	}
	
	gds::Colour GetToolbarButtonLowLight(){
		return constants::DarkGrey;
	}
	
	gds::Colour GetToolbarButtonText(){
		return constants::Black;
	}
	
	gds::Colour GetStatusBarBackground(){
		return constants::Grey;
	}
	
	gds::Colour GetStatusBarText(){
		return constants::Black;
	}
	
	gds::Colour GetTabsUnselectedBackground(){
		return constants::Grey;
	}
	
	gds::Colour GetTabsSelectedBackground(){
		return constants::White;
	}
	
	gds::Colour GetTabsText(){
		return constants::Black;
	}
	
	gds::Colour GetTabsFocus(){
		return constants::DarkGrey;
	}
	
	gds::Colour GetResizeHandleBackground(){
		return constants::Grey;
	}
	
	gds::Colour GetResizeHandleForeground(){
		return constants::Black;
	}
	
	gds::Colour GetResizeHandleDown(){
		return constants::DarkGrey;
	}
	
	gds::Colour GetProgressBarHiLight(){
		return constants::Grey;
	}
	
	gds::Colour GetProgressBarLowLight(){
		return constants::DarkGrey;
	}
}

namespace fonts{
	gds::Font GetButtonFont(){
		return gds::Font::Get("Resagnicto", gds_FontStyle::Bold);
	}
	
	uint32_t GetButtonTextSize(){
		return 12;
	}
	
	gds::Font GetLabelFont(){
		return gds::Font::Get("Resagnicto", gds_FontStyle::Normal);
	}
	
	uint32_t GetLabelTextSize(){
		return 12;
	}
	
	gds::Font GetTextBoxFont(){
		return gds::Font::Get("DejaVu Sans", gds_FontStyle::Normal);
	}
	
	uint32_t GetTextBoxTextSize(){
		return 13;
	}
	
	gds::Font GetCheckboxFont(){
		return gds::Font::Get("Resagnicto", gds_FontStyle::Normal);
	}
	
	uint32_t GetCheckboxTextSize(){
		return 12;
	}
	
	gds::Font GetRadioButtonFont(){
		return gds::Font::Get("Resagnicto", gds_FontStyle::Normal);
	}
	
	uint32_t GetRadioButtonTextSize(){
		return 12;
	}
	
	gds::Font GetTextAreaFont(){
		return gds::Font::Get("DejaVu Sans Mono", gds_FontStyle::Normal);
	}
	
	uint32_t GetTextAreaTextSize(){
		return 13;
	}
	
	gds::Font GetGroupBoxFont(){
		return gds::Font::Get("Resagnicto", gds_FontStyle::Bold);
	}
	
	uint32_t GetGroupBoxTextSize(){
		return 12;
	}
	
	gds::Font GetListBoxFont(){
		return gds::Font::Get("Resagnicto", gds_FontStyle::Normal);
	}
	
	uint32_t GetListBoxTextSize(){
		return 12;
	}
	
	gds::Font GetDetailListFont(){
		return gds::Font::Get("Resagnicto", gds_FontStyle::Normal);
	}
	
	uint32_t GetDetailListTextSize(){
		return 12;
	}
	
	gds::Font GetIconViewFont(){
		return gds::Font::Get("Resagnicto", gds_FontStyle::Normal);
	}
	
	uint32_t GetIconViewTextSize(){
		return 10;
	}
	
	gds::Font GetTreeViewFont(){
		return gds::Font::Get("Resagnicto", gds_FontStyle::Normal);
	}
	
	uint32_t GetTreeViewTextSize(){
		return 12;
	}
	
	gds::Font GetToolbarButtonFont(){
		return gds::Font::Get("Resagnicto", gds_FontStyle::Normal);
	}
	
	uint32_t GetToolbarButtonTextSize(){
		return 10;
	}
	
	gds::Font GetStatusBarFont(){
		return gds::Font::Get("Resagnicto", gds_FontStyle::Normal);
	}
	
	uint32_t GetStatusBarTextSize(){
		return 12;
	}
	
	gds::Font GetTabsFont(){
		return gds::Font::Get("Resagnicto", gds_FontStyle::Normal);
	}
	
	uint32_t GetTabsTextSize(){
		return 12;
	}
}

namespace icons{
	static std::shared_ptr<gds::Surface> LoadIcon(const char *path){
		auto r = resc::Resc_LocalOpen(gui_resc_data, gui_resc_size);
		auto fd = resc::Resc_OpenResc(r, path);
		auto ret = GDS_LoadPNG(fd);
		close(fd);
		resc::Resc_Close(r);
		return std::make_shared<gds::Surface>(std::move(gds::Surface::Wrap(ret, true)));
	}
	
	std::shared_ptr<gds::Surface> GetTreeViewDefaultChildren(){
		static std::shared_ptr<gds::Surface> ret;
		if(!ret) ret = LoadIcon("tv_default_icon.png");
		return ret;
	}
	
	std::shared_ptr<gds::Surface> GetTreeViewDefaultNoChildren(){
		static std::shared_ptr<gds::Surface> ret;
		if(!ret) ret = LoadIcon("tv_default_nochildren.png");
		return ret;
	}
	
	std::shared_ptr<gds::Surface> GetTreeViewDefaultOpen(){
		static std::shared_ptr<gds::Surface> ret;
		if(!ret) ret = LoadIcon("tv_default_open.png");
		return ret;
	}
	
	std::shared_ptr<gds::Surface> GetMessageBoxDefault(){
		static std::shared_ptr<gds::Surface> ret;
		if(!ret) ret = LoadIcon("mb_default.png");
		return ret;
	}
	
	std::shared_ptr<gds::Surface> GetInputBoxDefault(){
		static std::shared_ptr<gds::Surface> ret;
		if(!ret) ret = LoadIcon("ib_default.png");
		return ret;
	}
}

static uint64_t lastClicktime = 0;

uint64_t GetDoubleClickTime(){
	return 500;
}

bool IsDoubleClick(){
	uint64_t now = bt_rtc_millis();
	bool ret = false;
	if((now - lastClicktime) < GetDoubleClickTime()) ret = true;
	lastClicktime = now;
	return ret;
}

}
}
