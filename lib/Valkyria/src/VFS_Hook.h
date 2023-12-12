#pragma once
#include <cstdint>


namespace Valkyria::VFS
{
	void HookScriptRead(uint32_t fnPackReadScript, bool isDebugInfo = false);
	void HookMediasRead(uint32_t fnPackReadMedias, bool isDebugInfo = false);
}