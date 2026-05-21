#ifndef SYSTEMERRORS_H
#define SYSTEMERRORS_H

/// \brief Error bit index values.
typedef enum {
   ERR_INIT_CNA, ///< Initialisation error Coin Acceptor selftest
   ERR_INIT_CLD,  ///< Initialisation error Cola Dispenser selftest
   ERR_INIT_CHD   ///< Initialisation error Change Dispenser selftest
} error_t;

/// Type name for bit mapped errors: 8 bits.
typedef unsigned char systemErrors_t;

/// Set error bit.
/// \param err error bit index.
/// \return previous value error bit.
int setSystemErrorBit(error_t err);

/// Set error bit.
/// \param err error bit index.
/// \return boolean value 0 or 1.
int getSystemErrorBit(error_t err);

/// \return value of systemErrorBits.
systemErrors_t getSystemErrorBits(void);

/// \return string showing 8 error bits in binary format.
/// Example: "11000000"
const char *getSystemErrorBitsString(void);

#endif