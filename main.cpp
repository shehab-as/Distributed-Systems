#include <iostream>
#include <netdb.h>
#include <thread>
#include <cstring>
#include "Client/Client.h"
#include "Server/Server.h"
#include "Message/Message.h"

using namespace std;

void serve()
{
    auto server = Server((char *) "localhost", 1234, 10);
}
int main() {
    thread t1(serve);
    auto client = Client();
    char message[50] = "~Server() is not ~serve";
    Message request(MessageType::Request, message, strlen(message), 0);
    request = client.execute(&request, (char *) "localhost", 1234);
    std::cout << request.getMessage() << endl;
    t1.join();

}