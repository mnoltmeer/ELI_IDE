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

#ifndef settingsH
#define settingsH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Dialogs.hpp>
//---------------------------------------------------------------------------
class TIDESettings : public TForm
{
__published:	// IDE-managed Components
	TLabel *Label2;
	TLabel *Label3;
	TButton *SaveSettings;
	TButton *CancelSettings;
	TComboBox *SelectTheme;
	TLabel *Label1;
	TCheckBox *EnableSyntaxHighlight;
	TCheckBox *EnableCodeInsight;
	TCheckBox *AutoShowCodeInsight;
	TOpenDialog *OpenDLL;
	TButton *SetELIPath;
	TLabel *Label4;
	TLabel *IsCorrectELIPath;
	TEdit *ELIPath;
	TLabel *Label5;
	TEdit *CountSpacesForTabs;
	TFontDialog *FontDialog;
	TButton *SelectFont;
	TEdit *CurrentFontSize;
	TEdit *CurrentFont;
	TLabel *LbELIVersion;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall CancelSettingsClick(TObject *Sender);
	void __fastcall SaveSettingsClick(TObject *Sender);
	void __fastcall EnableCodeInsightClick(TObject *Sender);
	void __fastcall SetELIPathClick(TObject *Sender);
	void __fastcall ELIPathChange(TObject *Sender);
	void __fastcall EnableSyntaxHighlightClick(TObject *Sender);
	void __fastcall SelectFontClick(TObject *Sender);
private:	// User declarations
	void __fastcall CheckELIPath();

public:		// User declarations
	__fastcall TIDESettings(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TIDESettings *IDESettings;
//---------------------------------------------------------------------------
#endif
