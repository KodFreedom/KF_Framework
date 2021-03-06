//--------------------------------------------------------------------------------
//  ゲームオブジェクト生成関数
//　gameObjectSpawner.cpp
//  Author : Xu Wenjie
//--------------------------------------------------------------------------------
#include "game_object_spawner.h"
#include "game_object.h"
#include "game_object_actor.h"
#include "../main_system.h"
#include "../resources/material_manager.h"
#include "../resources/mesh_manager.h"
#include "../render_system/renderer_manager.h"
#include "../component/renderer/mesh_renderer_2d.h"
#include "../component/renderer/mesh_renderer_3d.h"
#include "../component/renderer/mesh_renderer_3d_skin.h"
#include "../component/collider/sphere_collider.h"
#include "../component/collider/obb_collider.h"
#include "../component/collider/aabb_collider.h"
#include "../component/collider/field_collider.h"
#include "../component/rigidbody/rigidbody3D.h"
#include "../component/animator/animator.h"
#include "../component/behavior/actor/player_controller.h"
#include "../component/behavior/ui/player_ui_controller.h"
#include "../component/behavior/actor/enemy_controller.h"
#include "../component/behavior/ui/flash_button_controller.h"
#include "../component/behavior/ui/button_controller.h"
#include "../component/behavior/ui/enemy_ui_controller.h"
#include "../component/behavior/ui/scroll_2d_controller.h"
#include "../component/behavior/actor/actor_state/mutant/player/player_mutant_idle_state.h"
#include "../component/behavior/actor/actor_state/knight/player/player_knight_idle_state.h"
#include "../component/behavior/actor/actor_state/zombie/enemy/enemy_zombie_idle_state.h"
#include "../component/animator/motion_state/mutant/mutant_idle_motion_state.h"
#include "../component/animator/motion_state/knight/knight_idle_motion_state.h"
#include "../component/animator/motion_state/zombie/zombie_idle_motion_state.h"

#if defined(EDITOR)
#include "../component/behavior/editor/field_editor.h"
#include "../component/behavior/editor/model_editor.h"
#include "../component/behavior/editor/enemy_editor.h"
#include "../component/behavior/editor/editor_controller.h"
#endif // EDITOR

//--------------------------------------------------------------------------------
//
//  public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  SkyBox生成処理
//--------------------------------------------------------------------------------
GameObject* GameObjectSpawner::CreateSkyBox(const Vector3& position, const Vector3& rotation, const Vector3& scale)
{
    auto result = MY_NEW GameObject;

    //コンポネント
    auto renderer = MY_NEW MeshRenderer3d(*result);
    renderer->SetMesh(L"skyBox");
    renderer->SetShaderType(ShaderType::kNoLightNoFog);
    renderer->SetMaterial(L"sky");
    renderer->SetBoundingSpherePosition(Vector3::kZero);
    renderer->SetBoundingSphereRadius(1000.0f);
    result->AddRenderer(renderer);

    //パラメーター
    auto transform = result->GetTransform();;
    transform->SetPosition(position);
    transform->SetScale(scale);
    transform->SetRotation(rotation);

    //初期化
    result->Init();
    return result;
}

//--------------------------------------------------------------------------------
//  MeshField生成処理
//--------------------------------------------------------------------------------
GameObject* GameObjectSpawner::CreateField(const String& name)
{
    auto result = MY_NEW GameObject;

    //コンポネント
    String& field_name = name + L"Field";
    auto renderer = MY_NEW MeshRenderer3d(*result);
    renderer->SetMesh(name + L".mesh");
    renderer->SetMaterial(field_name);
    renderer->SetBoundingSpherePosition(Vector3::kZero);
    renderer->SetBoundingSphereRadius(1000.0f);
    result->AddRenderer(renderer);
    result->AddCollider(MY_NEW FieldCollider(*result, name));

    //初期化
    result->Init();
    return result;
}

//--------------------------------------------------------------------------------
//  Cube生成処理
//--------------------------------------------------------------------------------
GameObject* GameObjectSpawner::CreateCube(const Vector3& position, const Vector3& rotation, const Vector3& scale)
{
    auto result = MY_NEW GameObject;

    //コンポネント
    auto renderer = MY_NEW MeshRenderer3d(*result);
    renderer->SetMesh(L"cube");
    renderer->SetMaterial(L"cube");
    result->AddRenderer(renderer);
    result->AddCollider(MY_NEW ObbCollider(*result, kDynamic, scale * 0.5f));
    result->SetRigidbody(MY_NEW Rigidbody3D(*result));

    //パラメーター
    auto transform = result->GetTransform();
    transform->SetPosition(position);
    transform->SetScale(scale);
    transform->SetRotation(rotation);

    //初期化
    result->Init();
    return result;
}

