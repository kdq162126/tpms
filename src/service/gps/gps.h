/*
 * gps.h
 *
 *  Created on: Feb 20, 2023
 *      Author: khanh
 */

#ifndef SERVICE_GPS_GPS_H_
#define SERVICE_GPS_GPS_H_

typedef enum GPS_STATE {
    GPS_ST_INACTIVE = 0,
    GPS_ST_INITALIZE,
    GPS_ST_ACTIVE,
    GPS_ST_FAIL
} GPS_STATE;

typedef struct Gps_t Gps;
struct Gps_t {
    GPS_STATE state;
    char lat[10];
    char lon[10];
    void (*active)(Gps*);
    void (*deactive)(Gps*);
    void (*updatePositionInfo)(Gps*);
};

void GpsHandleStateMachine(Gps* this);
void GpsParseResponse(Gps* this, char* resp);

static inline void GpsSetState(Gps* this, GPS_STATE state) {
    this->state = state;
}

static inline GPS_STATE GpsGetState(Gps* this) {
    return this->state;
}

static inline void GpsActive(Gps* this) {
    this->active(this);
}

static inline void GpsDeactive(Gps* this) {
    this->deactive(this);
}

static inline void GpsUpdatePositionInfo(Gps* this) {
    this->updatePositionInfo(this);
}

#endif /* SERVICE_GPS_GPS_H_ */
