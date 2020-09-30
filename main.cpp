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

#include "..\work-functions\MyFunc.h"
#include "..\ELI\eli_interface.h"
#include "ELISourceHighlighter.h"
#include "ELICodeInsight.h"
#include "debug.h"
#include "settings.h"
#include "objstack.h"
#include "varstack.h"
#include "classstack.h"
#include "paramstack.h"
#include "functionstack.h"
#include "sethost.h"
#include "main.h"

String cur_proj_path, ide_dir, LogPath;
bool changed; //сповіщає про внесення змін у код скрипту

bool FullScreen;
int LogHeight; //визначає висоту логу
String FontName;
int FontSize;
TColor CurrColor;
String Theme;
String InterpreterPath;
bool SyntaxHighlight, CodeInsight, AutoCodeInsight;
int SpacesForTabs;
int EliStatus;
String Version;
String HostApplication;

HINSTANCE dllhandle;

GETELIINTERFACE GetELI;
FREEELIINTERFACE FreeELI;

ELI_INTERFACE *eIface;

TStringList *LastFiles;
TStringList *HostApplicationList;
TMenuItem *MenuLastFiles[5];
TPopupMenu *CodeInsightPopup;
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TELIExtIDEForm *ELIExtIDEForm;
//---------------------------------------------------------------------------

String CreateProjName(String proj_path)
{
  String name = proj_path;
  int pos = name.LastDelimiter("\\");

  name.Delete(1, pos);

  return name;
}
//---------------------------------------------------------------------------

__fastcall TELIExtIDEForm::TELIExtIDEForm(TComponent* Owner)
	: TForm(Owner)
{
  LastFiles = new TStringList();
  HostApplicationList = new TStringList();

  Version = GetVersionInString(Application->ExeName.c_str());

  Caption = "ELI IDE " + Version + " - <new>";

  LogPath = GetEnvironmentVariable("USERPROFILE") + "\\Documents";

  ide_dir = Application->ExeName;
  int pos = ide_dir.LastDelimiter("\\");
  ide_dir.Delete(pos, ide_dir.Length() - (pos - 1));

  ReadSettings();
  HostApplication = "<none>";
  CreateMenuLastFiles();
  InitLexems();
  ChangeTheme(Theme);

  EliStatus = ConnectELI();

  BtNew->Glyph = Images->GetBitmap(8, 20, 20);
  BtOpen->Glyph = Images->GetBitmap(9, 22, 22);
  BtSave->Glyph = Images->GetBitmap(11, 20, 20);
  BtRun->Glyph = Images->GetBitmap(13, 25, 25);
  BtRunTo->Glyph = Images->GetBitmap(12, 25, 25);
  BtBreak->Glyph = Images->GetBitmap(14, 22, 22);
  PPTranslateToLine->Bitmap = Images->GetBitmap(12, 16, 16);
  MenuTranslate->Bitmap = Images->GetBitmap(13, 16, 16);
  MenuTranslateToCursor->Bitmap = Images->GetBitmap(12, 16, 16);
  MenuStopTranslate->Bitmap = Images->GetBitmap(14, 16, 16);
  MenuNew->Bitmap = Images->GetBitmap(8, 16, 16);
  MenuLoad->Bitmap = Images->GetBitmap(9, 16, 16);
  MenuSave->Bitmap = Images->GetBitmap(10, 16, 16);
  MenuSaveAs->Bitmap = Images->GetBitmap(10, 16, 16);

  cur_proj_path = "";

  changed = false;

  SaveProject->InitialDir = GetCurrentDir();
  OpenProject->InitialDir = GetCurrentDir();

  if (FullScreen)
	WindowState = wsMaximized;
  else
	WindowState = wsNormal;
}
//---------------------------------------------------------------------------

void __fastcall TELIExtIDEForm::AddLastFile(String file)
{
  try
	 {
	   int exists_file_ind = LastFiles->IndexOf(file);

	   if (exists_file_ind > -1)
		 {
		   LastFiles->Delete(exists_file_ind);
		   LastFiles->Insert(0, file);
		 }
	   else
		 LastFiles->Insert(0, file);

	   while (LastFiles->Count > 5)
		 LastFiles->Delete(LastFiles->Count - 1);

	   WriteLastFilesList();
	 }
  catch (Exception &e)
	 {
	   SaveLog(LogPath + "\\exceptions.log", "AddLastFile: " + e.ToString());
	 }
}
//---------------------------------------------------------------------------

void __fastcall TELIExtIDEForm::ShowLastFiles()
{
  ReadLastFilesList();

  try
	 {
	   for (int i = 0; i < LastFiles->Count; i++)
		  {
			MenuLastFiles[i]->Caption = LastFiles->Strings[i];
            MenuLastFiles[i]->Visible = true;
		  }
	 }
  catch (Exception &e)
	 {
	   SaveLog(LogPath + "\\exceptions.log", "ShowLastFiles: " + e.ToString());
	 }
}
//---------------------------------------------------------------------------

void __fastcall TELIExtIDEForm::CreateMenuLastFiles()
{
  for (int i = 0; i < 5; i++)
	 {
	   MenuLastFiles[i] = new TMenuItem(MainMenu);
	   MainMenu->Items->Find("File")->Add(MenuLastFiles[i]);
	   MenuLastFiles[i]->Tag = i;
	   MenuLastFiles[i]->Visible = false;
	   MenuLastFiles[i]->OnClick = PPLastFileClick;
	 }
}
//---------------------------------------------------------------------------

int __fastcall TELIExtIDEForm::ConnectELI()
{
  int res;

  if (!dllhandle)
	{
      try
		 {
		   dllhandle = LoadLibrary(InterpreterPath.c_str());
		 }
	  catch (Exception &e)
		 {
		   Log->Lines->Add("ConnectELI: " + e.ToString());
		   Log->Lines->Add("Error connecting to ELI library");
		   res = -1;
		 }
	}

  if (dllhandle)
	{
	  try
		 {
           GetELI = (GETELIINTERFACE) GetProcAddress(dllhandle, "GetELIInterface");
	  	   FreeELI = (FREEELIINTERFACE) GetProcAddress(dllhandle, "FreeELIInterface");
		 }
	  catch (Exception &e)
		 {
		   Log->Lines->Add("ConnectELI: " + e.ToString());
		   res = -1;
		 }

	  if (!GetELI)
		{
		  Log->Lines->Add("Error initialisation GetELI");
		  res = -2;
		}
	  else if (!FreeELI)
		{
		  Log->Lines->Add("Error initialisation FreeELI");
		  res = -3;
		}
	  else if (eIface)
		{
          Log->Lines->Add("ELI_INTERFACE initialised");
		  res = 0;
        }
	  else if (!GetELI(&eIface))
		{
		  Log->Lines->Add("Error initialisation ELI_INTERFACE");
		  res = -4;
		}
	  else
		{
		  Log->Lines->Add("ELI_INTERFACE initialised");
		  res = 0;
		}
	}
  else
	{
      Log->Lines->Add("Error connecting to ELI library");
	  res = -1;
    }

  return res;
}
//---------------------------------------------------------------------------

int __fastcall TELIExtIDEForm::ReleaseELI()
{
  int res;

  if (eIface)
	{
      if (!FreeELI(&eIface))
		{
		  Log->Lines->Add("ELI_INTERFACE error");
		  res = -1;
		}
	  else
		{
		  Log->Lines->Add("ELI_INTERFACE released");
		  res = 0;
		}
	}

  FreeLibrary(dllhandle);
  dllhandle = NULL;

  return res;
}
//---------------------------------------------------------------------------

