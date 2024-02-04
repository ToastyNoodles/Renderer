#pragma once
#include "../Core/Camera.h"

namespace Renderer
{
	void Init();
	void RenderFrame();

	inline bool DrawLightObjects = true;
	inline bool DrawSkybox = true;
}