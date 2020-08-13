//---------------------------------------------------------------------------

#ifndef paramstackH
#define paramstackH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
//---------------------------------------------------------------------------
class TShowParamStackForm : public TForm
{
__published:	// IDE-managed Components
	TMemo *Stack;
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
private:	// User declarations
public:		// User declarations
	__fastcall TShowParamStackForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TShowParamStackForm *ShowParamStackForm;
//---------------------------------------------------------------------------
#endif
