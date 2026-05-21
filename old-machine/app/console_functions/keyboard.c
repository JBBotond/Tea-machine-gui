#include "keyboard.h"
#include "devConsole.h"
#include "display.h"

#include <ctype.h>
#include <stdio.h>

//--------------------------------------------------------------------- Keyboard

void KYBinitialise(void)
{
   DCSdebugSystemInfo("Keyboard: initialised");
}

void KYBclear(void)
{
   while ((getchar()) != '\n')
   {
      // Remove all remaining buffered input chars
   }
}

char KYBgetchar(void)
{
   char c = getchar();
   KYBclear();
   return c;
}

int KYBgetint(int ifWrongValue)
{
   int input = 0;
   // scanf reads input buffer until space, tab or enter.
   int nOk = scanf(" %d", &input);

   KYBclear();
   // Check if input is an int (nOk == 1), if not return ifWrongValue
   if (nOk != 1)
   {
      input = ifWrongValue;
   }
   return input;
}

double KYBgetdouble(double ifWrongValue)
{
   double input = 0.0;
   // scanf reads input buffer until space, tab or enter.
   int nOk = scanf(" %lf", &input);

   KYBclear();
   // Check if input is an double (nOk == 1), if not return ifWrongValue
   if (nOk != 1)
   {
      input = ifWrongValue;
   }
   return input;
}
