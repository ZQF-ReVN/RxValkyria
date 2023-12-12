#include "RxPath.h"
#include "RxStr.h"
#include "RxFile.h"
#include "Platform/Platform.h"

#include <queue>
#include <stdexcept>
#include <Windows.h>


namespace Rut::RxPath
{
	size_t GetFileName(char* cpPath, size_t nBytes)
	{
		size_t ite_unit = nBytes - 1;
		size_t file_name_len = 0;
		char* file_name_ptr = nullptr;

		do
		{
			char tmp_unit = cpPath[ite_unit];

			if (tmp_unit == '/' || tmp_unit == '\\')
			{
				if ((ite_unit > 0) && ((uint8_t)cpPath[ite_unit - 1] > 0x7F)) { continue; } // check is dbcs char
				file_name_len = nBytes - (ite_unit + 1);
				file_name_ptr = cpPath + (ite_unit + 1);
				break;
			}
		} while ((--ite_unit) != -1);

		if (file_name_ptr)
		{
			file_name_len ? (void)(memcpy(cpPath, file_name_ptr, file_name_len * sizeof(char))) : (void)(0);
			cpPath[file_name_len] = '\0';
		}
		else
		{
			file_name_len = nBytes;
		}

		return file_name_len;
	}

	size_t GetFileName(wchar_t* wpPath, size_t nChar)
	{
		size_t ite_unit = nChar - 1;
		size_t file_name_len = 0;
		wchar_t* file_name_ptr = nullptr;

		do
		{
			wchar_t tmp_unit = wpPath[ite_unit];

			if (tmp_unit == L'/' || tmp_unit == L'\\')
			{
				file_name_len = nChar - (ite_unit + 1);
				file_name_ptr = wpPath + (ite_unit + 1);
				break;
			}
		} while ((--ite_unit) != -1);

		if (file_name_ptr)
		{
			file_name_len ? (void)(memcpy(wpPath, file_name_ptr, file_name_len * sizeof(wchar_t))) : (void)(0);
			wpPath[file_name_len] = L'\0';
		}
		else
		{
			file_name_len = nChar;
		}

		return file_name_len;
	}

	std::string GetFileName(std::string msPath)
	{
		size_t bytes = GetFileName(msPath.data(), msPath.size());
		msPath.resize(bytes);
		return msPath;
	}

	std::wstring GetFileName(std::wstring wsPath)
	{
		size_t chars = GetFileName(wsPath.data(), wsPath.size());
		wsPath.resize(chars);
		return wsPath;
	}


	size_t RemoveFileName(char* cpPath, size_t nBytes)
	{
		size_t ite_unit = nBytes - 1;

		do
		{
			char tmp_unit = cpPath[ite_unit];

			if (tmp_unit == '/' || tmp_unit == '\\')
			{
				if ((ite_unit > 0) && ((uint8_t)cpPath[ite_unit - 1] > 0x7F)) { continue; } // check is dbcs char
				cpPath[ite_unit + 1] = '\0';
				return ite_unit + 1;
			}
		} while ((--ite_unit) != -1);

		return nBytes;
	}

	size_t RemoveFileName(wchar_t* wpPath, size_t nChar)
	{
		size_t ite_unit = nChar - 1;

		do
		{
			wchar_t tmp_unit = wpPath[ite_unit];

			if (tmp_unit == L'/' || tmp_unit == L'\\')
			{
				wpPath[ite_unit + 1] = L'\0';
				return ite_unit + 1;
			}
		} while ((--ite_unit) != -1);

		return nChar;
	}

	std::string RemoveFileName(std::string msPath)
	{
		size_t bytes = RemoveFileName(msPath.data(), msPath.size());
		msPath.resize(bytes);
		return msPath;
	}

	std::wstring RemoveFileName(std::wstring msPath)
	{
		size_t chars = RemoveFileName(msPath.data(), msPath.size());
		msPath.resize(chars);
		return msPath;
	}


