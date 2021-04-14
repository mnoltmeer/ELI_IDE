//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "..\work-functions\MyFunc.h"
#include "main.h"
#include "sethost.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TSetHostAppForm *SetHostAppForm;
extern TELIExtIDEForm *ELIExtIDEForm;
extern String HostApplication;
extern TStringList *HostApplicationList;
//---------------------------------------------------------------------------

__fastcall TSetHostAppForm::TSetHostAppForm(TComponent* Owner)
	: TForm(Owner)
{
  Left = ELIExtIDEForm->ClientWidth / 2 - ClientWidth / 2;
  Top = ELIExtIDEForm->ClientHeight / 2 - ClientHeight / 2;

  HostAppList->Items->Clear();
  HostAppList->Items->AddStrings(HostApplicationList);
  HostAppList->ItemIndex = 0;
}
//---------------------------------------------------------------------------

void __fastcall TSetHostAppForm::ApplyClick(TObject *Sender)
{
  HostApplication = HostAppList->Text;
  HostApplicationList->Clear();
  HostApplicationList->AddStrings(HostAppList->Items);
  Close();
}
//---------------------------------------------------------------------------

void __fastcall TSetHostAppForm::CancelClick(TObject *Sender)
{
  Close();
}
//---------------------------------------------------------------------------

void __fastcall TSetHostAppForm::SetHostAppClick(TObject *Sender)
{
  OpenHostApp->FileName = "";

  if (OpenHostApp->Execute())
	{
	  HostAppList->Items->Add(OpenHostApp->FileName);
	  HostAppList->ItemIndex = HostAppList->Items->IndexOf(OpenHostApp->FileName);
    }
}
//---------------------------------------------------------------------------
