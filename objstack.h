//---------------------------------------------------------------------------

#ifndef objstackH
#define objstackH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
//---------------------------------------------------------------------------
class TShowObjStackForm : public TForm
{
__published:	// IDE-managed Components
	TMemo *Stack;
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
private:	// User declarations
public:		// User declarations
	__fastcall TShowObjStackForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TShowObjStackForm *ShowObjStackForm;
//---------------------------------------------------------------------------
#endif
