#pragma once
#include <cstdint>


namespace ZQF::ReVN::RxValkyria
{
#pragma pack(push,1)
	struct Struct
	{
		struct SysData_Seg_0
		{
			std::uint32_t uiTimestamp;
			std::uint32_t uiSysMode;
			std::uint8_t ucDisplayFlag;
			std::uint8_t ucLanguageFlag; // 1 = jp, 0 = en
			char aWindowTitle[260];
			char aGameTitle[260];
			std::uint16_t usWindowWide;
			std::uint16_t usWindowHigh;
			std::uint16_t uiUn4;
			std::uint32_t uiDICount;
			std::uint32_t uiDGCount;
			std::uint32_t uiDTCount;
			std::uint32_t uiSICount;
			std::uint32_t uiSGCount;
			std::uint32_t uiSTCount;
			std::uint32_t aUn5[8];
			char aData06Path[260];
			char aData00Path[260];
			char aData02Path[260];
			char aData05Path[260];
			char aData03Path[260];
			char aData04Path[260];
			char aData01Path[260];
			char aDataxxPath[260];
			char aDataName[260];
			char aFlagodnName[260];
			std::uint32_t uiGameEndLabelValue;
			char aGameEndLabelName[260];
			std::uint32_t uiSysMenu0LabelValue;
			char aSysMenu0LabelName[260];
			std::uint32_t uiSysMenu1LabelValue;
			char aSysMenu1LabelName[260];
			char aFontName[260];
			std::uint8_t aUn1[4092];
			char aCheckKey[260];
		};

		struct VFS_Entry
		{
			char aFilePath[260];
			std::uint32_t uiFOA;
			std::uint32_t uiSize;
			char aFileName[260];
		};


		/*
		struct Pack
		{
			uint32_t uiIndexSize;
			VAL_Pack_Entry[uiIndexSize / sizeof(VAL_Pack_Entry)]
			// data..
		}
		*/

		struct Pack_Entry
		{
			char aFileName[260];
			std::uint32_t nOffset; // after index
			std::uint32_t nSize;

			static constexpr inline auto SizeBytes{ sizeof(aFileName) + sizeof(nOffset) + sizeof(nSize) };
		};


		/*
		struct SDT_Label
		{
			uint8_t[? ? ] aLabelName; // end with '\0', enc-> xor 0xFF
			uint32_t uiLableRVA; // FOA = uiHeaderSize + uiLableRVA :004521B7
		};
		*/

		struct SDT_HDR_Info
		{
			std::uint32_t uiHDRSize;
			std::uint32_t uiMsgCount;
			std::uint32_t uiSelectCount;
			std::uint32_t uiLabelCount;
			std::uint32_t uiCheckDataFOA;

			static constexpr inline auto SizeBytes{ sizeof(uiHDRSize) + sizeof(uiMsgCount) + sizeof(uiSelectCount) + sizeof(uiLabelCount) + sizeof(uiCheckDataFOA) };
		};

		/*
		struct SDT_HDR
		{
			VAL_SDT_HDR_Info Info;
			VAL_SDT_Label[SDT_HDR_Info.uiLabelCount];
			uint8_t aCheckData[9]; // end with '\0'
		};
		*/

		/*
		struct SDT_File
		{
			VAL_SDT_HDR
			// Code
		}
		*/

		struct VM
		{
			struct Script_Entry
			{
				Script_Entry* pNext;
				std::uint16_t uiID;
				std::uint16_t uiAlign;
				std::uint32_t* pData;
				std::uint32_t uiSize;
				std::uint32_t* pHdrSize;
				std::uint32_t* pLabelCount;
				std::uint32_t* pLabelIndex;
				std::uint32_t* pMsgCount;
				std::uint32_t* pMsgBuffer;
				std::uint32_t* pSeletCount;
				std::uint32_t* pSelectBuffer;
			};

			struct Context
			{
				std::uint16_t usScriptID;
				std::uint16_t usAlign;
				Script_Entry* pScript;
				std::uint32_t uiPC;
				std::uint32_t uiLine;
				std::uint32_t uiUn0;
			};

			struct Context_Ptr
			{
				Context* Ptr;
				std::uint32_t Un0;
				std::uint32_t Un1;
			};

			std::uint32_t uiUn0;
			Script_Entry* pScriptList;
			std::uint8_t aUn1[1464];
			char aScriptPath[260];
			char aDataXXPath[260];
			std::uint8_t aUn1_[2204];
			std::uint16_t uiContextPtrIndex;
			std::uint8_t aUn2[262];
			Context_Ptr aContextPtr[255];
		};
	};
#pragma pack(pop)
}