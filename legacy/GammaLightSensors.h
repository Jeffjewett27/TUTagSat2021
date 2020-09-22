/*
 * Author: Jeff Jewett
 * Modification Date: 5/10/20
*/

void startLightGammaThread();
void stopLightGammaThread();

//try not to call from other threads
void gammaInitialize();
void lightInitialize();
void gammaLightThread();

//these functions have the side effect of resetting the respective counter to zero
uint32_t gammaReadReset();
uint32_t lightToFrequencyReadReset();