//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "..\..\ELI\eli_interface.h"
#include "..\..\MyFunc\MyFunc.h"
#include "main.h"
#include "objstack.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TShowObjStackForm *ShowObjStackForm;

extern ELI_INTERFACE *eIface;
extern String ide_dir;
//---------------------------------------------------------------------------
__fastcall TShowObjStackForm::TShowObjStackForm(TComponent* Owner)
	: TForm(Owner)
{
  Owner = ELIExtIDEForm;
  Parent = ELIExtIDEForm;
}
//---------------------------------------------------------------------------

void __fastcall TShowObjStackForm::FormClose(TObject *Sender, TCloseAction &Action)
{
  ELIExtIDEForm->MenuShowObjStack->Click();
}
//---------------------------------------------------------------------------

