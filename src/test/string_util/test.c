#include "stdio.h"
#include "string_util.c"
#include "printf.h"

// void testBuildCsv(void) {
//     char cmd[100];
//     char* buff = cmd;
//     buff = StringAppendString(buff, "AT=0");
//     buff = StringAppendInt(buff, 10000);
//     buff = StringAppendString(buff, "hehe");
//     buff = StringAppendFloat(buff, 312.143123122);
//     printf("%s\n", cmd);
// }

// void testString(char* host, char* port) {
//     char cmd[100];
//     char* buff = cmd;
//     char* args[] = { host, port };
//     buff = StringJoin(buff, args, 2, ',');
//     char* arr2[] = { "A", "B", "C" };
//     buff = StringJoin(buff, arr2, 3, ',');
//     printf("%s\n", cmd);
// }

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

void main() {
    // char cmd[200];
    // CreateMockSensorMessage(cmd);
    // printf("%s\n", cmd);

    float x = 123.4567;
    char buf[10];

    gcvt(x, 6, buf);

    printf("buffer is: %s\n", buf);
}





