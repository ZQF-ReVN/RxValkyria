#pragma once
#include <cstdint>


namespace Valkyria
{
	struct VAL_SysData_Seg_0
	{
		uint8_t ucUn0;
		uint8_t ucUn1;
		uint8_t ucUn2;
		uint8_t ucUn3;
		uint32_t uiSysMode;
		uint8_t ucDisplayFlag;
		uint8_t ucLanguageFlag; // 1 = jp, 0 = en
		char aWindowTitle[260];
		char aGameTitle[260];
		uint16_t usWindowWide;
		uint16_t usWindowHigh;
		uint16_t uiUn4;
		uint32_t uiDICount;
		uint32_t uiDGCount;
		uint32_t uiDTCount;
		uint32_t uiSICount;
		uint32_t uiSGCount;
		uint32_t uiSTCount;
		uint32_t aUn5[8];
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
		uint32_t uiGameEndLabelValue;
		char aGameEndLabelName[260];
		uint32_t uiSysMenu0LabelValue;
		char aSysMenu0LabelName[260];
		uint32_t uiSysMenu1LabelValue;
		char aSysMenu1LabelName[260];
		char aFontName[260];
		uint8_t aUn1[4092];
		char aCheckKey[260];
	};

	struct VAL_Pack_Entry
	{
		char aFileName[260];
		uint32_t uiOffset; // after index
		uint32_t uiSize;
	};

	struct VAL_VFS_Entry
	{
		char aFilePath[260];
		uint32_t uiFOA;
		uint32_t uiSize;
		char aFileName[260];
	};

	/*
	struct VAL_Pack
	{
		uint32_t uiIndexSize;
		VAL_Pack_Entry[uiIndexSize / sizeof(VAL_Pack_Entry)]
		// data..
	}
	*/

	/*
	struct VAL_SDT_Label
	{
		uint8_t[? ? ] aLabelName; // end with '\0', enc-> xor 0xFF
		uint32_t uiLableRVA; // FOA = uiHeaderSize + uiLableRVA :004521B7
	};
	*/

	struct VAL_SDT_HDR_Info
	{
		uint32_t uiHDRSize;
		uint32_t uiMsgCount;
		uint32_t uiSelectCount;
		uint32_t uiLabelCount;
		uint32_t uiCheckDataFOA;
	};

	/*
	struct VAL_SDT_HDR
	{
		VAL_SDT_HDR_Info Info;
		VAL_SDT_Label[SDT_HDR_Info.uiLabelCount];
		uint8_t aCheckData[9]; // end with '\0'
	};
	*/

	/*
	struct VAL_SDT_File
	{
		VAL_SDT_HDR
		// Code
	}
	*/

	struct VAL_Script_Entry
	{
		VAL_Script_Entry* pNext;
		uint16_t uiID;
		uint16_t uiAlign;
		uint32_t* pData;
		uint32_t uiSize;
		uint32_t* pHdrSize;
		uint32_t* pLabelCount;
		uint32_t* pLabelIndex;
		uint32_t* pMsgCount;
		uint32_t* pMsgBuffer;
		uint32_t* pSeletCount;
		uint32_t* pSelectBuffer;
	};

	struct VAL_Context
	{
		uint16_t usScriptID;
		uint16_t usAlign;
		VAL_Script_Entry* pScript;
		uint32_t uiPC;
		uint32_t uiLine;
		uint32_t uiUn0;
	};

	struct VAL_Context_Ptr
	{
		VAL_Context* Ptr;
		uint32_t Un0;
		uint32_t Un1;
	};

	struct VAL_VM
	{
		uint32_t uiUn0;
		VAL_Script_Entry* pScriptList;
		uint8_t aUn1[1464];
		char aScriptPath[260];
		char aDataXXPath[260];
		uint8_t aUn1_[2204];
		uint16_t uiContextPtrIndex;
		uint8_t aUn2[262];
		VAL_Context_Ptr aContextPtr[255];
	};
}