	// nBytes exclude null char
	size_t GetSuffix(char* cpPath, const size_t nBytes)
	{
		size_t ite_byte = nBytes - 1;

		do
		{
			char tmp_unit = cpPath[ite_byte];

			if (tmp_unit == '.')
			{
				if ((ite_byte > 0) && ((uint8_t)cpPath[ite_byte - 1] > 0x7F)) { continue; } // check is dbcs char
				size_t suffix_bytes = nBytes - ite_byte;
				memcpy(cpPath, cpPath + ite_byte, suffix_bytes * sizeof(char));
				cpPath[suffix_bytes] = '\0';
				return suffix_bytes;
			}
			else if (tmp_unit == '/')
			{
				if ((ite_byte > 0) && ((uint8_t)cpPath[ite_byte - 1] > 0x7F)) { continue; } // check is dbcs char
				break;
			}
			else if (tmp_unit == '\\')
			{
				if ((ite_byte > 0) && ((uint8_t)cpPath[ite_byte - 1] > 0x7F)) { continue; } // check is dbcs char
				break;
			}
		} while ((--ite_byte) != -1);

		cpPath[0] = '\0';
		return 0;
	}

	// nChar exclude null char
	size_t GetSuffix(wchar_t* wpPath, const size_t nChar)
	{
		size_t ite_unit = nChar - 1;

		do
		{
			wchar_t tmp_unit = wpPath[ite_unit];

			if (tmp_unit == L'.')
			{
				size_t suffix_bytes = nChar - ite_unit;
				memcpy(wpPath, wpPath + ite_unit, suffix_bytes * sizeof(wchar_t));
				wpPath[suffix_bytes] = L'\0';
				return suffix_bytes;
			}
			else if (tmp_unit == L'/')
			{
				break;
			}
			else if (tmp_unit == L'\\')
			{
				break;
			}
		} while ((--ite_unit) != -1);

		wpPath[0] = L'\0';
		return 0;
	}

	std::string GetSuffix(std::string msPath)
	{
		size_t suffix_bytes = GetSuffix(msPath.data(), msPath.size());
		msPath.resize(suffix_bytes);
		return msPath;
	}

	std::wstring GetSuffix(std::wstring wsPath)
	{
		size_t suffix_chars = GetSuffix(wsPath.data(), wsPath.size());
		wsPath.resize(suffix_chars);
		return wsPath;
	}


	// nBytes exclude null char
	size_t RemoveSuffix(char* cpPath, size_t nBytes)
	{
		size_t ite_unit = nBytes - 1;

		do
		{
			char tmp_unit = cpPath[ite_unit];

			if (tmp_unit == '.')
			{
				if ((ite_unit > 0) && ((uint8_t)cpPath[ite_unit - 1] > 0x7F)) { continue; } // check is dbcs char
				cpPath[ite_unit] = '\0';
				return ite_unit;
			}
			else if (tmp_unit == '/')
			{
				if ((ite_unit > 0) && ((uint8_t)cpPath[ite_unit - 1] > 0x7F)) { continue; } // check is dbcs char
				break;
			}
			else if (tmp_unit == '\\')
			{
				if ((ite_unit > 0) && ((uint8_t)cpPath[ite_unit - 1] > 0x7F)) { continue; } // check is dbcs char
				break;
			}
		} while ((--ite_unit) != -1);

		return nBytes;
	}

	// nChar exclude null char
	size_t RemoveSuffix(wchar_t* wpPath, size_t nChar)
	{
		size_t ite_unit = nChar - 1;

		do
		{
			wchar_t tmp_unit = wpPath[ite_unit];

			if (tmp_unit == L'.')
			{
				wpPath[ite_unit] = L'\0';
				return ite_unit;
			}
			else if (tmp_unit == L'/')
			{
				break;
			}
			else if (tmp_unit == L'\\')
			{
				break;
			}
		} while ((--ite_unit) != -1);

		return nChar;
	}

