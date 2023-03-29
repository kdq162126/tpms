#include "gps.h"
#include "string.h"

void GpsHandleStateMachine(Gps* this) {
    switch (this->state) {
    case GPS_ST_INACTIVE:
        break;
    case GPS_ST_INITALIZE:
        this->active(this);
        this->updatePositionInfo(this);
        GpsSetState(this, GPS_ST_ACTIVE);
        break;
    case GPS_ST_ACTIVE:
        this->updatePositionInfo(this);
        break;
    case GPS_ST_FAIL:
        this->deactive(this);
        GpsSetState(this, GPS_ST_INACTIVE);
        break;
    }
}

// Example: 1951.000,3150.7223N,11711.9293E,0.7,62.2,2,000.00,0.0,0.0,110513,09
void GpsParseResponse(Gps* this, char* resp) {
    // UTC: Unused
    char* token = strtok(resp, ",");

    // Latitude
    token = strtok(NULL, ",");
    memcpy(this->lat, token, strlen(token));

    // Longtitude
    token = strtok(NULL, ",");
    memcpy(this->lon, token, strlen(token));
}