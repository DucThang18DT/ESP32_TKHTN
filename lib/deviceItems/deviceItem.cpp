#include "DeviceItem.h"

void DeviceItem::newPinName(int pinName){
  _pinName = pinName;
  // Serial.printf("\nnewPin = %d\n", _pinName);
  pinMode(_pinName, OUTPUT);
}

DeviceItem::DeviceItem(String name, int id, int pinName, Status status, bool timer, bool timerOnState, bool timerOffState, TypeStatus typeState,
                       long timerOn, long timerOff, Repeat repeat, bool *days)
{
  newName(name);
  newId(id);
  newPinName(pinName);
  newTypeState(typeState);
  newState(status);
  newTimer(timer);
  newTimerOnState(timerOnState);
  newTImerOffState(timerOffState);
  newTimerOn(timerOn);
  newTimerOff(timerOff);
  newRepeat(repeat);
  newDays(days);
}

DeviceItem::DeviceItem(String name, int id, int pinName)
{
  newName(name);
  newId(id);
  newPinName(pinName);

  newTypeState(TypeStatus::OnOff);
  newState(Status::OFF);
  newTimer(false);
  newTimerOnState(false);
  newTImerOffState(false);
  newTimerOn(-1);
  newTimerOff(-1);
  newRepeat(Repeat::Once);
  bool days[] = {false, false, false, false, false, false, false};
  newDays(days);
}

void DeviceItem::newDays(bool *day)
{
  for (int i = 0; i < (sizeof(_days) / sizeof(bool)); i++)
  {
    _days[i] = day[i];
  }
}

void DeviceItem::newTypeState(TypeStatus type = TypeStatus::OnOff)
{
  _typeState = type;
  switch (_typeState)
  {
  case TypeStatus::OnOff:
    if (_state == Status::OPEN || _state == Status::ON)
      _state = Status::ON;
    else
      _state = Status::OFF;
    break;
  case TypeStatus::OpenClose:
    if (_state == Status::OPEN || _state == Status::ON)
      _state = Status::OPEN;
    else
      _state = Status::CLOSE;
    break;
  }
}

void DeviceItem::newState(Status state)
{
  switch (_typeState)
  {
  case TypeStatus::OnOff:
    if ((state == Status::CLOSE) || (state == Status::OFF))
      _state = Status::OFF;
    else
      _state = Status::ON;
    break;
  case TypeStatus::OpenClose:
    if ((state == Status::CLOSE) || (state == Status::OFF))
      _state = Status::CLOSE;
    else
      _state = Status::OPEN;
    break;
  default:
    _state = Status::OFF;
  }
  digitalWrite(_pinName, ((_state == OPEN) || (_state == ON)) ? HIGH: LOW);
  // Serial.printf("\nnewStatus = %d\n", ((_state == OPEN) || (_state == ON)) ? HIGH: LOW);
}

void DeviceItem::changeState()
{
  switch (_typeState)
  {
  case TypeStatus::OnOff:
    if (_state == Status::ON)
      _state = Status::OFF;
    else
      _state = Status::ON;
    break;
  case TypeStatus::OpenClose:
    if (_state == Status::OPEN)
      _state = Status::CLOSE;
    else
      _state = Status::OPEN;
    break;
  }
  digitalWrite(_pinName, ((_state == ON) || (_state == OPEN))? HIGH: LOW);
}

void DeviceItem::newDay(int index = 0, bool value = false){
  _days[index] = value;
  bool isEveryday = true;
    for (int i = 0; i < (sizeof(_days)/sizeof(bool)); i++)
      if (_days[i] == false) {
        isEveryday = false;
        break;
      }
    if (isEveryday) {
      newRepeat(Repeat::Everyday);
      bool days[] = {false, false, false, false, false, false, false};
      newDays(days);
    }  
}

void DeviceItem::turnOff(){
  newState(Status::OFF);
  digitalWrite(_pinName, LOW);
}

void DeviceItem::setTimerOn(int hour = 0, int minute = -1){
  if (minute < 0)
      _timerOn = -1;
    else if (hour >= 0 && minute >= 0) _timerOn = hour * 3600 + minute * 60;
}

void DeviceItem::setTimerOff(int hour = 0, int minute = -1){
  if (minute < 0)
      _timerOn = -1;
    else if (hour >= 0 && minute >= 0) _timerOff = hour * 3600 + minute * 60;
}

int DeviceItem::getHourOn(){
  if (_timerOn > -1)
      return _timerOn / 3600;
  else
    return 0;
}

int DeviceItem::getMinuteOn(){
  if (_timerOn > -1)
      return _timerOn / 60 - (_timerOn / 3600) * 60;
    else
      return -1;
}

