#include <SQLiteCpp/Database.h>
#include <iostream>
#include "Registry.h"

int main() {


    Registry Reg(NULL,1234,1,"/home/farida/Documents/Dist-DB.db");

//    int remove_entry_svc(std::string image_name, long int token);
//
//    Reg.remove_entry_svc("img100", -401554244);

//    long int token;
//    Reg.retrieve_token_svc("Zeyad", "xyz", token);
//    std:: cout << "token: " << token << std::endl;

    //setNumViews_EachUser_svc(std::string image_id, long int peer_token)//, int noViews)

    //int n = Reg.setNumViews_EachUser_svc("img1", -401554244 );


//    long int token;
//    int n = Reg.retrieve_token_svc("ahmed", "1234", token);
//    std::cout << "token: " << token << std::endl;
//
//    int n2 = Reg.check_token_svc(-401554244);
//    std::cout << "n2: " << n2 << std::endl;

//    bool can_view;
//    int n = Reg.check_viewImage_svc("img1",can_view , 1);
//
//    std::cout << "can: " << can_view << std::endl;

//    std::string owner_addr;
//    uint16_t owner_port=0;
//
//   int n = Reg.get_client_addr_svc("img100",owner_addr,owner_port, 3456789);
//    std::cout << "n: " << n << std::endl;
//    std::cout << "owner_addr: " << owner_addr << std::endl;
//    std::cout << "owner_port: " << owner_port << std::endl;




//    int n = Reg.remove_entry_svc("img1",1111);
//    std::cout << n;


//    int n = Reg.add_entry_svc("img100", -401554244, "192.11.111", 123);
//    std::cout << n;


//    std::vector<std::string> list;
//    auto n = Reg.view_imagelist_svc(list,-401554244);
//
//    for (int i=0; i < list.size(); i++)
//        std::cout << list[i] << std::endl;


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
