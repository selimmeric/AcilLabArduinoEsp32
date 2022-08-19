
void cmd_atz(int arg_cnt, char **args) {
  sprintf(outputString, "%s v%s", "Acil Labs", "3");
  SerialOut.println(outputString);
  SerialOut.println("OK");
}

void cmd_at_laser(int arg_cnt, char **args) {
  if (arg_cnt > 1) {
    int cmdint = cmdStr2Num(args[1], 10);
    if (cmdint == 0) //
    {
      if (def.laser_1 == 0) {
        SerialOut.println(F("acildi"));
        def.laser_1 = 1;
      } else {
        SerialOut.println(F("kapandi"));
        def.laser_1 = 0;
      }
    } else {
      if (def.laser_2 == 0) {
        SerialOut.println(F("acildi"));
        def.laser_2 = 1;
      } else {
        SerialOut.println(F("kapandi"));
        def.laser_2 = 0;
      }
    }
    setValues();
  }
}
void cmd_at_eeprom(int arg_cnt, char **args) {
  if (arg_cnt > 1) {
    char c = *args[1];
    if (c == 'W' || c == 'w') // Write all parameters to eeprom // eproma mevcut veriler yazılıyor. Ew
    {
      EEPROM.put(0, def);
      EEPROM.commit();
    } else if (c == 'R' || c == 'r') // Read all parameters from eeprom // epromda kayıtlı veriler aktif degişkene yükleniyor - yapılan değişiklikleri geri almak için kullanılır. - Er
    {
      EEPROM.get(0, def);
    } else if (c == 'D' || c == 'd') // System Restore Point // Orjinal firmware verileri yüklenir. - Ed
    {
      setDefault();
    }
  };
  SerialOut.println(F("OK"));
}
void cmd_at_reset(int arg_cnt, char **args) {
  SerialOut.println (F("Restarting in 3 seconds"));
  SerialOut.println (F("3"));
  delay(1000);
  SerialOut.println (F("2"));
  delay(1000);
  SerialOut.println (F("1"));
  delay(1000);
  ESP.restart();
}
void cmd_at_info(int arg_cnt, char **args) {
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
              
              

  SerialOut.println(jsonText);
}
void cmd_at_setwifi(int arg_cnt, char **args) {
  if (arg_cnt > 1) {
    int sizeofarg1 = sizeof(args[1]);
    if (strncmp("id", args[1], sizeofarg1) == 0) {
      def.id = cmdStr2Num(args[2], 10);
    } else if (strncmp("offset", args[1], sizeofarg1) == 0) {
      def.offset = cmdStr2Long(args[2]);
    } else if (strncmp("ready", args[1], sizeofarg1) == 0) {
      def.ready = cmdStr2Num(args[2], 10);
    } else if (strncmp("state", args[1], sizeofarg1) == 0) {
      def.state = cmdStr2Long(args[2]);
    } else if (strncmp("return_data", args[1], sizeofarg1) == 0) {
      def.return_data = cmdStr2Long(args[2]);
    } else if (strncmp("countertime", args[1], sizeofarg1) == 0) {
      def.countertime = cmdStr2Long(args[2]);
    } else if (strncmp("lastcountertime", args[1], sizeofarg1) == 0) {
      def.lastcountertime = cmdStr2Long(args[2]);
    } else if (strncmp("ssid", args[1], sizeofarg1) == 0) {
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
  SerialOut.print (F("ssid          : \t"));
  SerialOut.println (def.webInfo.ssid);
  SerialOut.print (F("password      : \t"));
  SerialOut.println (def.webInfo.password);
  SerialOut.print (F("host          : \t"));
  SerialOut.println (def.webInfo.host);
  SerialOut.print (F("ip          : \t"));
  SerialOut.println (def.webInfo.ip);
  SerialOut.print (F("serverip          : \t"));
  SerialOut.println (def.webInfo.serverip);
  SerialOut.print (F("port          : \t"));
  SerialOut.println (def.webInfo.port);
  SerialOut.print (F("gateway          : \t"));
  SerialOut.println (def.webInfo.gateway);
  SerialOut.print (F("mask          : \t"));
  SerialOut.println (def.webInfo.mask);
  SerialOut.print (F("laser in: \t"));
  SerialOut.println (def.laser_1);
  SerialOut.print (F("laser out: \t"));
  SerialOut.println (def.laser_2);
  SerialOut.print (F("tip     : \t"));
  SerialOut.println (def.webInfo.tip);
  SerialOut.println(F("OK"));
}
void setValues() {
  if (def.Led.r == 0) {
    digitalWrite(LEDRED_PIN, LOW); // sets the digital pin on
    SerialOut.println(F("Kirmizi Led Kapandı"));
  }  else {
    digitalWrite(LEDRED_PIN, HIGH);
    SerialOut.println(F("Kirmizi Led Acildi"));
  }
  if (def.Led.g == 0) {
    digitalWrite(LEDGREEN_PIN, LOW);  // sets the digital pin on
    SerialOut.println(F("Yeşil Led Kapandı"));
  } else {
    digitalWrite(LEDGREEN_PIN, HIGH);
    SerialOut.println(F("Yeşil Led Acildi"));
  }
  if (def.Led.y == 0) {
    digitalWrite(LEDYELLOW_PIN, LOW);  // sets the digital pin on
    SerialOut.println(F("Sarı Led Kapandı"));
  } else {
    digitalWrite(LEDYELLOW_PIN, HIGH);
    SerialOut.println(F("Sarı Led Acildi"));
  }
  if (def.laser_1 == 0) {
    digitalWrite(LASER_PIN_IN, LOW); // sets the digital pin on
    SerialOut.println(F("Lazer 1 Kapandı"));
  } else {
    digitalWrite(LASER_PIN_IN, HIGH);
    SerialOut.println(F("Lazer 1 Acildi"));
  }
  if (def.laser_2 == 0) {
    digitalWrite(LASER_PIN_OUT, LOW);  // sets the digital pin on
    SerialOut.println(F("Lazer 2 Kapandı"));
  } else {
    digitalWrite(LASER_PIN_OUT, HIGH);
    SerialOut.println(F("Lazer 2 Acildi"));
  }
}
