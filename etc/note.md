
```C
struct SDT_Label
{
	uint8_t[??] aLabelName; // end with '\0', enc-> xor 0xFF
	uint32_t uiLableRVA; // FOA = uiHeaderSize + uiLableRVA :004521B7
};

struct SDT_HDR_Info
{
	uint32_t uiHDRSize;
	uint32_t uiMsgCount;
	uint32_t uiSelectCount;
	uint32_t uiLabelCount;
	uint32_t uiCheckDataFOA;
};

struct SDT_HDR
{
	SDT_HDR_Info
	SDT_Label[SDT_HDR_Info.uiLabelCount]
	uint8_t aCheckData[uiHDRSize - sizeof(SDT_Label[SDT_HDR_Info.uiLabelCount])]; // end with '\0'
};

struct SDT
{
	SDT_HDR
	// Code
}


entry_ptr = [this + 4] -> the first sdt entry

struct VAL_Script_Entry
{
  VAL_Script_Entry *pNext;
  uint16_t uiID;
  uint16_t uiAlign;
  uint32_t *pData;
  uint32_t uiSize;
  uint32_t *pHdrSize;
  uint32_t *pLabelCount;
  uint32_t *pLabelIndex;
  uint32_t *pMsgCount;
  uint32_t *pMsgBuffer;
  uint32_t *pSeletCount;
  uint32_t *pSelectBuffer;
};


#pragma pack(push, 1)
struct VAL_Context
{
  uint16_t usScriptID;
  uint16_t usUn0;
  VAL_Script_Entry *pScript;
  uint32_t uiPC;
  uint32_t uiLine;
  uint32_t uiUn0;
};
#pragma pack(pop)

struct VAL_Context_Ptr
{
  VAL_Context *Ptr;
  uint32_t Un0;
  uint32_t Un1;
};

struct VAL_VM
{
  uint32_t uiUn0;
  VAL_Script_Entry *pScriptList;
  uint8_t aUn1[1464];
  char aScriptPath[260];
  char aDataXXPath[260];
  uint8_t aUn1_[2204];
  uint16_t uiContextPtrIndex;
  uint8_t aUn2[262];
  VAL_Context_Ptr aContextPtr[255];
};


struct VAL_SysData_Seg_0
{
  uint32_t uiTimestamp;
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
  uint8_t aDebugKey[260];
};



struct VAL_Pack_Entry
{
  char aFileName[260];
  uint32_t uiOffset; // after index
  uint32_t uiSize;
}

struct VAL_VFS_Entry
{
  char aFilePath[260];
  uint32_t uiFOA;
  uint32_t uiSize;
  char aFileName[260];
}

struct VAL_Pack
{
  uint32_t uiIndexSize;
  VAL_Pack_Entry[uiIndexSize / sizeof(VAL_Pack_Entry)]
  // data..
}

```



```
label_index_offset -> VAL::VM::GetCallOffset 
VAL::VM::GetCallOffset
	从标签索引开始偏移，并获取新的文件偏移

OP: 0x0A44 
Name: FarCallLabel 
Address: 0x00468AE1
Formate: [word:op][byte:type][dword:script_id][byte:type][str(xor(0xFF)):label_name]
Example: 440A 00 01000000 08 B39E9D9A93A0B89E929AAC8B9E8D8BFF

// 跳转到指定脚本
OP: 0x0A48
Name: JMP
Address: 0x0046886D
Formate: [word:op][byte:un][dword:script_id]
Example: 480A 00 02000000

// 依据label_index_offset在标签索引表中找到要跳转的FOA，设置给PC
OP: 0x0A42
Name: GOTO
Address: 0x0046886D
Formate: [word:op][dword:label_index_offset]
Example: 420A 61000000

OP: 0x0A43
Name: Call
Address: 0x004688EB
Formate: [word:op][dword:label_index_offset]

OP: 0x0A3D
Name: if{
Address: 0x004684A4
Formate: 

OP: 0x0A17
Name: SubProcLabel
Address: 0x00465330
Formate: [word:op][byte:type][dword:][byte:type][dword:script_id][byte:type][str(xor(0xFF)):proc_name]

// 文本框对话字符串
OP: 0x0E01
Name: MSGWNDTEXT
Address: 
Formate: [word:op][wdord:un][dword:line_number][str(xor(0xFF)):msg_text]
Example: 010E 1111 4C040000 7EBF7E9C7E9C692575907D207D16716167B17EBDFF

// 文本框人物名字符串
OP: 0x0E00
Name: MSGWNDNAME
Address: 
Formate: [word:op][str(xor(0xFF)):NameString]
Example: 000E 7E86716167B17E85FF

// 设置分支选项
OP: 0x0E1C
Name: SelEXSet
Address: 0x0046E612
Formate: [word:op][dword:un(always 0x00000000)][byte:type][str(xor(0xFF)):select_text][byte:type][str(xor(0xFF)):select_text][byte:end_flag]
Example: 1C0E 00000000 08 7E8A7CBE7C747C927D336A1D724D7D367D377D167E89FF 08 7E8A7CBE7C747C927D336A1D724D7D367D377D187D377D5D7E89FF FF

// 换行，停留在文本框，等待用户输入事件
OP: 0x0E04
Name: R
Address: 
Formate: [word:op][wdord:un][dword:line_number]
Example: 040E 1111 D2000000
```



