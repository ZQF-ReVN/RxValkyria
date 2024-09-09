#include "VFS_Hook.h"
#include <ZxHook/SHooker.h>


namespace ZQF::ReVN::RxValkyria
{
    static auto __cdecl PackReadScript_Hook(const char* cpPackPrefix, const std::uint8_t* pInfoBuffer, const std::uint32_t* pScriptCount, const char* cpScriptSuffix, const std::uint32_t nReadMod) -> bool
    {
        // if (Rut::RxSys::FileExist(cpPackPrefix)) { return false; }
        return ZQF::ZxHook::SHooker<PackReadScript_Hook>::FnRaw(cpPackPrefix, pInfoBuffer, pScriptCount, cpScriptSuffix, nReadMod);
    }

    static auto __cdecl PackReadMedias_Hook(const char* cpFilePath0, const char* cpFilePath1, const std::uint32_t* pResFOA, const std::uint32_t* pResSize, const std::uint32_t nReadMod) -> bool
    {
        // if (Rut::RxSys::FileExist(cpFilePath0)) { return false; }
        return ZQF::ZxHook::SHooker<PackReadMedias_Hook>::FnRaw(cpFilePath0, cpFilePath1, pResFOA, pResSize, nReadMod);
    }


    auto VFSHook::HookScriptRead(const std::uint32_t vaFnPackReadScript) -> void
    {
        ZQF::ZxHook::SHooker<PackReadScript_Hook>::Commit(vaFnPackReadScript);
    }

    auto VFSHook::HookMediasRead(const std::uint32_t vaFnPackReadMedias) -> void
    {
        ZQF::ZxHook::SHooker<PackReadMedias_Hook>::Commit(vaFnPackReadMedias);
    }
}