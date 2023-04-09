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

// Example: 102424.000,21.06823,105.80386,2.0,19.6,3,000.00,1.4,0.8,060423,09
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