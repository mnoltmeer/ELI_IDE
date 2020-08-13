//---------------------------------------------------------------------------

#ifndef sethostH
#define sethostH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.Dialogs.hpp>
//---------------------------------------------------------------------------
class TSetHostAppForm : public TForm
{
__published:	// IDE-managed Components
	TBitBtn *Cancel;
	TBitBtn *Apply;
	TComboBox *HostAppList;
	TButton *SetHostApp;
	TOpenDialog *OpenHostApp;
	void __fastcall ApplyClick(TObject *Sender);
	void __fastcall CancelClick(TObject *Sender);
	void __fastcall SetHostAppClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TSetHostAppForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TSetHostAppForm *SetHostAppForm;
//---------------------------------------------------------------------------
#endif
