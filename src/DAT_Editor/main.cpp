#include <iostream>

#include <Rut/RxCmd.h>
#include <Valkyria/DAT_Editor.h>


static void UserMain(int argc, wchar_t* argv[])
{
	try
	{
		Rut::RxCmd::Arg arg;
		arg.AddCmd(L"-dat", L"dat path");
		arg.AddCmd(L"-dir", L"files dir");
		arg.AddCmd(L"-mode", L"mode: [unpack] [repack]");
		arg.AddExample(L"-mode unpack -dat data06-00.dat -dir data06/");
		arg.AddExample(L"-mode repack -dir data06/ -dat data06-01.dat");
		if (arg.Load(argc, argv) == false) { return; }

		if (arg[L"-mode"] == L"unpack")
		{
			Valkyria::DAT::Unpack(arg[L"-dat"], arg[L"-dir"]);
		}
		else if (arg[L"-mode"] == L"repack")
		{
			Valkyria::DAT::Repack(arg[L"-dir"], arg[L"-dat"]);
		}
	}
	catch (const std::exception& err)
	{
		std::cerr << err.what() << std::endl;
	}
}

int wmain(int argc, wchar_t* argv[])
{
	::UserMain(argc, argv);
}