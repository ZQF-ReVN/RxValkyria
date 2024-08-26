#include <RxValkyria/Core/SDT_HDR.h>
#include <RxValkyria/Core/SDT_String.h>


namespace ZQF::RxValkyria::SDT
{
	HDR_Info::HDR_Info()
	{

	}

	auto HDR_Info::Load(const std::uint8_t* const pData) -> void
	{
		const uint32_t* tmp_ptr = (uint32_t*)pData;
		m_uiHeaderSize = tmp_ptr[0];
		m_uiMsgCount = tmp_ptr[1];
		m_uiSelectCount = tmp_ptr[2];
		m_uiLabelCount = tmp_ptr[3];
		m_uiCheckDataFOA = tmp_ptr[4];
	}

	auto HDR_Info::Make(ZxMem& rfMem) const -> void
	{
		rfMem
			<< static_cast<std::uint32_t>(m_uiHeaderSize)
			<< static_cast<std::uint32_t>(m_uiMsgCount)
			<< static_cast<std::uint32_t>(m_uiSelectCount)
			<< static_cast<std::uint32_t>(m_uiLabelCount)
			<< static_cast<std::uint32_t>(m_uiCheckDataFOA);
	}

	auto HDR_Info::SizeBytes() const noexcept -> std::size_t
	{
		return sizeof(m_uiHeaderSize) + sizeof(m_uiMsgCount) + sizeof(m_uiSelectCount) + sizeof(m_uiLabelCount) + sizeof(m_uiCheckDataFOA);
	}

	auto HDR_Info::GetHeaderSize() const noexcept -> std::size_t
	{
		return m_uiHeaderSize;
	}

	auto HDR_Info::GetLabelCount() const noexcept -> std::size_t
	{
		return m_uiLabelCount;
	}

	auto HDR_Info::GetMsgCount() const noexcept -> std::size_t
	{
		return m_uiMsgCount;
	}

	auto HDR_Info::GetSelectCount() const noexcept -> std::size_t
	{
		return m_uiSelectCount;
	}

	// check data size include null char
	auto HDR_Info::GetCheckDataSize() const noexcept -> std::size_t
	{
		return static_cast<std::size_t>(m_uiHeaderSize - m_uiCheckDataFOA);
	}


	Label_Entry::Label_Entry()
	{

	}

	auto Label_Entry::Load(const std::uint8_t* const pData) -> void
	{
		m_msLabelName = String::Decode(pData);
		m_uiLableInCodeOffset = *(uint32_t*)(pData + m_msLabelName.size() + 1);
	}

	auto Label_Entry::Make(ZxMem& rfMem) const -> void
	{
		auto label_name_ptr = rfMem.PtrCur<uint8_t*>();
		rfMem << std::span{ m_msLabelName.data(), m_msLabelName.size() + 1 };
		String::Encode(label_name_ptr);

		rfMem << static_cast<std::uint32_t>(m_uiLableInCodeOffset);
	}

	auto Label_Entry::SizeBytes() const -> std::size_t
	{
		return m_msLabelName.size() + 1 + sizeof(m_uiLableInCodeOffset);
	}


	Label_Index::Label_Index()
	{

	}

	auto Label_Index::Load(const std::uint8_t* const pData, const std::size_t uiLabelCount) -> void
	{
		const uint8_t* cur_ptr = pData;

		for (size_t ite_label = 0; ite_label < uiLabelCount; ite_label++)
		{
			Label_Entry label;
			label.Load(cur_ptr);
			cur_ptr += label.SizeBytes();
			m_vcLabels.emplace_back(std::move(label));
		}
	}

	auto Label_Index::Make(ZxMem& rfMem) const -> void
	{
		for (auto& label : m_vcLabels)
		{
			label.Make(rfMem);
		}
	}

	auto Label_Index::SizeBytes() const -> std::size_t
	{
		std::size_t size = 0;
		for (auto& label : m_vcLabels)
		{
			size += label.SizeBytes();
		}
		return size;
	}


	HDR::HDR()
	{

	}

	auto HDR::Load(const std::uint8_t* const pData) -> void
	{
		const uint8_t* cur_ptr = pData;

		m_Info.Load(cur_ptr);
		cur_ptr += m_Info.SizeBytes();

		m_Labels.Load(cur_ptr, m_Info.GetLabelCount());
		cur_ptr += m_Labels.SizeBytes();

		m_msCheckData = { (char*)cur_ptr,m_Info.GetCheckDataSize() - 1 };
	}

	auto HDR::Make(ZxMem& rfMem) const -> void
	{
		m_Info.Make(rfMem);
		m_Labels.Make(rfMem);
		rfMem << std::span{ m_msCheckData.data(), m_msCheckData.size() + 1 };
	}

	auto HDR::SizeBytes() const -> std::size_t
	{
		return m_Info.GetHeaderSize();
	}

	auto HDR::GetInfo() const -> const HDR_Info&
	{
		return m_Info;
	}

	auto HDR::GetCheckDataSize() const -> std::size_t
	{
		return this->SizeBytes() - this->GetInfo().SizeBytes() - m_Labels.SizeBytes();
	}

	auto HDR::SetCheckData(const std::string_view msCheckData) -> void
	{
		m_msCheckData = msCheckData;
	}
}