void __fastcall TELIExtIDEForm::ChangeTheme(String name)
{
  try
	 {
	   TStyleManager::SetStyle(name);

	   if ((Theme == "Silver") || (Theme == "Windows10"))
		  {
			Font->Color = clBlack;
			CurrColor = clBlack;
			InitExprColors(ESH_LIGHT_THEME);
		  }
		else
		  {
			Font->Color = clWhite;
			CurrColor = clWhite;
			InitExprColors(ESH_DARK_THEME);
		  }
	 }
  catch (Exception &e)
	 {
	   SaveLog(LogPath + "\\exceptions.log", "ChangeTheme: " + e.ToString());
	 }
}
//---------------------------------------------------------------------------

void __fastcall TELIExtIDEForm::LockUIByELI(bool block)
{
  try
	 {
	   if (block)
		  {
			MenuTranslate->Enabled = false;
			MenuTranslateToCursor->Enabled = false;
			MenuStopTranslate->Enabled = false;
			MenuDebugTranslate->Enabled = false;
			MenuShowVarStack->Enabled = false;
			MenuShowObjStack->Enabled = false;
			MenuShowClassStack->Enabled = false;
			MenuShowParamStack->Enabled = false;
			MenuShowFuncStack->Enabled = false;
			MenuSetHostApp->Enabled = false;
			BtDebug->Enabled = false;
			BtVarStack->Enabled = false;
			BtObjStack->Enabled = false;
			BtClassStack->Enabled = false;
			BtParamStack->Enabled = false;
			BtFuncStack->Enabled = false;
			BtRun->Enabled = false;
			BtRunTo->Enabled = false;
			BtBreak->Enabled = false;
		  }
		else
		  {
			MenuTranslate->Enabled = true;
			MenuTranslateToCursor->Enabled = true;
			MenuStopTranslate->Enabled = true;
			MenuDebugTranslate->Enabled = true;
			MenuShowVarStack->Enabled = true;
			MenuShowObjStack->Enabled = true;
			MenuShowClassStack->Enabled = true;
			MenuShowParamStack->Enabled = true;
			MenuShowFuncStack->Enabled = true;
            MenuSetHostApp->Enabled = true;
			BtDebug->Enabled = true;
			BtVarStack->Enabled = true;
			BtObjStack->Enabled = true;
			BtClassStack->Enabled = true;
			BtParamStack->Enabled = true;
			BtFuncStack->Enabled = true;
			BtRun->Enabled = true;
			BtRunTo->Enabled = true;
            BtBreak->Enabled = true;
		  }
	 }
  catch (Exception &e)
	 {
	   SaveLog(LogPath + "\\exceptions.log", "LockUIByELI: " + e.ToString());
	 }
}
//---------------------------------------------------------------------------

void __fastcall TELIExtIDEForm::PPLastFileClick(TObject *Sender)
{
  try
	 {
	   TMenuItem *menu = dynamic_cast<TMenuItem*>(Sender);

	   SaveCurrentProject();
	   Editor->Clear();
	   Params->Clear();
	   LineNum->Clear();
	   ColNum->Clear();
	   Log->Clear();

	   Editor->Lines->LoadFromFile(LastFiles->Strings[menu->Tag], TEncoding::UTF8);

       Editor->SelectAll();
	   Editor->SelAttributes->Assign(IDESettings->FontDialog->Font);
	   Editor->SelLength = 0;

	   cur_proj_path = LastFiles->Strings[menu->Tag];
	   changed = false;
       BtSave->Glyph = Images->GetBitmap(11, 20, 20);

//посилаємо спец. пустий символ для підсвітки
	   if (SyntaxHighlight)
		 SendMessage(Editor->Handle, WM_KEYUP, 0, NULL);

       Caption = "ELI IDE " + Version + " - " + CreateProjName(LastFiles->Strings[menu->Tag]);

	   AddLastFile(LastFiles->Strings[menu->Tag]);
	 }
  catch (Exception &e)
	 {
	   SaveLog(LogPath + "\\exceptions.log", "PPLastFileClick: " + e.ToString());
	 }
}
//---------------------------------------------------------------------------

void __fastcall TELIExtIDEForm::PPCodeInsightMenuClick(TObject *Sender)
{
  try
	 {
	   TMenuItem *menu = dynamic_cast<TMenuItem*>(Sender);

	   String lexem = ReplaceFragmentLexemeInLine(menu->Hint, Editor->CaretPos.Y, Editor->CaretPos.X);

	   if (lexem != "")
		 Editor->Lines->Strings[Editor->CaretPos.Y] = lexem;

       if (SyntaxHighlight)
		 HighlightSource(Editor, Editor->CaretPos.Y);
	 }
  catch (Exception &e)
	 {
	   SaveLog(LogPath + "\\exceptions.log", "PPCodeInsightMenuClick: " + e.ToString());
	 }
}
//---------------------------------------------------------------------------

void __fastcall TELIExtIDEForm::SetLog(int left, int top, int width, int height)
{
  Log->Left = left;
  Log->Top = top;
  Log->ClientWidth = width;
  Log->ClientHeight = height;
}
//---------------------------------------------------------------------------

void __fastcall TELIExtIDEForm::SetPanel(int left, int top, int width, int height)
{
  BtnPanel->Left = left;
  BtnPanel->Top = top;
  BtnPanel->ClientWidth = width;
  BtnPanel->ClientHeight = height;
}
//---------------------------------------------------------------------------

void __fastcall TELIExtIDEForm::SetEditor(int left, int top, int width, int height)
{
  Editor->Left = left;
  Editor->Top = top;
  Editor->ClientWidth = width;
  Editor->ClientHeight = height;
}
//---------------------------------------------------------------------------

void __fastcall TELIExtIDEForm::AutoScaleUI()
{
  int EditorTop, CountersTop;

  if (MenuShowPanel->Checked)
	{
	  SetPanel(0, 0, 400, 24);
	  BtnPanel->Show();
	  EditorTop = BtnPanel->ClientHeight;
	}
  else
	{
	  SetPanel(0, 0, 0, 0);
	  BtnPanel->Hide();
	  EditorTop = 0;
	}

  if (MenuShowLog->Checked)
	{
	  SetLog(0, ClientHeight - 205, ClientWidth - 20, 200);
	  Log->Show();
	  CountersTop = Log->Top - LineNum->ClientHeight - 8;
	}
  else
	{
	  SetLog(0, 0, 0, 0);
	  Log->Hide();
	  CountersTop = ClientHeight - LineNum->ClientHeight - 8;
	}

  LineNum->Left = Editor->Left;
  ColNum->Left = LineNum->Left + LineNum->Width + 10;
  LineNum->Top = CountersTop;
  ColNum->Top = CountersTop;
  Label1->Top = CountersTop;
  LbParam->Top = CountersTop;
  Params->Top = CountersTop;

  SetEditor(0,
			EditorTop,
			ClientWidth - 4,
			ClientHeight - EditorTop - (ClientHeight - CountersTop + 5));
}
//---------------------------------------------------------------------------

void __fastcall TELIExtIDEForm::PPCopyClick(TObject *Sender)
{
  Editor->CopyToClipboard();
}
//---------------------------------------------------------------------------

void __fastcall TELIExtIDEForm::PPCutClick(TObject *Sender)
{
  Editor->CutToClipboard();

  if (SyntaxHighlight)
	SendMessage(Editor->Handle, WM_KEYUP, 0, NULL);
}
//---------------------------------------------------------------------------

