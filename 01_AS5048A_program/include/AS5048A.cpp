#include "AS5048A.h"

/**
 * Constructor
 */
AS5048A::AS5048A(uint8_t cs, bool debug /*=false*/)
	: _cs(cs), errorFlag(false), ocfFlag(false), position(0), debug(debug)
{
}

