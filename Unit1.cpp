//
//#define WIN32_LEAN_AND_MEAN // �������� �� ���������� winsock.h <-> wonsock2.h
//
#include "Unit1.h"
#include "Unit2.h" // ����� ��� ������ � F2 (����� ������ ������� ������������� ����������)
#include "Unit3.h" // ����� ��� ������ � F3 (����� ������ ������� ������)
//
#include <vcl.h>
#pragma  hdrstop
#include <IniFiles.hpp>
#include <System.hpp>
#include <shellapi.h>
//
#include "SysUtils.hpp"
#include <DateUtils.hpp> // ��� DayOf, WeekOf, MonthOf � ��.
#include <Clipbrd.hpp>
#include <dos.h> // sleep
#include <sysopen.h>
//
#include <iostream.h>
#include <typeinfo.h>
//
#include "stdio.h"
#include <string.h>
#include "math.h"
#include "Math.hpp"
#include <time.h>
#include <stdlib.h>
#include <locale>
//
#include <setjmp.h> // ��� ���������� ������������ �������� � ���������
jmp_buf env_BypassMacro; // � env_BypassMacro ����������� ��������� � ����� ������ setjmp
short i_env_BypassMacro = -1; // ��� ���������� setjmp (����� longjmp ��������� ������ ��� i_env_BypassMacro >= 0)
//
#include <IniFiles.hpp>
#include <sys\timeb.h>
#include <SysUtils.hpp>
#include <ctype.h> // isdigit()
#include <vcl/dstring.h>
//
using namespace std;
//
#include "parser.h"
//
#define  TParser ::TParser // !!! ����� ����������� � ���� ������ � � #include "parser.cpp" !!!
#include "parser.cpp" // Written by Chaos Master, 11-th of Febrary, 2000
//------------------------------------------------------------------------------
#pragma package(smart_init)
// #pragma link "TProcTimer"
#pragma resource "*.dfm"
#pragma warn -8057 // ��������� ��������� E8057 Parameter 'Sender' is never used
#pragma warn -8080
#pragma warn -8059
#pragma warn -8012
//
TF1 *F1; // ������� ����
//
//#include "pcre.h" // ���������� ���������� ��������� �� Borland C++ RTL
//
TParser parser; // ����� TParser ��������� :: ��� ����������� ��������� ������� ���������
//
//==============================================================================
#define APM Application->ProcessMessages(); // ���� ���������� Windows
//
#define max(a, b) (((a) > (b)) ? (a) : (b))
#define min(a, b) (((a) < (b)) ? (a) : (b))
//
#define DSTA(str) DelSpacesTabsAround(str) // ������ ������� � Tab ������ ������
#define DAS(str)  DeleteAllSpaces(str) // ������ ��� ������� � ������ str
//
char // 3-� ��� 2-� �������� ������� ( + ���� ��������� + ����������� )
pattern[] = "[A-Z]{3}" // ��������� ������� (����� 3 ���������, ��������)
            "[ A-za-x0-9_]{1,32}"  // ������ + �������_1 (1-32 �������)
            "[ ,A-za-x0-9_]{1,32}" // ������ + ������� + �������_2 (1-32 �������)
            "[ ,A-za-x0-9_]{1,32}" // ������ + ������� + �������_3 (1-32 �������)
            "[ ,A-za-x0-9_]{1,32}" // ������ + ������� + �������� (1-32 �������)
            "[ ;A-za-x0-9_]{0,}"   // ������ + ����� � ������� + �����������
            ; // ������ ������
//
#define strcat(dest,src) (strncat(dest,src,sizeof(dest)-strlen(dest)-5)) // ���������� ���������� src � dest
// !!!!! ����� 5 - ������ �������������� ������ ("�� ������ ������") !!!!!!!!!!!
//
char Ident[] = "Bakanov Valery Mikhailovich, http://vbakanov.ru/left_1.htm , Moscow, Russia, 2009-2025\n \
����� ������������ �������� ������� ������� �� �������� ���� ����������� ��������-���������������� ���������������� \
(������ ������ ����� ����� ����� ���������!) � ������� ������� ����� ���� �� ���������� ���\n \
The author of the software Valery Bakanov is not an ardent supporter of Object-Oriented Programming \
(Linus Torvalds definitely sits inside the author!) and therefore most of the code does not use OOP";
//
char *Info_CommonStr[] = { // �������������� ������ ����������� ShellExecute(...)
"Base.pdf", // [#0]
"http://vbakanov.ru/dataflow", // [#1]
"http://vbakanov.ru/spf@home", // [#2]
"http://vbakanov.ru/poems_04.htm#dataflow", // [#3]
"https://algowiki-project.org/ru/", // [#4]
"https://www.ispras.ru/dcouncil/docs/diss/2021/chernyh/chernyh.php?sphrase_id=5604670", // [#5]
"https://www.litres.ru/book/v-m-bakanov/prakticheskiy-analiz-algoritmov-i-effektivnost-parallelnyh-vyc-70184365/" // ����� ������ �� LitRes.ru [#6]
} ;
//
char trueLowerCase[]  = "true",        trueUpperCase[] ="TRUE",
     falseLowerCase[] = "false",       falseUpperCase[]="FALSE",
     trueOutput[]     = "1 | true",    falseOutput[] = "0 | false", // ����� �������������������� ����������� ������� (\x00A6 - ����������� ������������ �����)
     notDefined[]     = "? ? ?",
     startComments[]  = ";\0", // ������ ������ ����������� � ������ ";"
     symbolNot_1 = '!',                symbolNot_2 = '~', // ������� ����������� ��������� ! ��� ~
     SET[]       = "SET"; // ��� ���������� SET
//
#define is_SET(Set) ( !strcmp( Set, SET ) ) // ��� ���������� SET
#define is_PredicatOrSET(Set) ( is_Predicat( Set ) || !strcmp( Set, SET ) ) // ��� �������� ��� SET
char SPECUL[] = " [specul]"; // ������� �������������� ��������� ����������
//
#define strNcpy(d,s) strncpy(d,s,sizeof(d)) // ���������� �������
//------------------------------------------------------------------------------
#define GetRand(a,b) (rand()%((b)-(a)+1)+(a)) // ������������� ��������� ����� �� a �� b (������������)
//------------------------------------------------------------------------------
#define ERR_ALTERNAT -13.4567 // ������� ������� �� 0 ��� ���������� ����������������
//------------------------------------------------------------------------------
#define REAL double // ��� ������������ ����� ��� �����������
#define INT signed long // ��� �����
//
#define ULI unsigned long int // ������� ����� ��� �����
#define UI  unsigned int // ����� ��� �����
//------------------------------------------------------------------------------
#define sleep_for_vizu_buffer Delay_Vizu_Buffer(); // ����� ��� ������������ ������
//------------------------------------------------------------------------------
#define   _16     16 // ����������� �������� ��� ���� ���������
#define   _32     32
#define   _64     64
#define   _128   128
#define   _256   256
#define   _512   512
#define  _1024  1024
#define  _2048  2048
#define  _4096  4096
#define  _8192  8192
#define _16384 16384
//------------------------------------------------------------------------------
#define ACC_REAL 4 // ����� ������ ����� ���.��� ������ "���������" �� E-�������
//==============================================================================
#define _SET_LEN 4  // ����� ������ ��������� ���������� (3 ������� + "\0")
#define _ID_LEN  53 // 53 - ����� ������ ����� ���������� (32 ������� + 20 + "\0")
//------------------------------------------------------------------------------
#define do_Run \
{ F1->BitBtn_Run->Enabled = true; /* "��������" ��� ������ ������ ���������� */ \
  F1->E_AIU->Enabled      = true; \
  F1->Files->Enabled      = true; \
  F1->Edit->Enabled       = true; \
  F1->Work->Enabled       = true; \
  F1->Wonderful->Enabled  = true; \
  F1->Analize->Enabled    = true; \
  F1->Label_Data->Font->Color   = clBlack; \
  F1->Label_Buffer->Font->Color = clBlack; }
//
#define do_Stop \
{ F1->BitBtn_Run->Enabled = false; /* "���������" ��� ������ ����� ���������� */ \
  F1->E_AIU->Enabled      = false; \
  F1->Files->Enabled      = false; \
  F1->Edit->Enabled       = false; \
  F1->Work->Enabled       = false; \
  F1->Wonderful->Enabled  = false; \
  F1->Analize->Enabled    = false; }
//
INT tick_ScanForGraph = 10, // ����� ��������������� ������ ������� ������������� ���������� (�����/������)
    d_APM = 1000; // ������ d_APM ���� ���������� Windows
INT StartNumb = 100; // ��������� ����� ���������� ��� ������ � ���� ���
#define DEFAULT_RETURN_GET_DATA 0.123456789 // ������������ ��� ������������ ��������� ����� � Mem_Data[]
//
struct timeb t0, t1; // ������ ������ ������� ����������
void   ftime(struct timeb *buf);
//
INT all_maxProcs, // ����� ����������� � ����������� ���
    simult_maxProcs, // max ����� ������������ ����������� � ����������� ���
    serial_Ticks,    // ����� ������ ������ ��� (���������� ����� ����������������� ����������, �����)
    parallel_Ticks;  // ����� ���������� ������������ ���������, �����
//
//------------------------------------------------------------------------------
char uniqueStr[_512] = "\0"; // ���������� ������ ��� ���� ������ ( ���� + ����� �� ���� )
struct {  // ������� ��� �������� � Unload_Date()
 char savePRO[_512] /*
      saveTPR[_512],
      saveDAT[_512],
      saveTST[_512],
      saveCOI[_512],
      saveGV[_512],
      saveMVR[_512] */ ;
} SF; // ������ ��� �������� ��� ������ (kj,fks)
//
__int64 SessionStartTime = 0L; // ����� ������ ������ ������������ (������)
//
void   __fastcall Read_Config( int Rule ); // ���������� ��������� ��������� � �������� F1
void   __fastcall Write_Config();
void   __fastcall Delay(long mSecs);  // ����� mSecs ����������� (� ����������� ���������� WINDOWS)
void   __fastcall RunExternal(char* CommandLine, byte RuleParent, byte Priority, bool RuleMessage);
bool   __fastcall Read_Instructions(); // ������ ���������� �� ����� FileNameSet � ������ �������� Mem_Instruction[]
//
bool   __fastcall Vizu_Instructions(); // ������������ ���������� � SG_Set  �� ������� �������� Mem_Instruction[]
void   __fastcall Vizu_Data(); // ������������ ������ � SG_Data �� ������� �������� Mem_Data[]
void   __fastcall Vizu_Buffer(); // ������������ ������ � SG_Buffer �� ������� �������� Mem_Buffer[]
//
char*  __fastcall Vizu_Flags(INT i_Set); // ������������ ������ ���������� Mem_Instruction[i_Set]
//
void   __fastcall Primary_Init_Data(); // �������������� ������������ ������ ������� ������ etc etc
void   __fastcall ExecuteInstructions_Except_SET(INT i_Set); // ��������� ��� (����� SET) ���������� ����� i_Set
void   __fastcall Install_All_Flags(); // ������� ��� ����� � Mem_Instruction[]
void   __fastcall Calc_Stat_Proc();  // ����������� ���������� ������������� ���
//
float  __fastcall Calc_Param(INT i_Set); // ����������� ���������� ���������� i_Set
float  __fastcall Calc_Priority(float Param); // �� �������� Param (����������) ������������
// ��������� ������ ���������� ��� �� ������� �� ��������� ���
// ������� ��� (���������� � ����������) � ��� ������ ��������� �� � ����� Mem_Buffer[]
bool   __fastcall Test_aResult_Eq_aOperand(INT i_Set); // ���� �� ���������� ������ ���������� � �������� ������ ��� ����� ���������
char*  __fastcall Get_Time_asLine(); // ������� ����� (� ������ ������� � 0) � ������
//
void   __fastcall AttemptExecMaxInstructions_fromBuffer(); // �������� ������� �� ������ � ��������� �������� ���-����������
void   __fastcall Vizu_Flow_Exec(); // ��������������� ������� ���������� ���������
void   __fastcall Start_DataFlow_Process(int Mode); // ������ ���� (� �������� Mode ������)
//
int    __fastcall Get_CountOperandsByInstruction(char *Set); // ���������� ����� ��������� ���������� Set
char*  __fastcall GetSubString(char *Str, int n1, int n2); // ������� ��������� �� Str �� n1 �� n2 �������� ������������
char*  __fastcall Line_Set(INT i_Set, short int Rule, REAL Result); // ���������� ����� ���������� �� Mem_Instruction[i]
void   __fastcall add_Data(INT i_Set, char* aResult, REAL Data); // ��������� � Mem_Data[] ����� �� ������ (������!) Addr
void   __fastcall add_Buffer(INT i_Set, int Rule); // ��������� � ����� ������ ������ � ���-����������� i_Set (Rule ���������� ����� ������ �������)
int    __fastcall Get_TicksByInstruction(char *Set); // ���������� �� Set_Params->Time ����� ���������� ���������� Set(char *Set);
int    __fastcall Get_Free_Proc(); // ���������� ����� ��������� ���
REAL   __fastcall get_Data(char Addr[], bool flagCountRead); // ���������� �� Mem_Data[] ����� �� ������ (������!) Addr (��� flagCountRead ���� Mem_Data[i].CountRead �� ����������)
void   __fastcall Display_Error(char *str); // ���������� �� �������������� ������
void   __fastcall Test_Visu_Buffer_Fill(); // ������������ ��������� ������������� ������
int    __fastcall Select_Instruction_fromBuffer(); // ���������� ����� ���������� �� ������ �� ������� ��� ����������
void   __fastcall Calc_Level_Buffer_Fill(); // ��������� ������������� ������ � %% � ������� Level_Buffer_Fill
//
float  __fastcall Calc_01_Param_Instruction(INT i_Set); // ����c���� ���������� ���������� i_Set �� 01-�� ������
float  __fastcall Calc_02_Param_Instruction(INT i_Set); // ����c���� ���������� ���������� i_Set �� 02-�� ������
float  __fastcall Calc_03_Param_Instruction(INT i_Set); // ����c���� ���������� ���������� i_Set �� 03-�� ������
//
int    __fastcall Work_TimeSets_Protocol_IC();  // �������� � ������� ������������� ����������
int    __fastcall Work_TimeSets_Protocol_AIU(); // �������� � �������������� ��� (��������� ������)
////////////////////////////////////////////////////////////////////////////////
void   __fastcall Finalize_Only_SET(INT i_Set); // ���������� ��� ���������� ���������� ���������� SET ����� i_Set
void   __fastcall Finalize_Except_SET(INT i_Proc); // ���������� ��� ���������� ���������� ���������� �� ��� i_Proc
void   __fastcall Delay_Vizu_Buffer(); // ���� Delay_Buffer �������� ��� ������������ ������
////////////////////////////////////////////////////////////////////////////////
REAL   __fastcall Calc_ConnectedIndex(int Rule); // ���������� ������������� ��������������� ����� ���������
REAL   __fastcall StrToReal(char *str, INT i_Set); // ����������� ������ � REAL � ��������� ��� ���������� ���������� ����� i_Set
//
bool   __fastcall Test_All_Operands(); // ��������� ������������� ������������� ��������� ��� ���� ���������� (������� ��� ������)
void   __fastcall Mixed_Instructions(); // ���������� ��� ���������� � Mem_Instructions[] � ��������� �������
void   __fastcall Start_DF( int Mode ); // ����� ���������� (��� / � �������������� ���������� ��� Mode 0 / #0 )
//
int    __fastcall RunPreProcessor(); // ������������ ��������� ��������
int    __fastcall PreProcRow_For1(INT iRow, char* c, INT iCycle); // ����������� iRow ������ ���� ������� ����������� �������
//int    __fastcall FindAndReplaceSubString(char* Source, int Start, int Len, char* Old, char* New);
char*  __fastcall ParseAndCalculateIndex(char* Expression); // ������ � ���������� ��������� (��� ����������) exp
// int    __fastcall CalculateIndexes(char* str); // ������������� ���������� �������� ������� (��� ����������) ������ str
void   __fastcall Out_Data_SBM1(); // ����� ������ � ������� ����� StatusBar
char*  __fastcall strReplace( char* dest, int num, const char* source, const char* orig, const char* rep ); // ������ ��������� � ������
////////////////////////////////////////////////////////////////////////////////
bool   __fastcall is_ResultIsPredicat(char* str); // ���������� TRUE, ���� str - ���� ���������
bool   __fastcall is_Predicat(char* Set); // ���������� TRUE, ���� ���������� Set ���� ���������
//
void   __fastcall Draw_AllTableInstructions(); // �������� ������ ������� ���������� ������ � ��������
void   __fastcall Clear_AllTableInstructions(); // ������� ���� ���� ����� ������� ����������
void   __fastcall StopCalculations(int Rule); // ������� ����������� �������� � Rule
void   __fastcall ReplaceEqualLengthSubstring( char *String, char *OldSubstring, char *NewSubstring ); // ������ "\" �� "/"
//
int Delay_Buffer = 0; // ����� (� ��������) ��� ������������ ������
//
void   __fastcall DelSpacesTabsAround(char *str); // ������� ������� ����� � ������ ������ str
char*  __fastcall DeleteSymbolAll( char str[], char symb ); // ������� ��� symb � ������ str
char*  __fastcall DeleteAllSpaces( char *str ); // ������� ��� ������� � ������ str
//
void  __fastcall Save_Protocol_Master(); // ���������� ��������-��������� ���������� ������� (*.pro)
void  __fastcall Save_Protocol_AIU(); // ���������� ���������_�������������_���_��_������� (*.tpr)
void  __fastcall Save_Protocol_Data(); // ���������� ���������_������ (*.dat)
void  __fastcall Save_Protocol_ExecInstructions(); // ���������� ���������_����������_����������_��_������� (*.tst)
void  __fastcall Save_Protocol_ConnectedGraph(); // ���������� ���������_CDZPYJCNB_UHFAF (*.coi)
//
void  __fastcall Save_All_Protocols_To_Out_Dir(); // ��������� ��� ��������� � ���������� Out!Data
//
bool  __fastcall AddLineToProtocol(char *str); // �������� ������ � ��������� ���� ���������
//
void /*__fastcall*/ t_printf(char *fmt, ...); // ��������� ����� � ���� ���������
//
void  __fastcall Save_IGA(); // ��������� ���� � ���� ������ ��� (������ *.GV)
void  __fastcall Extended_Save_IGA(); // ��������� ������ � ������� ���������� ���������� (������ *.MVR)
char* __fastcall PutDateTimeToString(INT flag); // ������ ������� ���� � ������� � ������ � ���������������
//
void  __fastcall Make_Row_Current( INT Row ); // ������� ������ Row (�������� � 0) �������
void  __fastcall Run_Infinity(); // ������ ����������� ��������� ����������� ����� ���
//
void  __fastcall GetFileFromServer(  char *FileNameSource, char *FileNameDestination, bool Replace  ); // ����� ���� � �������
void  __fastcall PutFileToServer(  char *FileNameSource, char *FileNameDestination, bool Replace  ); // ��������� ���� �� ������
void  __fastcall Unload_Install(); // ��������� � ������� ��������������� ������ ��������
void  __fastcall Work_LogInOut( int Rule); // �������� � ������/����� ������ ��������� DATA_FLOW.EXE
void  __fastcall Upload_Data( int Rule); // ��������� ����� �� ������ (� ����������� �� Rulq)
//==============================================================================
#define mR F1->M1  // ������ � M1 (����� ��������� ���������� ���������)
#define mB F1->SG_Buffer // ������ � ������� ����� ������������ ������ ������
#define mP F1->PB_1      // ������ � ���������������������� ������ ������
#define mS F1->SG_Instruction // ������ � ������� �����  ����������
#define mD F1->SG_Data   // ������ � ������� ����� ������
#define mTpr F1->Tpr // ������ � F1->Tpr
#define mPM  F1->PM  // ������ � F1->PM
//------------------------------------------------------------------------------
#define SBM  F1->SB // ������ ������ ������ ������
#define SBM0 F1->SB->Panels->Items[0] // ��� ������ � Panels[0]
#define SBM1 F1->SB->Panels->Items[1] // ��� ������ � Panels[1]
#define SBM2 F1->SB->Panels->Items[2] // ��� ������ � Panels[2]
//
#define MI_AOP1(i) ( !strcmp(Mem_Instruction[i].aOp1,aResult) ) // �������� ���������� � aResult 1-�� ����� �������� i-��� ����������
#define MI_AOP2(i) ( !strcmp(Mem_Instruction[i].aOp2,aResult) ) // ... 2-�� �������� i-��� ����������
//
#define MI_FOP1(i) ( Mem_Instruction[i].fOp1 ) // ��������� � ����� ���������� 1-�� �������� i-��� ����������
#define MI_FOP2(i) ( Mem_Instruction[i].fOp2 ) // ... 2-�� �������� i-��� ����������
//
char
pattern_01[] = "[=][0-9]{1,}[/][0-9]{1,}[:]" // =n1/n2: -name1 Val1 -name2 Val2 ...
               "([ ][-][A-za-x0-9_]{1,}[ ][.,0-9]{1,}){0,}" // ������ ����� �� �������������
               ;
char
pattern_02[] = "[=][0-9]{1,}[/][0-9]{1,}[:]" // =n1/n2: -name1 minVal1 maxval1 -name2 minVal2 maxVal2 ...
               "([ ][-][A-za-x0-9_]{1,}[ ][.,0-9]{1,}[ ][.,0-9]{1,}){0,}" // ������ ����� �� �������������
               ;
//------------------------------------------------------------------------------
//
struct { // DrawColorTest (��������� ����� ������ ��� ������������ ��� ���������� ���������)
 TColor clOperandOperation, // RGB(255,185,185) ������� ���� ������ � SG_Set �� �������� "��������<->��������"
        clNonExecuted, // RGB(255,185,255) ���������� ���� �� �������� "������������� ������"
        clNonUsedResult, // RGB(255,192,0) ���������� ���� �� �������� "���������������� ����������"
        clNonDefOperands; // RGB(0,255,255) ������� ���� �� �������� "�������������� ��������"
} DCT; // [DrawColorTest] ��������� ������ ��� ������������ ��� ���������� ���������
//
struct { // DrawColorExec (��������� ����� ������ ��� ���������� ���������)
 bool needDrawColors;
 TColor clReadyOperand, // RGB(255,153,255) ���� ������ ��������_������ (���������)
        clTruePredicat, // RGB(128,255,0) ...��������=TRUE (������)
        clSpeculateExec, // ���� ���������� �������������� ����������
        clExecSet; // RGB(255,51,153) ...����������_����������� (������-�������)
} DCE; // [DrawColorExec] ��������� ������ � �������� ����������
//
struct { // DrawGraph (��������� ������� ������������� ����������)
 TColor clGraphStart, // ������ � ����� ���� ������� ������������� ���������� RGB(255,0,0)->RGB(0,255,0)
        clGraphEnd;
} DGR; // [DrawGraph] ���� ���� ������� ������������� ����������
//
struct { // Read-Write-Config (����� ������ � �������� ����� ������������ �������)
 char *Sect1,  *Sect1_Var1,*Sect1_Var2,*Sect1_Var3,*Sect1_Var4,
      *Sect2, // ������ ���������� - ����������� �������������
      *Sect3,  *Sect3_Var1,*Sect3_Var2,
      *Sect4,  *Sect4_Var1,*Sect4_Var2,
      *Sect5,  *Sect5_Var1,
      *Sect6,  *Sect6_Var1,
      *Sect7,  *Sect7_Var1,
      *Sect8,  *Sect8_Var1,*Sect8_Var2,*Sect8_Var3,*Sect8_Var4,
      *Sect9,  *Sect9_Var1,
      *Sect10, *Sect10_Var1,
      *Sect11, *Sect11_Var1,
      *Sect12, *Sect12_Var1,
      *Sect13, *Sect13_Var1,*Sect13_Var2,*Sect13_Var3,*Sect13_Var4,*Sect13_Var5,
      *Sect14, *Sect14_Var1,*Sect14_Var2,*Sect14_Var3,*Sect14_Var4,
      *Sect15, *Sect15_Var1,*Sect15_Var2,
      *Sect16, *Sect16_Var1,
      *Sect17, *Sect17_Var1,
      *Sect18, *Sect18_Var1,
      *Sect19, *Sect19_Var1;
} RWC = {
 "Max_Lengths", // [1] �������� ����������
  "max_Instruction","max_Data","max_Proc","max_Buffer",
 "Ticks", // [2] ������ ����� ���������� ���������� � �����
 "Editor_File", // [3] ������ ����� ���������� ���������
  "File","Mode",
 "Strategy", // [4] ��������� ������� ������ ���������� �� ������
  "how_Calc_Param","how_Calc_Prior",
 "Delay_Vizu_Buffer", // [5] ����� ������������ ������ (��� #0 ����� ���������� ��������� ����� ��������)
  "Delay",
 "Graph", // [6] ���������� �������� ������ ������� ������������� ����������
  "tick_Scan",
 "Tick_Interval", // [7] // ����� ���� � �������������
  "Interval",
 "Pos_F1", // [8] ��������� � ������� ������� �����
  "Top","Left","Width","Height",
 "LastFileNameSet", // [9] ��� ����� ���������
  "FileNameSet",
 "RuleVarData", // [10] ������� - ��������� ���� ���������� � ������������ ���������� �� ��������
  "attrVar",
 "Auto_Graph", // [11] ������������ �� ������ �������������� ���������� ����� �� ��� ��������� ���������� ���������
  "NeedAutoGraph",
 "WaitNextInstruction", // [12] ����� ������� ����� ����� ����� ���������� ���������� ����� �������, ��� ��������� ��������
  "WaitTicks",
 "DrawColorExec", // [13] ����� ����� ��� ���������� ���������
  "needDrawColors","clReadyOperand","clTruePredicat","clExecSet","clSpeculateExec",
 "DrawColorTest", // [14] ����� ����� ��� ������������ ���������
  "clOperandOperation","clNonExecuted","clNonUsedResult","clNonDefOperands",
 "DrawColorGraph", // [15] ������ � ����� ���� ������� ������������� ����������
  "clGraphStart","clGraphEnd",
 "StartNumbOps", // [16] ��������� ����� ���������� ��� ����������� � *.gv
  "StartNumb",
 "Vizu_Dynamic", // [17] ����� ��� �������� ����������� ������ � SG_Set[][] ����� �� ����������� ���������
  "pass_Counts",
 "RuleSpeculateExec",  // [18] ��������� �� (��� ���������� ����������) ���������� � Predicate_����=false
  "SpeculateExec",
 "RuleTokenUse", // [19] ������-����������� ��� ������������ �������
  "TokenUse"
 } ; // [Read-Write-Config] ����� ������ � �������� ����� ������������
//
//------------------------------------------------------------------------------
#define BUFFER_EMPTY (-1313) // ����� ����
//==============================================================================
//
char FileNameINI[_512]    = "data_flow.ini", // ��� ����� ��������
     FileNameSet[_512]    = "data_flow.set", // ��� ����� ������ ���������� ��� ������
     FileNameSetPrP[_512], // ��� ����� ���������� ����� �������������
     ExtPrP[]             = ".set_PrP", // ���������� ����� ����� ���������� ����� �������������
     ExtIGA[]             = ".gv", // ���������� ����� ����� ��� �������� ���-������
     ExtMVR[]             = ".mvr", // ���������� ������ ��� �������� ������ � ������� � ������� ���������� ��������
     FileNameEditor[_512] = "notepad.exe"; // ���� ���������� ���������
FILE *fptrIn, *fptrOut; // ��������� �� ������� �������
//
bool modeEdit  = false, // ����� ������ ���������� ���������
     modeGraph = true, // ����� ��������� ������� ������������� ����������
     modeStopRunInfinity = true; // ���������� ����������� ���������� ��������� ����������� ���������
//
int Regim = 1; // ��� Regim = 1 ���������� ����������
               // ��� Regim = 0 ���������� ����������� �������������
               // ��� Regim = 2 ����c����� �������������� �����������
//
int Code_Error = 1; // Code_Error = -1  -.- ������� ������� �� 0
                    // Code_Error = -2  -.- ������� ���������� ����������� ����� �� �������������� �����
                    // Code_Error = -3  -.- ������ ���������� ��������
                    // Code_Error = -4  -.- ������ ���������� � ������� (x**y)
                    // Code_Error = -5  -.- ������ ������������ ���������
                    // Code_Error = -13 -.- ����� ���������� ��������� � ����� (��� ������) �������� ����������
////////////////////////////////////////////////////////////////////////////////
//
#define MAX_PROC 1000000 // ������������ ����� ��� 10^6
struct mp {
 bool Busy; // ��������� ��� (���� 0=FALSE - ��������, ����� - �����)
 INT  i_Set; // ����� ���������� (�� ���� ����������)
 char aOp1[_ID_LEN], // ����� (������!) ������� ��������
      aOp2[_ID_LEN], // ����� (������!) ������� ��������
   aResult[_ID_LEN]; // ����� (������!) ����������
 REAL Op1, Op2, Result; // �������� ������ �� ������� aOp1,aOp2,aResult (��� ��������)
// double t_Start; // ������ ������� ������ ���������� ����������
 INT tick_Start; // ������ (���, ����) ������ ���������� ����������
} M_P, *Mem_Proc=NULL;
INT max_Proc = _128, // �������������� ������
    Free_Proc; // ������� ����� ��������� ���
//
////////////////////////////////////////////////////////////////////////////////
#define MAX_INSTRUCTION 1000000 // ������������ ������ ���� ���������� 10^6
// �������� ��������� �������� ����������
struct mi {
 char Set[_SET_LEN], // ��������� ����������
      aOp1[_ID_LEN],  // ����� (������!) ������� ��������
      aOp2[_ID_LEN],  // ...�� �� ������� ��������
      aResult[_ID_LEN], // ����� (������!) ���������� ��������
      aPredicat[_ID_LEN]; // ����� (������) ����� ��������� ��������
 bool fOp1,fOp2, // ������� ���������� 1-�� � 2-�� ���������
      fPredicat, // ������������� ����� ��������� ( true/false ��� ���������� )
      fPredicat_TRUE, // TRUE - ���������� ��������� (���������� ����� ��������� � ����� ������)
      fSpeculateExec, // ��� TRUE ���������� �����������, �� ��������� �������� (����� Itanium)
      fExec,       // ������� ���� (TRUE), ��� ��� ���������� � ������ ������ ����������
      fExecOut,    // ������� ���������� ���������� (TRUE - ����������� ���� ���)
      fAddBuffer;  // ������� ����, ��� ���������� ��������� � ������
 char Comment[_256];  // ����������� � ����������
} M_I, *Mem_Instruction=NULL;
INT max_Instruction = _128,   // �������������� ������
    Really_Set = 0,   // ������� ��������
    Already_Exec = 0; // ��� ���������
//
////////////////////////////////////////////////////////////////////////////////
// #define MAX_SEL 1000000 // ��� ����� ���������� ������ ����� � SG_Set (������ <= NAX_SET)
// �������� ������� �������� ���������� ������ �����
struct sc {
 int Row, Col; // ����� ������ � ������� � SC_Set
 TColor clBackground, clSymbol; // ����� ���� ������ � �������� ��� �������������
} S_C, *Sel_Cell=NULL;
INT Really_Select = 0; // ������� ��������
//
/////////////////////////////////////////////////////////////////////////////
#define MAX_DATA 1000000 // ������������ ������ ���� ������ 10^6
// �������� ��������� ����� ������ ������
struct md {
 char Addr[_ID_LEN]; // ����� ������ (������!)
 REAL Data; // ���������� ������
 ULI CountRead; // ����� ���������� �������
 INT i_Set; // ����� ����������, � ���������� ������� ���� ��������� ��� ��������
} M_D, *Mem_Data=NULL;
INT max_Data = _128,  // �������������� ������
    Really_Data = 0; // ������� ��������
//
////////////////////////////////////////////////////////////////////////////////
#define MAX_BUFFER 1000000 // ������������ ������ ������ ���-���������� 10^6
// �������� ��������� ������ ���-����������
struct mb {
 INT i_Set; // ����� ���-���������� �� ���� ����������
 float Param, // �������� ��� ����������� ����������
       Priority; // ���������� ��������� (��� ������ - ��� ������ ���������� ����������� �� ����������)
} M_B, *Mem_Buffer=NULL;
INT max_Buffer = _128,  // �������������� ������
    Really_Buffer = 0, // ������� �����
    Really_Buffer_Old; // �� �� �� ���������� ���������
