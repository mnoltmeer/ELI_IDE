//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "main.h"
#include "debug.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TDebugOutputForm *DebugOutputForm;

//---------------------------------------------------------------------------
__fastcall TDebugOutputForm::TDebugOutputForm(TComponent* Owner)
	: TForm(Owner)
{
  Owner = ELIExtIDEForm;
  Parent = ELIExtIDEForm;
}
//---------------------------------------------------------------------------

void __fastcall TDebugOutputForm::FormClose(TObject *Sender, TCloseAction &Action)
{
  ELIExtIDEForm->MenuDebugTranslate->Click();
}
//---------------------------------------------------------------------------

