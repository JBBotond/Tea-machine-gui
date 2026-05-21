#include "display.h"
#include "appInfo.h"
#include "devConsole.h"
#include "systemErrors.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//---------------------------------------------------------------------- DiSPlay

#define DSP_HEIGHT 15 ///< The number of available display rows
#define DSP_WIDTH 70  ///< The number of available display columns

static char display[DSP_HEIGHT][DSP_WIDTH + 1] = {{0}};
static char topDisplay[DSP_WIDTH] = {0};

void DSPinitialise(void)
{
   for (int i = 0; i < DSP_WIDTH; i++)
   {
      topDisplay[i] = '=';
   }
   strncpy(display[0], topDisplay, DSP_WIDTH);
   strncpy(display[DSP_HEIGHT - 1], topDisplay, DSP_WIDTH);
   for (int i = 1; i < DSP_HEIGHT - 1; i++)
   {
      display[i][0] = '|';
   }
   strncpy(&display[1][1], " " APP " v" VERSION, DSP_WIDTH - 5);

   DSPshowDisplay();
   DCSdebugSystemInfo("Display %dx%d: initialised", DSP_WIDTH, DSP_HEIGHT);
}

void DSPclear(void)
{
   if (!system(NULL))
   {
      printf("\nERROR command processor is not available\n\n");
      exit(EXIT_FAILURE); //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   }

#ifdef __APPLE__
   int error = system("clear"); // Execute MAC OSX clear command
#endif
#ifdef _WIN32
   int error = system("cls"); // Execute WIN32 cls command
#endif
#ifdef __linux__
   int error = system("clear"); // Execute Linux clear command
#endif

   if (error != 0)
   {
      printf("\nERROR command for starting a terminal fails\n\n");
   }
}

void DSPclearLine(int row)
{
   strcpy(display[row], "| ");
}

void DSPshowSystemErrorBits(void)
{
   printf("|  System error bits: %s\n", getSystemErrorBitsString());
   printf("%s\n", display[0]);
}

void DSPshowDisplay(void)
{
   DSPclear();
   for (int row = 0; row < DSP_HEIGHT; row++)
   {
      printf("%s\n", display[row]);
   }
   DSPshowSystemErrorBits();
   puts("\nDevelopment Console:");
}

void DSPshow(int row, const char fmt[], ...)
{
   va_list arg;

#ifndef NOWAIT
   DCSdebugSystemInfo("** Press <Enter>, for update display **");
   getchar();
#endif

   DSPclearLine(row);

   va_start(arg, fmt);
   vsnprintf(&display[row][2], DSP_WIDTH - 3, fmt, arg); 
   va_end(arg);

   DSPshowDisplay();
}

void DSPshowDelete(int row, const char fmt[], ...)
{
   va_list arg;
#ifndef NOWAIT
   DCSdebugSystemInfo("** Press <Enter>, for update display **");
   getchar();
#endif
   for (int r = row; r < DSP_HEIGHT - 1; r++)
   {
      DSPclearLine(r);
   }
   va_start(arg, fmt);
   vsnprintf(&display[row][2], DSP_WIDTH - 3, fmt, arg);
   va_end(arg);

   DSPshowDisplay();
}
