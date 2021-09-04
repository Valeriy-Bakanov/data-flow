#define TEST_PRINT  /* �������� ������ */ \
/*
t_printf( "\n-=-#%d/%d/%d %d-%d-%d %s{%f} %s{%d}\n", \
              i_Set,i,Rule, \
              flagNot,flagPredicat,flagPredicat_TRUE, \
              aResult, Result, \
              aPredicat, \
              Mem_Instruction[i].fPredicat_TRUE );
/*
t_printf( "\n-=- ������������� ���������� (i_Set): #%d [%s %s,%s,%s(%f),|%s|]  {%d/%d/%d|%d/%d/%d}\n\
-=- ��������� ����������    (i/Rule): #%d/%d [%s %s,%s,%s,|%s|]  {%d/%d/%d/%d|%d/%d}\n",\
i_Set, \
Mem_Instruction[i_Set].Set, Mem_Instruction[i_Set].aOp1, Mem_Instruction[i_Set].aOp2, \
aResult, Result, Mem_Instruction[i_Set].aPredicat, \
  Mem_Instruction[i_Set].fPredicat, Mem_Instruction[i_Set].fPredicat_TRUE,  Mem_Instruction[i_Set].fSpeculateExec, \
  Mem_Instruction[i_Set].fExec, Mem_Instruction[i_Set].fExecOut, Mem_Instruction[i_Set].fAddBuffer, \
i, Rule, \
Mem_Instruction[i].Set, Mem_Instruction[i].aOp1, Mem_Instruction[i].aOp2, Mem_Instruction[i].aResult, \
  Mem_Instruction[i].aPredicat, \
  flagNot,flagPredicat,flagPredicat_TRUE, SpeculateExec, \
  Mem_Instruction[i].fPredicat_TRUE, Mem_Instruction[i].fSpeculateExec );
*/
#define DO_OPS_2 /* ������� 2-� ��������� � ���������� */ \
if( Ready_Op1 ) { \
 MI_FOP1(i) = true; \
 snprintf(tmp,sizeof(tmp), " #%d/%d(1|2)", i,Rule); strcat(strInfoLine, tmp); } \
/*-*/ \
if( Ready_Op2 ) { \
 MI_FOP2(i) = true; \
 snprintf(tmp,sizeof(tmp), " #%d/%d(2|2)", i,Rule); strcat(strInfoLine, tmp); } \
/*-*/ \
if( MI_FOP1(i) && MI_FOP2(i) ) { \
 snprintf(tmp,sizeof(tmp), " #%d/%d(*|2)", i,Rule); strcat(strInfoLine, tmp); }
/*-*/
#define DO_OPS_1 /* ������� 1-�� �������� � ���������� */ \
if( Ready_Op1 ) { \
 MI_FOP1(i) = true; \
 snprintf(tmp,sizeof(tmp), " #%d/%d(1|1)", i,Rule); strcat(strInfoLine, tmp); \
 snprintf(tmp,sizeof(tmp), " #%d/%d(*|2)", i,Rule); strcat(strInfoLine, tmp); }
/*-*/
#define TURN_ON_FP_TRUE /* �������� ��� ��� ��������� ������ ���������� �����-��������� */ \
if( flagPredicat_TRUE ) \
 Mem_Instruction[i].fPredicat_TRUE = true;  // ��������� ��� ����-�������� ��� ��������� ������
/*-*/
#define TURN_ON_FSPECUL_EXEC_2 /* ���������� ��� ������ �������������� ���������� (2 ��������)  */ \
if(  MI_FOP1(i) && MI_FOP2(i) && /* �� ������ ���������� 1-� � 2-� �������� ������ */ \
  ( ( flagPredicat && /* ��� �����-���������� ��������� � �������� */ \
     !Mem_Instruction[i].fPredicat_TRUE ) ||  /* �������� �����-�������� ������� */ \
     !strcmp( aPredicat, falseLowerCase ) ) ) /* ����������� false */ \
 Mem_Instruction[i].fSpeculateExec = true; /* ��������� ���� ��� ��������� ������ � ������� ������� */
