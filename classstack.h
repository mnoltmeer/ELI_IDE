//---------------------------------------------------------------------------

#ifndef classstackH
#define classstackH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
//---------------------------------------------------------------------------
class TShowClassStackForm : public TForm
{
__published:	// IDE-managed Components
	TMemo *Stack;
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
private:	// User declarations
public:		// User declarations
	__fastcall TShowClassStackForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TShowClassStackForm *ShowClassStackForm;
//---------------------------------------------------------------------------
#endif
