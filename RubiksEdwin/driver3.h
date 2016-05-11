#pragma once
#include <memory.h>

void getInput(int /*face*/, int /*sticker*/, int /*direction*/, int *);
int getColor(int /*face*/, int /*sticker*/);
void rotate2Front(int);
void rotateback(void);
void move(int, int);
void rotate(int, bool);
void rotateSliver(bool, bool, int, int);
void PeelOffStickers();