//
#define TURN_ON_FSPECUL_EXEC_1 /* ���������� ��� ������ �������������� ���������� (1 �������)  */ \
if( MI_FOP1(i) && /* �� ������ ���������� 1-� ������� ����� */ \
  ( ( flagPredicat && /* ��� �����-���������� ��������� � �������� */ \
   !Mem_Instruction[i].fPredicat_TRUE ) ||  /* �������� �����-�������� ������� */ \
   !strcmp( aPredicat, falseLowerCase ) ) ) /* ����������� false */ \
 Mem_Instruction[i].fSpeculateExec = true; /* ��������� ���� ��� ��������� ������ � ������� ������� */
//
#define ADD_TO_BUFF_2 /* �������� ���������� � ����� ��� ������������ ���������� (2 ��������) */ \
if( MI_FOP1(i) && MI_FOP2(i) ) /* ����� ���������� � 1-�� � 2-�� ��������� */ \
 Add_toBuffer( i, Rule ); /* �������� ���-������� � ����� ������ ��� ���������� */
//
#define ADD_TO_BUFF_1 /* �������� ���������� � ����� ��� ������������ ���������� (1 �������) */ \
if( MI_FOP1(i) ) /* ����� ���������� � 1-�� ���������f */ \
 Add_toBuffer( i, Rule ); /* �������� ���-������� � ����� ������ ��� ���������� */
//
#define ADD_TO_BUFF_SPECUL_2 /* �������� ���������� � ����� � ������������ �������������� ���������� (2 ��������) */ \
if( MI_FOP1(i) && MI_FOP2(i) && /* �� ������ ���������� 1-� � 2-� �������� ������ */ \
  ( ( !SpeculateExec &&   Mem_Instruction[i].fPredicat_TRUE ) || /* ����������� ���������� */ \
  (    SpeculateExec && ( Mem_Instruction[i].fPredicat_TRUE || /* ����������� ���������� */ \
                          Mem_Instruction[i].fSpeculateExec ) ) ) ) /* ������������� ���������� */ \
 Add_toBuffer( i, Rule ); /* �������� ���-���������� � ����� ������ ��� ���������� */
//
#define ADD_TO_BUFF_SPECUL_1 /* �������� ���������� � ����� � ������������ �������������� ���������� (1 �������) */ \
if( MI_FOP1(i) && /* �� ������ ���������� 1-� ������� ����� */ \
  ( ( !SpeculateExec &&   Mem_Instruction[i].fPredicat_TRUE ) || /* ����������� ���������� */ \
  (    SpeculateExec && ( Mem_Instruction[i].fPredicat_TRUE || /* ����������� ���������� */ \
                          Mem_Instruction[i].fSpeculateExec ) ) ) ) /* ������������� ���������� */ \
 Add_toBuffer( i, Rule ); /* �������� ���-���������� � ����� ������ ��� ���������� */
