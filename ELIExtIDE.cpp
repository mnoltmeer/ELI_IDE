/*!
Copyright 2019-2020 Maxim Noltmeer (m.noltmeer@gmail.com)

This file is part of ELI IDE.

	ELI IDE is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	ELI IDE is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with ELI IDE.  If not, see <http://www.gnu.org/licenses/>.
*/
//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
//---------------------------------------------------------------------------
#include <Vcl.Styles.hpp>
#include <Vcl.Themes.hpp>
USEFORM("sethost.cpp", SetHostAppForm);
USEFORM("paramstack.cpp", ShowParamStackForm);
USEFORM("objstack.cpp", ShowObjStackForm);
USEFORM("main.cpp", ELIExtIDEForm);
USEFORM("varstack.cpp", ShowVarStackForm);
USEFORM("settings.cpp", IDESettings);
USEFORM("debug.cpp", DebugOutputForm);
USEFORM("classstack.cpp", ShowClassStackForm);
USEFORM("functionstack.cpp", ShowFuncStackForm);
//---------------------------------------------------------------------------
int WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
	try
	{
		Application->Initialize();
		Application->MainFormOnTaskBar = true;
		Application->CreateForm(__classid(TELIExtIDEForm), &ELIExtIDEForm);
		Application->CreateForm(__classid(TIDESettings), &IDESettings);
		Application->CreateForm(__classid(TDebugOutputForm), &DebugOutputForm);
		Application->CreateForm(__classid(TShowVarStackForm), &ShowVarStackForm);
		Application->CreateForm(__classid(TShowObjStackForm), &ShowObjStackForm);
		Application->CreateForm(__classid(TShowClassStackForm), &ShowClassStackForm);
		Application->CreateForm(__classid(TShowParamStackForm), &ShowParamStackForm);
		Application->CreateForm(__classid(TShowFuncStackForm), &ShowFuncStackForm);
		Application->CreateForm(__classid(TSetHostAppForm), &SetHostAppForm);
		Application->Run();
	}
	catch (Exception &exception)
	{
		Application->ShowException(&exception);
	}
	catch (...)
	{
		try
		{
			throw Exception("");
		}
		catch (Exception &exception)
		{
			Application->ShowException(&exception);
		}
	}
	return 0;
}
//---------------------------------------------------------------------------
