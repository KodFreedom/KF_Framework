//--------------------------------------------------------------------------------
//�@render_system.cpp
//	�����_�[�V�X�e��
//	Author : �����^(KodFreedom)
//--------------------------------------------------------------------------------
#include "render_system.h"

//--------------------------------------------------------------------------------
//  �ÓI�����o�ϐ�
//--------------------------------------------------------------------------------
const Vector2 RenderSystem::kOffset2d = Vector2(
    static_cast<float>(SCREEN_WIDTH) * 0.5f,
    static_cast<float>(SCREEN_HEIGHT) * 0.5f);

const Matrix44 RenderSystem::kProjection2d = Matrix44(
    2.0f / static_cast<float>(SCREEN_WIDTH), 0.0f, 0.0f, 0.0f,
    0.0f, -2.0f / static_cast<float>(SCREEN_HEIGHT), 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    -1.0f, 1.0f, 0.0f, 1.0f);
