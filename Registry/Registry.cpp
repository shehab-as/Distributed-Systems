#include <thread>
#include <string>
#include <vector>
#include <SQLiteCpp/Database.h>
#include <iostream>
#include "Registry.h"
#include "../common/Message/Message.h"

Registry::Registry(char *_listen_hostname, uint16_t _listen_port, int num_of_workers) : serverConnector(
        _listen_hostname, _listen_port) {

    std::vector<std::thread> workers;
    load_DBs();

    for (int i = 0; i < num_of_workers; i++)
        workers.push_back(std::thread(&Registry::runRegistry, this));

    for (int i = 0; i < num_of_workers; i++)
        workers[i].join();

}

Registry::~Registry() {

}

// Fancy way to surpress infinite loop warning inside of this method
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"

void Registry::runRegistry() {
    // The core of the registry
    // Each worker thread should run this method
    sockaddr_in sender_addr;

    while (true) {
        Message recv_message = Message();
        ssize_t bytes_read = serverConnector.recv_with_block(recv_message, sender_addr);
        handleRequest(recv_message, sender_addr);
    }
}

#pragma clang diagnostic pop

void Registry::handleRequest(Message request, sockaddr_in sender_addr) {
    // THE GREAT SWITCH
    // Check which RPC method/operation was called and send reply accordingly
    // 0: view_imagelist_svc(std::vector<std::string> &image_container);
    // 1: add_entry_svc(std::string image_name);
    // 2: remove_entry_svc(std::string image_name);
    // 3: get_client_addr_svc(std::string image_name, sockaddr_in &owner_addr);
    // 4: retrieve_token_svc(std::string username, std::string password, int &token);
    // 5: check_viewImage_svc(std::string image_id, bool &can_view, int token);
    switch (request.getOperation()) {
        case 0: {
            // 0: view_imagelist_svc(std::vector<std::string> &image_container);
            std::vector<std::string> image_container;
            std::vector<std::string> params;
            params = request.getParams();
            //Note here: taking only token from params vector to call view_imagelist function.
            //~Shehab
            long int token = stoi(params[params.size() - 1]);
            auto n = view_imagelist_svc(image_container, token);
            Message reply(MessageType::Reply, 0, request.getRPCId(), std::to_string(n), image_container.size(),
                          image_container);
            serverConnector.send_no_ack(reply, sender_addr);
            break;
        }
        case 1: {
            // 1: add_entry_svc(std::string image_name, int token);
            std::vector<std::string> params;  // params[0] = image_name, params[size-1] = token
            params = request.getParams();
            std::string image_name = params[0];
            long int token = stoi(params[params.size() - 1]);
            auto n = add_entry_svc(image_name, token);
            std::vector<std::string> reply_params;
            Message reply(MessageType::Reply, 1, request.getRPCId(), std::to_string(n), (size_t) 0, reply_params);
            serverConnector.send_no_ack(reply, sender_addr);
            break;
        }
        case 2: {
            // 2: remove_entry_svc(std::string image_name, int token);
            std::vector<std::string> params;
            params = request.getParams();
            std::string image_name = params[0];     // params[0] = image_name, params[size-1] = token
            long int token = stoi(params[params.size() - 1]);
            auto n = remove_entry_svc(image_name, token);
            std::vector<std::string> reply_params;
            Message reply(MessageType::Reply, 2, request.getRPCId(), std::to_string(n), (size_t) 0, reply_params);
            serverConnector.send_no_ack(reply, sender_addr);
            break;
        }
        case 3: {
            // 3: int get_client_addr_svc(std::string image_name, std::string &owner_addr, int &owner_port, int token);
            std::vector<std::string> params;
            params = request.getParams();
            std::string image_name = params[0];
            std::string owner_addr;
            uint16_t owner_port;
            long int token = stoi(params[params.size() - 1]);
            auto n = get_client_addr_svc(image_name, owner_addr, owner_port, token);
            std::vector<std::string> reply_params;
            reply_params.push_back(owner_addr);
            reply_params.push_back(std::to_string(owner_port));
            Message reply(MessageType::Reply, 3, request.getRPCId(), std::to_string(n), reply_params.size(),
                          reply_params);
            serverConnector.send_no_ack(reply, sender_addr);
            break;
        }

        case 4: {
            // 4: int retrieve_token_svc(std::string username, std::string password, int &token);
            std::vector<std::string> params;
            params = request.getParams();
            std::string username = params[0];
            std::string password = params[1];
            long int token;
            auto n = retrieve_token_svc(username, password, token);
            std::vector<std::string> reply_params;
            reply_params.push_back(std::to_string(token));
            Message reply(MessageType::Reply, 4, request.getRPCId(), std::to_string(n), reply_params.size(),
                          reply_params);
            serverConnector.send_no_ack(reply, sender_addr);
            break;
        }

        case 5: {
            //int check_viewImage_svc(std::string image_id, bool &can_view, int token);
            std::vector<std::string> params;
            params = request.getParams();
            std::string image_id = params[0];
            long int token = stoi(params[params.size() - 1]);
            bool can_view;
            auto n = check_viewImage_svc(image_id, can_view, token);
            std::vector<std::string> reply_params;
            reply_params.push_back(std::to_string(can_view));
            Message reply(MessageType::Reply, 5, request.getRPCId(), std::to_string(n), reply_params.size(),
                          reply_params);
            serverConnector.send_no_ack(reply, sender_addr);
            break;
        }

        default: {
            break;
        }
    }

}

