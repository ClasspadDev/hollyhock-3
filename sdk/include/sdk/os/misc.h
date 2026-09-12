/**
 * @file
 * @brief Uncategorized functions
 * 
 * If they dont fit anywhere else but are too few to make a new header
 */

#pragma "once"

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

/**
 * Spinwaits using external timer unit 5 (ETMU5).
 * Useful for communication via USB.
 *
 * @param ms Sleep duration in milliseconds
 */
extern void (*EMTU_Sleep)(unsigned ms);

#ifdef __cplusplus
}
#endif
