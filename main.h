/*!
Copyright 2019-2021 Maxim Noltmeer (m.noltmeer@gmail.com)

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
//---------------------------------------------------------------------------

#ifndef mainH
#define mainH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.ActnCtrls.hpp>
#include <Vcl.ActnMan.hpp>
#include <Vcl.ButtonGroup.hpp>
#include <Vcl.ToolWin.hpp>
#include <Vcl.WinXCtrls.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Grids.hpp>
#include <Xml.adomxmldom.hpp>
#include <Xml.XMLDoc.hpp>
#include <Xml.xmldom.hpp>
#include <Xml.XMLIntf.hpp>
#include <Xml.Win.msxmldom.hpp>
#include <System.ImageList.hpp>
#include <Vcl.ImgList.hpp>
#include <Vcl.Clipbrd.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.Styles.hpp>
#include <Vcl.Themes.hpp>
#include <Vcl.BaseImageCollection.hpp>
#include <Vcl.ImageCollection.hpp>

//---------------------------------------------------------------------------

class TELIExtIDEForm : public TForm
{
__published:	// IDE-managed Components
	TOpenDialog *OpenProject;
	TSaveDialog *SaveProject;
	TMainMenu *MainMenu;
	TMenuItem *MenuView;
	TMenuItem *MenuSave;
	TMenuItem *MenuLoad;
	TMenuItem *MenuTranslateToCursor;
	TMenuItem *MenuTranslate;
	TMenuItem *MenuSaveAs;
	TMenuItem *MenuNew;
	TMenuItem *MenuPreferences;
	TMenuItem *MenuFile;
	TMenuItem *MenuActions;
	TMenuItem *MenuShowLog;
	TRichEdit *Editor;
	TMemo *Log;
	TPopupMenu *EditorPopup;
	TMenuItem *PPCopy;
	TMenuItem *PPCut;
	TMenuItem *PPPaste;
	TLabel *Label1;
	TEdit *Params;
	TEdit *LineNum;
	TEdit *ColNum;
	TLabel *LbParam;
	TPanel *BtnPanel;
	TMenuItem *MenuShowPanel;
	TMenuItem *PPSelectAll;
	TMenuItem *Recentfiles1;
	TMenuItem *PPTranslateToLine;
	TMenuItem *MenuAbout;
	TMenuItem *MenuDebugTranslate;
	TMenuItem *MenuShowVarStack;
	TMenuItem *MenuShowObjStack;
	TMenuItem *MenuShowClassStack;
	TMenuItem *MenuShowParamStack;
	TMenuItem *MenuShowFuncStack;
	TMenuItem *MenuInsert;
	TMenuItem *MenuInsertClass;
	TMenuItem *MenuInsertProcedure;
	TMenuItem *MenuInsertMakeCode;
	TMenuItem *MenuInsertFor;
	TMenuItem *MenuInsertSelect;
	TBitBtn *BtNew;
	TBitBtn *BtOpen;
	TBitBtn *BtSave;
	TBitBtn *BtRun;
	TBitBtn *BtRunTo;
	TBitBtn *BtDebug;
	TBitBtn *BtVarStack;
	TBitBtn *BtObjStack;
	TBitBtn *BtClassStack;
	TBitBtn *BtParamStack;
	TBitBtn *BtFuncStack;
	TImageCollection *Images;
	TBitBtn *BtBreak;
	TMenuItem *MenuStopTranslate;
	TMenuItem *MenuSetHostApp;

	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall MenuShowLogClick(TObject *Sender);
	void __fastcall MenuPreferencesClick(TObject *Sender);
	void __fastcall MenuShowPanelClick(TObject *Sender);
	void __fastcall EditorMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
	void __fastcall EditorKeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall EditorChange(TObject *Sender);
	void __fastcall PPSelectAllClick(TObject *Sender);
	void __fastcall PPCopyClick(TObject *Sender);
	void __fastcall PPCutClick(TObject *Sender);
	void __fastcall PPPasteClick(TObject *Sender);
	void __fastcall MenuNewClick(TObject *Sender);
	void __fastcall MenuLoadClick(TObject *Sender);
	void __fastcall ParamsChange(TObject *Sender);
	void __fastcall MenuSaveClick(TObject *Sender);
	void __fastcall MenuSaveAsClick(TObject *Sender);
	void __fastcall MenuFileClick(TObject *Sender);
	void __fastcall EditorKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall EditorKeyUp(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall MenuAboutClick(TObject *Sender);
	void __fastcall MenuTranslateClick(TObject *Sender);
	void __fastcall PPTranslateToLineClick(TObject *Sender);
	void __fastcall MenuTranslateToCursorClick(TObject *Sender);
	void __fastcall MenuDebugTranslateClick(TObject *Sender);
	void __fastcall MenuShowVarStackClick(TObject *Sender);
	void __fastcall MenuShowObjStackClick(TObject *Sender);
	void __fastcall MenuShowClassStackClick(TObject *Sender);
	void __fastcall MenuShowParamStackClick(TObject *Sender);
	void __fastcall MenuShowFuncStackClick(TObject *Sender);
	void __fastcall MenuInsertClassClick(TObject *Sender);
	void __fastcall MenuInsertProcedureClick(TObject *Sender);
	void __fastcall MenuInsertMakeCodeClick(TObject *Sender);
	void __fastcall MenuInsertForClick(TObject *Sender);
	void __fastcall MenuInsertSelectClick(TObject *Sender);
	void __fastcall BtDebugClick(TObject *Sender);
	void __fastcall BtVarStackClick(TObject *Sender);
	void __fastcall BtObjStackClick(TObject *Sender);
	void __fastcall BtClassStackClick(TObject *Sender);
	void __fastcall BtParamStackClick(TObject *Sender);
	void __fastcall BtFuncStackClick(TObject *Sender);
	void __fastcall BtNewClick(TObject *Sender);
	void __fastcall BtOpenClick(TObject *Sender);
	void __fastcall BtSaveClick(TObject *Sender);
	void __fastcall BtRunClick(TObject *Sender);
	void __fastcall BtRunToClick(TObject *Sender);
	void __fastcall MenuStopTranslateClick(TObject *Sender);
	void __fastcall BtBreakClick(TObject *Sender);
	void __fastcall MenuSetHostAppClick(TObject *Sender);

private:	// User declarations
	void __fastcall AutoScaleUI();
	void __fastcall SetLog(int left, int top, int width, int height);
	void __fastcall SetPanel(int left, int top, int width, int height);
	void __fastcall SetEditor(int left, int top, int width, int height);
	void __fastcall EditorFormResize(TObject *Sender);
	void __fastcall EditorFormShow(TObject *Sender);
	void __fastcall WriteLastFilesList();
	void __fastcall ReadLastFilesList();
	void __fastcall WriteSettings();
	void __fastcall ReadSettings();
	void __fastcall ShowLastFiles();
	void __fastcall CreateMenuLastFiles();
	void __fastcall SaveCurrentProject();
	void __fastcall AddLastFile(String file);
	void __fastcall PPLastFileClick(TObject *Sender);
	void __fastcall PPCodeInsightMenuClick(TObject *Sender);
	void __fastcall CreateCodeInsightMenu(String entity_name);
	void __fastcall ClearCodeInsightMenu();
	void __fastcall OpenCodeInsightMenu();
	String __fastcall ExtractFragmentLexemeFromLine(int line_ind, int cursor_pos);
	String __fastcall ReplaceFragmentLexemeInLine(const String &lexeme, int line_ind, int cursor_pos);
	int __fastcall FindLexemBorder(int line_ind, int cursor_pos);
	void __fastcall ActivateCodeInsight();
	void __fastcall AddCloseBraces(String &text);
	int __fastcall CountBraces(const String &text, wchar_t brace);
	void __fastcall Translate(String text, String params);

public:		// User declarations
	__fastcall TELIExtIDEForm(TComponent* Owner);

	void __fastcall ChangeTheme(String name);
	int __fastcall ConnectELI();
	int __fastcall ReleaseELI();
	void __fastcall LockUIByELI(bool block);
    void __fastcall CancelHighlight();
};
//---------------------------------------------------------------------------
extern PACKAGE TELIExtIDEForm *ELIExtIDEForm;
//---------------------------------------------------------------------------

#endif
