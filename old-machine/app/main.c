#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>
#include "fsm_functions/fsm.h"
#include "console_functions/keyboard.h"
#include "console_functions/display.h"
#include "console_functions/devConsole.h"

extern char * eventEnumToText[];
extern char * stateEnumToText[];

state_t state;
event_t event;
//some default values
int totalTeas = 0;
char* selectedTea = "Black Tea";
float teaPrice = 0.0;
float insertedMoney = 0.0;

// function prototypes
void S_Init_onEntry(void);
void S_Init_onExit(void);

void S_Idle_onEntry(void);
void S_Idle_onExit(void);

void S_SelectTea_onEntry(void);
void S_SelectTea_onExit(void);

void S_AskForMoney_onEntry(void);
void S_AskForMoney_onExit(void);

void S_MakeTea_onEntry(void);
void S_MakeTea_onExit(void);

void S_DoneTea_onEntry(void);
void S_DoneTea_onExit(void);

void S_Shutdown_onEntry(void);
void S_Shutdown_onExit(void);

//helper functions
event_t getUserChoice(void);
event_t getTeaSelection(void);
event_t getMoneyInput(void);
void delay_us(uint32_t d);

int main(void)
{
   //           state                              onEntry()                   onExit()
   FSM_AddState(S_START,          &(state_funcs_t){  NULL,                      NULL });
   FSM_AddState(S_INIT,           &(state_funcs_t){  S_Init_onEntry,            S_Init_onExit });
   FSM_AddState(S_IDLE,           &(state_funcs_t){  S_Idle_onEntry,            S_Idle_onExit });
   FSM_AddState(S_SELECT_TEA,     &(state_funcs_t){  S_SelectTea_onEntry,       S_SelectTea_onExit });
   FSM_AddState(S_ASK_FOR_MONEY,  &(state_funcs_t){  S_AskForMoney_onEntry,     S_AskForMoney_onExit });
   FSM_AddState(S_MAKE_TEA,       &(state_funcs_t){  S_MakeTea_onEntry,         S_MakeTea_onExit });
   FSM_AddState(S_DONE_TEA,       &(state_funcs_t){  S_DoneTea_onEntry,         S_DoneTea_onExit });
   FSM_AddState(S_SHUTDOWN,       &(state_funcs_t){  S_Shutdown_onEntry,        S_Shutdown_onExit });

   //                                 from               event                    to
   FSM_AddTransition(&(transition_t){ S_START,           E_START,                 S_INIT });
   FSM_AddTransition(&(transition_t){ S_INIT,            E_CONTINUE,              S_IDLE });
   FSM_AddTransition(&(transition_t){ S_IDLE,            E_SELECT_TEA,            S_SELECT_TEA });
   FSM_AddTransition(&(transition_t){ S_IDLE,            E_SHUTDOWN,              S_SHUTDOWN });
   FSM_AddTransition(&(transition_t){ S_SELECT_TEA,      E_TEA_SELECTED,          S_ASK_FOR_MONEY });
   FSM_AddTransition(&(transition_t){ S_ASK_FOR_MONEY,   E_ENOUGH_MONEY,          S_MAKE_TEA });
   FSM_AddTransition(&(transition_t){ S_ASK_FOR_MONEY,   E_NOT_ENOUGH_MONEY,      S_ASK_FOR_MONEY });
   FSM_AddTransition(&(transition_t){ S_MAKE_TEA,        E_STILL_MAKING,          S_MAKE_TEA });
   FSM_AddTransition(&(transition_t){ S_MAKE_TEA,        E_POURED_AND_WAITED,     S_DONE_TEA });
   FSM_AddTransition(&(transition_t){ S_DONE_TEA,        E_RESET,                 S_IDLE });

   // unexpected events
   FSM_FlushEnexpectedEvents(true);

   // set the fsm in the S_START state
   // and add a E_START event into the event buffer
   state = S_START;
   FSM_AddEvent(E_START);    // machine is on

   while(event != E_SHUTDOWN)
   {
      if(!FSM_NoEvents())
      {
         // get the event and handle it
         event = FSM_GetEvent();
         state = FSM_EventHandler(state, event);
      }
   }
}


void S_Init_onEntry(void)
{
   // Initialize the console
   DSPinitialise();
   DSPshowDisplay();
   KYBinitialise();

   DCSdebugSystemInfo("Current state: %s\n", stateEnumToText[state]);
   DCSdebugSystemInfo("Current event: %s\n", eventEnumToText[event]);
   DSPshow(2, "Tea Machine Starting");
   delay_us(20000);
   DSPshow(3, "System Ready");
   DCSdebugSystemInfo("Transition to %s\n", eventEnumToText[E_CONTINUE]);

   FSM_AddEvent(E_CONTINUE);
}

void S_Init_onExit(void)
{
   DCSdebugSystemInfo("Exit state:  %s\n", stateEnumToText[state]);
}

void S_Idle_onEntry(void)
{
   DCSdebugSystemInfo("Entry state:  %s\n", stateEnumToText[state]);
   DSPshow(4, "Waiting for input");
   event_t choice = getUserChoice();
   FSM_AddEvent(choice);
}

void S_Idle_onExit(void)
{
   DCSdebugSystemInfo("Exit state:  %s\n", stateEnumToText[state]);
   DCSdebugSystemInfo("Exit event:  %s\n", eventEnumToText[event]);
}

