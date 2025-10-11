#pragma once
#include "BaseDrawManager.h"

class GameMain
{
private:
	BaseDrawManager* m_DrawManager = nullptr;

public:
	void Init(BaseDrawManager* _drawManager);
};

