/*
Project: Clia - Comand line interface for arduino 
File: clia-basic.ino
Version: 0.1
Create by: Rom1 <rom1@canel.ch> - CANEL - https://www.canel.ch
Date: 04/04/18
License: GNU GENERAL PUBLIC LICENSE v3
Language: Arduino (C/C++)
Description: A basic example of using the library
*/

#include <clia.h>

Clia term(Serial, "a> ");

void cmd1(char *val);
void cmd2(char *val);

void setup(void)
{
  Serial.begin(115200);

  t_fcmd cmd;
  cmd = t_fcmd{"cmd1", &cmd1};
  term.add(cmd);
  cmd = t_fcmd{"cmd2", &cmd2};
  term.add(cmd);
}

void loop(void)
{
  term.handler();
}

void cmd1(char *val)
{
  Serial.println("Hello");
}

void cmd2(char *val)
{
  Serial.println(val);
}


// vim: ft=arduino tw=100 et ts=2 sw=2
