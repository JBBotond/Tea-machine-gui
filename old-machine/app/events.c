// global variables
char * eventEnumToText[] =
{
   "E_NO",                  ///< No event
   "E_START",               ///< Machine powered on
   "E_CONTINUE",            ///< Continue to next state
   "E_SELECT_TEA",          ///< User wants to select tea
   "E_TEA_SELECTED",        ///< Tea has been selected
   "E_ENOUGH_MONEY",        ///< User inserted enough money
   "E_NOT_ENOUGH_MONEY",    ///< User didn't insert enough money
   "E_POURED_AND_WAITED",   ///< Tea poured and waiting time complete
   "E_STILL_MAKING",        ///< Still in the making process
   "E_RESET",               ///< Reset to idle
   "E_SHUTDOWN",            ///< User wants to shutdown
};
