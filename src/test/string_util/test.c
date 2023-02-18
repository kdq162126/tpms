#include "stdio.h"
#include "string_util.c"

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

void main() {
    testBuildCsv();
}





