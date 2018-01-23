/*--------------------------------------------------------------------
This file is part of the Arduino DEBUGGING library.

The Arduino DEBUGGING library is free software: you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation, either version 3 of the
License, or (at your option) any later version.

The Arduino DEBUGGING library is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with The Arduino DEBUGGING library.  If not, see
<http://www.gnu.org/licenses/>.
--------------------------------------------------------------------*/

#ifndef Debugger_h
#define Debugger_h

#include <stdint.h>
#include "Arduino.h"

////////////////////////////////////////////////////////////////////////////////
// Definitions

////////////////////////////////////////////////////////////////////////////////
// Debugger

class Debugger
{
  public:
    Debugger();

    void enable(int baudRate);
    void disable();
    void disableSerial();

    // Main Methods invoked by following methods
    void print(String praefix, String data, String postfix);
    void println(String praefix, String data, String postfix);

    // Inline
    void print(String data);
    void print(int data);
    void print(unsigned int data);
    void print(long data);
    void print(unsigned long data);
    void print(uint8_t data);

    void print(String praefix, String data);
    void print(String praefix, int data);
    void print(String praefix, unsigned int data);
    void print(String praefix, long data);
    void print(String praefix, unsigned long data);
    void print(String praefix, uint8_t data);

    void print(String praefix, int data, String postfix);
    void print(String praefix, unsigned int data, String postfix);
    void print(String praefix, long data, String postfix);
    void print(String praefix, unsigned long data, String postfix);
    void print(String praefix, uint8_t data, String postfix);

    // New Line
    void println(String data);
    void println(int data);
    void println(unsigned int data);
    void println(long data);
    void println(unsigned long data);
    void println(uint8_t data);

    void println(String praefix, String data);
    void println(String praefix, int data);
    void println(String praefix, unsigned int data);
    void println(String praefix, long data);
    void println(String praefix, unsigned long data);
    void println(String praefix, uint8_t data);

    void println(String praefix, int data, String postfix);
    void println(String praefix, unsigned int data, String postfix);
    void println(String praefix, long data, String postfix);
    void println(String praefix, unsigned long data, String postfix);
    void println(String praefix, uint8_t data, String postfix);


  private:
    int _debugging = true;
    int _baudRate;
};

#endif
