//--------------------------------------------------------------------------------
//	プレイヤーステートマシン
//　player_state.h
//	Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "player_state.h"
#include "main.h"
#include "main_system.h"
#include "input.h"
#include "camera_manager.h"
#include "camera.h"
#include "actor_controller.h"

//--------------------------------------------------------------------------------
//  操作更新
//--------------------------------------------------------------------------------
void PlayerState::UpdateInput(ActorController& actor)
{
	auto input = MainSystem::Instance()->GetInput();
	auto& axis = Vector2(input->MoveHorizontal(), input->MoveVertical());
	auto camera = MainSystem::Instance()->GetCameraManager()->GetMainCamera();
	auto& camera_forward = Vector3::Scale(camera->GetWorldForward(), Vector3(1.0f, 0.0f, 1.0f)).Normalized();
	auto& movement = (camera->GetWorldRight() * axis.x_ + camera_forward * axis.y_).Normalized();
	actor.SetMovement(movement);
	actor.SetIsAttack(input->GetKeyTrigger(Key::kAttack));
	actor.SetIsJump(input->GetKeyTrigger(Key::kJump));
}