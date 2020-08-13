//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "..\..\ELI\eli_interface.h"
#include "..\..\MyFunc\MyFunc.h"
#include "main.h"
#include "functionstack.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TShowFuncStackForm *ShowFuncStackForm;

extern ELI_INTERFACE *eIface;
extern String ide_dir;
//---------------------------------------------------------------------------
__fastcall TShowFuncStackForm::TShowFuncStackForm(TComponent* Owner)
	: TForm(Owner)
{
  Owner = ELIExtIDEForm;
  Parent = ELIExtIDEForm;
}
//---------------------------------------------------------------------------

void __fastcall TShowFuncStackForm::FormClose(TObject *Sender, TCloseAction &Action)
{
  ELIExtIDEForm->MenuShowFuncStack->Click();
}
//---------------------------------------------------------------------------
