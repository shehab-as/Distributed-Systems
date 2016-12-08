#include <thread>
#include <string>
#include <functional>
#include <vector>
#include <SQLiteCpp/Database.h>
#include <iostream>
#include "Registry.h"
#include "../common/Message/Message.h"

Registry::Registry(char *_listen_hostname, uint16_t _listen_port, int num_of_workers, std::string DB_location)
        : serverConnector(
        _listen_hostname, _listen_port) {

    pathLocation = DB_location;
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
    // The core of the regiretristry
    // Each worker thread should run this method
    sockaddr_in sender_addr;
    sender_addr.sin_family = 2;

    while (true) {
        Message recv_message = Message();
        ssize_t bytes_read = serverConnector.recv_with_block(recv_message, MessageType::Request, sender_addr);
        handleRequest(recv_message, sender_addr);
    }
}

#pragma clang diagnostic pop/home/zeyad

void Registry::handleRequest(Message request, sockaddr_in sender_addr) {
    // THE GREAT SWITCH
    // Check which RPC method/operation was called and send reply accordingly
    switch (request.getOperation()) {
        case VIEW_IMAGELIST: {
            std::vector<std::string> image_container, params;

            params = request.getParams();
            long int token = stoi(params[0]);

            auto n = view_imagelist_svc(image_container, token);

            Message reply(MessageType::Reply, request.getOperation(), request.getRPCId(), std::to_string(n),
                          image_container.size(), image_container);

            serverConnector.send_no_ack(reply, sender_addr);
            break;
        }

        case ADD_ENTRY: {
            std::vector<std::string> params, reply_params;  // params[0] = image_name, params[size-1] = token
            params = request.getParams();
            std::string image_name = params[0];
            long int token = stoi(params[1]);

            auto n = add_entry_svc(image_name, token, sender_addr);

            Message reply(MessageType::Reply, 1, request.getRPCId(), std::to_string(n), (size_t) 0, reply_params);

            serverConnector.send_no_ack(reply, sender_addr);
            break;
        }

        case REMOVE_ENTRY: {
            std::vector<std::string> params, reply_params;

            params = request.getParams();
            std::string image_name = params[0];     // params[0] = image_name, params[size-1] = token
            long int token = stoi(params[params.size() - 1]);

            auto n = remove_entry_svc(image_name, token);

            Message reply(MessageType::Reply, 2, request.getRPCId(), std::to_string(n), (size_t) 0, reply_params);

            serverConnector.send_no_ack(reply, sender_addr);
            break;
        }
        case GET_CLIENT_ADDR: {
            std::vector<std::string> params, reply_params;

            params = request.getParams();
            std::string image_name = params[0];
            std::string owner_addr;
            uint16_t owner_port;
            long int token = stoi(params[params.size() - 1]);

            auto n = get_client_addr_svc(image_name, owner_addr, owner_port, token);

            reply_params.push_back(owner_addr);
            reply_params.push_back(std::to_string(owner_port));
            Message reply(MessageType::Reply, 3, request.getRPCId(), std::to_string(n), reply_params.size(),
                          reply_params);

            serverConnector.send_no_ack(reply, sender_addr);
            break;
        }

        case RETRIEVE_TOKEN: {
            std::vector<std::string> params, reply_params;
            long int token;

            params = request.getParams();
            std::string username = params[0];
            std::string password = params[1];

            auto n = retrieve_token_svc(username, password, token);

            reply_params.push_back(std::to_string(token));
            Message reply(MessageType::Reply, 4, request.getRPCId(), std::to_string(n), reply_params.size(),
                          reply_params);

            serverConnector.send_no_ack(reply, sender_addr);
            break;
        }

        case CHECK_VIEWIMAGE: {
            std::vector<std::string> params, reply_params;
            bool can_view;

            params = request.getParams();
            std::string image_id = params[0];
            long int token = stoi(params[params.size() - 1]);

            auto n = check_viewImage_svc(image_id, can_view, token);

            reply_params.push_back(std::to_string(can_view));
            Message reply(MessageType::Reply, 5, request.getRPCId(), std::to_string(n), reply_params.size(),
                          reply_params);

            serverConnector.send_no_ack(reply, sender_addr);
            break;
        }

        case CHECK_TOKEN: {
            std::vector<std::string> params, reply_params;

            params = request.getParams();
            long int token = stoi(params[0]);
            auto n = check_token_svc(token);

            reply_params.push_back(std::to_string(n));
            Message reply(MessageType::Reply, 6, request.getRPCId(), std::to_string(n), reply_params.size(),
                          reply_params);

            serverConnector.send_no_ack(reply, sender_addr);
            break;

        }

        case SET_IMAGE_VIEWABLE_BY: {

            std::vector<std::string> params, reply_params;

            params = request.getParams();
            std::string image_id = params[0];
            long int user_token = stoi(params[1]);
            std::string allowed_user = params[2];

            auto n = set_image_viewable_by_svc(image_id, user_token, allowed_user);

            Message reply(MessageType::Reply, 7, request.getRPCId(), std::to_string(n), reply_params.size(),
                          reply_params);

            serverConnector.send_no_ack(reply, sender_addr);
            break;

        }

        case REVOKE_ACCESS: {

            std::vector<std::string> params, reply_params;

            params = request.getParams();
            std::string image_id = params[0];
            long int user_token = stoi(params[1]);
            std::string user_to_revoke = params[2];

            auto n = revoke_access_svc(image_id, user_token, user_to_revoke);

            Message reply(MessageType::Reply, 8, request.getRPCId(), std::to_string(n), reply_params.size(),
                          reply_params);

            serverConnector.send_no_ack(reply, sender_addr);
            break;

        }

        case UPDATE_VIEWS: {
            std::vector<std::string> params, reply_params;

            params = request.getParams();
            std::string image_name = params[0];
            long int user_token = stoi(params[1]);
            int views = stoi(params[2]);

            auto n = update_views_svc(image_name, user_token, views);

            Message reply(MessageType::Reply, 9, request.getRPCId(), std::to_string(n), reply_params.size(),
                            reply_params);

            serverConnector.send_no_ack(reply, sender_addr);
            break;
        }

        case RETRIEVE_UPDATED_VIEWS: {

            std::vector<std::string> params, reply_params;
            long int token;

            params = request.getParams();
            std::string image_name = params[0];
            long int user_token = stoi(params[1]);
            int views = stoi(params[2]);

            auto n = retrieve_updated_views_svc(image_name, user_token, views);

            Message reply(MessageType::Reply, 10, request.getRPCId(), std::to_string(n), reply_params.size(),
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
int Registry::view_imagelist_svc(std::vector<std::string> &image_container, long int token)
{
    if (viewable_by_DB.empty())
        return -1;
    else
        update_viewable_by();

    auto n = check_token_svc(token);
    update_imageList();


    if (n == 0) {
        for (int i = 0; i < viewable_by_DB.size(); i++) {
            std::string str;
            if (viewable_by_DB[i].token == token) {
                str = viewable_by_DB[i].img_name;
                image_container.push_back(str);
            }

        }
        return 0;
    }
    return -1;

}


//return 0 if a new image is inserted else -1
// needs testing
int Registry::add_entry_svc(std::string image_name, long int token, sockaddr_in owner) {

    std::string owner_addr = std::to_string(owner.sin_addr.s_addr);
    int owner_port = owner.sin_port;
    auto n = check_token_svc(token);


    //if token is correct, insert imagename, owner_addr, owner_port

    update_imageList();

    for (int i =0; i < img_DB.size(); i++)
        if (img_DB[i].img_name==image_name)
            return 0;

    if (n == 0) {
        try {
            // Open a database file
            SQLite::Database db(pathLocation, SQLite::OPEN_READWRITE, 0, "");

            SQLite::Statement img_query(db,
                                        "INSERT INTO image (Img_name, owner_addr, owner_port,token) VALUES ( '" +
                                        image_name +
                                        "', '" + std::string(owner_addr) + "', '" + std::to_string(owner_port) +
                                        "' ,'" + std::to_string(token) +
                                        "');");
            int noRowsModified = img_query.exec();
            return 0;

        }
        catch (std::exception &e) {
            std::cout << "add_entry_svc exception: " << e.what() << std::endl;
        }

    } else {
        return -1;
    }

}


int Registry::remove_entry_svc(std::string image_name, long int token) {

    auto n = check_token_svc(token);
    bool check_owner = false;

    for (int i = 0; i < img_DB.size(); i++)
        if (img_DB[i].img_name == image_name)
            if (img_DB[i].token == token)
                check_owner = true;

//    const char const_char_image_name = image_name;
    if (n == 0 && check_owner) {
        try {
            SQLite::Database db(pathLocation, SQLite::OPEN_READWRITE, 0, "");
            SQLite::Statement view_query(db, "DELETE FROM viewable_by WHERE img_name ='" + image_name + "';");
            int noRowsModified1 = view_query.exec();
            SQLite::Statement img_query(db, "DELETE FROM image WHERE Img_name ='" + image_name + "' and token= " +
                                            std::to_string(token) + " ;");
            int noRowsModified = img_query.exec();
            update_viewable_by();
            update_imageList();
            return 0;
        }
        catch (std::exception &e) {
            std::cout << "remove_entry_svc exception: " << e.what() << std::endl;
        }

    } else {
        return -1;
    }
}


int
Registry::get_client_addr_svc(std::string image_name, std::string &owner_addr, uint16_t &owner_port, long int token) {

    auto n = check_token_svc(token);

    if (n == 0)
        for (int i = 0; i < img_DB.size(); i++)
            if (img_DB[i].img_name == image_name) {
                owner_addr = img_DB[i].owner_addr;
                owner_port = (uint16_t) img_DB[i].owner_port;
                return 0;
            }

    return -1;
}

// return 0 if token is retrieved else return -1 if a new token is created
//and a new username , password will be created in database
int Registry::retrieve_token_svc(std::string username, std::string password, long int &token) {

    update_users();

    for (int i = 0; i < usr_DB.size(); i++)
        if (usr_DB[i].username == username)
            if (usr_DB[i].password == password) {
                token = usr_DB[i].token;
                return 0;
            } else
                return -1;


    std::string to_hash = std::string(username) + std::string(password);
    std::hash<std::string> str_hash;
    size_t token_size_t = str_hash(to_hash);
    token = (int) token_size_t;

    SQLite::Database db(pathLocation, SQLite::OPEN_READWRITE, 0, "");
    SQLite::Statement usr_query(db, "INSERT INTO user (token, username, password) VALUES ( '" + std::to_string(token) +
                                    "', '" + username + "', '" + password + "');");
    int noRowsModified = usr_query.exec();

    return 1;
}

// return 0 if user can view image , -1 otherwise
int Registry::check_viewImage_svc(std::string image_id, bool &can_view, long int token) {

    update_viewable_by();

    can_view = false;
    for (int i = 0; i < viewable_by_DB.size(); i++)
        if (viewable_by_DB[i].img_name == image_id && viewable_by_DB[i].token == token) {

            can_view = true;
            return 0;
        }

    return -1;
}

int Registry::set_image_viewable_by_svc(std::string image_id, long int user_token, std::string allowed_user) {

    update_imageList();
    long int peer_token = fetch_token(allowed_user);

    update_viewable_by();

    for (int i = 0; i < viewable_by_DB.size(); i++)
        if(viewable_by_DB[i].img_name == image_id && viewable_by_DB[i].token == peer_token)
            return 0;


    for (int i = 0; i < img_DB.size(); i++)
        if (img_DB[i].token == user_token && img_DB[i].img_name == image_id) {
            try {
                SQLite::Database db(pathLocation, SQLite::OPEN_READWRITE, 0, "");
                SQLite::Statement viewable_by_query(db, "INSERT INTO viewable_by (img_name, token) VALUES ( '" +
                                                        image_id + "', '" + std::to_string(peer_token) + "');");
                int noRowsModified = viewable_by_query.exec();
                return 0;
            }
            catch (std::exception &e) {
                std::cout << "set_image_viewable_by_svc exception: " << e.what() << std::endl;
            }
        }
    return -1;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

int Registry::update_views_svc(std::string image_name, long int user_token, int& views_val)
{

    //TO DO..
}

int Registry::retrieve_updated_views_svc(std::string image_name, long int user_token, int& views)
{

    //TO DO..
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//


//should return n?
void Registry::load_DBs() {

    user usr;
    image img;
    viewable_by view;
    try {
        // Open a database file
        SQLite::Database db(
                pathLocation); //location of database should be in constructor of Registry and used as a string?

        // Compile a SQL query, containing one parameter (index 1)
        SQLite::Statement usr_query(db, "SELECT * FROM user");
        SQLite::Statement img_query(db, "SELECT * FROM image");
        SQLite::Statement viewable_by_query(db, "SELECT * FROM viewable_by");

        //  Loop to execute the query step by step, to get rows of result
        while (usr_query.executeStep()) {
            // Demonstrate how to get some typed column value
            usr.token = usr_query.getColumn(0);
            usr.username = usr_query.getColumn(1).getString();
            usr.password = usr_query.getColumn(2).getString();

            usr_DB.push_back(usr);
        }

        while (img_query.executeStep()) {
            // Demonstrate how to get some typed column value
            img.img_name = img_query.getColumn(0).getString();
            img.owner_addr = img_query.getColumn(1).getString();
            img.owner_port = img_query.getColumn(2);
            img.token = img_query.getColumn(3);

            img_DB.push_back(img);
        }

        while (viewable_by_query.executeStep()) {

            view.img_name = viewable_by_query.getColumn(0).getString();
            view.token = viewable_by_query.getColumn(1);

            viewable_by_DB.push_back(view);
        }


    }
    catch (std::exception &e) {
        std::cout << "load_DBs exception: " << e.what() << std::endl;
    }

}

//check the token in the user database
//if found return 0 else -1
int Registry::check_token_svc(long int token) {

    update_users();


    for (int i = 0; i < usr_DB.size(); i++) {
        if (usr_DB[i].token == token) {
            return 0;
        }
    }
    return -1;
}

//updating usr_db vector
void Registry::update_users() {

    usr_DB.clear();
    usr_DB.resize(0);
    user usr;
    try {
        SQLite::Database db(pathLocation);
        SQLite::Statement usr_query(db, "SELECT * FROM user");
        while (usr_query.executeStep()) {
            // Demonstrate how to get some typed column value
            usr.token = usr_query.getColumn(0);
            usr.username = usr_query.getColumn(1).getString();
            usr.password = usr_query.getColumn(2).getString();

            usr_DB.push_back(usr);
        }
    }
    catch (std::exception &e) {
        std::cout << " update_users exception: " << e.what() << std::endl;
    }


}

//updating img_db vector
void Registry::update_imageList() {
    img_DB.clear();
    img_DB.resize(0);
    image img;

    try {
        SQLite::Database db(pathLocation);
        SQLite::Statement img_query(db, "SELECT * FROM image");
        while (img_query.executeStep()) {
            // Demonstrate how to get some typed column value
            img.img_name = img_query.getColumn(0).getString();
            img.owner_addr = img_query.getColumn(1).getString();
            img.owner_port = img_query.getColumn(2);
            img.token = img_query.getColumn(3);

            img_DB.push_back(img);
        }
    }
    catch (std::exception &e) {
        std::cout << "exception: " << e.what() << std::endl;
    }
}

//updating viewable_by vector
void Registry::update_viewable_by() {
    viewable_by_DB.clear();
    viewable_by_DB.resize(0);
    viewable_by view;

    try {
        SQLite::Database db(pathLocation);
        SQLite::Statement viewable_by_query(db, "SELECT * FROM viewable_by");
        while (viewable_by_query.executeStep()) {

            view.img_name = viewable_by_query.getColumn(0).getString();
            view.token = viewable_by_query.getColumn(1);

            viewable_by_DB.push_back(view);
        }
    }
    catch (std::exception &e) {
        std::cout << "exception: " << e.what() << std::endl;
    }
}

long int Registry::fetch_token(std::string username) {
    for (int i = 0; i < usr_DB.size(); i++)
        if (usr_DB[i].username == username)
            return usr_DB[i].token;

    return -1;
}

int Registry::revoke_access_svc(std::string image_id, long int user_token, std::string user_to_revoke) {

    update_viewable_by();
    auto n = check_token_svc(user_token);
    bool check_owner = false;

    if (n == 0) {
        for (int i = 0; i < img_DB.size(); i++)     //check that the user is the owner of the image
            if (img_DB[i].img_name == image_id)
                if (img_DB[i].token == user_token)
                    check_owner = true;
    } else
        return -1;

    long int peer_token = fetch_token(user_to_revoke);

    for (int i = 0; i < viewable_by_DB.size(); i++)
        if ((viewable_by_DB[i].token == peer_token && viewable_by_DB[i].img_name == image_id) && check_owner)
            try {
                SQLite::Database db(pathLocation, SQLite::OPEN_READWRITE, 0, "");

                SQLite::Statement viewable_by_query(db, "DELETE FROM viewable_by WHERE img_name ='" + image_id +
                                                        "' and token= " + std::to_string(peer_token) + ";");
                int noRowsModified = viewable_by_query.exec();
                update_viewable_by();
                return 0;

            }
            catch (std::exception &e) {
                std::cout << "revoke_access_svc exception: " << e.what() << std::endl;
            }

    return -1;


    //return 0;
}
