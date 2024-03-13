#pragma once
#include "../Core/Camera.h"
#include "GBuffer.h"

namespace Renderer
{
	void Init();
	void RenderFrame();

	inline GBuffer gbuffer;
}