#include "UVSensor.h"
#include "abvolts.h"

const int CS_PIN = 21;
const int SCL_PIN = 20;
const int DO_PIN = 19;
const int DI_PIN = 18;

const int AD_SLOT = 0;

void uvInitialize() {
    ad_init(CS_PIN, SCL_PIN, DO_PIN, DI_PIN);
}

int uvReadRawValue() {
    return ad_in(AD_SLOT);
}