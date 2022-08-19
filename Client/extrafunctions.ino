void setDefault() {
  def.eepromSet = 40;
  strcpy(def.nameOfProject, "AcilLabs");
  //def.active = 1;
  def.state = 1;          // Mac
  def.ready = 1;
  def.id = 1;
  def.laser_1 = 1;
  def.laser_2 = 1;
  def.return_data = 1;
  def.countertime = 0;
  def.offset = 0;
  def.Led.r = 0;
  def.Led.g = 0;
  def.Led.y = 0;
  def.connecedDeviceCount = 0;
  def.lastcountertime = 0;
  def.webInfo.tip = 1; // client
  strcpy(def.webInfo.host, "smeric");
  strcpy(def.webInfo.ssid, "smeric");
  strcpy(def.webInfo.ip, "192.168.137.1");
  strcpy(def.webInfo.mask, "255.255.255.0");
  strcpy(def.webInfo.gateway, "192.168.137.1");
  strcpy(def.webInfo.password, "123454321");
  strcpy(def.webInfo.serverip, "192.168.137.1");

  strcpy(def.Devices[0].ip, "192.168.137.1");
  setValues();
}
String millis2time(unsigned long millisdata) {
  String Time = "";
  unsigned long ss;
  byte mm, hh;
  ss = millisdata / 1000;
  hh = ss / 3600;
  mm = (ss - hh * 3600) / 60;
  ss = (ss - hh * 3600) - mm * 60;
  if (hh < 10)Time += "0";
  Time += (String)hh + ":";
  if (mm < 10)Time += "0";
  Time += (String)mm + ":";
  if (ss < 10)Time += "0";
  Time += (String)ss;
  return Time;
}