	std::string RemoveSuffix(std::string msPath)
	{
		size_t bytes = RemoveSuffix(msPath.data(), msPath.size());
		msPath.resize(bytes);
		return msPath;
	}

	std::wstring RemoveSuffix(std::wstring wsPath)
	{
		size_t chars = RemoveSuffix(wsPath.data(), wsPath.size());
		wsPath.resize(chars);
		return wsPath;
	}


	char* FormatSlash(char* cpPath, char cSlash)
	{
		switch (cSlash)
		{
		case '\\':
		{
			for (size_t ite = 0; cpPath[ite]; ite++)
			{
				if ((uint8_t)cpPath[ite] > 0x7F) { ite++; continue; }

				if (cpPath[ite] == '/') { cpPath[ite] = '\\'; }
			}
		}
		break;

		case '/':
		{
			for (size_t ite = 0; cpPath[ite]; ite++)
			{
				if (cpPath[ite] == '\\') { cpPath[ite] = '/'; }
			}
		}
		break;
		}

		return cpPath;
	}

	wchar_t* FormatSlash(wchar_t* wpPath, wchar_t wcSlash)
	{
		switch (wcSlash)
		{
		case L'\\':
		{
			for (size_t ite = 0; wpPath[ite]; ite++)
			{
				if (wpPath[ite] == L'/') { wpPath[ite] = L'\\'; }
			}
		}
		break;

		case L'/':
		{
			for (size_t ite = 0; wpPath[ite]; ite++)
			{
				if (wpPath[ite] == L'\\') { wpPath[ite] = L'/'; }
			}
		}
		break;
		}

		return wpPath;
	}

	std::string FormatSlash(std::string msPath, char cSlash)
	{
		FormatSlash(msPath.data(), cSlash);
		return msPath;
	}

	std::wstring FormatSlash(std::wstring wsPath, wchar_t wcSlash)
	{
		FormatSlash(wsPath.data(), wcSlash);
		return wsPath;
	}
}

namespace Rut::RxPath
{

	bool Exist(std::string_view msPath)
	{
		return Platform::Exist(msPath.data());
	}

	bool Exist(std::wstring_view wsPath)
	{
		return Platform::Exist(wsPath.data());
	}


	bool DirExist(std::string_view msPath)
	{
		return Platform::DirExist(msPath.data());
	}

	bool DirExist(std::wstring_view wsPath)
	{
		return Platform::DirExist(wsPath.data());
	}


	bool FileExist(std::string_view msPath)
	{
		return Platform::FileExist(msPath.data());
	}

	bool FileExist(std::wstring_view wsPath)
	{
		return Platform::FileExist(wsPath.data());
	}


	bool MakeDir(std::string_view msPath)
	{
		return Platform::MakeDir(msPath.data());
	}

	bool MakeDir(std::wstring_view wsPath)
	{
		return Platform::MakeDir(wsPath.data());
	}


	static bool MakeDirViaPath(const char* cpPath)
	{
		return MakeDirViaPath(RxStr::ToWCS(cpPath, 0).c_str());
	}

	static bool MakeDirViaPath(const wchar_t* wpPath)
	{
		const size_t len = Platform::StrLen(wpPath);
		wchar_t path[Platform::PLATFORM_MAX_PATH];
		Platform::StrCpy(path, Platform::PLATFORM_MAX_PATH, wpPath);

		for (size_t ite_char = 0; ite_char < len; ite_char++)
		{
			switch (path[ite_char])
			{
			case L'/':
			case L'\\':
			{
				path[ite_char] = L'\0';
				Platform::MakeDir(path);
				path[ite_char] = L'\\';
			}
			break;

			case L'.':
			case L':': { ite_char++; } break;
			}
		}

		return true;
	}

	bool MakeDirViaPath(std::string_view msPath)
	{
		return MakeDirViaPath(msPath.data());
	}

	bool MakeDirViaPath(std::wstring_view wsPath)
	{
		return MakeDirViaPath(wsPath.data());
	}


