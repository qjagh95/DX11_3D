#include "../stdafx.h"
#include "RenderState.h"

JEONG_USING

JEONG::RenderState::RenderState()
	:m_RenderState(NULLPTR), m_OldRenderState(NULLPTR)
{
}

JEONG::RenderState::~RenderState()
{
	SAFE_RELEASE(m_RenderState);
	SAFE_RELEASE(m_OldRenderState);
}
