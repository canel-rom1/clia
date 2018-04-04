/*
Project: Clia - Comand line interface for arduino 
File: clia.cpp
Version: 0.1
Create by: Rom1 <rom1@canel.ch> - CANEL - https://www.canel.ch
Date: 04/04/18
License: GNU GENERAL PUBLIC LICENSE v3
Language: Arduino (C/C++)
Description: A library for arduino to make a small command line interface
*/

#include "clia.h"

/* Variables Globale */
char cmdline[CMDLINEBUFFER + 1];
int cmdline_nbchar = 0;
t_fcmd *cmd_array = NULL;
int cmd_array_cnt = 0;
bool new_prompt = true;
int retcmd;

/* Functions */
void Clia::clearBuf(char *buf)
{
  for(unsigned int i=0 ; i < strlen(buf) ; i++)
    buf[i] = ' ';
}

void Clia::handler(void)
{
  char c;

  if(new_prompt)
  {
    serial.print(prompt);
    new_prompt = false;
  }

  if(serial.available())
  {
    c = serial.read();

    switch(c)
    {
      case BS:
        if(cmdline_nbchar > 0)
        {
          cmdline_nbchar--;
          serial.write(BS);
          serial.write(SPACE);
          serial.write(BS);
        }
        break;
      case CR:
        cmdline[cmdline_nbchar++] = '\0';
        serial.println();
        new_prompt = true;
        retcmd = selectCmd(formatCmd(cmdline));
        if(retcmd != 0)
          serial.println("Error: Command not found");
        clearBuf(cmdline);
        cmdline_nbchar=0;
        break;
      case ESC:
        for(; cmdline_nbchar > 0 ; cmdline_nbchar--)
        {
          serial.write(BS);
          serial.write(SPACE);
          serial.write(BS);
        }
        break;
      default:
        if(32 <= c and c <= 126)
        {
          cmdline[cmdline_nbchar++] = c;
          serial.write(c);
        }
    }
  }
}

int Clia::selectCmd(t_cmd cmd)
{
  for(int i=0 ; i < cmd_array_cnt+1 ; i++)
  {
    if(!strcmp(cmd_array[i].name, cmd.name))
    {
      ((void(*)(char*))cmd_array[i].funct)(cmd.arg);
      return 0;
    }
  }
  return 1;
}

t_cmd Clia::formatCmd(char *cmd_raw)
{
  t_cmd cmd;
  int i = 0;
  int nb_char_name;
  int nb_char_arg;
  int space = 0;

  while(1)
  {
    if(cmd_raw[i] == ' ')
    {
      nb_char_name = i;
      space = 1;
    }
    if(cmd_raw[i] == '\0')
    {
      if(space == 0)
        nb_char_name = i;
      else
        nb_char_arg = i - nb_char_name - 1;
      break;
    }
    i++;
  }

  cmd.name = (char*)realloc(cmd.name, sizeof(char) * (nb_char_name + 1));

  for(int i=0 ; i<nb_char_name ; i++)
    cmd.name[i] = cmd_raw[i];
  cmd.name[nb_char_name] = '\0';

  if(nb_char_arg > 0)
  {
    cmd.arg = (char*)realloc(cmd.arg, sizeof(char) * (nb_char_arg + 1));

    for(int i=0 ; i<nb_char_arg ; i++)
      cmd.arg[i] = cmd_raw[nb_char_name + 1 + i];
    cmd.arg[nb_char_arg] = '\0';
  }

  return cmd;
}

int Clia::add(t_fcmd cmd)
{
  cmd_array = (t_fcmd*)realloc(cmd_array, sizeof(t_fcmd)*(cmd_array_cnt+1));
  if(cmd_array == NULL)
    return -1;

  cmd_array[cmd_array_cnt++] = cmd;

  return cmd_array_cnt;
}


// vim: ft=arduino tw=100 et ts=2 sw=2
