//
void   __fastcall Expansion_SET_1D( char* s ); // ���������� ������� �������������� (���������� SET)
void   __fastcall Expansion_SET_2D( char* s );
void   __fastcall Expansion_1_2_Opd_1D( char* s, int nOpd ); // ��������� ���������� � 1-2 ����������
void   __fastcall Expansion_1_2_Opd_2D( char* s, int nOpd );
void   __fastcall SelectInstrForMacrosExpans_1D(); // ����� ���� ���������� ��� ���������� ��������������
void   __fastcall SelectInstrForMacrosExpans_2D(); // ����� ���� ���������� ��� ���������� ��������������
double __fastcall pcExpression( char* Expression, double Value_1, double Value_2 ); // ��������� � ������� double
int    __fastcall testIndex(char* str, char chr ); // ���������� ����� �� 'chr' ��������
//
void   __fastcall handlXXX_asVariable_1D( char* Field, INT i );
void   __fastcall handlXXX_asMassive_1D(  char* Field, INT i ); // ��������� ���� Field ���� Opd_1,Opd_2,Res ��� 1D-�������������
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
void   __fastcall handlXXX_asVariable_2D( char* Field, INT i, INT j );
void   __fastcall handlXXX_asMassive_2D(  char* Res, INT i, INT j ); // ��������� ���� Field ���� Opd_1,Opd_2,Res ��� 1D-�������������
//
void   __fastcall replace_Str( char* s, const char* sOld, const char* sNew ); // ������ cI �� sNew � ������ s
bool   __fastcall makeMassive_1D( char* str, INT i ); // ������� � ������������ (���������) ������� � 1D-�������������
bool   __fastcall makeMassive_2D( char* str, INT i ); // ...2D-�������������
void   __fastcall tf_printf( char* str ); // ����� ������ str �� ����� ��������� � ���� fptrOut (global)
//
// ����������� �� ����� ����������: "�����>0" � "������ ������ - �����" ��� "������=attrvar"
#define permissName(str) ( strlen(str) && /* �����>0 '�' */ \
                         ( isalpha(str[0]) || /* ������=����� '���' */ \
                           !memcmp(str,attrVar,strlen(attrVar)) ) ) // ������!=attrVar)
//
#define test_isMassive_1D(s) \
                   ( strchr(s,'[') && strchr(s,']') && /* ������� '[' � ']' ������������ � ������ s */ \
               (int)(strchr(s,'[')-s)>=1 && /* ������ '[' �� ������ � ������ s */ \
               (int)(strchr(s,']')-strchr(s,'['))>=1 ) // ����� '[' � ']' �� ����� ������ �������
//
#define test_isMassive_2D(s) \
                   ( strchr(s,'[') && strchr(s,':') && strchr(s,']') && /* ������� '[', ':' � ']' ������������ � ������ s */ \
               (int)(strchr(s,'[')-s)>=1 && /* ������ '[' �� ������ � ������ s */ \
               (int)(strchr(s,':')-strchr(s,'['))>=1 && /*  ����� '[' � '[' �� ����� ������ ������� */ \
               (int)(strchr(s,']')-strchr(s,':'))>=1 ) // ����� ';' � ']' �� ����� ������ �������
