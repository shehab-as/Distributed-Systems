#include <iostream>
#include <netdb.h>
#include <thread>
#include <cstring>
#include "Client/Client.h"
#include "Server/Server.h"
#include "Message/Message.h"

using namespace std;

int main() {
    auto server = Server((char *) "localhost", 1234, 10);
}