#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <streambuf>
#include "../common/Message/Message.h"
#include "Peer.h"
#include <netinet/in.h>
#include "images"

int Peer::download_image_svc(std::string image_name, long int token, std::vector<std::string>& reply_params)
{
    int n_token;
    // n = check_token(username, token)
    // n = 0    Success

    try {
        //try catch block
        //Opening image file to stream then storing it into a string.
        std::ifstream fin(image_name, std::ios::binary);
        std::string image_data;
        image_data.reserve(1000000);
        std::copy(std::istreambuf_iterator<char>(fin),
                  std::istreambuf_iterator<char>(),
                  std::back_insert_iterator(image_data));

        reply_params.push_back(image_data);
    }
    catch (const std::exception &e)
    {
        std::cout<<e.what()<<std::endl;
    }

}

long int Peer::retrieve_token_svc(std::string username, std::string password)
{

}
