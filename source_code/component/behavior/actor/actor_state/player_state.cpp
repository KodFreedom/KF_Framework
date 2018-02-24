//--------------------------------------------------------------------------------
//  プレイヤーステートマシン
//　player_state.h
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "player_state.h"
#include "../../../../main_system.h"
#include "../../../../input/input.h"
#include "../../../../camera/camera_manager.h"
#include "../../../../camera/camera.h"
#include "../player_controller.h"

//--------------------------------------------------------------------------------
//  操作更新
//--------------------------------------------------------------------------------
void PlayerState::UpdateInput(PlayerController& actor)
{
    auto& input = MainSystem::Instance().GetInput();
    Vector2 axis(input.MoveHorizontal(), input.MoveVertical());

    auto camera = MainSystem::Instance().GetCameraManager().GetMainCamera();
    Vector3& camera_forward = Vector3::Scale(camera->GetWorldForward(), Vector3(1.0f, 0.0f, 1.0f)).Normalized();
    Vector3& movement = camera->GetWorldRight() * axis.x_ + camera_forward * axis.y_;

    actor.SetMovement(movement);
    actor.SetIsLightAttack(input.GetKeyTrigger(Key::kLightAttack));
    actor.SetIsStrongAttack(input.GetKeyTrigger(Key::kStrongAttack));
    actor.SetIsSkill(input.GetKeyTrigger(Key::kSkill));
    actor.SetIsJump(input.GetKeyTrigger(Key::kJump));
    actor.SetIsDefence(input.GetKeyPress(Key::kBlock));
}