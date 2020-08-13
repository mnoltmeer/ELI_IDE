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
#include "..\..\..\ELI\eli_interface.h"
#include "..\..\..\MyFunc\MyFunc.h"

HINSTANCE dllhandle;

GETELIINTERFACE GetELI;
FREEELIINTERFACE FreeELI;

ELI_INTERFACE *eIface;

bool debugging;
int h_log, h_debug, h_varstack, h_objstack, h_clstack, h_prmstack, h_funcstack;
wchar_t dll_path[4096], script_path[4096], params[4096];
String app_dir;
//---------------------------------------------------------------------------

void SendStackContent(TStringList *stack, HWND h)
{
  try
	 {
	   SendMessage(h, WM_SETTEXT, sizeof(stack->Text.c_str()), (LPARAM)stack->Text.c_str());
	 }
  catch (Exception &e)
	{
	  SaveLog(app_dir + "\\exceptions.log", "et::SendStackContent():" + e.ToString());
	}
}
//---------------------------------------------------------------------------

void ExportStacks()
{
  try
	 {
	   TStringList *msg = new TStringList();

	   try
		  {
			if (h_varstack)
			  {
				StrToList(msg, eIface->ShowVarStack());
				SendStackContent(msg, (HWND)h_varstack);
			  }

			if (h_objstack)
			  {
				StrToList(msg, eIface->ShowObjStack());
				SendStackContent(msg, (HWND)h_objstack);
			  }

			if (h_clstack)
			  {
				StrToList(msg, eIface->ShowClassStack());
				SendStackContent(msg, (HWND)h_clstack);
			  }

			if (h_prmstack)
			  {
				StrToList(msg, eIface->ShowParamStack());
				SendStackContent(msg, (HWND)h_prmstack);
			  }

			if (h_funcstack)
			  {
				StrToList(msg, eIface->ShowFuncStack());
				SendStackContent(msg, (HWND)h_funcstack);
			  }
		  }
	   __finally {delete msg;}
	 }
  catch (Exception &e)
	{
	  SaveLog(app_dir + "\\exceptions.log", "et::ExportStacks():" + e.ToString());
	}
}
//---------------------------------------------------------------------------

void CreateTranslateLog()
{
  try
	 {
	   TStringList *msg = new TStringList();

	   try
		  {
			if (h_log)
			  {
				StrToList(msg, eIface->ShowInfoMessages());
				SendStackContent(msg, (HWND)h_log);
			  }

			if (h_debug)
			  {
				msg->LoadFromFile(app_dir + "\\translate.log", TEncoding::UTF8);
				SendStackContent(msg, (HWND)h_debug);
			  }
		  }
	   __finally {delete msg;}
	 }
  catch (Exception &e)
	{
	  SaveLog(app_dir + "\\exceptions.log", "et::CreateTranslateLog():" + e.ToString());
	}
}
//---------------------------------------------------------------------------

int _tmain(int argc, _TCHAR* argv[])
{
  int res;

  try
	 {
	   app_dir = Application->ExeName;
	   int pos = app_dir.LastDelimiter("\\");
	   app_dir.Delete(pos, app_dir.Length() - (pos - 1));

	   wcscpy(dll_path, argv[1]);
	   wcscpy(script_path, argv[2]);
	   wcscpy(params, argv[3]);
	   debugging = StrToInt(argv[4]);

	   h_log = StrToInt(argv[5]);
	   h_debug = StrToInt(argv[6]);
	   h_varstack = StrToInt(argv[7]);
	   h_objstack = StrToInt(argv[8]);
	   h_clstack = StrToInt(argv[9]);
	   h_prmstack = StrToInt(argv[10]);
	   h_funcstack = StrToInt(argv[11]);

	   dllhandle = LoadLibrary(dll_path);

	   if (NULL == dllhandle)
		 throw new Exception("No DLL!");
	   else
		 {
		   GetELI = (GETELIINTERFACE) GetProcAddress(dllhandle, "GetELIInterface");
		   FreeELI = (FREEELIINTERFACE) GetProcAddress(dllhandle, "FreeELIInterface");

		   if (!GetELI)
			 throw new Exception("Error GetELI");

		   if (!FreeELI)
			 throw new Exception("Error FreeELI");

		   if (GetELI(&eIface) == 0)
			 throw new Exception("Error interface ELI_INTERFACE");

		   std::wcout << std::endl
					 << "[Interpreter version: "
					 << String(eIface->GetVersion()).c_str()
					 << "]"
					 << std::endl
					 << std::endl;


		   if (debugging)
			 SaveToFile(app_dir + "\\translate.log", "");

		   const wchar_t *result;

		   std::wcout << "Translating..." << std::endl << std::endl;

		   result = eIface->RunScriptFromFile(script_path, params, debugging);

		   if (_wcsicmp(result, L"-err-") == 0)
			 res = 0;
		   else
			 res = 1;

           if (res == 0)
			 {
			   MessageBox(NULL, L"Translate: FAILED", L"Result", MB_ICONERROR | MB_OK);
               CreateTranslateLog();
			   ExportStacks();
			 }
           else
			 {
			   MessageBox(NULL, L"Translate: OK", L"Result", MB_ICONINFORMATION | MB_OK);

			   CreateTranslateLog();
			   ExportStacks();
			 }
         }
	 }
  catch (Exception &e)
	 {
	   SaveLog(app_dir + "\\exceptions.log", "et::main(): Initialisation error " + e.ToString());
	   res = -1;
	 }

  std::wcout << "Finishing with result: " << res << std::endl;

  return res;
}
//---------------------------------------------------------------------------
