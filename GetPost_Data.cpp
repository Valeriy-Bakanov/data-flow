//
#define FTP_SERVER "vbakanov.ru" // FTP-сервер
#define FTP_SERVER_PART ""FTP_SERVER"/dataflow/content" // частичный путь на сервере (FTP_SERVER - корректно конкатенируется!)
#define FILENAMESERVER  "dataflow/content/install_df.exe" // локальный путь к инсталляции системы
//

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void __fastcall TF1::FTP_Get_Connected(TObject *Sender)
{ // вызывается при соединении с сервером
 SBM0->Text = " Соединение с сервером установлено";
 Delay( 500 );
} //----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void __fastcall TF1::FTP_Get_Disconnected(TObject *Sender)
{ // вызывается при разрыве соединения с сервером
 SBM0->Text = " Соединение с сервером разорвано";
 Delay( 500 );
} //----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void __fastcall TF1::FTP_Get_OnStatus(TObject *axSender, const TIdStatus axStatus, const AnsiString asStatusText)
{ // состояние процесса вЫгрузки
  switch( axStatus )
  {
   case hsResolving:     //SBM0->Text = " A host name is being resolved for an IP address...";
                         SBM0->Text = " Доменное имя успешно разрешено в IP-адрес...";
        break;
   case hsConnecting:    //SBM0->Text = " A connection is being opened...";
                         SBM0->Text = " Соединение устанавливается...";
        break;
   case hsConnected:    //SBM0->Text = " A connection has been made...";
                        SBM0->Text = " Соединение успешно устанавлено...";
        break;
   case hsDisconnecting: //SBM0->Text = " The connection is being closed...";
                         SBM0->Text = " Соединение закрывается...";
        break;
   case hsDisconnected:  //SBM0->Text = " The connection has been closed...";
                         SBM0->Text = " Соединение разорвано...";
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
{ // вызывается при вЫгрузке данных с сервера на клиент
 if( FileSizeFromServer )
  SBM0->Text = Format(" %.0f%% (из %d байт) данных получено с сервера",
                      OPENARRAY(TVarRec, (100.0*AWorkCount/FileSizeFromServer, int(FileSizeFromServer)) ) );
 Delay( 1 );
} //----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void __fastcall TF1::FTP_Get_OnWorkBegin(TObject *Sender, TWorkMode AWorkMode, const int AWorkCountMax)
{ // вызывается в начало вЫгрузке данных с сервера на клиент
// FileSizeFromServer = AWorkCountMax; // !!!!! при Get всегда AWorkCountMax == 0 !!!!!
 SBM0->Text = Format(" %d байт запрошено для получения с сервера", OPENARRAY(TVarRec, (int(FileSizeFromServer)) ) );
 Delay( 500 );
} //----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void __fastcall TF1::FTP_Get_OnWorkEnd(TObject *Sender, TWorkMode AWorkMode)
{ // вызывается в конце вЫгрузки данных с сервера на клиент
 Delay( 500 );
 SBM0->Text = Format(" %d байт получено с сервера", OPENARRAY(TVarRec, (int(FileSizeFromServer)) ) );
} //----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void __fastcall TF1::EndedUploadFile(TObject *Sender)
{ // принудительно разорвать соединение с сервером - прекратить вЫгрузку файла
 F1->FTP_Get->Disconnect(); // разрываем соединение с сервером
} //----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void __fastcall TF1::OnClickGetInstall(TObject *Sender)
{ // получить файл с сервера
 Unload_Install(); // получить инсталляционный файл системы с сервера
} // ---------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
char* __fastcall UserPassword( int Rule )
{ // при Rule =1/#1 генерятся строки User/Password соответственно
 INT k, n=1, Sum=0, Max=1000; // Max - верхний предел поиска простых чисел
 static char User[_128]="\0", // строка Usr
             Password[_128]="\0"; // строка Password
//
 if( strlen(User) && strlen(Password) ) // если непустые строки...
 {
  if( Rule == 1 ) return User ;
  else            return Password ;
 }
//
 for (INT i=2; i<Max; i++) // перебираем числа
 {
  bool y = 1; // пусть будет простое
  if (i%2 == 0) y = 0; // делится нацело на 2, значит не простое
  for (INT j=3; (j*j <= i) && y; j+=2)
   if (i%j == 0) {y = 0; break;} // делится нацело на какое нибудь число до корня, значит не простое
   if(y)
   {
    n ++ ;
    Sum += i ;
//    t_printf( "n=%10d   i=%20d   [i+Sum] = %20d  [(i+Sum)%%i] = %-20d [(Sum-i)%%i] = %-20d", n, i, i+Sum, (Sum+i) % i, (Sum-i) % i );
//    Application->ProcessMessages();
//
    k=(i+Sum) % i ; // определяющее число (i+Sum)%i
    // "valer-bak" // [i](строки)=i(номер простого числа)-N(константа)
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
    k=(Sum-i) % i ; // определяющее число (i+Sum)%i
    // "5k9lpsk" // [i](строки)=i(номер простого числа)-N(константа)
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
} // --- формирование строк User/Password на основе генератора простых чисел ---

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void __fastcall GetFileFromServer( char *FileNameSource, char *FileNameDestination, bool Replace  )
{ // получить файл с HTTP-сервера
//
// F1->FTP_Get->OnWorkBegin = F1->FTP_Get_OnWorkBegin ; // при Get всегда AWorkCountMax == 0 !!!!!
//
 F1->FTP_Get->Host     = FTP_SERVER ; // "192.162.103.128" ;
 F1->FTP_Get->User     = UserPassword( 1 ) ;
 F1->FTP_Get->Password = UserPassword( 2 ) ;
//
 F1->FTP_Get->TransferType = ftBinary ; // для корректности пересылки двоичных файлов
//
 if( F1->FTP_Get->Connected() ) // проверочка-с..!
  F1->FTP_Get->Disconnect();
//
 try
 {
  F1->FTP_Get->Connect() ; // пытаемся соединиться с сервером...
  FileSizeFromServer = F1->FTP_Get->Size( FileNameSource ) ; // получаем размер выгружаемого файла ( невозможно по OnWorkBegin )
 }
  catch(const EIdException& E) // ловим исключение Indy-компонента
 {
  SBM0->Text = Format(" Ошибка соединения: %s", OPENARRAY(TVarRec, (E.Message) ) );
  MessageBeep( MB_ICONASTERISK ); // предупреждение...
  ShowMessage( Format(" Ошибка соединения: %s\n%s", OPENARRAY(TVarRec, (E.Message, "Похоже, у Вас проблемы с Сетью..!") ) ) );
  F1->FTP_Get->Disconnect(); // отсоединяемся от сервера
  return;
 }
//
 try
 {
  F1->FTP_Get->Get( FileNameSource,
                    FileNameDestination,
                    Replace ) ; // собственно загрузка файла
 }
  catch(const EIdException& E) // ловим исключение Indy-компонента
 {
  SBM0->Text = Format(" Ошибка загрузки файла c серверf: %s", OPENARRAY(TVarRec, (E.Message) ) );
  MessageBeep( MB_ICONASTERISK ); // предупреждение...
  ShowMessage(" Ошибка загрузки файла на сервер: " + E.Message);
 }
//
 F1->FTP_Get->Disconnect(); // отсоединяемся от сервера
} // ----- конец GetFileFromServer() -------------------------------------------

   ////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void __fastcall PostFileToServer( char *FileNameSource, char *FileNameDestination, bool Replace )
{ //
//
 F1->FTP_Post->Host     = FTP_SERVER ; // "192.162.103.128" ;
 F1->FTP_Post->User     = UserPassword( 1 ) ;
 F1->FTP_Post->Password = UserPassword( 2 ) ;
//
 F1->FTP_Post->TransferType = ftBinary ; // для корректности пересылки двоичных файлов
//
 if( F1->FTP_Post->Connected() ) // проверочка-с..!
  F1->FTP_Post->Disconnect();
//
 try
 {
  F1->FTP_Post->Connect() ; // пытаемся соединиться с сервером...
 }
  catch(const EIdException& E) // ловим исключение Indy-компонента
 {
//  SBM0->Text = Format(" Ошибка соединения: %s", OPENARRAY(TVarRec, (E.Message) ) );
//  MessageBeep( MB_ICONASTERISK ); // предупреждение...
//  ShowMessage(" Ошибка соединения: " + E.Message);
  F1->FTP_Post->Disconnect(); // отсоединяемся от сервера
  return;
 }
//
 try
 {
  F1->FTP_Post->Put( FileNameSource, FileNameDestination, Replace ) ; // собственно вЫгрузка файла на сервер
 }
  catch(const EIdException& E) // ловим исключение Indy-компонента
 {
//  SBM0->Text = Format(" Ошибка загрузки файла на сервер: %s", OPENARRAY(TVarRec, (E.Message) ) );
//  MessageBeep( MB_ICONASTERISK ); // предупреждение...
//  ShowMessage(" Ошибка загрузки файла на сервер: " + E.Message);
 }
//
 F1->FTP_Post->Disconnect(); // отсоединяемся от сервера
} // ----- конец PostFileToServer() --------------------------------------------

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void __fastcall Upload_Data() // вЫгрузить данные на сервер
{
unsigned long szSize = _1024;
char *szCompName = new char[szSize], // имя компьютера
     *szUserName = new char[szSize], // имя пользователя
     *szLocalIP  = new char[szSize]; // локальный IP
//     *szDateTime = new char[szSize]; // текущие дата/время
//
 GetComputerName(szCompName, &szSize); // имя компьютера
 GetUserName(szUserName, &szSize); // имя пользователя
 strcpy( szLocalIP,  F1->IPWatch->LocalIP().c_str() ) ; // текущий IP
// strcpy( szDateTime, PutDateTimeToString(1) ) ; // текущие дата/время (используем uniqueStr - присваивается в Start_DF() )
//
 PostFileToServer( FileNameSet, // вЫгрузка SET-файла с клиента...
                   Format("%s/[%s %s][%s][%s][%s][%s][%s].upload", // ... на сервер
                          OPENARRAY( TVarRec,
                          (FTP_SERVER_PART,
                          ExtractFileName(ParamStr(0)).c_str(),"Data",
                          uniqueStr,FileNameSet,
                          szCompName,szUserName,szLocalIP) ) ).c_str(),
                  true ); // с заменой файла при существании с таким же именем
///
 PostFileToServer( tmpPRO, // вЫгрузка PRO-файла с клиента...
                   Format("%s/[%s %s][%s][%s][%s][%s][%s].upload", // ... на сервер
                          OPENARRAY( TVarRec,
                          (FTP_SERVER_PART,
                          ExtractFileName(ParamStr(0)).c_str(),"Data",
                          uniqueStr,ChangeFileExt( FileNameSet, ".pro").c_str(),
                          szCompName,szUserName,szLocalIP) ) ).c_str(),
                  true ); // с заменой файла при существании с таким же именем
//
 delete szCompName;
 delete szUserName;
 delete szLocalIP;
// delete szDateTime;
//
} // ----- конец Upload_Data() -------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void __fastcall Unload_Install() // загрузить с cервера инсталляционную версию продукта
{
 char FileNameOnServer[_512]="\0", // полные имена файла на сервере и клиенте
      FileNameOnClient[_512]="\0";
//
 snprintf( PathToSubDirInData,sizeof(PathToSubDirInData), "%s%s/", ExtractFilePath(Application->ExeName), NameSubDirInData); // + In!Data
 ReplaceEqualLengthSubstring( PathToSubDirInData, "\\", "/" ); // замена "\" на "/"
//
 if( !DirectoryExists( PathToSubDirInData ) ) // если не существует этого каталога...
  if( !CreateDir( PathToSubDirInData ) ) // если не удалось создать...
   strcpy( PathToSubDirInData, '\0' ); // обнуляем путь к подкаталогу PathToSubDirOutData
//
 snprintf( FileNameOnServer,sizeof(FileNameOnServer), "%s/%s", FTP_SERVER, FILENAMESERVER ); // полное имя файла на сервере (+++)
//
 char szFILENAMESERVER[_512]; // рабочая строка
 strcpy( szFILENAMESERVER, FILENAMESERVER ); // копируем строку-константу в рабочую строку
 ReplaceEqualLengthSubstring( szFILENAMESERVER, "/", "\\" ); // перевели "/" в "\" , ибо ExtractFileName() не работает с '/'
 snprintf( FileNameOnClient,sizeof(FileNameOnClient), "%s%s",  PathToSubDirInData, ExtractFileName(szFILENAMESERVER) ); // куда сохранять на клиенте (+++) ????
//
// t_printf( "\n\n\nS=%s\nC=%s\n\n", FileNameOnServer, FileNameOnClient );
//
 GetFileFromServer( FileNameOnServer, // загрузка файла с сервера...
                    FileNameOnClient, // ...на клиент
                    true ); // с заменой файла при существании с таким же именем
//
} // ----- конец Unload_Install() ----------------------------------------------

////////////////////////////////////////////////////////////////////////////////
void __fastcall Work_LogInOut( bool Rule )
{ // начать/завершить работу с программой DATA_FLOW.EXE
unsigned long szSize = _1024;
char *szCompName = new char[szSize], // имя компьютера
     *szUserName = new char[szSize], // имя пользователя
     *szLocalIP  = new char[szSize], // локальный IP
     *szDateTime = new char[szSize]; // текущие дата/время
//
 GetComputerName(szCompName, &szSize); // имя компьютера
 GetUserName(szUserName, &szSize); // имя пользователя
 strcpy( szLocalIP,  F1->IPWatch->LocalIP().c_str() ) ; // текущий IP
 strcpy( szDateTime, PutDateTimeToString(1) ) ; // текущие дата/время (используем uniqueStr - присваивается в Start_DF() )
//
 char logInOut[_512] = "\0"; // EXE-имя + LogIn/LogOut при Rule = true/false
 sprintf( logInOut, "%s %s", ExtractFileName(ParamStr(0)).c_str(), Rule ? "LogIn" : "LogOut" );
/*
 char str[_256] = "\0";
 sprintf ( str,  Format("%s/[%s][%s][%s][%s][%s][%s].upload", // ... на сервер
                 OPENARRAY( TVarRec,
                 (FTP_SERVER_PART,logInOut,szDateTime,ExtractFileName(FileNameINI),szCompName,szUserName,szLocalIP)) ).c_str()) ;
//
 t_printf( "\n%s\n%s\n%s   %d\n", FileNameINI, logInOut, str, strlen(str) );
*/
//return;
//
 PostFileToServer( FileNameINI, // загрузка INI-файла с клиента...
                   Format("%s/[%s][%s][%s][%s][%s][%s].upload", // ... на сервер
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