//--------------------------------------------------------------------------------
//  XModel生成処理
//--------------------------------------------------------------------------------
GameObject* GameObjectSpawner::CreateXModel(const String& name, const Vector3& position, const Vector3& rotation, const Vector3& scale, const RenderPriority& render_priority)
{
    auto result = MY_NEW GameObject;

    //Name
    auto& file_info = Utility::AnalyzeFilePath(name);
    result->SetName(file_info.name);

    //コンポネント
    auto renderer = MY_NEW MeshRenderer3d(*result);
    renderer->SetMesh(name);
    renderer->SetMaterial(L"default");
    renderer->SetShaderType(ShaderType::kNoLightNoFog);
    renderer->SetRenderPriority(render_priority);
    result->AddRenderer(renderer);

    //パラメーター
    auto transform = result->GetTransform();
    transform->SetPosition(position);
    transform->SetScale(scale);
    transform->SetRotation(rotation);

    //初期化
    result->Init();
    return result;
}

//--------------------------------------------------------------------------------
//    Model生成処理
//--------------------------------------------------------------------------------
GameObject* GameObjectSpawner::CreateModel(const String& name, const Vector3& position, const Quaternion& rotation, const Vector3& scale)
{
    auto& file_info = Utility::AnalyzeFilePath(name);
    if (!file_info.type._Equal(L"model")) return nullptr;
    
    //Modelファイルの開く
    String path = L"data/model/" + name;
    ifstream file(path, ios::binary);
    if (!file.is_open())
    {
        assert(file.is_open());
        return nullptr;
    }
    BinaryInputArchive archive(file);
    auto result = CreateChildNode(nullptr, archive);

    //Name
    result->SetName(file_info.name);

    //Trans
    auto transform = result->GetTransform();
    transform->SetPosition(position);
    transform->SetScale(scale);
    transform->SetRotation(rotation);

    file.close();

    //初期化
    result->Init();
    return result;
}

//--------------------------------------------------------------------------------
//  BasicPolygon2d生成処理
//--------------------------------------------------------------------------------
GameObject* GameObjectSpawner::CreateBasicPolygon2d(const Vector3& scale, const Layer& layer, const String& material_name, const ShaderType& shader_type, const RenderPriority& render_priority, const float& rotation, const Vector3& position)
{
    auto result = MY_NEW GameObject(layer);

    //コンポネント
    auto renderer = MY_NEW MeshRenderer2d(*result, render_priority, shader_type);
    renderer->SetMesh(L"polygon2d");
    renderer->SetMaterial(material_name);
    result->AddRenderer(renderer);

    //パラメーター
    auto transform = result->GetTransform();
    transform->SetScale(scale);
    transform->SetRotation(Vector3(0.0f, 0.0f, rotation));
    transform->SetPosition(position);

    result->Init();
    return result;
}

//--------------------------------------------------------------------------------
//  ScrollPolygon2d生成処理
//--------------------------------------------------------------------------------
GameObject* GameObjectSpawner::CreateScrollPolygon2d(const Short2& pattern, const short frame_per_pattern, const Vector3& scale, const Layer& layer, const String& material_name, const ShaderType& shader_type, const RenderPriority& render_priority, const float& rotation, const Vector3& position)
{
    auto result = MY_NEW GameObject(layer);

    //コンポネント
    auto renderer = MY_NEW MeshRenderer2d(*result, render_priority, shader_type);
    renderer->SetMesh(L"polygon2d");
    renderer->SetMaterial(material_name);
    result->AddRenderer(renderer);
    result->AddBehavior(MY_NEW Scroll2dController(*result, pattern, frame_per_pattern));

    //パラメーター
    auto transform = result->GetTransform();
    transform->SetScale(scale);
    transform->SetRotation(Vector3(0.0f, 0.0f, rotation));
    transform->SetPosition(position);

    result->Init();
    return result;
}

//--------------------------------------------------------------------------------
//  FlashButton2d生成処理
//--------------------------------------------------------------------------------
GameObject* GameObjectSpawner::CreateFlashButton2d(const float flash_speed, const Vector3& scale, const Layer& layer, const String& material_name, const ShaderType& shader_type, const RenderPriority& render_priority, const float& rotation, const Vector3& position)
{
    auto result = MY_NEW GameObject(layer);

    //コンポネント
    auto renderer = MY_NEW MeshRenderer2d(*result, render_priority, shader_type);
    renderer->SetMesh(L"polygon2d");
    renderer->SetMaterial(material_name);
    result->AddRenderer(renderer);
    result->AddBehavior(MY_NEW FlashButtonController(*result, flash_speed));

    //パラメーター
    auto transform = result->GetTransform();
    transform->SetScale(scale);
    transform->SetRotation(Vector3(0.0f, 0.0f, rotation));
    transform->SetPosition(position);

    result->Init();
    return result;
}

