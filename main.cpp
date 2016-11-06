#include <iostream>
#include <netdb.h>
#include <thread>
#include <cstring>
#include "Client/Client.h"
#include "Server/Server.h"
#include "common/Message/Message.h"

using namespace std;


// For client
void client(int thread_num) {
    auto client = Client((char *) "localhost", 0, (char *) "10.40.62.55", 1235);
    for (int i = 0; i < 900; i++) {
        string message = "thread #" + to_string(thread_num) + " packet # " + to_string(i);
        Message request(MessageType::Request, (char *) message.c_str(), message.size(), 0);
        auto reply = client.execute(&request);
        //cout << (char *) reply.getMessage() << endl;
    }

}


//Testing
int main() {
    vector<thread> workers;
    for (int i = 0; i < 50; i++)
        workers.push_back(thread(client, i));

    for (int i = 0; i < 50; i++) {
        cout << "Waiting for thread #" << i << " to join()\n";
        workers[i].join();
    }

    cout << "All threads returned\n";

}


