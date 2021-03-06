#define BTOS_NO_USING

#include "stdmode.hpp"
#include "calc.hpp"

#include <gui/button.hpp>

#include <dev/keyboard.h>

#include <cstdlib>
#include <exception>
#include <cmath>

namespace gui = btos_api::gui;
namespace gds = btos_api::gds;
namespace wm = btos_api::wm;

void StandardMode::UpdateOp(){
	switch(opState){
		case OpState::Default:
			curOp->SetText("=");
			break;
		case OpState::Add:
			curOp->SetText("+");
			break;
		case OpState::Subtract:
			curOp->SetText("-");
			break;
		case OpState::Multiply:
			curOp->SetText("\xC3\x97");
			break;
		case OpState::Divide:
			curOp->SetText("\xC3\xB7");
			break;
	}
}

double StandardMode::Calculate(){
	auto value = GetCurrentValue();
	double ret = -1.0;
	switch(opState){
		case OpState::Default:
			ret = value;
			break;
		case OpState::Add:
			ret = curTotal + value;
			break;
		case OpState::Subtract:
			ret = curTotal - value;
			break;
		case OpState::Multiply:
			ret = curTotal * value;
			break;
		case OpState::Divide:
			ret = curTotal / value;
			break;
		default:
			throw std::exception();
	}
	curValue = ret;
	curValueValid = true;
	return ret;
}

void StandardMode::HandleDecimal(){
	auto text = awaiting ? "" : output->GetText();
	if(text.find('.') == std::string::npos){
		text += '.';
		output->SetText(text);
		awaiting = false;
		curValueValid = false;
	}else if(text.empty()){
		output->SetText("0.");
		awaiting = false;
		curValueValid = false;
	}
}

void StandardMode::HandleClear(){
	opState = OpState::Default;
	UpdateOp();
	curTotal = 0;
	output->SetText("");
	curValueValid = false;
}

void StandardMode::HandleClearEntry(){
	output->SetText("");
	awaiting = true;
	curValueValid = false;
}

void StandardMode::HandleSqrt(){
	curTotal = std::sqrt(GetCurrentValue());
	output->SetText(tfm::format("%s", curTotal));
	opState = OpState::Default;
	curValue = curTotal;
	curValueValid = true;
	UpdateOp();
}

void StandardMode::HandlePercent(){
	double result = curTotal * (GetCurrentValue() / 100.0);
	output->SetText(tfm::format("%s", result));
	curValue = result;
	curValueValid = true;
	awaiting = true;
}

bool StandardMode::HandleKeyPress(uint32_t key){
	if(!(key & KeyFlags::NonASCII)){
		char c = KB_char(key);
		c = tolower(c);
		switch(c){
			case '0':
				HandleDigit<0>();
				break;
			case '1':
				HandleDigit<1>();
				break;
			case '2':
				HandleDigit<2>();
				break;
			case '3':
				HandleDigit<3>();
				break;
			case '4':
				HandleDigit<4>();
				break;
			case '5':
				HandleDigit<5>();
				break;
			case '6':
				HandleDigit<6>();
				break;
			case '7':
				HandleDigit<7>();
				break;
			case '8':
				HandleDigit<8>();
				break;
			case '9':
				HandleDigit<9>();
				break;
			case '.':
				HandleDecimal();
				break;
			
			case '+':
				HandleOp<OpState::Add>();
				break;
			case '-':
				HandleOp<OpState::Subtract>();
				break;
			case '*':
				HandleOp<OpState::Multiply>();
				break;
			case '/':
				HandleOp<OpState::Divide>();
				break;
			case '=':
			case '\n':
				HandleOp<OpState::Equals>();
				break;
			
			case 'r':
			case 's':
				HandleSqrt();
				break;
			case '%':
			case 'p':
				HandlePercent();
				break;
			
			
			case 'c':
				HandleClear();
				break;
			case 'e':
			case 0x08:
				HandleClearEntry();
				break;
		}
	}
	return true;
}

double StandardMode::GetCurrentValue(){
	if(curValueValid) return curValue;
	else return strtod(output->GetText().c_str(), nullptr);
}

StandardMode::StandardMode(const gds::Point &p) : pos(p) {}

