#include "pch.h"
#include "TimerHud.h"
#include "Shared/Video/DebugHud.h"
#include "Shared/Emulator.h"
#include "Shared/BaseControlManager.h"
#include "Nes/NesConsole.h"

static void DrawHudDigit(DebugHud* hud, Emulator* emu, int number, int x, int y, int xOffset, int yOffset)
{
	static constexpr int colors[4] = { 0x00111111, 0x00117111, 0x00111111 , 0x005F0072 };
	static constexpr int bgColor = 0x00A9A9A9;

	x += xOffset;
	y += yOffset;

	int color = colors[0];
	if(emu->GetConsole()->GetControlManager()->IsLagFrame()) {
		color = 0x00BB1111;
	}

	hud->DrawRectangle(x, y, 4, 5, color, true, 1);

	switch(number) {
		case 0: hud->DrawRectangle(x + 1, y + 1, 2, 3, bgColor, true, 1); break;
		case 1:
			hud->DrawRectangle(x, y, 1, 5, bgColor, true, 1);
			hud->DrawRectangle(x + 1, y + 1, 1, 3, bgColor, true, 1);
			hud->DrawRectangle(x + 3, y, 1, 4, bgColor, true, 1);
			break;
		case 2:
			hud->DrawRectangle(x, y + 1, 3, 1, bgColor, true, 1);
			hud->DrawRectangle(x + 1, y + 3, 3, 1, bgColor, true, 1);
			break;
		case 3:
			hud->DrawRectangle(x, y + 1, 3, 1, bgColor, true, 1);
			hud->DrawRectangle(x, y + 3, 3, 1, bgColor, true, 1);
			break;
		case 4:
			hud->DrawRectangle(x, y + 3, 3, 2, bgColor, true, 1);
			hud->DrawRectangle(x + 1, y, 2, 2, bgColor, true, 1);
			break;
		case 5:
			hud->DrawRectangle(x + 1, y + 1, 3, 1, bgColor, true, 1);
			hud->DrawRectangle(x, y + 3, 3, 1, bgColor, true, 1);
			break;
		case 6:
			hud->DrawRectangle(x + 1, y + 1, 3, 1, bgColor, true, 1);
			hud->DrawRectangle(x + 1, y + 3, 2, 1, bgColor, true, 1);
			break;
		case 7:
			hud->DrawRectangle(x, y + 2, 1, 3, bgColor, true, 1);
			hud->DrawRectangle(x + 1, y + 1, 2, 4, bgColor, true, 1);
			break;
		case 8:
			hud->DrawRectangle(x + 1, y + 3, 2, 1, bgColor, true, 1);
			hud->DrawRectangle(x + 1, y + 1, 2, 1, bgColor, true, 1);
			break;
		case 9:
			hud->DrawRectangle(x, y + 3, 3, 2, bgColor, true, 1);
			hud->DrawRectangle(x + 1, y + 1, 2, 1, bgColor, true, 1);
			break;
	}
}

static void DrawHudSeparator(DebugHud* hud, int type, int x, int y, int xOffset, int yOffset)
{
	x += xOffset;
	y += yOffset;

	const int color = 0x00111111;

	if(type == 0) {
		hud->DrawRectangle(x + 0, y + 1, 2, 1, color, true, 1);
		hud->DrawRectangle(x + 0, y + 3, 2, 1, color, true, 1);
	} else {
		hud->DrawRectangle(x + 0, y + 4, 2, 1, color, true, 1);
	}
}

void DrawTimerHud(DebugHud* hud, Emulator* emu, int frames, int x, int y, int xOffset, int yOffset)
{

	const double NTSC_FPS = 60.0988138974405;
	double seconds = frames / NTSC_FPS;

	int mins = (int)(seconds / 60);
	int secs = (int)seconds % 60;
	int millis = (int)((seconds - (int)seconds) * 1000);

	int digits[] = {
		mins / 10, mins % 10,
		secs / 10, secs % 10,
		millis / 100, (millis / 10) % 10, millis % 10
	};

	int dx = 0;

	DrawHudDigit(hud, emu, digits[0], x + dx, y, xOffset, yOffset); dx += 5;
	DrawHudDigit(hud, emu, digits[1], x + dx, y, xOffset, yOffset); dx += 6;
	DrawHudSeparator(hud, 0, x + dx, y, xOffset, yOffset); dx += 4;
	DrawHudDigit(hud, emu, digits[2], x + dx, y, xOffset, yOffset); dx += 5;
	DrawHudDigit(hud, emu, digits[3], x + dx, y, xOffset, yOffset); dx += 6;
	DrawHudSeparator(hud, 1, x + dx, y, xOffset, yOffset); dx += 4;
	DrawHudDigit(hud, emu, digits[4], x + dx, y, xOffset, yOffset); dx += 5;
	DrawHudDigit(hud, emu, digits[5], x + dx, y, xOffset, yOffset); dx += 5;
	DrawHudDigit(hud, emu, digits[6], x + dx, y, xOffset, yOffset);
}