bool buffer_Fill = false; // ���� TRUE - ����� ��������
float Level_Buffer_Fill = 0.0; // ������������� ������ � %% [0 - 100]
//
// �������� ���������� ������ ����������
struct ip {
 char Set[_SET_LEN]; // ��������� ���������� (3 ������� + ������������� "0")
 int nInputOperands, // ����� ������� ��������� ����������
     Ticks; // ����� ���������� ���������� (� �����)
} I_P, Set_Params[] = {
 {"SET", 1,    0},
 {"ADD", 2,   20}, // ��������� ����������, ����� ������� ���������, ����� ���������� � �����
 {"SUB", 2,   20},
 {"MUL", 2,  200},
 {"DIV", 2,  200}, // 5
 {"DQU", 2,  200},
 {"DRE", 2,  200},
 {"SQR", 1,  500},
 {"SIN", 1, 1000},
 {"COS", 1, 1000}, // 10
 {"TAN", 1, 1000},
 {"LOG", 1, 1000},
 {"POW", 2, 1000},
 {"ASN", 1, 1000},
 {"ACN", 1, 1000}, //15
 {"ATN", 1, 1000},
 {"EXP", 1, 1000},
 {"SNH", 1, 1000},
 {"CNH", 1, 1000},
 {"TNH", 1, 1000}, // 20
 {"CPY", 1,  100},
 {"ABS", 1,    2},
 {"SGN", 1,    2},
 {"NEG", 1,    2},
 {"FLR", 1,    2}, // 25
 {"CEL", 1,    2},
 {"RND", 2,  100},
//
 {"PGE", 2,    1}, // ��������� ���� �����
 {"PLE", 2,    1},
 {"PEQ", 2,    1}, //30
 {"PNE", 2,    1},
 {"PGT", 2,    1},
 {"PLT", 2,    1},
//
 {"PNT", 1,    1},
 {"POR", 2,    2}, // 35
 {"PAN", 2,    2},
 {"PIM", 2,    2},
 {"PEV", 2,    2}, // 38
//
 {"PAP", 2,    2},
 {"PHS", 2,    2},
 {"PRM", 2,    2}, // 41
 {"PXR", 2,    2},
} ;
int Count_Sets = sizeof(Set_Params) / sizeof(ip); // �������� ����� ��� � ������� Instruction_Params[]
////////////////////////////////////////////////////////////////////////////////
//
#define MAX_FLOAT 1.0e+300 // ������������ ����� � ��������� ������� ...........
#define MIN_FLOAT 1.0e-300 // ����������� ����� � ��������� �������
#define EPS       1.0e-300 // ����������� �������� ���������� ------------------
// M_PI_2 = pi/2 ���������� � math.h
//
////////////////////////////////////////////////////////////////////////////////
INT localTick = 0,  // ����� ����� ����� (����������� ������) �� �������� MasterTimer
    localTickOfEndLastExecuteSet = 0, // ����� � ����� ��������� ����������� ����������
    waitAboveOfEndLastExecuteSet = 1000; // ����� ������� ����� ����������� ���������
int tick_Interval = 10; // ���� ������� ���� ������� � ������������� (���������)
////////////////////////////////////////////////////////////////////////////////
const int minW_F1 = 1024, minH_F1 = 600; // ����������� ������ ���� ������� ����� (F1)
////////////////////////////////////////////////////////////////////////////////
INT  FileSizeFromServer; // ������ ����� ��� �������� � �������
char NameSubDirOutData[] = "Out!Data", // ��� ����������� ��� ������ ������������ ������
     PathToSubDirOutData[_256], // ������ ���� � ����������� ������ ������������ ������
     NameSubDirInData[]  = "In!Data", // ��� ����������� ��� �������� ������ ������
     PathToSubDirInData[_256]; // ������ ���� � ����������� �������� ������ ������
//     MySite[] = "http://vbakanov.ru"; // ����� ����� �����
////////////////////////////////////////////////////////////////////////////////
//
// ����������� ��������� ������� ������ ���������� �� ������
////////////////////////////////////////////////////////////////////////////////
int how_Calc_Prior = 0, // ���������� ���������� (0 - ����� �������. Parameter, 1 - �������
    how_Calc_Param = 0; // ���������� ��������� (0 - ����������,
                        // 1 - ����� ���������� ����������,
                        // 2 - ��������� �����,
                        // 3 - ����� ���� ����������, ��� ������� ���������� ������ �������� �����
                        // (����� ��� ������) �� ������� ���������
////////////////////////////////////////////////////////////////////////////////
char attrVar[6]  = "$\0", // �� 5 �������� � ������ ����������, ��������� ����������� ������������� ��������� ���� ����������
     startPos[2] = "?\0"; // ��������� ������� � ������ ��� ������ ����������� ����� �������
// =============================================================================
// ���������� ������� ��������� � ������������ Mem_Data[] � Mem_Buffer[] � ��������� � ��������
bool flagAlarmData   = true,
     flagAlarmBuffer = true,
     flagAlarmParser = true;
//
int outGlobal; // ��� �������� ����, �� �������� ������� ������� ������ �����
//
bool SpeculateExec = false; // ��� true ���������� � flagPredicate_TRUE=false ���������, �� ��������� �� ���������
//
INT dummy_Ticks = 1 , // ����� ����������� ����� ��� ���������� ���������� ������ 0 �����
    pass_Counts = 0, // ���������� pass_Counts ����������� ���������� ��� ����������� ������ � SG_Set[][]
    count_Ops=0; // ����� ����������� ���������� �� ������� (��� � �����������)
//
////////////////////////////////////////////////////////////////////////////////
// ----- ������ ��������� �������������� ---------------------------------------
 char  cI, cJ; // ��� ���������� �����
 INT   minI, maxI, dI, // ������, ����� � ��� ��������� ���������
       minJ, maxJ, dJ,
       for_ID = -1; // ����� ������������� � ���������
 char  str[_1024], // ������ �������������� ����������
       SetName[_64], Opd_1[_512],Opd_2[_512], Res[_512], Pred[_512], // ������ ��� ���������, 1/2-�� ���������, ��������a, ����������
       Predic[_128], Comm[_1024],  // ���������, ������������
       nameMass[_512], indexMass[_512], indexMass_1[_512], indexMass_2[_512], // ��������� ������ ��������
       expr[_1024], tmp[]="?\0";
 char  nameMass_1[_128], nameMass_2[_128], strIndex[_512]; // ���� '������ � �������'
 char  err_01[]="\n-M- ��� ������� '%s' �����������! -M-\n";
 char* stdFunc[] = {"sin","cos","tg","ctg","arcsin","arccos","arctg","arcctg", // �����.��������.������� �������/����������� (strstr)
                    "sh", "ch", "th","cth","exp",   "lg",    "ln",   "sqrt"};
 char replaceFmt[]="{$%d@}"; // ������ ��� ������/�������������� �������� ��� stdFunc
 bool flagMacroTitle_1D, flagMacroTitle_2D, TokenUse = false; // ������� ���������� ������ � ���������� ������� 1D/2D-��������������
 TStringList *mBody    = new TStringList(), // ������� ����� ����� - ��������� ��� �������
             *mExpand  = new TStringList(), // ������� ����� ����� - ��������� ��� ���������� ������� �������
             *strToken = new TStringList(); // ������ ��� ������������ ����������
// ----- ����� ������ ��������� �������������� ---------------------------------
////////////////////////////////////////////////////////////////////////////////
#include "Finalize_XXX.cpp" // Finalize_Only_SET, Finalize_Except_SET
#include "Macros.cpp" // ���������� �������� � �������������
#include "FTP_GetPost_DF.cpp" // ������ � �����
//==============================================================================
//
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
__fastcall TF1::TF1(TComponent* Owner) : TForm(Owner)
{ // ����������� ��� �������� ������� ����� (�����������)
//
 F1->Position = poDefault; //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//
 ::time_t t;
 srand((unsigned) time(&t));
//
 Master_Timer->Enabled = false; // ��������� ������� ������
//
 strcpy(FileNameINI, ChangeFileExt(ParamStr(0), ".ini").c_str()); // ���� � INI-����� ��� ��,��� � EXE
//
 Read_Config( 0 ); // ������ ���� ������������ (� ���������������� ������ ������������ ��������)
//
////////////////////////////////////////////////////////////////////////////////
//
 Mem_Instruction = ( mi* ) malloc( max_Instruction      * sizeof( mi ) ); // �������������� ������ ������ ��� ������������ �������
 Sel_Cell        = ( sc* ) malloc( 10 * max_Instruction * sizeof( sc ) );
 Mem_Proc        = ( mp* ) malloc( max_Proc             * sizeof( mp ) );
 Mem_Data        = ( md* ) malloc( max_Data             * sizeof( md ) );
 Mem_Buffer      = ( mb* ) malloc( max_Buffer           * sizeof( mb ) );
//
////////////////////////////////////////////////////////////////////////////////
 Tpr = new TStringList(); // ������� ����� ����� ��� ������� ������������� ���
////////////////////////////////////////////////////////////////////////////////
//
 mB->Cells[0][0] = " #/���������"; // ����� ��������� (������)
 mB->ColWidths[0]= 100;
//
 mB->Cells[1][0] = " �����./�����.";
 mB->ColWidths[1]= 115;
//
////////////////////////////////////////////////////////////////////////////////
//
 mS->Cells[0][0]  = " # �/�"; // ������� ������ (����������)
 mS->ColWidths[0] = 55;
//
 mS->Cells[1][0]  = " �����";
 mS->ColWidths[1] = 55;
//
 mS->Cells[2][0]  = " �������-1";
 mS->ColWidths[2] = 115;
//
 mS->Cells[3][0]  = " �������-2";
 mS->ColWidths[3] = 115;
//
 mS->Cells[5][0]  = " ��������"; ///////////////////////////////////////////////
 mS->ColWidths[4] = 115;
//
 mS->Cells[4][0]  = " ���������";
 mS->ColWidths[5] = 115;
//
 mS->Cells[6][0]  = " �����";
 mS->ColWidths[6] = 90;
//
 mS->Cells[7][0]  = " �����������";
//
 mS->ColWidths[7] = sizeof(M_I.Comment); // ����� ���� �����������
//
////////////////////////////////////////////////////////////////////////////////
//
 mD->Cells[0][0] = " ����������";  // ������� ������
 mD->ColWidths[0]= 95;
//
 mD->Cells[1][0] = " ��������";
 mD->ColWidths[1]= 100;
//
////////////////////////////////////////////////////////////////////////////////
//
 DecimalSeparator = '.'; // ����������� ����� � ���������� ������ - �����
//
 if( Read_Instructions() ) // ������ ���������� �� ����� FileNameSet � ������ �������� Mem_Instruction[]
 {
  SBM0->Text = " ������� ������ � ������� ������ ���������� / F9 ..."; // ����� ������ � StatusBarMain
//
  Install_All_Flags(); // �������� ��� ����� ����������
//
  Vizu_Instructions(); // ������������ ���������� � SG_Set �� ������� �������� Mem_Instruction[]
 }
 else // �� ������� ��������� ���� ����������..!
 {
  BitBtn_Run->Enabled = false; // "���������" ������ ����������
  SBM0->Text = " �������� ���� ��������� ��� ����������..."; // ����� ������ � StatusBarMain
  MessageBeep( MB_ICONEXCLAMATION ); // ��������������...
  Delay( 1000 ); // ����� 1000 ����
 }
//
} // ----- ����� F1 ------------------------------------------------------------


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void t_printf(char *fmt, ...)
{  // �� ����� ������ ������������� ��������� !
 int lenBuff = _256; // ��������� ����� ������ ������
//
 char* buff = (char*) malloc( lenBuff ); // ����� ��������������� ������-������ ��� ���������� ������
 va_list args;  // ���������� �������� ������ � ��������� ( ...  )
 va_start( args, fmt );  // ������ ������ ���������� � ���������
//
 int new_lenBuff = vsnprintf( buff, lenBuff, fmt, args ); // ����������� ����� ������ ��� ������������ ������ ������
//
 if( new_lenBuff > lenBuff ) // ���������� ��������� �����..!
 {
  buff = (char*) realloc( buff, new_lenBuff+1 ); // ����������� ����� (� ������ '\0' � �����)...
  vsnprintf( buff, new_lenBuff+1, fmt, args ); // ������ ������� � (�����������) ����� buff
 }
//
 va_end(args);  // ����� ������ ���������� � ���������
//
 AddLineToProtocol( buff ); // ���������� � Nemo_Run ( ����������� "\n" )
//
// free( buff );
} // ---- ����� t_printf -------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void __fastcall // ������� ������� � Tabs ����� � ������ ������ str
DelSpacesTabsAround(char *str)
{  // Trim, TrimLeft, TrimRight �������� ������ � AnsiString
// strcpy(str, Trim(AnsiString(str)).c_str());
//
// ������� ������� � Tabs � ������ ������
 int i=0, j;
//
 while( (str[i] == ' ') || (str[i] == '\t') )
  i++;
//
 if(i>0)
 {
  for(j=0; j<strlen(str); j++)
   str[j]=str[j+i];
  str[j]='\0';
 }
//
// ������� ������� � Tabs � ����� ������
 i=strlen(str)-1;
//
 while( (str[i] == ' ') || (str[i] == '\t') )
  i--;
//
 if(i<(strlen(str)-1))
  str[i+1]='\0';
//
} // --- ����� ������� DelSpacesTabsAround -------------------------------------

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void __fastcall ReplaceEqualLengthSubstring( char *String, char *OldSubstring, char *NewSubstring )
// � ������ Strint �������� ��������� OldSubstring �� NewSubstring ���������� �����
{
 INT lenOldSubstring = strlen( OldSubstring ),
     lenNewSubstring = strlen( NewSubstring ) ;
 char *p ;
//
 if( lenOldSubstring != lenNewSubstring )
  return ;
//
 do // ������, ���� p != NULL
  if( p = strstr( String, OldSubstring ) ) // � ������ String ����� ��������� OldSubstring
   memcpy( p,NewSubstring,lenOldSubstring ); // �������� NewSubstring ������� � p
 while( p ) ;
//
} // --- ����� ������� ReplaceEqualLegthSubstring ------------------------------

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
char* __fastcall DeleteAllSpaces( char *str )
{ // ������� ��� ������� � ������ str
//
// ��� ������������� AnsiReplaceStr ytj,[jlbvj #include <vcl/StrUtils.hpp>
// str = ( AnsiReplaceStr( str, " ", "" ) ).c_str(); // �������� ���� ��������
//
// �������������� ����� � http://www.quizful.net/interview/cpp/VbW07kq70NCY
 for ( register INT i=0,j=0; str[i];
  (str[j++] = str[i]!=' ' ?
  str[i] :
  (j--, str[j]), i++,
  (!str[i] ?
  (str[j]=0) :
  0)) );
//
 return str;
//
} // ------ ����� DeleteAllSpaces ----------------------------------------------


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
char* __fastcall DeleteSymbolAll( char str[], char symb )
{ // ������� ��� ��������� ��������� �������
 register INT i, j;
//
 for ( i=j=0; str[i]!='\0'; i++ )
  if (str[i] != symb )
   str[j++] = str[i];
//
 str[j] = '\0';
//
 return str;
} // ----- ����� DeleteSymbolAll -----------------------------------------------

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void __fastcall
RunExternal(char* CommandLine, byte RuleParent, byte Priority, bool RuleMessage)
// �������� ���������� �������-������� �������� ��������� �����e CommandLine
// ��� RuleParent=0 �������-�������� ���� ��������� ������ ������� (��� ����
// �������� �������� ������ WINDOWS-����������� ����� ������ ProcessMessages)
// ��� RuleParent=1 �������-�������� �� ���� ��������� ������ �������
// ��� ���� ������ ��������� RuleParent �������-��������
// ����� ������� ��������-������� �����������.
//-----------------------------------------------------------------------
// Priority=0/1/2/3 ������������� ����������� ������������ ����������
//           REALTIME/HIGH/NORMAL/IDLE �������������� (��� 16-��� �� ������������)
//          (��� ������ �������� Priority ������������� IDLE)
//-----------------------------------------------------------------------
//  ��� RuleMessage=TRUE �������� ��������� �� ������� }
{
  STARTUPINFO si;
  PROCESS_INFORMATION pi;
  DWORD dwCreationFlag,dwExitCode;
  bool out;
//
// ��� ���� ��������� si (����� ���� cb) ������ ���� ��������
// �.�. ���� ����� �����, ���������� ���� ������� ������ ����...
// ���� �������� ����� ���������� ��������� ������ (������������ FillChar)
  memset( &si, 0, sizeof(STARTUPINFO) );
  si.cb = sizeof(STARTUPINFO);  // �������� ���� cb
//
  switch (Priority)  // ��������� ���������� ��������-�������
  {
   case 0: dwCreationFlag=REALTIME_PRIORITY_CLASS;
           break;
   case 1: dwCreationFlag=HIGH_PRIORITY_CLASS;
           break;
   case 2: dwCreationFlag=NORMAL_PRIORITY_CLASS;
           break;
   default:
           dwCreationFlag=IDLE_PRIORITY_CLASS;
  }
//
  out=CreateProcess(NULL,CommandLine,NULL,NULL,false,
                    dwCreationFlag,NULL,NULL,&si,&pi);
//
  if (!out) // ���� ����� ��������... out=FALSE
  {
   if (RuleMessage)  // ���� ����� ����� ������ ��������� �� �������
    MessageDlg("��������, ����������\n\r\n\r" +
                AnsiUpperCase(CommandLine)  +
               "\n\r\n\r����������...  (������ " +
                IntToStr(GetLastError()) + ")",
                mtError,
                TMsgDlgButtons() << mbYes, 0);
   return;
  } // ����� IF out = FALSE
//
  if (out) // ���� ����� ������... out = TRUE
  {
   if (RuleParent == 0) // ���� �������� ������ ����� ��������� ������ �������
   {
    CloseHandle(pi.hThread); // ����� ������ �� ����� - �������
    if (WaitForSingleObject(pi.hProcess, INFINITE) != WAIT_FAILED) // ����������� ���� ��������
    {
     GetExitCodeProcess(pi.hProcess, &dwExitCode); // ���� ������ ������� ��������...
     if (RuleMessage) // ���� ����� ����� ������ ��������� �� �������
      if (dwExitCode != WAIT_OBJECT_0) // WAIT_OBJECT_0 = ������������ ���������� ��������
       MessageDlg("��������, �������\n\r\n\r" +
                   AnsiUpperCase(CommandLine) +
                  "\n\r\n\r�������� � ������� " +
                   IntToStr(dwExitCode) + "\n\r",
                   mtError,
                   TMsgDlgButtons() << mbOK, 0);
     CloseHandle(pi.hProcess); // ����������� ����� ��������
    } // ����� IF WaitForSingleObject...
   } // ����� IF RuleParent = 0
//
   if (RuleParent == 1) // �������� �� ������ ����� ��������� ������ �������
    return; // ����� - ������ �� �����
//
   if ((RuleParent != 0) && // �������� �����������
       (RuleParent != 1))
    Application->Terminate(); // ��������� ������������ �������
//
 } // ����� IF out=TRUE
//
} // ����� RunExternal ---------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void __fastcall
TF1::SG_Instruction_Edit(TObject *Sender) // ����� ��������� ����� ����������
{
//
 if( !modeEdit ) // ����������� �����
  ShellExecute(0, NULL, FileNameEditor, FileNameSet, NULL, SW_SHOWNORMAL);
//
 else // ��������� �����
 {
  char tmp[_256];
  snprintf( tmp,sizeof(tmp), "%s %s", FileNameEditor, FileNameSet );
  RunExternal(tmp, 0, 2, true); // ���������� � ��������� ���������
  Rewrite_Files( Sender ); // ���������� ����
 }
//
 Read_Config( 1 ); // ���������� ����� ��� ��������� ��������� � �������� F1
 MessageBeep( MB_OK ); // �������� ��������������...
//
} // ����� SG_Sets_Edit --------------------------------------------------------


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void __fastcall TF1::INI_Edit(TObject *Sender)
{ // ����� ��������� ����� ��������
//
 if( !modeEdit ) // ����������� �����
  ShellExecute(0, NULL, FileNameEditor, FileNameINI, NULL, SW_SHOWNORMAL);
//
 else // ��������� �����
 {
  char tmp[_256];
  snprintf(tmp,sizeof(tmp), "%s %s", FileNameEditor, FileNameINI);
  RunExternal(tmp, 0, 2, true); // ���������� � ��������� ���������
  Rewrite_Files( Sender ); // ���������� ����
 }
//
 Read_Config( 1 ); // ���������� ����� ��� ��������� ��������� � �������� F1
 MessageBeep( MB_OK ); // �������� ��������������...
//
} // ����� INI_Edit ------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void __fastcall // ����� ������
TF1::On_Close_Main_Form(TObject *Sender)
{
 F1->Close();
} //----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void __fastcall // ������� �� ��������� ��������� ���.�����. ���� SG_Data
TF1::SG_Data_TopLeftChanged(TObject *Sender)
{
 SG_Data->LeftCol = 0;
} //----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void __fastcall  // ������� �� ��������� ��������� ���.�����. ���� SG_Buffer
TF1::SG_Buffer_TopLeftChanged(TObject *Sender)
{
 SG_Buffer->LeftCol = 0;
} //----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void __fastcall // ������� ������ ���� �� SG (����� "������", ��� TStringGrid)
TF1::SG_Data_Info(TObject *Sender)
{
 if( SG_Data->Cells[SG_Data->Col][SG_Data->Row] == "" ) // ������ ������ !..
  return;
//
 char str[_128] = "\0";
//
 if( is_ResultIsPredicat( Mem_Data[SG_Data->Row-1].Addr ) ) // ��� ��������� ���������� ����������-���������...
  snprintf( str, sizeof(str), "     %s = %s",    Mem_Data[SG_Data->Row-1].Addr, Mem_Data[SG_Data->Row-1].Data ? trueOutput : falseOutput );
 else
  snprintf( str, sizeof(str), "     %s = %.10g", Mem_Data[SG_Data->Row-1].Addr, Mem_Data[SG_Data->Row-1].Data );
//
 MessageBox(0, str," �������� ���������� � ������ ������", MB_OK | MB_ICONINFORMATION | MB_TOPMOST | MB_TASKMODAL ); // ��������� ���� ..!
//
} //----------------------------------------------------------------------------

