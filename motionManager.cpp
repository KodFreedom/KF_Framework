//--------------------------------------------------------------------------------
//
//　motionManager.cpp
//	Author : Xu Wenjie
//	Date   : 2017-11-06
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  インクルードファイル
//--------------------------------------------------------------------------------
#include "main.h"
#include "motionManager.h"
#include "motionInfo.h"

//--------------------------------------------------------------------------------
//  静的メンバー変数宣言
//--------------------------------------------------------------------------------
MotionManager* MotionManager::instance = nullptr;

//--------------------------------------------------------------------------------
//	
//  Public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  モーションをの読み込み
//--------------------------------------------------------------------------------
void MotionManager::Use(const string& motionName)
{
	if (motionName.empty()) return;

	//すでに読み込んだら処理終了
	auto iterator = motions.find(motionName);
	if (iterator != motions.end())
	{
		++iterator->second.UserNumber;
		return;
	}

	//モーションの読み込み
	//Todo:ファイルからモーションの読み込み
	MotionStruct info;
	info.UserNumber = 1;
	motions.emplace(motionName, info);
}

//--------------------------------------------------------------------------------
//  モーションをの破棄
//--------------------------------------------------------------------------------
void MotionManager::Disuse(const string& motionName)
{
	if (motionName.empty()) return;
	auto iterator = motions.find(motionName);
	if (iterator == motions.end()) return;
	--iterator->second.UserNumber;
	if (iterator->second.UserNumber == 0)
	{// 誰も使ってないので破棄する
		SAFE_DELETE(iterator->second.Pointer);
		motions.erase(iterator);
	}
}

