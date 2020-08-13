//---------------------------------------------------------------------------

#ifndef functionstackH
#define functionstackH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
//---------------------------------------------------------------------------
class TShowFuncStackForm : public TForm
{
__published:	// IDE-managed Components
	TMemo *Stack;
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
private:	// User declarations
public:		// User declarations
	__fastcall TShowFuncStackForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TShowFuncStackForm *ShowFuncStackForm;
//---------------------------------------------------------------------------
#endif
