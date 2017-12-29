//--------------------------------------------------------------------------------
//　render_system.cpp
//	レンダーシステム
//	Author : 徐文杰(KodFreedom)
//--------------------------------------------------------------------------------
#include "render_system.h"

//--------------------------------------------------------------------------------
//  静的メンバ変数
//--------------------------------------------------------------------------------
const Vector2 RenderSystem::kOffset2d = Vector2(
    static_cast<float>(SCREEN_WIDTH) * 0.5f,
    static_cast<float>(SCREEN_HEIGHT) * 0.5f);

const Matrix44 RenderSystem::kProjection2d = Matrix44(
    2.0f / static_cast<float>(SCREEN_WIDTH), 0.0f, 0.0f, 0.0f,
    0.0f, -2.0f / static_cast<float>(SCREEN_HEIGHT), 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    -1.0f, 1.0f, 0.0f, 1.0f);
