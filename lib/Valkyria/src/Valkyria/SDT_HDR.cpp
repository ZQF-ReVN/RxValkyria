#include "SDT_HDR.h"
#include "SDT_String.h"


namespace Valkyria::SDT
{
	HDR_Info::HDR_Info()
	{

	}

	void HDR_Info::Load(const uint8_t* const pData)
	{
		const uint32_t* tmp_ptr = (uint32_t*)pData;
		m_uiHeaderSize = tmp_ptr[0];
		m_uiMsgCount = tmp_ptr[1];
		m_uiSelectCount = tmp_ptr[2];
		m_uiLabelCount = tmp_ptr[3];
		m_uiCheckDataFOA = tmp_ptr[4];
	}

	void HDR_Info::Make(Rut::RxMem::Auto& rfMem) const
	{
		rfMem.SetSize(this->GetSize());
		uint32_t* cur_ptr = (uint32_t*)rfMem.GetPtr();
		cur_ptr[0] = m_uiHeaderSize;
		cur_ptr[1] = m_uiMsgCount;
		cur_ptr[2] = m_uiSelectCount;
		cur_ptr[3] = m_uiLabelCount;
		cur_ptr[4] = m_uiCheckDataFOA;
	}

	constexpr size_t HDR_Info::GetSize() const noexcept
	{
		return sizeof(m_uiHeaderSize) + sizeof(m_uiMsgCount) + sizeof(m_uiSelectCount) + sizeof(m_uiLabelCount) + sizeof(m_uiCheckDataFOA);
	}

	constexpr size_t HDR_Info::GetHeaderSize() const noexcept
	{
		return m_uiHeaderSize;
	}

	constexpr size_t HDR_Info::GetLabelCount() const noexcept
	{
		return m_uiLabelCount;
	}

	constexpr size_t HDR_Info::GetMsgCount() const noexcept
	{
		return m_uiMsgCount;
	}

	constexpr size_t HDR_Info::GetSelectCount() const noexcept
	{
		return m_uiSelectCount;
	}

	// check data size include null char
	constexpr size_t HDR_Info::GetCheckDataSize() const noexcept
	{
		return m_uiHeaderSize - m_uiCheckDataFOA;
	}


	Label_Entry::Label_Entry()
	{

	}

	void Label_Entry::Load(const uint8_t* const pData)
	{
		m_msLabelName = String::Decode(pData);
		m_uiLableInCodeOffset = *(uint32_t*)(pData + m_msLabelName.size() + 1);
	}

	void Label_Entry::Make(Rut::RxMem::Auto& rfMem) const
	{
		rfMem.SetSize(this->GetSize());
		uint8_t* cur_ptr = rfMem.GetPtr();

		memcpy(cur_ptr, m_msLabelName.data(), m_msLabelName.size() + 1);
		String::Encode(cur_ptr);

		cur_ptr += m_msLabelName.size() + 1;
		*(uint32_t*)cur_ptr = m_uiLableInCodeOffset;
	}

	constexpr size_t Label_Entry::GetSize() const
	{
		return m_msLabelName.size() + 1 + sizeof(m_uiLableInCodeOffset);
	}


	Label_Index::Label_Index()
	{

	}

	void Label_Index::Load(const uint8_t* const pData, size_t uiLabelCount)
	{
		const uint8_t* cur_ptr = pData;

		for (size_t ite_label = 0; ite_label < uiLabelCount; ite_label++)
		{
			Label_Entry label;
			label.Load(cur_ptr);
			cur_ptr += label.GetSize();
			m_vcLabels.emplace_back(std::move(label));
		}
	}

	void Label_Index::Make(Rut::RxMem::Auto& rfMem) const
	{
		rfMem.SetSize(this->GetSize());

		size_t write_size = 0;
		Rut::RxMem::Auto buffer;
		for (auto& label : m_vcLabels)
		{
			label.Make(buffer);
			memcpy(rfMem.GetPtr() + write_size, buffer.GetPtr(), buffer.GetSize());
			write_size += buffer.GetSize();
		}
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

	void HDR::Load(const uint8_t* const pData)
	{
		const uint8_t* cur_ptr = pData;

		m_Info.Load(cur_ptr);
		cur_ptr += m_Info.GetSize();

		m_Labels.Load(cur_ptr, m_Info.GetLabelCount());
		cur_ptr += m_Labels.GetSize();

		m_msCheckData = { (char*)cur_ptr,m_Info.GetCheckDataSize() - 1 };
	}

	void HDR::Make(Rut::RxMem::Auto& rfMem) const
	{
		rfMem.SetSize(this->GetSize());

		size_t write_size = 0;
		Rut::RxMem::Auto buffer;

		m_Info.Make(buffer);
		memcpy(rfMem.GetPtr() + write_size, buffer.GetPtr(), buffer.GetSize());
		write_size += buffer.GetSize();

		m_Labels.Make(buffer);
		memcpy(rfMem.GetPtr(), buffer.GetPtr(), buffer.GetSize());
		write_size += buffer.GetSize();

		memcpy(rfMem.GetPtr(), m_msCheckData.data(), m_msCheckData.size() + 1);
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