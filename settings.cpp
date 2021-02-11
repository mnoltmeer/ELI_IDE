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

#include "main.h"
#include "ELISourceHighlighter.h"
#include "..\ELI\eli_interface.h"
#include "settings.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TIDESettings *IDESettings;

extern String FontName;
extern int FontSize;
extern String Theme;
extern String InterpreterPath;
extern bool SyntaxHighlight, CodeInsight, AutoCodeInsight;
extern HINSTANCE dllhandle;
extern int SpacesForTabs;
extern int EliStatus;
extern ELI_INTERFACE *eIface;
//---------------------------------------------------------------------------
__fastcall TIDESettings::TIDESettings(TComponent* Owner)
	: TForm(Owner)
{
  Owner = ELIExtIDEForm;
}
//---------------------------------------------------------------------------

void __fastcall TIDESettings::CheckELIPath()
{
  try
	 {
	   if (!FileExists(ELIPath->Text))
		 {
		   IsCorrectELIPath->Caption = "INVALID PATH!";

		   if (dllhandle)
			 ELIExtIDEForm->ReleaseELI();

		   ELIExtIDEForm->LockUIByELI(true);
		 }
	   else
		 {
		   IsCorrectELIPath->Caption = "EXISTING PATH";

		   InterpreterPath = ELIPath->Text;

		   EliStatus = ELIExtIDEForm->ConnectELI();

		   if (!dllhandle)
			 ELIExtIDEForm->LockUIByELI(true);
		   else if (EliStatus < 0)
			 ELIExtIDEForm->LockUIByELI(true);
		   else
			 {
			   ELIExtIDEForm->LockUIByELI(false);
			   LbELIVersion->Caption = eIface->GetVersion();
             }
		 }
	 }
  catch (Exception &e)
	 {
	   ELIExtIDEForm->Log->Lines->Add("TIDESettings::CheckELIPath: " + e.ToString());
	 }
}
//---------------------------------------------------------------------------

void __fastcall TIDESettings::FormShow(TObject *Sender)
{
  try
	 {
	   CurrentFont->Text = FontName;
	   CurrentFontSize->Text = StrToInt(FontSize);
	   SelectTheme->ItemIndex = SelectTheme->Items->IndexOf(Theme);
	   EnableSyntaxHighlight->Checked = SyntaxHighlight;
	   EnableCodeInsight->Checked = CodeInsight;
	   AutoShowCodeInsight->Checked = AutoCodeInsight;
	   CountSpacesForTabs->Text = IntToStr(SpacesForTabs);
	   ELIPath->Text = InterpreterPath;
	 }
  catch (Exception &e)
	 {
	   ELIExtIDEForm->Log->Lines->Add("TIDESettings::FormShow: " + e.ToString());
	 }
}
//---------------------------------------------------------------------------

void __fastcall TIDESettings::CancelSettingsClick(TObject *Sender)
{
  Close();
}
//---------------------------------------------------------------------------

void __fastcall TIDESettings::SaveSettingsClick(TObject *Sender)
{
  try
	 {
	   FontName = FontDialog->Font->Name;
	   FontSize = FontDialog->Font->Size;

	   if (Theme != SelectTheme->Text)
		 {
		   Theme = SelectTheme->Text;
		   ELIExtIDEForm->ChangeTheme(Theme);
		 }

	   SyntaxHighlight = EnableSyntaxHighlight->Checked;
	   CodeInsight = EnableCodeInsight->Checked;
	   AutoCodeInsight = AutoShowCodeInsight->Checked;
	   SpacesForTabs = CountSpacesForTabs->Text.ToInt();
	   InterpreterPath = ELIPath->Text;

	   if (SyntaxHighlight)
		 SendMessage(ELIExtIDEForm->Editor->Handle, WM_KEYUP, 0, NULL);
	 }
  catch (Exception &e)
	 {
	   ELIExtIDEForm->Log->Lines->Add("TIDESettings::SaveSettingsClick: " + e.ToString());
	 }

  Close();
}
//---------------------------------------------------------------------------

void __fastcall TIDESettings::EnableCodeInsightClick(TObject *Sender)
{
  if (!EnableCodeInsight->Checked)
    AutoShowCodeInsight->Checked = false;
}
//---------------------------------------------------------------------------

void __fastcall TIDESettings::SetELIPathClick(TObject *Sender)
{
  if (OpenDLL->Execute() && OpenDLL->FileName != "")
	{
	  ELIPath->Text = OpenDLL->FileName;
    }
}
//---------------------------------------------------------------------------

void __fastcall TIDESettings::ELIPathChange(TObject *Sender)
{
  CheckELIPath();
}
//---------------------------------------------------------------------------

void __fastcall TIDESettings::EnableSyntaxHighlightClick(TObject *Sender)
{
  if (EnableSyntaxHighlight->Checked)
	SendMessage(ELIExtIDEForm->Editor->Handle, WM_KEYUP, 0, NULL);
  else
	ELIExtIDEForm->CancelHighlight();
}
//---------------------------------------------------------------------------

void __fastcall TIDESettings::SelectFontClick(TObject *Sender)
{
  if (FontDialog->Execute())
	{
	  CurrentFont->Text = FontDialog->Font->Name;
	  CurrentFontSize->Text = IntToStr(FontDialog->Font->Size);

	  ELIExtIDEForm->Editor->Font->Assign(FontDialog->Font);
	  ELIExtIDEForm->Editor->SelectAll();
	  ELIExtIDEForm->Editor->SelAttributes->Assign(FontDialog->Font);
	  ELIExtIDEForm->Editor->SelLength = 0;

//посилаємо спец. пустий символ для підсвітки
	  if (SyntaxHighlight)
		SendMessage(ELIExtIDEForm->Editor->Handle, WM_KEYUP, 0, NULL);
	}
}
//---------------------------------------------------------------------------