```

// count pack entry matching suffiix
entry_count = 0
for ifs : match(data06-*)[data06-01,data06-02,data06-03,data06-04....]
{
  pack_index = ifs.read(ifs.read(4,beg),cur)
  for pack_entry in pack_index
  {
    for if suffix == '.sdt'
    {
      entry_count++
    }
  }
}

// make vfs index
vfs_index = new(entry_count * sizef(vfs_entry))
for ifs : match(data06-*)[data06-01,data06-02,data06-03,data06-04....]
{
  pack_index = ifs.read(ifs.read(4,beg),cur)
  for pak_entry in pack_index
  {
    for if suffix == '.sdt'
    {
      cur_vfs_entry = vfs_entry.back()
      for vfs_entry in vfs_index
      {
        if pak_entry.flename == vfs_entry.filename
        {
          cur_vfs_entry(vfs_entry)
        }
      }
      cur_vfs_entry(pack_entry)
    }
  }
}
```

```
VAL-0 ------ VAL-37 old version .odn
VAL-0044 ----- VAL-66 new version .sdt .dat

Tested:VAL-0039 VAH-0001 VAL-0040 VAL-0064 VAL-0017Re VAL-0065 VAL-0066(DL)
(2017 - 2023)
```



## Encrypt The Key

**Param**: Game Title Shit-Jis Encoding String    : `プレイ！プレイ！プレイ！ロック！`

**Param**: Game Title Shit-Jis Encoding Length  : `0x20 ` (bytes count)

**Param**: Key String ASCII : `VAL-0064`

**Param**: Key String Hex   : `56 41 4C 2D 30 30 36 34`

**Algorithm**:

- Swap `Key String Hex` Byte Order: 

  - low 4 bits and heigh 4bits swap

  - `65 14 C4 D2 03 03 63 43`

- Encrypt Both Ends Via `Game Title Shit-Jis Encoding Length` : 

  - first byte add title len, last byte sub (0x2F * (nTitleLen / 0x2F) - nTitleLen)
  - `85 14 C4 D2 03 03 63 63`

**Encrypted Key Hex**: `85 14 C4 D2 03 03 63 63`



## Create SDT Check Data

**Param**: Game Title Shit-Jis Encoding String    : `プレイ！プレイ！プレイ！ロック！`

**Param**: Game Title Shit-Jis Encoding Length  : `0x20`  (bytes count)

**Param**: Key String ASCII: `VAL-0064`

**Param**: Key String Hex  : `56 41 4C 2D 30 30 36 34`

**Param**: SDT File Size: `0x3C`

**Algorithm**:

- Get  Encrypted Key Hex: 

  - `85 14 C4 D2 03 03 63 63`

- Each Byte Add `SDT File Size`

  - If file size greater than 0xFF truncate to uint8_t, so 0x3C and 0x43C have the same key data.

  - `1C 50 00 0E 3F 3F 9F 9F`

**Check Data**:  `1C 50 00 0E 3F 3F 9F 9F`