	std::uintmax_t GetFileSize(std::string_view msPath)
	{
		return Platform::GetFileSize(msPath.data());
	}

	std::uintmax_t GetFileSize(std::wstring_view wsPath)
	{
		return Platform::GetFileSize(wsPath.data());
	}


	std::string ModulePathA(void* pBase)
	{
		std::string path;
		path.resize(Platform::PLATFORM_MAX_PATH);
		path.resize(Platform::GetModulePath(path.data(), Platform::PLATFORM_MAX_PATH, pBase));
		return path;
	}

	std::wstring ModulePathW(void* pBase)
	{
		std::wstring path;
		path.resize(Platform::PLATFORM_MAX_PATH);
		path.resize(Platform::GetModulePath(path.data(), Platform::PLATFORM_MAX_PATH, pBase));
		return path;
	}


	std::string ModuleNameA(void* pBase)
	{
		std::string name = ModulePathA(pBase);
		size_t bytes = GetFileName(name.data(), name.size());
		name.resize(bytes);
		return name;
	}

	std::wstring ModuleNameW(void* pBase)
	{
		std::wstring name = ModulePathW(pBase);
		size_t len = GetFileName(name.data(), name.size());
		name.resize(len);
		return name;
	}


	std::string ModuleDirA()
	{
		std::string path;
		path.resize(Platform::PLATFORM_MAX_PATH);
		path.resize(Platform::GetModuleDir(path.data(), Platform::PLATFORM_MAX_PATH));
		return path;
	}

	std::wstring ModuleDirW()
	{
		std::wstring path;
		path.resize(Platform::PLATFORM_MAX_PATH);
		path.resize(Platform::GetModuleDir(path.data(), Platform::PLATFORM_MAX_PATH));
		return path;
	}

}

namespace Rut::RxPath
{
	static void CheckPath(std::string& msPath)
	{
		FormatSlash(msPath.data(), '\\');
		if (msPath.back() != '\\') { msPath.append(1, '\\'); }
	}

	static void CheckPath(std::wstring& wsPath)
	{
		FormatSlash(wsPath.data(), L'\\');
		if (wsPath.back() != L'\\') { wsPath.append(1, L'\\'); }
	}

	bool AllFilePaths(std::string msPath, std::vector<std::string>& vecList)
	{
		CheckPath(msPath);

		std::queue<std::string> dirs;
		WIN32_FIND_DATAA find_data = { 0 };

		dirs.push(msPath.data());

		for (HANDLE hFind = INVALID_HANDLE_VALUE; !dirs.empty(); dirs.pop())
		{
			std::string& dirName = dirs.front();

			hFind = FindFirstFileA((dirName + "*").c_str(), &find_data);
			if (hFind == INVALID_HANDLE_VALUE) { return false; }

			do
			{
				if (!strcmp(find_data.cFileName, ".") || !strcmp(find_data.cFileName, "..")) { continue; }

				if (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					dirs.push(dirName + find_data.cFileName + "\\");
					continue;
				}

				vecList.emplace_back(FormatSlash(dirName + find_data.cFileName, L'/'));

			} while (FindNextFileA(hFind, &find_data));

			FindClose(hFind);
		}

		return true;
	}

	bool AllFilePaths(std::wstring wsPath, std::vector<std::wstring>& vecList)
	{
		CheckPath(wsPath);

		std::queue<std::wstring> dirs;
		WIN32_FIND_DATAW find_data = { 0 };

		dirs.push(wsPath.data());

		for (HANDLE hfile = INVALID_HANDLE_VALUE; !dirs.empty(); dirs.pop())
		{
			std::wstring& dirName = dirs.front();

			hfile = FindFirstFileW((dirName + L"*").c_str(), &find_data);
			if (hfile == INVALID_HANDLE_VALUE) { return false; }

			do
			{
				if (!wcscmp(find_data.cFileName, L".") || !wcscmp(find_data.cFileName, L"..")) { continue; }

				if (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					dirs.push(dirName + find_data.cFileName + L"\\");
					continue;
				}

				vecList.emplace_back(FormatSlash(dirName + find_data.cFileName, L'/'));

			} while (FindNextFileW(hfile, &find_data));

			FindClose(hfile);
		}

		return true;
	}

