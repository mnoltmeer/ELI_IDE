//---------------------------------------------------------------------------

#ifndef varstackH
#define varstackH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
//---------------------------------------------------------------------------
class TShowVarStackForm : public TForm
{
__published:	// IDE-managed Components
	TMemo *Stack;
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
private:	// User declarations
public:		// User declarations
	__fastcall TShowVarStackForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TShowVarStackForm *ShowVarStackForm;
//---------------------------------------------------------------------------
#endif
