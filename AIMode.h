//--------------------------------------------------------------------------------
//	AI���[�h
//�@AIMode.h
//	Author : Xu Wenjie
//	Date   : 2017-07-17
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
//  �C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
//  �O���錾
//--------------------------------------------------------------------------------
class CEnemyBehaviorComponent;

//--------------------------------------------------------------------------------
//  �N���X
//--------------------------------------------------------------------------------
class CAIMode
{
public:
	CAIMode() {}
	virtual ~CAIMode() {}
	virtual void Update(CEnemyBehaviorComponent& enemy) = 0;
};