/*******************************************************************
    Copyright (C) 2009 FreakLabs
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions
    are met:

    1. Redistributions of source code must retain the above copyright
       notice, this list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright
       notice, this list of conditions and the following disclaimer in the
       documentation and/or other materials provided with the distribution.
    3. Neither the name of the the copyright holder nor the names of its contributors
       may be used to endorse or promote products derived from this software
       without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE THE COPYRIGHT HOLDERS AND CONTRIBUTORS ``AS IS'' AND
    ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
    ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
    FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
    DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
    OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
    HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
    LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
    OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
    SUCH DAMAGE.

    Originally written by Christopher Wang aka Akiba.
    Please post support questions to the FreakLabs forum.

*******************************************************************/
/*!
    \file ws.c

    This implements a simple command line interface for the Arduino so that
    its possible to execute individual functions within the sketch.
*/
/**************************************************************************/
//#include <avr/pgmspace.h>

#if defined(__AVR__)
#include <avr/pgmspace.h>
#elif defined(ESP8266)
#include <pgmspace.h>
#endif

#include <Arduino.h>
#include "wsWebSocket.h"

// command line message buffer and pointer
static uint8_t wsmsg[MAX_MSG_SIZE];
static uint8_t *wsmsg_ptr;
static ws_t *ws_tbl_list, *ws_tbl;
const char ws_prompt[] PROGMEM = ">";


/**************************************************************************/
/*!
    Generate the main command prompt
*/
/**************************************************************************/
void ws_display()
{

}

/**************************************************************************/
/*!
    Parse the command line. This function tokenizes the command input, then
    searches for the command table entry associated with the commmand. Once found,
    it will jump to the corresponding function.
*/
/**************************************************************************/
void ws_parse(char *ws)
{
  uint8_t argc, i = 0;
  char *argv[MAX_ARG_SIZE];
  char wsbuf[MAX_BUF_SIZE];
  ws_t *ws_entry;

  fflush(stdout);

  // parse the command line statement and break it up into space-delimited
  // strings. the array of strings will be saved in the argv array.
  argv[i] = strtok(ws, " ");
  do
  {
    argv[++i] = strtok(NULL, " ");
  } while ((i < MAX_ARG_SIZE) && (argv[i] != NULL));

  // save off the number of arguments for the particular command.
  argc = i;

  // parse the command table for valid command. used argv[0] which is the
  // actual command name typed in at the prompt
  for (ws_entry = ws_tbl; ws_entry != NULL; ws_entry = ws_entry->next)
  {
    if (!strcmp(argv[0], ws_entry->ws))
    {
      ws_entry->func(argc, argv);
      ws_display();
      return;
    }
  }
  ws_display();
}

/**************************************************************************/
/*!
    This function processes the individual characters typed into the command
    prompt. It saves them off into the message buffer unless its a "backspace"
    or "enter" key.
*/
/**************************************************************************/
void ws_handler(char c)
{
  switch (c)
  {
    //  case '.':
    case '\r':
      // terminate the msg and reset the msg ptr. then send
      // it to the handler for processing.
      *wsmsg_ptr = '\0';
      //     stream->print("\r\n");
      ws_parse((char *)wsmsg);
      wsmsg_ptr = wsmsg;
      break;

    case '\n':
      // ignore newline characters. they usually come in pairs
      // with the \r characters we use for newline detection.
      break;

    case '\b':
      // backspace
      //     stream->print(c);
      if (wsmsg_ptr > wsmsg)
      {
        wsmsg_ptr--;
      }
      break;

    default:

      if (c >= 65 && c <= 90)
      {
        c = c ^ 0x20;
      }
      // normal character entered. add it to the buffer
      Serial.print(c);
      *wsmsg_ptr++ = c;
      break;
  }
}

/**************************************************************************/
/*!
    This function should be set inside the main loop. It needs to be called
    constantly to check if there is any available input at the command prompt.
*/
/**************************************************************************/
void wsPoll(uint8_t * payload, size_t length)
{
      Serial.println(length);
      payload[length]='\0';
      ws_parse((char *)payload);
//
//  Serial.println(length);
//
//  for (int i = 0; i < length; i++) {
//    Serial.print((char)payload[i]);
//    ws_handler((char)payload[i]);
//  }
//  ws_handler('\r');
//  Serial.println();
}

/**************************************************************************/
/*!
    Initialize the command line interface. This sets the terminal speed and
    and initializes things.
*/
/**************************************************************************/
void wsInit()
{
  // init the command table
  ws_tbl_list = NULL;

}

/**************************************************************************/
/*!
    Add a command to the command table. The commands should be added in
    at the setup() portion of the sketch.
*/
/**************************************************************************/
void wsAdd(const char *name, void (*func)(int argc, char **argv))
{
  // alloc memory for command struct
  ws_tbl = (ws_t *)malloc(sizeof(ws_t));

  // alloc memory for command name
  char *ws_name = (char *)malloc(strlen(name) + 1);

  // copy command name
  strcpy(ws_name, name);

  // terminate the command name
  ws_name[strlen(name)] = '\0';

  // fill out structure
  ws_tbl->ws = ws_name;
  ws_tbl->func = func;
  ws_tbl->next = ws_tbl_list;
  ws_tbl_list = ws_tbl;
}