void __fastcall TELIExtIDEForm::PPPasteClick(TObject *Sender)
{
  Editor->PlainText = true;

  Editor->PasteFromClipboard();

  Editor->PlainText = false;

  if (SyntaxHighlight)
	SendMessage(Editor->Handle, WM_KEYUP, 0, NULL);
}
//---------------------------------------------------------------------------

void __fastcall TELIExtIDEForm::PPSelectAllClick(TObject *Sender)
{
  Editor->SelStart = 0;
  Editor->SelLength = Editor->Text.Length();
}
//---------------------------------------------------------------------------

void __fastcall TELIExtIDEForm::FormClose(TObject *Sender, TCloseAction &Action)
{
  SaveCurrentProject();
  WriteSettings();
  ReleaseELI();
  SaveToFile(LogPath + "\\translate.log", "");
  delete LastFiles;
  delete HostApplicationList;
}
//---------------------------------------------------------------------------

void __fastcall TELIExtIDEForm::FormShow(TObject *Sender)
{
  try
	 {
	   AutoScaleUI();
	   IDESettings->FontDialog->Font->Name = FontName;
	   IDESettings->FontDialog->Font->Size = FontSize;
	   IDESettings->FontDialog->Font->Color = CurrColor;

	   if (ParamStr(1) != "")
		 {
		   cur_proj_path = ParamStr(1);
		   Editor->Lines->LoadFromFile(cur_proj_path, TEncoding::UTF8);

		   Caption = "ELI IDE " + Version + " - " + CreateProjName(OpenProject->FileName);
		   changed = false;
           BtSave->Glyph = Images->GetBitmap(11, 20, 20);

		   AddLastFile(cur_proj_path);
		 }

	   Editor->Font->Assign(IDESettings->FontDialog->Font);
	   Editor->SelectAll();
	   Editor->SelAttributes->Assign(IDESettings->FontDialog->Font);
	   Editor->SelLength = 0;

       if (EliStatus < 0)
		 LockUIByELI(true);

	   if (MenuDebugTranslate->Checked && MenuDebugTranslate->Enabled)
		 {
		   BtDebug->Glyph = Images->GetBitmap(7, 25, 25);
		   DebugOutputForm->Show();
		 }
	   else
		 BtDebug->Glyph = Images->GetBitmap(6, 25, 25);

	   if (MenuShowVarStack->Checked && MenuShowVarStack->Enabled)
		 {
		   BtVarStack->Glyph = Images->GetBitmap(1, 25, 25);
		   ShowVarStackForm->Show();
		 }
	   else
		 BtVarStack->Glyph = Images->GetBitmap(0, 25, 25);

	   if (MenuShowObjStack->Checked && MenuShowObjStack->Enabled)
		 {
		   BtObjStack->Glyph = Images->GetBitmap(2, 25, 25);
		   ShowObjStackForm->Show();
		 }
	   else
		 BtObjStack->Glyph = Images->GetBitmap(0, 25, 25);

	  if (MenuShowClassStack->Checked && MenuShowClassStack->Enabled)
		{
		  BtClassStack->Glyph = Images->GetBitmap(3, 25, 25);
		  ShowClassStackForm->Show();
		}
	  else
		BtClassStack->Glyph = Images->GetBitmap(0, 25, 25);

	  if (MenuShowParamStack->Checked && MenuShowParamStack->Enabled)
		{
		  BtParamStack->Glyph = Images->GetBitmap(4, 25, 25);
		  ShowParamStackForm->Show();
		}
	  else
		BtParamStack->Glyph = Images->GetBitmap(0, 25, 25);

	  if (MenuShowFuncStack->Checked && MenuShowFuncStack->Enabled)
		{
		  BtFuncStack->Glyph = Images->GetBitmap(5, 25, 25);
		  ShowFuncStackForm->Show();
		}
	  else
		BtFuncStack->Glyph = Images->GetBitmap(0, 25, 25);

//посилаємо спец. пустий символ для підсвітки
	   if (SyntaxHighlight)
		 SendMessage(Editor->Handle, WM_KEYUP, 0, NULL);
	 }
  catch (Exception &e)
	 {
	   SaveLog(LogPath + "\\exceptions.log", "FormShow: " + e.ToString());
	 }
}
//---------------------------------------------------------------------------

void __fastcall TELIExtIDEForm::WriteLastFilesList()
{
  try
	 {
	   TRegistry *reg = new TRegistry();

	   try
		  {
			reg->RootKey = HKEY_CURRENT_USER;

			if (!reg->KeyExists("Software\\ELI\\UI"))
			  reg->CreateKey("Software\\ELI\\UI");

			if (reg->OpenKey("Software\\ELI\\UI", false))
			  {
				String list = ListToStr(LastFiles, ";");

				reg->WriteString("LastFiles", list);
			  }
		  }
	   __finally {delete reg;}
	 }
  catch (Exception &e)
	 {
	   SaveLog(LogPath + "\\exceptions.log", "WriteLastFilesList: " + e.ToString());
	 }
}
//---------------------------------------------------------------------------

void __fastcall TELIExtIDEForm::ReadLastFilesList()
{
  try
	 {
	   TRegistry *reg = new TRegistry();

	   try
		  {
			reg->RootKey = HKEY_CURRENT_USER;

			if (reg->OpenKey("Software\\ELI\\UI", false))
			  StrToList(LastFiles, reg->ReadString("LastFiles"), ";");
		  }
	   __finally {delete reg;}
	 }
  catch (Exception &e)
	 {
	   SaveLog(LogPath + "\\exceptions.log", "ReadLastFilesList: " + e.ToString());
	 }
}
//---------------------------------------------------------------------------

