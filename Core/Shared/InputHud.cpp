#include "pch.h"
#include "Shared/InputHud.h"
#include "Shared/Emulator.h"
#include "Shared/BaseControlManager.h"
#include "Shared/BaseControlDevice.h"
#include "Shared/EmuSettings.h"
#include "Shared/Video/DebugHud.h"
#include "Shared/Video/VideoDecoder.h"
#include "Shared/Movies/MovieManager.h"
#include "Shared/CheatManager.h"
#include "Shared/HistoryViewer.h"

static constexpr int colors[4] = { 0x00111111, 0x00117111, 0x00711111 , 0x005F0072 };
static int bgColor = 0x00A9A9A9;
int prevLagCount = 0;

InputHud::InputHud(Emulator* emu, DebugHud* hud)
{
	_emu = emu;
	_hud = hud;
}

void InputHud::DrawButton(int x, int y, int width, int height, bool pressed, bool isTurbo)
{
	int color = pressed ? isTurbo ? 0x00D7FFFF : 0x00FFFFFF : colors[GetSettingColor()];
	_hud->DrawRectangle((_xOffset + x), (_yOffset + y), width, height, color, true, 1);
}

void InputHud::DrawNumber(int number, int x, int y)
{
	x += _xOffset;
	y += _yOffset;

	int numColor = colors[GetSettingColor()];
	if(_emu->GetConsole()->GetControlManager()->IsLagFrame()) {
		numColor = 0x00BB1111;
	}

	_hud->DrawRectangle(x, y, 4, 5, numColor, true, 1);
	switch(number) {
		case 0:
			_hud->DrawRectangle(x + 1, y + 1, 2, 3, bgColor, true, 1);
			break;

		case 1:
			_hud->DrawRectangle(x, y, 1, 5, bgColor, true, 1);
			_hud->DrawRectangle(x + 1, y + 1, 1, 3, bgColor, true, 1);
			_hud->DrawRectangle(x + 3, y, 1, 4, bgColor, true, 1);
			break;

		case 2:
			_hud->DrawRectangle(x, y + 1, 3, 1, bgColor, true, 1);
			_hud->DrawRectangle(x + 1, y + 3, 3, 1, bgColor, true, 1);
			break;

		case 3:
			_hud->DrawRectangle(x, y + 1, 3, 1, bgColor, true, 1);
			_hud->DrawRectangle(x, y + 3, 3, 1, bgColor, true, 1);
			break;

		case 4:
			_hud->DrawRectangle(x, y + 3, 3, 2, bgColor, true, 1);
			_hud->DrawRectangle(x + 1, y, 2, 2, bgColor, true, 1);
			break;

		case 5:
			_hud->DrawRectangle(x + 1, y + 1, 3, 1, bgColor, true, 1);
			_hud->DrawRectangle(x, y + 3, 3, 1, bgColor, true, 1);
			break;

		case 6:
			_hud->DrawRectangle(x + 1, y + 1, 3, 1, bgColor, true, 1);
			_hud->DrawRectangle(x + 1, y + 3, 2, 1, bgColor, true, 1);
			break;

		case 7:
			_hud->DrawRectangle(x, y + 2, 1, 3, bgColor, true, 1);
			_hud->DrawRectangle(x + 1, y + 1, 2, 4, bgColor, true, 1);
			break;

		case 8:
			_hud->DrawRectangle(x + 1, y + 3, 2, 1, bgColor, true, 1);
			_hud->DrawRectangle(x + 1, y + 1, 2, 1, bgColor, true, 1);
			break;

		case 9:
			_hud->DrawRectangle(x, y + 3, 3, 2, bgColor, true, 1);
			_hud->DrawRectangle(x + 1, y + 1, 2, 1, bgColor, true, 1);
			break;

		default:
			break;
	}
}

void InputHud::DrawMousePosition(MousePosition pos)
{
	if(pos.X >= 0 && pos.Y >= 0) {
		//These are drawn on the "debug"/"lua" HUD because its size always matches the console's output size
		//Drawing on _hud causes issues when the "fixed size" option is selected
		OverscanDimensions overscan = _emu->GetSettings()->GetOverscan();
		_emu->GetDebugHud()->DrawRectangle(pos.X - 1 - overscan.Left, pos.Y - 1 - overscan.Top, 3, 3, 0x00FF0000, true, 1);
		_emu->GetDebugHud()->DrawRectangle(pos.X - 1 - overscan.Left, pos.Y - 1 - overscan.Top, 3, 3, 0x00808080, false, 1);
	}
}