int DeviceItem::getHourOff(){
  if (_timerOff > -1)
      return _timerOff / 3600;
  else
    return 0;
}

int DeviceItem::getMinuteOff(){
  if (_timerOff > -1)
      return _timerOff / 60 - (_timerOff / 3600) * 60;
    else
      return -1;
}

void DeviceItem::addObject(std::vector<DeviceItem> listItems[], String jsonString){
  DynamicJsonDocument json(1024);
  deserializeJson(json, jsonString);
  json.shrinkToFit();
  
  bool _days[7];
  for (int j = 0; j<7; j++){
    _days[j] = json["days"][j].as<bool>();
    // Serial.printf("\n(DeviceItem::addObject) _days[%d] : ",j);
    // Serial.println(_days[j]);
    }

  listItems->push_back(DeviceItem(
    json["name"].as<String>(),
    json["id"].as<int>(),
    json["pinName"].as<int>(),
    (json["status"].as<int>() == 1)? Status::ON:Status::OFF,
    json["timer"].as<bool>(),
    json["timerOnState"].as<bool>(),
    json["timerOffState"].as<bool>(),
    (json["typeState"].as<int>() == 1)? TypeStatus::OnOff:TypeStatus::OpenClose,
    json["timerOn"].as<int>(),
    json["timerOff"].as<int>(),
    (json["repeat"].as<int>() == 0) ? Repeat::Once :(json["repeat"].as<int>() == 1 ? Repeat::Everyday:Repeat::Option),
    _days
  ));
  // Serial.println("\n -- Device infor --");
  // Serial.printf("\nid = %d\ttimer = %d", listItems->at(listItems->size() -1).id(), listItems->at(listItems->size() -1).timer()? 1 : 0);
  // Serial.printf("\nOffState = %d\tOff = %d\n", listItems->at(listItems->size() -1).timerOffState()? 1:0, listItems->at(listItems->size() -1).timerOff());
  // Serial.printf("\nOnState = %d\tOn = %d\n", listItems->at(listItems->size() -1).timerOnState()? 1:0, listItems->at(listItems->size() -1).timerOn());
  // // Serial.printf("\n(DeviceItem::addObject) json size: %d", json.size());
  // // Serial.printf("\n(DeviceItem::addObject) list size: %d", listItems->size());
  json.clear();
  // Serial.printf("\n(DeviceItem::addObject) json size affter: %d", json.size());
  // Serial.println("\n(DeviceItem::addObject) Complete add device\n");
}

void DeviceItem::updateObject(std::vector<DeviceItem> listItems[], int index, String jsonString){
  // Serial.printf("(DeviceItem::update Object) list size: %d", (listItems)->size());
  DynamicJsonDocument json(1024);
  deserializeJson(json, jsonString);
  Serial.printf("\n(DeviceItem::update Object) Update device: ");
  Serial.printf(json["name"]);
  Serial.println("\n(DeviceItem::update Object) Device index: ");
  Serial.println(index);

  bool _days[7];
  for (int j = 0; j<7; j++)
  _days[j] = json["days"][j].as<bool>();
  // Serial.println("(DeviceItem::update Object) start update");

  listItems->at(index).newName(json["name"].as<String>());
  // Serial.println("update OK");
  listItems->at(index).newId(json["id"].as<int>());
  // Serial.println("update OK");
  listItems->at(index).newPinName(json["pinName"].as<int>());
  // Serial.println("update OK");
  listItems->at(index).newState((json["status"].as<int>() == 1)? Status::ON:Status::OFF);
  // Serial.println("update OK");
  listItems->at(index).newTimer(json["timer"].as<bool>());
  // Serial.println("update OK");
  listItems->at(index).newTimerOnState(json["timerOnState"].as<bool>());
  // Serial.println("update OK");
  listItems->at(index).newTImerOffState(json["timerOffState"].as<bool>());
  // Serial.println("update OK");
  listItems->at(index).newTypeState((json["typeState"].as<int>() == 1)? TypeStatus::OnOff:TypeStatus::OpenClose);
  // Serial.println("update OK");
  listItems->at(index).newTimerOn(json["timerOn"].as<int>());
  // Serial.println("update OK");
  listItems->at(index).newTimerOff(json["timerOff"].as<int>());
  // Serial.println("update OK");
  listItems->at(index).newRepeat((json["repeat"].as<int>() == 0) ? Repeat::Once :(json["repeat"].as<int>() == 1 ? Repeat::Everyday:Repeat::Option));
  // Serial.println("update OK");
  listItems->at(index).newDays(_days);
  // Serial.println("update OK");
  
  Serial.println(listItems->at(index).name());
  Serial.println("\n(DeviceItem::update Object) Complete update object");
}

