#include "stdio.h"
#include "ec200u.c"
#include "at_modem.c"
#include "timeout_clock.c"
#include "string_util.c"

void main() {
    Ec200u ec200u;
    // Ec200uCheckATCmd(&ec200u);
    // Ec200uOpenNetwork(&ec200u, "hello", "1234");
    // Ec200uConnectServer(&ec200u, "tpmd", "username", "password");
    Ec200uPublishMessage(&ec200u, "2", "1", "/device", "hello");
}