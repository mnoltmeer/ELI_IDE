/*!
Copyright 2019-2020  Maxim Noltmeer (m.noltmeer@gmail.com)

This file is part of ELI IDE.

	ELI Extended IDE is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	ELI IDE is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with ELI IDE.  If not, see <http://www.gnu.org/licenses/>.
*/
//---------------------------------------------------------------------------

#pragma hdrstop

#include "..\..\MyFunc\MyFunc.h"
#include "ELISourceHighlighter.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

const wchar_t *VARSYM = L"$";
const wchar_t *OBJSYM = L"&";
const wchar_t *STRSYM = L"'";

static std::vector<ExprColor> ExpColors;

static std::vector<MarkedFragment> vecSelFragments; //������, ���� ��������� ��������� ���������,
											 //�� ����� ������������


void InitExprColors(int theme_style)
{
  ExpColors.clear();

  TColor HlDirect, HlExpr, HlVarSym, HlObjSym, HlComment, HlBrace, HlEndl;

  if (theme_style == ESH_LIGHT_THEME) //����� ����
	{
	  HlDirect = clGreen;
	  HlExpr = clBlue;
	  HlVarSym = clMaroon;
	  HlObjSym = clFuchsia;
	  HlComment = clSkyBlue;
	  HlBrace = clBlue;
	  HlEndl =  clMaroon;
	}
  else //����� ����
	{
	  HlDirect = clLime;
	  HlExpr = clAqua;
	  HlVarSym = clYellow;
	  HlObjSym = clFuchsia;
	  HlComment = clSilver;
	  HlBrace = clAqua;
	  HlEndl =  clYellow;
	}

//directives
   ExpColors.push_back(ExprColor(L"#endl", HlDirect));
   ExpColors.push_back(ExprColor(L"#begin", HlDirect));
   ExpColors.push_back(ExprColor(L"#end", HlDirect));
   ExpColors.push_back(ExprColor(L"#include", HlDirect));
   ExpColors.push_back(ExprColor(L"#exit", HlDirect));
   ExpColors.push_back(ExprColor(L"#procedure", HlDirect));
   ExpColors.push_back(ExprColor(L"#drop procedure", HlDirect));
   ExpColors.push_back(ExprColor(L"#make", HlDirect));
   ExpColors.push_back(ExprColor(L"#run", HlDirect));
   ExpColors.push_back(ExprColor(L"#class", HlDirect));
   ExpColors.push_back(ExprColor(L"#modify class", HlDirect));
   ExpColors.push_back(ExprColor(L"#property", HlDirect));
   ExpColors.push_back(ExprColor(L"#method", HlDirect));
   ExpColors.push_back(ExprColor(L"#public property", HlDirect));
   ExpColors.push_back(ExprColor(L"#public method", HlDirect));
   ExpColors.push_back(ExprColor(L"#drop property", HlDirect));
   ExpColors.push_back(ExprColor(L"#drop method", HlDirect));
   ExpColors.push_back(ExprColor(L"#drop class", HlDirect));
   ExpColors.push_back(ExprColor(L"#return", HlDirect));
   ExpColors.push_back(ExprColor(L"#var", HlDirect));
   ExpColors.push_back(ExprColor(L"#varstack", HlDirect));
   ExpColors.push_back(ExprColor(L"#funcstack", HlDirect));
   ExpColors.push_back(ExprColor(L"#prmstack", HlDirect));
   ExpColors.push_back(ExprColor(L"#objstack", HlDirect));
   ExpColors.push_back(ExprColor(L"#clstack", HlDirect));
   ExpColors.push_back(ExprColor(L"#procstack", HlDirect));
   ExpColors.push_back(ExprColor(L"#frgstack", HlDirect));

//words & expressions
   ExpColors.push_back(ExprColor(L"if", HlExpr));
   ExpColors.push_back(ExprColor(L"else", HlExpr));
   ExpColors.push_back(ExprColor(L"for", HlExpr));
   ExpColors.push_back(ExprColor(L"count", HlExpr));
   ExpColors.push_back(ExprColor(L"while", HlExpr));
   ExpColors.push_back(ExprColor(L"select", HlExpr));
   ExpColors.push_back(ExprColor(L"when", HlExpr));
   ExpColors.push_back(ExprColor(L"sym", HlExpr));
   ExpColors.push_back(ExprColor(L"num", HlExpr));

//special symbols
   ExpColors.push_back(ExprColor(VARSYM, HlVarSym));
   ExpColors.push_back(ExprColor(OBJSYM, HlObjSym));
   ExpColors.push_back(ExprColor(L"//", HlComment));
   ExpColors.push_back(ExprColor(L"{", HlBrace));
   ExpColors.push_back(ExprColor(L"}", HlBrace));
   ExpColors.push_back(ExprColor(L";", HlEndl));
}
//---------------------------------------------------------------------------

