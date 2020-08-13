//---------------------------------------------------------------------------

#ifndef debugH
#define debugH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
//---------------------------------------------------------------------------
class TDebugOutputForm : public TForm
{
__published:	// IDE-managed Components
	TMemo *DebugLog;
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
private:	// User declarations
public:		// User declarations
	__fastcall TDebugOutputForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TDebugOutputForm *DebugOutputForm;
//---------------------------------------------------------------------------
#endif
