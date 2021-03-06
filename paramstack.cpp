//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "..\ELI\eli_interface.h"
#include "..\work-functions\MyFunc.h"
#include "main.h"
#include "paramstack.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TShowParamStackForm *ShowParamStackForm;

extern ELI_INTERFACE *eIface;

//---------------------------------------------------------------------------
__fastcall TShowParamStackForm::TShowParamStackForm(TComponent* Owner)
	: TForm(Owner)
{
  Owner = ELIExtIDEForm;
  Parent = ELIExtIDEForm;
}
//---------------------------------------------------------------------------

void __fastcall TShowParamStackForm::FormClose(TObject *Sender, TCloseAction &Action)
{
  ELIExtIDEForm->MenuShowParamStack->Click();
}
//---------------------------------------------------------------------------