void __fastcall TF1::About_Common(TObject *Sender)
{ // ����� ShellExecute �� ������ Info_CommonStr[ Info_Common->Tag ]
 ShellExecute(0, NULL, Info_CommonStr[ ((TMenuItem*)Sender)->Tag ], NULL, NULL,SW_SHOWNORMAL);
} //----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void __fastcall // ���������� ��� �����
TF1::Rewrite_Files(TObject *Sender)
{
 Read_Instructions(); // ������ FileNameSet � Mem_Instruction[]
//
 mR->Clear(); //  // �������� ����� ������ ��������� �������
//
 Install_All_Flags(); // �������� ��� ����� � Mem_Instruction[]
//
 Vizu_Instructions(); // ��������������� ���������� � ����_����������
//
 Really_Data = 0; // �������� ��� ������
 Vizu_Data();  // ��������������� ������ � ����_������
//
 Really_Buffer = 0; // �������� �����
 Vizu_Buffer();  // ��������������� ������ � ������
//
 Clear_AllTableInstructions(); // ������� ����� ���� ����� ������� ����������
} //----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void __fastcall TF1::Show_Chema(TObject *Sender)
{ // �������� ����� DATA-FLOW ����������� � PDF
 ShellExecute(0, NULL, "Chema.pdf", NULL, NULL,SW_SHOWNORMAL);
} //----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void __fastcall TF1::N12Click(TObject *Sender)
{  // �������� �������� DATA-FLOW ����������� � PDF
 ShellExecute(0, NULL, "Base.pdf", NULL, NULL,SW_SHOWNORMAL);
} //----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void __fastcall // ����� mSecs ����������� (� ������������ ���������� WINDOWS)
Delay(long mSecs)
{ // ��� mSecs<0 ����� �������� ���������� ��� � ��������
 unsigned long t,
          FirstTick = ::GetTickCount(); // ��������� ������ �������
 t=(mSecs>=0 ? mSecs : -1e3*mSecs);
 while( ::GetTickCount()-FirstTick < t ) // ��������, ���� �������� �������� ������� �� ����...
  Application->ProcessMessages(); // ���� ���������� WINDOWS
}
// --- ����� Delay -------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void __fastcall Read_Config( int Rule )
{// ��������������� ������ �� ����� ������������
// (��� Rule=1 �� ��������������� ��������� � ������ F1)
//
 TIniFile *tINI = new TIniFile(ExpandFileName(FileNameINI)); // ������� ������ ���� TIniIFile
//------------------------------------------------------------------------------
//
 max_Instruction  = tINI->ReadInteger(RWC.Sect1, RWC.Sect1_Var1, MAX_INSTRUCTION); // ����� ����� ���� ������
  max_Instruction = (max_Instruction >  MAX_INSTRUCTION) ? MAX_INSTRUCTION : max_Instruction;
  max_Instruction = (max_Instruction <= 0) ? 1 : max_Instruction;
//
 max_Data  = tINI->ReadInteger(RWC.Sect1, RWC.Sect1_Var2, MAX_DATA); // ����� ����� ���� ������
  max_Data = (max_Data >  MAX_DATA) ? MAX_DATA : max_Data;
  max_Data = (max_Data <= 0) ? 1 : max_Data;
//
 max_Proc  = tINI->ReadInteger(RWC.Sect1, RWC.Sect1_Var3, MAX_PROC); // ����� ��������������� ���
  max_Proc = (max_Proc >  MAX_PROC) ? MAX_PROC : max_Proc;
  max_Proc = (max_Proc <= 0) ? 1 : max_Proc;
//
 F1->E_AIU->Text = max_Proc; // ������ � E_AIU =================================
//
 max_Buffer  = tINI->ReadInteger(RWC.Sect1, RWC.Sect1_Var4, MAX_BUFFER); // ����� ����� ������ ����������
  max_Buffer = (max_Buffer >  MAX_BUFFER) ? MAX_BUFFER : max_Buffer;
  max_Buffer = (max_Buffer <= 0) ? 1 : max_Buffer;
//
////////////////////////////////////////////////////////////////////////////////
//
 Mem_Instruction = ( mi* ) realloc( Mem_Instruction, max_Instruction * sizeof( mi ) ); // �������������� ������ ��� ������������ �������
 Sel_Cell        = ( sc* ) realloc( Sel_Cell,   10 * max_Instruction * sizeof( sc ) ); // ���� "������" ���������=NULL, �� ������ malloc()
 Mem_Proc        = ( mp* ) realloc( Mem_Proc,   max_Proc   * sizeof( mp ) );
 Mem_Data        = ( md* ) realloc( Mem_Data,   max_Data   * sizeof( md ) );
 Mem_Buffer      = ( mb* ) realloc( Mem_Buffer, max_Buffer * sizeof( mb ) );
//
////////////////////////////////////////////////////////////////////////////////
//
 how_Calc_Param = tINI->ReadInteger(RWC.Sect4, RWC.Sect4_Var1, 0); // ��� ��������� ��������
 how_Calc_Prior = tINI->ReadInteger(RWC.Sect4, RWC.Sect4_Var2, 0); // ��� ��������� ���������
//
 Delay_Buffer = abs (tINI->ReadInteger(RWC.Sect5, RWC.Sect5_Var1, 0)); // �������� ������������ ������ (��������)
//
 tick_ScanForGraph = tINI->ReadInteger(RWC.Sect6, RWC.Sect6_Var1, 1); // ������ ������ ������ (������)
//
 tick_Interval = tINI->ReadInteger(RWC.Sect7, RWC.Sect7_Var1, 10); // ������ ���� (�����������)
 F1->Master_Timer->Interval = tick_Interval;
//
 if( Rule != 1 ) // ��� Rule==1 �� ���� �������� ��������� � ������ ����� F1
 {
  F1->Top    = tINI->ReadInteger(RWC.Sect8, RWC.Sect8_Var1,      20); // ��������� � ������� ������� �����
  F1->Left   = tINI->ReadInteger(RWC.Sect8, RWC.Sect8_Var2,      20);
  F1->Width  = tINI->ReadInteger(RWC.Sect8, RWC.Sect8_Var3, minW_F1);
  F1->Height = tINI->ReadInteger(RWC.Sect8, RWC.Sect8_Var4, minH_F1);
 }
//
 strcpy( FileNameSet, tINI->ReadString(RWC.Sect9, RWC.Sect9_Var1, "DataFlow.set").c_str()); // ��� ����� ���������
//
//------------------------------------------------------------------------------
//
 for(int i=0; i<Count_Sets; i++) // �� ������ ����������
  Set_Params[i].Ticks = tINI->ReadInteger(RWC.Sect2, Set_Params[i].Set, Set_Params[i].Ticks);
//
 strcpy(FileNameEditor, tINI->ReadString(RWC.Sect3, RWC.Sect3_Var1, "notepad.exe").c_str()); // ���� ���������� ���������
 modeEdit = tINI->ReadBool(RWC.Sect3, RWC.Sect3_Var2, false); // ����� ������ ���������� ���������
//
//------------------------------------------------------------------------------
 strcpy( attrVar, tINI->ReadString(RWC.Sect10, RWC.Sect10_Var1, "$\0").c_str() );
// ������ ������ ��� ����������, �� ������������� ������� ������������� ������������
//------------------------------------------------------------------------------
 modeGraph = tINI->ReadBool(RWC.Sect11, RWC.Sect11_Var1, true); // ����� ��������� ������� ������������� ����������
//-----------------------------------------------------------------------------
 waitAboveOfEndLastExecuteSet = tINI->ReadInteger(RWC.Sect12, RWC.Sect12_Var1, 1000); // c������ ����� ����� ��������� ����������
//
 DCE.needDrawColors     = tINI->ReadBool(RWC.Sect13,    RWC.Sect13_Var1, true);  // ���� �� �������� ������ ������ ��� ����������
 DCE.clReadyOperand     = tINI->ReadInteger(RWC.Sect13, RWC.Sect13_Var2, RGB(255,153,255)); // ���� ���������� ���������
 DCE.clTruePredicat     = tINI->ReadInteger(RWC.Sect13, RWC.Sect13_Var3, RGB(128,255,  0)); // ���� ��������� TRUE �����-���������
 DCE.clExecSet          = tINI->ReadInteger(RWC.Sect13, RWC.Sect13_Var4, RGB(255, 51,153)); // ���� ����������  ����������
 DCE.clSpeculateExec    = tINI->ReadInteger(RWC.Sect13, RWC.Sect13_Var5, RGB(192,192,192)); // ���� ������ ���������� ������������ ����������� ����������
//
 DCT.clOperandOperation = tINI->ReadInteger(RWC.Sect14, RWC.Sect14_Var1, RGB(255,185,185)); // ����� ��������<->��������
 DCT.clNonExecuted      = tINI->ReadInteger(RWC.Sect14, RWC.Sect14_Var2, RGB(255,185,255)); // ������������� ����������
 DCT.clNonUsedResult    = tINI->ReadInteger(RWC.Sect14, RWC.Sect14_Var3, RGB(255,192,0)); // ���������������� ����������
 DCT.clNonDefOperands   = tINI->ReadInteger(RWC.Sect14, RWC.Sect14_Var4, RGB(0,255,255)); // ������������� ��������
//
 DGR.clGraphStart = tINI->ReadInteger(RWC.Sect15, RWC.Sect15_Var1, RGB(255,0,0)); // ������ ���� ������� ������������� ����������
 DGR.clGraphEnd   = tINI->ReadInteger(RWC.Sect15, RWC.Sect15_Var2, RGB(0,255,0)); // ����� ���� ������� ������������� ����������
//
 StartNumb     = tINI->ReadInteger(RWC.Sect16, RWC.Sect16_Var1, 100); // ��������� ����� ����������
//
 pass_Counts   = tINI->ReadInteger(RWC.Sect17, RWC.Sect17_Var1, 0); // ����� ��������� ��� ����������� ������ � SG_Set[][]
//
 SpeculateExec = tINI->ReadInteger(RWC.Sect18, RWC.Sect18_Var1, 0); // ��� true ���������� � flagPredicate_TRUE=false ���������, �� ��������� �� ���������
//
 TokenUse      = tINI->ReadBool(RWC.Sect19, RWC.Sect19_Var1, 0); // ������������ �� ����������� � ������ ���������� ���������� ����������
//
 delete tINI; // ���������� ������ - ����� �� ����� !...
//
 Out_Data_SBM1(); // ����� ������ � ������� ����� StatusBar --------------------
//
} // --- ����� Read_Config -----------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void __fastcall Write_Config()  // ��������� ������ � ���� ������������
{
//
 TIniFile* tINI = new TIniFile(FileNameINI); // ������� ������ ���� TIniIFile
//
// tINI->WriteInteger(RWC.Sect4, RWC.Sect4_Var1, How_Calc_Param); // ��� ��������� �������� ����������
// tINI->WriteInteger(RWC.Sect4, RWC.Sect4_Var2, How_Calc_Prior); // ��� ��������� ���������
 tINI->WriteInteger(RWC.Sect1, RWC.Sect1_Var3, StrToInt(F1->E_AIU->Text)); // max_Proc - ����� ��������������� ���
//
 tINI->WriteInteger(RWC.Sect8, RWC.Sect8_Var1, F1->Top); // ��������� � ������� ������� �����
 tINI->WriteInteger(RWC.Sect8, RWC.Sect8_Var2, F1->Left);
 tINI->WriteInteger(RWC.Sect8, RWC.Sect8_Var3, F1->Width);
 tINI->WriteInteger(RWC.Sect8, RWC.Sect8_Var4, F1->Height);
//
// --- ���� ���� � Set-�a��� = �������� ��������, ���� �� ����� (������ ��� �����)
 if( !strcmp( ExtractFileDir(FileNameSet).c_str(),ExtractFileDir(ParamStr(0)).c_str() ) )
  strcpy( FileNameSet,ExtractFileName(FileNameSet).c_str() );
//
 tINI->WriteString(RWC.Sect9, RWC.Sect9_Var1, FileNameSet); // ��� ����� �������
//
// tINI->WriteString(RWC.Sect10, RWC.Sect10_Var1, attrVar);
// ������ ������ ��� ����������, �� ������������� ������� ������������� ������������
//
 tINI->WriteBool(RWC.Sect13,    RWC.Sect13_Var1, DCE.needDrawColors); // ���� �� �������� ������ ������ ��� ����������
 tINI->WriteInteger(RWC.Sect13, RWC.Sect13_Var2, DCE.clReadyOperand); // ���� ���������� ���������
 tINI->WriteInteger(RWC.Sect13, RWC.Sect13_Var3, DCE.clTruePredicat); // ���� ��������� TRUE ����� ����������
 tINI->WriteInteger(RWC.Sect13, RWC.Sect13_Var4, DCE.clExecSet); // ���� ����������  ����������
 tINI->WriteInteger(RWC.Sect13, RWC.Sect13_Var5, DCE.clSpeculateExec); // ���� ������ ���������� ������������ ����������� ��������
//
 tINI->WriteInteger(RWC.Sect14, RWC.Sect14_Var1, DCT.clOperandOperation); // ����� ��������->��������
 tINI->WriteInteger(RWC.Sect14, RWC.Sect14_Var2, DCT.clNonExecuted); // ������������� ����������
 tINI->WriteInteger(RWC.Sect14, RWC.Sect14_Var3, DCT.clNonUsedResult); // ���������������� ����������
 tINI->WriteInteger(RWC.Sect14, RWC.Sect14_Var4, DCT.clNonDefOperands); // ������������� ��������
//
 tINI->WriteInteger(RWC.Sect15, RWC.Sect15_Var1, DGR.clGraphStart); // ������ ���� ������� ������������� ����������
 tINI->WriteInteger(RWC.Sect15, RWC.Sect15_Var2, DGR.clGraphEnd); // ����� ���� ������� ������������� ����������
//
 delete tINI; // ���������� ������ - ����� �� ����� !...
//
 MessageBeep( MB_OK ); // �������� ��������������...
//
} // --- ����� Write_Config ----------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
bool __fastcall // ������������ ���������� � SG_Set �� ������� �������� Mem_Instruction[]
Vizu_Instructions()
{
 char Set[_SET_LEN], // ��������� ����������
      tmp[_512];
//
 if( !Really_Set )
 {
  mS->RowCount = Really_Set + 2; // ��������� ����� ����� � ��������������� �������
  for(int j=0; j<9; j++) // ������ ������ � ������� 1 (������ ������)
   mS->Cells[j][1] = "\0";
  return true;
 }
//
 mS->RowCount = Really_Set + 1; // ��������� ����� ����� � ��������������� �������
//
 for(UI i=0; i<Really_Set; i++) // ������ ������������� ���������� (����� � SG_Set)...
  {
   for(int j=0; j<7; j++) // �� ���� �������� ������ i
    mS->Cells[j][i+1] = "\0"; // �������� !
//
////////////////////////////////////////////////////////////////////////////////
   mS->Cells[6][i+1] = Vizu_Flags(i); // ��������������� ����� ������ ����������

// ������� ������� /////////////////////////////////////////////////////////////
   snprintf(tmp,sizeof(tmp), "%7d", i); // ����� '�������' � ������� ���� (���������)
   mS->Cells[0][i+1] = tmp; // ����� ����������
//
// ������ ������� - ��������� ���������� ///////////////////////////////////////
   strcpy(Set, Mem_Instruction[i].Set); // � Set - ��������� ���������� (��� ����� ��������...)
//
   snprintf(tmp,sizeof(tmp), "  %s", Set); // ��� ������� ����� ����������
   mS->Cells[1][i+1] = tmp; // ��������� ����������
//
////////////////////////////////////////////////////////////////////////////////
   switch( Get_CountOperandsByInstruction(Set) ) // ... ����� ������� ��������� ���������� Set
   {
    case 1: mS->Cells[2][i+1] = Mem_Instruction[i].aOp1; // ����� ������� (� �������������) ��������
            mS->Cells[3][i+1] = "\0";
            mS->Cells[5][i+1] = Mem_Instruction[i].aPredicat; // ����� ���������
            mS->Cells[4][i+1] = Mem_Instruction[i].aResult; // ����� ����������
            mS->Cells[7][i+1] = Mem_Instruction[i].Comment; // �����������
//
            break;
//
    case 2: mS->Cells[2][i+1] = Mem_Instruction[i].aOp1; // ����� ������� ��������
            mS->Cells[3][i+1] = Mem_Instruction[i].aOp2; // ����� ������� ��������
            mS->Cells[5][i+1] = Mem_Instruction[i].aPredicat; // ����� ���������
            mS->Cells[4][i+1] = Mem_Instruction[i].aResult; // ����� ����������
            mS->Cells[7][i+1] = Mem_Instruction[i].Comment; // �����������
//
            break;
//
   default: break;
//
   } // ����� switch
//
   if( is_PredicatOrSET( Set ) ) // ��� SET ��� ��������� (���� ����� ���������� ������)
    mS->Cells[5][i+1] = "\0"; // ... ���� ��������� - ������..!
//
  } // ����� ����� �� i=0, Really_Set
//
  return true;
//
} // --- ����� Vizu_Instructions -----------------------------------------------

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void __fastcall // ������������ ���������� � SG_Data �� ������� �������� Mem_Data[]
Vizu_Data()
{
 char tmp[_512];
//
// mD->Cells->BeginUpdate(); // � TStringGrid ���� BeginUpdate � EndUpdate
 mD->DoubleBuffered = true; // ������� ������������ - ����� �� ������..!
//
 if(!Really_Data)
 {
  mD->RowCount = Really_Data + 2; // ��������� ����� ����� � ��������������� �������
  for(int j=0; j<2; j++) // ������ ������ � ������� 1 (������ ������)
   mD->Cells[j][1] = " ";
  return;
 }
//
 mD->RowCount = Really_Data + 1; // ��������� ����� ����� � ��������������� �������
//
 for( INT i=0; i<Really_Data; i++ ) // ������ ������������� ���������� (����� � SG_Data)...
  {
   mD->Cells[0][i+1] = Mem_Data[i].Addr; // �������� ������
   snprintf(tmp,sizeof(tmp), "%.*g", ACC_REAL, Mem_Data[i].Data); // �������� ����� �� ������� ������
   mD->Cells[1][i+1] = tmp;
// ����� �������� ���������� ���������� ========================================
//
 if( is_ResultIsPredicat( Mem_Data[i].Addr ) ) // ��� ��� �����-��������� ?
  mD->Cells[1][i+1] = Mem_Data[i].Data ? trueOutput : falseOutput;
//
 } // ����� if( INT i=0; ...
//
} // --- ����� Vizu_Data -------------------------------------------------------


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void __fastcall
Vizu_Buffer() // ������������ ������ � SG_Buffer �� ������� �������� Mem_Buffer[]
{
 char tmp[_512];
// mB->Cells->BeginUpdate(); // � TStringGrid ���� BeginUpdate � EndUpdate
 mB->DoubleBuffered = true; // ������� ������������ - ����� �� ������..!
//
 if(!Really_Buffer)
 {
  mB->RowCount = Really_Buffer + 2; // ��������� ����� ����� � ��������������� �������
  for( INT j=0; j<2; j++ ) // ������ ������ � ������� 1 (������ ������)
   mB->Cells[j][1] = " ";
  return;
 }
//
 mB->RowCount = Really_Buffer + 1; // ��������� ����� ����� � ��������������� �������
//
 for( INT i=0; i<Really_Buffer; i++ ) // ������ ������������� ���������� (����� � SG_Buffer)...
  {
   snprintf(tmp,sizeof(tmp), "%7d / %s",  // ����� ���������� � �� ��������� ��� ������
                             Mem_Buffer[i].i_Set, Mem_Instruction[Mem_Buffer[i].i_Set].Set);
   mB->Cells[0][i+1] = tmp;
//
   snprintf(tmp,sizeof(tmp), "%.*g / %.*g", // ������� ������ ���� "Param / Priority"
                             ACC_REAL,Mem_Buffer[i].Param,
                             ACC_REAL,Mem_Buffer[i].Priority);
   mB->Cells[1][i+1] = tmp;
  }
//
} // ����� Vizu_Buffer ---------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void __fastcall // ������ ���� ������������
TF1::Main_Form_OnCreate(TObject *Sender)
{
// ReadConfig();
} //----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void __fastcall // ������� �������� ����������
TF1::OnClose_F1(TObject *Sender, TCloseAction &Action)
{
 Write_Config(); // ���������� ����� ������������
//
 if(ParamCount() == 4) // ����� ������ ����� ���������� ��������� ������ (�� ������ ��������)
 {
  Action=caFree; // ��������� ���������� ��� ��������
  return;
 }
//
 switch(MessageBox(0, "�� � ����� ���� ������ ��������� ������ ?",
                       "��������������",
                        MB_YESNO | MB_ICONWARNING | MB_TOPMOST))
 {case IDYES: Action=caFree; // ������ ������ Yes
              Regim = 0; // ����� ������ ���������
//
              if( F2 )
               F2->Close(); // ���� ������� ������������� ���������� �������
//
              if( F3 )
               F3->Close(); // ���� ������� ������������� ���������� �������
//
              Delay(100); // ����-c
//
              Work_LogInOut( 1 ); // �������� � ����� ������ ��������� DATA_FLOW.EXE
              break;
//
  case IDNO:  Action=caNone; // ������ ������ No
              break;
 }
} // ---------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void __fastcall // ������� ������ ����
TF1::Stop_Calculations(TObject *Sender)
{
 Write_Config();
//
 StopCalculations( 0 ); // ���������� ����������� �������������
//
 do_Run // "��������" ��� ������ ����������
//
} // ----- ����� Stop_Calculations ---------------------------------------------

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void __fastcall
StopCalculations( int Rule )
{ // ��� Rule==0 "���������� ����������� �������������",
  // ��� Rule==1 "��������� ��������� �� ���������� ���� ���-����������"
  // ��� Rule==2 "���������� ��������� �����������"
 char tmp[_512];
 INT countSets = 0;
//
 F1->Master_Timer->Enabled = false; // ��� ��� �� ������ ������..!
//
 Regim = 0; // ������� ����� ������������� (������)
//
 switch( Rule ) // �������� Rule ��. ����
 {
  case 0:
    strcpy(tmp, "\n-I- ���������� ��������� ����������� �� ���������� ������������ -I-");
    SBM0->Text = tmp; // ����� ������ � StatusBarMain
    AddLineToProtocol( tmp );
    break;
//
  case 1:
    for( INT i=0; i<Really_Set; i++ ) // �� ���� ����������� ������ ���������
     if( strcmp( Mem_Instruction[i].Set,"SET" ) ) // ���� ��� �� SET...
      countSets++;
//
    snprintf(tmp,sizeof(tmp), "\n-W- ��������� �����������: � ������� [%d] ������ �� �������� �� ����� ���-���������� (���������/����� ����������: %d/%d �������� SET) -W-",
             waitAboveOfEndLastExecuteSet, mTpr->Count, countSets );
    SBM0->Text = tmp; // ����� ������ � StatusBarMain
    AddLineToProtocol( tmp );
//
    if( !mTpr->Count ) // �� ����������� ������ �����, ��������, SET
    {
     do_Run // "��������" ��� ������ ������ ����������
     return;
    }
//
    break;
//
  case 2:
    snprintf(tmp,sizeof(tmp), "-I- ���������� ��������� �����������... -I-");
    SBM0->Text = tmp; // ����� ������ � StatusBarMain
    AddLineToProtocol( tmp ); // ����� ������ � ������� ��������� ����������
    break;
//
  default:
    break;
 } // ����� switch
//
 MessageBeep( MB_OK ); // �������� ��������������...
//
 if( Rule != 0 ) // ��� �������� �� ������ �� ���� ��������� ���������� ���������� ���������
  Calc_Stat_Proc(); // ���������� ���������� ������������� ��� -----------------
//
 Regim = 2; // ����� ������������� ����� ���������� ��������� - ���� �� ������ Show_Graph()
//
 if( modeGraph & Rule != 0 ) // ���� FALSE, �� ������ �� F6
  F1->Show_Graph( NULL ); // �������� ������ ������������� ����������
//
} // ----- ����� Stop_Calculations ---------------------------------------------


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
REAL __fastcall // ���������� �� Mem_Data[] ����� �� ������ (�����) "Addr"
get_Data( char Addr[], bool flagCountRead ) // ��� flagCountRead ������ Mem_Data[i].CountRead �� ����������������
{ // ���� ������ 'Addr / D�ta' �� ���������� - �������� �������������� � ��������� = 1.0e0
 char tmp[_512];
//
 for( INT i=0; i<Really_Data; i++ ) // ���� ���� �������� ����������������� �������� ������� Mem_Data[]
  if( !strcmp(Mem_Data[i].Addr,Addr) ) // ...���� �����..!
   {
    if( flagCountRead ) // �� �������� Mem_Data[i].CountRead
     Mem_Data[i].CountRead ++ ; // ����������� ������� ����� ���������� �� 1 ...
    return Mem_Data[i].Data;
   } // ����� if( !strcmp(Mem_Data[i].Addr,Addr) )
//
// �� �����... ����������� �� ���� !!! /////////////////////////////////////////
 t_printf( "\n-E- %s() �� ����� � Mem_Data[] �������� �� ������ %s. ������� %.9f -E-\n",
           __FUNC__, Addr, DEFAULT_RETURN_GET_DATA );
//
 MessageBeep( MB_ICONERROR ); // ���������� �������� � �������!..
//
 return DEFAULT_RETURN_GET_DATA ;
} // ---------------------------------------------------------------------------


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
char* __fastcall // ���������� ����� ���������� �� Mem_Instruction[i]
Line_Set( INT i_Set, short int Rule, REAL Result )
{ // ��� Rule = 0  ���������� aResult �� �������� (�������� "?")
  // ��� Rule = 1  ���������� aResult ������������ get_Data()
  // ��� Rule = -1 �������� Result ������ �� ���������� ���������� (������������ ���
  // ���������� ������������� ����������, ����� ��������� � Mem_Data[] �� ���������������)
  // ��� Rule = 2 ������������ ������ ������ ����������
 char Set[_SET_LEN],
      tmp[_512], tmp1[_512];
//
 strcpy(Set, Mem_Instruction[i_Set].Set); // ��������� ����������... ����� ���� ����� �������� !
//
 switch( Rule ) // ����� �� �������� Rule
 {
////////////////////////////////////////////////////////////////////////////////
  case 0: strcpy(tmp1, "?");
//
          if( is_SET( Set )) // ��� SET ........................................
           snprintf(tmp,sizeof(tmp), "%s{%.*g}, %s{%s} %s",
                    Mem_Instruction[i_Set].Set,
                    ACC_REAL, atof(Mem_Instruction[i_Set].aOp1),
                    Mem_Instruction[i_Set].aResult, tmp1,
                    Mem_Instruction[i_Set].Comment);
          break; // break case 0;
////////////////////////////////////////////////////////////////////////////////
  case -1:
  case 1: snprintf(tmp1,sizeof(tmp1), "%.*g", ACC_REAL,
                   Rule==1 ? get_Data( Mem_Instruction[i_Set].aResult, false ) : Result ); // ���������� �� ������ (������!) aResult
//
          if( is_SET( Set ) ) // ��� SET .......................................
           snprintf(tmp,sizeof(tmp), "%s{%.*g}, %s{%s} %s",
                    Mem_Instruction[i_Set].Set,
                    ACC_REAL, atof(Mem_Instruction[i_Set].aOp1),
                    Mem_Instruction[i_Set].aResult, tmp1,
                    Mem_Instruction[i_Set].Comment);
          else
//==============================================================================
          switch( Get_CountOperandsByInstruction(Set) )
           { // ... �� ����� ������� ��������� ���������� Set
            case 1: snprintf(tmp,sizeof(tmp), "%s %s{%.*g}, %s{%s} ; %s",
                             Mem_Instruction[i_Set].Set,
                             Mem_Instruction[i_Set].aOp1,
                             ACC_REAL, get_Data( Mem_Instruction[i_Set].aOp1, false ),
                             Mem_Instruction[i_Set].aResult, tmp1,
                             Mem_Instruction[i_Set].Comment);
                    break;
            case 2: snprintf(tmp,sizeof(tmp), "%s %s{%.*g}, %s{%.*g}, %s{%s} ; %s",
                             Mem_Instruction[i_Set].Set,
                             Mem_Instruction[i_Set].aOp1,
                             ACC_REAL, get_Data( Mem_Instruction[i_Set].aOp1, false ),
                             Mem_Instruction[i_Set].aOp2,
                             ACC_REAL, get_Data( Mem_Instruction[i_Set].aOp2, false ),
                             Mem_Instruction[i_Set].aResult, tmp1,
                             Mem_Instruction[i_Set].Comment);
                    break;
           default: break;
//
           } // ����� switch( Get_CountOperandsByInstruction(Set) )
//==============================================================================
//
          break; // break case 1;
//
////////////////////////////////////////////////////////////////////////////////
  case 2: if( is_SET( Set ) ) // ��� SET .......................................
           snprintf(tmp,sizeof(tmp), "%s{%.*g}, %s %s",
                    Mem_Instruction[i_Set].Set,
                    ACC_REAL, atof(Mem_Instruction[i_Set].aOp1),
                    Mem_Instruction[i_Set].aResult,
                    Mem_Instruction[i_Set].Comment);
          else
//==============================================================================
          switch( Get_CountOperandsByInstruction(Set) )
           { // ... �� ����� ������� ��������� ���������� Set
            case 1: snprintf(tmp,sizeof(tmp), "%s %s, %s ; %s",
                             Mem_Instruction[i_Set].Set,
                             Mem_Instruction[i_Set].aOp1,
                             Mem_Instruction[i_Set].aResult,
                             Mem_Instruction[i_Set].Comment);
                    break;
            case 2: snprintf(tmp,sizeof(tmp), "%s %s, %s, %s ; %s",
                             Mem_Instruction[i_Set].Set,
                             Mem_Instruction[i_Set].aOp1,
                             Mem_Instruction[i_Set].aOp2,
                             Mem_Instruction[i_Set].aResult,
                             Mem_Instruction[i_Set].Comment);
                    break;
           default: break;
//
           } // ����� switch(Get_CountOperandsByInstruction(Set))
//==============================================================================
//
          break; // break case -1;
//
  default: break;
//
 } // ����� switch( Rule )
//
////////////////////////////////////////////////////////////////////////////////
//
 DSTA( tmp ); // �������� Tabs � ������� ������� � �����
//
 return tmp;
//
} // ------ ����� Line_Set -----------------------------------------------------


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void __fastcall // ��������� � Mem_Data[] ����� Data �� ������ (�����) Addr
add_Data( INT i_Set, char* aResult, REAL Data )
{// i_Set - ����� ����������, � ���������� ������ ���� ��������� ��� �������e
//  aResult - ��� ������ (����������), Data - �������� ����������
//  ���� ������ 'Addr / ***' ���������� - �������������� ���� Data, ��������
//  �������������� � ��� ������ ���������������; ���� �� ���������� - �������
//  ������ Data � �������������
 char tmp[_1024], tmp1[_1024],
      fmt[]="������ (%d)", // ��� ������ �� �����...
      fmt1[] = "\n-W- add_Data(): � ���������� #%d [%s %s, %s] ��������� [%s] ��� ���������� � ������ ������ �������� ������� ������������� ������������ -W-\n", // ������ ������� (1 �������)
      fmt2[] = "\n-W- add_Data(): � ���������� #%d [%s %s, %s, %s] ��������� [%s] ��� ���������� � ������ ������ �������� ������� ������������� ������������ -W-\n"; // ������ ������� (2 ��������)
//
// �� ������� �� ������� ������������� ������������ (�.�. ��� �� ������ � ��������������� "Addr" ?)
//
 for( INT i=0; i<Really_Data; i++ ) // ...���� ���� ������ ����������������� ��������, �� ���������� �� �����..!
//
  if( !strcmp( Mem_Data[i].Addr, aResult ) ) // ������ � ������� (������) aResult � ������� Mem_Data[].Attr ��� ���� !!!
  {
   if( !memcmp( aResult, attrVar, strlen(attrVar) ) ) // ���� ������ ������� aResult ���� attrVar - ����� ������������ ������...
   {
    F1->Label_Data->Caption = Format(fmt, OPENARRAY(TVarRec, (int(i+1)) )); // ����� �� �����...
    F1->Label_Data->Repaint(); // ������������
//
    Mem_Data[i].Data = Data; // ���������� ������ �� ��������� ������...
    Mem_Data[i].CountRead = 0; // -.-.-.-.-
    return;
   } // ����� if( !memcmp( Addr,attrVar,strlen(attrVar) )
//
   if( Data == Mem_Data[i].Data ) // ���� ������������ �������� ����� ��� �������������...
   {
    t_printf( "-\n-W- %s(): ���������� �������� ������� �������� �������� ������ %s ��� �� ��������� %.*g (%s)... -W-\n-",
              __FUNC__, Mem_Data[i].Addr, ACC_REAL,Data, Get_Time_asLine());
    return;
   }  // ����� if( Data == Mem_Data[i].Data )
//
   F1->Master_Timer->Enabled = false; // ��������� ������
//
   int nInputOperands = Get_CountOperandsByInstruction( Mem_Instruction[i].Set ); // �������� ����� ��������� ����������
//
   switch( nInputOperands ) // ����� ������� ��������� ���������� Set
   {
    case 1: t_printf( fmt1, i_Set, Mem_Instruction[i_Set].Set,Mem_Instruction[i_Set].aOp1,
                                   Mem_Instruction[i_Set].aResult, Mem_Instruction[i_Set].aResult );
            break;
//
    case 2: t_printf( fmt2, i_Set, Mem_Instruction[i_Set].Set,Mem_Instruction[i_Set].aOp1,Mem_Instruction[i_Set].aOp2,
                                   Mem_Instruction[i_Set].aResult, Mem_Instruction[i_Set].aResult );
            break;
//
    default: break;
   }
//
strcpy(tmp, "��������� {%.*g} ���������� ���������� #%d ������ ���� ������� �� ������ %s, ������ ��� ��� ��������� ������ {%.*g}. \
������ ����� ��������� �������� ������������ ������������, ���������� ���� �������� ��������� ���������� ��������� ����� ���� ������������� ! \
\n\n���������� ������ / �������� ������� �������� / ��������� ���������� ��������� ?\n");
   snprintf(tmp1,sizeof(tmp1), tmp, ACC_REAL, Data, i_Set, aResult, ACC_REAL, Mem_Data[i].Data);
//
   int out = MessageDlg(tmp1, mtWarning, TMsgDlgButtons() << mbOK << mbCancel << mbAbort, 0); // ������ ���� ������ ���������� ��������
//
   switch (out) // �������� ������� �� ���� �������� �� MessageDlg()...
   {
    case mrOk: // ���������� ������ �� ����� ������
               snprintf(tmp,sizeof(tmp), "%.*g", ACC_REAL, Data);
               mD->Cells[1][i+1] = tmp; // ������ � ����_������
               t_printf( "-W- %s(): ��������� �������� ������������ ������������: ��������� {%.*g} ���������� ���������� #%d ������ ���� ������� �� ������ %s, �� ��� ��� ��������� ������ {%.*g}. ������ ������������... -W-",
                          __FUNC__, ACC_REAL, Data, i_Set, aResult, ACC_REAL, Mem_Data[i].Data );
//
               F1->Label_Data->Caption = Format(fmt, OPENARRAY(TVarRec, (int(i+1)) )); // ����� � ����...
               F1->Label_Data->Repaint(); // ������������
//
               Mem_Data[i].Data = Data; // ���������� ������ �� ��������� ������...
               Mem_Data[i].CountRead = 0; // -.-.-.-.-
               F1->Master_Timer->Enabled = true; // ����� �������� ������... � ���� ���� ��� ����� !..
               return;
//
    case mrCancel: // ���������� �� �������� � ��������� ���������� ��������
               t_printf( "-W- %s(): ��������� �������� ������������ ������������: ��������� {%.*g} ���������� ���������� #%d ������ ���� ������� �� ������ %s, �� ��� ��� ��������� ������ {%.*g}. ������ �� ������������... -W-",
                          __FUNC__, ACC_REAL, Data, i_Set, aResult, ACC_REAL, Mem_Data[i].Data );
               F1->Master_Timer->Enabled = true; // ����� �������� ������... � ���� ���� ��� ����� !..
               return;
//
    case mrAbort: // ��������� ���������� ���������...
               F1->Stop_Calculations(NULL);
               F1->Master_Timer->Enabled = false; // ��������� ������...
               return;
//
   } // ����� ������ �� switch (������ ��������� ��������� ������������� �����������)
//
  } // ����� if(!strcmp(Mem_Data[i].Addr, Addr))
//
// ����� ������ �� ����������... ���!!! ����� ��������� ........................
//
 if( Really_Data > 0.5*max_Data ) // ������������� �� ��������� ������������� �������..!
 {
  snprintf( tmp,sizeof(tmp), "������ (%d%)", int( 1e2 * Really_Data / max_Data) );
  F1->Label_Data->Caption = tmp; // ����� ������� (���) ������
  F1->Label_Data->Repaint();
 }
//
 if( Really_Data >= max_Data-1 ) // ��������� ���������� ������� ���� ������
 {
  if( flagAlarmData ) // ���� �������� ���������...
  {
   t_printf( "-\n-W- %s(): ������ ������ (%d) ���������, �������� ������ ���������� (%s)... -W-\n-",
              __FUNC__, max_Data, Get_Time_asLine());
   ShowMessage( "������ ������ ���������, ��� ���������� ������ �������� ������ ����������..." );
   MessageBeep( MB_ICONASTERISK ); // ��������������...
//
   flagAlarmData = false;
  } // ����� ������� ���� ��������� �� �������...
//
   return;
//
 } // ����� if( Really_Data >= Max_Data )
//
 F1->Label_Data->Caption = Format(fmt, OPENARRAY(TVarRec, (int(Really_Data+1)) )); // ����� � ����...
 F1->Label_Data->Repaint(); // ������������
//
 strcpy( Mem_Data[Really_Data].Addr, aResult ); // �������� � ���_������ ��� ����������
 Mem_Data[Really_Data].Data  = Data; // �������� ����������
 Mem_Data[Really_Data].CountRead = 0; // -.-.-.-.-
 Mem_Data[Really_Data].i_Set = i_Set; // ����� ����������, ������� ��� ��������� ���������
//
// ������� ��������� ...........................................................
 t_printf( "-I- %s(): ������ {%.*g} (��������� ���������� ���������� #%d) �� ������ %s ������� ��������� � ������ ������ (%s) -I-",
           __FUNC__, ACC_REAL, Data, i_Set, aResult, Get_Time_asLine());
//
 mD->RowCount = Really_Data + 2; // ����������� 2 (����� ��� Really_Data=0 ���� 2, � �� 1)
 snprintf(tmp,sizeof(tmp), "%.*g", ACC_REAL, Data);
 mD->Cells[1][Really_Data+1] = tmp; // ������ � ����_������
 mD->Cells[0][Really_Data+1] = aResult;
//
 Really_Data ++ ;
} // ----- ����� add_Data ------------------------------------------------------


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void __fastcall // ������� ��� ����� � Mem_Instructions[]
Install_All_Flags()
{
 for( INT i=0; i<Really_Set; i++ ) // �� ���� ����������� ... �� ��� �������� !
 {
  MI_FOP1(i) = false;
  MI_FOP2(i) = false;
  Mem_Instruction[i].fPredicat      = false;
  Mem_Instruction[i].fPredicat_TRUE = false;
//
  Mem_Instruction[i].fExec          = false;
  Mem_Instruction[i].fExecOut       = false;
  Mem_Instruction[i].fAddBuffer     = false;
//
  Mem_Instruction[i].fSpeculateExec = false;
 }
} // -----  Install_All_Flags --------------------------------------------------


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void __fastcall TF1::Load_Sets(TObject *Sender)
{ // ������� ���� ��������� (���� *.set)
//
 OD_1->Files->Clear(); // ������ �������
//
 if(OD_1->Execute()) // ���� ���-�� �������...
  {
   strcpy( FileNameSet, OD_1->FileName.c_str() ); // ��������� ���� ���������� �������� � FileNameSet
//
   Write_Config(); // ���������� ���� ������������ (� �������� �������� ���� FileNameSet)
//
   Read_Instructions(); // ������ ���� � Mem_Instruction[]
//
   mR->Clear(); // �������� ����� ������ ��������� �������
//
   Install_All_Flags(); // �������� ��� ����� � Mem_Instruction[] !!!!!!!!!!!!!!
//
   Vizu_Instructions(); // ��������������� ���������� � ����_����������
//
   Really_Data = 0; // �������� ��� ������
   Vizu_Data();  // ��������������� ������ � ����_������
//
   Really_Buffer = 0; // �������� ������
   Vizu_Buffer();  // ��������������� �����
//
   Clear_AllTableInstructions(); // ������� ����� ���� ����� ������� ����������
//
   SBM0->Text = " ������� ������ � ������� ������ ����������..."; // ����� ������ � StatusBarMain
   BitBtn_Run->Enabled = true; // �������� ������ ���������
//
//   REAL index = Calc_ConnectedIndex( 1 ); // ������� �������� ��������� c ����������� � ����
  }
} //------����� Load_Sets-------------------------------------------------------


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
char* __fastcall // ������������ ��������� ������ Str �� ������� n1 �� n2 (������� � 1 !!! )
GetSubString( char *Str, int n1, int n2 )
{
 char tmp[_1024] = "\0"; // ������� ������
 int j = -1;
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
void __fastcall  // ����������� ���������� ������������� ���
Calc_Stat_Proc()
{
 if( Regim==2 || // ���� Regim==2 "���������� ��������� �����������"
    !mTpr->Count ) // "���" �� ����������� �� ����� ���������� (�����, ��������, SET)
  return;
//
 char Set[_SET_LEN],
      tmp[_1024], tmp1[_1024], w[_512];;
 INT n_Sets, // ����� ���������� ������ ����������
     sum_tProc; // ����� ������ ������� �� ���
//
 F1->Label_Data->Font->Color   = clBlack;
 F1->Label_Buffer->Font->Color = clBlack;
//
 all_maxProcs   = 0; // ������� ������������ � �������� ����� ���
 serial_Ticks   = 0; // ����� ������ ������ ��� (���������� ����� ����������������� ����������)
 parallel_Ticks = 0; // ����� ���������� ������������ ���������
//
 ftime(&t1); // ����� ������ ������� ��������� ���������� ���������
//
// ������������ ���������� �������� ��� �� ������ �� Tpr -----------------------
 for( INT i=0; i<mTpr->Count; i++ )
 {
  if( !(i % d_APM) ) // ������ d_APM ���
   APM // ���� ����������  Windows ---------------------------------------------
//
  if( !(i % 100 ) ) // ���� i ������ 100
  {
   sprintf( w, " ��������������� ��������� ������ (%.0f%%)...", 1e2*i / mTpr->Count);
   SBM0->Text = w;
  }
//
  strcpy(tmp, mTpr->Strings[i].c_str()); // ��������� ������ �� mTpr � tmp
//
  serial_Ticks += _atoi64(GetSubString(tmp, 31,40)); // ��������� �� ���� ����������� �����������
//
  strcpy(tmp1, GetSubString(tmp, 1,10)); // ����� ����������������� ���
  if( _atoi64(tmp1) > all_maxProcs ) // ���� ������������ �������� ������ ���
   all_maxProcs = _atoi64(tmp1);
 } // ����� �� ������� � Tpr
//
 all_maxProcs ++ ; // ��� ���� � ���� !
//
////////////////////////////////////////////////////////////////////////////////
 strcpy(tmp, mTpr->Strings[0].c_str()); // ������ ������ ���������� ������ ����������
 parallel_Ticks = _atoi64(GetSubString(tmp, 11,20));
//..............................................................................
 strcpy(tmp, mTpr->Strings[mTpr->Count-1].c_str()); // ������ ����� ���������� ��������� ����������
 parallel_Ticks = _atoi64(GetSubString(tmp, 21,30)) - parallel_Ticks; // �������� ������� � ���������� �������
//
// ----- �������� parallel_Ticks �� "������ ����" (��� �� �� �����) -----------
 if( parallel_Ticks <= 0 )
 {
  t_printf( "\n-E- ����� ����� ���������� ��������� �� ������������ - ���������� �� ��������������... -E-" );
  Regim = 2; // �� ������������ ������ ������������� ����������
  return;
 }
//
////////////////////////////////////////////////////////////////////////////////
 simult_maxProcs = Work_TimeSets_Protocol_IC(); // max ������������ ���������� ���
////////////////////////////////////////////////////////////////////////////////
//
 SBM0->Text = " ��������� ���������..."; // ����� ������ � StatusBarMain (������ 0)
 SBM->Repaint(); // ������������ �������������
//
// ����� ������ ��� ������������ ...............................................
//
 t_printf( "\n����� ���������� ���������:" );
//
 t_printf( "===========================" );
//
 t_printf( "������������ = %d ������ (%.*g ���), ������������ %d (��� %d ������������) ����/� ��� �� %d ���������",
            parallel_Ticks,
            ACC_REAL,(t1.time + 1.0e-3*t1.millitm) - (t0.time + 1.0e-3*t0.millitm),
            all_maxProcs, // ����� ��� ���� ������������
            simult_maxProcs, // max ������������ ���������� ���
            max_Proc ); // ����� � ������� ������
//
 t_printf( "���������������� = %ld ������", serial_Ticks );
//
 t_printf( "��������� (����������) ���������� = %.*g\n", ACC_REAL, 1.0*serial_Ticks / parallel_Ticks);
//
 REAL index = Calc_ConnectedIndex( 0 );  // ������� �������� ��������� ��� ���������� � ����
//
 if (index != ERR_ALTERNAT) // ������� ��������, � �� �������������!!!
  t_printf( "���������� ���������������� ����� ��������� = %.3g \n",
             index); // ������� �������� ��������� ��� ���������� �
//
 t_printf( "����� ����������� %d ���������� (�� �������� SET)\n", mTpr->Count);
//
// ������ ��������� ��� ������ �� ����������� ���������� .......................
 for(int j=0; j<Count_Sets; j++) // �� ������ ����������
 {
   sprintf( w, " ������������� ��������� ������ (%.0f%%)...", 1e2*j / Count_Sets);
   SBM0->Text = w;
//
  strcpy(Set, Set_Params[j].Set); // ��������� ��� �������� ������
//
  n_Sets = 0; // ������� ����� ���������� ���������� Set
//
  for( INT i=0; i<mTpr->Count; i++ ) // �� ������ Tpr
  {
   if( !(i % d_APM) ) // ������ d_APM ���
    APM // ���� ����������  Windows --------------------------------------------
//
   strcpy(tmp,  mTpr->Strings[i].c_str()); // ��������� ������ �� Tpr � tmp
   strcpy(tmp1, GetSubString(tmp, 41,50)); // ����� ���������� � ���� ������ tmp1
//
   if(!strcmp(Set, Mem_Instruction[atoi(tmp1)].Set)) // ���� ���������� ...
    n_Sets ++ ;
  } // ����� ����� �� ������� Tpr
//
  if( n_Sets ) // ������ ���� ����������� ���� �� ���...
   t_printf( "���������� %s ����������� %d ��� (%.3g%% )",
              Set, n_Sets, 1e2 * n_Sets / mTpr->Count);
//
 } // ����� ����� �� ������ ���������� � Set_Params[]
//
// ������ ��������� ������� (�� �������) ������������� ������� �� ���
 for(int i=0; i<all_maxProcs; i++) // �� ����� ���...
 {
  sum_tProc = 0.0; // �������� ����� ������ ������ ������� ���
//
  for(ULI j=0; j<mTpr->Count; j++) // �� ������ ����������� ����������...
  {
   strcpy(tmp,  mTpr->Strings[j].c_str()); // ��������� ������ �� Tpr � tmp
   strcpy(tmp1, GetSubString(tmp, 1,10)); // ����� ��� � ���� ������ tmp1
//
   if(i == _atoi64(tmp1)) // ����� � Tpr ����� ��� i
   {
    strcpy(tmp1, GetSubString(tmp, 31,40)); // ����� ���������� ����� ���������� �� i-��� ���
    sum_tProc += _atoi64(tmp1);
   }
  } // ����� ����� �� ������ ����������
//
 t_printf( "�����.������.�-�� (���) ����� %d �������� %ld ������ (%.3g%% )",
            i, sum_tProc, 1e2 * sum_tProc / parallel_Ticks);
//
 } // ����� ����� ��� ���
//
 Save_All_Protocols_To_Out_Dir(); // ���������� ���������� � Out!Data
//
} // ----- ����� Calc_Stat_Proc ------------------------------------------------


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
char* __fastcall // ������������ ������ ���������� Mem_Instruction[] ����� i_Set
Vizu_Flags( INT i_Set )
{
 char Set[_SET_LEN],
      Delimeter[] = "|", // ������ ����������� ������� ������
      Indiff[]    = "x", // ������ '�����������'
      tmp[_512];
//
 strcpy(Set, Mem_Instruction[i_Set].Set); // ��������� ��������� ���������� (��� �������)
//
////////////////////////////////////////////////////////////////////////////////
 if( is_SET( Set ) ) // ��� ���������� SET......................................
  snprintf(tmp,sizeof(tmp), " %1s%1s%1s%1s%1s%1s%1s%1s%1s%1s%1s%1d%1s",
               Delimeter, Indiff,
               Delimeter, Indiff,
               Delimeter, Indiff,
               Delimeter, Indiff,
               Delimeter, Indiff,
               Delimeter, Mem_Instruction[i_Set].fExecOut,
               Delimeter);
//
 else
////////////////////////////////////////////////////////////////////////////////
 switch( Get_CountOperandsByInstruction(Set) ) // ... ����� ������� ��������� ���������� Set
 {
  case 1: snprintf(tmp,sizeof(tmp), " %1s%1d%1s%1s%1s%1d%1s%1d%1s%1d%1s%1d%1s", // � ���� ���������� 1 �������
                       Delimeter, Mem_Instruction[i_Set].fOp1,
                       Delimeter, Indiff,
                       Delimeter, Mem_Instruction[i_Set].fPredicat_TRUE,
                       Delimeter, Mem_Instruction[i_Set].fAddBuffer,
                       Delimeter, Mem_Instruction[i_Set].fExec,
                       Delimeter, Mem_Instruction[i_Set].fExecOut,
                       Delimeter);
          break;
//
  case 2: snprintf(tmp,sizeof(tmp), " %1s%1d%1s%1d%1s%1d%1s%1d%1s%1d%1s%1d%1s", // � ���� ���������� 2 ��������
                       Delimeter, Mem_Instruction[i_Set].fOp1,
                       Delimeter, Mem_Instruction[i_Set].fOp2,
                       Delimeter, Mem_Instruction[i_Set].fPredicat_TRUE,
                       Delimeter, Mem_Instruction[i_Set].fAddBuffer,
                       Delimeter, Mem_Instruction[i_Set].fExec,
                       Delimeter, Mem_Instruction[i_Set].fExecOut,
                       Delimeter);
          break;
//
 default: strcpy( tmp, notDefined );
          break;
//
 } // ����� switch
//
 return (tmp);
//
} // ----- ����� Vizu_Flags ----------------------------------------------------


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void __fastcall // ���������� �� �������������� ������
Display_Error(char *str)
{
// MessageDlg(str, mtError, TMsgDlgButtons() << mbOK, 0);
 F1->Master_Timer->Enabled = false; // ���������� ������� ������
//
 do_Run // "��������" ��� ������ ����������
//
 MessageBox( 0, str, "�������� � �����������...", MB_OK | MB_ICONERROR | MB_SYSTEMMODAL );
//
} // ---------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void __fastcall
Vizu_Flow_Exec() // ��������������� ������� ���������� ���������
{
 char tmp[_512];
//
 snprintf(tmp,sizeof(tmp), " ���: %d/%d | ����� %.1f%% | ��������� %.1f%% (%d/%d) ����������",
              max_Proc - Free_Proc, max_Proc,
              1.0e2 * Really_Buffer / max_Buffer,
              1.0e2 * Already_Exec  / Really_Set,
              Already_Exec, Really_Set);
 SBM0->Text = tmp; // ����� ������ � StatusBarMain (������ 0)
// SBM0->Canvas->Repaint(); // ������������ �������������
 SBM->Repaint(); // ������������ �������������
//
} // ����� Vizu_Flow_Exec ------------------------------------------------------


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void __fastcall // ������ ���������� ( ������� ������ ��������� )
TF1::Run_Calculations(TObject *Sender)
{
 Write_Config(); // ���������� ���� ������������
 max_Proc = StrToInt(F1->E_AIU->Text); // �������� � ����� (������)
 Out_Data_SBM1(); // ����� ������ � ������� ����� StatusBar -------------------
//
 Start_DF( 0 ); // ����� ��� ������������� ����������
} // ---- ����� TF1::Run_Calculations ------------------------------------------


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void __fastcall
Start_DF( int Mode )
{
// Mode = 0 ������������� ������� ������ ���������
// Mode = 1 -,-,- "���������� ���������� � ���������"
//
 Read_Config( 1 ); // ���������� ���� ������������ (��� ��������� ��������� � �������� F1)
//
 count_Ops = 0; // ���������� ����������� ���������� � ������ ������ (������)
//
// --- ������ ���� � �������� ������ ������������ ������ (������� ���� � �����)
 snprintf( PathToSubDirOutData,sizeof(PathToSubDirOutData), "%s%s\\", ExtractFilePath ( Application->ExeName ), NameSubDirOutData);
///
 if( !DirectoryExists( PathToSubDirOutData ) ) // ���� �� ���������� ����� ��������...
  if( !CreateDir( PathToSubDirOutData ) ) // ���� �� ������� �������...
   strNcpy( PathToSubDirOutData, '\0' ); // �������� ���� � ����������� PathToSubDirOutData
//
// ������� ���������� ����� ������ ---------------------------------------------
 strNcpy( uniqueStr, PutDateTimeToString(1) ); // ���������� ������ ( ���� + ����� �� ���� )
//
 randomize(); // ������������� ������� rand() ��� ���������� ���������� RND
//
 F1->Master_Timer->Enabled = true; // �������� ������� ������
//
 localTick = // ������ ���������� ��������� (������)
 localTickOfEndLastExecuteSet = 0; // ������ �������.���� ������.�������.���������� (������)
//
// ��� ������������ ������ ��������� � ������������ Mem_Data[] � Mem_Buffer � ��������� � �������� (����������)
 flagAlarmData = flagAlarmBuffer = flagAlarmParser = true;
//
 F1->Label_Data->Font->Color   = clBlack;
 F1->Label_Buffer->Font->Color = clBlack;
//
 mR->Clear(); // �������� ����� ������ ��������� �������
//
 if( F2 )
  F2->Close(); // ���� ������� ������������� ���������� �������
//
 if( F3 )
  F3->Close(); // ���� ������� ������������� ���������� �������
//
 ULI max_Proc_New = StrToInt( F1->E_AIU->Text ); // ����� ����� ��� (������������ ������������)
//
 max_Proc = (max_Proc >  MAX_PROC) ? MAX_PROC : max_Proc;
 max_Proc = (max_Proc <=        0) ? 1        : max_Proc;
 F1->E_AIU->Text = max_Proc_New;
 F1->E_AIU->Repaint(); // ������������� ����������...
//
 if( max_Proc_New != max_Proc ) // �������� ���� ����� ���, ��� ����
 {
  max_Proc = max_Proc_New; // ��������...
//
  Write_Config(); // ��������� ��� ����� � ����� ��������
//
  Mem_Proc = ( mp* ) realloc( Mem_Proc, max_Proc * sizeof( mp ) ); // ���������������� ������ ������ ��� ���
 } // �����  if( max_Proc_New != max_Proc )
//
 mR->Clear(); // �������� ����� ������ ��������� �������
//
 Install_All_Flags(); // �������� ��� ����� � Mem_Instruction[]
 Vizu_Instructions(); // ��������������� ��� ���������� � ����_����������
//
 Really_Data = 0; // �������� ��� ������
 Vizu_Data();  // ��������������� ������ � ����_������
//
 Really_Buffer = 0; // �������� ����� ������
 Vizu_Buffer();  // ��������������� ���������� � ������_����������
//
 Out_Data_SBM1(); // ����� ������ � ������� ����� StatusBar --------------------
//
 Clear_AllTableInstructions(); // �������� ��� ������ ������� ������������ ����������
//
 Make_Row_Current( 0 ); // ���������� ������� ������ �������
//
 Start_DataFlow_Process( Mode ); // �������� ����
//
} // ----- ����� Start_DF ------------------------------------------------------


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void __fastcall // ������ ���� (� ������, �������� Mode )
Start_DataFlow_Process( int Mode )
{
// Mode = 0 ������������� ������� ������ ���������
// Mode = 1 -,-,- "���������� ���������� � ���������"
//
 do_Stop // "���������" ��� ������ ����������
//
 Regim = 1; // ������ ���������� ���������
//
 for( INT i=0; i<max_Proc; i++ ) // ��� ���...
  Mem_Proc[i].Busy = false; // "c�������"..!!!
//
 Free_Proc = max_Proc; // ���� ��� ��� ��������
//
 mR->Clear(); // �������� ����� ������ ��������� �������
//
 Read_Instructions(); // ���������� ��������� �� �����
//
 if( Mode ) // ��������� ���������� ���������� � ������_����������
  Mixed_Instructions(); // ���������� ����������
//
 Vizu_Instructions(); // ��������������� ���������� � ����_����������
//
 Really_Data = 0; // �������� ��� ������
 Vizu_Data();  // ��������������� ������ � ����_������
//
 Really_Buffer = 0; // �������� �����
 Vizu_Buffer();  // ��������������� ������ � ������
//
////////////////////////////////////////////////////////////////////////////////
 mTpr->Clear(); // �������� ������ ����� ������ ��� ������� ������ ���� ���
////////////////////////////////////////////////////////////////////////////////
//
 Already_Exec = 0; // ...��� ���������� ���������
//
 Install_All_Flags(); // �������� ��� �����
//
////////////////////////////////////////////////////////////////////////////////
 ftime( &t0 );  // ����� ������ ������� ������ ���������� ���������
 localTick = 0; // ����� ������ ������� � ���� !
////////////////////////////////////////////////////////////////////////////////
//
// ��� ����������� �����  Primary_Init_Data() !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//
 if( !Test_All_Operands() ) // ���� ���� ���� ��������� �� ����������� ��� ���������..1
 {
  do_Run // "��������" ��� ������ ����������
  return; // ����������� ���������� ���������
 }
//
 Upload_Data( 0 ); // ��������� SET-���� �� ������ ( Rule == 1 )
//
 Primary_Init_Data(); // ������������� ������ ����������� SET etc etc
////////////////////////////////////////////////////////////////////////////////
 sleep_for_vizu_buffer // ����-� ��� ������������ ������
////////////////////////////////////////////////////////////////////////////////
} // ----- ����� Start_DataFlow_Process (int Mode) -----------------------------


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void __fastcall // ���� ������� � ���������� ����������
Primary_Init_Data()
{///////////////////////////////////////////////////////////////////////////////
// ���� ����� - ��������� ���� ������� � Mem_Data[] �������� ��� ������� ��������
// ���������� ��� ������, ��� ���� ��������� ������ ��� ���������� SET
////////////////////////////////////////////////////////////////////////////////
//
 for( INT i=0; i<Really_Set; i++ ) // ���� �� ���� ����������� � Mem_Instruction .......
  {
   if( is_SET( Mem_Instruction[i].Set ) ) // ��� "�����������" SET......................
    Finalize_Only_SET( i ); // �������������� �� ���, � ������� ��������������
//
// ����� Finalize_Only_SET ��������� ���������� i_Set (��������� �����. ������ �
// Mem_Data, ������������� ���� "���������" � ������ ����������, ��������� Mem_Instruction[]
// �� ���� ����������� �� ������� � ������ ��������� ����� ��� ���������� � Mem_Data)
//
  } // ����� ����� �� ���� ����������
//
// ��������� �������������� ������������� ������ (��������� ���������� ���� SET)
//
} // ----- ����� Primary_Init_Data ---------------------------------------------


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void __fastcall // ���������� ���������� ����� i_Set
ExecuteInstructions_Except_SET( INT i_Set )
{ // ����� ���������� SET !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 REAL Op1, Op2, Result, Predicate;
 char Set[_SET_LEN],
      aOp1[_ID_LEN],    aOp2[_ID_LEN],
      aResult[_ID_LEN], aPredicate[_ID_LEN],
      tmp[_512];
 INT  i_Proc; // ����� ���������� ���
 __int64 Divident, Devider; // Quotient, Remainder; // ���������/�����������//�������/������� (����� 64 ���)
//
 if( !Regim ) // �� ��������� - ��������� ���� ---------------------------------
  return;
//
 if( is_SET( Mem_Instruction[i_Set].Set ) ) // ���������� SET �� ������������ ����� !!!
  return;
//
// ----- ����� �������� �� ����, ��� ��������� ����n������ �� �������� ���� � ����� ������ !!!
//
////////////////////////////////////////////////////////////////////////////////
// ���� ��������� ��� ��� ���������� ���������� ����� i_Set ....................
 for(i_Proc=0; i_Proc<max_Proc; i_Proc++) // �� ���� ��� .......................
  if(!Mem_Proc[i_Proc].Busy) // ��������� (���) i_Proc �������� ................
   goto find_free_proc;
//
 t_printf( "-W- %s(): ��� ��������� ���! �� ���� ���������� �� ����� ���� ���������... -W-", __FUNC__ );
//
 return;
//
// ��� !!! ���� ��������� ��� !!! ==============================================
//
 find_free_proc: // �� ��������� ��� i_Proc ��������� ���������� i_Set =========
//
////////////////////////////////////////////////////////////////////////////////
 Mem_Proc[i_Proc].Busy = true; // '���������' ��� ..............................
//
////////////////////////////////////////////////////////////////////////////////
 Free_Proc -- ; // ����� ��������� ��� ��������� �� �������
////////////////////////////////////////////////////////////////////////////////
//
 Mem_Instruction[i_Set].fExec = true; // ���������� ���� '����������' ��� ������ ����������
 mS->Cells[6][i_Set+1] = Vizu_Flags(i_Set); // ��������������� ����� ������ ����������
//
// ������������ ��������� ������� ��� ���������� �� ���m ��� ///////////////////
 strcpy( Set, Mem_Instruction[i_Set].Set ); // ��� (���������) �������
 strcpy( aResult, Mem_Instruction[i_Set].aResult ); // ����� ���������� (��� ���� ��������)
//
////////////////////////////////////////////////////////////////////////////////
 switch( Get_CountOperandsByInstruction(Set) ) // ... ����� ������� ��������� ���������� Set
 {
  case 1: strcpy( aOp1, Mem_Instruction[i_Set].aOp1 ); // ����� (������!) ������� ��������
          Op1 = get_Data( aOp1, true ); // �������� ������� ��������
          break;
//
  case 2: strcpy(aOp1, Mem_Instruction[i_Set].aOp1); // ����� (������!) ������� ��������
          strcpy(aOp2, Mem_Instruction[i_Set].aOp2); // ... ������� ��������
          Op1 = get_Data( aOp1, true ); // �������� ������� ��������
          Op2 = get_Data( aOp2, true ); // �������� ������� ��������
          break;
//
 default: break;
//
 } // ����� switch( Get_CountOperandsByInstruction(Set) )
//
//||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
//
// �������� ������ ������� (� �����) ������ ���������� ������ ����������
 Mem_Proc[i_Proc].tick_Start = localTick; // ����� ���� ������ ����������.........
//
////////////////////////////////////////////////////////////////////////////////
 if(!strcmp(Set, "ADD")) // ��� ���������� ADD
 {
  Result = Op1 + Op2; // ��������� ��������
 }
// ����� ���������� ���������� ADD .............................................
//
////////////////////////////////////////////////////////////////////////////////
 else
 if(!strcmp(Set, "SUB")) // ��� ���������� SUB
 {
  Result = Op1 - Op2; // ��������� ��������
 }
// ����� ���������� ���������� SUB .............................................
//
////////////////////////////////////////////////////////////////////////////////
 else
 if(!strcmp(Set, "MUL")) // ��� ���������� MUL
 {
  Result = Op1 * Op2; // ��������� ��������
 }
// ����� ���������� ���������� MUL .............................................
//
////////////////////////////////////////////////////////////////////////////////
 else
 if(!strcmp(Set, "DIV")) // ��� ���������� DIV
 {
  if( fabs (Op2) <= MIN_FLOAT) // ������ ������ �� ����� ��������� �����...
   {
    snprintf(tmp,sizeof(tmp), "-E- %s(): ���������� %s (#%d) �� ����� ���� ��������� (�������� = {%.*g}). ������� ���������� �����������... -E-",
                  __FUNC__, Set, i_Set, ACC_REAL, Op2);
    AddLineToProtocol(tmp);
    Display_Error(tmp);
    Code_Error = -1; // ������� ������ �� 0.0
    return;
   }
  else
   Result = Op1 / Op2; // ��������� �������� DIV
 }
// ����� ���������� ���������� DIV .............................................
//
////////////////////////////////////////////////////////////////////////////////
 else
 if(!strcmp(Set, "DQU")) // ��� ���������� DQU (��������� ������ �������� �� �������)
 {
  Divident = Op1; // ������� ��� __int64
  Devider  = Op2; // �������� ��� __int64
//
  if( !Devider ) // ���� 0...
  {
   snprintf(tmp,sizeof(tmp), "-E- %s(): ���������� %s �� ����� ���� ��������� (�������� �������). ������� ���������� �����������... -E-",
                 __FUNC__, Set);
   AddLineToProtocol(tmp);
   Display_Error(tmp);
   Code_Error = -1; // ������� ������ �� 0
   return;
   }
  else
   Result = REAL ( Divident / Devider ); // ��������� �������� DQU
 }
// ����� ���������� ���������� DQU .............................................
//
////////////////////////////////////////////////////////////////////////////////
 else
 if(!strcmp(Set, "RND")) // ��� ���������� RND (����� ��������� ����� �� ��������� ���������� ���������
 {
  Result = Op1 + ( Op2 - Op1 ) * REAL(rand()) / REAL(RAND_MAX);
 }
// ����� ���������� ���������� RND .............................................
//
////////////////////////////////////////////////////////////////////////////////
 else
 if(!strcmp(Set, "DRE")) // ��� ���������� DQU (��������� ������ ������� �� �������)
 {
  Divident = Op1; // ������� ��� __int64
  Devider  = Op2; // �������� ��� __int64

  if( !Devider ) // ���� 0...
  {
   snprintf(tmp,sizeof(tmp), "-E- %s(): ���������� %s �� ����� ���� ��������� (�������� �������). ������� ���������� �����������... -E-",
                 __FUNC__, Set);
   AddLineToProtocol(tmp);
   Display_Error(tmp);
   Code_Error = -1; // ������� ������ �� 0
   return;
  }
  else
   Result = REAL ( Divident % Devider ); // ��������� �������� DRE
 }
// ����� ���������� ���������� RND .............................................
//
////////////////////////////////////////////////////////////////////////////////
 else
 if(!strcmp(Set, "SQR")) // ��� ���������� SQR
 {
  if(Op1 < 0.0) // ������ ��������� ���������� ������ �� �������������� �����...
  {
   snprintf(tmp,sizeof(tmp), "-E- %s(): ���������� %s (#%d) �� ����� ���� ��������� ��� ������������� ������ {%.*g}. ������� ���������� �����������... -E-",
                 __FUNC__, Set, i_Set, ACC_REAL, Op1);
   AddLineToProtocol(tmp);
   Display_Error(tmp);
   Code_Error = -2; // ��������� ������� ������� �� 0 ---
   return;
  }
  else
   Result = sqrt ( Op1 );
 }
// ����� ���������� ���������� SQR .............................................
//
////////////////////////////////////////////////////////////////////////////////
 else
 if(!strcmp(Set, "SIN")) // ��� ���������� SIN
 {
  Result = sin ( Op1 ); // ���������� ����������� �������
 }
// ����� ���������� ���������� SIN .............................................
//
////////////////////////////////////////////////////////////////////////////////
 else
 if(!strcmp(Set, "COS")) // ��� ���������� COS
 {
  Result = cos ( Op1 ); // ���������� ����������� �������
 }
// ����� ���������� ���������� COS .............................................
//
////////////////////////////////////////////////////////////////////////////////
 else
 if(!strcmp(Set, "ASN")) // ��� ���������� ASN
  {
   Result = asin ( Op1 ); // ���������� ����������� �������
  }
// ����� ���������� ���������� ASN .............................................
//
////////////////////////////////////////////////////////////////////////////////
 else
 if(!strcmp(Set, "ACN")) // ��� ���������� ACN
 {
   Result = acos ( Op1 ); // ���������� ����������� �������
 }
// ����� ���������� ���������� ACN .............................................
//
////////////////////////////////////////////////////////////////////////////////
 else
 if(!strcmp(Set, "TAN")) // ��� ���������� TAN... ���� ������� ������� ��
// � ����� ��������� ������? ������ ��������, ������ !..
  {
    Result = tan ( Op1 ); // ������������ ����������� �������
  }
// ����� ���������� ���������� TAN .............................................
//
////////////////////////////////////////////////////////////////////////////////
 else
 if(!strcmp(Set, "ATN")) // ��� ���������� ATN... ���� ������� ������� ��
// � ����� ��������� ������? ������ ��������, ������ !..
  {
   Result = atan ( Op1 ); // ���������� ����������� �������
  }
// ����� ���������� ���������� ATN .............................................
//
////////////////////////////////////////////////////////////////////////////////
 else
 if(!strcmp(Set, "LOG")) // ��� ���������� LOG
 {
  Result = log( Op1 );
 }
// ����� ���������� ���������� LOG .............................................
//
////////////////////////////////////////////////////////////////////////////////
 else
 if(!strcmp(Set, "POW")) // ��� ���������� POW
 {
  Result = pow( Op1, Op2 ) ;
 }
// ����� ���������� ���������� POW .............................................
//
////////////////////////////////////////////////////////////////////////////////
 else
 if(!strcmp(Set, "EXP")) // ��� ���������� EXP
 {
  Result = exp ( Op1 ); // ���������� ����������� �-�
 }
// ����� ���������� ���������� EXP .............................................
//
////////////////////////////////////////////////////////////////////////////////
 else
 if(!strcmp(Set, "SNH")) // ��� ���������� SNH
 {
  Result = sinh ( Op1 ); // ���������� ����������� �������
 }
// ����� ���������� ���������� SNH .............................................

////////////////////////////////////////////////////////////////////////////////
 else
 if(!strcmp(Set, "CNH")) // ��� ���������� CNH
 {
  Result = cosh ( Op1 ); // ���������� ����������� �������
 }
// ����� ���������� ���������� CNH .............................................
//
////////////////////////////////////////////////////////////////////////////////
 else
 if(!strcmp(Set, "TNH")) // ��� ���������� TNH
 {
  Result = tanh ( Op1 ); // ���������� ����������� �������
 }
// ����� ���������� ���������� TNH .............................................

////////////////////////////////////////////////////////////////////////////////
 else
 if(!strcmp(Set, "CPY")) // ��� ���������� CPY ( Result <= Operand_1 )
 {
  Result =  Op1;
 }
// ����� ���������� ���������� CPY .............................................
//
////////////////////////////////////////////////////////////////////////////////
 else
 if(!strcmp(Set, "ABS")) // ��� ���������� ABS ( Result = abs ( Operand_1 ) )
 {
  Result = fabs ( Op1 );  // ���������� ����������� �-�
 }
// ����� ���������� ���������� ABS .............................................

////////////////////////////////////////////////////////////////////////////////
 else
 if(!strcmp(Set, "SGN")) // ��� ���������� SGN ( Result <= ���� (signum) Operand_1 )
 {
  Result = ( Op1 >= 0.0) ? (1.0) : (-1.0) ;
 }
// ����� ���������� ���������� ABS .............................................

////////////////////////////////////////////////////////////////////////////////
 else
 if(!strcmp(Set, "NEG")) // ��� ���������� NEG ( Result = - Operand_1 )
 {
  Result = - Op1;
 }
// ����� ���������� ���������� NEG .............................................
//
////////////////////////////////////////////////////////////////////////////////
 else
 if(!strcmp(Set, "FLR")) // ��� ���������� FLR ( Result = floor( Operand_1 ) )
 {
  Result = floor( Op1 );
 }
// ����� ���������� ���������� FLR .............................................
//
////////////////////////////////////////////////////////////////////////////////
 else
 if(!strcmp(Set, "CEL")) // ��� ���������� CEL ( Result = ceil( Operand_1 ) )
 {
  Result = ceil( Op1 );
 }
// ����� ���������� ���������� CEL .............................................
//
////////////////////////////////////////////////////////////////////////////////
 else
 if(!strcmp(Set, "PGE")) // ��� ���������� PGE
 {
  Result = ( Op1 >= Op2 ) ? 1.0 : 0.0 ;
 }
// ����� ���������� ���������� PGE .............................................
//
////////////////////////////////////////////////////////////////////////////////
 else
 if(!strcmp(Set, "PLE")) // ��� ���������� PLE
 {
  Result = ( Op1 <= Op2 ) ? 1.0 : 0.0 ;
 }
// ����� ���������� ���������� PLE .............................................
//
////////////////////////////////////////////////////////////////////////////////
 else
 if(!strcmp(Set, "PEQ")) // ��� ���������� PEQ
 {
  Result = ( Op1 == Op2 ) ? 1.0 : 0.0 ;
 }
// ����� ���������� ���������� PEQ .............................................
//
////////////////////////////////////////////////////////////////////////////////
 else
 if(!strcmp(Set, "PNE")) // ��� ���������� PNE
 {
  Result = ( Op1 != Op2 ) ? 1.0 : 0.0 ;
 }
// ����� ���������� ���������� PNE .............................................
//
////////////////////////////////////////////////////////////////////////////////
 else
 if(!strcmp(Set, "PGT")) // ��� ���������� PGT
 {
  Result = ( Op1 > Op2 ) ? 1.0 : 0.0 ;
 }
// ����� ���������� ���������� PGT .............................................
//
////////////////////////////////////////////////////////////////////////////////
 else
 if(!strcmp(Set, "PLT")) // ��� ���������� PLT
 {
  Result = ( Op1 < Op2 ) ? 1.0 : 0.0 ;
 }
// ����� ���������� ���������� PLT .............................................
//
////////////////////////////////////////////////////////////////////////////////
 else
 if(!strcmp(Set, "PNT")) // ��� ���������� PNT (��������� - NOT)
 {
  Result = ( Op1 == 0.0 ) ? 1.0 : 0.0 ; // ��������� - NOT
 }
// ����� ���������� ���������� PNT .............................................
//
////////////////////////////////////////////////////////////////////////////////
 else
 if(!strcmp(Set, "PAN")) // ��� ���������� PAN
 {
  Result = ( Op1 && Op2 ) ? 1.0 : 0.0 ; // ���������� "�" (AND)
 }
// ����� ���������� ���������� PAN .............................................
//
////////////////////////////////////////////////////////////////////////////////
 else
 if(!strcmp(Set, "POR")) // ��� ���������� POR
 {
  Result = ( Op1 || Op2 ) ? 1.0 : 0.0 ; // ���������� "���" (OR)
 }
// ����� ���������� ���������� POR .............................................
//
////////////////////////////////////////////////////////////////////////////////
 else
 if(!strcmp(Set, "PIM")) // ��� ���������� PIM (����������, ����������)
 {
  Result = ( ( Op1 != 0.0 ) && ( Op2 == 0.0 ) ) ? 0.0 : 1.0 ; // ���������� (����������)
 }
// ����� ���������� ���������� PIM .............................................
//
////////////////////////////////////////////////////////////////////////////////
 else
 if(!strcmp(Set, "PRM")) // ��� ���������� PRM (�������� ����������)
 {
  Result = ( ( Op1 == 0.0 ) && ( Op2 != 0.0 ) ) ? 0.0 : 1.0 ; // �������� ����������
 }
// ����� ���������� ���������� PRN .............................................
//
////////////////////////////////////////////////////////////////////////////////
 else
 if(!strcmp(Set, "PEV")) // ��� ���������� PEV (������������)
 {
  Result = ( ( ( Op1 != 0.0 ) && ( Op2 != 0.0 ) ) ||
             ( ( Op1 == 0.0 ) && ( Op2 == 0.0 ) ) )
            ? 1.0 : 0.0 ; // ���������������
 }
// ����� ���������� ���������� PEV .............................................
//
////////////////////////////////////////////////////////////////////////////////
 else
 if(!strcmp(Set, "PAP")) // ��� ���������� PAP (������� �����, ���-��)
 {
  Result = ( ( Op1 == 0.0 ) && ( Op2 == 0.0 ) ) ? 1.0 : 0.0 ; // ������� �����, ���-��
 }
// ����� ���������� ���������� PAP .............................................
//
////////////////////////////////////////////////////////////////////////////////
 else
 if(!strcmp(Set, "PHS")) // ��� ���������� PHS (����� �������, �-��)
 {
  Result = ( ( Op1 != 0.0 ) && ( Op2 != 0.0 ) ) ? 0.0 : 1.0 ; // ����� �������, �-��
 }
// ����� ���������� ���������� PHS .............................................
//
////////////////////////////////////////////////////////////////////////////////
 else
 if(!strcmp(Set, "PXR")) // ��� ���������� PXR (������� ����������, ����������� ���)
 {
  Result = ( ( ( Op1 == 0.0 ) && ( Op2 == 0.0 ) ) ||
             ( ( Op1 == 1.0 ) && ( Op2 == 1.0 ) )
           ) ? 0.0 : 1.0 ; // ������� ����������, ����������� ���
 }
// ����� ���������� ���������� PXR .............................................
//
//||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
// ����� �� ���� ����������� � ������ ������....................................
//||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
//
// ��������� � ������ ��� ���������� �� ������� ��������� � ���������� ��������
   Mem_Proc[i_Proc].i_Set = i_Set; // ����� ���������� �� Mem_Instruction[]
   strcpy( Mem_Proc[i_Proc].aOp1,    Mem_Instruction[i_Set].aOp1 );
   strcpy( Mem_Proc[i_Proc].aOp2,    Mem_Instruction[i_Set].aOp2 );
   strcpy( Mem_Proc[i_Proc].aResult, Mem_Instruction[i_Set].aResult );
//
// ��������� � ������ ��� ���������� �� ��������� � ���������� �������� --------
   Mem_Proc[i_Proc].Op1    = Op1; // ��������� ������ �������
   Mem_Proc[i_Proc].Op2    = Op2; // ��������� ������ �������
   Mem_Proc[i_Proc].Result = Result; // ��������� ��������� ��������
//
   snprintf( tmp, sizeof(tmp),"-I- %s(): ���������� #%d [%s] ������ ����������� �� ��� #%d -I-",
              __FUNC__, i_Set, Line_Set(i_Set, 2, 0.0), i_Proc );
   if( SpeculateExec && Mem_Instruction[i_Proc].fSpeculateExec ) // ������������� ����������
    strcat( tmp, SPECUL );
   t_printf( tmp ) ;
//
  if( pass_Counts>0 && !(count_Ops % pass_Counts) ) // ������ pass_Counts ���
    Make_Row_Current( i_Set ); // ������� ������ ������ � SG_Set[][] ������� (��������� �������� ����������)
//
   count_Ops ++ ; // ����� ���������� �� ������� ��������� ( ��� ��������� �� ���� ������ count_Ops ++ )
//
} // ------- ����� ExecuteInstructions_Except_SET ------------------------------


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void   __fastcall
add_Buffer( INT i_Set, int Rule ) // ��������� � ����� ������ Mem_Buffer[] ������ ����� i_Set � ���-�����������
// ����������� �� ���� ������, � �� ����� �� Mem_Instruction[] � ���� Mem_Buffer[].i_Set
// � Mem_Buffer[]->(float)Param ��������� �������� "����������" (��� ������������ ������������)
// ���� ������� ��� ��������� ������ (���������� ���� Mem_Instruction[].fAddBuffer) - �� �����������
// ����� ���������� ���� Mem_Instruction[].fAddBuffer ���������������
// Rule ���������� ����� ������ �������
{
 char tmp[_512];
 static bool flagColor = true; // ���� �������������� � ������������ Mem_Buffer[]
//
 if(Really_Buffer >= max_Buffer) // ����� �����
 {
  if( flagAlarmBuffer ) // ���� ��� ������� ���������
  {
   t_printf( "->\n-E- %s(): ���������� #%d/%d [%s] �� ��������� � �����, ��� �� ����� (%s) -E-\n->",
              __FUNC__, i_Set,Rule, Line_Set(i_Set, 2, 0.0), Get_Time_asLine() );
//
   flagAlarmBuffer = false;
  }
//
  if( flagColor ) // ������� ������ ������� Label_Buffer
   F1->Label_Buffer->Font->Color = clRed;
  else
   F1->Label_Buffer->Font->Color = clBlack;
//
  F1->Label_Buffer->Refresh(); // ����������...
//
  flagColor = !flagColor;
//
  return;
 } // ����� if(Really_Buffer >= 0.99*Max_Buffer)
////////////////////////////////////////////////////////////////////////////////
//
 if( Mem_Instruction[i_Set].fAddBuffer ) // ���������� ��� ���� ��������� � �����
  return;
//
 Mem_Buffer[Really_Buffer].i_Set = i_Set; // ������ �� ����� ����������
 Mem_Buffer[Really_Buffer].Param = Calc_Param(i_Set); // �������� ��������� ���������� ���������� i_Set
 Mem_Buffer[Really_Buffer].Priority = Calc_Priority(Mem_Buffer[Really_Buffer].Param); // �������� ����������
//
////////////////////////////////////////////////////////////////////////////////
 t_printf( "-I- %s(): ���������� #%d/%d [%s] ��������� � ����� (%s) -I-",
            __FUNC__, i_Set,Rule, Line_Set(i_Set, 2, 0.0), Get_Time_asLine() );
////////////////////////////////////////////////////////////////////////////////
 Mem_Instruction[i_Set].fAddBuffer = true; // ���� ���������� ���������� � ����� =======
 mS->Cells[6][i_Set+1] = Vizu_Flags(i_Set); // ��������������� ����� ������ ���������� =
////////////////////////////////////////////////////////////////////////////////
//
 Really_Buffer_Old = Really_Buffer; // ��������� ���������� ��������
 Really_Buffer ++ ; // ��������� ������� - ��������� �� ��������� ������� �������
//
//==============================================================================
 Calc_Level_Buffer_Fill(); // ���������� ������ ������������� ������
//
 Test_Visu_Buffer_Fill(); // ������������ ��������� ������������� ������
//
 Vizu_Buffer(); // ������������ ������ ������
//
////////////////////////////////////////////////////////////////////////////////
//
 if( Really_Buffer == max_Buffer ) // ����� ������
 {
  t_printf( "-W- %s(): ����� �����, ������ ���������� ��������� ������ (%s) -W-",
             __FUNC__, Get_Time_asLine());
//
  buffer_Fill = true; // ����� �����
//
  return;
 }
//
} // ----- ����� add_Buffer ----------------------------------------------------


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
float __fastcall
Calc_Priority( float Param ) // �� �������� Param (����������) ������������
// ��������� ������ ���������� ��� �� ������� �� ��������� ���
{
//
 if(!how_Calc_Prior) // ���� ���� - ����� ���������������
  return (Param);
//
 else // ���� �� ���� - ������� ���������������
  {
   if(!Param) // ����� �� ������ �� ����
    Param += 1.0e-9;
   return (1.0 / Param);
  }
//
} // ����� Calc_Priority -------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
float __fastcall
Calc_Param( INT i_Set ) // ��� ���������� i_Set ������������ �� �������� ����������
{
// char Set[_SET_LEN];
 float Param = 1.0; // ���������
//
 if(!how_Calc_Param) // ���� ���� - ��� ����������
  Param = 1.0;
//
 else
 if(how_Calc_Param == 1) // ���� ������� - ��������� ����� =====================
  Param = rand() % 1000;
//
 else  // ���� ������ - ��������������� ������� ���������� ���������� ==========
 if(how_Calc_Param == 2)
  Param = 1.0 * Get_TicksByInstruction(Mem_Instruction[i_Set].Set);
//
 else  // ���� ������ - ����� ����� ���� ����������, ��� ���.��������� ������ ��������� ���� � ����� ��.���������
 if(how_Calc_Param == 3)
  Param = Calc_01_Param_Instruction(i_Set);
//
 else  // ���� �������� - ����� ����� ��������� ���� ����������, ����������� �� ������ � ����������� ������
 if(how_Calc_Param == 4)
  Param = Calc_02_Param_Instruction(i_Set);
//
 else  // ���� ������� - � ������ ����� ��� ���������� ��������� �����.��������� � ���.���������� ������
 if(how_Calc_Param == 5)
  Param = Calc_03_Param_Instruction(i_Set);
//
 return Param; // ������� �������� ����������
//
} // ����� Calc_Param ----------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void __fastcall
Test_Visu_Buffer_Fill() // ��������� � ���������� ���������� ������ ����������� (� ��������)
{
 char tmp[_512];
 float level[]={0.25, 0,50, 0,75, 0,90}, // ������ ������������
       now,old; // ������� � ���������� ���������� ������ ����������
//
 now = Really_Buffer / max_Buffer;
 old = Really_Buffer_Old / max_Buffer;
///
 F1->PB_1->Position = (F1->PB_1->Max - F1->PB_1->Min) *
                       Really_Buffer / max_Buffer; // ��������� ��������
//
 for(int i=0; i<3; i++) // �� ������� level
  {
   if(now >= level[i] && old < level[i]) // ��������  ����� ����� levels[i]
    {
     t_printf( "-I- %s(): ����� �������� ����� ��� �� %.0g %%  (%s) -I-",
                      __FUNC__, 1.0e2*level[i], Get_Time_asLine());
    }
//
   else
   if(now <= level[i] && old > level[i]) // ��������  ���� ����� levels[i]
    {
     t_printf( "-I- %s(): ����� �������� ����� ��� �� %.0g %% (%s) -I-",
                      __FUNC__, 1.0e2*level[i], Get_Time_asLine());
    }
//
  } // ����� �� level
//
} // ����� Test_Visu_Buffer_Fill -----------------------------------------------

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
int __fastcall
Select_Set_fromBuffer() // ���������� ����� ���������� �� ������ �� ������� ��� ����������
{ // �������� ��������� ���������� � ������ � ������������ Really_Buffer
// struct timeb t1; // ��������� ���������� �������
 float Prior = -1.0e20;
 int i_Set_Prior_Max, i_Buffer;
 char tmp[_512];
//
 if(!Really_Buffer)
  {
   t_printf( "-W- %s(): ����� ���� ! (%s) -W-", __FUNC__, Get_Time_asLine());
   return BUFFER_EMPTY;
  }
//------------------------------------------------------------------------------
//
 for(int i=(Really_Buffer-1); i>=0; i--) // �� ����� ������ ������ ���� (����� ������� ������ � ������)
// for(int i=0; i<Really_Buffer; i++) // �� ����� ������ ����� ����� (����� ������� ��������� � ������)
  if(Mem_Buffer[i].Priority >= Prior) // ���� max (Mem_Buffer[i].Priority)...
   {
    i_Set_Prior_Max = Mem_Buffer[i].i_Set; // ��������� ����� ��������� � Mem_Instruction[]
    i_Buffer = i; // ��������� ����� ���������� � ������ Mem_Buffer[]
    Prior = Mem_Buffer[i].Priority;
   }
//
// ����, i_Set_Prior_Max - ���������� � ������������ ����������� ///////////////
//
 t_printf( "-I- %s(): ���������� #%d/%d [%s] (�����. = %.*g) ������� �� ������ ��� ���������� (%s) -I-",
            __FUNC__, i_Set_Prior_Max, i_Buffer,
            Line_Set(i_Set_Prior_Max, 2, 0.0),
            ACC_REAL,Mem_Buffer[i_Buffer].Priority,
            Get_Time_asLine());
//
// ������ ������ ���������� i_Set_Prior_Max �� ������
//
 if(! i_Buffer) // ��� ������ � ������ ����������
  for(UI i=1;  i<Really_Buffer; i++) // �� ���� ���������� � ������
   {
    M_B = Mem_Buffer[i]; // ����� �� 1 � ������ ������...
    Mem_Buffer[i-1] = M_B;
   }
//
 else
 if(i_Buffer == (Really_Buffer-1)) // ��� ��������� � ������ ����������
  ; // ������ �� ������ - ���� ��������� Really_Buffer �� 1
//
 else // ��������� ������
  for(UI i=i_Buffer; i<Really_Buffer; i++) // �� ����������� >= i_Save � ������
   {
    M_B = Mem_Buffer[i+1]; // ����� �� 1 � ������ ������...
    Mem_Buffer[i] = M_B;
   }
//
////////////////////////////////////////////////////////////////////////////////
//
 Really_Buffer -- ;
//
////////////////////////////////////////////////////////////////////////////////
//
 Calc_Level_Buffer_Fill(); // ���������� ������������� ������
//
 Test_Visu_Buffer_Fill(); // ����� ������� ������
//
 Vizu_Buffer(); // ������������ ������ ������
//
 return( i_Set_Prior_Max ); // ������� ����� � Mem_Instruction ���������� � ���������� �����������
//
} // ����� Select_Set_fromBuffer -----------------------------------------------

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void __fastcall
Calc_Level_Buffer_Fill() // ��������� ������������� ������ � %% � ������� Level_Buffer_Fill
{
 char tmp[_512];

 Level_Buffer_Fill = (100.0 * Really_Buffer) / max_Buffer;

 snprintf(tmp,sizeof(tmp), "������� �� ������ ( %d / %d = %.3�%% )",
              Really_Buffer,max_Buffer,Level_Buffer_Fill);
//
} // ����� Calc_Level_Buffer_Fill ----------------------------------------------

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
bool __fastcall
Test_aResult_Eq_aOperand( INT i_Set ) // �� ��������� �� � ���������� i_Set
{ // ����� aResult � �������� ������� ��������� (������� ����������� -
  // ��������� �������� ������������� ������������) ?
 char tmp[_512];
//
 if( !memcmp( Mem_Instruction[i_Set].aResult,attrVar,strlen(attrVar) ) ) // ��������� �� strlen(attrVar) ��������
  return false; // �� aResult �� ���������������� ������� ������������� ������������
//
////////////////////////////////////////////////////////////////////////////////
 switch( Get_CountOperandsByInstruction(Mem_Instruction[i_Set].Set) ) // ����� ������� ��������� ���������� Set
 {
  case 1: if( !strcmp( Mem_Instruction[i_Set].aResult, Mem_Instruction[i_Set].aOp1 ) ) // ��������� ?
           {
            t_printf( "-W- %s(): � ���������� #%d [%s] ����� ���������� ��������� � ������� �������� ��������. ������� ������� ������������ ������������... -W-",
                             __FUNC__, i_Set, Line_Set(i_Set, 2, 0.0));
            return true; // �� - ���� ���������� !!!
           }
          else
           return false;
          break;
//
  case 2: if( !strcmp( Mem_Instruction[i_Set].aResult, Mem_Instruction[i_Set].aOp1 ) || // ���...
              !strcmp( Mem_Instruction[i_Set].aResult, Mem_Instruction[i_Set].aOp2 ) ) // ��������� ?
           {
            t_printf( "-W- %s(): � ���������� #%d [%s] ����� ���������� ��������� � ������� ������ �� ������� ���������. ������� ������� ������������ ������������... -W-",
                             __FUNC__, i_Set, Line_Set(i_Set,2, 0.0));
            return true; // �� - ���� ���������� !!!
           }
          else
           return false;
          break;
//
 default: break;
//
 } // ����� switch
//
} // --- ����� Test_aResult_Equal_aOperand -------------------------------------


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
char* __fastcall
Get_Time_asLine() // ������� �� ��� (� ������ ������� � 0) � ������
{
 static // ����� ������ �� "����������" ������� �� �����
 char tmp[_512];
//
 snprintf(tmp,sizeof(tmp), "������� ������: %d ������", localTick);
 return tmp;
//
} // ����� Get_Time_asLine -----------------------------------------------------


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
int  __fastcall
Get_Free_Proc() // ���������� ����� ��������� ���
{
 int Free_Proc = 0; // ����� ��������� ���
//
 for( INT i=0; i<max_Proc; i++ ) // �� ������ ���
  if( !Mem_Proc[i].Busy ) // ���� ��� ����� i ��������...
   Free_Proc ++ ;
//
 return Free_Proc; // ����� ��������� ���
//
} // ����� Get_Free_Proc -------------------------------------------------------


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void __fastcall
AttemptExecMaxInstructions_fromBuffer()
{ // �������� ������� �� ������ � ��������� �������� ���-���������� ------------
// ����, ����� � ������ Really_Buffer ������ � Free_Proc ��������� ��� ---------
// int Possible_Exec_Set;
//
 if(!Really_Buffer || !Free_Proc) // ������� ������...
  return;
//
 int Possible_Exec_Set = min(Really_Buffer, Free_Proc); // �����������...
//
 for( INT i=1; i<=Possible_Exec_Set; i++ )
  {
   INT i_Set = Select_Set_fromBuffer(); // ������� ���������� � ������������ ����������� �� ������
//   if(i_Set != BUFFER_EMPTY) // ���� ����� �� ����...
   ExecuteInstructions_Except_SET(i_Set); // ��������� ���������� i_Set ( ����� SET )
  }
//
} // ����� AttemptExecMaxInstructions_fromBuffer -------------------------------


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
float __fastcall
Calc_01_Param_Instruction( INT i_Set ) // ����c���� ���������� ���������� i_Set �� 01-�� ������
{ // ���������� ����� ����� ���� ����������, ��� ������� ��������� ������ ��������
// ���� �� ����� �� ������� ��������� (�������, �� �������� ����� ���������)
// ��� ���� ����� ���������� ������� ��������� �� ������������� !
 char aResult[_ID_LEN];
 float Param = 0; // �������� ����������
//
// strcpy(Set, Mem_Instruction[i_Set].Set); // ��������� i_Set ��������� � Set...
 strcpy(aResult, Mem_Instruction[i_Set].aResult); // ����� ���������� ���������� i_Set
//
 for( INT i=0; i<Really_Set; i++ ) // �� ���� �����������...
  {
   if(i == i_Set) // ���� � ����� ���������� �� ���������� ---------------------
    continue;
//
   switch(Get_CountOperandsByInstruction(Mem_Instruction[i].Set)) // �� ����� ��������� ���������� i
    {
     case 1: if(!strcmp(aResult, Mem_Instruction[i].aOp1)) // aResult[i_Set] == aOp1[i]
               Param ++;
             break;
//
     case 2: if(!strcmp(aResult, Mem_Instruction[i].aOp1) || // aResult[i_Set] == aOp1[i] "���"
                !strcmp(aResult, Mem_Instruction[i].aOp2))   // aResult[i_Set] == aOp2[i]
               Param ++;
             break;
    } // ����� swith(Get_CountOperandsByInstruction(Set))
//
  } // ����� �� for()  ���� ����������
//
 return Param;  // �������� ����������
//
} // ����� Calc_01_Param_Instruction -------------------------------------------


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
float __fastcall
Calc_02_Param_Instruction(INT i_Set) // ����c���� ���������� ���������� i_Set �� 02-�� ������
{ // ���������� ����� ����� ������� ��������� ���� ����������, ��� ������� ���������
// ������ �������� ������� ��������� (������� �� �������� ����� ���������)
// ��� ���� ����� ���������� ������� ��������� �� ������������� !
 char aResult[_ID_LEN];
 float Param = 0; // �������� ����������

// strcpy(Set, Mem_Instruction[i_Set].Set); // ��������� i_Set ��������� � Set...
 strcpy(aResult, Mem_Instruction[i_Set].aResult); // ����� ���������� ���������� i_Set
//
 for( INT i=0; i<Really_Set; i++ ) // �� ���� �����������...
  {
   if(i == i_Set) // ���� � ����� ���������� �� ���������� ---------------------
    continue;
//
   switch(Get_CountOperandsByInstruction(Mem_Instruction[i].Set)) // �� ����� ��������� ���������� i
    {
     case 1: if(!strcmp(aResult, Mem_Instruction[i].aOp1)) // aResult[i_Set] == aOp1[i]
               Param ++;
             break;
//
     case 2: if(!strcmp(aResult, Mem_Instruction[i].aOp1))  // aResult[i_Set] == aOp1[i]
               Param ++;
//
             if(!strcmp(aResult, Mem_Instruction[i].aOp2)) // aResult[i_Set] == aOp2[i]
               Param ++;
             break;
//
    } // ����� swith(Get_CountOperandsByInstruction(Set))
//
  } // ����� �� for()  ���� ����������
//
 return Param;  // �������� ����������
//
} // ����� Calc_02_Param_Instruction ---------------------------------------------------


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
float __fastcall
Calc_03_Param_Instruction( INT i_Set ) // ����c���� ���������� ���������� i_Set �� 03-�� ������
{ // ������������ "���" ����������� ���������� =1.0 ������������� ������, �����
// ��������� ���������� ������ �������� ����������� � ��������� ���������� � ����������
// ���� � ����������� ���������� N ������� ���������, �� ������� M ��� ������ �
// ��������� ���������� ������ ��������� ��������� �� ������ � � �� (N-M) ���������,
// �� ���������� ����� K/(N-M) --- ��� K=(N-M) �������� ��� ��������� ����������� ����������
 char aResult[_ID_LEN];
 float Param = 0; // �������� ����������
//
 strcpy(aResult, Mem_Instruction[i_Set].aResult); // ����� ���������� ���������� i_Set
//
 for( INT i=0; i<Really_Set; i++ ) // �� ���� �����������...
  {
   if(i == i_Set) // ���� � ����� ���������� �� ���������� ---------------------
    continue;
//
   switch( Get_CountOperandsByInstruction(Mem_Instruction[i].Set) ) // �� ����� ��������� ���������� i
    {
     case 1: if( !MI_FOP1(i) && // ������ ������� ������� �� ����� "�"
                  MI_AOP1(i) ) // aResult[i_Set] == aOp1[i]
               Param ++;
             break;
//
     case 2: if( !MI_FOP1(i) && // ������ ������� ������� �� ����� "�"
                  MI_FOP2(i) && // ������ ������� ������� ����� "�"
                  MI_AOP1(i) )  // aResult[i_Set] == aOp1[i]
               Param ++ ;
//
             if(  MI_FOP1(i) && // ������ ������� ������� ����� "�"
                 !MI_FOP2(i) && // ������ ������� ������� �� ����� "�"
                  MI_AOP1(i) )  // aResult[i_Set] == aOp2[i]
               Param ++ ;
//
             if( !MI_FOP1(i) && // ������ ������� ������� �� ����� "�"
                 !MI_FOP2(i) )  // ������ ������� ������� �� ����� "�"
              if(!strcmp( Mem_Instruction[i].aOp1, aResult)) // aResult[i_Set] == aOp1[i]
               Param += 0.5 ;
              else
              if(!strcmp(aResult, Mem_Instruction[i].aOp2)) // aResult[i_Set] == aOp2[i]
               Param += 0.5 ;
             break;

    } // ����� swith(Get_CountOperandsByInstruction(Set))
//
  } // ����� �� for()  ���� ����������
//
 return Param;  // �������� ����������
//
} // ����� Calc_02_Param_Instruction ---------------------------------------------------


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void __fastcall
Delay_Vizu_Buffer() // ���� Delay_Buffer �������� ��� ������������ ������
{
 if(!Delay_Buffer) // ����� ���� - ������ �� ������...
  return;
 else
  Delay(Delay_Buffer);
//
} // ����� Delay_Vizu_Buffer ---------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void __fastcall // ���������� �� "����� ������������" �������� ����
TF1::Most_Wonderful(TObject *Sender)
{
// ������������ �������� DATA-FLOW ������������ - ��������� ���������� ��
// ������� �� ������������������ ������������ ���������� !!!
 char tmp[_1024];
//
 strcpy(tmp, "... ����� ������������ � ���, ��� ��� DATA-FLOW ������\n");
 strcat(tmp, "��������� ���������� �� ������� �� ������������������\n");
 strcat(tmp, "���������� (��� ���������� ����������� ����������\n");
 strcat(tmp, "�������, � �� ������������������� ����������).\n\n");
 strcat(tmp, "������� ��������� ������� ���������� ���������� � �����\n");
 strcat(tmp, "��������� ������, ���� ��������� � ���������?");
//
 switch( MessageBox(0, tmp, " ����������/������", MB_YESNO) )
 {
  case IDYES: // ������ ������ Yes
              Start_DF( 1 ); // ����� ���������� � �������������� ����������
              break;
//
  case IDNO:  // ������ ������ No
              break;
 } // ����� switch
//
} //----------------------------------------------------------------------------


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void __fastcall
TF1::On_Master_Timer(TObject *Sender)
{ // ���������� ������ Interval ����������� �������� Master_Timer
//
 char tmp[_512];
 INT i_Proc, i_Set, dt_ticks;
//
////////////////////////////////////////////////////////////////////////////////
 localTick ++ ; // ��������� ����� ����� (����������) �� 1
////////////////////////////////////////////////////////////////////////////////
//
 for(i_Proc=0; i_Proc<max_Proc; i_Proc++) // �� ���� ���
  {
   if( !Mem_Proc[i_Proc].Busy ) // ��� ����� i_Proc ��������
    continue; // ������� � ���������� ���
//
   i_Set = Mem_Proc[i_Proc].i_Set; // ����� ���������� � Mem_Instruction[]
//
   dt_ticks = Get_TicksByInstruction( Mem_Instruction[i_Set].Set ); // ����� ������ ��� ���������� ���������� Set
//
   if( dt_ticks <= 0 ) // �������� ���������� ��� ��� ���������� �� ��������� ���������� 0
    localTick -= dummy_Ticks ;
//
   if( (localTick - Mem_Proc[i_Proc].tick_Start) >= dt_ticks ) // ����� ���������� ���������� �����
   {
    Finalize_Except_SET( i_Proc ); // �� ����������� ���������� ���������� i_Proc
    // ������������� ����� ���������� ��������� � ������ ���������� �, ���� ��� �������� ������,
    // ��������� ������� ���������� � ����� ��������� ��� ������������ ����������
    localTickOfEndLastExecuteSet = localTick; // ����� ��������� ���������� ����������
   }
// '>=' ������ '==' �������� � ����� ��������� ������ ���������� !!!!!!!!!!!!!!!
// �� ������� ��������� ���������� ������ Finalize_Except_SET(i_Proc) - � ���� ���������
// ��� ������ ���������� ����� ���������� ���� Mem_Proc[i_Proc].Busy = FALSE � ��������� continue
//
  } // ����� ����� �� ���
//
 if( localTick - localTickOfEndLastExecuteSet >=
     waitAboveOfEndLastExecuteSet ) // ������ �� ����������� ����� wait... �����
 {
  Master_Timer->Enabled = false; // ��������� ������� ������ (������ ���������� ��� ������� �� �����!!!)
//  MessageBox(0, "����� ���������� ���������", "����������", MB_OK);
  StopCalculations( 1 ); // ���������� ����������� ��-�� �������
 }
//
 return;
//
} // --- ����� On_Master_Timer -------------------------------------------------


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void __fastcall TF1::SG_SetSelectCell(TObject *Sender, int ACol,
                                      int ARow, bool &CanSelect)
{ // ���������� ��� ������ ������ ----------------------------------------------
//
} //----------------------------------------------------------------------------


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void __fastcall // ���������� ��� ��������� ������ ������ ������� SG_Instruction
TF1::SG_Instruction_DrawCell(TObject *Sender, int ACol, int ARow,
                      TRect &Rect, TGridDrawState State)
{
//
 if( Really_Select <= 0 ) // ������ �� ���� �������� ������
  return;
//
 for( INT i=0; i<Really_Select; i++ ) // �� ����������� ������ ���������� �����
 {
//
  if( !Sel_Cell[i].Col && !Sel_Cell[i].Row ) // ���� ��� �������...
   continue;
//
  if( ACol!=Sel_Cell[i].Col || ARow!=Sel_Cell[i].Row ) // ��� �� ������� ������!..
   continue;
//
  if( Sel_Cell[i].clBackground == mS->Canvas->Brush->Color && // ���� ���� ������ ������ �� ����...
      Sel_Cell[i].clSymbol     == mS->Canvas->Font->Color ) // ���� ����� ������ ������ �� ����...
   continue;
//
  mS->Canvas->Brush->Color = Sel_Cell[i].clBackground; // ��������� ���� ���� ������
  mS->Canvas->FillRect( Rect ); // ��������� ������� ������
//
  mS->Canvas->Font->Color  = Sel_Cell[i].clSymbol; // ��������� ���� �����
  mS->Canvas->TextOut( Rect.Left + 2, Rect.Top + 2, mS->Cells[ACol][ARow] ); // ������������ �����
//
 } // ����� ����� for( INT i=0 ... �� ����������� ������� Sel_Cell[] -----------
//
} //----------------------------------------------------------------------------


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void __fastcall TF1::DrawNotExecuteSets(TObject *Sender)
{ // ��������������� ��������������� ����������
//
 Clear_AllTableInstructions(); // ������� ����� ���� ����� ������� ����������
//
 Really_Select = 0; // ������� ����������� � Sel_Cell ����� (������)
//
 for( INT i=0; i<Really_Set; i++ ) // �� ���� ������� Mem_Instruction[]
 {
  if( !Mem_Instruction[i].fExecOut ) // ���� ���������� �� ����������� �� ����...
  {
   Sel_Cell[Really_Select].Col = 6;
   Sel_Cell[Really_Select].Row = i + 1;
   Sel_Cell[Really_Select].clBackground = DCT.clNonExecuted;
   Sel_Cell[Really_Select].clSymbol     = clBlack;
//
   Really_Select ++ ;
  }
 } // ����� �� ���� ������� Mem_Instruction[i] -----------------------------------------
//
 mS->Repaint();
//
 char tmp[_512];
 snprintf(tmp,sizeof(tmp), " �������� %d �������/�...", Really_Select);
 SBM0->Text = tmp; // ����� ������ � StatusBarMain
//
} //----------------------------------------------------------------------------


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void __fastcall TF1::DrawNotUsedResults(TObject *Sender)
{ // ����������������� ����������, ��������� ���������� �������
// �� ������������ �������� ������� ������������ � �������� ������� ���������
 char aResult[_ID_LEN]; // ��������� ���������� � ��� ����������
 INT n_Op;
//
 Clear_AllTableInstructions(); // ������� ����� ���� ����� ������� ����������
//
 Really_Select = 0; // ������� ����������� � Sel_Cell[] ����� (������)
//
 for( INT i=0; i<Really_Set; i++ ) // �� ���� ������� Mem_Instruction[]
 {
  strcpy(aResult, Mem_Instruction[i].aResult); // ��������� ��� ���������� ���������� ����������
//
  for( INT j=0; j<Really_Set; j++ ) // ... ����� �� �� ���� ������� Mem_Instruction[] ��� ������ �� ���������
  {
   if( is_SET( Mem_Instruction[j].Set ) ) // ���������� SET �� ������������� !!!
    continue;
//
   n_Op = Get_CountOperandsByInstruction(Mem_Instruction[j].Set); // ����� ��������� ���������� j
//
   if( n_Op == 1 && // ���� ���� �������... "�"
       MI_AOP1(j) ) // aResult ����� aOp1
    goto end_i;
//
   if( n_Op == 2 && // ���� ��� ��������... "�"
       (
        MI_AOP1(j) || // aResult ����� aOp1 "���"
        MI_AOP2(j) // aResult ����� aOp2
       )
     )
    goto end_i;
//
  } // ����� ����������� ����� �� Mem_Instruction[j]
//
// ���� ��� ����������, ��������� ���������� ���� ������ � Sel_Cell[] ��� ���������
//
   Sel_Cell[Really_Select].Col = 4;
   Sel_Cell[Really_Select].Row = i + 1;
   Sel_Cell[Really_Select].clBackground = DCT.clNonUsedResult;
   Sel_Cell[Really_Select++].clSymbol   = clBlack;
//
end_i: // � ����� ����� �� i
//
 } // ����� �������� ����� �� Mem_Instruction[i]
//
 mS->Repaint();
//
 char tmp[_512];
 snprintf(tmp,sizeof(tmp), " �������� %d �������/�...", Really_Select);
 SBM0->Text = tmp; // ����� ������ � StatusBarMain
//
} //----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void __fastcall TF1::DrawNotDetermineOperands(TObject *Sender)
{ // ��������������� ��������, �� ������������ ������������ ����������
 char aOp1[_ID_LEN],
      aOp2[_ID_LEN],
      aResult[_ID_LEN]; // ��������� ���������� � ��� ����������
 bool flag_1,   flag_2,
      flag_1_1, flag_1_2, flag_2_2;
 INT n_Op;
//
 Clear_AllTableInstructions(); // ������� ����� ���� ����� ������� ����������
//
 Really_Select = 0; // ������� ����������� � Sel_Cell ����� (������)
//
 for( INT i=0; i<Really_Set; i++ ) // �� ���� ������� Mem_Instruction[]
 {
  if( is_SET( Mem_Instruction[i].Set ) ) // ���������� SET �� ���������������...
   continue;
//
  flag_1 = flag_2 = false; // ����� ������� ���������� � 1-� ��� 2-�� ����������
//
  n_Op = Get_CountOperandsByInstruction(Mem_Instruction[i].Set); // ����� ��������� ���������� i
//
  if(n_Op == 1) // ���� ���������� � 1-� ���������!
   flag_1 = true;
//
  if(n_Op == 2) // ���� ���������� � 2-�� ���������!
   flag_2 = true;
//
  strcpy(aOp1, Mem_Instruction[i].aOp1); // ��������� ����� ��������� ���������� ����� i
  strcpy(aOp2, Mem_Instruction[i].aOp2);
//
  flag_1_1 = flag_1_2 = flag_2_2 = false; // ����������������� �����
//
  for( INT j=0; j<Really_Set; j++ ) // ... ����� �� �� ���� ������� Mem_Instruction[] ��� ������ �� �����������
  {
   strcpy(aResult, Mem_Instruction[j].aResult); // ��������� ��� ���������� ���������� ����������
//
   if(n_Op == 1 && !strcmp(aOp1, aResult)) // ���������� � 1-� ��������� �� ������
    flag_1_1 = true;
//
   if(n_Op == 2 && !strcmp(aOp1, aResult)) // ���������� � 1-� ��������� �� ����
    flag_1_2 = true;
//
   if(n_Op == 2 && !strcmp(aOp2, aResult)) // ���������� �� 2-� ��������� �� ����
    flag_2_2 = true;
//
  } // ����� ����������� ����� �� Mem_Instruction[j]
//
// ���� ��� ����������, ��������� ���������� ���� ������ � Sel_Draw[] ��� ���������
//
   if( (flag_1 && !flag_1_1) || // ���� ���������� � 1-� ��������� "�" ������ ������� (�� ������)
       (flag_2 && !flag_1_2) )  // ���� ���������� � 2-�� ���������� "�" ������ ������� (�� ����)
   {
    Sel_Cell[Really_Select].Col = 2; // ���������� ������� � aOp1
    Sel_Cell[Really_Select].Row = i + 1;
    Sel_Cell[Really_Select].clBackground = DCT.clNonDefOperands;
    Sel_Cell[Really_Select++].clSymbol   = clBlack;
   }
//
   if(flag_2 && !flag_2_2) // ���� ���������� � 2-�� ���������� "�" ������ ������� (�� ����)
   {
    Sel_Cell[Really_Select].Col = 3; // ���������� ������� � aOp2
    Sel_Cell[Really_Select].Row = i + 1;
    Sel_Cell[Really_Select].clBackground = DCT.clNonDefOperands;
    Sel_Cell[Really_Select++].clSymbol   = clBlack;
   }
//
 } // ����� �������� ����� �� Mem_Instruction[i]
//
 mS->Repaint();
//
 char tmp[_512];
 snprintf(tmp,sizeof(tmp), " �������� %d �������/�...", Really_Select);
 SBM0->Text = tmp; // ����� ������ � StatusBarMain
//
} //----------------------------------------------------------------------------


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void __fastcall Draw_AllTableInstructions()
{ // ��������������� �������� ���������� ��������� DATA-FLOW �����������
// � �������� ������������ (����� �������������) ����� ���������� ������� Mem_Instruction[]
//
 if( !DCE.needDrawColors ) // �� ���� ������ ������������...
  return;
//
 Really_Select = 0; // ������� ����������� � Sel_Cell[] ����� (������)
//
 for( INT i=0; i<Really_Set; i++ ) // �� ���� ������� Mem_Instruction[]
 {
  if( MI_FOP1(i) ) // ���� 1-� ������� �����...
  {
   Sel_Cell[Really_Select].Col = 2; // ������� �������-1
   Sel_Cell[Really_Select].Row = i + 1;
   Sel_Cell[Really_Select].clBackground = DCE.clReadyOperand;
   Sel_Cell[Really_Select++].clSymbol   = clBlack;
  }
//
  if( MI_FOP2(i) && // ���� 2-� ������� �����...
      Get_CountOperandsByInstruction( Mem_Instruction[i].Set ) == 2 ) // ���� ��� �������� � ����������
  {
   Sel_Cell[Really_Select].Col = 3; // ������� �������-2
   Sel_Cell[Really_Select].Row = i + 1;
   Sel_Cell[Really_Select].clBackground = DCE.clReadyOperand;
   Sel_Cell[Really_Select++].clSymbol   = clBlack;
  }
//
  if( Mem_Instruction[i].fExecOut ) // ����������, ������� ����������� 1 ��� (����� ���������)
  {
   Sel_Cell[Really_Select].Col = 4; // ������� ���������
   Sel_Cell[Really_Select].Row = i + 1;
   Sel_Cell[Really_Select].clBackground = DCE.clExecSet,
   Sel_Cell[Really_Select++].clSymbol   = clBlack;
  }
//
  if( SpeculateExec && // ����� ����� �������������� ����������
      Mem_Instruction[i].fSpeculateExec ) // ���� ������ ���������� ������������ ������������� ���������
  {
   Sel_Cell[Really_Select].Col = 4; // ������� ���������
   Sel_Cell[Really_Select].Row = i + 1;
   Sel_Cell[Really_Select].clBackground = DCE.clSpeculateExec,
   Sel_Cell[Really_Select++].clSymbol   = clBlack;
  }
//
  if( !is_PredicatOrSET( Mem_Instruction[i].Set ) && // ��� ���������� - �� ��������
       Mem_Instruction[i].fPredicat_TRUE ) // ���� ��������� TRUE
  {
   Sel_Cell[Really_Select].Col = 5; // ������� ��������
   Sel_Cell[Really_Select].Row = i + 1;
   Sel_Cell[Really_Select].clBackground = DCE.clTruePredicat;
   Sel_Cell[Really_Select++].clSymbol   = clBlack;
  }
//
 } // ����� ����� �� ������� Sel_Cell[] for( INT...
////////////////////////////////////////////////////////////////////////////////
  mS->Repaint(); // �����! �� ����� ������ ������� ���������������� (� ���������������� �������)
////////////////////////////////////////////////////////////////////////////////
//
} //----- ����� Draw_AllTableInstructions ---------------------------------------------


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void __fastcall TF1::OnShow_F1(TObject *Sender)
{ // ���������� ��� ��������� ������� �����
//
 Read_Config( 0 ); // ���������� ���� ������������
//
 F1->Resize(); // ������������� �������������� ������� ���� ��� ���������� ��������� ��������� ���� Control's
//
 if( ParamCount() == 4 ) // ���� ������ ����� ���������� ��������� ������
 {
  strcpy(FileNameSet, ParamStr(1).c_str()); // ��� ����� ���������� (� �������)
  max_Proc        = StrToInt(ParamStr(2).c_str()); // ����� ��� (��� ������������� - �������� ������ �� F1->E_AIU->Text)
  F1->E_AIU->Text = ParamStr(2).c_str(); // ������ �� ����� (�� ������ ������ ������)
  how_Calc_Param  = StrToInt(ParamStr(3).c_str()); // ��� ��������� ��������� ����������
  how_Calc_Prior  = StrToInt(ParamStr(4).c_str()); // ��� ��������� ���������� ���������
//
  Write_Config(); // ��������� � INI-���� ������ ��� ����������� ���������
//
  BitBtn_Run->Click(); // ���������� ������ ������ �ר� !!!
//
  do_Stop // "���������" ��� ������ ����������
 } // �����  if ParamCount...
//
  SessionStartTime = (__int64) ( ((double)TDateTime::CurrentDateTime()-365.0*100) * 365.0*24*60*60*1000 ); // ������ 100*365 ���� (��������� � ����)...
  Work_LogInOut( 0 ); // �������� � ������ ������ ��������� DATA_FLOW.EXE (��������� "LogIn")
//
} //----- ����� F1_OnShow ------------------------------------------------------


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
REAL __fastcall
Calc_ConnectedIndex (int Rule ) // ��� Rule # 0 ������ �������� � ���� ��������a
{ // ����������� ������ ��������� (����������� ���-�� ����������, ��������� �� ���������)
// �� ���������� ���������� ��������������� � �������������� ����� ���������
// ����� �����, ��������� � �������� - ������� �������
 char aResult[_ID_LEN];
 int n_Op, // ����� ������� ��������� � ����������
     n_EqResOp, i_EqResOp,  // ����� ����������, ��������� (�� ����� "aResult->aOpN") �� ������
     n_Repeat,  i_Repeat; // ������� ����� ����� �� ��������� n_EqResOp
 int Sum_EqResOp = 0, // ����� ����� n_EqResOp
     Sum_Repeat = 0, // ����� ����� n_Repeat
     Sum_Repeat_1 = 0, // ����� ����� n_Repeat ��� n_ResOp >= 2
     n_RepeatEq_1 = 0; // ����� �������� ������ "1->1"
 char tmp[_512]; // ������� ������
//
 TStringList *CI = new TStringList(); // ������� ����� ����� CI ��� ���������� ������� ���������
//
 CI->Sorted = false; // �� ����������� ������
 CI->Duplicates = dupAccept; // ���������� ������ ���������
//
 for( INT i=0; i<Really_Set; i++ ) // �� ���� ������� Mem_Instruction[i]
 {
  if( is_SET( Mem_Instruction[i].Set ) ) // ���������� SET �� ���������������...
   continue;
//
  n_EqResOp = 0; // ����� ����������, ��������� �� ��������� ���������� ������� (i-����)
//
  strcpy(aResult, Mem_Instruction[i].aResult); // ��������� ����� aResult ��� ���������� i
//
  for( INT j=0; j<Really_Set; j++ ) // ... ����� �� �� ���� ������� Mem_Instruction[] ��� ������ �� ���������
  {
//
   if( is_SET( Mem_Instruction[j].Set ) ) // ���������� SET �� ������������� !!!
    continue;
//
   n_Op = Get_CountOperandsByInstruction(Mem_Instruction[j].Set); // ����� ��������� � ����������  j
//
   if(n_Op == 1) // ���� �������...
   {
    if(!strcmp(aResult, Mem_Instruction[j].aOp1))
     n_EqResOp ++ ;
   }
   else
   if(n_Op == 2) // ��� ��������...
   {
    if(!strcmp(aResult, Mem_Instruction[j].aOp1))
     n_EqResOp ++ ;
    if(!strcmp(aResult, Mem_Instruction[j].aOp2))
     n_EqResOp ++ ;
   }
//
  } // ����� ����������� ����� �� Mem_Instruction[j] ///////////////////////////////////
//
// ����, � ����� ������� n_EqResOp ���������� (����� ���� = 0) !!!!!
//
  if(!n_EqResOp) // ���� ��� ����������� �� aResult - �� ��� �������� ������...
   continue; // ����������� ���� �� i ... ����� ���������� ������ �� ���������� !!!
//
// ���������, ������� �� � CI ������ � ����� �� n_EqResOp ...
  bool i_flag = false; // ������� ����, ��� ������� ������ � i_EqResOp = n_EqResOp
//
  for( INT k=0; k<CI->Count; k++ ) // �� ���� ������� CI (�������� ����, ��� CI->Count = 0)
  {
   sscanf(CI->Strings[k].c_str(), "%10d%10d", &i_EqResOp, &i_Repeat); // ��������� ������ CI
//
   if(i_EqResOp == n_EqResOp) // ������� �������� i_EqResOp, ������ n_EqResOp
   {
    i_Repeat ++ ;
    snprintf(tmp,sizeof(tmp), "%10d%10d%30c", n_EqResOp, i_Repeat, 32); // + 30 ��������!
    CI->Delete(k); // ������� "������" ������ � �������� k
    CI->Insert(k, tmp); // �������� "�����" ������ � �������� k
    i_flag = true; // ... ��, ��� ������ i_EqResOp == n_EqResOp ...
   }
  } // ����� ����� �� k
//
// � ���� � CI ��� ������ � n_EqResOp ? ����� ������� ����� ������ ��� n_Repeat = 1
  if(!i_flag) // ������ � n_EqResOp �� ������� - ������� � !
  {
//   snprintf(tmp,sizeof(tmp), "%10d%10d%30c%s", n_EqResOp, 1, 32, aResult); // i_Repaint=1 // + 30 ��������!
   snprintf(tmp,sizeof(tmp), "%10d%10d%30c", n_EqResOp, 1, 32); // i_Repeat=1 // + 30 ��������!
   CI->Add(tmp); // �������� ������ � CI
  }
//
 } // ����� �������� ����� �� Mem_Instruction[i] ///////////////////////////////
//
// ������ ���������� ���������� ------------------------------------------------
// ��������� ����� ����� ������� = �����(n_Repeat) ����� ����� � n_EqResOp=0
//
  for( INT k=0; k<CI->Count; k++ ) // �� ���� ������� CI, ����� ����� � n_EqResOp=0
  {
//    Sum_EqResOp  += atoi(Get_SubString(CI->Strings[k].c_str(), 1,10));
   Sum_Repeat += atoi(GetSubString(CI->Strings[k].c_str(), 11,20)); // ����� �����
//
   if(atoi(GetSubString(CI->Strings[k].c_str(), 1,10)) >= 2)
    Sum_Repeat_1 += atoi(GetSubString(CI->Strings[k].c_str(), 11,20)); // ����� n_ResOp >= 2
  }
//
// ���������� ������ ��� ���������� ����������� --------------------------------
  for( INT k=0; k<CI->Count; k++ ) // �� ���� ������� CI
  {
   sscanf(CI->Strings[k].c_str(), "%10d%10d", &i_EqResOp, &i_Repeat); // ��������� ������ CI
   snprintf(tmp,sizeof(tmp), "%10d%10d%10.3f%10.3f%%", i_EqResOp, i_Repeat, // �������� ������
                 1.0*i_Repeat/Sum_Repeat, 1.0e2*i_Repeat/Sum_Repeat);
   CI->Delete(k); // ������� ������ � �������� k
   CI->Insert(k, tmp); // �������� ������ � �������� k
  }
//
// ������� ������ ��������� ��������������� ����� ��������� (�������� �������� ������)
//
  for( INT k=0; k<CI->Count; k++ ) // �� ���� ������� CI
  {
   if(atoi(GetSubString(CI->Strings[k].c_str(), 1,10)) == 1) // ������ � n_ResOp = 1
    n_RepeatEq_1 = atoi(GetSubString(CI->Strings[k].c_str(), 11,20));
  }
//
  REAL index;
  if (n_RepeatEq_1)
   {
    index = (REAL) Sum_Repeat_1 / n_RepeatEq_1;
    snprintf(tmp,sizeof(tmp), "���������� ���������������� = %.3f", index);
    CI->Add(tmp);
   }
  else
   index = ERR_ALTERNAT; // ������� �������������
//
////////////////////////////////////////////////////////////////////////////////
//
  CI->Sorted = true; // ����������� (�� ������� ���� - n_EqResOp) � ������� �����������
//
// �e���� ������ ������ (������ = 0) ������������� n_ResOp = 1 etc etc etc
//
  if(Rule) // ��������� � ����
  {
   char FileNameConnectIndex[_512]; // ���� � ����� �������� ���������
   strcpy(FileNameConnectIndex, ChangeFileExt(FileNameSet, ".coi").c_str());  // ���� � COI-�����
   CI->SaveToFile(FileNameConnectIndex);
  }
//
  delete CI; // ���������� ����� ����� CI
//
  return index;
//
} // ----- ����� ������������ Calc_ConnectedIndex ------------------------------


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
REAL __fastcall
StrToReal( char *str, INT i_Set )
{ // ������������ ������ � ������������ ����� � ��������� ������������ �����������
 REAL out;
 char tmp[_512];
//
 try
 {
  out = StrToFloat( AnsiString(str) );
 }
 catch (Exception* exception)
 {
  snprintf(tmp,sizeof(tmp), "������ ����������� ������ '%s' � ������������ ����� � ���������� #%d (�������� %.*g). \n\n��������� ��������������",
                str, i_Set, ACC_REAL, out);
  strcat(tmp, "  �� ���������, �� �� ������������� � ����������� ��������� ����������� ������ ��� ������� !..");
  MessageBox(0, tmp, "��������������", MB_OK | MB_ICONWARNING | MB_TOPMOST);
 }
//
 return out;
//
} // ----- ����� ������� StrToReal ---------------------------------------------

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
bool __fastcall Test_All_Operands()
{ // ��������� ��� ���������� �� ������������� ������������� ���������
// ���� ������ ������������ ���� ����������� ������ ���������
// ��� ������� ���������� ���������� ������������ 1, ��� ������ ���������� - 7
 char Set[_SET_LEN], // ��������� ����������
      aOp1[_ID_LEN],
      aOp2[_ID_LEN],
      aResult[_ID_LEN], // ������� 1, ������� 2, ���������
      str[_512],
      fmt1[] = "\n-E- Test_All_Operands(): � ���������� #%d [%s %s, %s] ������� [%s] ������������� ���������� -E-\n", // ������ ������� (1 �������)
      fmt2[] = "\n-E- Test_All_Operands(): � ���������� #%d [%s %s, %s, %s] ������� #%d [%s] ������������� ���������� -E-\n"; // ������ ������� (2 ��������)
 int nInputOperands; // ����� ������� ��������� ����������
 bool flagOp1,flagOp2, flagAll=true; // ����� ������������ ���������
//
 for( INT i=0; i<Really_Set; i++ ) // �� ���� ����������� � ������
 {
  strcpy( Set,Mem_Instruction[i].Set ); // ��������� ��������� i
//
  if( is_SET( Set ) ) // ���� ��� SET...
   continue; // ���������� ���� �� i
//
  strcpy( aOp1,Mem_Instruction[i].aOp1 ); // ������� 1 ���������� i �������� � aOp1
  strcpy( aOp2,Mem_Instruction[i].aOp2 ); // ������� 2 ���������� i �������� � aOp2
  nInputOperands = Get_CountOperandsByInstruction( Set ); // �������� ����� ��������� ���������� i
//
  flagOp1 = flagOp2 = false; // ������������� �������, ��� ������������ ���...
//
  for( INT j=0; j<Really_Set; j++ ) // �� ���� ����������� � ������
  {
   if( j==i && !is_SET( Set ) ) // ���� � ����� �� ����������... �� � SET ���� ����������!
    continue; // ���������� ���� �� j
//
   strcpy( aResult,Mem_Instruction[j].aResult ); // ��������� ���������� j �������� � aResult
//
   switch( nInputOperands ) // ����� ������� ��������� ���������� Set
   {
    case 1: if( !strcmp( aOp1,aResult )) // ���� aOp1 = aResult -> Ok ������� 1
             flagOp1 = true; // ������� 1 ��������
     break;
//
    case 2: if( !strcmp( aOp1,aResult )) // ���� aOp1 = aResult -> Ok ������� 1
             flagOp1 = true; // ������� 1 ��������
//
            if( !strcmp( aOp2,aResult )) // ���� aOp2 = aResult -> Ok ������� 1
             flagOp2 = true;  // ������� 2 ��������
     break;
//
    default: break;
//
   } // ����� switch( nInputOperands )
//==============================================================================
  } // ����� ����� �� j
//==============================================================================
// ������� ��������� -----------------------------------------------------------
//
   switch( nInputOperands ) // ����� ������� ��������� ���������� Set
   {
    case 1: if( !flagOp1 )
            {
             flagAll = false; // ���� ���� ���� ��������������..!
             t_printf( fmt1, i, Set,Mem_Instruction[i].aOp1, Mem_Instruction[i].aResult, Mem_Instruction[i].aOp1 );
            }
            break;
//
    case 2: if( !flagOp1 )
            {
             flagAll = false; // ���� ���� ���� ��������������..!
             t_printf( fmt2, i, Set,Mem_Instruction[i].aOp1, Mem_Instruction[i].aOp2,Mem_Instruction[i].aResult, 1,Mem_Instruction[i].aOp1 );
            }
//
            if( !flagOp2 )
            {
             flagAll = false; // ���� ���� ���� ��������������..!
             t_printf( fmt2, i, Set,Mem_Instruction[i].aOp1,Mem_Instruction[i].aOp2,Mem_Instruction[i].aResult, 2,Mem_Instruction[i].aOp2 );
            }
            break;
//
    default: break;
//
    } //  // ����� switch( nInputOperands )
//
 } // ����� ����� �� i
//
//==============================================================================
//
 if( flagAll ) // �� ���� �������...
  return true;  // ��� ������ -> ��������� ������!
//
  F1->Master_Timer->Enabled = false; // ���������� ������� ������
//
  snprintf( str,sizeof(str), "%s(): %s", __FUNC__,
"� ��������� ����������� ��������, ������������� �� ������� ����������� \
���������� ������������ ������ ��� ����� ���������; \
��. ������ � ���� ����������������.\n\n\
���������� ���������� ��������� (�������������) ��� ���������� \
(�� ���� ����� � ����)..?" );
//
  if( MessageBox(0, str, "���������� / ����������?",
                          MB_YESNO | MB_ICONQUESTION | MB_APPLMODAL ) == IDYES )
   return false; // ����� ����������
  else
  {
   F1->Master_Timer->Enabled = true; // ������������ ������� ������
   return true; // ��� ������... ��������� ������!
  }
//
} // ---- Test_All_Operands ----------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void __fastcall TF1::Show_Graph(TObject *Sender)
{ // �������� ������ ������������� ����������
//
 if( Regim != 2 || // ���� ��������� �� ����������� ��������� (������ �� F6)
     parallel_Ticks <= 0 ) // ����� ���������� �� �������������
 {
  MessageBeep( MB_OK ); // �������� ��������������...
  return;
 }
//
 char str[_512];
//
 F2->Chart_IC->Title->Text->Clear(); // ������� TSting ����� ����� �����������
 F2->Chart_IC->Title->Text->Add( "����������� ���������� ����� ������������ �������������" );
 F2->Chart_IC->Title->Text->Add( "���������� � ������� ������� (� ������) ���������� ���������" );
 F2->Chart_IC->Title->Text->Add( "\0" );
//
 F2->Chart_IC->Foot->Text->Clear(); // ������� TSting ����� ����� �����������
 snprintf( str,sizeof(str), "������������ ����������: %d ������, ��������� = %.*g", parallel_Ticks, ACC_REAL, 1.0 * serial_Ticks / parallel_Ticks );
 F2->Chart_IC->Foot->Text->Add( str );
//
 snprintf( str,sizeof(str), "����� ������������� %d ����/� ��� �� %d ���������", all_maxProcs, max_Proc );
 F2->Chart_IC->Foot->Text->Add( str );
//
 snprintf( str,sizeof(str), "�� ��� ������������ �� ����� %d ����/� ���", simult_maxProcs );
 F2->Chart_IC->Foot->Text->Add( str );
//
 F2->Top  = F1->Top  + 40;
 F2->Left = F1->Left + 40;
//
 F2->Width  = 470; // ����� ��������� �����...
 F2->Height = 2 * F2->Width / 3 ;
//
 F2->Show(); // �������� ���� ����� F2 � ����������� ������
//
 F2->Caption = " ������� ������������� ���������� (��������� ";
 F2->Caption = F2->Caption + AnsiUpperCase(FileNameSet) + ")"; // �������� ��� ����� ���������
//
} //----------------------------------------------------------------------------


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void __fastcall TF1::OnKeyPress_F1(TObject *Sender, char &Key)
{ // ���������� ��� ������� ������� � F1 (���� ����� ���)
//
 if( Key == VK_RETURN ) // ������ Enter
 {
  Key = NULL;
//
  Write_Config(); // ���������� ���� ������������
  max_Proc = StrToInt(F1->E_AIU->Text); // �������� � ����� (������)
  Out_Data_SBM1(); // ����� ������ � ������� ����� StatusBar -------------------
 }
//
 else
//
 if( !( isdigit( Key ) || Key == VK_BACK || Key == VK_ESCAPE ) )
 {
  Key = NULL; // ������ ����� '0-9', BackSpace, Esc �� ����������..!
//
  Beep( 440, 100 ); // �������� ��������������...
  Beep( 880, 150 );
  Beep( 2048, 100 );
 }
//
} //----------------------------------------------------------------------------


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void __fastcall Out_Data_SBM1()
{ // ������� ������ � ����������� ����� StatusBar (��� ������ - �������) -------
 char tmp[_512];
//
 snprintf(tmp,sizeof(tmp), " ���/�����./����./���./���� (�����.) = %d/%d/%d/%d/%d (%d|%d/%d)",
              max_Proc, max_Instruction, max_Data, max_Buffer, F1->Master_Timer->Interval,
              how_Calc_Param, how_Calc_Prior, pass_Counts);
 SBM1->Text=tmp;
} // ------ ����� Out_Data_SBM1 ------------------------------------------------


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
char* __fastcall strReplace( char* dest, int num, const char* source, const char* orig, const char* rep )
{ // dest  - new string (dest), num - sizeof(dest)-1
//   source - source string for replace, orig - source substring, rep - dest substring
  const char* ptr;
  size_t len1 = strlen(orig), len2 = strlen(rep);
  char*  tmp  = dest;
//
  num -= 1;
//
  while( (ptr = strstr(source, orig)) != NULL )
  {
   num -= (ptr - source) + len2;
   if( num < 1 )
     break;
//
   strncpy( dest, source, (size_t)(ptr - source) );
   dest += ptr - source;
   strncpy(dest, rep, len2);
   dest += len2;
   source  = ptr + len1;
  } // ����� while
//
  for( ; (*dest = *source) && (num > 0); --num)
  {
   ++dest;
   ++source;
  }
//
  return tmp;
//
} //  --------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
bool __fastcall is_ResultIsPredicat( char* str )
{ // ���������� TRUE, ���� str ���� ��������� ���������� ����������� ����������
//
 for( INT i=0; i<Really_Set; i++ ) // �� ���� ����������� � ��������� // (�������� � ����� � ������)
 {
  if( is_Predicat( Mem_Instruction[i].Set ) &&  // ��� ���������� - �������� ?
     !strcmp( Mem_Instruction[i].aResult,str ) ) // ��� str ��������� � ������ ����������
   return true;
//
 } // ����� for(UI i...
//
  return false;
//
} // ----- ����� is_Predicat --------------------------------------------------


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
int __fastcall
Get_CountOperandsByInstruction(char *Set)
{ // ���������� ����� ������� ��������� � ���������� � ���������� Set
 char tmp[_256];
//
 for( INT i=0; i<Count_Sets; i++ ) // �� ������ ���������� ...
  if( !strcmp( Set_Params[i].Set, Set ) ) // ��� ���������� � ���������� Set
   return Set_Params[i].nInputOperands ;
//
// �� �����... ����������� �� ���� !!! /////////////////////////////////////////
  t_printf( "-E- %s() �� ����� � Set_Params[] �������� ��� ���������� '%s' ������ (-13) -E-",
                   __FUNC__, Set);
//
  return (-13); // ��� ����� ���������� ...
//
} // ---------------------------------------------------------------------------


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
int __fastcall // ���������� �� Set_Params[] ����� ���������� ���������� � ���������� Set � �����
Get_TicksByInstruction(char *Set)
{ // ���� ��������� �� �������, ������������ 100 (�����) .......................
 char tmp[_1024];
//
 for( INT i=0; i<Count_Sets; i++ ) // ���� ����� �������� ����������������� ��������
 {
  if( !strcmp( Set_Params[i].Set, Set ) )
   return Set_Params[i].Ticks > 0 ? Set_Params[i].Ticks : 0 ; // return 0 ��� Set_Params[i].Ticks <= 0
 }
//
// �� �����... ����������� �� ���� !!! /////////////////////////////////////////
  snprintf(tmp,sizeof(tmp), "- W - %s() �� ����� � Set_Params[] ������� ���������� ���������� %s. ������� 100 ����� - W -",
                __FUNC__, Set);
  MessageBox(0, tmp, "������ !", MB_OK);
  AddLineToProtocol(tmp);
//
  return ( 100 ); // ... default ...
} // ---------------------------------------------------------------------------


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
bool __fastcall is_Predicat(char* Set)
{  // ����������  TRUE, ���� Set - ��������� ����������-���������
 if ( !strcmp( Set, "PGE" ) || // ���������� PGE
      !strcmp( Set, "PLE" ) || // ...
      !strcmp( Set, "PEQ" ) ||
      !strcmp( Set, "PNE" ) || // ...
      !strcmp( Set, "PGT" ) ||
      !strcmp( Set, "PLT" ) ||
      !strcmp( Set, "PNT" ) || // ���������
      !strcmp( Set, "POR" ) || // OR (����������) ��� ���� ����������
      !strcmp( Set, "PAN" ) || // AND (����������) ��� ���� ����������
      !strcmp( Set, "PIM" ) || // ����������, ���������� ���� ����������
      !strcmp( Set, "PEV" ) || // ������������ ���� ����������
//
      !strcmp( Set, "PAP" ) || // ������� ����� ���� ����������
      !strcmp( Set, "PHS" ) || // ����� ������� ���� ����������
      !strcmp( Set, "PRM" ) || // �������� ���������� ���� ����������
      !strcmp( Set, "PXR" ) )  // ������� ���������� (����������� ���) ���� ����������
//      !strcmp( Set, SET ) ) // ��� "SET"
  return true;
 else
  return false;
//
} // ------ ����� is_Predicat -------------------------------------------------


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void __fastcall Clear_AllTableInstructions()
{  // ������� ��� ������ ������� ����������
//
 Really_Select = 0; // ����� ���������� ����� - �������
 mS->Repaint(); // ������� ���������� ���������...
//
} // ------ ����� Clear_AllTableInstructions -----------------------------------------------


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void __fastcall TF1::SelectColors(TObject *Sender)
{  // ����� ������ ��� ��������� � ������� ����������
 outGlobal = ((TComponent *)Sender)->Tag; // ����� �� ��������� MenuItem ������ ��� ���������
//
 switch( outGlobal ) // ����� �� �������� outGlobal
 {
  case 11: CD_1->Color = DCT.clOperandOperation;
           if( CD_1->Execute() )
            DCT.clOperandOperation = CD_1->Color;
           break;
  case 12: CD_1->Color = DCT.clNonExecuted;
           if( CD_1->Execute() )
            DCT.clNonExecuted = CD_1->Color;
           break;
  case 13: CD_1->Color = DCT.clNonUsedResult;
           if( CD_1->Execute() )
            DCT.clNonUsedResult = CD_1->Color;
           break;
  case 14: CD_1->Color = DCT.clNonDefOperands;
           if( CD_1->Execute() )
            DCT.clNonDefOperands = CD_1->Color;
           break;
// =============================================================================
  case 21: CD_1->Color = DCE.clReadyOperand;
           if( CD_1->Execute() )
            DCE.clReadyOperand = CD_1->Color;
           break;
  case 22: CD_1->Color = DCE.clTruePredicat;
           if( CD_1->Execute() )
            DCE.clTruePredicat = CD_1->Color;
           break;
  case 23: CD_1->Color = DCE.clExecSet;
           if( CD_1->Execute() )
            DCE.clExecSet = CD_1->Color;
           break;
  case 24: CD_1->Color = DCE.clSpeculateExec;
           if( CD_1->Execute() )
            DCE.clSpeculateExec = CD_1->Color;
           break;
// =============================================================================
  case 31: CD_1->Color = DGR.clGraphStart;
           if( CD_1->Execute() )
            DGR.clGraphStart = CD_1->Color;
           break;
  case 32: CD_1->Color = DGR.clGraphEnd;
           if( CD_1->Execute() )
            DGR.clGraphEnd = CD_1->Color;
           break;
// =============================================================================
  default: break;
//
 } // ����� switch
//
 Write_Config(); // ��������� ��������� ������
//
} //----------------------------------------------------------------------------


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void __fastcall TF1::CD_1_Show(TObject *Sender)
{ // ���������� ��� ��������� ������� ������ ������
//
 switch( outGlobal ) // ����� �� �������� outGlobal
 {
  case 11: SetWindowText( CD_1->Handle, "����� ����� ����� ��������. ������������ �������� <-> ���������" );
           break;
  case 12: SetWindowText( CD_1->Handle, "����� ����� ����� ��������������� ����������" );
           break;
  case 13: SetWindowText( CD_1->Handle, "����� ����� ����� ���������������� �����������" );
           break;
  case 14: SetWindowText( CD_1->Handle, "����� ����� ����� ��������˨���� ���������" );
           break;
//
  case 21: SetWindowText( CD_1->Handle, "����� ����� ����� ������� ���������" );
           break;
  case 22: SetWindowText( CD_1->Handle, "����� ����� ����� ������ ���������� � ��������� ������" );
           break;
  case 23: SetWindowText( CD_1->Handle, "����� ����� ����� ������������� ����������" );
           break;
//
  default: break;
 } // ����� switch
//
} //----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void __fastcall TF1::OnKeyUp_F1(TObject *Sender, WORD &Key,
                                TShiftState Shift)
{ // ���������� ��� ������� ������ �� ������� �����
//
 if( Key == VK_F1 ) // ������ F1 (����������� ������)
 {
  WinExec( "hh.exe data_flow.chm::/main_form.html", SW_SHOWNORMAL );
//  MessageBeep( MB_ICONASTERISK ); // ��������..!
 }
 else
//
 if( Shift.Contains(ssCtrl) && ( Key == 'X' || Key == 'x' ) )
  BitBtn_Stop->Click(); // ���������� ������ ������ BitBtn_Stop
//  Stop_Calculations( Sender ); // ���������� ����������
//
 else
//
 if( Shift.Contains(ssCtrl) && ( Key == 'I' || Key == 'i' ) )
  Run_Infinity(); // ������ ����������� ��������� ����������� ����� ���
//
 else
 if( Shift.Contains(ssCtrl) && ( Key == 'A' || Key == 'a' ) )
  modeStopRunInfinity = true ; // ���������� ����������� ���������� ��������� ���������� ����������
//
 Key = NULL; // �������� ��� ������� ������
//
} //----------------------------------------------------------------------------


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void __fastcall TF1::Save_All_Protocols(TObject *Sender)
{ // ���������� ���� ���������� ������� ������� (���������� �� F2 �� �������� ����)
//
 do_Stop // "���������" ��� ������ ���������� 
//
 Save_Protocol_Master(); // ��������� ������� �������� (*.pro)
 Save_Protocol_AIU(); // ��������� �������� ������������ ��� �� ������� (*.tpr)
 Save_Protocol_Data(); // ��������� �������� ������������ ������ (*.dat)
 Save_Protocol_ExecInstructions(); // ��������� �������� ���������� ���������� (*.tst)
 Save_Protocol_ConnectedGraph(); // ��������� �������� ��������� � ������.����� (*.coi)
//
 SBM0->Text = " ��������� ��������� ��������� !.."; // ����� ������ � StatusBarMain
//
 do_Run // "��������" ��� ������ ����������
//
 Delay(300); // ...����� �������!..
} // ---------------------------------------------------------------------------


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void __fastcall TF1::Save_Data(TObject *Sender)
{ // ��������� ������ ������� (���������� �� Alt+F2 ���������� ����)
 Save_Protocol_Data(); // ��������� �������� ������������ ������ (*.dat)
//
 SBM0->Text = " ��������� ������ �������� !.."; // ����� ������ � StatusBarMain
 Delay(100); // ...����� �������!..
//
} //----------------------------------------------------------------------------


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
int __fastcall Work_TimeSets_Protocol_IC()
{ // ����������� � ������������ max ����� ������������ ���������� ��� (�����������)
// � ����������� ����� ������ ��� ��������� ������� ������������� ����������
 char tmp[_512],
      tmp1[_512],
      tmp2[_512],
      w[_512];
 INT tick_End, // ����� ������� ��������� ����������� ���������� (� ������)
     start, end, // ������ � ����� ���������� ������ ���������� (� ������)
     n_i, // ����� ����������� ���������� � ������ �������
     n_proc, // ����� ���
     n_set,  // ����� ����������
     simult_max_Proc = 0, // ������������ ������������ ���������� ����� ��� (�����������)
     i_tick; // ����..!
//
////////////////////////////////////////////////////////////////////////////////
 F2->Series1->Clear(); // ������� ����� ������ ��� ������� ������������� ����������
////////////////////////////////////////////////////////////////////////////////
 if( !mTpr || // ���� ���� TStringList-������ �� ����������... "���"
     !mTpr->Count ) // ���� ����� ����� = 0 ...
  goto ended;
//
 strcpy(tmp,  mTpr->Strings[mTpr->Count-1].c_str()); // ��������� ������ ������� � Tpr
 strcpy(tmp1, GetSubString(tmp, 21,30)); // ����� ����� ���������� ��������� ����������
 tick_End = _atoi64( tmp1 ); // ����� ���� ���������� ( � ������ )
//
 for( i_tick=0; i_tick<=tick_End; i_tick++ ) // �� ���� ������ ���������
 {
  if( !(i_tick % d_APM) ) // ������ d_APM ���
   APM // ���� ���������� Windows -----------------------------------------------
//
  if( i_tick % tick_ScanForGraph ) // �������� ������ tick_ScanForGraph (global) ������
   continue;
//
  if( !(i_tick % 100) ) // ������ d_APM ���...
  {
   sprintf( w, " ��������� ������ ��� ������ ������ ��� (%.0f%%)...", 1e2*i_tick/tick_End);
   SBM0->Text = w;
  }
//
  n_i = 0; // ����� ����������� ���������� � ������ ������� ������� t_i
//
// ������ ���� ���������� ��� ������ �� mTpr->Count ���������� ...............
////////////////////////////////////////////////////////////////////////////////
  for( INT i=0; i<mTpr->Count; i++ ) // �� ������� ����������� ���������� � Tpr
  {
   if( (!i % d_APM) ) // ������ d_APM ���...
    APM // ���� ���������� Windows ---------------------------------------------
//
   n_proc = _atoi64( GetSubString(mTpr->Strings[i].c_str(),  1,10) ); // ����� ���, �� ������� ���������� �����������
   n_set  = _atoi64( GetSubString(mTpr->Strings[i].c_str(), 41,50) ); // ����� ����������� ����������
   start  = _atoi64( GetSubString(mTpr->Strings[i].c_str(), 11,20) ); // ����� ������ ���������� ���������� (� ������)
   end    = _atoi64( GetSubString(mTpr->Strings[i].c_str(), 21,30) ); // ����� ����� ���������� ���������� (� ������)
//
// ����������� ����� ���������� ��� � ���������� [ (start) - (end) ] .......
   if( (i_tick >= start) && (i_tick < end) ) // ������ (i_tick) ��������� ����� (start) � (end)
   {
    n_i ++ ; // ����� ����������� ���������� � ������ ������� ������� tick_i
    simult_max_Proc = max( simult_max_Proc, n_i ); // ������������ ������������ ���������� ����� ���
   } // �����  if( (i_tick >= start) && (i_tick < end) )
//
  } // ����� ����� �� ������� Tpr
////////////////////////////////////////////////////////////////////////////////
// ��������� ������ Series1 ----------------------------------------------------
//  if( flagGraph ) // ���� �� ���� ��������� ������ ��� ����������...
  F2->Series1->Add( n_i, i_tick, RGB( GetRValue(DGR.clGraphStart) + 1.0 * i_tick / parallel_Ticks * (GetRValue(DGR.clGraphEnd) - GetRValue(DGR.clGraphStart)) ,
                                      GetGValue(DGR.clGraphStart) + 1.0 * i_tick / parallel_Ticks * (GetGValue(DGR.clGraphEnd) - GetGValue(DGR.clGraphStart)) ,
                                      GetBValue(DGR.clGraphStart) + 1.0 * i_tick / parallel_Ticks * (GetBValue(DGR.clGraphEnd) - GetBValue(DGR.clGraphStart)) ) ) ;
//
////////////////////////////////////////////////////////////////////////////////
//
  } // ����� ����� �� ���� ������ i_tick ���������
//
// ----- ��������� ����� x=i_tick-1,y=0 ��� ����������� ��������������� ��� ������� �� �������
  F2->Series1->Add( 0, --i_tick, DGR.clGraphEnd ) ;
//
ended: /////////////////////////////////////////////////////////////////////////
//
 return simult_max_Proc ; // ������������ ������������ ���������� ����� ���
//
} // ------- ����� ��������� Work_TimeSets_Protocol ----------------------------


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void __fastcall Save_Protocol_Master()
{ // ���������� ��������_��������� ���������� ������� (*.pro)
 char FileNameProtocol[_512]; // ���� ��������� '*.pro'
//
////////////////////////////////////////////////////////////////////////////////
// ��������� �������_�������� (*.pro) //////////////////////////////////////////
//
 if( !mR->Lines->Count ) // ���� ����� ������� 0...
  return;
//
 SBM0->Text = " ����������� �������� ������� (PRO-����)..."; // ����� ������ � StatusBarMain
 Delay(100); // ����� ������ �������...
//
 strcpy( FileNameProtocol, ChangeFileExt(FileNameSet, ".pro").c_str() );  // ���� � PRO-�����
 mR->Lines->SaveToFile( FileNameProtocol ); // ��������� ���� ���������
//
} // ----- ����� ������� Save_Protocol -----------------------------------------


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void __fastcall Save_Protocol_AIU()
{ // ���������� ��������� ������������� ��� (*.tpr)
 char FileNameTimeProc[_512]; // ���� ��������� ������������� ��� '*.tpr'
//
 if( !mTpr || // ���� ���� TStringList-������ �� ����������...
     !mTpr->Count ) // ���� ��� �����...
  return;
//
 SBM0->Text = " ����������� �������� ������������� ��� (TPR-����)..."; // ����� ������ � StatusBarMain
 Delay(100); // ����� ������ �������...
//
 strcpy( FileNameTimeProc, ChangeFileExt(FileNameSet, ".tpr").c_str() ); // ���� � TPR-�����
//
 mTpr->SaveToFile( FileNameTimeProc );
//
} // ----- ����� ������� Save_Protocol_AIU -------------------------------------


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void __fastcall Save_Protocol_Data()
{ // ���������� ����� ������������ ������ (*.dat)
 char FileNameData[_512], // ���� ������������ ������ '*.dat'
      tmp[_512];
 FILE *fptr;
////////////////////////////////////////////////////////////////////////////////
// ��������� ��������_������_������ (*.dat) ////////////////////////////////////
//
 if( !Really_Data ) // ���� ������ ���...
  return;
//
 strcpy( FileNameData, ChangeFileExt(FileNameSet, ".dat").c_str() ); // ���� � DAT-�����
 if( !(fptr = fopen(FileNameData, "w")) ) // ���� �� ��������...
  return;
//
 SBM0->Text = " ����������� �������� ������ ������� (DAT-����).."; // ����� ������ � StatusBarMain
 Delay(100); // ����� ������ �������...
//
 for( INT i=0; i<Really_Data; i++ ) // �� ���� Mem_Data[i]
 {
  snprintf( tmp,sizeof(tmp), "#%d [%s]", Mem_Data[i].i_Set, Line_Set(Mem_Data[i].i_Set, 2, 0.0) ); // � ���������� ����� ������� ������������ ������ ��������..?
//
  if( is_ResultIsPredicat( Mem_Data[i].Addr ) ) // ��� ��������� ���������� ����������-���������...
   fprintf( fptr, "%20s%10s%10d     %s\n",   Mem_Data[i].Addr, Mem_Data[i].Data ? trueOutput : falseOutput,
                   Mem_Data[i].CountRead, tmp );
  else
   fprintf( fptr, "%20s%10.4g%10d     %s\n", Mem_Data[i].Addr, Mem_Data[i].Data, // ��� �� �������� !.. %10.4g !!! 
                   Mem_Data[i].CountRead, tmp );
//
 } // ����� ����� for( ULI i=0; i<Really_Data; i++ )
//
 fclose( fptr ); // ������� ����
//
} // ----- ����� ������� Save_Data ---------------------------------------------


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void __fastcall Save_Protocol_ExecInstructions()
{ // ���������� ��������� ���������� ���������� �� ������� (*.tst)
 char FileNameTimeSets[_512], // ���� ��������� ���������� ���������� ��� '*.tst'
      tmp2[_512],
      tmp3[_2048]; // ��� ���=99 ����� ~1200 ��������
 INT tick_End, // ����� ������� ��������� ����������� ���������� (� ������)
     start, end, // ������ � ����� ���������� ������ ���������� (� ������)
     n_i, // ����� ����������� ���������� � ������ ������� �������
     n_proc, // ����� ���
     n_set;  // ����� ����������
//
 FILE *fptr;
//
////////////////////////////////////////////////////////////////////////////////
// ��������� ��������_����������_���������� (*.tst) ////////////////////////////
//
 if( !mTpr || // ���� ���� TStringList-������ �� ����������... "���"
     !mTpr->Count ) // ���� ����� ����� = 0 ...
  return;
//
  strcpy( FileNameTimeSets, ChangeFileExt(FileNameSet, ".tst").c_str() ); // ���� � TST-�����
  if( !(fptr = fopen(FileNameTimeSets, "w")) ) // ���� �� ��������...
   return;
//
  SBM0->Text = " ����������� �������� ���������� ���������� (TST-����)..."; // ����� ������ � StatusBarMain
  Delay(100); // ����� ������ �������...
//
  tick_End = _atoi64(GetSubString(mTpr->Strings[mTpr->Count-1].c_str(), 21,30)); // ����� ���� ���������� ( � ������ )
//
  for( INT i_tick=0; i_tick<=tick_End; i_tick++ ) // �� ���� ������ ���������
  {
   if( i_tick % tick_ScanForGraph ) // �������� ������ tick_ScanForGraph ������
    continue;
//
   if( !(i_tick % d_APM) ) // ������ d_APM ���...
    APM // ���� ���������� Windows
//
   n_i = 0; // ����� ����������� ���������� � ������ ������� ������� t_i
   strcpy(tmp3, "\0"); // �������� tmp3
//
// ������ ���� ���������� ��� ������ �� Tpr (0-n_Tpr) ���������� .........
////////////////////////////////////////////////////////////////////////////////
   for( INT i=0; i<mTpr->Count; i++ ) // �� ������� ����������� ���������� � Tpr
   {
    n_proc = _atoi64( GetSubString(mTpr->Strings[i].c_str(),  1,10) ); // ����� ���, �� ������� ���������� �����������
    n_set  =  _atoi64( GetSubString(mTpr->Strings[i].c_str(), 41,50) ); // ����� ����������� ����������
    start  = _atoi64( GetSubString(mTpr->Strings[i].c_str(), 11,20) ); // ����� ������ ���������� ���������� (� ������)
    end    = _atoi64( GetSubString(mTpr->Strings[i].c_str(), 21,30) ); // ����� ����� ���������� ���������� (� ������)
//
// ����������� ����� ���������� ��� � ���������� [ (start) - (end) ] .......
    if( (i_tick >= start) && (i_tick < end) ) // ������ (i_tick) ��������� ����� (start) � (end)
    {
     n_i ++ ; // ����� ����������� ���������� � ������ ������� ������� tick_i
//
     snprintf( tmp2,sizeof(tmp2), "%d/%d/%s  ", n_proc, n_set, Mem_Instruction[n_set].Set );
     strcat( tmp3, tmp2 ); // ����������� '���/�����_�����/���������'
//
     if( strlen(tmp3) > 0.7 * sizeof(tmp3) ) // ����� tmp3 �� �������������..!
      strcpy( tmp3, " (...) " );
    } // ����� if( (i_tick >= tick_1) && (i_tick < tick_2) ) // ������ (i_tick) ��������� ����� (tick_1) � (tick_2)
//
   } // ����� for( ULI i=0; i<mTpr->Count; i++ ) // �� ������� ����������� ���������� � Tpr
////////////////////////////////////////////////////////////////////////////////
  fprintf(fptr, "%10ld%10d     %s\n", i_tick, n_i, tmp3);
//
  } // ����� for( INT i_tick=0; i_tick<=tick_End; i_tick++ ) // �� ���� ������ ���������
//
 fclose( fptr ); // ������� ����
//
} // ----- ����� ������� Save_ExecInsrucrions ----------------------------------


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void __fastcall Save_Protocol_ConnectedGraph()
{ // ���������� ��������� ��������� ����� (*.coi)
 char tmp[_512],
      tmp1[_512],
      tmp2[_512],
      tmp3[_2048]; // ��� ���=99 ����� ~1200 ��������
 ULI tick_End, // ����� ������� ��������� ����������� ���������� (� ������)
     tick_1, tick_2, // ������ � ����� ���������� ������ ���������� (� ������)
     n_i, // ����� ����������� ���������� � ������ ������� �������
     n_proc, // ����� ���
     n_set;  // ����� ����������
// FILE *fptr;
//
// ��������� ��������_�������_���������_�����_��������� (*.coi) ////////////////
//
 SBM0->Text = " ����������� �������� ������� ��������� ����� ��������� (COI-����)...";
 Delay(100); // ����� ������ �������...
//
 Calc_ConnectedIndex( 1 ); // ������� �������� ��������� c ����������� � ����
//
} // ----- ����� ������� Save_Protocol_ConnectedGraph --------------------------


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
bool __fastcall AddLineToProtocol(char *str)
{ // ��������� ������ str � ���� ���������� ������������� ������ (����������� "\n")
 char c, cc[2] = "?\0", buf[_16384] = "\0"; // buf - "�������" ������ ��� ������
//
 for( int Pos=0; Pos<=strlen(str); Pos++ ) // Pos - ����� ������� � ������ str
 {
  c = str[Pos]; // ��������� Pos-� ������ ������ str
//
  if ( (c!='\n') && (c!='\0') ) // ��� �� UPR-������ (�����������) � �� ����� ������
  {
   cc[0] = c;
   strcat( buf, cc ); // ��������� ������ � � buf
  } // ����� if( c != UPR )
//
  else
//  if( (c=='\n') || (c=='\0') ) // ����������� ������ ��� ����� ������
  {
   F1->M1->Lines->Add( buf ); // ����� ������ buf � M1
   strcpy( buf, "\0" ); // �������� ������ ��� ���������� ������
  }
//
 } // ����� �� �������� ������ str
//
 return true ; // ��� Lua ������ ���� ���-�� ���������� (void �� �����������!)
//
} // --- ����� c_AddLineToProtocol ----------------------------------------------


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void __fastcall TF1::M1_ClearAll(TObject *Sender)
{ // �������� M1
 M1->Clear(); // ��������� �������� M1 (TMemo)
} //----------------------------------------------------------------------------


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void __fastcall TF1::M1_CopyAll(TObject *Sender)
{ // ���������� M1 � ClipBoard
// Clipboard->AsText = M1->Text.
 M1->SelectAll(); // �������� ���� ����� � M1
 M1->CopyToClipboard(); // ���������� �� �� M1 � Clipboard
 M1->SelLength = 0; // ����� ���������
} //----------------------------------------------------------------------------


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void __fastcall TF1::M1_CopyToNotepad(TObject *Sender)
{ // ���������� M1 � Notepad
 char FileName[_256];
 strcpy( FileName, ChangeFileExt( FileNameSet, ".txt" ).c_str()); // ��� ������� + "txt"
 M1->Lines->SaveToFile( FileName ); // ������ ��� ������ �� M1 � ���� FileName
 ShellExecute( Handle, "open", FileName, NULL, NULL, SW_RESTORE ); // ������� ���� FileName
//
 sleep( 1 ); // #include <dos.h>
//
 DeleteFile( FileName ); // ���������� (���������) ����
} //----------------------------------------------------------------------------


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void __fastcall TF1::Save_IGA_Click(TObject *Sender)
{ // ��������� ��������� � ���� ��������������� ����� ��������� (���) - ������ ���
 Save_IGA(); // ���������� GV-�����
} //----- ����� TF1::Save_IGA_Click --------------------------------------------


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void __fastcall Save_IGA()
{ // ��������� ��������� � ���� ������ ��� (������ *.GV)
 INT i,j, nArg, fromOp, iEdge = 0; // ����� ���� � ���
 TStringList *sIGA = new TStringList(); // ������ ����� ��� ����������� ���
 char FileName[_256],
      tmp[_2048],
      aResult[_ID_LEN],
      aPredicat[_ID_LEN];
 bool flagNot, flagPredicat;
//
// ��������� ��������������_����_��������� (*.gv) //////////////////////////////
//
 SBM0->Text = " ����������� �������������� ���� ��������� (GV-����)...";
 Delay(100); // ����� ������ �������...
//
 strcpy( FileName, FileNameSet ); // ��� ����� ���
 strcpy( FileName, ChangeFileExt( FileName, ExtIGA ).c_str()); // ".gv" ����������� ��������, ��� ".dot" ������������� � Word'�
//
 snprintf(tmp,sizeof(tmp), "#\n// Valery Bakanov research computer complex (2008 and further); e881e@mail.ru, http://vbakanov.ru/left_1.htm" );
 sIGA->Add( tmp ); // �������� ������ �����������
//
 snprintf(tmp,sizeof(tmp), "\/* This file was automatically created thru program DATA_FLOW.EXE\n   from the original data file %s \*/\n#",
          AnsiUpperCase(ExtractFileName(FileNameSet) ) );
 sIGA->Add( tmp ); // �������� ������ �����������
//
 snprintf(tmp,sizeof(tmp), "digraph %s {", AnsiUpperCase(ChangeFileExt(ExtractFileName(FileNameSet),"\0")) );
// ChangeFileExt(ExtractFileName(FileName),'')
 sIGA->Add( tmp ); // �������� c����� ������ �������� �����
//
// === ���� ���� (�������������� �����������) � ��������� ----------------------
// ���������� ������ - ��� i-� ���������� (���� aResult) -----------------------
// ���������� ���� - ��� j-� ���������� (���� aOp1 ��� aOp2) -------------------
 for( i=0; i<Really_Set; i++ ) // �� ���� ��c�������� (����������) - ��� ������
 {
  strcpy( aResult, Mem_Instruction[i].aResult ); // ���������...
//
// ���������� ���� (���������� [i] ���������� � ����������� [j] ----------------
  for( j=0; j<Really_Set; j++ ) // �� ���� ��c�������� (����������) - ��� ����
  {
   if( is_SET( Mem_Instruction[j].Set ) || // c ���� (SET) �� ���������� (��� � SET ���� ������)
       i == j ) // ������ � ����� �� ���������� (��� ������ ��� ����)...
    continue;
//
   strcpy( aPredicat, Mem_Instruction[j].aPredicat ); // ���������...
//--- ���������, ���������� �� ��� ����� ���������� � '!' ��� '~'
   flagNot = ( aPredicat[0] == symbolNot_1 || aPredicat[0] == symbolNot_2 )
               ? true : false; // TRUE, ���� � ���� aPredicat ������ ������ '!' ��� '~'
//--- ��� ���� ���������� (�� true � �� false) ? -------------------------------
   flagPredicat = ( strcmp( aPredicat, trueLowerCase ) && strcmp( aPredicat, falseLowerCase )
                   ? true : false );
//
// ������� �� ���������� '!' ��� '~'
   if( flagPredicat ) // �� true/false
    if( flagNot )
     strcpy( aPredicat, &Mem_Instruction[j].aPredicat[1] ); // ������� �� ������� �������...
//
   if( !strcmp( Mem_Instruction[j].aOp1, aResult ) || // c Result ������� ��� ������� ��������...
       !strcmp( Mem_Instruction[j].aOp2, aResult ) || // c Result ������� ��� ������� ��������...
      ( flagPredicat &&
        is_Predicat( Mem_Instruction[i].Set ) &&
       !is_Predicat( Mem_Instruction[j].Set ) &&
       !strcmp( aPredicat, aResult ) ) ) // ����������� �� ����� ����������
    {
     snprintf(tmp,sizeof(tmp), "%d -> %d ; -Ticks %d/%d [[%d]%s %s,%s,%s,%s; %s] -> [[%d]%s %s,%s,%s,%s; %s]",
              i+StartNumb, j+StartNumb,
              Get_TicksByInstruction(Mem_Instruction[i].Set), Get_TicksByInstruction(Mem_Instruction[j].Set), // ����� ������.���������� "������/����" (�� "������� ->")
              i+StartNumb, Mem_Instruction[i].Set,Mem_Instruction[i].aOp1,Mem_Instruction[i].aOp2,Mem_Instruction[i].aResult,Mem_Instruction[i].aPredicat,Mem_Instruction[i].Comment,
              j+StartNumb, Mem_Instruction[j].Set,Mem_Instruction[j].aOp1,Mem_Instruction[j].aOp2,Mem_Instruction[j].aResult,Mem_Instruction[j].aPredicat,Mem_Instruction[j].Comment);
//     snprintf(tmp,sizeof(tmp), "%d -> %d ;", i+StartNumb,j+StartNumb); // ������� �������... ����� ����������!..
     sIGA->Add( tmp );
     iEdge ++ ;
    }
//
   } // ����� ����������� ����� �� �� j (������ ����������) --------------------
//
 } // ����� �������� ����� �� i (������ ����������) ----------------------------
////////////////////////////////////////////////////////////////////////////////
//
 sIGA->Add( "}" ); // �������� c����� � ����� ������ ���
//
 snprintf(tmp,sizeof(tmp), "#  Total edges in this directed graph: %d", iEdge );
 sIGA->Insert( 1, tmp ); // �������� ������ �����������
//
 sIGA->SaveToFile( FileName ); // ��������� � ����
 sIGA->Free(); // ��������� ������ ����� sIGA
//
} //----- ����� Save_IGA -------------------------------------------------------


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void __fastcall TF1::ExtendedSave_IGA_Click(TObject *Sender)
{ // ��������� � ���� ������ ��� � *.GV + ������ � ������� ���������� ����������� � *.MVS
 Save_IGA(); // ��������� ������ ��� (������ *.GV)
 Extended_Save_IGA(); // ���������� GV-�����
} //-----  ����� TF1::ExtebdedSave_IGA_Click -----------------------------------


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void __fastcall Extended_Save_IGA()
{ // ��������� ������ � ������� ���������� ���������� (������ *.MVR)
 INT i; // ����� ����������
 char FileName[_256];
 FILE *fptr;
//
// ��������� ������_�_�������_����������_����������� (*.mvr) ///////////////////
//
 SBM0->Text = " ����������� ���� ������ � ������� ���������� ���������� (MVR-����)...";
 Delay(100); // ����� ������ �������...
//
 strcpy( FileName, FileNameSet ); // ��� ����� "MVR"
 strcpy( FileName, ChangeFileExt( FileName, ExtMVR).c_str()); // ������ ".MVR" - ������ � ������� ���������� ������ ���������� (������� �����)
//
 if( !(fptr = fopen(FileName, "w")) ) // ���� �� ��������...
  return;
//
 fprintf( fptr, "; Valery Bakanov research computer complex (2008 and further); e881e@mail.ru, http://vbakanov.ru/left_1.htm\n" );
 fprintf( fptr, "; This file was automatically created thru program DATA_FLOW.EXE\n; from the original data file %s\n;\n",
          AnsiUpperCase(ExtractFileName(FileNameSet) ) );
//
 for( i=0; i<Really_Set; i++ ) // �� ���� ��c�������� (����������)
  fprintf(fptr, "=%d/%d: -Ticks %d ; [%s,%s,%s,%s,%s,%s]\n",
                 i+StartNumb,i+StartNumb,Get_TicksByInstruction(Mem_Instruction[i].Set),
                 Mem_Instruction[i].Set,
                 Mem_Instruction[i].aOp1,Mem_Instruction[i].aOp2,Mem_Instruction[i].aResult,
                 Mem_Instruction[i].aPredicat,Mem_Instruction[i].Comment);
//
 fclose(fptr);
//
} //----- ����� Extended_Save_IGA ----------------------------------------------


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void __fastcall TF1::Show_AIU(TObject *Sender)
{ // �������� ������ �������� ��� �� ������� ���������� ���������
//
 if( Regim != 2 || // ���� ��������� �� ����������� ��������� (������ �� F6)
     parallel_Ticks <= 0 ) // ����� ���������� �� �������������
 {
  MessageBeep( MB_OK ); // �������� ��������������...
  return;
 }
//
 char str[_512];
//
 F3->Chart_AIU->Title->Text->Clear(); // ������� TSting ����� ����� �����������
 F3->Chart_AIU->Title->Text->Add( "������������ ������������� ��������� ��� �����������" );
 F3->Chart_AIU->Title->Text->Add( "���������� � ������� ������� (� ������) ���������� ���������" );
 F3->Chart_AIU->Title->Text->Add( "(����� �� ������ �������������� ����� - ������ ����������)" );
 F3->Chart_AIU->Title->Text->Add( "\0" );
//
 F3->Chart_AIU->Foot->Text->Clear(); // ������� TSting ����� ����� �����������
//
 snprintf( str,sizeof(str), "������������ ����������: %d ������, ��������� = %.*g",
                             parallel_Ticks, ACC_REAL, 1.0 * serial_Ticks / parallel_Ticks );
 F3->Chart_AIU->Foot->Text->Add( str );
//
 snprintf( str,sizeof(str), "����� ������������� %d ����/� ��� �� %d ���������", all_maxProcs, max_Proc );
 F3->Chart_AIU->Foot->Text->Add( str );
//
 snprintf( str,sizeof(str), "�� ��� ������������ �� ����� %d ����/� ���", simult_maxProcs );
 F3->Chart_AIU->Foot->Text->Add( str );
//
 F3->Top  = F1->Top  + 40;
 F3->Left = F1->Left + 40;
//
 F3->Width  = 500; // ����� ��������� �����...
 F3->Height = 2 * F3->Width / 3 ;
//
 Work_TimeSets_Protocol_AIU(); // ���������� �������� ��� ���������
//
 F3->Show(); // �������� ���� ����� F2 � ����������� ������
//
 F3->Caption = " ������� ������������� ��� �� ������� (��������� ";
 F3->Caption = F3->Caption + AnsiUpperCase(FileNameSet) + ")"; // �������� ��� ����� ���������
//
} //------ ����� Show_AIU ------------------------------------------------------


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
int __fastcall Work_TimeSets_Protocol_AIU()
{ // ����������� � ������������ max ����� ������������ ���������� ��� (�����������)
// � ����������� ����� ������ ��� ��������� ������� ������������� ����������
 INT i_Op, n_Op, ii_Op, nn_Op,
     i_Proc, start, end, dur, i_Set, ii_Set;
 char aResult[_ID_LEN];
//
////////////////////////////////////////////////////////////////////////////////
 F3->Series_AIU->Clear(); // ������� ����� ������ ��� ������� ������������� ���
////////////////////////////////////////////////////////////////////////////////
 if( !mTpr || // ���� ���� TStringList-������ �� ����������... "���"
     !mTpr->Count ) // ���� ����� ����� = 0 ...
  return 1;
//
 F3->Chart_AIU->BottomAxis->SetMinMax( 0, parallel_Ticks ); // ��� �������
 F3->Chart_AIU->LeftAxis->SetMinMax(   0, all_maxProcs+1 ); // ��� �������
//
// F3->Series1->Pointer->VertSize = 3; // ������������ ������ ���������������
// F3->Series1->Marks->Font->Size = 12; // ������ �������� ������ �� ���������������
// F3->Series1->Marks->Style = talMark; // ��� �����
// (talAuto, talNone, talValue, talMark, talText, talPointValue)
//
 F3->Series_AIU->Pointer->VertSize = 3; // ������������ ������ ���������������
 F3->Series_AIU->Marks->Font->Size = 7; // ������ �������� ������ �� ���������������
//
 if( all_maxProcs <= 10 || Really_Set+1 <= 100 ) // ��� <= 100 ��� ����� ���������� <= 100
 {
  F3->Series_AIU->Pointer->VertSize = 8; // ������������ ������ ���������������
 }
//
 for( i_Op=0; i_Op<mTpr->Count; i_Op++ ) // �� ������� ����������� ���������� � mTpr
 {
  i_Proc = _atoi64( GetSubString(mTpr->Strings[i_Op].c_str(), 1,10) ); // ����� ���
  start  = _atoi64( GetSubString(mTpr->Strings[i_Op].c_str(),11,20) ); // ������ ������ ���������� ����������
  end    = _atoi64( GetSubString(mTpr->Strings[i_Op].c_str(),21,30) ); // ������ ��������� ���������� ����������
  dur    = _atoi64( GetSubString(mTpr->Strings[i_Op].c_str(),31,40) ); // ������ ������������ ����������
  i_Set  = _atoi64( GetSubString(mTpr->Strings[i_Op].c_str(),41,50) ); // ����� �������������� �� ������ ��� ���������

//  t_printf( "%03d %010d %010d %010d %010d %010d [%s %s,%s->%s] %s",
//            i_Op,
//            i_Proc, start, end, dur, i_Set,
//            Mem_Instruction[i_Set].Set, Mem_Instruction[i_Set].aOp1, Mem_Instruction[i_Set].aOp2, Mem_Instruction[i_Set].aResult, // ��� ����������
//            "��� #" + AnsiString(i_Proc) );

// ��������� ��� ������� �� ������� �����
  F3->Series_AIU->AddGantt( start, end, i_Proc+1, "��� #" + AnsiString(i_Proc) );
//
  strcpy( aResult, Mem_Instruction[i_Set].aResult ); // ���.������. ���������� i_Set ( aResult ������������ � MI_AOP1/MI_AOP2 )
//
  for( ii_Op=0; ii_Op<mTpr->Count; ii_Op++ ) // �� ������� ����������� ���������� � Tpr
  {
   if( i_Op == ii_Op ) // ������ (����������) ���� � ����� �� ����������..!
    continue;
//
   ii_Set = atoi( GetSubString(mTpr->Strings[ii_Op].c_str(),41,50) ); // ����� �������������� �� ������ ��� ���������
//
   if( ( Get_CountOperandsByInstruction(Mem_Instruction[ii_Set].Set)==1 &&  MI_AOP1(ii_Set) ) || // ����� ���� �������...
       ( Get_CountOperandsByInstruction(Mem_Instruction[ii_Set].Set)==2 && (MI_AOP1(ii_Set) || MI_AOP2(ii_Set)) ) ) // ��� ��������...
    F3->Series_AIU->NextTask->Value[i_Op] = ii_Op ; // ������� ����� ����� "i_Op <-> ii_Op"
//
  }  // �����  for( ii_Op=0; ii_Op < mTpr->Count; ii_Op++ )
//
 } // �����  for( i_Op=0; i_Op < mTpr->Count; i_Op++ )
//
 return 0 ; // ������������ ������������ ���������� ����� ���
//
} // ------- ����� Work_TimeSets_Protocol_AIU ----------------------------------


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
char* __fastcall PutDateTimeToString(INT flag)
{ // ������ ������� ���� � ������� � ������ � ���������������
 char str[_512];
//
 if( flag == 0 ) // ������������ ��� ����� ( ������ ':' ���������� )
  strNcpy( str, TDateTime::CurrentDateTime().FormatString("dd.mm.yyyy'/'hh:nn:ss:zzz").c_str() );
 else // ���������� ��� ����� ( ������ ':' ���������� )
  strNcpy( str, TDateTime::CurrentDateTime().FormatString("dd-mm-yyyy'_'hh-nn-ss-zzz").c_str() );
//
 return str ;
} // ����� PutDateTimeToString -------------------------------------------------


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void __fastcall Make_Row_Current( INT Row )
{ // ������� ������� �������� ������ � ������� ����������
//
 if( Row>=0 && Row<=mS->RowCount-2 ) // ������ ����� �� 0 �� max-2
  mS->Row = Row+1; // � ������� SG_Set ���������� ������� ������ Row (�������� � 0)
 else
  MessageBeep( MB_ICONASTERISK ); // ��������������...  );
//
} // --- ����� Move_Cell -------------------------------------------------------


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void __fastcall TF1::OnResize_F1(TObject *Sender)
{ // ���������� ��� ������ ����������� �������� ���� F1
 int tmp,  // ������� ����������
     dH = 40; // ���������� �� ������ ����� �������� ��������� � ������ ������� ����
//   Scrollbar_Width  = GetSystemMetrics(SM_CXVSCROLL), // ������ ������������� ScrollBar'a
//   Scrollbar_Height = GetSystemMetrics(SM_CYHSCROLL); // ������ ��������������� ScrollBar'a
 float partProtocolH = 0.35; // ������������� ���� �� ������ ���� ������ ��������� ������� ������
//
// ��������� ������ �������� ���� F1
 F1->Width  = max( F1->Width,  minW_F1 ); // ����� ������ �������� ���� F1
 F1->Height = max( F1->Height, minH_F1 );
//
// ��������� ������ � ��������� ���������� ����������� ���� F1
//
// ������� ����� - ���� ������ ��������� ������� -------------------------------
 mR->Height = F1->Height * partProtocolH;
 mR->Width  = F1->ClientWidth - 8;
//
// ������ ��� ������� (���������� ������) --------------------------------------
 tmp = F1->M1->Top + F1->M1->Height + dH;
 mB->Top  = tmp; // ������������ "��������" ����������� ��� ������� (Properties)
 mP->Top  = tmp;
 mS->Top  = tmp;
 mD->Top  = tmp;
//
// ������ ��� ������� (������) -------------------------------------------------
 tmp = F1->ClientHeight - mB->Top - SBM->Height - 5;
 mB->Height  = tmp; // ������������ "��������" ����������� ��� ������� (Properties)
 mP->Height  = tmp;
 mS->Height  = tmp;
 mD->Height  = tmp;
//
// ������ ��� ������� (�� ������) ----------------------------------------------
 mB->Width = mB->ColWidths[0] + mB->ColWidths[1] + 6; // F1->ClientWidth * partBufferW;
 mP->Left  = mB->Left + mB->Width + 2;
 mS->Left  = mP->Left + mP->Width + 4;
//
 mS->Width = F1->ClientWidth - mS->Left - mD->Width - 8;
 mD->Left  = F1->ClientWidth - mD->Width - 4; // mS->Left  + mS->Width + 2;
 mD->Width = mD->ColWidths[0] + mD->ColWidths[1] + 6; //F1->ClientWidth - mD->Left - 6;
//
// ������� ��� (������) --------------------------------------------------------
 tmp = mR->Top + mR->Height + ( dH - F1->BitBtn_Run->Height ) / 2;
 F1->BitBtn_Run->Top  = tmp; F1->BitBtn_Run->Left  = mB->Left;
 F1->BitBtn_Stop->Top = tmp; F1->BitBtn_Stop->Left = mS->Left;
//
// ������� ��� ( E_AIU + Label_AIU ) -------------------------------------------
 F1->E_AIU->Top     = tmp + 2; F1->E_AIU->Left     = F1->BitBtn_Stop->Left + F1->BitBtn_Stop->Width + 70;
 F1->Label_AIU->Top = tmp + 5; F1->Label_AIU->Left = F1->E_AIU->Left + F1->E_AIU->Width + 7;
//
// ������� ��� (�������) -------------------------------------------------------
 tmp = mR->Top + mR->Height + ( dH - F1->BitBtn_Run->Height ) / 2 + F1->BitBtn_Run->Height - F1->Label_Buffer->Height;
 F1->Label_Buffer->Top = tmp; F1->Label_Buffer->Left = mB->Left + mB->Width - F1->Label_Buffer->Width;
 F1->Label_Set->Top    = tmp; F1->Label_Set->Left    = mS->Left + mS->Width - F1->Label_Set->Width;
//
// ����� ������ Label_Data ������������� �� ���������� ���� Anchors={false,false,true,TRUE}
 F1->Label_Data->Top   = tmp; F1->Label_Data->Left  = mD->Left + mD->Width - F1->Label_Data->Width - 3; // ������������ SG_Data
//
// ���������� ������ ���������� ������� SG_Set (������� ������������ � ����������)
 tmp = mS->ColWidths[0] + mS->ColWidths[1] + mS->ColWidths[2]  // ������ ������ 0-5 ��������
     + mS->ColWidths[3] + mS->ColWidths[4] + mS->ColWidths[5];
//
 mS->ColWidths[7] = SG_Instruction->Width - tmp - 28; // ������ 8-�� �� ����� ������� SG_Set
//
 if( mS->ColWidths[7] < sizeof(M_I.Comment) ) // �� ����� sizeof(M_S.Comment)
  mS->ColWidths[7] = sizeof(M_I.Comment);
//
// ������ ������ �������������� ������ -----------------------------------------
 SBM->SimplePanel = false; // �� ������ (��������� ����������) !!!
 SBM0->Width = 3 * F1->ClientWidth / 8;
 SBM1->Width = SBM0->Width;
// SBM2->Width = F1->ClientWidth / 4;
//
} //----- ����� F1_OnResize ----------------------------------------------------


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void __fastcall Run_Infinity()
{ // ������ ���������� ���������� ����������� ����� ��� (�� Alt+A)
//
 modeStopRunInfinity = false ; // �������������������� ����� ���
//
 while( 1 ) // ������� ����������� ����� ���
 {
  if( modeStopRunInfinity ) // ���������� ������������� ������������ ����������
  {
   StopCalculations( 0 ); // ���������� ����������� �������������
   break; // ����� �� �����
  }
//
  Delay( -5 ); // ����� 5 ���
//
  if( F1->BitBtn_Run->Enabled ) // ���� ������ F1->BitBtn_Run ����� �������...
  {
   F1->BitBtn_Run->Click(); // ���������� ������ ������ F1->BitBtn_Run
   Delay( -60 ); // ����� 60 ��� ������ ������� ������������� ����������
  } // ����� if( F1->BitBtn_Run->Enabled )
//
 } // ����� while( 1 )
//
} // --- ����� Run_Repeat ------------------------------------------------------


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void __fastcall TF1::Result_toOperands(TObject *Sender)
{ // ���������� ������ ��� ������� ��������, ��������� ��
// ���������� ���������� ������ (���������� � SG; ������� 4) ����������
 char Set[_SET_LEN],
      aResult[_ID_LEN],
      aPredicat[_ID_LEN],
      str[_2048], tmp[_1024]; // ��� ������� str ���� �������
 bool c1, c2; // ����� ������������ ���������
//
 Clear_AllTableInstructions(); // ������� ����� ���� ����� ������� ����������
//
 if( mS->Col != 4 ) // ��� ������ �� ������� ����������... ���������� !!!
  return;
//
 strcpy( aResult, Mem_Instruction[mS->Row-1].aResult ); // ��������� ����� aResult
//
 Really_Select = 0; // ������� ����������� � Sel_Cell ����� (������)
//
 for( INT i=0; i<Really_Set; i++ ) // �� ���� ������� Mem_Instruction[i]
 {
//
  c1 = c2 = false; // ���������� � �������...
//
  strcpy(Set, Mem_Instruction[i].Set); // ����� ��������� i-��� ����������
//
  if( is_SET( Set ) ) // ���������� SET �� ������������� !!!
    continue;
//
// ----- ���� ��������� (������� 5) ������ switch �� ������������ --------------
//
  switch( Get_CountOperandsByInstruction(Set) ) // ... ����� ������� ��������� ���������� Set
  {
   case 1: if( !strcmp(aResult, Mem_Instruction[i].aOp1) ) // ���� ���������� aResult � aOp1 (���� �������) ?
           {
            Sel_Cell[Really_Select].Col = 2; // ������� 2
            Sel_Cell[Really_Select].Row = i + 1;
            Sel_Cell[Really_Select].clBackground = DCT.clOperandOperation;
            Sel_Cell[Really_Select++].clSymbol   = clBlack;
//
            snprintf(tmp,sizeof(tmp), " %d(1|1)", i); // 1-� (� ������������) �������
            strcat(str, tmp);
//
            snprintf(tmp,sizeof(tmp), " %d(*|1)", i); // 1-� (� ������������) �������
            strcat(str, tmp);
           }
//
           break;
//
   case 2: if( !strcmp(aResult, Mem_Instruction[i].aOp1) ) // ���� ���������� aResult � aOp1  (��� ��������) ?
           {
            Sel_Cell[Really_Select].Col = 2; // ������� 2
            Sel_Cell[Really_Select].Row = i + 1;
            Sel_Cell[Really_Select].clBackground = DCT.clOperandOperation;
            Sel_Cell[Really_Select++].clSymbol   = clBlack;
//
            c1 = true;
           }
//
           if( !strcmp(aResult, Mem_Instruction[i].aOp2) ) // ���� ���������� aResult � aOp2 (��� ��������) ?
           {
            Sel_Cell[Really_Select].Col = 3; // ������� 3
            Sel_Cell[Really_Select].Row = i + 1;
            Sel_Cell[Really_Select].clBackground = DCT.clOperandOperation;
            Sel_Cell[Really_Select++].clSymbol   = clBlack;
//
            c2 = true;
           }
//
           break;
//
  default: break;
//
  } // ����� switch
//
////////////////////////////////////////////////////////////////////////////////
//
  if( c1 ) // ���� ���� ���������� � 1-� ���������
  { snprintf(tmp,sizeof(tmp), " %d(1|2)", i);
    strcat(str, tmp); }
//
  if( c2 ) // ���� ���� ���������� �� 2-� ���������
  { snprintf(tmp,sizeof(tmp), " %d(2|2)", i);
    strcat(str, tmp); }
//
  if( c1 && c2 ) // ���� ���������� � 1-� � 2-� ����������
  { snprintf(tmp,sizeof(tmp), " %d(*|2)", i);
    strcat(str, tmp); }
//
////////////////////////////////////////////////////////////////////////////////
// ----- ���� ��������� (������� 5) ������ switch �� ������������� -------------
//
// ----- �������� ��������� ���� ��������� (������� 5) -------------------------
//
  strcpy( aPredicat, Mem_Instruction[i].aPredicat ); // ��������� ��� �������� ���������
//
  if( !strcmp(aResult,aPredicat) || // ������ ���������� "���" ...
    ( ( aPredicat[0]==symbolNot_1 || aPredicat[0]==symbolNot_2 ) && // ������ ������ - ���������
       !strcmp(aResult,&aPredicat[1] ) ) ) // ���������� ������� �� ������� �������
  {
   Sel_Cell[Really_Select].Col = 5; // ������� 5 (�������� ���������)
   Sel_Cell[Really_Select].Row = i + 1;
   Sel_Cell[Really_Select].clBackground = DCT.clOperandOperation;
   Sel_Cell[Really_Select++].clSymbol   = clBlack;
//
   snprintf(tmp,sizeof(tmp), " %d(Pred)", i); // �������-��������
   strcat(str, tmp);
//
  }
//
 } // ����� ����� for( INT i=0; i<Really_Set; i++ ) // �� ���� ������� Mem_Instruction[i]
////////////////////////////////////////////////////////////////////////////////
//
 mS->Repaint();
//
 if( strlen(str) ) // ������ �� ������
  t_printf( "\n-I- %s(): ��������� ���������� ���������� #%d ������������ %d ���/� � �������� ��������/��: %s -I-",
            __FUNC__ + 5, mS->Row-1, Really_Select, str); // (+5) ����������� �� TF1::
 else // ������ ������
  t_printf( "\n-I- %s(): ��������� ���������� ���������� #%d �� ������������ � �������� �������� �������� !!! -I-",
            __FUNC__ + 5, mS->Row-1, Really_Select, str); // (+5) ����������� �� TF1::
//
} //----- ����� TF1::Result_toOperands -----------------------------------------


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void __fastcall TF1::Operand_toResult(TObject *Sender)
{ // ���������� ������ ��� ���������� ���������� ����������, ��
// ������� ������� ������ ������� �������
//
 char Set[_SET_LEN],
      aOp1[_ID_LEN],
      aOp2[_ID_LEN],
      aResult[_ID_LEN],
      aPredicat[_ID_LEN];
//      tmp[_512];
 INT Col, Row, n_Op,
     i_save, i_save_no = -1234567;
//
 i_save = i_save_no; // ��������������� ������������
//
 if( is_SET(Mem_Instruction[mS->Row-1].Set) ) // ��� SET �������� ������������...
  return;
//
 Clear_AllTableInstructions(); // ������� ����� ���� ����� ������� ����������
//
 Col = mS->Col; // ��������� ����� ������� (2-� ������������� aOp1, 3-� - aOp2)
 Row = mS->Row;
//
 if( Col==2 ) // ������ �������
  strcpy(aOp1, Mem_Instruction[mS->Row-1].aOp1); // ��������� ����� aOp1
 else
 if( Col==3) // ������ �������
  strcpy(aOp2, Mem_Instruction[mS->Row-1].aOp2); // ��������� ����� aOp2 (����� �� ����!!!)
 else
 if( Col==5 ) // �������-��������
  strcpy(aPredicat, Mem_Instruction[mS->Row-1].aPredicat); // ��������� ����� aPredicat
//
 strcpy( Set, Mem_Instruction[Row-1].Set ); // ����� ��������� ���������� ����������
 n_Op = Get_CountOperandsByInstruction(Set); // ����� � ������� ���������
//
 Really_Select = 0; //  ������� ����������� � Sel_Cell ����� (������)
//
 for( INT i=0; i<Really_Set; i++ ) // �� ���� ������� Mem_Instruction[]
 {
  strcpy( aResult, Mem_Instruction[i].aResult ); // ����� ����� ���������� i-��� ����������
//
  switch( Col )  // �� ������ ������� �������
  {
   case 2: // ������ ������� ������� ( aOp1, ������� 2 )
           if( !strcmp(aOp1,aResult) ) // ���� ���������� aOp1 � aResult ?
           {
            Sel_Cell[Really_Select].Col = 4; // ��������� - ������� 4
            Sel_Cell[Really_Select].Row = i + 1;
            Sel_Cell[Really_Select].clBackground = DCT.clOperandOperation;
            Sel_Cell[Really_Select++].clSymbol   = clBlack;
//
            i_save = i;
           }
//
   break;
//
   case 3: // ������ ������� ������� ( aOp2, ������� 3 )
           if( n_Op==2 ) // ������ (�� ����) ������� ������� ( aOp2, ������� 3 )
            if( !strcmp(aOp2,aResult) ) // ���� ���������� aOp2 � aResult ?
           {
            Sel_Cell[Really_Select].Col = 4; // ��������� - ������� 4
            Sel_Cell[Really_Select].Row = i + 1;
            Sel_Cell[Really_Select].clBackground = DCT.clOperandOperation;
            Sel_Cell[Really_Select++].clSymbol   = clBlack;
//
            i_save = i;
           }
//
   break;
//
   case 5: // �������-�������� ( aPredicat, ������� 5 )
           if( !strcmp(aResult,aPredicat) || // ������ ���������� "���" ...
            ( ( aPredicat[0]==symbolNot_1 || aPredicat[0]==symbolNot_2 ) && // ������ ������ - ���������
               !strcmp(aResult,&aPredicat[1] ) ) ) // ���������� ������� �� ������� �������
           {
            Sel_Cell[Really_Select].Col = 4; // ��������� - ������� 4
            Sel_Cell[Really_Select].Row = i + 1;
            Sel_Cell[Really_Select].clBackground = DCT.clOperandOperation;
            Sel_Cell[Really_Select++].clSymbol   = clBlack;
//
           i_save = i;
           }
//
   break;
//
   default: break;
//
  } // ����� sawitch( Col )
//
 } // ����� ����� for( INT i=0; i<Really_Set; i++ ) // �� ���� ������� Mem_Instruction[]
////////////////////////////////////////////////////////////////////////////////
//
 mS->Repaint();
//
 if(i_save != i_save_no) // ����������� �������!
  t_printf( "\n-I- %s(): %d-� ������� ���������� #%d ������� �� ���������� ���������� ���������� #%d -I-",
            __FUNC__ + 5, Col-1, mS->Row-1, i_save); // (+5) ����������� �� TF1::
 else // ����������� �� �������..
  t_printf( "\n-I- %s(): %d-� ������� ���������� #%d �� ������� �� ���������� ���������� ������� ���������� !!! -I-",
            __FUNC__ + 5, Col-1, mS->Row-1); // (+5) ����������� �� TF1::
//
} //--- ����� TF1::Operand_toResult --------------------------------------------


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void __fastcall TF1::OnPopup_Determine_Links(TObject *Sender)
{ // ���������� ��� ���������� SG_PopUpMenu
} //----- ����� TF1::OnPopup_Determine_Links -----------------------------------


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void __fastcall TF1::SG_MouseUp(TObject *Sender, TMouseButton Button,
                                TShiftState Shift, int X, int Y)
{ // �������������� ����� ���� �� ������ ������ ���� ��� SG_PopUpMenu->AutoPopup=false
 int Col,Row;
//
 if( !Button==mbRight ) // "�����" ������� ������ ������ ������ ����
  return;
//
 SG_Instruction->MouseToCell( X,Y, Col,Row ); // �� X,Y �������� Col,Row ��� ������� SG_Set
//
 SG_Instruction->Col = Col; // �������� ������
 SG_Instruction->Row = Row;
//
 if( X==-1 || Y==-1 ) // ������ ���� ��������� ��� SG_Set
  return;
//
 if( Col<2 || Col>5 ) // ����������� ������ ������� 2-5
  return;
//
 char *p = &SG_Instruction->Cells[Col][Row].c_str()[0]; // ����� ����� ������ ������...
//
 if( !strlen( p ) ) // ������ ������
  return;
//
 if( !strcmp( p , trueLowerCase ) || // � ������ �� 'true'
     !strcmp( p , falseLowerCase ) ) // � ������ �� 'false'
  return;
//
// ----- ����������� ���������� ��������� ������������ ���� SG_PopUpMenu -------
//
 OR_1->Enabled = false; // ������� ����������� ��� �������� ������������ ����
 RO_1->Enabled = false;
//
 if( Col==4 ) // ������ ������� ���������
  RO_1->Enabled = true; // �������������� ������� ���� "��������->���������"
//
 if( Col==2 || Col==3 || Col==5 ) // ������ ������� �������-1 "���" �������-2 "���" GHTLBRFB
  OR_1->Enabled = true; // �������������� ������� ���� "���������->��������"
//
 TPoint P = GetClientOrigin(); // ��� ����������� ���������...
//
 SG_PopUpMenu->Popup( X+P.x+SG_Instruction->Left, Y+P.y+SG_Instruction->Top ); // "���������" ���� SG_PopUpMenu
//
} // ----- ����� TF1::SG_MouseUp -----------------------------------------------


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void __fastcall  // ������������ ��������� � Mem_Instrucrtions[]
Mixed_Instructions()
{
 INT i1,i2; // ������ �������������� ���������� � Mem_Instruction[]
 ::time_t t; // ��������� �������� ��� ������� ��������� �����  (������� �����)
 mi vMI; // ������� ������� �������� Mem_Instruction[]
//
 do_Stop // "���������" ��� ������ ����������
//
 srand( (unsigned) time( &t ) ); // ������������� ������� ��������� ����� ������� ��������
//
 for( INT i=0; i<100*Really_Set; i++ ) // ������������ 100 * Really_Sets ���
 {
  i1 = random( Really_Set ), // "������" ����� ���������� (���� �������� � ����)
  i2 = random( Really_Set ); // "�����" ����� ����������
//
  vMI = Mem_Instruction[i1]; // ��������� "������"
  Mem_Instruction[i1] = Mem_Instruction[i2]; // #i1 <- #i2
  Mem_Instruction[i2] = vMI; // #i2 <- #i1
 }
//
 SBM0->Text = " ���������� ���������� ��������� �������..."; // ����� ������ � StatusBarMain
 MessageBeep( MB_ICONEXCLAMATION ); // �����������...
 Delay( 1000 );
//
 do_Run // "��������" ��� ������ ����������
//
} //-----  ����� Mixed_Instructions  -------------------------------------------

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
bool __fastcall Read_Instructions()
{ // ������ ���������� �� ����� FileNameSetPrP � ������ �������� Mem->Set[]
 FILE *fptr; // ��������� �� ��������� ����
 char str[_1024], // ������ ��� ���������� � ����������� ����������
      tmp[_512], // ������� ������
      Set[_SET_LEN], // ��������� ����������
      *p, *p1, *p2,
      fmt[]="������� (%d)"; // ��� ������ �� �����...
 bool flagPredicate; // TRUE ���� � ���������� ��������� ���� ���������
//
 Process_Macros(); // ���������� ��������������
//
 if(!(fptr = fopen(FileNameSetPrP, "r"))) // ���� ����� ������������� ������� �� �������...
  return false;
//
 snprintf( tmp,sizeof(tmp), " �������� ���� %s [*%s]", ExtractFileName(FileNameSetPrP), ExtPrP ); // ����� � ������ �������
 SBM2->Text = tmp;
//
////////////////////////////////////////////////////////////////////////////////
//
 for( INT i=0; i<max_Instruction; i++ ) // �� ������� ����������
  {
   if(fgets(str, sizeof(str), fptr) == NULL) // ������ ������ �� fptr
    break; // ���� ������ ���������, ������� fgets ���������� NULL
//
   if(str[strlen(str)-1] == 10) // ���� � ����� ������ ����� ������ (10) � ����������)...
    str[strlen(str)-1] = ' ';   // ... �� ������� �� ������ !
//
// ----- ��� Tab � ������ str �������� �� ������� ------------------------------
   for( INT i=0; i<strlen(str); i++ ) // �� ���� �������� ������...
    if( str[i] == VK_TAB ) // ���� i-��� ������ ���� Tab (9/0x9)...
     str[i] = VK_SPACE; // �� �������� ��� �� ������ (32/0x20) !
//
   DSTA( str ); // ������ ������ str �� ���������� � ������������� Tabs �  ��������
//
   if( !strlen(str) || // ���� ����� ������ �������...
       str[0]==startComments[0] || // ��� ������ ���������� � ";"...
       str[0]==':' ) // ������ ������...
    {
     i -- ; // ������ ����������...!
     continue;
    }
//
// ������ �������� (parsing) ������ ���������� str /////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
   if( !strpbrk(str,startComments) ) // ���� ";" � ������ ���!
    strcpy(Mem_Instruction[i].Comment, " "); // ������, ����������� ������....
   else
   {
    strcpy(Mem_Instruction[i].Comment, strstr(str,startComments)); // ��� ��� �� ";" - ���� �����������
    Mem_Instruction[i].Comment[0] = ' '; // �������� ";" ��������
    DSTA( Mem_Instruction[i].Comment ); // ����������� �� ���������� � �������� ��������
   }
//
   strtok(str,startComments); // ����������� (������ � ";") ������ !
   DSTA( str ); // "������" ����� �������� �������...
   strcat(str, " "); // �������� 1 ������ �� ������ ������ (��� ����������� ��������)
//
   p = strtok(str, " "); // "����������" ��������� �������
//
   strcpy(Set, AnsiUpperCase(p).c_str()); // ��������� ��������� � Set (AnsiUpperCase �������� � ���������)
   strcpy(Mem_Instruction[i].Set, Set); // ��������� ��������� ���������� � Mem_Instruction[]
//
// ---- ������ !false, ~false, !true, ~true � ���� ���������--------------------
//
////////////////////////////////////////////////////////////////////////////////
   flagPredicate = !is_Predicat( Set ); // TRUE ���� � ���������� ��������� ���� ���������
////////////////////////////////////////////////////////////////////////////////
//
   switch( Get_CountOperandsByInstruction( Set ) ) // ����� ������� ��������� ���������� Set
   {
    case 1: p = strtok(NULL, ","); // ����� ������������� �������� aOp1.........
            p ? strcpy(Mem_Instruction[i].aOp1, p) :
                strcpy(Mem_Instruction[i].aOp1, notDefined);
            DSTA( Mem_Instruction[i].aOp1 );
            strcpy(Mem_Instruction[i].aOp2, "\0"); // ������� ����������� !!!
//
            p = strtok(NULL, "?,"); // ����� ���������� aResult ... '?' ��� ','
            p ? strcpy(Mem_Instruction[i].aResult, p) :
                strcpy(Mem_Instruction[i].aResult, notDefined);
            DSTA( Mem_Instruction[i].aResult );
//
            if( flagPredicate ) // ���� ��������� ��������
            {
             p = strtok(NULL, " "); // ����� ����� ��������� aPredicate.........
//
             p ? strcpy(Mem_Instruction[i].aPredicat, p) :
                 strcpy(Mem_Instruction[i].aPredicat, "true");
// ---- ������ ������� !false, ~false, !true, ~true � ���� ��������� -----------
             if( ( Mem_Instruction[i].aPredicat[0] == symbolNot_1 ||
                   Mem_Instruction[i].aPredicat[0] == symbolNot_2 ) ) // ���������� � '!' ��� '~'
             {
              if( !strcmp( AnsiLowerCase(&Mem_Instruction[i].aPredicat[1]).c_str(),falseLowerCase ) ) // ���� ������� �� ������� ������� "false"'
               strcpy( Mem_Instruction[i].aPredicat, trueLowerCase );
//
              if( !strcmp( AnsiLowerCase(&Mem_Instruction[i].aPredicat[1]).c_str(),trueLowerCase ) )  // ���� ������� �� ������� ������� "true"'
               strcpy( Mem_Instruction[i].aPredicat, falseLowerCase );
             }
// ---- �����  ������� !false, ~false, !true, ~true � ���� ��������� -----------
            if( !strcmp( AnsiLowerCase(p).c_str(), trueLowerCase) )
             strcpy(Mem_Instruction[i].aPredicat, trueLowerCase);
            if( !strcmp( AnsiLowerCase(p).c_str(), falseLowerCase) )
             strcpy(Mem_Instruction[i].aPredicat, falseLowerCase);
            }
//
             Mem_Instruction[i].fExecOut   = false;
             Mem_Instruction[i].fAddBuffer = false;
//
             break;
//
    case 2: p = strtok(NULL, ","); // ����� ������� �������� aOp1...............
            p ? strcpy(Mem_Instruction[i].aOp1, p) :
                strcpy(Mem_Instruction[i].aOp1, notDefined);
            DSTA( Mem_Instruction[i].aOp1 );
//
            p = strtok(NULL, ","); // ����� ������� �������� aOp2...............
            p ? strcpy(Mem_Instruction[i].aOp2, p) :
                strcpy(Mem_Instruction[i].aOp2, notDefined);
            DSTA( Mem_Instruction[i].aOp2 );
//
            p = strtok(NULL, "?,"); // ����� ���������� aResult ... '?' ��� ','
            p ? strcpy(Mem_Instruction[i].aResult, p) :
                strcpy(Mem_Instruction[i].aResult, notDefined);
// ���������� ���������� ����������� ���������� � ������ ���������� SUB y08(07), temp_02
// ������ ����������� SUB y08(07), temp_02, temp_03 ... �������!!! �������� - ��� ���� ���������
            DSTA( Mem_Instruction[i].aResult );
//
            if( flagPredicate ) // ���� ��������� ��������
            {
             p = strtok(NULL, " "); // ����� ����� ��������� aPredicate.........
//
             p ? strcpy(Mem_Instruction[i].aPredicat, p) :
                 strcpy(Mem_Instruction[i].aPredicat, "true");
// ---- ������ ������� !false, ~false, !true, ~true � ���� ��������� -----------
             if( ( Mem_Instruction[i].aPredicat[0] == symbolNot_1 ||
                   Mem_Instruction[i].aPredicat[0] == symbolNot_2 ) ) // ���������� � '!' ��� '~'
             {
              if( !strcmp( AnsiLowerCase(&Mem_Instruction[i].aPredicat[1]).c_str(),falseLowerCase ) ) // ���� ������� �� ������� ������� "false"'
               strcpy( Mem_Instruction[i].aPredicat, trueLowerCase );
//
              if( !strcmp( AnsiLowerCase(&Mem_Instruction[i].aPredicat[1]).c_str(),trueLowerCase ) )  // ���� ������� �� ������� ������� "true"'
               strcpy( Mem_Instruction[i].aPredicat, falseLowerCase );
             }
// ---- �����  ������� !false, ~false, !true, ~true � ���� ��������� -----------
             if( !strcmp( AnsiLowerCase(p).c_str(), trueLowerCase ) )
              strcpy( Mem_Instruction[i].aPredicat, trueLowerCase );
             if( !strcmp( AnsiLowerCase(p).c_str(), falseLowerCase ) )
              strcpy( Mem_Instruction[i].aPredicat, falseLowerCase );
            }
//
             Mem_Instruction[i].fExecOut   = false;
             Mem_Instruction[i].fAddBuffer = false;
//
             break;
//
   default: return true;

   } // ����� switch...
//
   Really_Set = i + 1 ; // �������� ����� ���������� (�� 1 ������, ��� ���� i � ����)
//
   F1->Label_Set->Caption = Format(fmt, OPENARRAY(TVarRec, (int(i+1)) )); // ����� �� �����
   F1->Label_Set->Repaint(); // ������������
//
  } // ����� ����� �� ������� � ����� FileNameSets
//
 fclose(fptr);
//
 Vizu_Instructions(); // ��������������� ��� ����������
//
 return true ;
//
} // -------- ����� Read_Instructions ------------------------------------------


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void __fastcall Save_All_Protocols_To_Out_Dir()
{ // ���������� ���� ���������� ������� ������� � ������� � *.set � ������� � ���������� Out!Data
 char tmp[_512], cnst[_512];
//
 do_Stop // "���������" ��� ������ ����������
//
 snprintf( cnst, sizeof(cnst), "!%s!AIU=%d_Param=%d_Prior=%d!%s.txt", ExtractFileName(FileNameSet),
                                 max_Proc, how_Calc_Param, how_Calc_Prior, uniqueStr ); // ���������� ����� ����� �����
//
 Save_Protocol_Master(); // ��������� ������� �������� (*.pro)
//  snprintf( tmp, sizeof(tmp), "%s\\pro%s", NameSubDirOutData, cnst );
  snprintf( tmp, sizeof(tmp), "%s%s\\pro%s", ExtractFilePath(ParamStr(0)), NameSubDirOutData, cnst );
//
  MoveFile( ChangeFileExt( FileNameSet, ".pro").c_str(), tmp ); // ������� � ���������� NameSubDirOutData (Out!Data)
  strcpy( SF.savePRO, tmp ); // ��������� � ������� ��� ������������� � Upload_Data()
//
 Save_Protocol_AIU(); // ��������� �������� ������������ ��� �� ������� (*.tpr)
  snprintf( tmp, sizeof(tmp), "%s%s\\tpr%s", ExtractFilePath(ParamStr(0)), NameSubDirOutData, cnst );
  MoveFile( ChangeFileExt( FileNameSet, ".tpr").c_str(), tmp );
//  strcpy( SF.saveTPR, tmp ); // ��������� � ������� ��� ������������� � Upload_Data()
//
 Save_Protocol_Data(); // ��������� �������� ������������ ������ (*.dat)
  snprintf( tmp, sizeof(tmp), "%s%s\\dat%s", ExtractFilePath(ParamStr(0)), NameSubDirOutData, cnst );
  MoveFile( ChangeFileExt( FileNameSet, ".dat").c_str(), tmp );
//  strcpy( SF.saveDAT, tmp );
//
 Save_Protocol_ExecInstructions(); // ��������� �������� ���������� ���������� (*.tst)
  snprintf( tmp, sizeof(tmp), "%s%s\\tst%s", ExtractFilePath(ParamStr(0)), NameSubDirOutData, cnst );
  MoveFile( ChangeFileExt( FileNameSet, ".tst").c_str(), tmp );
//  strcpy( SF.saveTST, tmp );
//
 Save_Protocol_ConnectedGraph(); // ��������� �������� ��������� � ������.����� (*.coi)
  snprintf( tmp, sizeof(tmp), "%s%s\\coi%s", ExtractFilePath(ParamStr(0)), NameSubDirOutData, cnst );
  MoveFile( ChangeFileExt( FileNameSet, ".coi").c_str(), tmp );
//  strcpy( SF.saveCOI, tmp );
//
 Save_IGA(); // // ��������� ���� ������ ��� � ������������������� (*.gv)
  snprintf( tmp, sizeof(tmp), "%%s\\gv%s", ExtractFilePath(ParamStr(0)), NameSubDirOutData, cnst );
  MoveFile( ChangeFileExt( FileNameSet, ".gv").c_str(), tmp );
//  strcpy( SF.saveGV, tmp );
//
 Extended_Save_IGA(); // ��������� ������ � ������� ���������� ���������� (*.mvr)
  snprintf( tmp, sizeof(tmp), "%s%s\\mvr%s", ExtractFilePath(ParamStr(0)), NameSubDirOutData, cnst );
  MoveFile( ChangeFileExt( FileNameSet, ".mvr").c_str(), tmp );
//  strcpy( SF.saveMVR, tmp );
//
 SBM0->Text = " ��� ����� ���������� ���������...";
//
 do_Run // "��������" ��� ������ ����������
//
 Upload_Data( 1 ); // ��������� PRO (etc) - ����� �� ������ ( Rule == 1 )
//
} // --- ����� Save_All_Protocols_To_Out_Dir------------------------------------


////////////////////////////////////////////////////////////////////////////////
void __fastcall TF3::Series_AIU_GetMarkText(TChartSeries *Sender, int ValueIndex, AnsiString &MarkText)
{ // ��������� ������ ����� �� "�����" (������� ����������)
//
 MarkText = "$" + AnsiString( GetSubString(mTpr->Strings[ValueIndex].c_str(), 1,10) ).TrimLeft()
             + "/"
            "#" + AnsiString( GetSubString(mTpr->Strings[ValueIndex].c_str(),41,50) ).TrimLeft()
            + "/"
            + Mem_Instruction[atoi( GetSubString(mTpr->Strings[ValueIndex].c_str(),41,50) )].Set ;
//            + "/"
//            + AnsiString( GetSubString(F1->Tpr->Strings[ValueIndex].c_str(),31,40) ).TrimLeft()
//            + "/"
//            + AnsiString( ValueIndex ) ;
//
} //----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
void __fastcall TF3::Series_AIU_AfterDrawValues(TObject *Sender)
{ // ���������� ������������ ������ "�����"
 Series_AIU->Pointer->VertSize = Chart_AIU->LeftAxis->CalcSizeValue(0.3);
} //----------------------------------------------------------------------------

