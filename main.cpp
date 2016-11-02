#include <iostream>
#include <netdb.h>
#include <thread>
#include <cstring>
#include "Client/Client.h"
#include "Server/Server.h"
#include "Message/Message.h"

using namespace std;

// For server
int main() {
    auto server = Server((char *) "localhost", 1234, 10);
}

// For client
int client() {
    auto client = Client();
    char *message = (char *) "~Server not ~server()";
    Message request(MessageType::Request, message, strlen(message), 0);
    auto reply = client.execute(&request, (char *) "localhost", 1234);
    cout << reply.getMessage();

    return 0;
}