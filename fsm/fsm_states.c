#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include "fsm_functions/fsm.h"
#include "events.h"
#include "states.h"

static void DSPinitialise(void){}
static void DSPshowDisplay(void){}
static void DSPclear(void){}
static void DSPshow(int row, const char *fmt, ...){}
static void KYBinitialise(void){}
static void DCSdebugSystemInfo(const char *fmt, ...){}
extern void Bridge_showMessage(const char* msg);

extern int   Bridge_waitForGuiInput(void);
extern void  Bridge_onStateChanged(const char* stateName);

extern char * eventEnumToText[];
extern char * stateEnumToText[];

state_t state;
event_t fsm_event;
int totalTeas = 0;
char* selectedTea = "Black Tea";
float teaPrice = 0.0;
float insertedMoney = 0.0;

void delay_us(uint32_t d){ sleep(d/10000); }

void S_Init_onEntry(void){
    Bridge_onStateChanged("S_INIT");
    DSPinitialise(); DSPshowDisplay(); KYBinitialise();
    FSM_AddEvent(E_CONTINUE);
}
void S_Init_onExit(void){}

void S_Idle_onEntry(void){
    Bridge_onStateChanged("S_IDLE");
    event_t choice = (event_t)Bridge_waitForGuiInput();
    FSM_AddEvent(choice);
}
void S_Idle_onExit(void){}

void S_SelectTea_onEntry(void){
    Bridge_onStateChanged("S_SELECT_TEA");
    int index = Bridge_waitForGuiInput();
    switch(index){
    case 0: selectedTea = "Green Tea";  teaPrice = 2.00f; break;
    case 1: selectedTea = "Black Tea";  teaPrice = 2.00f; break;
    case 2: selectedTea = "Herbal Tea"; teaPrice = 2.50f; break;
    case 3: selectedTea = "Oolong Tea"; teaPrice = 2.75f; break;
    }
    FSM_AddEvent(E_TEA_SELECTED);
}
void S_SelectTea_onExit(void){}

void S_AskForMoney_onEntry(void){
    Bridge_onStateChanged("S_ASK_FOR_MONEY");
    event_t moneyEvent = (event_t)Bridge_waitForGuiInput();
    FSM_AddEvent(moneyEvent);
}
void S_AskForMoney_onExit(void){}

void S_MakeTea_onEntry(void){
    Bridge_onStateChanged("S_MAKE_TEA");
    Bridge_showMessage("Heating water...");
    delay_us(10000);
    Bridge_showMessage("Pouring water...");
    delay_us(10000);
    Bridge_showMessage("Steeping tea...");
    delay_us(10000);
    Bridge_showMessage("Almost ready...");
    delay_us(5000);
    FSM_AddEvent(E_POURED_AND_WAITED);
}
void S_MakeTea_onExit(void){}

void S_DoneTea_onEntry(void){
    Bridge_onStateChanged("S_DONE_TEA");
    totalTeas++;
    insertedMoney = 0.0;
    teaPrice = 0.0;
    delay_us(5000);
    FSM_AddEvent(E_RESET);
}
void S_DoneTea_onExit(void){}

void S_Shutdown_onEntry(void){
    Bridge_onStateChanged("S_SHUTDOWN");
    delay_us(2000);
}
void S_Shutdown_onExit(void){
    exit(0);
}

void fsm_setup(void)
{
    FSM_AddState(S_START,         &(state_funcs_t){ NULL,                  NULL });
    FSM_AddState(S_INIT,          &(state_funcs_t){ S_Init_onEntry,        S_Init_onExit });
    FSM_AddState(S_IDLE,          &(state_funcs_t){ S_Idle_onEntry,        S_Idle_onExit });
    FSM_AddState(S_SELECT_TEA,    &(state_funcs_t){ S_SelectTea_onEntry,   S_SelectTea_onExit });
    FSM_AddState(S_ASK_FOR_MONEY, &(state_funcs_t){ S_AskForMoney_onEntry, S_AskForMoney_onExit });
    FSM_AddState(S_MAKE_TEA,      &(state_funcs_t){ S_MakeTea_onEntry,     S_MakeTea_onExit });
    FSM_AddState(S_DONE_TEA,      &(state_funcs_t){ S_DoneTea_onEntry,     S_DoneTea_onExit });
    FSM_AddState(S_SHUTDOWN,      &(state_funcs_t){ S_Shutdown_onEntry,    S_Shutdown_onExit });

    FSM_AddTransition(&(transition_t){ S_START,          E_START,             S_INIT });
    FSM_AddTransition(&(transition_t){ S_INIT,           E_CONTINUE,          S_IDLE });
    FSM_AddTransition(&(transition_t){ S_IDLE,           E_SELECT_TEA,        S_SELECT_TEA });
    FSM_AddTransition(&(transition_t){ S_IDLE,           E_SHUTDOWN,          S_SHUTDOWN });
    FSM_AddTransition(&(transition_t){ S_SELECT_TEA,     E_TEA_SELECTED,      S_ASK_FOR_MONEY });
    FSM_AddTransition(&(transition_t){ S_ASK_FOR_MONEY,  E_ENOUGH_MONEY,      S_MAKE_TEA });
    FSM_AddTransition(&(transition_t){ S_ASK_FOR_MONEY,  E_NOT_ENOUGH_MONEY,  S_ASK_FOR_MONEY });
    FSM_AddTransition(&(transition_t){ S_MAKE_TEA,       E_STILL_MAKING,      S_MAKE_TEA });
    FSM_AddTransition(&(transition_t){ S_MAKE_TEA,       E_POURED_AND_WAITED, S_DONE_TEA });
    FSM_AddTransition(&(transition_t){ S_DONE_TEA,       E_RESET,             S_IDLE });

    FSM_FlushEnexpectedEvents(true);
    state = S_START;
    FSM_AddEvent(E_START);
}