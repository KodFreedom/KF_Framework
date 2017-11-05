//--------------------------------------------------------------------------------
//	描画用マネージャ
//　renderManager.h
//	Author : Xu Wenjie
//	Date   : 2017-09-20
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "main.h"
#include "renderManager.h"
#include "meshRenderer.h"
#include "renderer.h"

//--------------------------------------------------------------------------------
//
//  Public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  Update
//--------------------------------------------------------------------------------
void RenderManager::Update(void)
{
	//Todo : Z sort
}

//--------------------------------------------------------------------------------
//  Render
//--------------------------------------------------------------------------------
void RenderManager::Render(void)
{
	auto renderer = Renderer::Instance();
	int previousAlpha = -1;
	int previousFog = -1;
	int previousFillMode = -1;
	int previousSynthesis = -1;
	int previousCullMode = -1;
	int previousLighting = -1;
	for (int countAlpha = 0; countAlpha < Alpha::Max; ++countAlpha)
	{
		for (int countFog = 0; countFog < Fog::Max; ++countFog)
		{
			for (int countFillMode = 0; countFillMode < FillMode::Max; ++countFillMode)
			{
				for (int countSynthesis = 0; countSynthesis < Synthesis::Max; ++countSynthesis)
				{
					for (int countCullMode = 0; countCullMode < CullMode::Max; ++countCullMode)
					{
						for (int countLighting = 0; countLighting < Lighting::Max; ++countLighting)
						{
							auto& renderers = renderersArrays[countAlpha][countFog]
								[countFillMode][countSynthesis][countCullMode][countLighting];
							if (renderers.empty()) continue;
							if (previousAlpha != countAlpha)
							{
								previousAlpha = countAlpha;
								renderer->SetRenderState((Alpha)countAlpha);
							}
							if (previousFog != countFog)
							{
								previousFog = countFog;
								renderer->SetRenderState((Fog)countFog);
							}
							if (previousFillMode != countFillMode)
							{
								previousFillMode = countFillMode;
								renderer->SetRenderState((FillMode)countFillMode);
							}
							if (previousSynthesis != countSynthesis)
							{
								previousSynthesis = countSynthesis;
								renderer->SetRenderState((Synthesis)countSynthesis);
							}
							if (previousCullMode != countCullMode)
							{
								previousCullMode = countCullMode;
								renderer->SetRenderState((CullMode)countCullMode);
							}
							if (previousLighting != countLighting)
							{
								previousLighting = countLighting;
								renderer->SetRenderState((Lighting)countLighting);
							}
							for (auto iterator = renderers.begin(); iterator != renderers.end();)
							{
								(*iterator)->Render();
								iterator = renderers.erase(iterator);
							}
						}
					}
				}
			}
		}
	}
}

//--------------------------------------------------------------------------------
//  配列クリア処理
//--------------------------------------------------------------------------------
void RenderManager::Clear(void)
{
	for (auto& renderers : renderersArray)
	{
		renderers.clear();
	}
}

//--------------------------------------------------------------------------------
//  登録処理
//--------------------------------------------------------------------------------
void RenderManager::Register(MeshRenderer* renderer)
{
	renderersArrays[renderer->alpha][renderer->fog][renderer->fillMode]
		[renderer->synthesis][renderer->cullMode][renderer->lighting].push_back(renderer);
}

//--------------------------------------------------------------------------------
//
//  Private
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------
RenderManager::RenderManager()
{
	Clear();
}