//--------------------------------------------------------------------------------
//  Button2d生成処理
//--------------------------------------------------------------------------------
GameObject* GameObjectSpawner::CreateButton2d(const Vector3& scale, const Vector3& position, const String& material_name, const Layer& layer, const ShaderType& shader_type, const RenderPriority& render_priority, const float& rotation)
{
    auto result = MY_NEW GameObject(layer);

    //コンポネント
    result->AddBehavior(MY_NEW ButtonController(*result, material_name));
    auto renderer = MY_NEW MeshRenderer2d(*result, render_priority, shader_type);
    renderer->SetMesh(L"polygon2d");
    renderer->SetMaterial(material_name);
    result->AddRenderer(renderer);

    //パラメーター
    auto transform = result->GetTransform();
    transform->SetScale(scale);
    transform->SetRotation(Vector3(0.0f, 0.0f, rotation));
    transform->SetPosition(position);

    result->Init();
    return result;
}

//--------------------------------------------------------------------------------
//  3dGauge生成処理
//--------------------------------------------------------------------------------
GameObject* GameObjectSpawner::CreateGauge3d(const String& material_name, const ShaderType& shader_type)
{
    auto result = MY_NEW GameObject();

    //コンポネント
    auto renderer = MY_NEW MeshRenderer3d(*result);
    renderer->SetMesh(L"polygon3d");
    renderer->SetMaterial(material_name);
    renderer->SetShaderType(shader_type);
    result->AddRenderer(renderer);

    result->Init();
    return result;
}

//--------------------------------------------------------------------------------
//  EnemyUiController生成処理
//--------------------------------------------------------------------------------
GameObject* GameObjectSpawner::CreateEnemyUiController(void)
{
    auto result = MY_NEW GameObject();
    result->AddBehavior(MY_NEW EnemyUiController(*result));
    result->Init();
    return result;
}

//--------------------------------------------------------------------------------
//  Player生成処理
//--------------------------------------------------------------------------------
GameObjectActor* GameObjectSpawner::CreatePlayer(const String &name, const Vector3 &position, const Vector3 &rotation, const Vector3 &scale)
{
    auto result = MY_NEW GameObjectActor;
    auto transform = result->GetTransform();
    auto animator = result->GetAnimator();

    //Modelファイルの開く
    String path = L"data/model/actor/" + name + L".model";
    ifstream file(path, ios::binary);
    if (!file.is_open())
    {
        assert(file.is_open());
        return result;
    }
    BinaryInputArchive archive(file);
    CreateChildNode(transform, archive, animator);
    file.close();

    //Animator
    animator->SetAvatar(name);

    //コンポネント
    auto rigidbody = MY_NEW Rigidbody3D(*result);
    rigidbody->SetGravityMultiplier(4.0f);
    result->SetRigidbody(rigidbody);

    //Actor Controller
    auto actor_controller = MY_NEW PlayerController(*result, *rigidbody, *animator);
    actor_controller->GetParameter().SetMoveSpeed(10.0f);
    actor_controller->GetParameter().SetJumpSpeed(20.0f);
    actor_controller->GetParameter().SetMinTurnSpeed(kPi);
    actor_controller->GetParameter().SetMaxTurnSpeed(kPi * 2.0f);
    result->AddBehavior(actor_controller);

    //Nameによって初期状態の生成
    if (name._Equal(L"mutant"))
    {
        animator->Change(MY_NEW MutantIdleMotionState(0));
        actor_controller->Change(MY_NEW PlayerMutantIdleState);
    }
    else if (name._Equal(L"knight"))
    {
        animator->Change(MY_NEW KnightIdleMotionState(0));
        actor_controller->Change(MY_NEW PlayerKnightIdleState);
    }

    //UI
    result->AddBehavior(MY_NEW PlayerUiController(*result));

    //Collider
    auto collider = MY_NEW SphereCollider(*result, kDynamic, 0.6f);
    collider->SetOffset(Vector3(0.0f, 0.8f, 0.0f));
    result->AddCollider(collider);
    
    //Name and Tag
    result->SetName(name);
    result->SetTagToAllChildren(L"Player");

    //パラメーター
    transform->SetPosition(position);
    transform->SetScale(scale);
    transform->SetRotation(rotation);

    //初期化
    result->Init();

    return result;
}

