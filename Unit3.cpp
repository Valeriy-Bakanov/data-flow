//---------------------------------------------------------------------------
#include "stdio.h"
#include <vcl.h>
#pragma hdrstop

#include "Unit1.h" // ��� ������� � Unit1
#include "Unit2.h"
#include "Unit3.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TF3 *F3;

#define ULI unsigned long int // ������� ����� ��� �����
#define UI  unsigned int // ����� ��� �����
#define INT  long // ��� �����

#define _128  128
#define _1024 1024

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
char* __fastcall // ������������ ��������� ������ Str �� ������� n1 �� n2 (������� � 1 !!! )
GetSubString(char *Str, int n1, int n2)
{
 char tmp[_1024] = "\0"; // ������� ������
 int j = -1;
//
// strcpy(tmp, "\0"); // �������� ������
//
 for(int i=0; i<strlen(Str); i++)
  if( ((i+1) >= n1) &&
      ((i+1) <= n2))
  {
   tmp[++j] = Str[i];
   tmp[j+1] = (char)'\0';
  }
//
 strcat(tmp, "\0");
//
 return tmp ;
} // ---------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
__fastcall TF3::TF3(TComponent* Owner) : TForm(Owner)
{ // ����������� ������ ����� ( ����������� )
} //----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void __fastcall TF3::On_Resize(TObject *Sender)
{ // ���������� ��� ��������� �������� ����� F2
 int W = F1->Width, // ����� ������� ������ �������� ���� F1 ���������
     H = F1->Height,
     min_Width_F3  = 500, // ����������� �� ������ ������, ���������� � ����
     min_Height_F3 = 400; // ��������� 4/5
// -----------------------------------------------------------------------------
 if( F3->Width < min_Width_F3 ) // ��������� ������ - ����� ����� �� �����������..!
  F3->Width = min_Width_F3;
//
 if( F3->Height < min_Height_F3 )
  F3->Height = min_Height_F3;
//
} //----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void __fastcall TF3::Series1GetMarkText(TChartSeries *Sender, int ValueIndex, AnsiString &MarkText)
{ // ��������� ������ ����� �� "�����" (������� ����������)
 MarkText = "#" + AnsiString( GetSubString(F1->Tpr->Strings[ValueIndex].c_str(),41,50) ).TrimLeft();
} //----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void __fastcall TF3::Series1AfterDrawValues(TObject *Sender)
{ // ���������� ������������ ������ "�����"
 Series1->Pointer->VertSize = Chart_AIU->LeftAxis->CalcSizeValue(0.3);
} //----------------------------------------------------------------------------

