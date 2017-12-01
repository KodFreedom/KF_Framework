//--------------------------------------------------------------------------------
//
//@mode.cpp
//	Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "mode.h"
#include "main_system.h"
#include "game_object_manager.h"
#include "physics_system.h"
#include "ui_system.h"
#include "renderer_manager.h"
#include "light_manager.h"
#include "sound_manager.h"
#include "camera_manager.h"

//--------------------------------------------------------------------------------
//  I—¹ˆ—
//--------------------------------------------------------------------------------
void Mode::Uninit(void)
{
	auto main_system = MainSystem::Instance();
	main_system->GetPhysicsSystem()->Clear();
	main_system->GetGameObjectManager()->Clear();
	main_system->GetUISystem()->Clear();
	main_system->GetRendererManager()->Clear();
	main_system->GetLightManager()->Clear();
	main_system->GetSoundManager()->StopAll();
	main_system->GetCameraManager()->Clear();
}