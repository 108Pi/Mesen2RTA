#include "pch.h"
#include "Shared/Video/SystemHud.h"
#include "Shared/Video/DebugHud.h"
#include "Shared/Movies/MovieManager.h"
#include "Shared/MessageManager.h"
#include "Shared/BaseControlManager.h"
#include "Shared/Video/DrawStringCommand.h"
#include "Shared/Interfaces/IMessageManager.h"
#include "Shared/EmuSettings.h"

SystemHud::SystemHud(Emulator* emu)
{
	_emu = emu;
	MessageManager::RegisterMessageManager(this);
}

SystemHud::~SystemHud()
{
	MessageManager::UnregisterMessageManager(this);
}

void SystemHud::Draw(DebugHud* hud, uint32_t width, uint32_t height) const
{
	DrawCounters(hud, width);
	DrawMessages(hud, width, height);

	if(_emu->IsRunning()) {
		EmuSettings* settings = _emu->GetSettings();
		int xOffset = 0;
		if(_emu->IsPaused()) {
			DrawPauseIcon(hud);
		} else if(_emu->GetMovieManager()->Playing()) {
			DrawPlayIcon(hud);
			xOffset += 12;
		} else if(_emu->GetMovieManager()->Recording()) {
			DrawRecordIcon(hud);
			xOffset += 12;
		}
		if(_emu->GetRunTimer()->IsValid()) {
			DrawRunTimer(hud, _emu->GetRunTimer()->GetTime(), _emu->GetFps(), height);
		}
		if(!_emu->IsPaused()) {
			int emuSpeed = settings->GetEmulationSpeed();
			EmulationConfig cfg = settings->GetEmulationConfig();
			if(settings->CheckFlag(EmulationFlags::Turbo) || emuSpeed > 100 || emuSpeed == 0) {
				DrawTurboRewindIcon(hud, false, xOffset);
			} else if(settings->CheckFlag(EmulationFlags::Rewind) || emuSpeed < 100) {
				DrawTurboRewindIcon(hud, true, xOffset);
			}
		}
	}
}

void SystemHud::DrawMessage(DebugHud* hud, MessageInfo& msg, uint32_t screenWidth, uint32_t screenHeight, int& lastHeight) const
{
	//Get opacity for fade in/out effect
	uint8_t opacity = (uint8_t)(msg.GetOpacity() * 255);
	int textRightMargin = 4;

	string text = "[" + msg.GetTitle() + "] " + msg.GetMessage();

	int maxWidth = screenWidth - textRightMargin;
	TextSize size = DrawStringCommand::MeasureString(text, maxWidth);
	lastHeight += size.Y;
	DrawString(hud, screenWidth, text, screenWidth - textRightMargin - size.X, screenHeight - lastHeight, opacity);
}

void SystemHud::DrawString(DebugHud* hud, uint32_t screenWidth, string text, int x, int y, uint8_t opacity) const
{
	int maxWidth = screenWidth - x;
	opacity = 255 - opacity;
	for(int i = -1; i <= 1; i++) {
		for(int j = -1; j <= 1; j++) {
			hud->DrawString(x + i, y + j, text, 0 | (opacity << 24), 0xFF000000, 1, -1, maxWidth, true);
		}
	}
	hud->DrawString(x, y, text, 0xFFFFFF | (opacity << 24), 0xFF000000, 1, -1, maxWidth, true);
}

void SystemHud::ShowFpsCounter(DebugHud* hud, uint32_t screenWidth, int lineNumber) const
{
	int yPos = 10 + 10 * lineNumber;

	string fpsString = string("FPS: ") + std::to_string(_currentFPS); // +" / " + std::to_string(_currentRenderedFPS);
	uint32_t length = DrawStringCommand::MeasureString(fpsString).X;
	DrawString(hud, screenWidth, fpsString, screenWidth - 8 - length, yPos);
}

void SystemHud::ShowRunAhead(DebugHud* hud, uint32_t screenWidth, int lineNumber) const
{
	int yPos = 10 + 10 * lineNumber;

	int raf = _emu->GetSettings()->GetEmulationConfig().RunAheadFrames;
	string rafString = string("RA: ") + std::to_string(raf);
	uint32_t length = DrawStringCommand::MeasureString(rafString).X;
	DrawString(hud, screenWidth, rafString, screenWidth - 8 - length, yPos);
}

void SystemHud::ShowGameTimer(DebugHud* hud, uint32_t screenWidth, int lineNumber) const
{
	int yPos = 10 + 10 * lineNumber;
	uint32_t frameCount = _emu->GetFrameCount();
	double frameRate = _emu->GetFps();
	uint32_t seconds = (uint32_t)(frameCount / frameRate) % 60;
	uint32_t minutes = (uint32_t)(frameCount / frameRate / 60) % 60;
	uint32_t hours = (uint32_t)(frameCount / frameRate / 3600);

	std::stringstream ss;
	ss << std::setw(2) << std::setfill('0') << hours << ":";
	ss << std::setw(2) << std::setfill('0') << minutes << ":";
	ss << std::setw(2) << std::setfill('0') << seconds;

	string text = ss.str();
	uint32_t length = DrawStringCommand::MeasureString(text).X;
	DrawString(hud, screenWidth, ss.str(), screenWidth - 8 - length, yPos);
}

