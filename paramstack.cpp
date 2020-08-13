//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "..\..\ELI\eli_interface.h"
#include "..\..\MyFunc\MyFunc.h"
#include "main.h"
#include "paramstack.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TShowParamStackForm *ShowParamStackForm;

extern ELI_INTERFACE *eIface;
extern String ide_dir;
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

