
void ws_atz(int arg_cnt, char **args) {
  sprintf(outputString, "%s v%s", "Acil Labs", "3");
  webSocket.sendTXT(outputString);
}


void ws_at_info(int arg_cnt, char **args) {
String jsonText="{\"eepromSet\"="+(String)def.eepromSet+","+
              "\"nameOfProject\"=\""+(String)def.nameOfProject+"\","+
              "\"id\"="+(String)def.id+","+
              "\"tip\"="+(String)def.tip+","+
              "\"connecedDeviceCount\"="+(String)def.connecedDeviceCount+","+
              "\"offset\"="+(String)def.offset+","+
              "\"ready\"="+(String)def.ready+","+
              "\"state\"="+(String)def.state+","+
              "\"laser_1\"="+(String)def.laser_1+","+
              "\"laser_2\"="+(String)def.laser_2+","+
              "\"return_data\"="+(String)def.return_data+","+
              "\"countertime\"="+(String)def.countertime+","+
              "\"lastcountertime\"="+(String)def.lastcountertime+"}";
              
              
//              ip[15] = "192.168.1.1";
//              
//  int eepromSet = 0;
//  char nameOfProject[13] = "Acil Labs";
//  int id = 1;
//  int tip = 1;
//  unsigned long offset = 4000;
//  int ready = 0;
//  int state = 0;
//  int laser_1 = 0;
//  int laser_2 = 0;
//  int return_data = 0;
//  unsigned long countertime = 0;
//  unsigned long lastcountertime = 0;
//  devices Devices[4];
//  webinfo webInfo;
//  led Led;
//} parameters;
  webSocket.sendTXT(jsonText);
}

void ws_at_laser(int arg_cnt, char **args) {
  if (arg_cnt > 1) {
    int cmdint = cmdStr2Num(args[1], 10);
    if (cmdint == 0) //
    {
      if (def.laser_1 == 0) {
        webSocket.sendTXT("acildi");
        def.laser_1 = 1;
      } else {
        webSocket.sendTXT("kapandi");
        def.laser_1 = 0;
      }
    } else {
      if (def.laser_2 == 0) {
        webSocket.sendTXT("acildi");
        def.laser_2 = 1;
      } else {
        webSocket.sendTXT("kapandi");
        def.laser_2 = 0;
      }

    }
    setValues();
  }

}
void ws_at_eeprom(int arg_cnt, char **args) {
  if (arg_cnt > 1) {
    char c = *args[1];
    if (c == 'W' || c == 'w') // Write all parameters to eeprom // eproma mevcut veriler yaz??l??yor. Ew
    {
      EEPROM.put(0, def);
      EEPROM.commit();
    } else if (c == 'R' || c == 'r') // Read all parameters from eeprom // epromda kay??tl?? veriler aktif degi??kene y??kleniyor - yap??lan de??i??iklikleri geri almak i??in kullan??l??r. - Er
    {
      EEPROM.get(0, def);
    } else if (c == 'D' || c == 'd') // System Restore Point // Orjinal firmware verileri y??klenir. - Ed
    {
      setDefault();
    }
  };
  webSocket.sendTXT("OK");
}
void ws_at_reset(int arg_cnt, char **args) {
  webSocket.sendTXT("Restrting in 3 seconds");
  webSocket.sendTXT("3");
  delay(1000);
  webSocket.sendTXT("2");
  delay(1000);
  webSocket.sendTXT("1");
  delay(1000);
  ESP.restart();
}

void ws_at_setwifi(int arg_cnt, char **args) {
  if (arg_cnt > 1) {
    int sizeofarg1 = sizeof(args[1]);

    
    if (strncmp("id", args[1], sizeofarg1) == 0) {
      def.id = cmdStr2Num(args[2], 10);
    } else
    if (strncmp("offset", args[1], sizeofarg1) == 0) {
      def.offset = cmdStr2Long(args[2]);
    } else
    if (strncmp("ready", args[1], sizeofarg1) == 0) {
      def.ready = cmdStr2Num(args[2], 10);
    } else
    if (strncmp("state", args[1], sizeofarg1) == 0) {
      def.state = cmdStr2Long(args[2]);
    } else
    if (strncmp("return_data", args[1], sizeofarg1) == 0) {
      def.return_data = cmdStr2Long(args[2]);
    } else
    if (strncmp("countertime", args[1], sizeofarg1) == 0) {
      def.countertime = cmdStr2Long(args[2]);
    } else
    if (strncmp("lastcountertime", args[1], sizeofarg1) == 0) {
      def.lastcountertime = cmdStr2Long(args[2]);
    } else

    if (strncmp("ssid", args[1], sizeofarg1) == 0) {
      strcpy(def.webInfo.ssid, args[2]);
    } else if (strncmp("password", args[1], sizeofarg1) == 0) {
      strcpy(def.webInfo.password, args[2]);
    } else if (strncmp("host", args[1], sizeofarg1) == 0) {
      strcpy(def.webInfo.host, args[2]);
    } else if (strncmp("ip", args[1], sizeofarg1) == 0) {
      strcpy(def.webInfo.ip, args[2]);
    } else if (strncmp("mask", args[1], sizeofarg1) == 0) {
      strcpy(def.webInfo.mask, args[2]);
    } else if (strncmp("gateway", args[1], sizeofarg1) == 0) {
      strcpy(def.webInfo.gateway, args[2]);
    } else if (strncmp("tip", args[1], sizeofarg1) == 0) {
      def.webInfo.tip = cmdStr2Num(args[2], 10);
    } else if (strncmp("serverip", args[1], sizeofarg1) == 0) {
      strcpy(def.webInfo.serverip, args[2]);
    } else if (strncmp("port", args[1], sizeofarg1) == 0) {
      def.webInfo.port = cmdStr2Long(args[2]);
    } else if (strncmp("laser", args[1], sizeofarg1) == 0)
    {
      int cmdint = cmdStr2Num(args[2], 10);
      if (cmdint == 0) {
        def.laser_1 = 0;
        if (cmdStr2Num(args[3], 10) == 1)
          def.laser_1 = 1;
      } else {
        def.laser_2 = 0;
        if (cmdStr2Num(args[3], 10) == 1)
          def.laser_2 = 1;
      }
      setValues();
    }
  };
  webSocket.sendTXT ("ssid\t:\t" + (String)def.webInfo.ssid);
  webSocket.sendTXT ("password\t:\t" + (String)def.webInfo.password);
  webSocket.sendTXT ("host\t:\t" + (String)def.webInfo.host);
  webSocket.sendTXT ("ip\t:\t" + (String)def.webInfo.ip);
  webSocket.sendTXT ("serverip\t:\t" + (String)def.webInfo.serverip);
  webSocket.sendTXT ("port\t:\t" + (String)def.webInfo.port);
  webSocket.sendTXT ("gateway\t:\t" + (String)def.webInfo.gateway);
  webSocket.sendTXT ("mask\t:\t" + (String)def.webInfo.mask);
  webSocket.sendTXT ("laser in\t:\t" + (String)def.laser_1);
  webSocket.sendTXT ("laser out\t:\t" + (String)def.laser_2);
  webSocket.sendTXT ("tip\t:\t" + (String)def.webInfo.tip);
  webSocket.sendTXT ("OK");
}
