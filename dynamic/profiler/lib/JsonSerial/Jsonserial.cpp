#include "Jsonserial.h"

void Jsonserial::begin(Stream& serial) {
  _serialPtr = &serial;
  isJsonComplete = false;
}

bool Jsonserial::isJsonAvailable() {
  return _serialPtr->available() > 0;
}

String Jsonserial::processSerialData() {
  String serialData = _serialPtr->readStringUntil('\n');
  return serialData;
}

StaticJsonDocument<JSON_BUFFER_SIZE> Jsonserial::processJSON() {
  String jsonStr = processSerialData();
  DeserializationError error = deserializeJson(jsonDocument, jsonStr);
  if (error) {
    _serialPtr->print("JSON parsing error: ");
    _serialPtr->println(error.c_str());
    return StaticJsonDocument<JSON_BUFFER_SIZE>();
  }
  else {
    return jsonDocument;
  }
}

Array* Jsonserial::getArraysFromJson() {
  Array* paramArr = NULL;
  if (jsonDocument.containsKey("parametersArray") && jsonDocument["parametersArray"].is<JsonArray>()) {
    JsonArray jsonData = jsonDocument["parametersArray"];
    paramArr = new Array[jsonData.size()];
    for (int i = 0; i < jsonData.size(); i++) {
      paramArr[i].array = new int[jsonData[i].size()];
      paramArr[i].size = jsonData[i].size();
      for (int j = 0; j < jsonData[i].size(); j++) {
        paramArr[i].array[j] = jsonData[i][j];
      }
    }
  }
  return paramArr;
}

Array Jsonserial::getParamFromJson() {
  Array paramArr;
  paramArr.array = NULL;
  paramArr.size = 0;
  if (jsonDocument.containsKey("parameters") && jsonDocument["parameters"].is<JsonArray>()) {
    JsonArray jsonData = jsonDocument["parameters"];
    paramArr.array = new int[jsonData.size()];
    paramArr.size = jsonData.size();
    for (int i = 0; i < jsonData.size(); i++) {
        paramArr.array[i] = jsonData[i];
    }
  }
  return paramArr;
}

String Jsonserial::getFuncFromJson() {
  String fname;
  if (jsonDocument.containsKey("namefunc") && jsonDocument["namefunc"].is<String>()) {
    fname = jsonDocument["namefunc"].as<String>();
  }
  return fname;
}

void Jsonserial::sendWcets(long* wcets, int size) {
  StaticJsonDocument<JSON_BUFFER_SIZE> jsonOut;
  JsonArray wcetsArray = jsonOut.createNestedArray("wcets");
  for (int i = 0; i < size; i++) {
    wcetsArray.add(wcets[i]);
  }

  serializeJson(jsonOut, *_serialPtr);
  _serialPtr->println();
}