std::shared_ptr<gui::Form> StandardMode::Show(){
	using namespace std::placeholders;
	
	auto form = std::make_shared<gui::Form>(gds::Rect{pos.x, pos.y, 210, 237}, gui::FormOptions::Fixed, "Calculator");
	
	MakeToolbarAndMenu(form);
	
	curOp = std::make_shared<gui::Label>(gds::Rect{180, 42, 20, 25}, "=");
	output = std::make_shared<gui::TextBox>(gds::Rect{10, 42, 160, 25});
	output->OnKeyPress(std::bind(&StandardMode::HandleKeyPress, this, _1));
	
	auto opDivide = std::make_shared<gui::Button>(gds::Rect{170, 77, 30, 30}, "\xC3\xB7");
	opDivide->OnAction(std::bind(&StandardMode::HandleOp<OpState::Divide>, this));
	auto opMultiply = std::make_shared<gui::Button>(gds::Rect{170, 117, 30, 30}, "\xC3\x97");
	opMultiply->OnAction(std::bind(&StandardMode::HandleOp<OpState::Multiply>, this));
	auto opSubtract = std::make_shared<gui::Button>(gds::Rect{170, 157, 30, 30}, "-");
	opSubtract->OnAction(std::bind(&StandardMode::HandleOp<OpState::Subtract>, this));
	auto opAdd = std::make_shared<gui::Button>(gds::Rect{170, 197, 30, 30}, "+");
	opAdd->OnAction(std::bind(&StandardMode::HandleOp<OpState::Add>, this));
	
	auto clear = std::make_shared<gui::Button>(gds::Rect{10, 77, 30, 30}, "C");
	clear->OnAction(std::bind(&StandardMode::HandleClear, this));
	auto clearEntry = std::make_shared<gui::Button>(gds::Rect{10, 117, 30, 30}, "CE");
	clearEntry->OnAction(std::bind(&StandardMode::HandleClearEntry, this));
	auto opSqrt = std::make_shared<gui::Button>(gds::Rect{10, 157, 30, 30}, "\xE2\x88\x9A");
	opSqrt->OnAction(std::bind(&StandardMode::HandleSqrt, this));
	auto opPercent = std::make_shared<gui::Button>(gds::Rect{10, 197, 30, 30}, "%");
	opPercent->OnAction(std::bind(&StandardMode::HandlePercent, this));
	
	auto digit7 = std::make_shared<gui::Button>(gds::Rect{50, 77, 30, 30}, "7");
	digit7->OnAction(std::bind(&StandardMode::HandleDigit<7>, this));
	auto digit8 = std::make_shared<gui::Button>(gds::Rect{90, 77, 30, 30}, "8");
	digit8->OnAction(std::bind(&StandardMode::HandleDigit<8>, this));
	auto digit9 = std::make_shared<gui::Button>(gds::Rect{130, 77, 30, 30}, "9");
	digit9->OnAction(std::bind(&StandardMode::HandleDigit<9>, this));
	
	auto digit4 = std::make_shared<gui::Button>(gds::Rect{50, 117, 30, 30}, "4");
	digit4->OnAction(std::bind(&StandardMode::HandleDigit<4>, this));
	auto digit5 = std::make_shared<gui::Button>(gds::Rect{90, 117, 30, 30}, "5");
	digit5->OnAction(std::bind(&StandardMode::HandleDigit<5>, this));
	auto digit6 = std::make_shared<gui::Button>(gds::Rect{130, 117, 30, 30}, "6");
	digit6->OnAction(std::bind(&StandardMode::HandleDigit<6>, this));
	
	auto digit1 = std::make_shared<gui::Button>(gds::Rect{50, 157, 30, 30}, "1");
	digit1->OnAction(std::bind(&StandardMode::HandleDigit<1>, this));
	auto digit2 = std::make_shared<gui::Button>(gds::Rect{90, 157, 30, 30}, "2");
	digit2->OnAction(std::bind(&StandardMode::HandleDigit<2>, this));
	auto digit3 = std::make_shared<gui::Button>(gds::Rect{130, 157, 30, 30}, "3");
	digit3->OnAction(std::bind(&StandardMode::HandleDigit<3>, this));
	
	auto digit0 = std::make_shared<gui::Button>(gds::Rect{50, 197, 30, 30}, "0");
	digit0->OnAction(std::bind(&StandardMode::HandleDigit<0>, this));
	auto decimal = std::make_shared<gui::Button>(gds::Rect{90, 197, 30, 30}, ".");
	decimal->OnAction(std::bind(&StandardMode::HandleDecimal, this));
	auto opEquals = std::make_shared<gui::Button>(gds::Rect{130, 197, 30, 30}, "=");
	opEquals->OnAction(std::bind(&StandardMode::HandleOp<OpState::Equals>, this));
	
	form->AddControls({
		output, curOp, 
		opDivide, opMultiply, opSubtract, opAdd,
		clear, clearEntry, opSqrt, opPercent,
		digit7, digit8, digit9,
		digit4, digit5, digit6,
		digit1, digit2, digit3,
		digit0, decimal, opEquals,
	});
	form->SetFocus(output);
	
	return form;
}

std::string StandardMode::GetValue(){
	return output->GetValue();
}

void StandardMode::SetValue(const std::string &s){
	auto val = strtod(s.c_str(), nullptr);
	output->SetText(tfm::format("%f", val));
	curValue = val;
	curValueValid = true;
	awaiting = true;
}