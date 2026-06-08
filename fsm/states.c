char * stateEnumToText[] =
{
   "S_NO",              ///< Used for initialisation if state is not yet known
   "S_START",           ///< Initial state
   "S_INIT",            ///< Initialize tea machine
   "S_IDLE",            ///< Waiting for user
   "S_SELECT_TEA",      ///< Selecting tea type
   "S_ASK_FOR_MONEY",   ///< Asking for money payment
   "S_MAKE_TEA",        ///< Making tea
   "S_DONE_TEA",        ///< Tea completed
   "S_SHUTDOWN",        ///< Shutdown system
};
