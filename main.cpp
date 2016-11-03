#include <iostream>
#include <netdb.h>
#include <thread>
#include <cstring>
#include "Client/Client.h"
#include "Server/Server.h"
#include "Message/Message.h"

using namespace std;


// For client
int client() {
    int x=5;
    auto client = Client();
    char *message = (char *) "~Server not ~server()";
    Message request(MessageType::Request, message, strlen(message), 0);
    auto reply = client.execute(&request, (char *) "localhost", 1234);
    cout << (char*) reply.getMessage();

    return 0;
}

//for server
int server()
{
     auto server = Server((char *) "localhost", 1234, 10);
    return 0;

}
//Testing
int main() {

    std::thread c(client);
    std::thread s(server);

    c.join();
    s.join();

return 0;

}


