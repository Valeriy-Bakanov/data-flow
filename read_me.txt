
 IdFTP1->Password = "";
      IdFTP1->User = "anonymous";
      IdFTP1->Host  = "linux4u.jinr.ru";
      IdFTP1->Port = 21;
      IdFTP1->Connect();
      TMemoryStream *MS=new TMemoryStream;
      IdFTP1->Get("/welcome.msg",MS);
      MS->SaveToFile("C:\\mirrors");
      MS->Clear();
      IdFTP1->Disconnect();
      delete MS; 




TStringStream *respon = new TStringStream();
TStringList *post= new TStringList("");
post->Add(UnicodeString("param1=")+UnicodeString(Edit1->Text));
post->Add(UnicodeString("param2=")+UnicodeString(Edit2->Text));
IdHTTP1->Post(host+"slogin.php",post,respon);




https://ru.wikipedia.org/wiki/%D0%A1%D0%BF%D0%B8%D1%81%D0%BE%D0%BA_%D0%BA%D0%BE%D0%B4%D0%BE%D0%B2_%D0%BE%D1%82%D0%B2%D0%B5%D1%82%D0%BE%D0%B2_FTP