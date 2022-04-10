//
#define FTP_SERVER "vbakanov.ru" // FTP-������
#define FTP_SERVER_PART ""FTP_SERVER"/dataflow/content" // ��������� ���� �� ������� (FTP_SERVER - ��������� ���������������!)
#define FILENAMESERVER  "dataflow/content/install_df.exe" // ��������� ���� � ����������� �������
//

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void __fastcall TF1::FTP_Get_Connected(TObject *Sender)
{ // ���������� ��� ���������� � ��������
 SBM0->Text = " ���������� � �������� �����������";
 Delay( 500 );
} //----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void __fastcall TF1::FTP_Get_Disconnected(TObject *Sender)
{ // ���������� ��� ������� ���������� � ��������
 SBM0->Text = " ���������� � �������� ���������";
 Delay( 500 );
} //----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void __fastcall TF1::FTP_Get_OnStatus(TObject *axSender, const TIdStatus axStatus, const AnsiString asStatusText)
{ // ��������� �������� ��������
  switch( axStatus )
  {
   case hsResolving:     //SBM0->Text = " A host name is being resolved for an IP address...";
                         SBM0->Text = " �������� ��� ������� ��������� � IP-�����...";
        break;
   case hsConnecting:    //SBM0->Text = " A connection is being opened...";
                         SBM0->Text = " ���������� ���������������...";
        break;
   case hsConnected:    //SBM0->Text = " A connection has been made...";
                        SBM0->Text = " ���������� ������� �����������...";
        break;
   case hsDisconnecting: //SBM0->Text = " The connection is being closed...";
                         SBM0->Text = " ���������� �����������...";
        break;
   case hsDisconnected:  //SBM0->Text = " The connection has been closed...";
                         SBM0->Text = " ���������� ���������...";
        break;
//   case hsText:          //SBM0->Text = asStatusText;
//                         SBM0->Text = asStatusText;
//        break;
//   default:              //SBM0->Text = asStatusText;
//                         SBM0->Text = asStatusText;
  }
//
 Delay( 500 );
//
} //----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void __fastcall TF1::FTP_Get_OnWork(TObject *Sender, TWorkMode AWorkMode, const int AWorkCount)
{ // ���������� ��� �������� ������ � ������� �� ������
 if( FileSizeFromServer )
  SBM0->Text = Format(" %.0f%% (�� %d ����) ������ �������� � �������",
                      OPENARRAY(TVarRec, (100.0*AWorkCount/FileSizeFromServer, int(FileSizeFromServer)) ) );
 Delay( 1 );
} //----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void __fastcall TF1::FTP_Get_OnWorkBegin(TObject *Sender, TWorkMode AWorkMode, const int AWorkCountMax)
{ // ���������� � ������ �������� ������ � ������� �� ������
// FileSizeFromServer = AWorkCountMax; // !!!!! ��� Get ������ AWorkCountMax == 0 !!!!!
 SBM0->Text = Format(" %d ���� ��������� ��� ��������� � �������", OPENARRAY(TVarRec, (int(FileSizeFromServer)) ) );
 Delay( 500 );
} //----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void __fastcall TF1::FTP_Get_OnWorkEnd(TObject *Sender, TWorkMode AWorkMode)
{ // ���������� � ����� �������� ������ � ������� �� ������
 Delay( 500 );
 SBM0->Text = Format(" %d ���� �������� � �������", OPENARRAY(TVarRec, (int(FileSizeFromServer)) ) );
} //----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void __fastcall TF1::EndedUploadFile(TObject *Sender)
{ // ������������� ��������� ���������� � �������� - ���������� �������� �����
 F1->FTP_Get->Disconnect(); // ��������� ���������� � ��������
} //----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void __fastcall TF1::OnClickGetInstall(TObject *Sender)
{ // �������� ���� � �������
 Unload_Install(); // �������� ��������������� ���� ������� � �������
} // ---------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
char* __fastcall UserPassword( int Rule )
{ // ��� Rule =1/#1 ��������� ������ User/Password ��������������
 INT k, n=1, Sum=0, Max=1000; // Max - ������� ������ ������ ������� �����
 static char User[_128]="\0", // ������ Usr
             Password[_128]="\0"; // ������ Password
//
 if( strlen(User) && strlen(Password) ) // ���� �������� ������...
 {
  if( Rule == 1 ) return User ;
  else            return Password ;
 }
//
 for (INT i=2; i<Max; i++) // ���������� �����
 {
  bool y = 1; // ����� ����� �������
  if (i%2 == 0) y = 0; // ������� ������ �� 2, ������ �� �������
  for (INT j=3; (j*j <= i) && y; j+=2)
   if (i%j == 0) {y = 0; break;} // ������� ������ �� ����� ������ ����� �� �����, ������ �� �������
   if(y)
   {
    n ++ ;
    Sum += i ;
//    t_printf( "n=%10d   i=%20d   [i+Sum] = %20d  [(i+Sum)%%i] = %-20d [(Sum-i)%%i] = %-20d", n, i, i+Sum, (Sum+i) % i, (Sum-i) % i );
//    Application->ProcessMessages();
//
    k=(i+Sum) % i ; // ������������ ����� (i+Sum)%i
    // "valer-bak" // [i](������)=i(����� �������� �����)-N(���������)
/*    if( k== 88 && n== 25 ) User[i- 97] = 'v'  ;
    if( k== 85 && n== 28 ) User[i-106] = 'a'  ;
    if( k==137 && n== 38 ) User[i-161] = 'l'  ;
    if( k==144 && n== 40 ) User[i-170] = 'e'  ;
    if( k==162 && n== 44 ) User[i-189] = 'r'  ;
    if( k==639 && n==120 ) User[i-654] = '-'  ;
    if( k==544 && n==122 ) User[i-667] = 'b'  ;
    if( k==685 && n==130 ) User[i-726] = 'a'  ;
    if( k==236 && n==152 ) User[i-873] = 'k'  ;
    if( k==803 && n==167 ) User[i-982] = '\0' ; */
    if( k==685 && n==130 ) User[i-726] = 'a'  ;
    if( k== 85 && n== 28 ) User[i-106] = 'a'  ;
    if( k==144 && n== 40 ) User[i-170] = 'e'  ;
    if( k==803 && n==167 ) User[i-982] = '\0' ;
    if( k==162 && n== 44 ) User[i-189] = 'r'  ;
    if( k==544 && n==122 ) User[i-667] = 'b'  ;
    if( k== 88 && n== 25 ) User[i- 97] = 'v'  ;
    if( k==137 && n== 38 ) User[i-161] = 'l'  ;
    if( k==236 && n==152 ) User[i-873] = 'k'  ;
    if( k==639 && n==120 ) User[i-654] = '-'  ;
//
    k=(Sum-i) % i ; // ������������ ����� (i+Sum)%i
    // "5k9lpsk" // [i](������)=i(����� �������� �����)-N(���������)
/*    if( k== 18 && n==  8 ) Password[i- 19] = '5'  ;
    if( k== 31 && n== 13 ) Password[i- 40] = 'k'  ;
    if( k== 78 && n== 22 ) Password[i- 77] = '9'  ;
    if( k== 61 && n== 29 ) Password[i-106] = 'l'  ;
    if( k==199 && n== 48 ) Password[i-219] = 'p'  ;
    if( k==126 && n== 69 ) Password[i-342] = 's'  ;
    if( k==265 && n==113 ) Password[i-611] = 'k'  ;
    if( k==856 && n==977 ) Password[i-158] = '\0' ; */
    if( k==265 && n==113 ) Password[i-611] = 'k'  ;
    if( k==856 && n==977 ) Password[i-158] = '\0' ;
    if( k== 18 && n==  8 ) Password[i- 19] = '5'  ;
    if( k== 31 && n== 13 ) Password[i- 40] = 'k'  ;
    if( k== 78 && n== 22 ) Password[i- 77] = '9'  ;
    if( k== 61 && n== 29 ) Password[i-106] = 'l'  ;
    if( k==199 && n== 48 ) Password[i-219] = 'p'  ;
    if( k==126 && n== 69 ) Password[i-342] = 's'  ;
//
   }
 }
//
 if( Rule == 1 ) return User ;
 else            return Password ;
//
} // --- ������������ ����� User/Password �� ������ ���������� ������� ����� ---

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void __fastcall GetFileFromServer( char *FileNameSource, char *FileNameDestination, bool Replace  )
{ // �������� ���� � HTTP-�������
//
// F1->FTP_Get->OnWorkBegin = F1->FTP_Get_OnWorkBegin ; // ��� Get ������ AWorkCountMax == 0 !!!!!
//
 F1->FTP_Get->Host     = FTP_SERVER ; // "192.162.103.128" ;
 F1->FTP_Get->User     = UserPassword( 1 ) ;
 F1->FTP_Get->Password = UserPassword( 2 ) ;
//
 F1->FTP_Get->TransferType = ftBinary ; // ��� ������������ ��������� �������� ������
//
 if( F1->FTP_Get->Connected() ) // ����������-�..!
  F1->FTP_Get->Disconnect();
//
 try
 {
  F1->FTP_Get->Connect() ; // �������� ����������� � ��������...
  FileSizeFromServer = F1->FTP_Get->Size( FileNameSource ) ; // �������� ������ ������������ ����� ( ���������� �� OnWorkBegin )
 }
  catch(const EIdException& E) // ����� ���������� Indy-����������
 {
  SBM0->Text = Format(" ������ ����������: %s", OPENARRAY(TVarRec, (E.Message) ) );
  MessageBeep( MB_ICONASTERISK ); // ��������������...
  ShowMessage( Format(" ������ ����������: %s\n%s", OPENARRAY(TVarRec, (E.Message, "������, � ��� �������� � �����..!") ) ) );
  F1->FTP_Get->Disconnect(); // ������������� �� �������
  return;
 }
//
 try
 {
  F1->FTP_Get->Get( FileNameSource,
                    FileNameDestination,
                    Replace ) ; // ���������� �������� �����
 }
  catch(const EIdException& E) // ����� ���������� Indy-����������
 {
  SBM0->Text = Format(" ������ �������� ����� c ������f: %s", OPENARRAY(TVarRec, (E.Message) ) );
  MessageBeep( MB_ICONASTERISK ); // ��������������...
  ShowMessage(" ������ �������� ����� �� ������: " + E.Message);
 }
//
 F1->FTP_Get->Disconnect(); // ������������� �� �������
} // ----- ����� GetFileFromServer() -------------------------------------------

   ////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void __fastcall PostFileToServer( char *FileNameSource, char *FileNameDestination, bool Replace )
{ //
//
 F1->FTP_Post->Host     = FTP_SERVER ; // "192.162.103.128" ;
 F1->FTP_Post->User     = UserPassword( 1 ) ;
 F1->FTP_Post->Password = UserPassword( 2 ) ;
//
 F1->FTP_Post->TransferType = ftBinary ; // ��� ������������ ��������� �������� ������
//
 if( F1->FTP_Post->Connected() ) // ����������-�..!
  F1->FTP_Post->Disconnect();
//
 try
 {
  F1->FTP_Post->Connect() ; // �������� ����������� � ��������...
 }
  catch(const EIdException& E) // ����� ���������� Indy-����������
 {
//  SBM0->Text = Format(" ������ ����������: %s", OPENARRAY(TVarRec, (E.Message) ) );
//  MessageBeep( MB_ICONASTERISK ); // ��������������...
//  ShowMessage(" ������ ����������: " + E.Message);
  F1->FTP_Post->Disconnect(); // ������������� �� �������
  return;
 }
//
 try
 {
  F1->FTP_Post->Put( FileNameSource, FileNameDestination, Replace ) ; // ���������� �������� ����� �� ������
 }
  catch(const EIdException& E) // ����� ���������� Indy-����������
 {
//  SBM0->Text = Format(" ������ �������� ����� �� ������: %s", OPENARRAY(TVarRec, (E.Message) ) );
//  MessageBeep( MB_ICONASTERISK ); // ��������������...
//  ShowMessage(" ������ �������� ����� �� ������: " + E.Message);
 }
//
 F1->FTP_Post->Disconnect(); // ������������� �� �������
} // ----- ����� PostFileToServer() --------------------------------------------

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void __fastcall Upload_Data() // ��������� ������ �� ������
{
unsigned long szSize = _1024;
char *szCompName = new char[szSize], // ��� ����������
     *szUserName = new char[szSize], // ��� ������������
     *szLocalIP  = new char[szSize]; // ��������� IP
//     *szDateTime = new char[szSize]; // ������� ����/�����
//
 GetComputerName(szCompName, &szSize); // ��� ����������
 GetUserName(szUserName, &szSize); // ��� ������������
 strcpy( szLocalIP,  F1->IPWatch->LocalIP().c_str() ) ; // ������� IP
// strcpy( szDateTime, PutDateTimeToString(1) ) ; // ������� ����/����� (���������� uniqueStr - ������������� � Start_DF() )
//
 PostFileToServer( FileNameSet, // �������� SET-����� � �������...
                   Format("%s/[%s %s][%s][%s][%s][%s][%s].upload", // ... �� ������
                          OPENARRAY( TVarRec,
                          (FTP_SERVER_PART,
                          ExtractFileName(ParamStr(0)).c_str(),"Data",
                          uniqueStr,FileNameSet,
                          szCompName,szUserName,szLocalIP) ) ).c_str(),
                  true ); // � ������� ����� ��� ����������� � ����� �� ������
///
 PostFileToServer( tmpPRO, // �������� PRO-����� � �������...
                   Format("%s/[%s %s][%s][%s][%s][%s][%s].upload", // ... �� ������
                          OPENARRAY( TVarRec,
                          (FTP_SERVER_PART,
                          ExtractFileName(ParamStr(0)).c_str(),"Data",
                          uniqueStr,ChangeFileExt( FileNameSet, ".pro").c_str(),
                          szCompName,szUserName,szLocalIP) ) ).c_str(),
                  true ); // � ������� ����� ��� ����������� � ����� �� ������
//
 delete szCompName;
 delete szUserName;
 delete szLocalIP;
// delete szDateTime;
//
} // ----- ����� Upload_Data() -------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void __fastcall Unload_Install() // ��������� � c������ ��������������� ������ ��������
{
 char FileNameOnServer[_512]="\0", // ������ ����� ����� �� ������� � �������
      FileNameOnClient[_512]="\0";
//
 snprintf( PathToSubDirInData,sizeof(PathToSubDirInData), "%s%s/", ExtractFilePath(Application->ExeName), NameSubDirInData); // + In!Data
 ReplaceEqualLengthSubstring( PathToSubDirInData, "\\", "/" ); // ������ "\" �� "/"
//
 if( !DirectoryExists( PathToSubDirInData ) ) // ���� �� ���������� ����� ��������...
  if( !CreateDir( PathToSubDirInData ) ) // ���� �� ������� �������...
   strcpy( PathToSubDirInData, '\0' ); // �������� ���� � ����������� PathToSubDirOutData
//
 snprintf( FileNameOnServer,sizeof(FileNameOnServer), "%s/%s", FTP_SERVER, FILENAMESERVER ); // ������ ��� ����� �� ������� (+++)
//
 char szFILENAMESERVER[_512]; // ������� ������
 strcpy( szFILENAMESERVER, FILENAMESERVER ); // �������� ������-��������� � ������� ������
 ReplaceEqualLengthSubstring( szFILENAMESERVER, "/", "\\" ); // �������� "/" � "\" , ��� ExtractFileName() �� �������� � '/'
 snprintf( FileNameOnClient,sizeof(FileNameOnClient), "%s%s",  PathToSubDirInData, ExtractFileName(szFILENAMESERVER) ); // ���� ��������� �� ������� (+++) ????
//
// t_printf( "\n\n\nS=%s\nC=%s\n\n", FileNameOnServer, FileNameOnClient );
//
 GetFileFromServer( FileNameOnServer, // �������� ����� � �������...
                    FileNameOnClient, // ...�� ������
                    true ); // � ������� ����� ��� ����������� � ����� �� ������
//
} // ----- ����� Unload_Install() ----------------------------------------------