void __fastcall TELIExtIDEForm::ReadSettings()
{
  try
	 {
       TRegistry *reg = new TRegistry();

	   try
		  {
			reg->RootKey = HKEY_CURRENT_USER;

            if (reg->OpenKey("Software\\ELI\\Editor", false))
			  {
				if (reg->ValueExists("FontName"))
				  FontName = reg->ReadString("FontName");
				else
				  {
					FontName = "Tahoma";
					reg->WriteString("FontName", "Tahoma");
				  }

				if (reg->ValueExists("FontSize"))
				  FontSize = reg->ReadInteger("FontSize");
				else
				  {
					FontSize = 10;
					reg->WriteInteger("FontSize", 10);
				  }

				if (reg->ValueExists("Theme"))
				  Theme = reg->ReadString("Theme");
				else
				  {
					Theme = "Windows10";
					reg->WriteString("Theme", "Windows10");
				  }

				if (reg->ValueExists("SyntaxHighlight"))
				  SyntaxHighlight = reg->ReadBool("SyntaxHighlight");
				else
				  {
					SyntaxHighlight = true;
					reg->WriteBool("SyntaxHighlight", true);
				  }

				if (reg->ValueExists("CodeInsight"))
				  CodeInsight = reg->ReadBool("CodeInsight");
				else
				  {
					CodeInsight = true;
					reg->WriteBool("CodeInsight", true);
				  }

				if (reg->ValueExists("AutoCodeInsight"))
				  AutoCodeInsight = reg->ReadBool("AutoCodeInsight");
				else
				  {
					AutoCodeInsight = true;
					reg->WriteBool("AutoCodeInsight", true);
				  }

				if (reg->ValueExists("SpacesForTabs"))
				  SpacesForTabs = reg->ReadInteger("SpacesForTabs");
				else
				  {
					SpacesForTabs = 2;
					reg->WriteInteger("SpacesForTabs", 2);
				  }

				reg->CloseKey();
			  }

			if (reg->OpenKey("Software\\ELI\\UI", false))
			  {
				if (reg->ValueExists("FullScreen"))
				  FullScreen = reg->ReadBool("FullScreen");

				if (reg->ValueExists("ShowLog"))
				  MenuShowLog->Checked = reg->ReadBool("ShowLog");

				if (reg->ValueExists("ShowPanel"))
				  MenuShowPanel->Checked = reg->ReadBool("ShowPanel");

				if (reg->ValueExists("LastFiles"))
				  ReadLastFilesList();
				else
				  reg->WriteString("LastFiles", "");

				if (reg->ValueExists("FormHeight"))
				  ClientHeight = reg->ReadInteger("FormHeight");
				else
				  {
					ClientHeight = 600;
					reg->WriteInteger("FormHeight", 600);
				  }

				if (reg->ValueExists("FormWidth"))
				  ClientWidth = reg->ReadInteger("FormWidth");
				else
				  {
                    ClientWidth = 800;
					reg->WriteInteger("FormWidth", 800);
				  }

                reg->CloseKey();
			  }

			if (reg->OpenKey("Software\\ELI\\Translate", false))
			  {
                if (reg->ValueExists("InterpreterPath"))
				  InterpreterPath = reg->ReadString("InterpreterPath");
				else
				  {
					InterpreterPath = ide_dir + "\\ELI.dll";
					reg->WriteString("InterpreterPath", InterpreterPath);
				  }

				if (reg->ValueExists("ShowVarStackAfterTranslate"))
				  MenuShowVarStack->Checked = reg->ReadBool("ShowVarStackAfterTranslate");
				else
				  MenuShowVarStack->Checked = false;

				if (reg->ValueExists("ShowObjStackAfterTranslate"))
				  MenuShowObjStack->Checked = reg->ReadBool("ShowObjStackAfterTranslate");
                else
				  MenuShowObjStack->Checked = false;

				if (reg->ValueExists("ShowClassStackAfterTranslate"))
				  MenuShowClassStack->Checked = reg->ReadBool("ShowClassStackAfterTranslate");
                else
				  MenuShowClassStack->Checked = false;

				if (reg->ValueExists("ShowParamStackAfterTranslate"))
				  MenuShowParamStack->Checked = reg->ReadBool("ShowParamStackAfterTranslate");
                else
				  MenuShowParamStack->Checked = false;

				if (reg->ValueExists("ShowFuncStackAfterTranslate"))
				  MenuShowFuncStack->Checked = reg->ReadBool("ShowFuncStackAfterTranslate");
                else
				  MenuShowFuncStack->Checked = false;

				if (reg->ValueExists("DebugTranslate"))
				  MenuDebugTranslate->Checked = reg->ReadBool("DebugTranslate");
				else
				  MenuDebugTranslate->Checked = false;

				if (reg->ValueExists("HostApplicationList"))
				  StrToList(HostApplicationList, reg->ReadString("HostApplicationList"), ";");
                else
				  reg->WriteString("HostApplicationList", "<none>");

				reg->CloseKey();
			  }
		  }
	   __finally {delete reg;}
	 }
  catch (Exception &e)
	 {
	   SaveLog(LogPath + "\\exceptions.log", "ReadSettings: " + e.ToString());
	 }
}
//---------------------------------------------------------------------------

void __fastcall TELIExtIDEForm::WriteSettings()
{
  try
	 {
       TRegistry *reg = new TRegistry();

	   try
		  {
			reg->RootKey = HKEY_CURRENT_USER;

			if (!reg->KeyExists("Software\\ELI\\Editor"))
			  reg->CreateKey("Software\\ELI\\Editor");

            if (!reg->KeyExists("Software\\ELI\\UI"))
			  reg->CreateKey("Software\\ELI\\UI");

			if (!reg->KeyExists("Software\\ELI\\Translate"))
			  reg->CreateKey("Software\\ELI\\Translate");

            if (reg->OpenKey("Software\\ELI\\Editor", false))
			  {
				reg->WriteString("FontName", FontName);
				reg->WriteInteger("FontSize", FontSize);
				reg->WriteString("Theme", Theme);
				reg->WriteBool("SyntaxHighlight", SyntaxHighlight);
				reg->WriteBool("CodeInsight", CodeInsight);
				reg->WriteBool("AutoCodeInsight", AutoCodeInsight);
				reg->WriteInteger("SpacesForTabs", SpacesForTabs);

				reg->CloseKey();
			  }

			if (reg->OpenKey("Software\\ELI\\UI", false))
			  {
				reg->WriteInteger("FormHeight", ClientHeight);
				reg->WriteInteger("FormWidth", ClientWidth);
				reg->WriteBool("ShowLog", MenuShowLog->Checked);
				reg->WriteBool("ShowPanel", MenuShowPanel->Checked);

				if (WindowState == wsMaximized)
				  reg->WriteBool("FullScreen", true);
				else
				  reg->WriteBool("FullScreen", false);

				reg->CloseKey();
			  }

			if (reg->OpenKey("Software\\ELI\\Translate", false))
			  {
                reg->WriteString("InterpreterPath", InterpreterPath);
				reg->WriteBool("ShowVarStackAfterTranslate", MenuShowVarStack->Checked);
				reg->WriteBool("ShowObjStackAfterTranslate", MenuShowObjStack->Checked);
				reg->WriteBool("ShowClassStackAfterTranslate", MenuShowClassStack->Checked);
				reg->WriteBool("ShowParamStackAfterTranslate", MenuShowParamStack->Checked);
				reg->WriteBool("ShowFuncStackAfterTranslate", MenuShowFuncStack->Checked);
				reg->WriteBool("DebugTranslate", MenuDebugTranslate->Checked);

                String list = ListToStr(HostApplicationList, ";");

				reg->WriteString("HostApplicationList", list);

				reg->CloseKey();
			  }
		  }
	   __finally {delete reg;}
	 }
  catch (Exception &e)
	 {
	   SaveLog(LogPath + "\\exceptions.log", "WriteSettings: " + e.ToString());
	 }
}
//---------------------------------------------------------------------------

void __fastcall TELIExtIDEForm::CreateCodeInsightMenu(String entity_name)
{
  try
	 {
	   std::vector<Lexeme> *results;

	   try
		  {
			if (CodeInsightPopup)
			  delete CodeInsightPopup;

			CodeInsightPopup = new TPopupMenu(this);
            CodeInsightPopup->AutoHotkeys = maManual;

			results = GetResults(entity_name);

			for (int i = 0; i < results->size(); i++)
			   {
				 TMenuItem *menu_item = new TMenuItem(CodeInsightPopup);

				 menu_item->OnClick = PPCodeInsightMenuClick;
				 menu_item->Caption = results->at(i).Type +
                                      "  " +
									  results->at(i).Signature +
									  " : " +
									  results->at(i).Description;
				 menu_item->Hint = results->at(i).InsertText;

				 CodeInsightPopup->Items->Add(menu_item);
			   }
		  }
	   __finally {delete results;}
	 }
  catch (Exception &e)
	 {
	   SaveLog(LogPath + "\\exceptions.log", "CreateCodeInsightMenu: " + e.ToString());
	 }
}
//---------------------------------------------------------------------------

void __fastcall TELIExtIDEForm::ClearCodeInsightMenu()
{
  try
	 {
	   delete CodeInsightPopup;
	 }
  catch (Exception &e)
	 {
	   SaveLog(LogPath + "\\exceptions.log", "ClearCodeInsightMenu: " + e.ToString());
	 }
}
//---------------------------------------------------------------------------

