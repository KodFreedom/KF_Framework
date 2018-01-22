//--------------------------------------------------------------------------------
//　renderer_manager.h
//  render the registered renderers
//  レンダラー管理者
//  Author : 徐文杰(KodFreedom)
//--------------------------------------------------------------------------------
#pragma once
#include "common_setting.h"
#include "shader_enum.h"

//--------------------------------------------------------------------------------
//  列挙型定義
//--------------------------------------------------------------------------------
enum RenderPriority
{
    kDefaultPriority = 0,
    kUseAlphaTest,
    kUseDepthSort,
    k2d,
    k2dMask,
    kPriorityMax
};

//--------------------------------------------------------------------------------
//  前方宣言
//--------------------------------------------------------------------------------
class MeshRenderer;

//--------------------------------------------------------------------------------
//  クラス宣言
//--------------------------------------------------------------------------------
class RendererManager
{
public:
    //--------------------------------------------------------------------------------
    //  生成処理
    //  return : MaterialManager*
    //--------------------------------------------------------------------------------
    static RendererManager* Create(void)
    {
        auto instance = MY_NEW RendererManager;
        return instance;
    }

    //--------------------------------------------------------------------------------
    //  破棄処理
    //--------------------------------------------------------------------------------
    void Release(void) 
    {
        Clear();
        MY_DELETE this;
    }

    //--------------------------------------------------------------------------------
    //  更新処理
    //--------------------------------------------------------------------------------
    void Update(void);

    //--------------------------------------------------------------------------------
    //  描画処理
    //--------------------------------------------------------------------------------
    void Render(void);

    //--------------------------------------------------------------------------------
    //  保存してるrenderersを全部削除
    //--------------------------------------------------------------------------------
    void Clear(void)
    {
        for (auto& renderers_array : renderers_arrays_)
        {
            for (auto& renderers : renderers_array)
            {
                renderers.clear();
            }
        }
    }

    //--------------------------------------------------------------------------------
    //  rendererを登録する
    //  renderer : レンダラー
    //--------------------------------------------------------------------------------
    void Register(MeshRenderer* renderer);

private:
    //--------------------------------------------------------------------------------
    //  constructors and destructors
    //--------------------------------------------------------------------------------
    RendererManager() {}
    RendererManager(const RendererManager& value) {}
    RendererManager& operator=(const RendererManager& value) {}
    ~RendererManager() {}

    //--------------------------------------------------------------------------------
    //  変数定義
    //--------------------------------------------------------------------------------
    list<MeshRenderer*> renderers_arrays_[kPriorityMax][kShaderMax]; // rendererを保存するところ
};