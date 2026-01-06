#pragma once

class DebugHud;
class Emulator;

extern bool smb1Running;
extern bool smb1Finished;

void DrawTimerHud(DebugHud* hud, Emulator* emu, int frames, int x, int y, int xOffset, int yOffset);
