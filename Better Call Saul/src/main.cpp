#include <BluetoothSerial.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>

BluetoothSerial SerialBT;

String data = "";

String teamId = "";

void Lista_Retele() {
  DynamicJsonDocument networksDoc(512);
  int numNetworks = WiFi.scanNetworks();
  for (int i = 0; i < numNetworks; ++i) {
    DynamicJsonDocument networkDoc(256); 
    JsonObject network = networkDoc.to<JsonObject>();
    network["ssid"] = WiFi.SSID(i);
    network["strength"] = WiFi.RSSI(i);
    network["encryption"] = WiFi.encryptionType(i);
    network["teamId"] = teamId; 
    String networkJson;
    serializeJson(network, networkJson);
    SerialBT.println(networkJson); 
    delay(100); 
  }
}

void conecteaza(String ssid, String pass)
{
    WiFi.begin(ssid.c_str(), pass.c_str());
    delay(7000);
    DynamicJsonDocument doc_raspuns(512);
    JsonObject raspuns = doc_raspuns.to<JsonObject>();
    raspuns["ssid"] = ssid;
    if ( WiFi.status() == WL_CONNECTED) 
        raspuns["connected"] = true;
    else 
        raspuns["connected"] = false;
    raspuns["teamId"] = teamId;
    String raspunsJson;
    serializeJson(raspuns, raspunsJson);
    SerialBT.println(raspunsJson);
}

void get_data()
{

  HTTPClient http;
    String url = "http://proiectia.bogdanflorea.ro/api/better-call-saul/characters";
    http.begin(url);
    http.setConnectTimeout(30000);
    http.setTimeout(30000);
    int httpResponseCode = http.GET();
    if (httpResponseCode > 0) {
      Serial.print("HTTP Response code: ");
      String payload = http.getString();
      DynamicJsonDocument jsonDoc(1024);
      deserializeJson(jsonDoc, payload);
      for(JsonObject obj : jsonDoc.as<JsonArray>())
      {
        DynamicJsonDocument objDoc(512);
        objDoc["id"] = obj["char_id"];
        objDoc["name"] = obj["name"];
        objDoc["image"] = obj["img"]; 
        objDoc["teamId"] = teamId; 
        String objJson;
        serializeJson(objDoc, objJson);
        SerialBT.println(objJson);
      }
    http.end();
  }
}

void detalii_extra(String id)
{
  HTTPClient http;
  String url = "http://proiectia.bogdanflorea.ro/api/better-call-saul/character?char_id=" + String(id);
  http.begin(url);
  http.setConnectTimeout(30000);
  http.setTimeout(30000);
  int httpResponseCode = http.GET();
  String payload = http.getString();
  DynamicJsonDocument jsonDoc(1024);
  deserializeJson(jsonDoc, payload);
  DynamicJsonDocument objDoc(512);
  objDoc["id"] = jsonDoc["char_id"];
  objDoc["name"] = jsonDoc["name"];
  objDoc["image"] = jsonDoc["img"];
  String birthday = jsonDoc["birthday"].as<String>();
  String status = jsonDoc["status"].as<String>();
  String nickname = jsonDoc["nickname"].as<String>();
  String portrayed = jsonDoc["portrayed"].as<String>();
  String occupationStr = "";
  bool ok = true;
  for (const auto& job : jsonDoc["occupation"].as<JsonArray>()) {
    if(!ok)
    {
      occupationStr += ", ";
    }
    occupationStr += job.as<String>();
    ok = false;
  }
  String appearanceStr = "";
  ok = true;
  for (const auto& season : jsonDoc["appearance"].as<JsonArray>()) {
    if (!ok) {
      appearanceStr += ", ";
    }
    appearanceStr += season.as<String>();
    ok = false;
  }
  String formattedString = "Birthday: " + birthday + "\n" +
                    "Occupation: " + occupationStr + "\n" +
                    "Status: " + status + "\n" +
                    "Nickname: " + nickname + "\n" +
                    "Portrayed: " + portrayed + "\n" +
                    "Appearance: " + appearanceStr;

  objDoc["description"] = formattedString.c_str();
  objDoc["teamId"] = teamId; 
  String objJson;
  serializeJson(objDoc, objJson);
  Serial.println(objJson);
  SerialBT.println(objJson);
  http.end();
}


void receivedData() {
  while (SerialBT.available()) {
    data = SerialBT.readStringUntil('\n');
  }
  Serial.println(data); 
  DynamicJsonDocument doc(15000);
  DeserializationError error = deserializeJson(doc, data);

  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.c_str());
    return;
  }

  String action = doc["action"];
  if (action == "getNetworks") {
    teamId = doc["teamId"].as<String>(); 
    Lista_Retele();
  }
  else if(action == "connect")
  {
    String ssid = doc["ssid"];
    String pass = doc["password"];
    conecteaza(ssid, pass);
  }
  else if(action == "getData")
  {
    get_data();
  }
  else if(action == "getDetails")
  {
    Serial.println(action);
    String id = doc["id"];
    detalii_extra(id);
  }
  data = "";
}

void setup() {
  Serial.begin(115200);
  SerialBT.begin("Better Call Saul API"); 
  Serial.println("The device started, now you can pair it with bluetooth!");
  WiFi.mode(WIFI_STA); 
}

void loop() {
  if (SerialBT.available()) {
    receivedData();
  }
}