void SystemHud::ShowFrameCounter(DebugHud* hud, uint32_t screenWidth, int lineNumber) const
{
	int yPos = 10 + 10 * lineNumber;
	uint32_t frameCount = _emu->GetFrameCount();

	string frameCounter = MessageManager::Localize("Frame") + ": " + std::to_string(frameCount);
	uint32_t length = DrawStringCommand::MeasureString(frameCounter).X;
	DrawString(hud, screenWidth, frameCounter, screenWidth - 8 - length, yPos);
}

void SystemHud::ShowLagCounter(DebugHud* hud, uint32_t screenWidth, int lineNumber) const
{
	int yPos = 10 + 10 * lineNumber;
	uint32_t count = _emu->GetLagCounter();

	string lagCounter = MessageManager::Localize("Lag") + ": " + std::to_string(count);
	uint32_t length = DrawStringCommand::MeasureString(lagCounter).X;
	DrawString(hud, screenWidth, lagCounter, screenWidth - 8 - length, yPos);
}

void SystemHud::DrawCounters(DebugHud* hud, uint32_t screenWidth) const
{
	int lineNumber = 0;
	PreferencesConfig cfg = _emu->GetSettings()->GetPreferences();

	if(_emu->IsRunning()) {
		if(cfg.ShowFps) {
			ShowFpsCounter(hud, screenWidth, lineNumber++);
		}
		if(cfg.ShowGameTimer) {
			ShowGameTimer(hud, screenWidth, lineNumber++);
		}
		if(cfg.ShowFrameCounter) {
			ShowFrameCounter(hud, screenWidth, lineNumber++);
		}
		if(cfg.ShowLagCounter) {
			ShowLagCounter(hud, screenWidth, lineNumber++);
		}
		if(_emu->GetSettings()->GetEmulationConfig().RunAheadFrames != 0) {
			ShowRunAhead(hud, screenWidth, lineNumber++);
		}
	}
}

void SystemHud::DisplayMessage(string title, string message)
{
	auto lock = _msgLock.AcquireSafe();
	_messages.push_front(std::make_unique<MessageInfo>(title, message, 1000));
}

void SystemHud::DrawMessages(DebugHud* hud, uint32_t screenWidth, uint32_t screenHeight) const
{
	int counter = 0;
	int lastHeight = 3;
	for(auto& msg : _messages) {
		if(counter < 3) {
			DrawMessage(hud, *msg.get(), screenWidth, screenHeight, lastHeight);
		} else {
			break;
		}
		counter++;
	}
}

void SystemHud::DrawBar(DebugHud* hud, int x, int y, int width, int height) const
{
	hud->DrawRectangle(x, y, width, height, 0xFFFFFF, true, 1);
	hud->DrawLine(x, y + 1, x + width, y + 1, 0x4FBECE, 1);
	hud->DrawLine(x + 1, y, x + 1, y + height, 0x4FBECE, 1);

	hud->DrawLine(x + width - 1, y, x + width - 1, y + height, 0xCC9E22, 1);
	hud->DrawLine(x, y + height - 1, x + width, y + height - 1, 0xCC9E22, 1);

	hud->DrawLine(x, y, x + width, y, 0x303030, 1);
	hud->DrawLine(x, y, x, y + height, 0x303030, 1);

	hud->DrawLine(x + width, y, x + width, y + height, 0x303030, 1);
	hud->DrawLine(x, y + height, x + width, y + height, 0x303030, 1);
}

void SystemHud::DrawPauseIcon(DebugHud* hud) const
{
	DrawBar(hud, 10, 7, 5, 12);
	DrawBar(hud, 17, 7, 5, 12);
}

void SystemHud::DrawPlayIcon(DebugHud* hud) const
{
	int x = 12;
	int y = 12;
	int width = 5;
	int height = 8;
	int borderColor = 0x00000;
	int color = 0xFFFFFF;

	for(int i = 0; i < width; i++) {
		int left = x + i * 2;
		int top = y + i;
		hud->DrawLine(left, top - 1, left, y + height - i + 1, borderColor, 1);
		hud->DrawLine(left + 1, top - 1, left + 1, y + height - i + 1, borderColor, 1);

		if(i > 0) {
			hud->DrawLine(left, top, left, y + height - i, color, 1);
		}

		if(i < width - 1) {
			hud->DrawLine(left + 1, top, left + 1, y + height - i, color, 1);
		}
	}
}

void SystemHud::DrawRecordIcon(DebugHud* hud) const
{
	int x = 12;
	int y = 11;
	int borderColor = 0x00000;
	int color = 0xFF0000;

	hud->DrawRectangle(x + 3, y, 4, 10, borderColor, true, 1);
	hud->DrawRectangle(x, y + 3, 10, 4, borderColor, true, 1);
	hud->DrawRectangle(x + 2, y + 1, 6, 8, borderColor, true, 1);
	hud->DrawRectangle(x + 1, y + 2, 8, 6, borderColor, true, 1);

	hud->DrawRectangle(x + 3, y + 1, 4, 8, color, true, 1);
	hud->DrawRectangle(x + 2, y + 2, 6, 6, color, true, 1);
	hud->DrawRectangle(x + 1, y + 3, 8, 4, color, true, 1);
}

