//--------------------------------------------------------------------------------
//Å@mode.cpp
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "mode.h"
#include "../main_system.h"
#include "../game_object/game_object_manager.h"
#include "../physics/physics_system.h"
#include "../render_system/renderer_manager.h"
#include "../light/light_manager.h"
#include "../camera/camera_manager.h"
#include "../render_system/shadow_map_system.h"

//--------------------------------------------------------------------------------
//  èIóπèàóù
//--------------------------------------------------------------------------------
void Mode::Uninit(void)
{
    auto& main_system = MainSystem::Instance();
    main_system.GetPhysicsSystem().Clear();
    main_system.GetGameObjectManager().Clear();
    main_system.GetRendererManager().Clear();
    main_system.GetLightManager().Clear();
    main_system.GetCameraManager().Clear();
    main_system.GetShadowMapSystem().Clear();
}