//--------------------------------------------------------------------------------
//  Enemy生成処理
//--------------------------------------------------------------------------------
GameObjectActor* GameObjectSpawner::CreateEnemy(const String &name, const Vector3 &position, const Vector3 &rotation, const Vector3 &scale)
{
    auto result = MY_NEW GameObjectActor;
    auto transform = result->GetTransform();
    auto animator = result->GetAnimator();

    //Modelファイルの開く
    String path = L"data/model/actor/" + name + L".model";
    ifstream file(path, ios::binary);
    if (!file.is_open())
    {
        assert(file.is_open());
        return result;
    }
    BinaryInputArchive archive(file);
    CreateChildNode(transform, archive, animator);
    file.close();

    //Animator
    animator->SetAvatar(name);
    animator->SetEnableIK(false);
    animator->Change(MY_NEW ZombieIdleMotionState(0));

    //コンポネント
    auto rigidbody = MY_NEW Rigidbody3D(*result);
    rigidbody->SetGravityMultiplier(4.0f);
    result->SetRigidbody(rigidbody);

    //Actor Controller
    auto actor_controller = MY_NEW EnemyController(*result, *rigidbody, *animator);
    actor_controller->Change(MY_NEW EnemyZombieIdleState);
    result->AddBehavior(actor_controller);

    //Collider
    auto collider = MY_NEW SphereCollider(*result, kDynamic, 0.6f);
    collider->SetOffset(Vector3(0.0f, 0.6f, 0.0f));
    result->AddCollider(collider);

    //Name and Tag
    result->SetName(name);
    result->SetTag(L"Enemy");

    //パラメーター
    transform->SetPosition(position);
    transform->SetScale(scale);
    transform->SetRotation(rotation);

    //初期化
    result->Init();

    return result;
}

#if defined(EDITOR)
//--------------------------------------------------------------------------------
//  エディタの作成
//--------------------------------------------------------------------------------
GameObject* GameObjectSpawner::CreateEditor(void)
{
    // FieldEditor
    auto field = MY_NEW GameObject;
    auto field_editor = MY_NEW FieldEditor(*field);
    field->AddBehavior(field_editor);
    auto renderer = MY_NEW MeshRenderer3d(*field);
    renderer->SetMesh(L"field");
    renderer->SetMaterial(L"editorField");
    renderer->SetBoundingSphereRadius(10000.0f);
    field->AddRenderer(renderer);
    field->Init();

    // Main Controller
    auto result = MY_NEW GameObject;
    auto model_editor = MY_NEW ModelEditor(*result);
    result->AddBehavior(model_editor);
    auto enemy_editor = MY_NEW EnemyEditor(*result);
    result->AddBehavior(enemy_editor);
    auto editor_controller = MY_NEW EditorController(*result);
    editor_controller->SetFieldEditor(field_editor);
    editor_controller->SetModelEditor(model_editor);
    editor_controller->SetEnemyEditor(enemy_editor);
    result->AddBehavior(editor_controller);
    renderer = MY_NEW MeshRenderer3d(*result);
    renderer->SetMesh(L"data/model/target.x");
    renderer->SetMaterial(L"default");
    renderer->SetShaderType(ShaderType::kNoLightNoFog);
    result->AddRenderer(renderer);

    // 初期化
    result->Init();
    return result;
}
#endif // EDITOR

