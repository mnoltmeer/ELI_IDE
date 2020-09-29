//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "..\ELI\eli_interface.h"
#include "..\work-functions\MyFunc.h"
#include "main.h"
#include "functionstack.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TShowFuncStackForm *ShowFuncStackForm;

extern ELI_INTERFACE *eIface;

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

