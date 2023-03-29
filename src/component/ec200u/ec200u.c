#include "ec200u.h"
#include "stdlib.h"

bool Ec200uCheckATCmd(Ec200u* this) {
    char* buff = this->base.txCmdBuff;
    buff = StringAppendString(buff, EC200U_CMD_CHECK_AT);
    AT_MODEM_BUILD_TAIL(buff);

    return ATmodemExcutiveCmd((ATmodem*)this, this->base.txCmdBuff, "OK", EC200U_NORMAL_CMD_TIMEOUT_MS, 3);
}

bool Ec200uResetModule(Ec200u* this) {
    char* buff = this->base.txCmdBuff;
    AT_MODEM_BUILD_HEAD(buff);
    buff = StringAppendString(buff, EC200U_CMD_SIM_FUNC);
    *buff++ = '1';
    *buff++ = ',';
    *buff++ = '1';
    AT_MODEM_BUILD_TAIL(buff);

    return ATmodemExcutiveCmd((ATmodem*)this, this->base.txCmdBuff, "OK", 20000, 3);
}

bool Ec200uTurnOffEcho(Ec200u* this) {
    char* buff = this->base.txCmdBuff;
    buff = StringAppendString(buff, EC200U_CMD_TURN_OFF_ECHO);
    AT_MODEM_BUILD_TAIL(buff);

    return ATmodemExcutiveCmd((ATmodem*)this, this->base.txCmdBuff, "OK", EC200U_NORMAL_CMD_TIMEOUT_MS, 3);
}

bool Ec200uCheckSimReady(Ec200u* this) {
    char* buff = this->base.txCmdBuff;

    AT_MODEM_BUILD_HEAD(buff);
    buff = StringAppendString(buff, EC200U_CMD_CHECK_NETWORK_STT);
    AT_MODEM_BUILD_TAIL(buff);

    return ATmodemExcutiveCmd((ATmodem*)this, this->base.txCmdBuff, "+CREG: 0,1", 1000, 10);
}

int8_t Ec200uOpenNetwork(Ec200u* this, char* host, char* port) {
    char* buff = this->base.txCmdBuff;

    AT_MODEM_BUILD_HEAD(buff);
    buff = StringAppendString(buff, EC200U_CMD_OPEN_NETWORK);
    *buff++ = '0';
    *buff++ = ',';
    buff = StringAppendStringWithQuote(buff, host);
    *buff++ = ',';
    buff = StringAppendString(buff, port);
    AT_MODEM_BUILD_TAIL(buff);

    char* expectResponse = "+QMTOPEN: 0,";
    bool isSuccess = ATmodemExcutiveCmd((ATmodem*)this, this->base.txCmdBuff, expectResponse, 10000, 3);
    if (!isSuccess) {
        return (-1);
    }

    char networkStatus = *(strstr(this->base.receiveData, expectResponse) + strlen(expectResponse));
    return atoi(&networkStatus);
}

bool Ec200uCloseNetwork(Ec200u* this) {
    char* buff = this->base.txCmdBuff;

    AT_MODEM_BUILD_HEAD(buff);
    buff = StringAppendString(buff, EC200U_CMD_CLOSE_NETWORK);
    buff = StringAppendString(buff, "0");
    AT_MODEM_BUILD_TAIL(buff);

    return ATmodemExcutiveCmd((ATmodem*)this, this->base.txCmdBuff, "+QMTCLOSE: 0,0", 30000, 3);
}

bool Ec200uConnectServer(Ec200u* this, char* clientID, char* username, char* password) {
    char* buff = this->base.txCmdBuff;

    AT_MODEM_BUILD_HEAD(buff);
    buff = StringAppendString(buff, EC200U_CMD_CONNECT_SERVER);
    *buff++ = '0';
    *buff++ = ',';
    buff = StringAppendStringWithQuote(buff, clientID);
    *buff++ = ',';
    buff = StringAppendStringWithQuote(buff, username);
    *buff++ = ',';
    buff = StringAppendStringWithQuote(buff, password);
    AT_MODEM_BUILD_TAIL(buff);

    return ATmodemExcutiveCmd((ATmodem*)this, this->base.txCmdBuff, "+QMTCONN: 0,0,0", 5000, 3);
}

