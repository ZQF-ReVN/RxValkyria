#include <iostream>
#include <filesystem>
#include "../../lib/Valkyria/SDT.h"


int wmain(int argc, wchar_t* argv[])
{
	try
	{
		switch (argc)
		{
		case 1:
		{
			std::string key;
			size_t title_len = 0;

			std::cout << "InputGameKey:";
			std::cin >> key;
			std::cout << "InputGameTitleLen:";
			std::cin >> title_len;

			Valkyria::SDT::Signer signer{ key,title_len };

			Rut::RxMem::Auto buffer;
			std::filesystem::create_directory(L"sdt_signed/");
			for (auto& entry : std::filesystem::directory_iterator(argv[1]))
			{
				if (entry.is_regular_file() == false) { continue; }
				if (entry.path().extension() != L".sdt") { continue; }

				buffer.LoadFile(entry.path().wstring());
				signer.Sign(buffer);
				buffer.SaveData(L"sdt_signed/" + entry.path().filename().wstring());
			}

			system("pause");

		}
		break;

		default:
		{
			std::cout << "";
		}
		}
	}
	catch (const std::runtime_error& err)
	{
		std::cerr << err.what() << std::endl;
	}
}