void SystemHud::DrawTurboRewindIcon(DebugHud* hud, bool forRewind, int xOffset) const
{
	int x = 12 + xOffset;
	int y = 12;
	int width = 3;
	int height = 8;

	int frameId = (int)(_animationTimer.GetElapsedMS() / 75) % 16;
	if(frameId >= 8) {
		frameId = (~frameId & 0x07);
	}

	static constexpr uint32_t rewindColors[8] = { 0xFF8080, 0xFF9080, 0xFFA080, 0xFFB080, 0xFFC080, 0xFFD080, 0xFFE080, 0xFFF080 };
	static constexpr uint32_t turboColors[8] = { 0x80FF80, 0x90FF80, 0xA0FF80, 0xB0FF80, 0xC0FF80, 0xD0FF80, 0xE0FF80, 0xF0FF80 };

	int color;
	if(forRewind) {
		color = rewindColors[frameId];
		x += 5;
	} else {
		color = turboColors[frameId];
	}

	int borderColor = 0x333333;
	int sign = forRewind ? -1 : 1;

	for(int j = 0; j < 2; j++) {
		for(int i = 0; i < width; i++) {
			int left = x + i * sign * 2;
			int top = y + i * 2;
			hud->DrawLine(left, top - 2, left, y + height - i * 2 + 2, borderColor, 1);
			hud->DrawLine(left + 1 * sign, top - 1, left + 1 * sign, y + height - i * 2 + 1, borderColor, 1);

			if(i > 0) {
				hud->DrawLine(left, top - 1, left, y + height + 1 - i * 2, color, 1);
			}

			if(i < width - 1) {
				hud->DrawLine(left + 1 * sign, top, left + 1 * sign, y + height - i * 2, color, 1);
			}
		}

		x += 6;
	}
}

void SystemHud::UpdateHud()
{
	{
		auto lock = _msgLock.AcquireSafe();
		_messages.remove_if([](unique_ptr<MessageInfo>& msg) { return msg->IsExpired(); });
	}

	if(_emu->IsRunning()) {
		if(_fpsTimer.GetElapsedMS() > 1000) {
			//Update fps every sec
			uint32_t frameCount = _emu->GetFrameCount();
			if(_lastFrameCount > frameCount) {
				_currentFPS = 0;
			} else {
				_currentFPS = (int)(std::round((double)(frameCount - _lastFrameCount) / (_fpsTimer.GetElapsedMS() / 1000)));
				_currentRenderedFPS = (int)(std::round((double)(_renderedFrameCount - _lastRenderedFrameCount) / (_fpsTimer.GetElapsedMS() / 1000)));
			}
			_lastFrameCount = frameCount;
			_lastRenderedFrameCount = _renderedFrameCount;
			_fpsTimer.Reset();
		}

		if(_currentFPS > 5000) {
			_currentFPS = 0;
		}
		if(_currentRenderedFPS > 5000) {
			_currentRenderedFPS = 0;
		}

		_renderedFrameCount++;
	}
}

void SystemHud::DrawRunTimer(DebugHud* hud, int frameCount, double fps, int height) const
{
	double time = frameCount / fps;
	
	PreferencesConfig* Prefs = &(_emu->GetSettings()->GetPreferences());
	int startx = Prefs->TimerXPos;
	hud->DrawRectangle(startx, height - 11, 42, 9, 0x00000000, true, 1);
	hud->DrawRectangle(startx + 1, height - 10, 40, 7, 0x00A9A9A9, true, 1);
	int mins = (int)time / 60;
	double secs = time - mins * 60;
	secs = floor(secs * 1000.0 + 0.5) / 1000.0;

	DrawNumber(hud, mins % 10, startx + 7, height - 9);
	DrawNumber(hud, mins / 10 % 10, startx + 2, height - 9);
	hud->DrawPixel(startx + 12, height - 8, 0x00000000, 1);
	hud->DrawPixel(startx + 12, height - 6, 0x00000000, 1);
	DrawNumber(hud, int(secs) % 10, startx + 19, height - 9);
	DrawNumber(hud, int(secs) / 10 % 10, startx + 14, height - 9);
	hud->DrawPixel(startx + 24, height - 5, 0x00000000, 1);
	DrawNumber(hud, int(secs * 10) % 10, startx + 26, height - 9);
	DrawNumber(hud, int(secs * 100) % 10, startx + 31, height - 9);
	DrawNumber(hud, int(secs * 1000) % 10, startx + 36, height - 9);
}

void SystemHud::DrawNumber(DebugHud* _hud, int number, int x, int y) const
{
	int numColor = 0x00000000;
	if(number < 0) {
		numColor = 0x00BB1111;
		number *= -1;
	}
	int bgColor = 0x00A9A9A9;
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