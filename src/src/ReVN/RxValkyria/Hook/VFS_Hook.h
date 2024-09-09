#pragma once
#include <cstdint>


namespace ZQF::ReVN::RxValkyria
{
	class VFSHook
	{
	public:
		static auto HookScriptRead(const std::uint32_t vaFnPackReadScript) -> void;
		static auto HookMediasRead(const std::uint32_t vaFnPackReadMedias) -> void;

	};
}