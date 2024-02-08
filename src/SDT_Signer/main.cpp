#include <iostream>
#include <filesystem>

#include <Valkyria/SDT_Signer.h>


static void UserMain()
{

}

static void DebugMain()
{
	std::string key = "VAL-0064";
	size_t title_len = 0x20;

	Valkyria::SDT::Signer signer{ key,title_len };

	Rut::RxMem::Auto buffer;
	std::filesystem::create_directory(L"sdt_signed/");
	for (auto& entry : std::filesystem::directory_iterator(L"make/"))
	{
		if (entry.is_regular_file() == false) { continue; }
		if (entry.path().extension() != L".sdt") { continue; }

		buffer.LoadFile(entry.path().wstring());
		signer.Sign(buffer);
		buffer.SaveData(L"sdt_signed/" + entry.path().filename().wstring());
	}
}

int main()
{
	try
	{
		DebugMain();
	}
	catch (const std::runtime_error& err)
	{
		std::cerr << err.what() << std::endl;
	}
}
