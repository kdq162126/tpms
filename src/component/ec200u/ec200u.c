#include "ec200u.h"

bool Ec200uCheckATCmd(Ec200u* this) {
    char cmd[EC200U_TX_BUFFER_CMD_LENGTH];
    char* buff = cmd;
    buff = StringAppendString(buff, EC200U_CMD_CHECK_AT);
    AT_MODEM_BUILD_TAIL(buff);

    return ATmodemExcutiveCmd((ATmodem*)this, cmd, "OK", EC200U_NORMAL_CMD_TIMEOUT_MS, 3);
}

bool Ec200uTurnOffEcho(Ec200u* this) {
    char cmd[EC200U_TX_BUFFER_CMD_LENGTH];
    char* buff = cmd;
    buff = StringAppendString(buff, EC200U_CMD_TURN_OFF_ECHO);
    AT_MODEM_BUILD_TAIL(buff);

    return ATmodemExcutiveCmd((ATmodem*)this, cmd, "OK", EC200U_NORMAL_CMD_TIMEOUT_MS, 3);
}

bool Ec200uCheckSimReady(Ec200u* this) {
    char cmd[EC200U_TX_BUFFER_CMD_LENGTH];
    char* buff = cmd;

    AT_MODEM_BUILD_HEAD(buff);
    buff = StringAppendString(buff, EC200U_CMD_CHECK_NETWORK_STT);
    AT_MODEM_BUILD_TAIL(buff);

    return ATmodemExcutiveCmd((ATmodem*)this, cmd, "+CREG: 0,2", EC200U_NORMAL_CMD_TIMEOUT_MS, 10);
}

bool Ec200uOpenNetwork(Ec200u* this, char* host, char* port) {
    char cmd[EC200U_TX_BUFFER_CMD_LENGTH];
    char* buff = cmd;

    AT_MODEM_BUILD_HEAD(buff);
    buff = StringAppendString(buff, EC200U_CMD_OPEN_NETWORK);
    *buff++ = '0';
    *buff++ = ',';
    buff = StringAppendStringWithQuote(buff, host);
    *buff++ = ',';
    buff = StringAppendString(buff, port);
    AT_MODEM_BUILD_TAIL(buff);

    return ATmodemExcutiveCmd((ATmodem*)this, cmd, "+QMTOPEN: 0,", 10000, 3);
}

bool Ec200uCloseNetwork(Ec200u* this) {
    char cmd[EC200U_TX_BUFFER_CMD_LENGTH];
    char* buff = cmd;

    AT_MODEM_BUILD_HEAD(buff);
    buff = StringAppendString(buff, EC200U_CMD_CLOSE_NETWORK);
    buff = StringAppendString(buff, "0");
    AT_MODEM_BUILD_TAIL(buff);

    return ATmodemExcutiveCmd((ATmodem*)this, cmd, "+QMTCLOSE: 0,0", 30000, 3);
}

bool Ec200uConnectServer(Ec200u* this, char* clientID, char* username, char* password) {
    char cmd[EC200U_TX_BUFFER_CMD_LENGTH];
    char* buff = cmd;

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

    return ATmodemExcutiveCmd((ATmodem*)this, cmd, "+QMTCONN: 0,0,0", 5000, 3);
}

bool Ec200uDisconnectServer(Ec200u* this) {
    char cmd[EC200U_TX_BUFFER_CMD_LENGTH];
    char* buff = cmd;

    AT_MODEM_BUILD_HEAD(buff);
    buff = StringAppendString(buff, EC200U_CMD_DISCONNECT_SERVER);
    buff = StringAppendString(buff, "0");
    AT_MODEM_BUILD_TAIL(buff);

    return ATmodemExcutiveCmd((ATmodem*)this, cmd, "+QMTDISC: 0,0", 30000, 3);
}

bool Ec200uPublishMessage(Ec200u* this, char* qos, char* retain, char* topic, char* message) {
    char cmd[EC200U_TX_BUFFER_CMD_LENGTH];
    char* buff = cmd;

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
    buff = StringAppendInt(buff, strlen(message));
    AT_MODEM_BUILD_TAIL(buff);

    bool isReadyPublish = ATmodemExcutiveCmd((ATmodem*)this, cmd, ">", 10000, 3);
    if (!isReadyPublish)
        return false;

    return ATmodemExcutiveCmd((ATmodem*)this, message, "+QMTPUBEX: 0,0,0", 10000, 1);
}