void InputHud::DrawOutline(int width, int height, int textX, int textY, int startX, int startY)
{
	InputConfig& cfg = _emu->GetSettings()->GetInputConfig();

	switch(cfg.DisplayInputPosition) {
		default:
		case InputDisplayPosition::TopLeft:
			break;

		case InputDisplayPosition::TopRight:
			_xOffset -= width + 1;
			startX *= -1;
			break;

		case InputDisplayPosition::BottomLeft:
			_yOffset -= height + 1;
			break;

		case InputDisplayPosition::BottomRight:
			_yOffset -= height + 1;
			_xOffset -= width + 1;
			startX *= -1;
			break;
	}

	int color = colors[GetSettingColor()];

	_hud->DrawRectangle(_xOffset + startX, _yOffset + startY, width, height, bgColor, true, 1);
	_hud->DrawRectangle(_xOffset + startX, _yOffset + startY, width, height, color, false, 1);
	_hud->DrawLine(_xOffset + width - textX + startX, _yOffset + startY, _xOffset + width - 1 + startX, _yOffset + startY, bgColor, 1);
	_hud->DrawLine(_xOffset + width - 1 + startX, _yOffset + startY, _xOffset + width - 1 + startX, _yOffset + textY - 1 + startY, bgColor, 1);

	_outlineWidth = width;
	_outlineHeight = height;
}

void InputHud::DrawController(ControllerData& data, BaseControlManager* controlManager)
{
	shared_ptr<BaseControlDevice> controller = controlManager->CreateControllerDevice(data.Type, data.Port);
	if(!controller) {
		return;
	}

	controller->SetRawState(data.State);
	controller->DrawController(*this);
}



void InputHud::EndDrawController()
{
	if(_outlineHeight > 0 && _outlineWidth > 0) {
		InputConfig& cfg = _emu->GetSettings()->GetInputConfig();

		switch(cfg.DisplayInputPosition) {
			default:
			case InputDisplayPosition::TopLeft:
				if(cfg.DisplayInputHorizontally) {
					_xOffset += _outlineWidth + 1;
				} else {
					_yOffset += _outlineHeight + 1;
				}
				break;

			case InputDisplayPosition::TopRight:
				if(!cfg.DisplayInputHorizontally) {
					_xOffset += _outlineWidth + 1;
					_yOffset += _outlineHeight + 1;
				}
				break;

			case InputDisplayPosition::BottomLeft:
				if(cfg.DisplayInputHorizontally) {
					_xOffset += _outlineWidth + 1;
					_yOffset += _outlineHeight + 1;
				}
				break;

			case InputDisplayPosition::BottomRight:
				if(cfg.DisplayInputHorizontally) {
					_yOffset += _outlineHeight + 1;
				} else {
					_xOffset += _outlineWidth + 1;
				}
				break;
		}

		_outlineWidth = 0;
		_outlineHeight = 0;
	}

	_controllerIndex++;
}

void InputHud::DrawControllers(FrameInfo size, vector<ControllerData> controllerData)
{
	if(_emu->GetAudioPlayerHud()) {
		//Don't draw controllers when playing an audio file
		return;
	}

	shared_ptr<IConsole> console = _emu->GetConsole();
	if(!console) {
		return;
	}

	InputConfig& cfg = _emu->GetSettings()->GetInputConfig();

	bool hasVisiblePort = false;
	for(int i = 0; i < 8; i++) {
		hasVisiblePort |= cfg.DisplayInputPort[i];
	}

	if(!hasVisiblePort) {
		return;
	}

	switch(cfg.DisplayInputPosition) {
		default:
		case InputDisplayPosition::TopLeft:
			_xOffset = 2;
			_yOffset = 2;
			break;
		case InputDisplayPosition::TopRight:
			_xOffset = size.Width - 1;
			_yOffset = 2;
			break;
		case InputDisplayPosition::BottomLeft:
			_xOffset = 2;
			_yOffset = size.Height - 1;
			break;
		case InputDisplayPosition::BottomRight:
			_xOffset = size.Width - 1;
			_yOffset = size.Height - 1;
			break;
	}

	_controllerIndex = 0;
	for(ControllerData& portData : controllerData) {
		DrawController(portData, console->GetControlManager());
	}

}

int InputHud::GetSettingColor()
{
	if(!_emu->GetHistoryViewer()->IsEndOfHistory()) {
		return 2;
	}

	if(_emu->IsDebugging()) {
		return 2;
	}

	if(_emu->GetMovieManager()->Playing()) {
		return 2;
	}

	if(_emu->GetIsUnclean()) {
		return 2;
	}

	CheatManager* cheats = _emu->GetCheatManager();
	if(cheats->HasCheats()) return 1;

	//check overclocking feature - specific to each console
	NesConfig& cfg_nes = _emu->GetSettings()->GetNesConfig();
	SnesConfig& cfg_snes = _emu->GetSettings()->GetSnesConfig();

	if(&cfg_nes != NULL) {
		if(cfg_nes.PpuExtraScanlinesBeforeNmi != 0 || cfg_nes.PpuExtraScanlinesAfterNmi != 0) {
			return 2;
		}
	}

	if(&cfg_snes != NULL) {
		if(cfg_snes.PpuExtraScanlinesBeforeNmi != 0 || cfg_snes.PpuExtraScanlinesAfterNmi != 0) {
			return 2;
		}

		if(cfg_snes.GsuClockSpeed != 100) {
			return 2;
		}
	}

	if(_emu->GetSettings()->GetVideoConfig().IntegerFpsMode) {
		return 2;
	}

	if(_emu->GetSettings()->GetEmulationConfig().RunAheadFrames != 0) {
		return 3;
	}

	return 0;
}
