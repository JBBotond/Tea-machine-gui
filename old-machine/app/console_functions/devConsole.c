#include "devConsole.h"
#include "display.h"
#include "keyboard.h"

#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void DCSinitialise(void)
{
   DSPinitialise();
   KYBinitialise();
   DCSdebugSystemInfo("Development Console: initialised");
}

int DCSsimulationSystemInputYN(const char questionText[])
{
   char input = '\0';
   int again = 0;

   do
   {
      printf("\n-- SIMULATION  %s [y/n]? ", questionText);
      int nOK = scanf(" %c", &input);
      input = toupper(input);

      again = (nOK != 1 || (strchr("YN", input) == NULL));
      if (again)
      {
         printf("** AGAIN");
      }
      KYBclear();
   } while (again);

   return (input == 'Y');
}

char DCSsimulationSystemInputChar(const char text[], const char chrs[])
{
   char input = '\0';
   int again = 0;

   do
   {
      int nOK = DCSsimulationSystemInput(text, " %c", &input);
      again = (nOK != 1 || (strchr(chrs, input) == NULL));
      if (again)
      {
         printf("** AGAIN");
      }
      KYBclear();
   } while (again);

   return input;
}

int DCSsimulationSystemInputInteger(const char text[], int min, int max)
{
   int input = 0;
   int again = 0;

   do
   {
      int nOK = DCSsimulationSystemInput(text, "%d", &input);
      again = (nOK != 1 || (input < min || input > max));
      if (again)
      {
         printf("** AGAIN  %d <= input <= %d ", min, max);
      }
      KYBclear();
   } while (again);

   return input;
}

int DCSsimulationSystemInput(const char text[], const char fmt[], ...)
{
   int nArgsOK = 0;
   va_list arg;

   printf("\n-- SIMULATION  %s ", text);
   va_start(arg, fmt);
   nArgsOK = vfscanf(stdin, fmt, arg);
   va_end(arg);

   return nArgsOK;
}

void DCSdebugSystemInfo(const char fmt[], ...)
{
   va_list arg;

   printf("\n-- DEBUG  ");
   va_start(arg, fmt);
   vfprintf(stdout, fmt, arg);
   va_end(arg);
}

void DCSsimulationSystemInfo(const char fmt[], ...)
{
   va_list arg;

   printf("\n-- SIMULATION  ");
   va_start(arg, fmt);
   vfprintf(stdout, fmt, arg);
   va_end(arg);
}

void DCSshowSystemError(const char fmt[], ...)
{
   va_list arg;

   printf("\n-- SYSTEM ERROR  ");
   va_start(arg, fmt);
   vfprintf(stdout, fmt, arg);
   va_end(arg);
}
