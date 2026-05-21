#ifndef DEVCONSOLE_H
#define DEVCONSOLE_H

/// Initialises the Development ConSole subsystem.
/// \todo Is DCS a subsystem? It is part of a development system.
void DCSinitialise(void);

/// Shows questionText extended with '[y/n]'.
/// User can enter Y by only pressing \<enter\>.
/// \return boolean value, equals true if Y has been chosen.
int DCSsimulationSystemInputYN(const char questionText[]);

/// Shows text, user can enter a char. If this char is not in chrs then the user
/// gets the same text extended with AGAIN.
/// \return char value.
char DCSsimulationSystemInputChar(const char text[], const char chrs[]);

/// Shows text, user can enter an integer value. If this value is not >= min
/// and <= max, the user gets the same text extended with AGAIN. 
/// \pre min < max. 
/// \return entered int value.
int DCSsimulationSystemInputInteger(const char text[], int min, int max);

/// Prints text and waits for input, Has scanf() interface.
/// \return the number of items in the successfully filled.
int DCSsimulationSystemInput(const char text[], const char fmt[], ...);

/// Shows debug related message, below the display.
/// Has printf() interface.
void DCSdebugSystemInfo(const char fmt[], ...);

/// Shows simulation related text, below the display.
/// Has printf() interface.
void DCSsimulationSystemInfo(const char fmt[], ...);

/// Shows system error related text, below the display.
/// Has printf() interface.
/// If a system error is detected, most of the time the system needs to
/// shutdown (if still possible) or will run at some reduced level of
/// performance (graceful degradation).
void DCSshowSystemError(const char fmt[], ...);

#endif
