//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "..\..\ELI\eli_interface.h"
#include "..\..\MyFunc\MyFunc.h"
#include "main.h"
#include "varstack.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TShowVarStackForm *ShowVarStackForm;

extern ELI_INTERFACE *eIface;
extern String ide_dir;
//---------------------------------------------------------------------------
__fastcall TShowVarStackForm::TShowVarStackForm(TComponent* Owner)
	: TForm(Owner)
{
  Owner = ELIExtIDEForm;
  Parent = ELIExtIDEForm;
}
//---------------------------------------------------------------------------

void __fastcall TShowVarStackForm::FormClose(TObject *Sender, TCloseAction &Action)
{
  ELIExtIDEForm->MenuShowVarStack->Click();
}
//---------------------------------------------------------------------------

