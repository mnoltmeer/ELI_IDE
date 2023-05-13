/*!
Copyright 2020 Maxim Noltmeer (m.noltmeer@gmail.com)

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
#include <windows.h>
#include <System.Classes.hpp>

#pragma hdrstop
#pragma argsused

#include <tchar.h>
#include <stdio.h>
#include <string.h>

#define BUILD_APP
#include "..\ELI\eli_script.h"
#include "..\work-functions\MyFunc.h"

int h_debug;
wchar_t dll_path[4096], script_path[4096], params[4096];
String LogPath;
extern String UsedAppLogDir;
//---------------------------------------------------------------------------

int _tmain(int argc, _TCHAR* argv[])
{
  int res;

  try
	 {
	   LogPath = GetEnvironmentVariable("USERPROFILE") + "\\Documents\\ELI";

       UsedAppLogDir = "ELI";

	   if (!DirectoryExists(LogPath))
		 CreateDir(LogPath);

	   if (argc <= 1)
		 {
		   std::wcout << "No params! Using:"
					  << std::endl
					  << "et.exe" << std::endl
					  << "\t<dll path> " << std::endl
					  << "\t<script path> " << std::endl
					  << "\t<param1;param2...;paramn> " << std::endl
					  << std::endl;

		   return 0;
		 }

	   try
		  {
			wcscpy(dll_path, argv[1]);
		  }
	   catch (Exception &e)
		 {
		   throw Exception("Can't initialise <dll path>");
		 }

	   try
		  {
			wcscpy(script_path, argv[2]);
		  }
	   catch (Exception &e)
		 {
		   throw Exception("Can't initialise <script path>");
		 }

       try
		  {
			if (argc >= 4)
			  wcscpy(params, argv[3]);
		  }
	   catch (Exception &e)
		 {
		   throw Exception("Can't initialise <params>");
		 }

	   std::unique_ptr<ELIScript> script(new ELIScript(dll_path));

       std::wcout << std::endl
				  << "[Interpreter version: "
				  << script->Interpreter->GetVersion()
				  << "]"
				  << std::endl
				  << std::endl;

	   script->LoadFromFile(script_path);
	   script->Params = params;
	   script->SaveLogInFile = true;

	   std::wcout << "Translating..." << std::endl << std::endl;

	   SaveToFile(LogPath + "\\translate.log", "");

	   res = script->Run();

	   if (res == 0)
		 std::wcout << std::endl << "Translate: FAILED" << std::endl;
	   else
		 std::wcout << std::endl << "Translate: SUCCESS"  << std::endl << "Result: " << script->Result << std::endl;

	   std::wcout << script->Log.c_str();

	   std::unique_ptr<TStringList> msg(new TStringList());

	   msg->LoadFromFile(LogPath + "\\translate.log", TEncoding::UTF8);
	   std::wcout << std::endl << msg->Text.c_str();
	 }
  catch (Exception &e)
	 {
	   SaveLogToUserFolder("et.log", "ELI", "Initialisation error: " + e.ToString());
	   res = -1;
	 }

  std::wcout << std::endl << "Finishing with result: " << res << std::endl;

  return res;
}
//---------------------------------------------------------------------------
