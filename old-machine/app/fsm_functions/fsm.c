#include "fsm.h"
#include "events.h"
#include  "states.h"
#include  "appInfo.h"
#include <string.h>
#include <stdio.h>

#define MAX_EVENTS_IN_BUFFER_MASK (MAX_EVENTS_IN_BUFFER - 1)
#if (MAX_EVENTS_IN_BUFFER & MAX_EVENTS_IN_BUFFER_MASK)
#error events size is not a power of two
#endif

// Global variables
state_funcs_t state_funcs[MAX_STATES] = {0};

transition_t transitions[MAX_TRANSITIONS];
static volatile uint8_t transition_cnt = 0;

event_t events[MAX_EVENTS_IN_BUFFER];
static volatile uint8_t head = 0;
static volatile uint8_t tail = 0;

static volatile bool flush_event = 0;

int numOfStates;
int numOfTransitions;

state_t FSM_EventHandler(const state_t state, const event_t event)
{
   state_t nextState = state;

   // Check all transitions in the transition matrix
   for(uint8_t i=0; i <= transition_cnt; ++i)
   {
      // Is the state equal to the from state?
      if(transitions[i].from == state)
      {
         // And is the event equal to the event?
         if(transitions[i].event == event)
         {
            // Execute the from state onExit() function
            if(state_funcs[transitions[i].from].onExit != NULL)
            {
               state_funcs[transitions[i].from].onExit();
            }

            // Set the next state
            nextState = transitions[i].to;

            // Execute the to state onEntry() function
            if(state_funcs[transitions[i].to].onEntry != NULL)
            {
               state_funcs[transitions[i].to].onEntry();
            }

            return nextState;
         }
      }
   }

   // Still here, so the event is unexpected in the current state. Remain in
   // current state. Optionally, return the event back in the event buffer.
   if(!flush_event)
   {
      FSM_AddEvent(event);
   }

   return nextState;
}

void FSM_FlushEnexpectedEvents(const bool flush)
{
   flush_event = flush;
}

void FSM_AddState(const state_t state, const state_funcs_t *funcs)
{
   if(state >= MAX_STATES)
   {
      // Error, state is out of bounds
      return;
   }

   // Copy the state and save locally
   memcpy(&state_funcs[state], funcs, sizeof(state_funcs_t));
   numOfStates++;
}

void FSM_AddTransition(const transition_t *transition)
{	
   if(transition_cnt == MAX_TRANSITIONS)
   {
      // Error, too many transitions
      return;
   }

   // Copy the transition and save locally
   memcpy(&transitions[transition_cnt], transition, sizeof(transition_t));

   ++transition_cnt;
   numOfTransitions++;
}

event_t FSM_PeekForEvent(void)
{
   return events[head];
}

bool FSM_NoEvents(void)
{
   return (head == tail);
}

event_t FSM_WaitForEvent(void)
{
   while(FSM_NoEvents())
   {;}

   return FSM_GetEvent();
}

uint8_t FSM_NofEvents(void)
{
   if(head == tail)
      return 0;
   else if(head > tail)
      return head - tail;
   else
      return MAX_EVENTS_IN_BUFFER - tail + head;
}

void FSM_AddEvent(const event_t event)
{
   uint8_t tmpHead;

   // Calculate index
   tmpHead = (head + 1) & MAX_EVENTS_IN_BUFFER_MASK;

   // Check if queue is full
   if(tmpHead == tail)
   {
      // Queue is full, flush the event
      return;
   }

   // Store the event in the queue
   events[tmpHead] = event;

   // Save the new index
   head = tmpHead;
}

event_t FSM_GetEvent(void)
{
   event_t event = E_NO;
   uint8_t tmpTail;

   if(!FSM_NoEvents())
   {
      // Calculate index
      tmpTail = (tail + 1) & MAX_EVENTS_IN_BUFFER_MASK;

      // Get the event from the queue
      event = events[tmpTail];

      // Store the new index
      tail = tmpTail;
   }

   return event;
}

void FSM_RunStateMachine(state_t state, event_t start_event)
{
   extern event_t event;
   state = S_START;
   FSM_AddEvent(start_event);    // Machine is switched on

   while(1)
   {
      if(!FSM_NoEvents())
      {
         // Get the event and handle it
         event = FSM_GetEvent();
         state = FSM_EventHandler(state, event);
      }
   }
}


void FSM_RevertModel(void)
{
   extern int numOfStates;
   extern int numOfTransitions;
   extern transition_t transitions[];
   extern char * stateEnumToText[];
   extern char * eventEnumToText[];

   printf("Transition count: %i\n", numOfTransitions);
   printf("States count: %i\n", numOfStates);

   printf("@startuml\n");
   printf("[*] --> %s : %s\n", stateEnumToText[transitions[0].to],eventEnumToText[transitions[0].event]);

   for (int i = 1; i < numOfTransitions; i++)
   {
      printf("%s --> %s : %s\n", stateEnumToText[transitions[i].from],stateEnumToText[transitions[i].to],eventEnumToText[transitions[i].event]);
   }
   printf("@enduml\n");
}
