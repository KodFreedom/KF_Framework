//--------------------------------------------------------------------------------
//�@kf_Utility.cpp
//  Utility methods
//	�֗��֐�
//	Author : �����^(KodFreedom)
//--------------------------------------------------------------------------------
#include <Windows.h>
#include "kf_utility.h"
using namespace kodfreedom;

//--------------------------------------------------------------------------------
//  �����܂ŕ���������
//--------------------------------------------------------------------------------
int Utility::GetStringUntilToken(FILE* file_pointer, const string& token, string& buffer)
{
    char c;
    buffer.clear();
    while ((c = (char)fgetc(file_pointer)) != EOF)
    {
        for (int count = 0; count < (int)token.length(); ++count)
        {
            if (c == token.at(count))
            {
                return buffer.length();
            }
        }
        buffer += c;
    }
    return -1;
}

//--------------------------------------------------------------------------------
//  �����܂ŕ���������
//--------------------------------------------------------------------------------
int Utility::GetStringUntilToken(String& file, const String& token, String& buffer)
{
    buffer.clear();
    for (auto itr = file.begin(); itr != file.end();)
    {
        auto c = *itr;
        itr = file.erase(itr);
        for (int count = 0; count < (int)token.length(); ++count)
        {
            if (c == token.at(count))
            {
                return buffer.length();
            }
        }
        buffer += c;
    }
    return -1;
}

//--------------------------------------------------------------------------------
//  ��r���镶����܂ŕ���������
//--------------------------------------------------------------------------------
int Utility::GetStringUntilString(FILE* file_pointer, const string& compare, string& buffer)
{
    static string nullBuffer;
    if (nullBuffer.empty()) nullBuffer.resize(256);
    do
    {
        buffer = nullBuffer;
        fgets(&buffer[0], (int)buffer.capacity(), file_pointer);
        if (buffer.find(compare) != string::npos)
        {
            return buffer.length();
        }
    } while (buffer.find("END_SCRIPT") == string::npos);
    return -1;
}

//--------------------------------------------------------------------------------
//  ��������J�E���g����
//--------------------------------------------------------------------------------
int Utility::GetStringCount(FILE* file_pointer, const string& token, const string& compare)
{
    int count = 0;
    string buffer;
    while (GetStringUntilToken(file_pointer, token, buffer) >= 0)
    {
        if (buffer.compare(compare) == 0)
        {
            ++count;
        }
    }

    //File�̓��ɖ߂�
    fseek(file_pointer, 0, SEEK_SET);

    return count;
}

//--------------------------------------------------------------------------------
//  ��������J�E���g����
//--------------------------------------------------------------------------------
int Utility::GetStringCount(String& file, const String& token, const String& compare)
{
    int count = 0;
    String buffer;
    while (GetStringUntilToken(file, token, buffer) >= 0)
    {
        if (buffer.compare(compare) == 0)
        {
            ++count;
        }
    }
    return count;
}

//--------------------------------------------------------------------------------
//  �p�X���疼�O�ƃ^�C�v����͂���
//--------------------------------------------------------------------------------
FileInfo Utility::AnalyzeFilePath(const String& path)
{
    FileInfo info;
    auto copy = path;
    reverse(copy.begin(), copy.end());

    //�t�@�C���^�̎擾
    if (GetStringUntilToken(copy, L".", info.type) > 0)
    {
        reverse(info.type.begin(), info.type.end());
    }

    //�t�@�C�����̎擾
    GetStringUntilToken(copy, L"\\/", info.name);
    reverse(info.name.begin(), info.name.end());
    return info;
}

//--------------------------------------------------------------------------------
//  �t�H���_����t�@�C�������擾����
//--------------------------------------------------------------------------------
vector<String> Utility::GetFilesFromFolder(const String& path, const String& extension)
{
    HANDLE handle;
    WIN32_FIND_DATA data;
    vector<String> file_names;

    //�g���q�̐ݒ�
    String search_name = path + L"\\*." + extension;

    handle = FindFirstFile(search_name.c_str(), &data);

    if (handle == INVALID_HANDLE_VALUE)
    {
        // file not found
        return file_names;
    }

    do
    {
        if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {}
        else
        {
            file_names.push_back(data.cFileName);
        }
    } while (FindNextFile(handle, &data));
    FindClose(handle);
    file_names.shrink_to_fit();
    return file_names;
}

//--------------------------------------------------------------------------------
//  �v���W�F�N�g�̃p�X���擾����
//--------------------------------------------------------------------------------
String Utility::GetProjectPath(void)
{
    char path[MAX_PATH + 1];
    if (0 != GetModuleFileNameA(NULL, path, MAX_PATH))
    {// ���s�t�@�C���̊��S�p�X���擾
        char drive[MAX_PATH + 1]
            , directory[MAX_PATH + 1]
            , filename[MAX_PATH + 1]
            , extension[MAX_PATH + 1];
        //�p�X�����\���v�f�ɕ������܂�
        _splitpath_s(path, drive, directory, filename, extension);
        string projectpath = drive;
        projectpath += directory;
        return String(projectpath.begin(), projectpath.end());
    }
    throw runtime_error("error to get path");
    return String();
}

//--------------------------------------------------------------------------------
//  �e�L�X�g��u8�ɕύX
//  return : string
//--------------------------------------------------------------------------------
string Utility::ToUtf8(const String& text)
{
    string result;
    int length = WideCharToMultiByte(CP_UTF8, 0, text.c_str(), text.length(), NULL, 0, NULL, NULL);
    if (length > 0)
    {
        result.resize(length);
        WideCharToMultiByte(CP_UTF8, 0, text.c_str(), text.length(), &result[0], length, NULL, NULL);
    }
    return result;
}