#include "VFS_Hook.h"
#include "../../Rut/RxSys.h"
#include "../../Rut/RxConsole.h"
#include "../../RxHook/RxHook.h"

using namespace Rut;


namespace Valkyria::VFS
{
    // input:[cpPackPrefix, cpScriptSuffix, nReadMod] , out:[pInfoBuffer, pScriptCount]
    typedef bool(__cdecl* fnPackReadScript_t)(char* cpPackPrefix, uint8_t* pInfoBuffer, uint32_t* pScriptCount, char* cpScriptSuffix, uint32_t nReadMod);
    static fnPackReadScript_t sg_fnPackReadScript = (fnPackReadScript_t)0x0041DA10;

    // input:[cpFilePath0, cpFilePath0, nReadMod] , out:[pResFOA, pResSize]
    typedef bool(__cdecl* fnPackReadMedias_t)(char* cpFilePath0, char* cpFilePath1, uint32_t* pResFOA, uint32_t* pResSize, uint32_t nReadMod);
    static fnPackReadMedias_t sg_fnPackReadMedias = (fnPackReadMedias_t)0x0041D770;


    bool __cdecl PackReadScript_Hook(char* cpPackPrefix, uint8_t* pInfoBuffer, uint32_t* pScriptCount, char* cpScriptSuffix, uint32_t nReadMod)
    {
        if (RxSys::FileExist(cpPackPrefix)) { return false; }
        return sg_fnPackReadScript(cpPackPrefix, pInfoBuffer, pScriptCount, cpScriptSuffix, nReadMod);
    }

    bool __cdecl PackReadMedias_Hook(char* cpFilePath0, char* cpFilePath1, uint32_t* pResFOA, uint32_t* pResSize, uint32_t nReadMod)
    {
        if (RxSys::FileExist(cpFilePath0)) { return false; }
        return sg_fnPackReadMedias(cpFilePath0, cpFilePath1, pResFOA, pResSize, nReadMod);
    }

    bool __cdecl PackReadScript_Hook_Debug(char* cpPackPrefix, uint8_t* pInfoBuffer, uint32_t* pScriptCount, char* cpScriptSuffix, uint32_t nReadMod)
    {
        if (RxSys::FileExist(cpPackPrefix))
        {
            RxConsole::PutFormat("ReadScript:%s [Hooked]\n", cpPackPrefix);
            return false;
        }

        bool status = sg_fnPackReadScript(cpPackPrefix, pInfoBuffer, pScriptCount, cpScriptSuffix, nReadMod);
        RxConsole::PutFormat("ReadScript:%s , ScriptCount:%d\n", cpPackPrefix, *pScriptCount);
        return status;
    }

    bool __cdecl PackReadMedias_Hook_Debug(char* cpFilePath0, char* cpFilePath1, uint32_t* pResFOA, uint32_t* pResSize, uint32_t nReadMod)
    {
        if (RxSys::FileExist(cpFilePath0))
        {
            RxConsole::PutFormat("ReadMedia:%s [Hooked]\n", cpFilePath0);
            return false;
        }

        std::string file_path = cpFilePath0;
        bool status = sg_fnPackReadMedias(cpFilePath0, cpFilePath1, pResFOA, pResSize, nReadMod);
        RxConsole::PutFormat("ReadMedia:%s, FOA:0x%X, Size:0x%X\n", file_path.c_str(), *pResFOA, *pResSize);
        return status;
    }


    void HookScriptRead(uint32_t fnPackReadScript, bool isDebugInfo)
    {
        sg_fnPackReadScript = (fnPackReadScript_t)fnPackReadScript;
        if (isDebugInfo)
        {
            RxHook::Detours::AttrachDirectly(&sg_fnPackReadScript, PackReadScript_Hook_Debug);
        }
        else
        {
            RxHook::Detours::AttrachDirectly(&sg_fnPackReadScript, PackReadScript_Hook);
        }
    }

    void HookMediasRead(uint32_t fnPackReadMedias, bool isDebugInfo)
    {
        sg_fnPackReadMedias = (fnPackReadMedias_t)fnPackReadMedias;
        if (isDebugInfo)
        {
            RxHook::Detours::AttrachDirectly(&sg_fnPackReadMedias, PackReadMedias_Hook_Debug);
        }
        else
        {
            RxHook::Detours::AttrachDirectly(&sg_fnPackReadMedias, PackReadMedias_Hook);
        }
    }
}