////////////////////////////////////////////////////////////////////////////////
void __fastcall Work_LogInOut( bool Rule )
{ // ������/��������� ������ � ���������� DATA_FLOW.EXE
unsigned long szSize = _1024;
char *szCompName = new char[szSize], // ��� ����������
     *szUserName = new char[szSize], // ��� ������������
     *szLocalIP  = new char[szSize], // ��������� IP
     *szDateTime = new char[szSize]; // ������� ����/�����
//
 GetComputerName(szCompName, &szSize); // ��� ����������
 GetUserName(szUserName, &szSize); // ��� ������������
 strcpy( szLocalIP,  F1->IPWatch->LocalIP().c_str() ) ; // ������� IP
 strcpy( szDateTime, PutDateTimeToString(1) ) ; // ������� ����/����� (���������� uniqueStr - ������������� � Start_DF() )
//
 char logInOut[_512] = "\0"; // EXE-��� + LogIn/LogOut ��� Rule = true/false
 sprintf( logInOut, "%s %s", ExtractFileName(ParamStr(0)).c_str(), Rule ? "LogIn" : "LogOut" );
/*
 char str[_256] = "\0";
 sprintf ( str,  Format("%s/[%s][%s][%s][%s][%s][%s].upload", // ... �� ������
                 OPENARRAY( TVarRec,
                 (FTP_SERVER_PART,logInOut,szDateTime,ExtractFileName(FileNameINI),szCompName,szUserName,szLocalIP)) ).c_str()) ;
//
 t_printf( "\n%s\n%s\n%s   %d\n", FileNameINI, logInOut, str, strlen(str) );
*/
//return;
//
 PostFileToServer( FileNameINI, // �������� INI-����� � �������...
                   Format("%s/[%s][%s][%s][%s][%s][%s].upload", // ... �� ������
                   OPENARRAY( TVarRec,
                   (FTP_SERVER_PART,logInOut,szDateTime,ExtractFileName(FileNameINI),szCompName,szUserName,szLocalIP)) ).c_str() ,
                   true ) ;
//
 delete szCompName;
 delete szUserName;
 delete szLocalIP;
 delete szDateTime;
//
} // ----- Work_LogIn ----------------------------------------------------------