	bool CurFileNames(std::string msFolder, std::vector<std::string>& vecList, bool isAddBasePath)
	{
		CheckPath(msFolder);

		const std::string folder = msFolder.data();

		WIN32_FIND_DATAA find_data = { 0 };

		const HANDLE hfile = FindFirstFileA((folder + "*").c_str(), &find_data);
		if (hfile == INVALID_HANDLE_VALUE) { return false; }

		do
		{
			if (!strcmp(find_data.cFileName, ".") || !strcmp(find_data.cFileName, "..")) { continue; }
			if (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) continue;

			if (isAddBasePath)
			{
				vecList.emplace_back(FormatSlash(msFolder + find_data.cFileName, '/'));
			}
			else
			{
				vecList.emplace_back(FormatSlash(find_data.cFileName, '/'));
			}

		} while (FindNextFileA(hfile, &find_data));

		FindClose(hfile);
		return true;
	}

	bool CurFileNames(std::wstring wsFolder, std::vector<std::wstring>& vecList, bool isAddBasePath)
	{
		CheckPath(wsFolder);

		const std::wstring folder = wsFolder.data();

		WIN32_FIND_DATAW find_data = { 0 };

		const HANDLE hfile = FindFirstFileW((folder + L"*").c_str(), &find_data);
		if (hfile == INVALID_HANDLE_VALUE) { return false; }

		do
		{
			if (!wcscmp(find_data.cFileName, L".") || !wcscmp(find_data.cFileName, L"..")) { continue; }
			if (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) continue;

			if (isAddBasePath)
			{
				vecList.emplace_back(FormatSlash(wsFolder + find_data.cFileName, L'/'));
			}
			else
			{
				vecList.emplace_back(FormatSlash(find_data.cFileName, L'/'));
			}

		} while (FindNextFileW(hfile, &find_data));

		FindClose(hfile);
		return true;
	}

	bool CurFolderNames(std::string msFolder, std::vector<std::string>& vecList, bool isAddBasePath)
	{
		CheckPath(msFolder);

		const std::string folder = msFolder.data();

		WIN32_FIND_DATAA find_data = { 0 };

		const HANDLE hfile = FindFirstFileA((folder + "*").c_str(), &find_data);
		if (hfile == INVALID_HANDLE_VALUE) { return false; }

		do
		{
			if (!strcmp(find_data.cFileName, ".") || !strcmp(find_data.cFileName, "..")) { continue; }
			if (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				if (isAddBasePath)
				{
					vecList.emplace_back(FormatSlash(msFolder + find_data.cFileName, '/'));
				}
				else
				{
					vecList.emplace_back(FormatSlash(find_data.cFileName, '/'));
				}
			}

		} while (FindNextFileA(hfile, &find_data));

		FindClose(hfile);
		return true;
	}

	bool CurFolderNames(std::wstring wsFolder, std::vector<std::wstring>& vecList, bool isAddBasePath)
	{
		CheckPath(wsFolder);

		WIN32_FIND_DATAW find_data = { 0 };

		const HANDLE hfile = FindFirstFileW((wsFolder + L"*").c_str(), &find_data);
		if (hfile == INVALID_HANDLE_VALUE) { return false; }

		do
		{
			if (!wcscmp(find_data.cFileName, L".") || !wcscmp(find_data.cFileName, L"..")) { continue; }
			if (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				if (isAddBasePath)
				{
					vecList.emplace_back(FormatSlash(wsFolder + find_data.cFileName, L'/'));
				}
				else
				{
					vecList.emplace_back(FormatSlash(find_data.cFileName, L'/'));
				}
			}

		} while (FindNextFileW(hfile, &find_data));

		FindClose(hfile);
		return true;
	}
}