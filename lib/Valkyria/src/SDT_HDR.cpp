#include "SDT_HDR.h"
#include "SDT_String.h"


namespace Valkyria::SDT
{
	HDR_Info::HDR_Info()
	{

	}

	void HDR_Info::Parse(uint8_t* pData)
	{
		uint32_t* tmp_ptr = (uint32_t*)pData;
		m_uiHeaderSize = tmp_ptr[0];
		m_uiMsgCount = tmp_ptr[1];
		m_uiSelectCount = tmp_ptr[2];
		m_uiLabelCount = tmp_ptr[3];
		m_uiCheckDataFOA = tmp_ptr[4];
	}

	Rut::RxMem::Auto HDR_Info::Make() const
	{
		Rut::RxMem::Auto mem_data;
		mem_data.SetSize(this->GetSize());
		uint32_t* cur_ptr = (uint32_t*)mem_data.GetPtr();
		{
			cur_ptr[0] = m_uiHeaderSize;
			cur_ptr[1] = m_uiMsgCount;
			cur_ptr[2] = m_uiSelectCount;
			cur_ptr[3] = m_uiLabelCount;
			cur_ptr[4] = m_uiCheckDataFOA;
		}
		return mem_data;
	}

	constexpr size_t HDR_Info::GetSize() const
	{
		return sizeof(m_uiHeaderSize) + sizeof(m_uiMsgCount) + sizeof(m_uiSelectCount) + sizeof(m_uiLabelCount) + sizeof(m_uiCheckDataFOA);
	}

	constexpr size_t HDR_Info::GetHeaderSize() const
	{
		return m_uiHeaderSize;
	}

	constexpr size_t HDR_Info::GetLabelCount() const
	{
		return m_uiLabelCount;
	}

	constexpr size_t HDR_Info::GetMsgCount() const
	{
		return m_uiMsgCount;
	}

	constexpr size_t HDR_Info::GetSelectCount() const
	{
		return m_uiSelectCount;
	}

	// check data size include null char
	constexpr size_t HDR_Info::GetCheckDataSize() const
	{
		return m_uiHeaderSize - m_uiCheckDataFOA;
	}


	Label_Entry::Label_Entry()
	{

	}

	void Label_Entry::Parse(uint8_t* pData)
	{
		m_msLabelName = String::Decode(pData);
		m_uiLableInCodeOffset = *(uint32_t*)(pData + m_msLabelName.size() + 1);
	}

	Rut::RxMem::Auto Label_Entry::Make() const
	{
		Rut::RxMem::Auto mem_data;
		mem_data.SetSize(this->GetSize());
		uint8_t* cur_ptr = mem_data.GetPtr();
		{
			memcpy(cur_ptr, m_msLabelName.data(), m_msLabelName.size() + 1);
			String::Encode(cur_ptr);
			cur_ptr += m_msLabelName.size() + 1;
			*(uint32_t*)cur_ptr = m_uiLableInCodeOffset;
		}
		return mem_data;
	}

	constexpr size_t Label_Entry::GetSize() const
	{
		return m_msLabelName.size() + 1 + sizeof(m_uiLableInCodeOffset);
	}


	Label_Index::Label_Index()
	{

	}

	void Label_Index::Parse(uint8_t* pData, size_t uiLabelCount)
	{
		uint8_t* cur_ptr = pData;

		for (size_t ite_label = 0; ite_label < uiLabelCount; ite_label++)
		{
			Label_Entry label;
			label.Parse(cur_ptr);
			cur_ptr += label.GetSize();
			m_vcLabels.emplace_back(std::move(label));
		}
	}

	Rut::RxMem::Auto Label_Index::Make() const
	{
		Rut::RxMem::Auto mem_data;
		mem_data.SetSize(this->GetSize());
		uint8_t* cur_ptr = mem_data.GetPtr();
		{
			for (auto& label : m_vcLabels)
			{
				Rut::RxMem::Auto label_mem = label.Make();
				memcpy(cur_ptr, label_mem.GetPtr(), label_mem.GetSize());
				cur_ptr += label_mem.GetSize();
			}
		}
		return mem_data;
	}

	constexpr size_t Label_Index::GetSize() const
	{
		size_t size = 0;
		for (auto& label : m_vcLabels)
		{
			size += label.GetSize();
		}
		return size;
	}


	HDR::HDR()
	{

	}

	void HDR::Parse(uint8_t* pData)
	{
		uint8_t* cur_ptr = pData;

		m_Info.Parse(cur_ptr);
		cur_ptr += m_Info.GetSize();

		m_Labels.Parse(cur_ptr, m_Info.GetLabelCount());
		cur_ptr += m_Labels.GetSize();

		m_msCheckData = { (char*)cur_ptr,m_Info.GetCheckDataSize() - 1 };
	}

	Rut::RxMem::Auto HDR::Make() const
	{
		Rut::RxMem::Auto mem_data;
		mem_data.SetSize(this->GetSize());
		uint8_t* cur_ptr = mem_data.GetPtr();

		Rut::RxMem::Auto info_mem = m_Info.Make();
		memcpy(cur_ptr, info_mem.GetPtr(), info_mem.GetSize());
		cur_ptr += info_mem.GetSize();

		Rut::RxMem::Auto labels_mem = m_Labels.Make();
		memcpy(cur_ptr, labels_mem.GetPtr(), labels_mem.GetSize());
		cur_ptr += labels_mem.GetSize();

		memcpy(cur_ptr, m_msCheckData.data(), m_msCheckData.size() + 1);

		return mem_data;
	}

	constexpr size_t HDR::GetSize() const
	{
		return m_Info.GetHeaderSize();
	}

	const HDR_Info& HDR::GetInfo() const
	{
		return m_Info;
	}

	const size_t HDR::GetCheckDataSize() const
	{
		return this->GetSize() - this->GetInfo().GetSize() - m_Labels.GetSize();
	}

	void HDR::SetCheckData(std::string_view msCheckData)
	{
		m_msCheckData = msCheckData;
	}
}