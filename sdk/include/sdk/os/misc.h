/**
 * @file
 * @brief Uncategorized functions
 * 
 * If they dont fit anywhere else but are too few to make a new header
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Queries the ADC for the current battery voltage.
 *
 * Interpret the return value as x.xx V.
 * 
 * @return The voltage in cV.
 */
extern int (*Battery_GetVoltage)();

#ifdef __cplusplus
}
#endif
