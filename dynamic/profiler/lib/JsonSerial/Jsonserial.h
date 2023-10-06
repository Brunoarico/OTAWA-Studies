#ifndef Jsonserial_h
#define Jsonserial_h

#include <Arduino.h>
#include <ArduinoJson.h>
#define JSON_BUFFER_SIZE 1024

#ifndef _ARRAY
typedef struct {
  int * array;
  int size;
} Array;
#define _ARRAY
#endif

class Jsonserial {
public:
  void begin(Stream& serial);
  StaticJsonDocument<JSON_BUFFER_SIZE> processJSON();
  String processSerialData();
  Array* getArraysFromJson();
  Array getParamFromJson();
  String getFuncFromJson();
  bool isJsonAvailable();
  void sendWcets(long* wcets, int size);          //ToDo

private:
  bool isJsonComplete;
  char receivedJson[256];
  
  
  Stream* _serialPtr;
  StaticJsonDocument<JSON_BUFFER_SIZE> jsonDocument;
};

#endif