//--------------------------------------------------------------------------------
//  モーションファイルの生成
//--------------------------------------------------------------------------------
void MotionManager::CreateMotionFileBy(const string& txtFilePath)
{
	FILE* filePointer;
	auto fileInfo = Utility::AnalyzeFilePath(txtFilePath);
	fopen_s(&filePointer, txtFilePath.c_str(), "r");
	if (!filePointer) return;

	//パーツ数の読み込み
	string buffer;
	
	string compare = "NUM_MODEL = ";
	int partsNumber = 0;
	if (Utility::GetStringUntilString(filePointer, compare, buffer) > 0)
	{
		char numberBuffer[16];
		for (int count = compare.length(), countNum = 0; buffer[count] != '\n'; ++count, ++countNum)
		{
			numberBuffer[countNum] = buffer[count];
		}
		partsNumber = atoi(numberBuffer);
	}

	//XFileの読み込み
	for (int countPart = 0; countPart < partsNumber; countPart++)
	{
		//XFILE XFile;
		compare = "MODEL_FILENAME = ";
		if (Utility::GetStringUntilString(filePointer, compare, buffer) > 0)
		{
			string modelPath;
			for (int count = compare.length(), countPath = 0; buffer[count] != '\t'; ++count, countPath++)
			{
				modelPath.push_back(buffer[count]);
			}
		}
	}

	/*
	//character情報の読み込み
	compare = "CHARACTERSET";
	if (Utility::GetStringUntilString(filePointer, compare, buffer))
	{
		//Move
		fgets(buffer, sizeof(buffer), filePointer);
		char strMove[8] = {};
		for (unsigned int count = strlen("\tMOVE = "), countMove = 0; buffer[count] != '\t'; ++count, countMove++)
		{
			strMove[countMove] = buffer[count];
		}
		//m_fMoveSpeed = (float)atof(strMove);

		//Jump
		fgets(buffer, sizeof(buffer), filePointer);
		char strJump[8] = {};
		for (unsigned int count = strlen("\tJUMP = "), countJump = 0; buffer[count] != '\t'; ++count, countJump++)
		{
			strJump[countJump] = buffer[count];
		}
		//m_fJumpSpeed = (float)atof(strJump);

		//Radius
		fgets(buffer, sizeof(buffer), filePointer);
		char strRadius[8] = {};
		for (unsigned int count = strlen("\tRADIUS = "), countRadius = 0; buffer[count] != '\t'; ++count, countRadius++)
		{
			strRadius[countRadius] = buffer[count];
		}
		//Radius = (float)atof(strRadius);
	}
	*/

	//parts infoの読み込み
	for (int countPart = 0; countPart < partsNumber; ++countPart)
	{
		if (Utility::GetStringUntilString(filePointer, "\tPARTSSET", buffer) > 0)
		{
			//Index
			fgets(&buffer[0], buffer.length(), filePointer);

			//Parent
			fgets(&buffer[0], buffer.length(), filePointer);
			char parentIDBuffer[4] = {};
			for (unsigned int count = strlen("\t\tPARENT = "), countParent = 0; buffer[count] != '\t'; ++count, ++countParent)
			{
				parentIDBuffer[countParent] = buffer[count];
			}
			int parentID = atoi(parentIDBuffer);

			//Pos
			fgets(&buffer[0], buffer.length(), filePointer);
			char positionBuffer[3][16] = {};
			int countAxis = 0;
			int countPos = 0;
			for (int count = strlen("\t\tPOS = "); buffer[count] != '\n'; ++count)
			{
				if (buffer[count] == ' ')
				{
					countAxis++;
					countPos = 0;
					continue;
				}
				positionBuffer[countAxis][countPos] = buffer[count];
				countPos++;
			}

			auto offsetPosition = Vector3((float)atof(positionBuffer[0]), (float)atof(positionBuffer[1]), (float)atof(positionBuffer[2]));

			//Rot
			fgets(&buffer[0], buffer.length(), filePointer);
			char rotationBuffer[3][16] = {};
			countAxis = 0;
			int countRot = 0;
			for (unsigned int count = strlen("\t\tROT = "); buffer[count] != '\n'; ++count)
			{
				if (buffer[count] == ' ')
				{
					countAxis++;
					countRot = 0;
					continue;
				}

				rotationBuffer[countAxis][countRot] = buffer[count];
				countRot++;
			}

			auto vOffsetRot = Vector3((float)atof(rotationBuffer[0]), (float)atof(rotationBuffer[1]), (float)atof(rotationBuffer[2]));
		}
	}

	//motionの読み込み
	int countMotion = 0;
	while (Utility::GetStringUntilString(filePointer, "MOTIONSET", buffer) > 0)
	{
		MotionInfo motionInfo;

		//LOOP
		if (Utility::GetStringUntilString(filePointer, "LOOP = ", buffer) > 0)
		{
			char loopBuffer[2] = {};
			for (int count = strlen("\tLOOP = "), countLoop = 0; buffer[count] != '\t'; ++count, ++countLoop)
			{
				loopBuffer[countLoop] = buffer[count];
			}
			motionInfo.IsLoop = atoi(loopBuffer) != 0 ? true : false;
		}
		
		//キーフレーム数取得
		int keyNumber = 0;
		if (Utility::GetStringUntilString(filePointer, "NUM_KEY = ", buffer) > 0)
		{
			char keyBuffer[2] = {};
			for (int count = strlen("\tNUM_KEY = "), countKey = 0; buffer[count] != '\t'; ++count, countKey++)
			{
				keyBuffer[countKey] = buffer[count];
			}
			keyNumber = atoi(keyBuffer);
		}
		
		vector<MotionFrame> keyFrames;
		vector<int> keyFrameNumbers;
		keyFrames.resize(keyNumber);
		keyFrameNumbers.resize(keyNumber);

		//キーフレームの読み込み
		for (int countKey = 0; countKey < keyNumber; ++countKey)
		{
			if (Utility::GetStringUntilString(filePointer, "KEYSET", buffer) > 0)
			{
				//Frame
				if (Utility::GetStringUntilString(filePointer, "FRAME = ", buffer) > 0)
				{
					char frameBuffer[4] = {};
					for (unsigned int count = strlen("\t\tFRAME = "), countFrame = 0; buffer[count] != '\n'; ++count, ++countFrame)
					{
						frameBuffer[countFrame] = buffer[count];
					}
					keyFrameNumbers[countKey] = atoi(frameBuffer);
				}
				
				//Set Key per Parts
				for (int countPart = 0; countPart < partsNumber; ++countPart)
				{
					Vector3 position;
					Quaternion rotation;

					//Pos
					if (Utility::GetStringUntilString(filePointer, "POS = ", buffer) > 0)
					{
						char positionBuffer[3][16] = {};
						int countAxis = 0;
						int countPos = 0;
						for (unsigned int count = strlen("\t\t\tPOS = "); buffer[count] != '\n'; ++count)
						{
							if (buffer[count] == ' ')
							{
								countAxis++;
								countPos = 0;
								continue;
							}

							positionBuffer[countAxis][countPos] = buffer[count];
							countPos++;
						}
						position = Vector3((float)atof(positionBuffer[0]), (float)atof(positionBuffer[1]), (float)atof(positionBuffer[2]));
					}

					//Rot
					if (Utility::GetStringUntilString(filePointer, "ROT = ", buffer) > 0)
					{
						char rotationBuffer[3][16] = {};
						int countAxis = 0;
						int countRot = 0;
						for (unsigned int count = strlen("\t\t\tROT = "); buffer[count] != '\n'; ++count)
						{
							if (buffer[count] == ' ')
							{
								countAxis++;
								countRot = 0;
								continue;
							}
							rotationBuffer[countAxis][countRot] = buffer[count];
							countRot++;
						}
						rotation = Vector3((float)atof(rotationBuffer[0]), (float)atof(rotationBuffer[1]), (float)atof(rotationBuffer[2])).ToQuaternion();
					}
					
					keyFrames[countKey].BoneFrames.push_back(BoneFrame(position, rotation));
				}
			}
		}

		//フレームに転換
		int countFrame = 0;
		for (int countKey = 0; countKey < keyNumber; ++countKey)
		{
			motionInfo.MotionFrames.resize(motionInfo.MotionFrames.size() + keyFrameNumbers[countKey]);
			for (int countKeyFrame = 0; countKeyFrame < keyFrameNumbers[countKey]; ++countKeyFrame)
			{
				float rate = (float)countKeyFrame / keyFrameNumbers[countKey];
				auto currentIterator = keyFrames[countKey].BoneFrames.begin();
				
				if (!motionInfo.IsLoop && countKey == keyNumber - 1)
				{// 最後のキーは補間なし
					for (int countBone = 0; countBone < partsNumber; ++countBone)
					{
						motionInfo.MotionFrames[countFrame].BoneFrames.push_back(BoneFrame(currentIterator->Position, currentIterator->Rotation));
						++currentIterator;
					}
				}
				else
				{// 補間
					auto nextIterator = keyFrames[(countKey + 1) % keyNumber].BoneFrames.begin();
					for (int countBone = 0; countBone < partsNumber; ++countBone)
					{
						auto& position = Math::Lerp(currentIterator->Position, nextIterator->Position, rate);
						auto& rotation = Math::Slerp(currentIterator->Rotation, nextIterator->Rotation, rate);
						motionInfo.MotionFrames[countFrame].BoneFrames.push_back(BoneFrame(position, rotation));
						++currentIterator;
						++nextIterator;
					}
				}
				++countFrame;
			}
		}

		// Save
		string motionName;
		switch (countMotion)
		{
		case 0:
			motionName = fileInfo.Name + "Neutral";
			break;
		case 1:
			motionName = fileInfo.Name + "Walk";
			break;
		case 2:
			motionName = fileInfo.Name + "Attack";
			break;
		case 3:
			motionName = fileInfo.Name + "Jump";
			break;
		case 4:
			motionName = fileInfo.Name + "Land";
			break;
		}
		string savePath = "data/MOTION/" + motionName + ".motion";
		FILE* saveFilePointer;

		/*
		fopen_s(&saveFilePointer, savePath.c_str(), "rb");
		MotionInfo test;
		if (saveFilePointer)
		{
			fread(&test.IsLoop, sizeof(bool), 1, saveFilePointer);
			int frameNumber = 0;
			fread(&frameNumber, sizeof(int), 1, saveFilePointer);
			test.MotionFrames.resize(frameNumber);
			for (int countFrame = 0; countFrame < frameNumber; ++countFrame)
			{
				int boneNumber = 0;
				fread(&boneNumber, sizeof(int), 1, saveFilePointer);
				for (int countBone = 0; countBone < boneNumber; ++countBone)
				{
					BoneFrame boneFrame;
					fread(&boneFrame, sizeof(BoneFrame), 1, saveFilePointer);
					test.MotionFrames[countFrame].BoneFrames.push_back(boneFrame);
				}
			}
			fclose(saveFilePointer);
		}
		*/

		fopen_s(&saveFilePointer, savePath.c_str(), "wb");
		if (saveFilePointer)
		{
			fwrite(&motionInfo.IsLoop, sizeof(bool), 1, saveFilePointer);
			int frameNumber = motionInfo.MotionFrames.size();
			fwrite(&frameNumber, sizeof(int), 1, saveFilePointer);
			for (int countFrame = 0; countFrame < frameNumber; ++countFrame)
			{
				int boneNumber = motionInfo.MotionFrames[countFrame].BoneFrames.size();
				fwrite(&boneNumber, sizeof(int), 1, saveFilePointer);
				for (auto& boneFrame : motionInfo.MotionFrames[countFrame].BoneFrames)
				{
					fwrite(&boneFrame, sizeof(BoneFrame), 1, saveFilePointer);
				}
			}
			fclose(saveFilePointer);
		}
		countMotion++;

		if (Utility::GetStringUntilString(filePointer, "END_MOTIONSET", buffer) < 0)
		{
			break;
		}
	}

	fclose(filePointer);
}

//--------------------------------------------------------------------------------
//	
//  Private
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  全てのモーションを破棄する
//--------------------------------------------------------------------------------
void MotionManager::uninit(void)
{
	for (auto iterator = motions.begin(); iterator != motions.end();)
	{
		SAFE_DELETE(iterator->second.Pointer);
		iterator = motions.erase(iterator);
	}
}
