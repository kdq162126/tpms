#include "stdio.h"
#include "string.h"

int main() {
    char* receiveData = "+QGPSLOC: 102843.000,21.06819,105.80383,2.4,23.0,3,000.00,1.6,0.9,060423,11\r\n\nOK\r\n";

    // // Remove head string
    // char* resp = ;
    // // Remove tail string
    // resp[strlen(resp) - strlen("\r\n\nOK\r\n")] = '\0';

    char resp[100];
    memset(resp, 0, 100);
    memcpy(resp, receiveData + strlen("+QGPSLOC: "), strlen(receiveData) - strlen("+QGPSLOC: ") - strlen("\r\n\nOK\r\n"));

    printf("%s\n", resp);

    while (1);
}