void __fastcall TELIExtIDEForm::OpenCodeInsightMenu()
{
  try
	 {
	   TPoint cursor;
	   GetCaretPos(&cursor);
	   cursor = Editor->ClientToScreen(cursor);
	   cursor = this->ScreenToClient(cursor);
	   CodeInsightPopup->Popup(cursor.X, cursor.Y + 40);
	 }
  catch (Exception &e)
	 {
	   SaveLog(LogPath + "\\exceptions.log", "OpenCodeInsightMenu: " + e.ToString());
	 }
}
//---------------------------------------------------------------------------

String __fastcall TELIExtIDEForm::ExtractFragmentLexemeFromLine(int line_ind, int cursor_pos)
{
  String res;

  try
	 {
	   res = Editor->Lines->Strings[line_ind]; //беремо рядок
	   res = res.SubString(1, cursor_pos); //обрізаємо до позиції курсору

	   int lex_border = FindLexemBorder(line_ind, cursor_pos);

       res = res.Delete(1, lex_border); //залишаємо саму лексему
	 }
  catch (Exception &e)
	 {
	   SaveLog(LogPath + "\\exceptions.log", "ExtractFragmentFromLine: " + e.ToString());
	   res = "";
	 }

  return res;
}
//---------------------------------------------------------------------------

String __fastcall TELIExtIDEForm::ReplaceFragmentLexemeInLine(const String &lexeme,
															  int line_ind,
															  int cursor_pos)
{
  String res;

  try
	 {
	   res = Editor->Lines->Strings[line_ind]; //беремо рядок

	   int lex_border = res.LastDelimiter(lexeme[1]);

	   res = res.Delete(lex_border, cursor_pos - lex_border + 1); //видаляємо фрагмент лексеми з рядка
	   res = res.Insert(lexeme, lex_border); //вставляємо повну версію лексеми
	 }
  catch (Exception &e)
	 {
	   SaveLog(LogPath + "\\exceptions.log", "ExtractFragmentFromLine: " + e.ToString());
	   res = "";
	 }

  return res;
}
//---------------------------------------------------------------------------

int __fastcall TELIExtIDEForm::FindLexemBorder(int line_ind, int cursor_pos)
{
  int res = 0;

  try
	 {
	   String operstr = Editor->Lines->Strings[line_ind]; //беремо рядок

	   for (int i = cursor_pos; i >= 1; i--)
		  {
			if ((operstr[i] == '_') || (operstr[i] == '#'))
			  {
				res = i - 1;
				break;
			  }
			else if (operstr[i] == '.')
			  {
				for (int j = i; j >= 1; j--)
				   {
					 if (operstr[j] == ' ')
					   break;
					 else if (operstr[j] == '&')
					   {
						 res = i - 1;
						 break;
                       }
				   }

                break;
              }
          }
	 }
  catch (Exception &e)
	 {
	   SaveLog(LogPath + "\\exceptions.log", "FindLexemBorder: " + e.ToString());
	   res = -1;
	 }

  return res;
}
//---------------------------------------------------------------------------

void __fastcall TELIExtIDEForm::ActivateCodeInsight()
{
  try
	 {
	   String fragment = ExtractFragmentLexemeFromLine(Editor->CaretPos.Y, Editor->CaretPos.X);
	   CreateCodeInsightMenu(fragment);
	   OpenCodeInsightMenu();
	   HighlightSource(Editor, Editor->CaretPos.Y);
	 }
  catch (Exception &e)
	 {
	   SaveLog(LogPath + "\\exceptions.log", "ActivateCodeInsight: " + e.ToString());
	 }
}
//---------------------------------------------------------------------------

void __fastcall TELIExtIDEForm::AddCloseBraces(String &text)
{
  try
	 {
	   int open_cnt = CountBraces(text, '{');
	   int cl_cnt = CountBraces(text, '}');

	   for (int i = 0; i < (open_cnt - cl_cnt); i++)
		  text.Insert("}", text.Length() - 1);
	 }
  catch (Exception &e)
	 {
	   SaveLog(LogPath + "\\exceptions.log", "AddCloseBraces: " + e.ToString());
	 }
}
//---------------------------------------------------------------------------

int __fastcall TELIExtIDEForm::CountBraces(const String &text, wchar_t brace)
{
  int cnt = 0;

  try
	 {
	   int ind = 1;

	   while (ind <= text.Length())
		 {
		   if (text[ind] == brace)
			 cnt++;

           ind++;
		 }
	 }
  catch (Exception &e)
	 {
	   SaveLog(LogPath + "\\exceptions.log", "CountBraces: " + e.ToString());
	   cnt = 0;
	 }

  return cnt;
}
//---------------------------------------------------------------------------

void __fastcall TELIExtIDEForm::Translate(String text, String params)
{
  try
	 {
	   SaveToFile(LogPath + "\\prepared.es", text);

	   if (HostApplication != "<none>")
		 {
           String prm = InterpreterPath + " " +
						LogPath + "\\prepared.es " +
						"\"" + params + "\" " +
						String((int)MenuDebugTranslate->Checked);

		   ShellExecute(NULL,
						L"open",
						HostApplication.c_str(),
						L"",
						NULL,
						SW_SHOWNORMAL);
		 }
	   else
		 {
           String prm = InterpreterPath + " " +
						LogPath + "\\prepared.es " +
						"\"" + params + "\" " +
						String((int)MenuDebugTranslate->Checked) + " " +
						String((int)Log->Handle) + " " +
						String((int)DebugOutputForm->DebugLog->Handle) + " " +
						String((int)ShowVarStackForm->Stack->Handle) + " " +
						String((int)ShowObjStackForm->Stack->Handle) + " " +
						String((int)ShowClassStackForm->Stack->Handle) + " " +
						String((int)ShowParamStackForm->Stack->Handle) + " " +
						String((int)ShowFuncStackForm->Stack->Handle);

		   ShellExecute(NULL,
						L"open",
						String(ide_dir + "\\et.exe").c_str(),
						prm.c_str(),
						NULL,
						SW_SHOWNORMAL);
		 }
	 }
  catch (Exception &e)
	 {
	   SaveLog(LogPath + "\\exceptions.log", "Translate: " + e.ToString());
	 }
}
//---------------------------------------------------------------------------

void __fastcall TELIExtIDEForm::CancelHighlight()
{
  try
	 {
	   String tmp = Editor->Text;
	   Editor->Clear();
	   Editor->Text = tmp;
	 }
  catch (Exception &e)
	 {
	   SaveLog(LogPath + "\\exceptions.log", "CancelHiglight: " + e.ToString());
	 }
}
//---------------------------------------------------------------------------

void __fastcall TELIExtIDEForm::FormResize(TObject *Sender)
{
  try
	 {
       AutoScaleUI();
	 }
  catch (Exception &e)
	 {
	   Log->Lines->Add(e.ToString());
	 }
}
//---------------------------------------------------------------------------

void __fastcall TELIExtIDEForm::MenuShowLogClick(TObject *Sender)
{
  MenuShowLog->Checked = !MenuShowLog->Checked;
  AutoScaleUI();
}
//---------------------------------------------------------------------------

void __fastcall TELIExtIDEForm::MenuShowPanelClick(TObject *Sender)
{
  MenuShowPanel->Checked = !MenuShowPanel->Checked;
  AutoScaleUI();
}
//---------------------------------------------------------------------------