//////////////////////////////////////////////////
//           Registry RPC Implementation        //
/////////////////////////////////////////////////
int Registry::view_imagelist_svc(std::vector<std::string> &image_container, long int token) {

    if (viewable_by_DB.empty())
        return -1;

    auto n = check_token(token); //retuns what

    for (int i = 0; i < viewable_by_DB.size(); i++) {
        if (viewable_by_DB[i].token == token)
            image_container.push_back(
                    (std::basic_string<char, std::char_traits<char>, std::allocator<char>> &&) viewable_by_DB[i].img_name);

    }

    return 0;
}

int Registry::add_entry_svc(std::string image_name, long int token) {

    auto n = check_token(token);





    return 0;
}

int Registry::remove_entry_svc(std::string image_name, long int token) {
    return 0;
}

int
Registry::get_client_addr_svc(std::string image_name, std::string &owner_addr, uint16_t &owner_port, long int token) {
    return 0;
}

int Registry::retrieve_token_svc(std::string username, std::string password, long int &token) {
    return 0;
}

int Registry::check_viewImage_svc(std::string image_id, bool &can_view, long int token) {
    return 0;
}

//should return n?
void Registry::load_DBs() {

    user usr;
    image img;
    viewable_by view;
    try {
        // Open a database file
        SQLite::Database db(
                "/home/farida/Documents/Dist-DB.db"); //location of database should be in constructor of Registry and used as a string?

        // Compile a SQL query, containing one parameter (index 1)
        SQLite::Statement usr_query(db, "SELECT * FROM user");
        SQLite::Statement img_query(db, "SELECT * FROM image");
        SQLite::Statement viewable_by_query(db, "SELECT * FROM viewable_by");

        // Loop to execute the query step by step, to get rows of result
        while (usr_query.executeStep()) {
            // Demonstrate how to get some typed column value
            usr.token = usr_query.getColumn(0);
            usr.username = usr_query.getColumn(1);
            usr.password = usr_query.getColumn(2);

            usr_DB.push_back(usr);
        }

        while (img_query.executeStep()) {
            // Demonstrate how to get some typed column value
            img.img_name = img_query.getColumn(0);
            img.owner_addr = img_query.getColumn(1);
            img.owner_port = img_query.getColumn(2);

            img_DB.push_back(img);
        }

        while (viewable_by_query.executeStep()) {

            view.img_name = viewable_by_query.getColumn(0);
            view.token = viewable_by_query.getColumn(1);

            viewable_by_DB.push_back(view);
        }


    }
    catch (std::exception &e) {
        std::cout << "exception: " << e.what() << std::endl;
    }

}

int Registry::check_token(long int token) {
    return 0;
}
