#include "systemErrors.h"

#include <stddef.h>

static systemErrors_t systemErrorBits = 0b00000000; // 0b prefix is not standard
static char systemErrorBitsString[sizeof(systemErrorBits) * 8 + 1] = "00000000";

int setSystemErrorBit(error_t err)
{
   int systemErrorBit = getSystemErrorBit(err);

   systemErrorBits |= 0x01 << err;

   return systemErrorBit;
}

int getSystemErrorBit(error_t err)
{
   return (systemErrorBits & (0x01 << err)) != 0;
}

systemErrors_t getSystemErrorBits(void)
{
   return systemErrorBits;
}

const char *getSystemErrorBitsString(void)
{
   for (size_t i = 0; i < sizeof(systemErrorBits) * 8; i++)
   {
      systemErrorBitsString[i] = '0';
      if (getSystemErrorBit(i))
      {
         systemErrorBitsString[i] = '1';
      }
   }
   return systemErrorBitsString;
}