void __fastcall TELIExtIDEForm::MenuPreferencesClick(TObject *Sender)
{
  IDESettings->Show();
  IDESettings->SetFocus();
}
//---------------------------------------------------------------------------

void __fastcall TELIExtIDEForm::EditorMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y)
{
  if (Button == mbRight)
	{
	  if (Editor->SelText == "") //не вибрано текст - блокуємо пункти "Вирізати" та "Скопіювати"
		{
		  EditorPopup->Items->Items[0]->Enabled = false;
		  EditorPopup->Items->Items[1]->Enabled = false;
		}
	  else
		{
		  EditorPopup->Items->Items[0]->Enabled = true;
		  EditorPopup->Items->Items[1]->Enabled = true;
		}

	  if (Clipboard()->AsText == "") //нема тексту для вставки, блокуємо "Вставити"
		EditorPopup->Items->Items[2]->Enabled = false;
	  else
		EditorPopup->Items->Items[2]->Enabled = true;

	  EditorPopup->Popup(Left + Editor->Left + X + 18,
						 Top + Editor->Top + Y - 10);
	}
  else
	{
      LineNum->Text = Editor->CaretPos.Y;
	  ColNum->Text = Editor->CaretPos.X;
    }
}
//---------------------------------------------------------------------------

void __fastcall TELIExtIDEForm::EditorKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
  if ((Key == VK_UP) || (Key == VK_DOWN) || (Key == VK_LEFT) || (Key == VK_RIGHT))
	{
	  LineNum->Text = Editor->CaretPos.Y;
	  ColNum->Text = Editor->CaretPos.X;
	}

  if (Key == 32)
	{
	  if (Shift.Contains(ssCtrl) && CodeInsight)
		{
		  ActivateCodeInsight();
		}
	}
  else if (Key == 120) //F9
	{
	  MenuTranslate->Click();
	}
  else if (Key == 119) //F8
	{
      MenuTranslateToCursor->Click();
	}
  else if (Key == 121) //F10
	{
	  MenuStopTranslate->Click();
	}
  else if (Key == 117) //F6
	{
	  MenuDebugTranslate->Click();
	}
  else if (Key == 116) //F5
	{
	  MenuShowFuncStack->Click();
	}
  else if (Key == 115) //F4
	{
	  MenuShowParamStack->Click();
	}
  else if (Key == 114) //F3
	{
	  MenuShowClassStack->Click();
	}
  else if (Key == 113) //F2
	{
	  MenuShowObjStack->Click();
	}
  else if (Key == 112) //F1
	{
	  MenuShowVarStack->Click();
	}
  else if (Key == 122) //F11 вкл/выкл подсветку синтаксиса
	{
	  SyntaxHighlight = !SyntaxHighlight;

	  if (SyntaxHighlight)
		SendMessage(Editor->Handle, WM_KEYUP, 0, NULL);
	  else
		CancelHighlight();
	}
  else if (Key == 123) //F12 вкл/выкл CodeInsight
	{
	  CodeInsight = !CodeInsight;

	  if (!CodeInsight)
        AutoCodeInsight = false;
	}
}
//---------------------------------------------------------------------------

