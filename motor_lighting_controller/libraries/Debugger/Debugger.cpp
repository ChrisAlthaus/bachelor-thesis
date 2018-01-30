#include "Arduino.h"
#include "Debugger.h"

////////////////////////////////////////////////////////////////////////////////

Debugger::Debugger() {

}

////////////////////////////////////////////////////////////////////////////////

void Debugger::enable(int baudRate) {
  _debugging = true;
  _baudRate = baudRate;

  Serial.end(); // opt
  Serial.begin(_baudRate);
}

void Debugger::disable() {
  _debugging = false;
}

void Debugger::disableSerial() {
  _debugging = false;

  Serial.end();
}

////////////////////////////////////////////////////////////////////////////////

void Debugger::print(String data) {
  String s = String(data);
  print("", s, "");
}
void Debugger::print(int data) {
  String s = String(data);
  print("", s, "");
}
void Debugger::print(long data) {
  String s = String(data);
  print("", s, "");
}
void Debugger::print(unsigned int data) {
  String s = String(data);
  print("", s, "");
}
void Debugger::print(unsigned long data) {
  String s = String(data);
  print("", s, "");
}
void Debugger::print(uint8_t data) {
  String s = String(data);
  print("", s, "");
}

void Debugger::print(String praefix, String data) {
  String s = String(data);
  print(praefix, s, "");
}
void Debugger::print(String praefix, int data) {
  String s = String(data);
  print(praefix, s, "");
}
void Debugger::print(String praefix, long data) {
  String s = String(data);
  print(praefix, s, "");
}
void Debugger::print(String praefix, unsigned int data) {
  String s = String(data);
  print(praefix, s, "");
}
void Debugger::print(String praefix, unsigned long data) {
  String s = String(data);
  print(praefix, s, "");
}
void Debugger::print(String praefix, uint8_t data) {
  String s = String(data);
  print(praefix, s, "");
}

void Debugger::print(String praefix, int data, String postfix) {
  String s = String(data);
  print(praefix, s, postfix);
}
void Debugger::print(String praefix, long data, String postfix) {
  String s = String(data);
  print(praefix, s, postfix);
}
void Debugger::print(String praefix, unsigned int data, String postfix) {
  String s = String(data);
  print(praefix, s, postfix);
}
void Debugger::print(String praefix, unsigned long data, String postfix) {
  String s = String(data);
  print(praefix, s, postfix);
}
void Debugger::print(String praefix, uint8_t data, String postfix) {
  String s = String(data);
  print(praefix, s, postfix);
}

////////////////////////////////////////////////////////////////////////////////

void Debugger::println(String data) {
  String s = String(data);
  println("", data, "");
}
void Debugger::println(int data) {
  String s = String(data);
  println("", s, "");
}
void Debugger::println(long data) {
  String s = String(data);
  println("", s, "");
}
void Debugger::println(unsigned int data) {
  String s = String(data);
  println("", s, "");
}
void Debugger::println(unsigned long data) {
  String s = String(data);
  println("", s, "");
}
void Debugger::println(uint8_t data) {
  String s = String(data);
  println("", s, "");
}

void Debugger::println(String praefix, String data) {
  String s = String(data);
  println(praefix, s, "");
}
void Debugger::println(String praefix, int data) {
  String s = String(data);
  println(praefix, s, "");
}
void Debugger::println(String praefix, long data) {
  String s = String(data);
  println(praefix, s, "");
}
void Debugger::println(String praefix, unsigned int data) {
  String s = String(data);
  println(praefix, s, "");
}
void Debugger::println(String praefix, unsigned long data) {
  String s = String(data);
  println(praefix, s, "");
}
void Debugger::println(String praefix, uint8_t data) {
  String s = String(data);
  println(praefix, s, "");
}

void Debugger::println(String praefix, int data, String postfix) {
  String s = String(data);
  println(praefix, s, postfix);
}
void Debugger::println(String praefix, long data, String postfix) {
  String s = String(data);
  println(praefix, s, postfix);
}
void Debugger::println(String praefix, unsigned int data, String postfix) {
  String s = String(data);
  println(praefix, s, postfix);
}
void Debugger::println(String praefix, unsigned long data, String postfix) {
  String s = String(data);
  println(praefix, s, postfix);
}
void Debugger::println(String praefix, uint8_t data, String postfix) {
  String s = String(data);
  println(praefix, s, postfix);
}

////////////////////////////////////////////////////////////////////////////////

void Debugger::print(String praefix, String data, String postfix) {
  if (_debugging) {
    Serial.print("\t"); // ?!
    Serial.print(praefix);
    Serial.print(data);
    Serial.print(postfix);
  }
}

void Debugger::println(String praefix, String data, String postfix) {
  if (_debugging) {
    Serial.print("\n");
    Serial.print(praefix);
    Serial.print(data);
    Serial.print(postfix);
  }
}
