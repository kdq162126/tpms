/*
 * ec200u.h
 *
 *  Created on: Feb 12, 2023
 *      Author: khanh
 */

#ifndef COMPONENT_EC200U_EC200U_H_
#define COMPONENT_EC200U_EC200U_H_

#include "at_modem.h"
#include "string.h"
#include "stdbool.h"
#include "string_util.h"

#define EC200U_CMD_CHECK_AT             "AT"
#define EC200U_CMD_SIM_FUNC             "CFUN="
#define EC200U_CMD_TURN_OFF_ECHO        "ATE0"

#define EC200U_CMD_CHECK_NETWORK_STT    "CREG?"
#define EC200U_CMD_CHECK_SIGNAL_QUALITY "CSQ"
#define EC200U_CMD_OPEN_NETWORK         "QMTOPEN="
#define EC200U_CMD_CLOSE_NETWORK        "QMTCLOSE="
#define EC200U_CMD_CONNECT_SERVER       "QMTCONN="
#define EC200U_CMD_DISCONNECT_SERVER    "QMTDISC="
#define EC200U_CMD_PUBLISH_MESSAGE      "QMTPUBEX="

#define EC200U_GPS_CMD_TURN_ON          "QGPS"
#define EC200U_GPS_CMD_TURN_OFF         "QGPSEND"
#define EC200U_GPS_CMD_GET_POSITION     "QGPSLOC"
#define EC200U_GPS_CMD_CONFIG           "QGPSCFG"

#define EC200U_NORMAL_CMD_TIMEOUT_MS    500UL
#define EC200U_TX_BUFFER_CMD_LENGTH     100UL   
#define EC200U_TX_BUFFER_DATA_LENGTH    300UL      

typedef struct Ec200u_t Ec200u;
struct Ec200u_t {
    ATmodem base;
};

bool Ec200uCheckATCmd(Ec200u* this);
bool Ec200uResetModule(Ec200u* this);
bool Ec200uTurnOffEcho(Ec200u* this);
bool Ec200uCheckSimReady(Ec200u* this);
int8_t Ec200uOpenNetwork(Ec200u* this, char* host, char* port);
bool Ec200uCloseNetwork(Ec200u* this);
bool Ec200uConnectServer(Ec200u* this, char* clientID, char* username, char* password);
bool Ec200uDisconnectServer(Ec200u* this);
bool Ec200uPublishMessage(Ec200u* this, char* qos, char* retain, char* topic, char* message);
bool Ec200uActiveRF(Ec200u* this);
bool Ec200uDeactiveRF(Ec200u* this);

int8_t Ec200uCheckGpsTurnOnStatus(Ec200u* this);
bool Ec200uTurnOnGPS(Ec200u* this);
bool Ec200uTurnOffGPS(Ec200u* this);
char* Ec200uAcquirePositionInfo(Ec200u* this);

#endif /* COMPONENT_EC200U_EC200U_H_ */
