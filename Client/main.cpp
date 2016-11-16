#include <iostream>
#include <vector>
#include <thread>
#include "Client.h"
#include "../common/CM/CM.h"

using namespace std;

int main(int argc, char *argv[]) {
    CM client((char *) "192.168.2.21", 0);
    vector<std::string> message = {"HI BYE"};
    Message request(MessageType::Request, message, message.size(), 25);
    auto request_mashalled = request.marshal();
    size_t BUFFER_SIZE = 7500;
    char BUFFER[BUFFER_SIZE];
    client.send_with_ack((char *) request_mashalled.c_str(), BUFFER, BUFFER_SIZE, 1000, 3, (char *) "10.40.38.71", 1234);
    cout << BUFFER << endl;
//    if (argc != 4) {
//        std::cerr << "Arguments: SOURCE_PORT, DESTINATION_ADDR, DESTINATION_PORT" << endl;
//    }
//
//    long source_port = strtol(argv[1], NULL, 10);
//    char * destination_addr = argv[2];
//    long destination_port = strtol(argv[3], NULL, 10);
//
//    auto client = Client((char *) "localhost", (uint16_t) source_port, destination_addr, (uint16_t) destination_port);
//
////    string message = "~Server() not ~server()";
//    vector<std::string> message = {to_string(1), "my_name_is"};
//    Message request(MessageType::Reply, message, message.size(), 25);
//    auto what = request.marshal();
//    Message who((char *) what.c_str());
//
////    auto reply = client.execute(&request);
////    cout << (char *) reply.getMessage() << endl;
}

/*
 * The following will cause the project's Server to drop some packets.
 *
 * -----------
 * EXPLNATION
 * -----------
 * Whenever Server receives a request, the receiving thread will sleep for a certain amount of time
 * to emulate a job being processed.
 * Afterwards, the same request that was originaly sent to the server will be sent as a reply.
 *
 * For the client, a request will initially be sent to the server's address.
 * Afterwards, the client will wait for a reply with a timeout of 1s
 * If after the 1s timeout the client does not receive a reply, the packet can be assumed to be dropped.
 *
 * However, due to UDP datagrams' nature, packet can be dropped in a nondeterministically due to routing issues, CPU
 * sheduling on the server etc...
 *
 * To be able to truly assume that packets are being dropped on the server side, 80 clients are made that will bombard
 * the server with constant requests. Given a single thread on the server side, packets are easily dropped on the server
 * side and we can detect this by outputting all the parameters received on the server and the messages the client did not
 * receive replies for. If a parameters a client did not receive a reply for does not appear in the output log of the parameters
 * the server receives, then we can assume that the packets were dropped on arrival due to the server's messages buffer
 * being full.
 *
 * The following code demonstrate how a single client can cause a server to drop packets.
 *
 * NOTE: Due to differences between networks and CPU speed on different computers, some of the following variables may
 * be tweaked to properly exhibit packets being dropped:
 *
 *      - Number of client threads
 *      - Number of messages sent per thread
 *      - Size of each parameters
 *      - Timeout amount for client's readFromSocketWithTimeout() method
 *      - Number of threads on the server (should be 1 for excercise 1)
 *      - Amount of time server sleeps after each request to emulate a job being executed
 *
 * The configuration described above and in the code below is what demonstrated to us consistent replication of the server's
 * packet dropping issue.
 *
 */

//void client(int thread_num) {
//    auto client = Client((char *) "localhost", 0, (char *) "192.168.1.116", 1234);
//    for (int i = 0; i < 100000; i++) {
//        string parameters = "thread #" + to_string(thread_num) + " packet # " + to_string(i);
//        parameters.resize(64980);
//        Message request(MessageType::Request, (char *) parameters.c_str(), parameters.size(), 0);
//        auto reply = client.execute(&request);
//    }
//
//}
//
//
//
//int main() {
//    vector<thread> workers;
//    for (int i = 0; i < 80; i++)
//        workers.push_back(thread(client, i));
//
//    for (int i = 0; i < 80; i++) {
//        cout << "Waiting for thread #" << i << " to join()\n";
//        workers[i].join();
//    }
//
//    cout << "All threads returned\n";
//
//}