String DeviceItem::buildJson(std::vector<DeviceItem> listItems[], String key = ""){
  // Serial.printf("\n(DeviceItem::buildJson) list Size: ");
  // Serial.println(listItems->size());
  DynamicJsonDocument doc(1024*listItems->size()); // size of doc equal size of listItems * 1 Mb.
  for (int i = 0; i < listItems->size(); i++){
    doc[key][i]["name"] = listItems->at(i).name();
    doc[key][i]["id"] = listItems->at(i).id();
    doc[key][i]["pinName"] = listItems->at(i).pinName();
    doc[key][i]["status"] = ((listItems->at(i).status() == Status::ON) || (listItems->at(i).status() == Status::OPEN))? 1 : 0;
    doc[key][i]["timer"] = listItems->at(i).timer();
    doc[key][i]["timerOnState"] = listItems->at(i).timerOnState();
    doc[key][i]["timerOffState"] = listItems->at(i).timerOffState();
    doc[key][i]["typeState"] = (listItems->at(i).typeState() == TypeStatus::OnOff)?0:1;
    doc[key][i]["timerOn"] = listItems->at(i).timerOn();
    doc[key][i]["timerOff"] = listItems->at(i).timerOff();
    doc[key][i]["repeat"] = (listItems->at(i).repeat() == Repeat::Everyday) ? 0 : (listItems->at(i).repeat() == Repeat::Once ? 1 :2);
    for (int j = 0; j <7; j++)
      doc[key][i]["days"][j] = listItems->at(i).day(j);
  }
  String jsonString;
  serializeJson(doc, jsonString);
  // Serial.println("\n(DeviceItem::build Json) Json String: \n" + jsonString);
  return jsonString;
}

String DeviceItem::toJson(){
  DynamicJsonDocument doc(1024); // size of doc is * 1 Mb.
    doc["name"] = name();
    doc["id"] = id();
    doc["pinName"] = pinName();
    doc["status"] = ((status() == Status::ON) || (status() == Status::OPEN))? 1 : 0;
    doc["timer"] = timer();
    doc["timerOnState"] = timerOnState();
    doc["timerOffState"] = timerOffState();
    doc["typeState"] = (typeState() == TypeStatus::OnOff)?0:1;
    doc["timerOn"] = timerOn();
    doc["timerOff"] = timerOff();
    doc["repeat"] = (repeat() == Repeat::Everyday) ? 0 : (repeat() == Repeat::Once ? 1 :2);
    for (int j = 0; j <7; j++)
      doc["days"][j] = day(j);
  
  String jsonString;
  serializeJson(doc, jsonString);
  // Serial.println("\n(DeviceItem::toJson) Json String: \n" + jsonString);
  return jsonString;
}

bool DeviceItem::isTimeToOn(){
  if (timer() && timerOnState())
    return checkTimer(timerOn(), true);
  return false;
}

bool DeviceItem::isTimeToOff(){
  if (timer() && timerOffState()) 
    return checkTimer(timerOff(), false);
  return false;
}

// Check timerOn and timerOff
// mode = true -> Check timerOn
// mode = false -> Check timerOff
bool DeviceItem::checkTimer(long time, bool mode){
  static WiFiUDP ntpUDP;
  static NTPClient timeClient(ntpUDP);
  // int time = mode == true? _timerOn: _timerOff;
  timeClient.begin();
  timeClient.setTimeOffset(+7*60*60);
  while(!timeClient.update()) 
    timeClient.forceUpdate();

  long timeNow = timeClient.getHours()*3600 + timeClient.getMinutes()*60 + timeClient.getSeconds();
  // Serial.printf("\n(DeviceItem::checkTimer) Time now: %d:%d:%d\n", timeClient.getHours(), timeClient.getMinutes(), timeClient.getSeconds());
  // Serial.printf("\n(DeviceItem::checkTimer) Time now in second: %d", timeNow);
  switch (repeat())
  {
  case Repeat::Once:
    if ((timeNow - time >= 0) && (timeNow - time <=  60)) {
      if (mode){
        newState(Status::ON);
        newTimerOnState(false);}
      else {
        newState(Status::OFF);
        newTImerOffState(false);
      }
      return true;
      }      
    break;
  case Repeat::Everyday:
    if ((timeNow - time >= 0) && (timeNow - time <=  60)) {
      if (mode) newState(Status::ON);
      else newState(Status::OFF);
      return true;}
    break;
  case Repeat::Option:
    if (day(timeClient.getDay()))
      if ((timeNow - time >= 0) && (timeNow - time <=  60)) {
        if (mode) newState(Status::ON);
        else newState(Status::OFF);
        return true;}
    break;
  }
  return false;
}