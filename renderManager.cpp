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
							int listNo = calculateListNo(countAlpha, countFog, countFillMode, countSynthesis, countCullMode, countLighting);
							if (meshRenderers[listNo].empty()) continue;
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
							for (auto iterator = meshRenderers[listNo].begin(); iterator != meshRenderers[listNo].end();)
							{
								(*iterator)->Render();
								iterator = meshRenderers[listNo].erase(iterator);
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
	for (auto& renderers : meshRenderers)
	{
		renderers.clear();
	}
}

//--------------------------------------------------------------------------------
//  登録処理
//--------------------------------------------------------------------------------
void RenderManager::Register(MeshRenderer* renderer)
{
	int listNo = calculateListNo((int)renderer->alpha, (int)renderer->fog, (int)renderer->fillMode, (int)renderer->synthesis, (int)renderer->cullMode, (int)renderer->lighting);
	meshRenderers[listNo].push_back(renderer);
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

//--------------------------------------------------------------------------------
//  リスト番号の算出
//--------------------------------------------------------------------------------
int RenderManager::calculateListNo(const int alpha, const int fog, const int fillMode, const int synthesis, const int cullMode, const int lighting)
{
	return ((int)AlphaBase * alpha +
		(int)FogBase * fog +
		(int)FillModeBase * fillMode +
		(int)SynthesisBase * synthesis +
		(int)CullModeBase * cullMode +
		(int)LightingBase * lighting);
}