void S_SelectTea_onEntry(void)
{
   DCSdebugSystemInfo("Entry state:  %s\n", stateEnumToText[state]);
   DSPshow(5, "Select your tea");
   event_t teaChoice = getTeaSelection();
   FSM_AddEvent(teaChoice);
}

void S_SelectTea_onExit(void)
{
   DCSdebugSystemInfo("Exit state:  %s\n", stateEnumToText[state]);
   DCSdebugSystemInfo("Selected tea: %s ($%.2f)\n", selectedTea, teaPrice);
}

void S_AskForMoney_onEntry(void)
{
   DCSdebugSystemInfo("Entry state:  %s\n", stateEnumToText[state]);
   DSPshow(6, "Price: $%.2f", teaPrice);
   DSPshow(7, "Insert money");
   event_t moneyEvent = getMoneyInput();
   FSM_AddEvent(moneyEvent);
}

void S_AskForMoney_onExit(void)
{
   DCSdebugSystemInfo("Exit state:  %s\n", stateEnumToText[state]);
   DCSdebugSystemInfo("Money status: Inserted $%.2f, Required $%.2f\n", insertedMoney, teaPrice);
}

void S_MakeTea_onEntry(void)
{
   DSPclear();
   DCSdebugSystemInfo("Entry state:  %s\n", stateEnumToText[state]);
   DSPshow(8, "Making %s", selectedTea);
   DSPshowDisplay();
   delay_us(30000);

   DSPshow(9, "Pouring tea...");
   DSPshowDisplay();
   delay_us(20000);

   DSPshow(10, "Please wait...");
   DSPshowDisplay();
   delay_us(20000);

   FSM_AddEvent(E_POURED_AND_WAITED);
}

void S_MakeTea_onExit(void)
{
   DCSdebugSystemInfo("Exit state:  %s\n", stateEnumToText[state]);
}

void S_DoneTea_onEntry(void)
{
   DCSdebugSystemInfo("Entry state:  %s\n", stateEnumToText[state]);
   DSPshow(10, "Tea ready!");
   DSPshow(11, "Enjoy your %s", selectedTea);
   totalTeas++;

   // calculate change
   float change = insertedMoney - teaPrice;
   if (change > 0.01) {
      DSPshow(12, "Change: $%.2f", change);
   }

   delay_us(25000);

   // reset for next transaction
   insertedMoney = 0.0;
   teaPrice = 0.0;

   FSM_AddEvent(E_RESET);
}

void S_DoneTea_onExit(void)
{
   DCSdebugSystemInfo("Exit state:  %s\n", stateEnumToText[state]);
   DSPclear();
}

void S_Shutdown_onEntry(void)
{
   DCSdebugSystemInfo("Entry state:  %s\n", stateEnumToText[state]);
   DSPclear();
   DSPshow(9, "Shutting down");
   DSPshow(10, "Total teas sold: %d", totalTeas);
   delay_us(15000);
}

void S_Shutdown_onExit(void)
{
   DCSdebugSystemInfo("Exit state:  %s\n", stateEnumToText[state]);
   exit(0);
}


// Helper functions

event_t getUserChoice(void)
{
   char kb;
   event_t command = E_SELECT_TEA;
   kb = DCSsimulationSystemInputChar("Select: 1=Make Tea, 2=Shutdown : ", "12");
   switch (kb)
   {
      case '1':
         command = E_SELECT_TEA;
         break;
      case '2':
         command = E_SHUTDOWN;
         break;
      default:
         command = E_NO;
   }
   return command;
}

event_t getTeaSelection(void)
{
   char kb;
   event_t command = E_TEA_SELECTED;
   kb = DCSsimulationSystemInputChar("Select tea: 1=Green, 2=Black, 3=Herbal, 4=Oolong : ", "1234");
   switch (kb)
   {
      case '1':
         selectedTea = "Green Tea";
         teaPrice = 2.00;
         command = E_TEA_SELECTED;
         break;
      case '2':
         selectedTea = "Black Tea";
         teaPrice = 2.00;
         command = E_TEA_SELECTED;
         break;
      case '3':
         selectedTea = "Herbal Tea";
         teaPrice = 2.50;
         command = E_TEA_SELECTED;
         break;
      case '4':
         selectedTea = "Oolong Tea";
         teaPrice = 2.75;
         command = E_TEA_SELECTED;
         break;
      default:
         command = E_NO;
   }
   return command;
}

event_t getMoneyInput(void)
{
   char kb;
   event_t command = E_NOT_ENOUGH_MONEY;

   DSPshow(13, "Current: $%.2f", insertedMoney);
   kb = DCSsimulationSystemInputChar("Insert: 1=$1, 2=$2, 3=$5 : ", "123");

   switch (kb)
   {
      case '1':
         insertedMoney += 1.00;
         break;
      case '2':
         insertedMoney += 2.00;
         break;
      case '3':
         insertedMoney += 5.00;
         break;
      default:
         break;
   }

   DCSdebugSystemInfo("Inserted: $%.2f, Required: $%.2f\n", insertedMoney, teaPrice);

   if (insertedMoney >= teaPrice) {
      command = E_ENOUGH_MONEY;
   } else {
      command = E_NOT_ENOUGH_MONEY;
   }

   return command;
}

// simulate delay in microseconds
void delay_us(uint32_t d)
{
   DCSdebugSystemInfo("Delay waiting for %d micro-seconds", d);
   sleep(d/10000);
}