//
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void __fastcall
Finalize_Only_SET( INT i_Set )
{ // ���������� ���������� SET ����� i_Set !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// ��������������, ��� ��� ��������� (����� ��������� �� ����)..................
// ���������� SET ����������� �� �����������, � ������� �������������� !!!
 char Set[_SET_LEN]="\0",
      aResult[_ID_LEN]="\0",
      aPredicat[_ID_LEN]="\0", // ���� ���������
      namePredicat[_ID_LEN] = "\0", // ��� ���� ��������� (��� ���������� ����� ���������)
      strInfoLine[_4096]="\0",
      tmp[_512]="\0";
 bool s_isPredicat, // TRUE, ���� ������������ (�������������) ���������� ���� ��������
      isPredicat, // TRUE, ���� ��������� ���������� ��������
      flagNot, // TRUE, ���� � ��������� ��������� ������ ������ ����� ����� ��������� "!" ��� "~"
      flagPredicat, // TRUE, ���� � ����� ��������� ���������� ���� ���������� (XXX ��� !XXX ��� ~XXX, ��� XXX == aResult)
      flagPredicat_TRUE, // TRUE, ���� � ��������� ���������� ���� ��������� TRUE (� ������ isNot)
      Ready_Op1,Ready_Op2 ; // ���������� ��� ��������� � ��������� ���������� � ������ ���������� � �������������
 short int Rule; //����������� ���������� � ������������� switch()
 REAL valPredicat; // �������� ����-��������� ����������� ����������
//
 if( !Regim ) // �� ��������� - ��������� ���� ---------------------------------
  return;
//
 REAL Result = StrToReal( Mem_Instruction[i_Set].aOp1, i_Set ); // ��������� �������� 1-�� �������� (���������) ���������� SET ����� i_Set
 strcpy( aResult, Mem_Instruction[i_Set].aResult ); // ������-����� ���������� ����������� ����������
//
 s_isPredicat = false; // is_Predicat( Mem_Instruction[i_Set].Set ); // � SET ������ FALSE
//
////////////////////////////////////////////////////////////////////////////////
 Add_toData( i_Set, aResult, Result ); // ������� � Mem_Data[] � ��� ������������
////////////////////////////////////////////////////////////////////////////////
//
// ��������� ���� ������������� ���������� SET .................................
//
 Mem_Instruction[i_Set].fExecOut = true; // ���������� ���� ���������� ����������
 mS->Cells[6][i_Set+1] = Vizu_Flags(i_Set); // ��������������� ��� � ������� SG_Sets
//
 t_printf( "-I- %s(){1}: ���������� #%d [%s] ��������� (%s) -I-",
            __FUNC__, i_Set, Line_Set(i_Set, 1, 0.0), Get_Time_asLine());
//
////////////////////////////////////////////////////////////////////////////////
// ��������� ���� ����� � ���� ���������, ����������� �� ����� � ������� aResult � ���� ���������� Mem_Sets[i_set]
//
 strcpy(strInfoLine, "\0"); // ������� strInfoLine
//
////////////////////////////////////////////////////////////////////////////////
 for( INT i=0; i<Really_Set; i++ ) // �� ����� ���� ���������� � Mem_Instruction[]
 {
  strcpy( Set, Mem_Instruction[i].Set ); // ... ��� ������� ��� ���������� ������ !
  if( is_SET( Set ) ) // ��� ���������� SET - �� ������������ ! ---------------
   continue;
//
  strcpy( aPredicat, Mem_Instruction[i].aPredicat ); // ����� �������� � aPredicat, �� ������� Mem_Instruction[i].aPredicat
//
  isPredicat = is_Predicat( Set ); // TRUE, ���� ��������� ���������� - ��������
//
//--- ���������, ���������� �� ��� ����� ��������� � '!' ��� '~' ...............
  if ( !isPredicat ) // ��� ���������� - �� ��������...
   flagNot = ( aPredicat[0] == symbolNot_1 || aPredicat[0] == symbolNot_2 )
               ? true : false; // TRUE, ���� � ���� aPredicat ������ ������ '!' ��� '~' (���������)
//
//--- ���������, ��������� �� ��� ������������ ���������� � ������ -------------
//--- ���������� � ���� ��������� ��������� ���������� -------------------------
  flagPredicat = false;  // ��������� ���������
  if( !isPredicat &&  // ��������� ���������� - �� ��������
       strcmp( aPredicat, trueLowerCase  ) && // "�" ��� �� ����������� true
       strcmp( aPredicat, falseLowerCase ) )  // "�" ��� �� ����������� false
   if( (  flagNot && !strcmp( &aPredicat[1], aResult ) ) || // ����-�������� ���������� (��� ���������� � '!' ��� '~' )
       ( !flagNot && !strcmp(  aPredicat,    aResult ) ) ) // ����-�������� �� ������������ (��� �� ���������� � '!' ��� '~' )
    flagPredicat = true;
//
//--- ������ ���������� �������� Result �� true ��� false � ������������ -------
//--- (� ������ ����������� true/false) ������������� flagPredicat_TRUE --------
//
  flagPredicat_TRUE = false; // ��������� ���������
  if( flagPredicat ) // ����������-�������� ����������, �� �������� ��� ����������
   if( ( flagNot && !Result ) || // ��� ���������� � '!' ��� '~' � Result==FALSE
      ( !flagNot &&  Result ) ) // ��� �� ���������� � '!' ��� '~' � Result==TRUE
    flagPredicat_TRUE = true;
//
//--- �������� ������������ ����������� true ��� false -------------------------
  if( !strcmp( aPredicat, trueLowerCase ) ) // ���� true...
   flagPredicat_TRUE = true;
  if( !strcmp( aPredicat, falseLowerCase ) ) // ���� false...
   flagPredicat_TRUE = false;
//
  Ready_Op1 = MI_AOP1( i ) ; // ���� ���������� �������� 2
  Ready_Op2 = MI_AOP2( i ) ; // ���� ���������� �������� 2
//
////////////////////////////////////////////////////////////////////////////////
  switch( Rule=10*Get_CountOperandsByInstruction(Set)+isPredicat )
  {
////////////////////////////////////////////////////////////////////////////////
   case 10: // ���� �������; const -> ��_�������� + (��������) flag-��������
//
   DO_OPS_1 // ��������� 1-�� �������� (�� 1-�� � ����������)
//
   TURN_ON_FP_TRUE // �������� ��� ��� ��������� ������ ���������� �����-���������
//
   TURN_ON_FSPECUL_EXEC_1 // ���������� ��� ������ �������������� ���������� (1 �������)
//
   TEST_PRINT // ���������� ������
//
   ADD_TO_BUFF_SPECUL_1 /* �������� ���������� � ����� � ������������ �������������� ���������� (1 �������) */ \
//
   break; // ����� Rule==10
//
////////////////////////////////////////////////////////////////////////////////
   case 11:  // ��� ��������; const -> ��������
//
   DO_OPS_1 // ��������� 1-�� �������� (�� 1-�� � ����������)
//
   TEST_PRINT // ���������� ������
//
   ADD_TO_BUFF_1 /* �������� ���������� � ����� ��� ������������ ���������� (1 �������) */
//
   break; // ����� Rule==11
//
////////////////////////////////////////////////////////////////////////////////
   case 20: // ��� ��������; const -> ��_�������� + (��������) flag-��������
//
   DO_OPS_2 // ��������� 2-� ��������� (�� 2-� � ����������)
//
   TURN_ON_FP_TRUE // �������� ��� ��� ��������� ������ ���������� �����-���������
//
   TURN_ON_FSPECUL_EXEC_2 // ���������� ��� ������ �������������� ���������� (2 ��������)
//
   TEST_PRINT // ���������� ������
//
   ADD_TO_BUFF_SPECUL_2 // �������� ���������� � ����� � ������������ �������������� ���������� (2 ��������)
//
   break; // ����� Rule==20
//
////////////////////////////////////////////////////////////////////////////////
   case 21: // ��� ��������; const -> ��������
//
   DO_OPS_2 // ��������� 2-� ��������� (�� 2-� � ����������)
//
   TEST_PRINT // ���������� ������
//
   ADD_TO_BUFF_2 // �������� ���������� � ����� ��� ������������ ���������� (2 ��������)
//
   break; // ����� Rule==21
//
////////////////////////////////////////////////////////////////////////////////
   default:
//
    t_printf( "\n-E- ������������ �������� i/Rule=%d/%d -E-\n", i,Rule );
//
   break; // ������ Rule �� ������ ---------------------------------------------
////////////////////////////////////////////////////////////////////////////////
//
  } // ����� switch �� Rule ��� i-��� ���������� -------------------------------
//
////////////////////////////////////////////////////////////////////////////////
//
   mS->Cells[6][i+1] = Vizu_Flags(i); // ��������������� ����� ������ ����������
//
  } // ����� for( INT i=0; i<Really_Set; i++ ) ---------------------------------
//  
////////////////////////////////////////////////////////////////////////////////
//
  Draw_AllTableInstructions(); // ��������� ����� ������ (����� ���������� ��� ���������� ������� SET'a )
//
  if( strlen(strInfoLine) ) // ���� � strInfoLine ���-�� ����������...
   t_printf( "-I- %s(){2}: �� ���������� ���������� #%d ����������� ����� ���������� ��������� � ����������: %s -I-",
              __FUNC__, i_Set, strInfoLine);
//
 Already_Exec ++ ; // ����� ��� ����������� ����������
//
 AttemptExecMaxInstructions_fromBuffer(); // �������� ��������� ��� ����� ������ ���-���������� �� ������
// ��� ������������ ���������� ���������� �� AttemptExecMaxInstructions_fromBuffer()
// ���������� ExecuteInstructions_Except_SET( i_Set )
//
 Vizu_Flow_Exec(); // ������������ �������� ����������� ����������
//
} // ----- ����� Finalize_Only_SET ---------------------------------------------


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void __fastcall // ���������� ��� ���������� ���������� ���������� �� ��� i_Proc
Finalize_Except_SET( INT i_Proc ) // ��� ������� ����� SET !!!!!!!!!!!!!!!!!!!!!
{ // ��������������� ����� ���������� � ������� ��������� ���� ����������, ���������
// �� ������� ��������� �� ���������� ���������� ������ �� ��� ����� i_Proc
 char Set[_SET_LEN]="\0",
      aResult[_ID_LEN]="\0",
      aPredicat[_ID_LEN]="\0", // ���� ���������
      namePredicat[_ID_LEN] = "\0", // ��� ���� ��������� (��� ���������� ����� ���������)
      strInfoLine[_4096]="\0",
      tmp[_512]="\0";
 bool s_isPredicat, // TRUE, ���� ������������ (�������������) ���������� ���� ��������
      isPredicat, // TRUE, ���� ��������� ���������� ���� ��������
      flagNot, // TRUE, ���� � ��������� ���������� ������ ������ ����� �����-��������� "!" ��� "~"
      flagPredicat, // TRUE, ���� � ����� ��������� ���������� ���� ���������� (XXX ��� !XXX ��� ~XXX, ��� XXX == aResult)
      flagPredicat_TRUE, // TRUE, ���� � ��������� ���������� ����-�������� TRUE (� ������ isNot)
      Ready_Op1,Ready_Op2 ; // ���������� ��� ��������� � ��������� ���������� � ������ ���������� � �������������
 short int Rule; //����������� ���������� � ������������� switch()
 REAL  valPredicat; // �������� ����-��������� ����������� ����������
//
 if( !Regim ) // �� ��������� - ��������� ���� ---------------------------------
  return;
//
 INT i_Set = Mem_Proc[i_Proc].i_Set; // �� ���� ��� ����������� ���������� ����� i_Set
//
 REAL Result = Mem_Proc[i_Proc].Result; // �������� ���������� ����������� ��������
//
 strcpy( aResult, Mem_Proc[i_Proc].aResult ); // ��������� ����� ���������� ���������� ���������� i_Set_Result
//
////////////////////////////////////////////////////////////////////////////////
// // ��� ������������� ���������� ��������� ���������� � ��� ������ �� ����� !
 if( !( SpeculateExec && Mem_Instruction[i_Set].fSpeculateExec ) )
 {
  Add_toData( i_Set, Mem_Proc[i_Proc].aResult, Result ); // �������� ��������� ������������� ����������
//
  t_printf( "-I- %s(){1}: ��� #%d ��������� ���������� #%d [%s] [%d/%d/%d ������] -I-",
            __FUNC__, i_Proc, i_Set, Line_Set(i_Set, 1, 0.0),
            Mem_Proc[i_Proc].tick_Start, localTick, localTick-Mem_Proc[i_Proc].tick_Start);
 }
//
////////////////////////////////////////////////////////////////////////////////
//
 s_isPredicat = is_Predicat( Mem_Instruction[i_Set].Set ); // TRUE, ���� ������������� ���������� ���� ��������
//
 Vizu_Data(); // ���������������...
//
////////////////////////////////////////////////////////////////////////////////
// �������� ������ � ����� ����� Tpr ��� ������� ������������� ���..............
//
 snprintf( strInfoLine,sizeof(strInfoLine), "%10d%10d%10d%10d%10d [%s]",
           i_Proc, Mem_Proc[i_Proc].tick_Start, localTick, localTick-Mem_Proc[i_Proc].tick_Start, i_Set,
           Line_Set(i_Set, -1, Result ) ) ;
//
 if( SpeculateExec && Mem_Instruction[i_Set].fSpeculateExec ) // ������������� ����������
  strcat( strInfoLine, SPECUL );
//
 mTpr->Add( strInfoLine ); // �������� ������ � ������ Tpr
// t_printf( "\n=-=-=-=%s\n", strInfoLine );
//
 strcpy(strInfoLine, "\0"); // ������� strInfoLine ��� ����������� �������������
//
 if( SpeculateExec && Mem_Instruction[i_Set].fSpeculateExec ) // ������������� ���������� !
  goto cont;
//
//==============================================================================
 for( INT i=0; i<Really_Set; i++ ) // �� ���� ����������� �� Mem_Instruction[]...
 {
//
  if( i == i_Set ) // ����� ������ ��� ������������ ����������..................
   continue;
//
  strcpy( Set, Mem_Instruction[i].Set ); // ... ��� ������� ��� ���������� ������ !
  if( is_SET(Set) ) // ���������� SET ��� ����� �������� ��������� !
   continue;
//
  strcpy( aPredicat, Mem_Instruction[i].aPredicat ); // ����� �������� � aPredicat, �� ������� Mem_Instruction[i].aPredicat
//
  isPredicat = is_Predicat( Set ); // TRUE, ���� ��� ���������� ��������
//
////////////////////////////////////////////////////////////////////////////////
//
//--- ���������, ���������� �� ��� ���������� ��������� � '!' ��� '~'
  if ( !isPredicat ) // ��� ���������� - �� ��������
   flagNot = ( aPredicat[0]==symbolNot_1 || aPredicat[0]==symbolNot_2 )
               ? true : false; // TRUE, ���� � ���� aPredicat ������ ������ '!' ��� '~'
//
//--- ���������, ��������� �� ��� ������������ ���������� � ������ -------------
//--- ���������� � ���� ��������� i-��� ���������� -----------------------------
  flagPredicat = false;  // ��������� ���������
  if( !isPredicat &&  // ��������� ���������� - �� ��������
       strcmp( aPredicat, trueLowerCase  ) && // "�" ��� �� ����������� true
       strcmp( aPredicat, falseLowerCase ) )  // "�" ��� �� ����������� false
   if( (  flagNot && !strcmp( &aPredicat[1], aResult ) ) || // ����-�������� ���������� (��� ���������� � '!' ��� '~' )
       ( !flagNot && !strcmp(  aPredicat,    aResult ) ) ) // ����-�������� �� ������������ (��� �� ���������� � '!' ��
    flagPredicat = true;
//
//--- ������ ���������� �������� Result �� true ��� false � ������������ -------
//--- (� ������ ����������� true/false) ������������� flagPredicat_TRUE --------
//
  flagPredicat_TRUE = false; // ��������� ���������
  if( flagPredicat ) // ����������-�������� ����������, �� �������� ��� ����������
   if( ( flagNot  && !Result ) || // ��� ���������� � '!' ��� '~' � Result==FALSE
       ( !flagNot &&  Result ) ) // ��� �� ���������� � '!' ��� '~' � Result==TRUE
    flagPredicat_TRUE = true;
//
//--- �������� ������������ ����������� true ��� false -------------------------
  if( !strcmp( aPredicat, trueLowerCase ) ) // ���� true...
   flagPredicat_TRUE = true;
  if( !strcmp( aPredicat, falseLowerCase ) ) // ���� false...
   flagPredicat_TRUE = false;
//
////////////////////////////////////////////////////////////////////////////////
//
  if(Mem_Instruction[i].fExec    || // ���� ���������� ����������� "���"
     Mem_Instruction[i].fExecOut || // ��� ��������� "���"
     Mem_Instruction[i].fAddBuffer) // ��� ��������� � �����
   continue;
//
  Ready_Op1 = MI_AOP1( i ) ; // ���� ���������� �������� 1
  Ready_Op2 = MI_AOP2( i ) ; // ���� ���������� �������� 2
//
////////////////////////////////////////////////////////////////////////////////
//
  switch( Rule = 100*Get_CountOperandsByInstruction(Set) + 10*s_isPredicat + isPredicat )
  {
////////////////////////////////////////////////////////////////////////////////
   case 100: // ���� �������; ��_�������� -> ��_�������� + (��������) flag-���������
//
   DO_OPS_1 // ��������� 1-�� �������� (�� 1-�� � ����������)
//
// TURN_ON_FP_TRUE( i ) - ���������� ������ ��� ��������->��_�������� (Rule==110 && Rule==210)
//
   TURN_ON_FSPECUL_EXEC_1 // ���������� ��� ������ �������������� ���������� (1 �������)
//
   TEST_PRINT // ���������� ������
//
   ADD_TO_BUFF_SPECUL_1 /* �������� ���������� � ����� � ������������ �������������� ���������� (1 �������) */ \
//
   break; // ����� Rule==100
//
////////////////////////////////////////////////////////////////////////////////
   case 101: // ���� �������; ��_�������� -> ��������
//
   DO_OPS_1 // ��������� 1-�� �������� (�� 1-�� � ����������)
//
   TEST_PRINT // ���������� ������
//
   ADD_TO_BUFF_1 // �������� ���������� � ����� ��� ������������ ���������� (1 �������)
//
   break; // ����� Rule==101
//
////////////////////////////////////////////////////////////////////////////////
   case 110: // ���� �������; �������T -> ��_�������� + (��������) flag-��������
//
   DO_OPS_1 // ��������� 1-�� �������� (�� 1-�� � ����������)
//
   TURN_ON_FP_TRUE // �������� ��� ��� ��������� ������ ���������� �����-���������
//
   TURN_ON_FSPECUL_EXEC_1 // ���������� ��� ������ �������������� ���������� (1 �������)
//
   TEST_PRINT // ���������� ������
//
   ADD_TO_BUFF_SPECUL_1 // �������� ���������� � ����� � ������������ �������������� ���������� (1 �������)
//
   break; // ����� Rule==110
//
////////////////////////////////////////////////////////////////////////////////
   case 111: // ���� �������; �������� -> ��������
//
   DO_OPS_1 // ��������� 1-�� �������� (�� 1-�� � ����������)
//
   TEST_PRINT // ���������� ������
//
   ADD_TO_BUFF_1 /* �������� ���������� � ����� ��� ������������ ���������� (1 �������) */ \
//
   break; // ����� Rule==111
//
////////////////////////////////////////////////////////////////////////////////
   case 200: // ��� ��������; ��_�������� -> ��_�������� + (��������) flag-��������
//
   DO_OPS_2 // ��������� 2-� ��������� (�� 2-� � ����������)
//
// TURN_ON_FP_TRUE( i ) - ���������� ������ ��� ��������->��_�������� (Rule==110 && Rule==210)
//
   TURN_ON_FSPECUL_EXEC_2 // ���������� ��� ������ �������������� ���������� (2 ��������)
//
   TEST_PRINT // ���������� ������
//
   ADD_TO_BUFF_SPECUL_2 // �������� ���������� � ����� � ������������ �������������� ���������� (2 ��������)
//
   break; // ����� Rule==200
//
////////////////////////////////////////////////////////////////////////////////
   case 201: // ��� ��������; ��_�������� -> ��������
//
   DO_OPS_2 // ��������� 2-� ��������� (�� 2-� � ����������)
//
   TEST_PRINT // ���������� ������
//
   ADD_TO_BUFF_2 // �������� ���������� � ����� ��� ������������ ���������� (2 ��������)
//
   break; // ����� Rule==201
//
////////////////////////////////////////////////////////////////////////////////
   case 210: // ��� ��������; �������� -> ��_�������� + ��������, ����-��������
//
   DO_OPS_2 // ��������� 2-� ��������� (�� 2-� � ����������)
//
   TURN_ON_FP_TRUE // �������� ��� ��� ��������� ������ ���������� �����-���������
//
   TURN_ON_FSPECUL_EXEC_2 // ���������� ��� ������ �������������� ���������� (2 ��������)
//
   TEST_PRINT // ���������� ������
//
   ADD_TO_BUFF_SPECUL_2 // �������� ���������� � ����� � ������������ �������������� ���������� (2 ��������)
//
   break; // ����� Rule==210
//
////////////////////////////////////////////////////////////////////////////////
   case 211: // ��� ��������; �������� -> ��������
//
   DO_OPS_2 // ��������� 2-� ���������
//
   TEST_PRINT // ���������� ������
//
   ADD_TO_BUFF_2 // �������� ���������� � ����� ��� ������������ ���������� (2 ��������)
//
   break; // ����� Rule==211
//
////////////////////////////////////////////////////////////////////////////////
   default: // ������ Rule �� ������ -------------------------------------------
//
    t_printf( "\n-E- ������������ �������� i/Rule=%d/%d -E-\n", i,Rule );
//
   break;
//
////////////////////////////////////////////////////////////////////////////////
//
  } // ����� switch �� Rule ��� i-��� ���������� -------------------------------
//
////////////////////////////////////////////////////////////////////////////////
//
   mS->Cells[6][i+1] = Vizu_Flags(i); // ��������������� ����� ����������
//
  } // ����� for( INT i=0; i<Really_Set; i++ ) ---------------------------------
//
////////////////////////////////////////////////////////////////////////////////
//
 if( strlen(strInfoLine) ) // ���� � strInfoLine ���-�� ������������...
  t_printf( "-I- %s(){2}: �� ���������� ���������� #%d/%d ����������� ����� ���������� ��������� � ����������: %s -I-",
             __FUNC__, i_Set, i_Proc, strInfoLine );
//
////////////////////////////////////////////////////////////////////////////////
cont: // ����� �������������� ���������� ���������� ============================
////////////////////////////////////////////////////////////////////////////////
//
// ������������� ���� ������������ ���������� ���������� .......................
 Mem_Instruction[i_Set].fExecOut = true; // ���������� ���� ����������_������������
// ������� ���� ����������_�����������
 Mem_Instruction[i_Set].fExec    = false; // ����� ���� ����������_�����������
//
 Draw_AllTableInstructions(); // ��������� ����� ������ (����� *.fExecOut ...)
//
////////////////////////////////////////////////////////////////////////////////
//
 Mem_Proc[i_Proc].Busy = false; // ��� ����� i_Proc ������ �������� !!! --------
//
 t_printf( "-I- %s(){3}: ��� #%d ����������� (%s) ����� ���������� ���������� #%d -I-",
            __FUNC__, i_Proc, Get_Time_asLine(), i_Set);
//
 Free_Proc ++ ; // ����� ��������� ��� ��������� �� 1 ==========================
//
////////////////////////////////////////////////////////////////////////////////
 sleep_for_vizu_buffer // ����-� ��� ������������ ������
////////////////////////////////////////////////////////////////////////////////
 AttemptExecMaxInstructions_fromBuffer(); // �������� ��������� ��� ����� ������ ���-���������� �� ������
// ��� ������������ ���������� ���������� �� AttemptExecMaxInstructions_fromBuffer()
// ���������� ExecuteInstructions_Except_SET( i_Set )
////////////////////////////////////////////////////////////////////////////////
 sleep_for_vizu_buffer // ������������� �����...
////////////////////////////////////////////////////////////////////////////////
//
 Already_Exec ++ ; // ����� ��� ����������� ����������
//
 Vizu_Flow_Exec(); // ������������ �������� ����������� ����������
//
} // --- ����� Finalize_Except_SET ---------------------------------------------







