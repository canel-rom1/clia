/*
Project: Clia - Comand line interface for arduino 
File: clia.h
Version: 0.1
Create by: Rom1 <rom1@canel.ch> - CANEL - https://www.canel.ch
Date: 04/04/18
License: GNU GENERAL PUBLIC LICENSE v3
Language: Arduino (C/C++)
Description: A library for arduino to make a small command line interface
*/

#ifndef __clia_h__
#define __clia_h__

#include <Arduino.h>
#include <stdlib.h>
#include <string.h>

#ifndef CMDLINEBUFFER
  #define CMDLINEBUFFER 50
#endif//CMDLINEBUFFER

/* ASCII values in decimal */
#define BS     8
#define LF    10
#define CR    13
#define ESC   27
#define SPACE 32

typedef struct t_fcmd
{
  const char *name;
  void (*funct)(char*);
}t_fcmd;

typedef struct t_cmd
{
  char *name = NULL;
  char *arg = NULL;
}t_cmd;

class Clia
{
  private:
    Stream &serial;

    t_cmd formatCmd(char *cmd_in);
    void clearBuf(char *buf);
    int selectCmd(t_cmd cmd);

  public:
    const char *prompt;

    Clia(Stream &serial, const char *prompt): serial(serial), prompt(prompt){};
    int add(t_fcmd cmd);
    void handler(void);
};
#endif//__clia_h__


// vim: ft=arduino tw=100 et ts=2 sw=2
