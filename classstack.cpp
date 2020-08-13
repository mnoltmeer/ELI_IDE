//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "..\..\ELI\eli_interface.h"
#include "..\..\MyFunc\MyFunc.h"
#include "main.h"
#include "classstack.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TShowClassStackForm *ShowClassStackForm;

extern ELI_INTERFACE *eIface;
extern String ide_dir;
//---------------------------------------------------------------------------
__fastcall TShowClassStackForm::TShowClassStackForm(TComponent* Owner)
	: TForm(Owner)
{
  Owner = ELIExtIDEForm;
  Parent = ELIExtIDEForm;
}
//---------------------------------------------------------------------------

void __fastcall TShowClassStackForm::FormClose(TObject *Sender, TCloseAction &Action)
{
  ELIExtIDEForm->MenuShowClassStack->Click();
}
//---------------------------------------------------------------------------

