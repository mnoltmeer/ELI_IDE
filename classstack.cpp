//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "..\ELI\eli_interface.h"
#include "..\work-functions\MyFunc.h"
#include "main.h"
#include "classstack.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TShowClassStackForm *ShowClassStackForm;

extern ELI_INTERFACE *eIface;

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

