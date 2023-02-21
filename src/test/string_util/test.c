#include "stdio.h"
#include "string_util.c"
#include "string.h"

void testBuildCsv(void) {
    char cmd[100];
    char* buff = cmd;
    buff = StringAppendString(buff, "AT=0");
    buff = StringAppendInt(buff, 10000);
    buff = StringAppendString(buff, "hehe");
    buff = StringAppendFloat(buff, 312.143123122);
    printf("%s\n", cmd);
}

void testString(char* host, char* port) {
    char cmd[100];
    char* buff = cmd;
    char* args[] = { host, port };
    buff = StringJoin(buff, args, 2, ',');
    char* arr2[] = { "A", "B", "C" };
    buff = StringJoin(buff, arr2, 3, ',');
    printf("%s\n", cmd);
}

void CreateMockSensorMessage(char* buff) {
    buff = JsonOpen(buff);
    buff = JsonFromString(buff, "deviceId", "TPMS001");
    *buff++ = ',';
    buff = JsonFromString(buff, "version", "123xxx");
    *buff++ = ',';
    buff = StringAppendStringWithQuote(buff, "tires");
    *buff++ = ':';
    *buff++ = '[';

    char* tireIds[] = { "111","222","333","444" };
    char* tireNames[] = { "L11", "L12", "R11" ,"R12" };
    char* positions[] = { "L11", "L12", "R11" ,"R12" };
    for (uint8_t i = 0; i < 4; i++) {
        buff = JsonOpen(buff);
        buff = JsonFromString(buff, "tireId", tireIds[i]);
        *buff++ = ',';
        buff = JsonFromString(buff, "tireName", tireNames[i]);
        *buff++ = ',';
        buff = JsonFromString(buff, "position", positions[i]);
        *buff++ = ',';
        buff = JsonFromInt(buff, "pressure", (i + 1) * 10);
        *buff++ = ',';
        buff = JsonFromInt(buff, "batteryLevel", (i + 2) * 10);
        *buff++ = ',';
        buff = JsonFromInt(buff, "temperature", (i + 1) * 10);
        buff = JsonClose(buff);
        *buff++ = ',';
    }
    buff--;
    *buff++ = ']';
    buff = JsonClose(buff);
}

void TestParseGpsResponse(char* fullMsg) {
    // Remove head string
    char* resp = fullMsg + strlen("+QGPSLOC: ");

    // Remove tail string
    *(resp + strlen(resp) - strlen("\r\n\nOK\r\n")) = '\0';

    char* token = strtok(resp, ",");
    token = strtok(NULL, ",");
    printf("Lat: %s\n", token);

    token = strtok(NULL, ",");
    printf("Lon: %s\n", token);
}

void main() {
    char buff[200] = { 0 };
    char* msg = "+QGPSLOC: 061951.000,21.03007018340863,105.79585080048011,0.7,62.2,2,000.00,0.0,0.0,110513,09\r\n\nOK\r\n";
    memcpy(buff, msg, strlen(msg));
    TestParseGpsResponse(buff);
}