//
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void __fastcall tf_printf( char* str )
{ // ����� ������ str �� ����� ��������� + � ���� fptrOut (global)
 t_printf( "%s", str );
 fprintf( fptrOut, "%s\n", str );
} // ----- ����� tf_printf -----------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
bool __fastcall Process_Macros()
{ // ������ ���������� �� ����� FileNameSet, ���� ��������� ��������, ����������
// ���� �������� � mBody (��� TStringList), �������� ���������� �������� � ������
// ����� ���� � ������ ������� (���������� ��������)
 char str[_1024]="\0", strSave[_1024]="\0"; // ������ ��� ���������� � ����������� ���������� + ���������� �������� ������
//
 flagMacroTitle_1D = flagMacroTitle_2D = false ; // !!!!!! ������� (global) !!!!!
//
 if( !(fptrIn = fopen( FileNameSet, "r") ) ) // ���� fptrIn (global) ������� �� �������...
  return false;
//
 strcpy( FileNameSetPrP, ChangeFileExt(FileNameSet,ExtPrP).c_str() ); // ��� ����� ���������� ����� �������������
 if( !(fptrOut = fopen( FileNameSetPrP, "w") ) ) // ���� ���������� ����� ������������� fptrOut (global) ������� �� �������...
  return false;
//
////////////////////////////////////////////////////////////////////////////////
//
 for( INT i=0; i<max_Instruction; i++ ) // �� ������� ����������
  {
   if(fgets(str, sizeof(str), fptrIn) == NULL) // ������ ������ �� fptr
    break; // ���� ������ ���������, ������� fgets ���������� NULL
//
   strcpy( strSave, str ); // �������� �������� ������
//
   if(str[strlen(str)-1] == 10) // ���� � ����� ������ ����� ������ (10) � ����������)...
    str[strlen(str)-1] = ' ';   // ... �� ������� �� ������ !
//
// ----- ��� Tab � ������ str �������� �� ������� ------------------------------
   for( INT i=0; i<strlen(str); i++ ) // �� ���� �������� ������...
    if( str[i] == VK_TAB ) // ���� i-��� ������ ���� Tab (9/0x9)...
     str[i] = VK_SPACE; // �� �������� ��� �� ������ (32/0x20) !
//
   DSTA( str ); // ������ ������ str �� ���������� � ������������� Tabs � ��������
//
   if( !strlen(str) || // ���� ����� ������ �������...
       str[0]==startComments[0] || // ��� ������ ���������� � ";"...
       str[0]==';' ) // ������ ������ ( ':' ������ ";" )
    goto cont;
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
  i_env_BypassMacro = setjmp( env_BypassMacro ); // ���������� ����� ��������
  if( i_env_BypassMacro ) // �� ���� - ������, ���� ��� ������ !!!!!!!!!!!!!!!!!
   goto label_BypassMacro;
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
   if( !makeMassive_1D( str, i ) ||  // ���������� ������� � 1D-������������
// continue; // ������ str � �������� ���� fptrOut ����������� �� �����
       !makeMassive_2D( str, i ) ) // ...� 2D-������������
    continue; // ������ str � �������� ���� fptrOut ����������� �� �����
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
label_BypassMacro: // ���� ��������� �� longjmp --------------------------------
////////////////////////////////////////////////////////////////////////////////
cont: // ��� �������� ������ � ������������ ����������� � ���� FileNameSetPrP --
////////////////////////////////////////////////////////////////////////////////
// ----- ��������� ������ � �������� ���� FileNameSetPrP -----------------------
 fputs( strSave, fptrOut ); // ��������� ������ ��������� �����
//
 } // ����� ����� i �� ������� � ����� FileNameSets
////////////////////////////////////////////////////////////////////////////////
//
 fclose( fptrIn  ); // ��������� ��� �������� �����
 fclose( fptrOut );
//
 return 0; // �� Ok
//
} // --------- ����� Process_Macros --------------------------------------------


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
bool __fastcall makeMassive_1D( char* str, INT i )
{ // ������� � ������������ (���������) ������ � 1D-������������� --------------
// ��� �������� false ������ str � �������� ���� ����������� �� ����� ! --------
  if( !strncmp( str, "for[", 4 ) && // ���� � ������ str ��������� 'for[' (������ 4 �������) '�'
       sscanf( str, "for[%c]=%d,%d,%d{", &cI, &minI,&maxI,&dI ) == 4 ) // ��� 4 ���� ��������� ���������..!
  { // ������ if( !strncmp( str, "for[", 4 ) && ...
//
   if( !dI || // ������� �������� ��������� ���������� ����� (�������� �  ����������� ������� )
     (maxI>minI && dI<=0) || (maxI<minI && dI>=0) )
   {
    t_printf( "\n-M- ����������� �������� ��������� [%d,%d,%d] ���������� ����� '%c' -M-\n", minI,maxI,dI,cI );
    return false; // ������ str � �������� ���� fptrOut ����������� �� �����
   }
//
   for_ID ++ ; // ����� for[... � ������ ���������
   flagMacroTitle_1D = true; // ����� ������ � ���������� 1D-�������
//
   mBody->Clear(); // ������ ������ �������
   mBody->Add( str ); // ��������� ��������� 1D-�������
//
   mExpand->Clear(); // ������ ���������� �������
   return false; // ������ str � �������� ���� fptrOut ����������� �� �����
  } // ����� if( !strncmp( str, "for[", 4 ) && ...
//
  if( flagMacroTitle_1D && str[0] != '}' ) // ���� �� ���� "}" � ������ ����� flagMacroTitle=true
  {
   mBody->Add( str ); // ��������� � mBody ������ �������
   return false; // ������ str � �������� ���� fptrOut ����������� �� �����
  }
//
  if( flagMacroTitle_1D && str[0] == '}' ) // ����� ��������� ������� � ������ ���������� "}"
  {
   flagMacroTitle_1D = false; // � ������ ���������� "}"
   mBody->Add( str ); // ��������� � mBody ������ "}"
// endN = i; // �������� ����� ������ � ��������� �������� �������
//
////////////////////////////////////////////////////////////////////////////////
   SelectInstrForMacrosExpans_1D(); // ����� ���� ���������� ��� ���������� ��������������
////////////////////////////////////////////////////////////////////////////////
   tf_printf( Format("; \n; ������ ���������� �������: ��������� 'for[%s]=%d,%d,%d {'\n; for_ID: %d", // cI -> %s !!!
              OPENARRAY(TVarRec,(cI, int(minI), int(maxI),int(dI), int(for_ID)))).c_str() );
//
   for( int j=0; j<mBody->Count; j++ ) // �������� �����
    tf_printf( Format("; %s", OPENARRAY(TVarRec, (mBody->Strings[j].c_str()) )).c_str() );
//
   tf_printf( ";\n; ������ ���������� ������� �� 1D ������-�������" ); // ������� ����������� ";" � �����
//
   for( int j=0; j<mExpand->Count; j++ ) // ���������� ���������� �� ������-�������
    tf_printf( Format("%s",  OPENARRAY(TVarRec, (mExpand->Strings[j].c_str()) )).c_str() );
//
   tf_printf( Format("; \n; ����� ���������� �������: ��������� 'for[%s]=%d,%d,%d {'\n;", // cI -> %s !!!
              OPENARRAY(TVarRec,(cI, int(minI),int(maxI),int(dI)))).c_str() );
////////////////////////////////////////////////////////////////////////////////
   return false; // ������ str � �������� ���� fptrOut ����������� �� �����
  }
//
} // ------ ����� makeMassive_1D -----------------------------------------------


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void __fastcall replace_Str( char* s, const char* sOld, const char* sNew )
{ // ��� ��������� sOld (����� ��� � ������ ����������� ������� stdFunc) � s ���������� �� sNew
//
 string strTmp; // expr - ���������� !!!
//
// ------ ������ stdFunc[] -----------------------------------------------------
 for( int i=0; i<sizeof(stdFunc)/sizeof(stdFunc[0]); i++ ) // �� ��������� ����������� ������� �������
 {
  strReplace( expr, sizeof(expr)-1, s, stdFunc[i], Format( replaceFmt,OPENARRAY(TVarRec,(i))).c_str() );
  strcpy( s, expr );
 } // ����� for( int i=0; i<sizeof(stdFunc)/sizeof(stdFunc[0]); i++ )
//
 for( int ii=10; ii>=2; ii-- ) // �������� �� ������� ii ���������������� cI
  if( strstr( s, strTmp.assign(ii,cI).c_str() ) )
  {
   t_printf( "\n-M- � ������ [%s] ��� ���������� �������� ������������ (%d) ������ ������� '%c' -M-\n", s,ii,cI );
   longjmp( env_BypassMacro, 1 ); // ����������� ������� � ����� setjmp !!!!!!!
  }
//
 strReplace( expr, sizeof(expr)-1, s, sOld, sNew ); // ������ ������ sOld �� ������ sNew (������ cI �� X[0])
//
 strcpy( s, expr ); // ��������� � ������������� ������ ---------------------------
//
// ------ �������������� stdFunc[] ---------------------------------------------
 for( int i=0; i<sizeof(stdFunc)/sizeof(stdFunc[0]); i++ ) // �� ��������� ����������� ������� �������
 {
   strReplace( expr, sizeof(expr)-1, s, Format( replaceFmt,OPENARRAY(TVarRec,(i))).c_str(), stdFunc[i] );
   strcpy( s, expr );
 } // ����� for( int i=0; i<sizeof(stdFunc)/sizeof(stdFunc[0]); i++ )
//
} // ------ ����� replace_Str --------------------------------------------------


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
double __fastcall pcExpression( char* Expression, double Value_1, double Value_2 )
{ // ��������� � ���������� ��������� Expression �� ���� ���������� Value_1/Value_2
//
 try
 {
  parser.Compile( Expression ); // �������� ���������
  parser.Evaluate( Value_1, Value_2 ); // ����� ���������� �� ���������� X[0],X[1] etc
  return parser.GetResult(); // ���������� double !!!
 }
 catch(TError error)
 {
  if( flagAlarmParser ) // ������ �������������� (global)
  {
   MessageBeep( MB_OK ); // �������� ��������������...
   snprintf( str,sizeof(str), "������������: �������� �������� ���������\n\n[%s]" // str - global
                              "\n\n������: [%s] � ������� %d",
             Expression, error.error, error.pos );
   MessageBox(0, str, " ������ !", MB_OK);
//
   flagAlarmParser = false; // global..!
  } // � ������ �� ��������..!
//
 } // ����� catch( )
//
} // -------- ����� pcExpression -----------------------------------------------


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
int __fastcall testIndex(char* str, char chr )
{ // ���� � ������ str ����� ����� 'chr' ����������� � ���� �����, ������� ����� ������
//
 int count = 0;
//
 for( int i=0; i<strlen(str); i++ ) // �� ���� ������ str
  if( isalpha(str[i]) && str[i] !=chr ) // ���� ����� � �� 'chr'
   count++;
//
 return count;
//
} // ------ ����� testIndex ----------------------------------------------------


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void __fastcall SelectInstrForMacrosExpans_1D()
{ // ����� ���� ���������� ������� ��� ���������� ��������������
//
 for( int j=1; j<mBody->Count-1; j++ ) // ���� �� ������� ���������� ������� (����� ��������� � ���������)
 {
  strncpy( SetName, mBody->Strings[j].c_str(), 3 ); // ��������� ��������� ���������� (3 ������ �������)
  strupr( SetName ); // ��������� � ��������� ��� ������������ ���������
//
  if( !strcmp( SetName, "SET") ) // ��� SET ..!
   Expansion_SET_1D( mBody->Strings[j].c_str() ); // ��������� ������ �� ���������� SET
  else // ��� �� SET (1-2 �������� � ����������)
   Expansion_1_2_Opd_1D( mBody->Strings[j].c_str(), Get_CountOperandsByInstruction( SetName ) );
 }
//
} // --------- ����� SelectInstrForMacrosExpans_1D -----------------------------


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void __fastcall SelectInstrForMacrosExpans_2D()
{ // ����� ���� ���������� ������� ��� ���������� ��������������
//
 for( int j=1; j<mBody->Count-1; j++ ) // ���� �� ������� ���������� ������� (����� ��������� � ���������)
 {
  strncpy( SetName, mBody->Strings[j].c_str(), 3 ); // ��������� ��������� ���������� (3 ������ �������)
  strupr( SetName ); // ��������� � ��������� ��� ������������ ���������
//
  if( !strcmp( SetName, "SET") ) // ��� SET ..!
   Expansion_SET_2D( mBody->Strings[j].c_str() ); // ��������� ������ �� ���������� SET
  else // ��� �� SET (1-2 �������� � ����������)
   Expansion_1_2_Opd_2D( mBody->Strings[j].c_str(), Get_CountOperandsByInstruction( SetName ) );
 }
//
} // --------- ����� SelectInstrForMacrosExpans_2D -----------------------------


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
bool __fastcall makeMassive_2D( char* str, INT i )
{ // ������� � ������������ (���������) ������ � 2D-������������� --------------
//
  if( !strncmp( str, "for[", 4 ) && // ���� � ������ str ��������� 'for[' (������ 4 �������) '�'
       sscanf( str, "for[%c:%c]=%d,%d,%d:%d,%d,%d{", &cI,&cJ,&minI,&maxI,&dI, // ��� 8 ����� ��������� ���������..!
                                                             &minJ,&maxJ,&dJ ) == 8 )
  { // ������ if( !strncmp( str, "for[", 4 ) && ...
//
   if( !dI || // ������� �������� ��������� ���������� ����� I (�������� � ����������� ������� )
     (maxI>minI && dI<=0) || (maxI<minI && dI>=0) )
   {
    t_printf( "\n-M- ����������� �������� ��������� [%d,%d,%d] ���������� ����� '%c' -M-\n", minI,maxI,dI,cI );
    return false; // ������ str � �������� ���� fptrOut ����������� �� �����
   }
//
   if( !dJ || // ������� �������� ��������� ���������� ����� J (�������� � ����������� ������� )
     (  maxJ>=minJ && dJ<=0) || ( maxJ<=minJ && dJ>=0) )
   {
    t_printf( "\n-M- ����������� �������� ��������� [%d,%d,%d] ���������� ����� '%c' -M-\n", minJ,maxJ,dJ,cJ );
    return false;  // ������ str � �������� ���� fptrOut ����������� �� �����
   }
//
   if( cI == cJ ) // ���������� ������� ����� ������
   {
    t_printf( "\n-M- ���������� ������� ��������� ������ - �������: '%c' , ����������: '%c' -M-\n", cI, cJ );
    return false;  // ������ str � �������� ���� fptrOut ����������� �� �����
   }
//
   for_ID ++ ; // ����� for[... � ������ ���������
   flagMacroTitle_2D = true; // ����� ������ � ���������� 1D-�������
//
   mBody->Clear(); // ������ ��������� �������
   mBody->Add( str ); // ��������� ��������� 2D-�������
//
   mExpand->Clear(); // ������ ���������� �������
   return false;  // ������ str � �������� ���� fptrOut ����������� �� �����
  } // ����� if( !strncmp( str, "for[", 4 ) && ...
//
  if( flagMacroTitle_2D && str[0] != '}' ) // ���� �� ���� "}" � ������ ����� flagMacroTitle=true
  {
   mBody->Add( str ); // ��������� � mBody ������ �������
   return false; // ������ str � �������� ���� fptrOut ����������� �� �����
  }
//
  if( flagMacroTitle_2D && str[0] == '}' ) // ����� ��������� ������� � ������ ���������� "}"
  {
   flagMacroTitle_2D = false; // � ������ ���������� "}"
   mBody->Add( str ); // ��������� � mBody ������ "}"
//
////////////////////////////////////////////////////////////////////////////////
   SelectInstrForMacrosExpans_2D(); // ����� ���� ���������� ��� ���������� ��������������
////////////////////////////////////////////////////////////////////////////////
   tf_printf( Format("; \n; ������ ���������� �������: ��������� 'for[%s:%s]=%d,%d,%d:%d,%d,%d {'\n; for_ID: %d", // cI -> %s !!!
              OPENARRAY(TVarRec,(cI,cJ, int(minI),int(maxI),int(dI), int(minJ),int(maxJ),int(dJ), int(for_ID)))).c_str() );
//
   for( int j=0; j<mBody->Count; j++ ) // �������� �����
    tf_printf( Format("; %s", OPENARRAY(TVarRec, (mBody->Strings[j].c_str()) )).c_str() );
//
   tf_printf( ";\n; ������ ���������� ������� �� 2D ������-�������" ); // ������� ����������� ";" � �����
//
   for( int j=0; j<mExpand->Count; j++ ) // ���������� ���������� �� ������-�������
    tf_printf( Format("%s",  OPENARRAY(TVarRec, (mExpand->Strings[j].c_str()) )).c_str() );
//
   tf_printf( Format("; \n; ����� ���������� �������: ��������� 'for[%s:%s]=%d,%d,%d:%d,%d,%d {'\n;", // cI -> %s !!!
              OPENARRAY(TVarRec,(cI,cJ, int(minI),int(maxI),int(dI),  int(minJ),int(maxJ),int(dJ) ))).c_str() );
////////////////////////////////////////////////////////////////////////////////
    return false;  // ������ str � �������� ���� fptrOut ����������� �� �����
  }
//
} // ------ ����� makeMassive_2D -----------------------------------------------

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void __fastcall Expansion_SET_1D( char* s )
{ // ��������� ������ �� ���������� SET , ��������� ���������� � mExpand (TStringList)
//
 int out;
 string strTmp;
 char *p, tmp_1[_512], tmp_2[_512]; // ��������� �������
//
// ----- ��������� 1-� ������� SET'� -------------------------------------------
//
  Comm[0] = '\0'; // ���������...
//
  strcpy( str, s ); // ��������, ��� ��� ��������� strtok �������� ������ s �����������
  strcpy( SetName,      p = strtok( str,  " " ) ); // ���������
  strcpy( Opd_1,   DAS( p = strtok( NULL, "," ) ) ); // 1-� �������
  strcpy( Res,     DAS( p = strtok( NULL, ";" ) ) ); // ���������
//
  strcpy( tmp_1,  Opd_1 ); // ��������� Opd_1
  strcpy( tmp_2,  Res ); // ��������� Res
//
  if( strchr(s,';') ) // ���� � ������ ���� ';'
   strcpy( Comm, &s[strchr(s,';')-s+1] ); // �������� � Comm �� ������ ';'
//
 for( INT i=minI; i<=maxI; i+=dI ) // �� ����� ��������� ��������� ���������� �������
 {
  strcpy( Opd_1, tmp_1 ); // ������������ Opd_1
  strcpy( Res,   tmp_2 ); // ������������ Res
//
  replace_Str( Opd_1, strTmp.assign(1,cI).c_str(), "X[0]" ); // ������ cI �� X[0] �������� ����� ����������� �������
//
  sprintf( Opd_1, "%.*g", sizeof(Mem_Data[0].Addr)-1, pcExpression( expr, (double)i, 0.0 ) );
// ------ ������������ ���� Res (��������� ���������� ����������) --------------
  if( test_isMassive_1D( Res ) )
   handlXXX_asMassive_1D( Res, i ); // ������������ ���������, ���� �� 1D-������������
  else
   handlXXX_asVariable_1D( Res, i ); // ������������ ���������, ���� �� ������� ����������
//
// ---- �������� ������� ������ ���������� �� ��������� ����� ------------------
//
  sprintf( str, "%s %s, %s ; %s", SetName, Opd_1, Res, Comm ); // ������� ���������� ��� ����������
//
  mExpand->Add( str ); // ������� ������� ���������� ������ ��� ���������� ��������
//
 } // ����� �� i (�� ����� ����� ����������) ===================================
//
} // ---------- ����� Expansion_SET_1D -----------------------------------------


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void __fastcall Expansion_SET_2D( char* s )
{ // ��������� ������ �� ���������� SET , ��������� ���������� � mExpand (TStringList)
//
 int out;
 string strTmp;
 char *p, tmp_1[_512], tmp_2[_512]; // ��������� �������
//
// ----- ��������� 1-� ������� SET'� -------------------------------------------
//
  Comm[0] = '\0'; // ���������...
//
  strcpy( str, s ); // ��������, ��� ��� ��������� strtok �������� ������ s �����������
  strcpy( SetName,      p = strtok( str,  " " ) ); // ���������
  strcpy( Opd_1,   DAS( p = strtok( NULL, "," ) ) ); // 1-� �������
  strcpy( Res,     DAS( p = strtok( NULL, ";" ) ) ); // ���������
//
  strcpy( tmp_1, Opd_1 ); // ��������� Opd_1
  strcpy( tmp_2, Res ); // ��������� Res
//
  if( strchr(s,';') ) // ���� � ������ ���� ';'
   strcpy( Comm, &s[strchr(s,';')-s+1] ); // �������� � Comm �� ������ ';'
//
 for( INT i=minI; i<=maxI; i+=dI ) // �� ����� ��������� ��������� i ���������� �������
 for( INT j=minJ; j<=maxJ; j+=dJ ) // �� ����� ��������� ��������� j ���������� �������
 {
  strcpy( Opd_1, tmp_1 ); // ������������ Opd_1
  strcpy( Res,   tmp_2 ); // ������������ Res
//
  replace_Str( Opd_1, strTmp.assign(1,cI).c_str(), "X[0]" ); // ������ cI �� X[0] �������� ����� ����������� �������
  replace_Str( Opd_1, strTmp.assign(1,cJ).c_str(), "X[1]" ); // ������ cJ �� X[1] ...
//
  sprintf( Opd_1, "%.*g", sizeof(Mem_Data[0].Addr)-1, pcExpression( expr, (double)i, double(j) ) ); // �pd_1, w - ������..1
// ------ ������������ ���� Res (��������� ���������� ����������) --------------
  if( test_isMassive_2D( Res ) )
   handlXXX_asMassive_2D( Res, i, j ); // ������������ ���������, ���� �� 2D-������������
  else
   handlXXX_asVariable_2D( Res, i, j ); // ������������ ���������, ���� �� ������� ����������
//
// ---- �������� ������� ������ ���������� �� ��������� ����� ------------------
  sprintf( str, "%s %s, %s ; %s", SetName, Opd_1, Res, Comm ); // ������� ��������������� ������ ����������
////////////////////////////////////////////////////////////////////////////////
  mExpand->Add( str ); // ������� ������� ������ ���������� ��� ����������
//
 } // ����� �� i,j (�� ����� ����� � �������� ����������) ======================
//
} // ---------- ����� Expansion_SET_2D -----------------------------------------


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void __fastcall Expansion_1_2_Opd_1D( char* s, int nOpd )
{ // ��������� ������ �� ���������� c 1-2 ���������� , ��������� ���������� �
// mExpand (TStringList) ; nOpd - ����� ���������
// bool outCode = true; // ������� ���������� SET
 boolean isPred; // ���� �� � �� P-��������� ��������
 char *p, *n, *ps, *pe, tmp[_512],
      tmp_1[_512], tmp_2[_512], tmp_3[_512], tmp_4[_512]; // ��������� ������� ��� ����������� �������� ����������
//
  strcpy( str, s ); // ��������, ��� ��� ��������� strtok �������� ������ s �����������
//
  Opd_2[0] = Comm[0] = '\0'; // ���������...
//
  strcpy( SetName,    p = strtok( str,  " " ) ); // ���������
  strcpy( Opd_1, DAS( p = strtok( NULL, "," ) ) ); // 1-� �������
//
  if( nOpd == 2 )
   strcpy( Opd_2, DAS( p = strtok( NULL, "," ) ) ) ; // 2-� �������
//
// ----- ���� ������� ��� ������������� ���� ��������� � P- � ��-P-����������---
  if( is_Predicat( SetName ) ) // �������� SetName = P-����������)
   strcpy( Res, DAS( p = strtok( NULL, ";" ) ) ); // ��������� (������������ � ������� ��� ��������)
  else // �� P-����������
  {
   strcpy( Res,  DAS( p = strtok( NULL, ",;" ) ) ); // ��������� ����������
   strcpy( str, s ); // ������������, ��� ��� ��������� strtok �������� ������ s �����������
   n = strstr( str, Res ); // ������ p � str
   n += strlen(Res); // ������ ������ ����� Res � str
//
   strcpy( tmp, str+(n-str) ); // tmp<-str (str ������� � n � �� ����� str) ; tmp - ��� ���������
//
   if( ps=strchr( tmp, ',') ) // ������ ',' � temp ������ - ������, �������� �������..!
   {
    isPred = true;
    pe=strchr( tmp, ';'); // ������ ';'
    strncpy( Pred, tmp+1, pe-tmp-1 ); // Pred<-tmp (tmp ������� � 1 � �� pe-tmp-1)
    DAS( Pred );
   } // ����� if( ps=strchr( tmp, ',') )
//
   else
   if( strchr( tmp, ';') ) // ������ ';' � temp ������ - ������, ���� ��������� ���..!
   {
    isPred = false;
    Pred[0] = '\0';
   }
//
  } // ����� else if( is_Predicat( SetName ) )
//
  if( strchr(s,';') ) // ���� � ������ ���� ';'
   strcpy( Comm, &s[strchr(s,';')-s+1] ); // �������� � Comm ��, ������ ';'
//
  strcpy( tmp_1, Opd_1 ); // ��������� ���������� �������� ����������
  strcpy( tmp_2, Opd_2 );
  strcpy( tmp_3, Res );
  strcpy( tmp_4, Pred );
//
 for( INT i=minI; i<=maxI; i+=dI ) // �� ��������� i-��������� ���������� 1D-�������
 {
// ----- ��������� �������� �� SET'� -------------------------------------------
  strcpy( Opd_1, tmp_1 ); // �������������� ���������� �������� ����������
  strcpy( Opd_2, tmp_2 );
  strcpy( Res,   tmp_3 );
  strcpy( Pred,  tmp_4 );
//
// ------ ������������ ���� Opd_1 (������ ������� ����������) ------------------
  if( test_isMassive_1D( Opd_1 ) )
   handlXXX_asMassive_1D( Opd_1, i ); // ������������ ���������, ���� �� 1D-������������
  else
   handlXXX_asVariable_1D( Opd_1, i ); // ������������ ���������, ���� �� ������� ����������
//
// ------ ������������ ���� Opd_2 (������ ������� ����������) ------------------
  if( nOpd == 2 ) // 2-� �������
   if( test_isMassive_1D( Opd_2 ) )
    handlXXX_asMassive_1D( Opd_2, i ); // ������������ ���������, ���� �� 1D-������������
   else
    handlXXX_asVariable_1D( Opd_2, i ); // ������������ ���������, ���� �� ������� ����������
//
// ------ ������������ ���� Pred (��������) ------------------------------------
  if( strlen( Pred ) ) // ���� �������� ������������...
   if( test_isMassive_1D( Pred ) )
    handlXXX_asMassive_1D( Pred, i ); // ������������ ���������, ���� �� 1D-������������
   else
    handlXXX_asVariable_1D( Pred, i ); // ������������ ���������, ���� �� ������� ����������
//
// ------ ������������ ���� Res (��������� ���������� ����������) --------------
  if( test_isMassive_1D( Res ) )
   handlXXX_asMassive_1D( Res, i ); // ������������ ���������, ���� �� 1D-������������
  else
   handlXXX_asVariable_1D( Res, i ); // ������������ ���������, ���� �� ������� ����������
//
// ---- �������� ������� ������ ���������� �� ��������� ����� ------------------
  if( nOpd == 1 ) // 1 ������� � ����������
   isPred ? sprintf( str, "%s %s, %s, %s ; %s", SetName, Opd_1, Res, Pred, Comm )
          : sprintf( str, "%s %s, %s ; %s",     SetName, Opd_1, Res,       Comm ) ;
  else // 2 �������� � ����������
   isPred ? sprintf( str, "%s %s, %s, %s, %s ; %s", SetName, Opd_1, Opd_2, Res, Pred, Comm )
          : sprintf( str, "%s %s, %s, %s ; %s",     SetName, Opd_1, Opd_2, Res,       Comm ) ;
////////////////////////////////////////////////////////////////////////////////
//
  mExpand->Add( str ); // ������� ������� ������ ���������� ��� ���������� � nExpand
//
 } // ����� ����� �� i
//
} // ---------- ����� Expansion_1_2_Opd_1D -------------------------------------


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void __fastcall Expansion_1_2_Opd_2D( char* s, int nOpd )
{ // ��������� ������ �� ���������� c 1-2 ���������� , ��������� ���������� �
// mExpand (TStringList) ; nOpd - ����� ���������
// bool outCode = true; // ������� ���������� SET
 boolean isPred; // ���� �� � �� P-��������� ��������
 char *p, *n, *ps, *pe, tmp[_512],
      tmp_1[_512], tmp_2[_512], tmp_3[_512], tmp_4[_512]; // ��������� ������� ��� ����������� �������� ����������
//
  strcpy( str, s ); // ��������, ��� ��� ��������� strtok �������� ������ s �����������
//
  Opd_2[0] = Comm[0] = '\0'; // ���������...
//
  strcpy( SetName,      p = strtok( str,  " " ) ); // ���������
  strcpy( Opd_1,   DAS( p = strtok( NULL, "," ) ) ); // 1-� �������
//
  if( nOpd == 2 )
   strcpy( Opd_2,  DAS( p = strtok( NULL, "," ) ) ) ; // 2-� �������
//
// ----- ���� ������� ��� ������������� ���� ��������� � P- � ��-P-����������---
  if( is_Predicat( SetName ) ) // �������� SetName = P-����������)
   strcpy( Res, DAS( p = strtok( NULL, ";" ) ) ); // ��������� (������������ � ������� ��� ��������)
  else // �� P-����������
  {
   strcpy( Res,  DAS( p = strtok( NULL, ",;" ) ) ); // ��������� ����������
   strcpy( str, s ); // ������������, ��� ��� ��������� strtok �������� ������ s �����������
   n = strstr( str, Res ); // ������ p � str
   n += strlen(Res); // ������ ������ ����� Res � str
//
   strcpy( tmp, str+(n-str) ); // tmp<-str (str ������� � n � �� ����� str) ; tmp - ��� ���������
//
   if( ps=strchr( tmp, ',') ) // ������ ',' � temp ������ - ������, �������� �������..!
   {
    isPred = true;
    pe=strchr( tmp, ';'); // ������ ';'
    strncpy( Pred, tmp+1, pe-tmp-1 ); // Pred<-tmp (tmp ������� � 1 � �� pe-tmp-1)
    DAS( Pred );
   } // ����� if( ps=strchr( tmp, ',') )
//
   else
   if( strchr( tmp, ';') ) // ������ ';' � temp ������ - ������, ���� ��������� ���..!
   {
    isPred = false;
    Pred[0] = '\0';
   }
//
  } // ����� else if( is_Predicat( SetName ) )
//
  if( strchr(s,';') ) // ���� � ������ ���� ';'
   strcpy( Comm, &s[strchr(s,';')-s+1] ); // �������� � Comm ��, ������ ';'
//
  strcpy( tmp_1, Opd_1 ); // ��������� ���������� �������� ����������
  strcpy( tmp_2, Opd_2 );
  strcpy( tmp_3, Res );
  strcpy( tmp_4, Pred );
//
 for( INT i=minI; i<=maxI; i+=dI ) // �� ��������� i-��������� ���������� 2D-�������
 for( INT j=minJ; j<=maxJ; j+=dJ ) // �� ��������� j-��������� ���������� 2D-�������
 {
  strcpy( Opd_1, tmp_1 );  //��������������� ���������� �������� ����������
  strcpy( Opd_2, tmp_2 );
  strcpy( Res,   tmp_3 );
  strcpy( Pred,  tmp_4 );
//
// ------ ������������ ���� Opd_1 (������ ������� ����������) ------------------
  if( test_isMassive_2D( Opd_1 ) )
   handlXXX_asMassive_2D( Opd_1, i, j ); // ������������ ���������, ���� �� 1D-������������
  else
   handlXXX_asVariable_2D( Opd_1, i, j ); // ������������ ���������, ���� �� ������� ����������
//
// ------ ������������ ���� Opd_2 (������ ������� ����������) ------------------
  if( nOpd == 2 ) // 2-� �������
   if( test_isMassive_2D( Opd_2 ) )
    handlXXX_asMassive_2D( Opd_2, i, j ); // ������������ ���������, ���� �� 2D-������������
   else
    handlXXX_asVariable_2D( Opd_2, i, j ); // ������������ ���������, ���� �� ������� ����������
//
// ------ ������������ ���� Pred (��������) ------------------------------------
  if( strlen( Pred ) ) // ���� �������� ������������
   if( test_isMassive_1D( Pred ) )
    handlXXX_asMassive_1D( Pred, i ); // ������������ ���������, ���� �� 1D-������������
   else
    handlXXX_asVariable_1D( Pred, i ); // ������������ ���������, ���� �� ������� ����������
//
////////////////////////////////////////////////////////////////////////////////
// ------ ������������ ���� Res (��������� ���������� ����������) --------------
  if( test_isMassive_2D( Res ) )
   handlXXX_asMassive_2D( Res, i, j ); // ������������ ���������, ���� �� 2D-������������
  else
   handlXXX_asVariable_2D( Res, i, j ); // ������������ ���������, ���� �� ������� ����������
//
// ---- �������� ������� ������ ���������� �� ��������� ����� ------------------
// ---- �������� ������� ������ ���������� �� ��������� ����� ------------------
  if( nOpd == 1 ) // 1 ������� � ����������
   isPred ? sprintf( str, "%s %s, %s, %s ; %s", SetName, Opd_1, Res, Pred, Comm )
          : sprintf( str, "%s %s, %s ; %s",     SetName, Opd_1, Res,       Comm ) ;
  else // 2 �������� � ����������
   isPred ? sprintf( str, "%s %s, %s, %s, %s ; %s", SetName, Opd_1, Opd_2, Res, Pred, Comm )
          : sprintf( str, "%s %s, %s, %s ; %s",     SetName, Opd_1, Opd_2, Res,       Comm ) ;
////////////////////////////////////////////////////////////////////////////////
  mExpand->Add( str ); // ������� ������� ������ ���������� ��� ����������
//
 } // ����� ����� �� i,j (�� ����� ����� � �������� ����������) ================
//
} // ---------- ����� Expansion_1_2_Opd_2D -------------------------------------


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void __fastcall handlXXX_asMassive_2D( char* Field, INT i, INT j )
{ // ������������ ���� XXX=Field ���� Opd_1,Opd_2,Res ��� 2D-�������������� ----
//
  string strTmp;
  char *p, tmp_1[_512],tmp_2[_512], expr_1[_512],expr_2[_512]; // ��������� �������
//
  strcpy( nameMass,    p = strtok( Field,  "[" ) ); // ������ �� '['
  strcpy( indexMass_1, p = strtok( NULL,    ":" ) ); // ������ �� '[' �� ':'
  strcpy( indexMass_2, p = strtok( NULL,   "]" ) ); // ������ �� ':' �� ']'
//
  if( !permissName( nameMass ) )
  { // �������� �� ������������ ����� ������� (����� > 0 �������� � ��������� ������ �����)
   t_printf( err_01, Field );
   longjmp( env_BypassMacro, 1 ); // ����������� ������� � ����� setjmp !!!!!!!
  }
//
  strReplace( tmp_1,  sizeof(tmp_1)-1,  indexMass_1, strTmp.assign(1,cI).c_str(), "X[0]" ); // ������ � indexMass_1 ������ cI �� X[0]
  strReplace( expr_1, sizeof(expr_1)-1, tmp_1,       strTmp.assign(1,cJ).c_str(), "X[1]" ); // ������ ������ cJ �� X[1]
//
  strReplace( tmp_2,  sizeof(tmp_2)-1,  indexMass_2, strTmp.assign(1,cI).c_str(), "X[0]" ); // ������ � indexMass_2 ������ cI �� X[0]
  strReplace( expr_2, sizeof(expr_2)-1, tmp_2,      strTmp.assign(1,cJ).c_str(), "X[1]" ); // ������ ������ cJ �� X[1]
//
  sprintf( Field, "%s[%d:%d]", nameMass, (INT)pcExpression( expr_1, (double)i, (double)j ),
                                         (INT)pcExpression( expr_2, (double)i, (double)j ) );
//
} // ----- ����� handlXXX_asMassive_2D -----------------------------------------


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void __fastcall handlXXX_asMassive_1D( char* Field, INT i )
{ // ������������ ���� XXX=Field ���� Opd_1,Opd_2,Pred,Res ��� 1D-��������������
//
  string strTmp;
  char *p;
//
  strcpy( nameMass,  p = strtok( Field, "[" ) ); // ������ �� '['
  strcpy( indexMass, p = strtok( NULL,  "]" ) ); // ������ �� '[' �� ']'
//
  if( !permissName(nameMass) )
  { // �������� �� ������������ ����� ������� (����� > 0 �������� � ��������� ������ �����)
   t_printf( err_01, Field );
   longjmp( env_BypassMacro, 1 ); // ����������� ������� � ����� setjmp !!!!!!!
  }
//
  strReplace( expr, sizeof(expr)-1, indexMass, strTmp.assign(1,cI).c_str(), "X[0]" ); // ������ ������ cI �� X[0]
//
  sprintf( Field, "%s[%d]", nameMass, (INT)pcExpression( expr, (double)i, 0.0 ) );
//
} // ----- ����� handlXXX_asMassive_1D -----------------------------------------


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void __fastcall handlXXX_asVariable_1D( char* Field, INT i )
{  // ������������ ���� XXX=Field ���� Opd_1,Opd_2,Pred,Res ��� 1D-��������������
// ������������ ���� ��������, ���� �� � ����� ������� ����������
//
 if( TokenUse && (&Field==&Res) ) // ������������ ����� ��� ����� ���� ����������
  strcpy( Field, Format("%s:%d:%d",OPENARRAY(TVarRec,(Field,int(for_ID),int(i)) ) ).c_str() );
//
} // ---------- ����� handlXXX_asVariable_1D -----------------------------------


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void __fastcall handlXXX_asVariable_2D( char* Field, INT i, INT j )
{ // ������������ ���� XXX=Field ���� Opd_1,Opd_2,Pred,Res ��� 2D-��������������
//
 if( TokenUse && (&Field==&Res) ) // ������������ ����� ��� ����� ���� ����������
  strcpy( Field, Format("%s:%d:%d:%d",OPENARRAY(TVarRec,(Field,int(for_ID),int(i),int(j)) ) ).c_str() );
//
} // ---------- ����� handlOpd_asVariable_2D -----------------------------------