void __fastcall TELIExtIDEForm::EditorKeyUp(TObject *Sender, WORD &Key, TShiftState Shift)
{
  if (Key == 120)
	return;

  if ((Key == VK_UP) || (Key == VK_DOWN) || (Key == VK_LEFT) || (Key == VK_RIGHT))
	{
	  LineNum->Text = Editor->CaretPos.Y;
	  ColNum->Text = Editor->CaretPos.X;
      return;
	}
  else if (Key == 9) //tab
	{
	  TPoint user_caret = Editor->CaretPos;
      user_caret.X += SpacesForTabs - 1;
	  LockWindowUpdate(Editor->Handle);
	  String str = Editor->Lines->Strings[Editor->CaretPos.Y];
	  str = RemoveTabs(str, SpacesForTabs);
	  Editor->Lines->Strings[Editor->CaretPos.Y] = str;
	  Editor->CaretPos = user_caret;
      LockWindowUpdate(NULL);
	}
  else if (Key == 13) //enter
	{
	  if (SyntaxHighlight)
        {
		  LockWindowUpdate(Editor->Handle);

		  if (SyntaxHighlight)
			HighlightSource(Editor, Editor->CaretPos.Y - 1);

		  LockWindowUpdate(NULL);
		}
	}
  else if (Key == 0) //пустий символ для активації підсвітки
	{
	  LockWindowUpdate(Editor->Handle);

	  if (SyntaxHighlight)
		HighlightSourceFull(Editor);

      LockWindowUpdate(NULL);
	}
  else if (AutoCodeInsight && (Key == 190)) //'.' для активації CodeInsight
    {
	  ActivateCodeInsight();
	}
  else if (AutoCodeInsight && (Key == 189) && Shift.Contains(ssShift)) //'_' для активації CodeInsight
    {
	  ActivateCodeInsight();
	}
  else if (AutoCodeInsight && (Key == 51) && Shift.Contains(ssShift)) //'#' для активації CodeInsight
	{
	  ActivateCodeInsight();
	}

  if (!(Shift.Contains(ssCtrl) || Shift.Contains(ssShift) || (Key == 16) || (Key == 17) || (Key == 18)))
	{
	  if (SyntaxHighlight)
		{
		  LockWindowUpdate(Editor->Handle);
		  HighlightSource(Editor, Editor->CaretPos.Y);
          LockWindowUpdate(NULL);
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TELIExtIDEForm::EditorKeyPress(TObject *Sender, System::WideChar &Key)
{
  //Caption = int(Key);

  switch (Key)
	{
	  case 19: //ctrl+s
		{
          MenuSave->Click();
		  break;
		}

	  case 15: //ctrl+o
		{
		  MenuLoad->Click();
		  break;
		}

	  default:
		{
		  break;
        }
	}
}
//---------------------------------------------------------------------------

void __fastcall TELIExtIDEForm::EditorChange(TObject *Sender)
{
  if (Editor->Text != "")
	{
	  changed = true;
	  BtSave->Glyph = Images->GetBitmap(10, 20, 20);
	}

  LineNum->Text = Editor->CaretPos.Y;
  ColNum->Text = Editor->CaretPos.X;
}
//---------------------------------------------------------------------------

void __fastcall TELIExtIDEForm::ParamsChange(TObject *Sender)
{
  changed = true;
  BtSave->Glyph = Images->GetBitmap(10, 20, 20);
}
//---------------------------------------------------------------------------

void __fastcall TELIExtIDEForm::SaveCurrentProject()
{
  if (changed)
	{
	  if (MessageDlg("Save current file?",
					 mtWarning,
					 TMsgDlgButtons() <<mbYes <<mbNo, 0) == mrYes)
		{
		  if (cur_proj_path != "")
			{
			  Editor->Lines->SaveToFile(cur_proj_path, TEncoding::UTF8);
			  changed = false;
			  BtSave->Glyph = Images->GetBitmap(11, 20, 20);
			}
		  else if (SaveProject->Execute())
			{
			  Editor->Lines->SaveToFile(SaveProject->FileName, TEncoding::UTF8);
			  changed = false;
              BtSave->Glyph = Images->GetBitmap(11, 20, 20);
			}
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TELIExtIDEForm::MenuNewClick(TObject *Sender)
{
  SaveCurrentProject();

  Editor->Clear();
  Params->Clear();
  LineNum->Clear();
  ColNum->Clear();
  Log->Clear();

  Editor->Lines->Add("#begin ;");
  Editor->Lines->Add("");
  Editor->Lines->Add("#end;");
  Editor->CaretPos = TPoint(0, 1);

  if (SyntaxHighlight)
	SendMessage(Editor->Handle, WM_KEYUP, 0, NULL);
}
//---------------------------------------------------------------------------

void __fastcall TELIExtIDEForm::MenuLoadClick(TObject *Sender)
{
  OpenProject->FileName = "";

  if (OpenProject->Execute())
	{
      SaveCurrentProject();

	  Editor->Clear();
	  Params->Clear();
	  LineNum->Clear();
	  ColNum->Clear();
	  Log->Clear();

	  Editor->Lines->LoadFromFile(OpenProject->FileName, TEncoding::UTF8);

      Editor->SelectAll();
	  Editor->SelAttributes->Assign(IDESettings->FontDialog->Font);
	  Editor->SelLength = 0;

	  cur_proj_path = OpenProject->FileName;
	  changed = false;
      BtSave->Glyph = Images->GetBitmap(11, 20, 20);

	  if (SyntaxHighlight)
		SendMessage(Editor->Handle, WM_KEYUP, 0, NULL);

	  AddLastFile(OpenProject->FileName);

	  Caption = "ELI IDE " + Version + " - " + CreateProjName(OpenProject->FileName);
	}
}
//---------------------------------------------------------------------------

void __fastcall TELIExtIDEForm::MenuSaveClick(TObject *Sender)
{
  SaveProject->FileName = "";

  if (cur_proj_path != "")
	{
      Editor->PlainText = true;
	  Editor->Lines->SaveToFile(cur_proj_path, TEncoding::UTF8);
	  changed = false;
	  BtSave->Glyph = Images->GetBitmap(11, 20, 20);
      Editor->PlainText = false;
	}
  else if (SaveProject->Execute())
	{
      Editor->PlainText = true;
	  Editor->Lines->SaveToFile(SaveProject->FileName, TEncoding::UTF8);
	  changed = false;
      BtSave->Glyph = Images->GetBitmap(11, 20, 20);
	  Editor->PlainText = false;
	}
}
//---------------------------------------------------------------------------

void __fastcall TELIExtIDEForm::MenuSaveAsClick(TObject *Sender)
{
  SaveProject->FileName = "";

  if (SaveProject->Execute())
	{
      Editor->PlainText = true;
	  Editor->Lines->SaveToFile(SaveProject->FileName, TEncoding::UTF8);
	  changed = false;
	  BtSave->Glyph = Images->GetBitmap(11, 20, 20);
	  Editor->PlainText = false;
	  cur_proj_path = SaveProject->FileName;
	  Caption = "ELI IDE " + Version + " - " + CreateProjName(SaveProject->FileName);
	}
}
//---------------------------------------------------------------------------

void __fastcall TELIExtIDEForm::MenuFileClick(TObject *Sender)
{
  ShowLastFiles();
}
//---------------------------------------------------------------------------

void __fastcall TELIExtIDEForm::MenuAboutClick(TObject *Sender)
{
  String text = "IDE version: " + GetVersionInString(Application->ExeName.c_str());
  text += "\r\nELI version: " + GetVersionInString(InterpreterPath.c_str());

  text += "\r\n\r\nCopyright 2019-2020 Maxim Noltmeer (m.noltmeer@gmail.com)\r\n\r\n\
ELI IDE is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License\r\n\
as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.\r\n\
ELI IDE is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty\r\n\
of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.\r\n\r\n\
You should have received a copy of the GNU General Public License along with ELI IDE. \
If not, see <http://www.gnu.org/licenses/>.\r\n\r\n\
Any questions, feedback and suggestions about this software are\
accepted by the Developer by email: m.noltmeer@gmail.com";

  MessageBox(Application->Handle, text.c_str(), L"About", MB_OK);
}
//---------------------------------------------------------------------------

void __fastcall TELIExtIDEForm::MenuTranslateClick(TObject *Sender)
{
  try
	 {
	   if (Editor->Text == "")
		 {
		   MessageBox(Application->Handle, L"Translate: EMPTY SCRIPT", L"Result", MB_ICONERROR | MB_OK);
		   return;
		 }

	   Translate(Editor->Text, Params->Text);
	 }
  catch (Exception &e)
	 {
	   SaveLog(LogPath + "\\exceptions.log", "MenuTranslateClick: " + e.ToString());
	 }
}
//---------------------------------------------------------------------------

void __fastcall TELIExtIDEForm::MenuStopTranslateClick(TObject *Sender)
{
  try
	 {
	   DWORD et_pid = GetProcessByExeName(L"et.exe");
	   HANDLE proc = OpenProcess(PROCESS_TERMINATE, 0, et_pid);

	   if (proc && MessageDlg("Terminate?",
							  mtConfirmation,
							  TMsgDlgButtons() << mbYes << mbNo,
				 			  0) == mrYes)
		 {
		   TerminateProcess(proc, 0);
           CloseHandle(proc);
		 }
	 }
  catch (Exception &e)
	 {
	   SaveLog(LogPath + "\\exceptions.log", "MenuStopTranslateClick: " + e.ToString());
	 }
}
//---------------------------------------------------------------------------

void __fastcall TELIExtIDEForm::PPTranslateToLineClick(TObject *Sender)
{
  MenuTranslateToCursor->Click();
}
//---------------------------------------------------------------------------

void __fastcall TELIExtIDEForm::MenuTranslateToCursorClick(TObject *Sender)
{
  try
	 {
	   if (Editor->Text == "")
		 {
		   MessageBox(Application->Handle, L"Translate: EMPTY SCRIPT", L"Result", MB_ICONERROR | MB_OK);
		   return;
		 }

//обріжемо текст скрипту до рядка, де стоїть курсор
	   TStringList *tmp_lines = new TStringList();
	   int line_ind = Editor->CaretPos.Y;
	   String text;

	   try
		  {
			tmp_lines->AddStrings(Editor->Lines);

			while (tmp_lines->Count > line_ind + 1)
			  tmp_lines->Delete(tmp_lines->Count - 1);

			text = tmp_lines->Text;
		  }
	   __finally {delete tmp_lines;}

	   AddCloseBraces(text); //додаємо відсутні закриваючі скобки
	   text += "#end;";

	   Translate(text, Params->Text);
	 }
  catch (Exception &e)
	 {
	   SaveLog(LogPath + "\\exceptions.log", "MenuTranslateToCursorClick: " + e.ToString());
	 }
}
//---------------------------------------------------------------------------

void __fastcall TELIExtIDEForm::MenuDebugTranslateClick(TObject *Sender)
{
  MenuDebugTranslate->Checked = !MenuDebugTranslate->Checked;

  if (MenuDebugTranslate->Checked)
	{
	  BtDebug->Glyph = Images->GetBitmap(7, 25, 25);
	  DebugOutputForm->Show();
	}
  else
	{
	  BtDebug->Glyph = Images->GetBitmap(6, 25, 25);
      DebugOutputForm->Hide();
	}
}
//---------------------------------------------------------------------------

void __fastcall TELIExtIDEForm::MenuSetHostAppClick(TObject *Sender)
{
  SetHostAppForm->Show();
}
//---------------------------------------------------------------------------

void __fastcall TELIExtIDEForm::MenuShowVarStackClick(TObject *Sender)
{
  MenuShowVarStack->Checked = !MenuShowVarStack->Checked;

  if (MenuShowVarStack->Checked)
	{
	  BtVarStack->Glyph = Images->GetBitmap(1, 25, 25);
	  ShowVarStackForm->Show();
	}
  else
	{
	  BtVarStack->Glyph = Images->GetBitmap(0, 25, 25);
	  ShowVarStackForm->Hide();
	}
}
//---------------------------------------------------------------------------

void __fastcall TELIExtIDEForm::MenuShowObjStackClick(TObject *Sender)
{
  MenuShowObjStack->Checked = !MenuShowObjStack->Checked;

  if (MenuShowObjStack->Checked)
	{
	  BtObjStack->Glyph = Images->GetBitmap(2, 25, 25);
	  ShowObjStackForm->Show();
	}
  else
	{
	  BtObjStack->Glyph = Images->GetBitmap(0, 25, 25);
	  ShowObjStackForm->Hide();
	}
}
//---------------------------------------------------------------------------

void __fastcall TELIExtIDEForm::MenuShowClassStackClick(TObject *Sender)
{
  MenuShowClassStack->Checked = !MenuShowClassStack->Checked;

  if (MenuShowClassStack->Checked)
	{
	  BtClassStack->Glyph = Images->GetBitmap(3, 25, 25);
	  ShowClassStackForm->Show();
	}
  else
	{
	  BtClassStack->Glyph = Images->GetBitmap(0, 25, 25);
	  ShowClassStackForm->Hide();
	}
}
//---------------------------------------------------------------------------

void __fastcall TELIExtIDEForm::MenuShowParamStackClick(TObject *Sender)
{
  MenuShowParamStack->Checked = !MenuShowParamStack->Checked;

  if (MenuShowParamStack->Checked)
	{
	  BtParamStack->Glyph = Images->GetBitmap(4, 25, 25);
	  ShowParamStackForm->Show();
	}
  else
	{
	  BtParamStack->Glyph = Images->GetBitmap(0, 25, 25);
      ShowParamStackForm->Hide();
	}
}
//---------------------------------------------------------------------------

void __fastcall TELIExtIDEForm::MenuShowFuncStackClick(TObject *Sender)
{
  MenuShowFuncStack->Checked = !MenuShowFuncStack->Checked;

  if (MenuShowFuncStack->Checked)
	{
	  BtFuncStack->Glyph = Images->GetBitmap(5, 25, 25);
	  ShowFuncStackForm->Show();
	}
  else
	{
	  BtFuncStack->Glyph = Images->GetBitmap(0, 25, 25);
      ShowFuncStackForm->Hide();
	}
}
//---------------------------------------------------------------------------

void __fastcall TELIExtIDEForm::MenuInsertClassClick(TObject *Sender)
{
  String name = InputBox("Inserting Class", "Input class name", "MyClass");

  String body = "#class " + name + "\r\n" +
				"{\r\n" +
				"#property Prop1 = 0;\r\n" +
				"#public property Prop2 = 0;\r\n" +
				"#method Foo() {#return &$this.Prop1;}\r\n" +
				"#public method " + name + "($x){&$this.Prop1 = $x;}\r\n" +
				"}";

  Clipboard()->SetTextBuf(body.c_str());
  Editor->PasteFromClipboard();

  if (SyntaxHighlight)
	SendMessage(Editor->Handle, WM_KEYUP, 0, NULL);
}
//---------------------------------------------------------------------------

void __fastcall TELIExtIDEForm::MenuInsertProcedureClick(TObject *Sender)
{
  String name = InputBox("Inserting Procedure", "Input procedure name", "MyProc");

  String body = "#procedure " + name + "()\r\n" +
				"{\r\n\r\n}";

  Clipboard()->SetTextBuf(body.c_str());
  Editor->PasteFromClipboard();

  if (SyntaxHighlight)
	SendMessage(Editor->Handle, WM_KEYUP, 0, NULL);
}
//---------------------------------------------------------------------------

void __fastcall TELIExtIDEForm::MenuInsertMakeCodeClick(TObject *Sender)
{
  String name = InputBox("Inserting Make Code", "Input variable name", "$var");

  String body = "#make " + name + "\r\n" +
				"{\r\n\r\n}";

  Clipboard()->SetTextBuf(body.c_str());
  Editor->PasteFromClipboard();

  if (SyntaxHighlight)
	SendMessage(Editor->Handle, WM_KEYUP, 0, NULL);
}
//---------------------------------------------------------------------------

void __fastcall TELIExtIDEForm::MenuInsertForClick(TObject *Sender)
{
  String val = InputBox("Inserting For loop", "Input Start value", "0");

  String body = "for (" + val + ", ";

  val = InputBox("Inserting For loop", "Input End value", "0");

  body += "<" + val + ", ";

  val = InputBox("Inserting For loop", "Input Tag", "1");

  body += "+" + val + ")\r\n{\r\n\r\n}";

  Clipboard()->SetTextBuf(body.c_str());
  Editor->PasteFromClipboard();

  if (SyntaxHighlight)
	SendMessage(Editor->Handle, WM_KEYUP, 0, NULL);
}
//---------------------------------------------------------------------------

void __fastcall TELIExtIDEForm::MenuInsertSelectClick(TObject *Sender)
{
  String val = InputBox("Inserting Select construct", "Value to check", "$var");

  String body = "select (" + val + ")\r\n" +
				"{\r\nwhen 0 then {}\r\n}";

  Clipboard()->SetTextBuf(body.c_str());
  Editor->PasteFromClipboard();

  if (SyntaxHighlight)
	SendMessage(Editor->Handle, WM_KEYUP, 0, NULL);
}
//---------------------------------------------------------------------------

void __fastcall TELIExtIDEForm::BtDebugClick(TObject *Sender)
{
  MenuDebugTranslate->Click();
}
//---------------------------------------------------------------------------

void __fastcall TELIExtIDEForm::BtVarStackClick(TObject *Sender)
{
  MenuShowVarStack->Click();
}
//---------------------------------------------------------------------------

void __fastcall TELIExtIDEForm::BtObjStackClick(TObject *Sender)
{
  MenuShowObjStack->Click();
}
//---------------------------------------------------------------------------

void __fastcall TELIExtIDEForm::BtClassStackClick(TObject *Sender)
{
  MenuShowClassStack->Click();
}
//---------------------------------------------------------------------------

void __fastcall TELIExtIDEForm::BtParamStackClick(TObject *Sender)
{
  MenuShowParamStack->Click();
}
//---------------------------------------------------------------------------

void __fastcall TELIExtIDEForm::BtFuncStackClick(TObject *Sender)
{
  MenuShowFuncStack->Click();
}
//---------------------------------------------------------------------------

void __fastcall TELIExtIDEForm::BtNewClick(TObject *Sender)
{
  MenuNew->Click();
}
//---------------------------------------------------------------------------

void __fastcall TELIExtIDEForm::BtOpenClick(TObject *Sender)
{
  MenuLoad->Click();
}
//---------------------------------------------------------------------------

void __fastcall TELIExtIDEForm::BtSaveClick(TObject *Sender)
{
  MenuSave->Click();
}
//---------------------------------------------------------------------------

void __fastcall TELIExtIDEForm::BtRunClick(TObject *Sender)
{
  MenuTranslate->Click();
}
//---------------------------------------------------------------------------

void __fastcall TELIExtIDEForm::BtRunToClick(TObject *Sender)
{
  MenuTranslateToCursor->Click();
}
//---------------------------------------------------------------------------

void __fastcall TELIExtIDEForm::BtBreakClick(TObject *Sender)
{
  MenuStopTranslate->Click();
}
//---------------------------------------------------------------------------