bool Ec200uDisconnectServer(Ec200u* this) {
    char* buff = this->base.txCmdBuff;

    AT_MODEM_BUILD_HEAD(buff);
    buff = StringAppendString(buff, EC200U_CMD_DISCONNECT_SERVER);
    buff = StringAppendString(buff, "0");
    AT_MODEM_BUILD_TAIL(buff);

    return ATmodemExcutiveCmd((ATmodem*)this, this->base.txCmdBuff, "+QMTDISC: 0,0", 30000, 3);
}

bool Ec200uPublishMessage(Ec200u* this, char* qos, char* retain, char* topic, char* message) {
    char* buff = this->base.txCmdBuff;

    AT_MODEM_BUILD_HEAD(buff);
    buff = StringAppendString(buff, EC200U_CMD_PUBLISH_MESSAGE);
    *buff++ = '0';
    *buff++ = ',';
    *buff++ = '0';
    *buff++ = ',';
    buff = StringAppendString(buff, qos);
    *buff++ = ',';
    buff = StringAppendString(buff, retain);
    *buff++ = ',';
    buff = StringAppendStringWithQuote(buff, topic);
    *buff++ = ',';
    buff = StringAppendInt(buff, (uint16_t)strlen(message));
    AT_MODEM_BUILD_TAIL(buff);

    bool isReadyPublish = ATmodemExcutiveCmd((ATmodem*)this, this->base.txCmdBuff, ">", 10000, 3);
    if (!isReadyPublish)
        return false;

    return ATmodemExcutiveCmd((ATmodem*)this, message, "+QMTPUBEX: 0,0,0\r\n", 10000, 1);
}



bool Ec200uTurnOnGPS(Ec200u* this) {
    char cmd[EC200U_TX_BUFFER_CMD_LENGTH];
    char* buff = cmd;

    AT_MODEM_BUILD_HEAD(buff);
    buff = StringAppendString(buff, EC200U_GPS_CMD_TURN_ON);
    AT_MODEM_BUILD_HEAD(buff);

    return ATmodemExcutiveCmd((ATmodem*)this, cmd, "OK\r\n", EC200U_NORMAL_CMD_TIMEOUT_MS, 3);
}

bool Ec200uTurnOffGPS(Ec200u* this) {
    char cmd[EC200U_TX_BUFFER_CMD_LENGTH];
    char* buff = cmd;

    AT_MODEM_BUILD_HEAD(buff);
    buff = StringAppendString(buff, EC200U_GPS_CMD_TURN_OFF);
    AT_MODEM_BUILD_HEAD(buff);

    return ATmodemExcutiveCmd((ATmodem*)this, cmd, "OK\r\n", EC200U_NORMAL_CMD_TIMEOUT_MS, 3);
}

char* Ec200uAcquirePositionInfo(Ec200u* this) {
    char cmd[EC200U_TX_BUFFER_CMD_LENGTH];
    char* buff = cmd;

    AT_MODEM_BUILD_HEAD(buff);
    buff = StringAppendString(buff, EC200U_GPS_CMD_GET_POSITION);
    *buff++ = '=';
    *buff++ = '2';
    AT_MODEM_BUILD_HEAD(buff);

    bool isSuccess = ATmodemExcutiveCmd((ATmodem*)this, cmd, "OK\r\n", EC200U_NORMAL_CMD_TIMEOUT_MS, 3);
    if (isSuccess) {
        // Remove head string
        char* resp = this->base.receiveData + strlen("+QGPSLOC: ");
        // Remove tail string
        resp[strlen(resp) - strlen("\r\n\nOK\r\n")] = '\0';
        return resp;
    }

    return NULL;
}



bool Ec200uActiveRF(Ec200u* this) {
    char* buff = this->base.txCmdBuff;

    AT_MODEM_BUILD_HEAD(buff);
    buff = StringAppendString(buff, EC200U_CMD_SIM_FUNC);
    *buff++ = '1';
    AT_MODEM_BUILD_TAIL(buff);

    return ATmodemExcutiveCmd((ATmodem*)this, this->base.txCmdBuff, "OK", 5000, 3);
}

bool Ec200uDeactiveRF(Ec200u* this) {
    char* buff = this->base.txCmdBuff;

    AT_MODEM_BUILD_HEAD(buff);
    buff = StringAppendString(buff, EC200U_CMD_SIM_FUNC);
    *buff++ = '0';
    AT_MODEM_BUILD_TAIL(buff);

    return ATmodemExcutiveCmd((ATmodem*)this, this->base.txCmdBuff, "OK", 5000, 3);
}

