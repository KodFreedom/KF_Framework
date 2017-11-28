//--------------------------------------------------------------------------------
//　motion_manager.cpp
//  manage the motiondatas' save, load
//	モーション管理者
//	Author : 徐文杰(KodFreedom)
//--------------------------------------------------------------------------------
#include "main.h"
#include "motion_manager.h"
#include "motion_data.h"

//--------------------------------------------------------------------------------
//	
//  Public
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  与えられた名前のモーションを使う
//--------------------------------------------------------------------------------
void MotionManager::Use(const String& motion_name)
{
	if (motion_name.empty()) return;

	//すでに読み込んだら処理終了
	auto iterator = motions.find(motion_name);
	if (iterator != motions.end())
	{
		++iterator->second.user_number;
		return;
	}

	//モーションの読み込み
	MotionInfo info;
	info.pointer = LoadFromFile(motion_name);
	motions.emplace(motion_name, info);
}

//--------------------------------------------------------------------------------
//  与えられた名前のモーションを使わない
//--------------------------------------------------------------------------------
void MotionManager::Disuse(const String& motion_name)
{
	if (motion_name.empty()) return;
	auto iterator = motions.find(motion_name);
	if (iterator == motions.end()) return;
	--iterator->second.user_number;
	if (iterator->second.user_number == 0)
	{// 誰も使ってないので破棄する
		SAFE_DELETE(iterator->second.pointer);
		motions.erase(iterator);
	}
}

//--------------------------------------------------------------------------------
//  モーションファイルの生成
//--------------------------------------------------------------------------------
/*
void MotionManager::CreateMotionFileBy(const string& txtFilePath)
{
FILE* filePointer = nullptr;
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

//Meshの読み込み

for (int countPart = 0; countPart < partsNumber; countPart++)
{
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

auto offsetRotation = Vector3((float)atof(rotationBuffer[0]), (float)atof(rotationBuffer[1]), (float)atof(rotationBuffer[2]));
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
string motion_name;
switch (countMotion)
{
case 0:
motion_name = fileInfo.Name + "Neutral";
break;
case 1:
motion_name = fileInfo.Name + "Walk";
break;
case 2:
motion_name = fileInfo.Name + "Attack";
break;
case 3:
motion_name = fileInfo.Name + "Jump";
break;
case 4:
motion_name = fileInfo.Name + "Land";
break;
}
string savePath = "data/MOTION/" + motion_name + ".motion";
FILE* saveFilePointer;

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
*/

//--------------------------------------------------------------------------------
//	
//  Private
//
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//  クリア処理
//--------------------------------------------------------------------------------
void MotionManager::Clear(void)
{
	for (auto iterator = motions.begin(); iterator != motions.end();)
	{
		SAFE_DELETE(iterator->second.pointer);
		iterator = motions.erase(iterator);
	}
}

//--------------------------------------------------------------------------------
//  ファイルから読み込む
//--------------------------------------------------------------------------------
MotionData* MotionManager::LoadFromFile(const String& motion_name)
{
	auto& path = L"data/motion/" + motion_name + L".motion";
	ifstream file(path);
	if (!file.is_open()) return nullptr;
	auto data = MY_NEW MotionData;
	BinaryInputArchive archive(file);
	archive.loadBinary(&data->is_loop_, sizeof(data->is_loop_));
	int frame_number = 0;
	archive.loadBinary(&frame_number, sizeof(frame_number));
	data->frames_.resize(frame_number);
	for (auto& frame : data->frames_)
	{
		int bone_number = 0;
		archive.loadBinary(&bone_number, sizeof(bone_number));
		frame.bone_transforms_.resize(bone_number);
		for (auto& bone_transform : frame.bone_transforms_)
		{
			archive.loadBinary(&bone_transform.translation_, sizeof(bone_transform.translation_));
			archive.loadBinary(&bone_transform.rotation_, sizeof(bone_transform.rotation_));
			archive.loadBinary(&bone_transform.scale_, sizeof(bone_transform.scale_));
		}
	}
	file.close();
	return data;
}