//--------------------------------------------------------------------------------
//
//  Private
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  モデルファイルからゲームオブジェクト作成
//--------------------------------------------------------------------------------
GameObject* GameObjectSpawner::CreateChildNode(Transform* parent, BinaryInputArchive& archive, Animator* animator)
{
    auto result = MY_NEW GameObject;

    //Node名
    int name_size;
    archive.loadBinary(&name_size, sizeof(name_size));
    string name;
    name.resize(name_size);
    archive.loadBinary(&name[0], name_size);
    result->SetName(String(name.begin(), name.end()));

    //Offset
    Vector3 position;
    Quaternion rotation;
    Vector3 scale;
    archive.loadBinary(&position, sizeof(position));
    archive.loadBinary(&rotation, sizeof(rotation));
    archive.loadBinary(&scale, sizeof(scale));
    auto transform = result->GetTransform();
    if (parent)
    {
        transform->RegisterParent(parent);
        transform->SetPosition(position);
        transform->SetRotation(rotation);
        transform->SetScale(scale);
    }

    //Collider
    int collider_number = 0;
    archive.loadBinary(&collider_number, sizeof(collider_number));
    for (int count = 0; count < collider_number; ++count)
    {
        int collider_type = 0;
        Vector3 collider_positon, collider_rotation, collider_scale;
        bool is_trigger;
        archive.loadBinary(&collider_type, sizeof(collider_type));
        archive.loadBinary(&collider_positon, sizeof(collider_positon));
        archive.loadBinary(&collider_rotation, sizeof(collider_rotation));
        archive.loadBinary(&collider_scale, sizeof(collider_scale));
        archive.loadBinary(&is_trigger, sizeof(is_trigger));

        Collider* collider = nullptr;
        switch (static_cast<ColliderType>(collider_type))
        {
        case kSphere:
            collider = MY_NEW SphereCollider(*result, kStatic, collider_scale.x_);
            break;
        case kAabb:
            collider = MY_NEW AabbCollider(*result, kStatic, collider_scale * 0.5f);
            break;
        case kObb:
            collider = MY_NEW ObbCollider(*result, kStatic, collider_scale * 0.5f);
            break;
        default:
            break;
        }

        if (!collider) continue;
        collider->SetTrigger(is_trigger);
        collider->SetOffset(collider_positon, collider_rotation);
        result->AddCollider(collider);

        if (is_trigger)
        {
            collider->Sleep();
        }
    }

    //Mesh
    int mesh_number = 0;
    archive.loadBinary(&mesh_number, sizeof(mesh_number));
    //if (mesh_number == 0)
    //{
    //    auto renderer = MY_NEW MeshRenderer3d(*result);
    //    renderer->SetMesh(L"cube");
    //    renderer->SetShaderType(kNoLightNoFog);
    //    result->AddRenderer(renderer);
    //}
    for (int count = 0; count < mesh_number; ++count)
    {
        //Mesh Name
        archive.loadBinary(&name_size, sizeof(name_size));
        name.resize(name_size);
        archive.loadBinary(&name[0], name_size);
        String mesh_name = String(name.begin(), name.end());

        //Material
        archive.loadBinary(&name_size, sizeof(name_size));
        name.resize(name_size);
        archive.loadBinary(&name[0], name_size);
        String material_name = String(name.begin(), name.end());

        //Render Priority
        RenderPriority priority;
        archive.loadBinary(&priority, sizeof(priority));

        //Shader Type
        ShaderType shader_type;
        archive.loadBinary(&shader_type, sizeof(shader_type));

        //Cast Shadow
        bool is_cast_shadow;
        archive.loadBinary(&is_cast_shadow, sizeof(is_cast_shadow));

        //Bounding Sphere
        Vector3 bounding_sphere_position;
        float bounding_sphere_radius;
        archive.loadBinary(&bounding_sphere_position, sizeof(bounding_sphere_position));
        archive.loadBinary(&bounding_sphere_radius, sizeof(bounding_sphere_radius));

        //Mesh type
        MeshType mesh_type;
        archive.loadBinary(&mesh_type, sizeof(mesh_type));

        //Check File Type
        if (mesh_type == k3dMesh)
        {//骨なし
            auto renderer = MY_NEW MeshRenderer3d(*result);
            renderer->SetMesh(mesh_name + L".mesh");
            renderer->SetMaterial(material_name);
            renderer->SetRenderPriority(priority);
            renderer->SetShaderType(shader_type);
            renderer->SetCastShadowFlag(is_cast_shadow);
            renderer->SetBoundingSpherePosition(bounding_sphere_position);
            renderer->SetBoundingSphereRadius(bounding_sphere_radius);
            result->AddRenderer(renderer);
        }
        else if (mesh_type == k3dSkin)
        {//ワンスキーンメッシュ
            assert(animator);
            auto renderer = MY_NEW MeshRenderer3dSkin(*animator, *result);
            renderer->SetMesh(mesh_name + L".skin");
            renderer->SetMaterial(material_name);
            renderer->SetRenderPriority(priority);
            renderer->SetShaderType(shader_type);
            renderer->SetCastShadowFlag(is_cast_shadow);
            renderer->SetBoundingSpherePosition(bounding_sphere_position);
            renderer->SetBoundingSphereRadius(bounding_sphere_radius);
            result->AddRenderer(renderer);
        }
        else
        {
            MessageBox(NULL, L"error mesh type!!", L"GameObjectSpawner::createChildNode", MB_OK | MB_ICONWARNING);
        }
    }

    //Child
    int child_number = 0;
    archive.loadBinary(&child_number, sizeof(child_number));
    for (int count = 0; count < child_number; ++count)
    {
        auto child = CreateChildNode(transform, archive, animator);
    }

    return result;
}