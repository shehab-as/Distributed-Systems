#include <SQLiteCpp/Database.h>
#include <iostream>
#include "Registry.h"

int main() {


    Registry Reg(NULL,1234,1);
    std::cout<< Reg.add_entry_svc("image name",2,"2222", 1234)<<std::endl;
    std::cout << "done" << std::endl;

//   std::string query =
//
//    auto n = db.exec()

//    CM server(NULL, 1234);
//    Message reply;
//    sockaddr_in sender_addr;
//    server.recv_with_block(reply, sender_addr);
//    std::cout << "Message reply size: " << reply.marshal().size() << std::endl;
//    std::cout << "Message Marshalled: " << reply.marshal() << std::endl;

}


//
// Created by zeyad on 11/18/16.

//
//#include "../DataBase/Db.h"
//#include <iostream>
//
//using namespace std;
//
//// Registry main
//int main()
//{
//    auto db=new Db();
//    return 0;
//}