String RemoveTabs(String fragment, int spaces_cnt)
{
  String spaces_remover;

  for (int i = 1; i <= spaces_cnt; i++)
	 spaces_remover += " ";

  fragment = ParseString(fragment, String(wchar_t(VK_TAB)), spaces_remover);

  return fragment;
}
//---------------------------------------------------------------------------

int HighlightSource(TRichEdit *src, int line_ind)
{
  int res = 0;

  try
	 {
       src->PlainText = false;
	   TPoint user_caret = src->CaretPos;
	   src->SelAttributes = src->DefAttributes;

	   MarkFragmentsInLine(src, line_ind);

	   for (int i = 0; i < vecSelFragments.size(); i++)
		  {
			src->SelStart = vecSelFragments[i].SelPos;
			src->SelLength = vecSelFragments[i].SelLength;
			src->SelAttributes->Color = vecSelFragments[i].SelColor;
		  }

	   src->CaretPos = user_caret;
	   src->SelAttributes->Color = src->Font->Color;

	 }
  catch (Exception &e)
	 {
	   SaveLog("exceptions.log", "ELISourceHighlighter::HighlightSource: " + e.ToString());
	   res = -1;
     }

  return res;
}
//---------------------------------------------------------------------------

int HighlightSourceFull(TRichEdit *src)
{
  int res = 0;

  try
	 {
	   for (int i = 0; i < src->Lines->Count; i++)
		  {
			HighlightSource(src, i);
		  }
	 }
  catch (Exception &e)
	 {
	   SaveLog("exceptions.log", "ELISourceHighlighter::HighlightSourceFull: " + e.ToString());
	   res = -1;
	 }

  return res;
}
//---------------------------------------------------------------------------

void MarkFragmentsInLine(TRichEdit *src, int line_ind)
{
  try
	 {
	   vecSelFragments.clear();

	   String text = src->Lines->Strings[line_ind];
	   unsigned int cnt_sym_before = 0;

	   for (int j = 0; j < line_ind; j++)
		  cnt_sym_before += src->Lines->Strings[j].Length();

	   src->SelStart = cnt_sym_before + line_ind;
	   src->SelLength = src->Lines->Strings[line_ind].Length();
	   src->SelAttributes->Color = src->Font->Color;

	   for (int i = 0; i < ExpColors.size(); i++)
		  {
			int pos;

			while (pos = text.Pos(ExpColors[i].Expression))
			  {
				int expr_len = wcslen(ExpColors[i].Expression);

				MarkedFragment frg = MarkedFragment(line_ind,
													cnt_sym_before + line_ind + pos - 1,
													expr_len,
													ExpColors[i].Color);
				vecSelFragments.push_back(frg);
				text.Delete(pos, expr_len);
				text.Insert(CreateDummyString(expr_len), pos);
			  }
		  }
	 }
  catch (Exception &e)
	 {
	   SaveLog("exceptions.log", "ELISourceHighlighter::MarkFragmentsInLine: " + e.ToString());
	 }
}
//---------------------------------------------------------------------------

String CreateDummyString(int length)
{
  String res;

  try
	 {
	   for (int i = 0; i < length; i++)
		  {
			res += '~';
		  }
	 }
  catch (Exception &e)
	 {
	   SaveLog("exceptions.log", "ELISourceHighlighter::MarkFragmentsInLine: " + e.ToString());
       res = "";
	 }

  return res;
}
//---------------------------------------------------------------------------

