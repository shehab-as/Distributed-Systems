#include <string>
#include <iostream>
#include <sstream>
#include "Message.h"
//Message Type: Request OR Reply

//Attributes & Methods not needed in Exercise 1:
//operation
//rpc_id

//Constructor
Message::Message(int op, std::vector<std::string> p_message, size_t p_message_size, int p_rpc_id)
        : operation(op), parameters(p_message),
          parameters_size(p_message_size),
          rpc_id(p_rpc_id) {

}

//Marshalled Constructor
Message::Message(char *marshalled_base64) {
    std::stringstream tokenizer(marshalled_base64);
    std::string token;

    tokenizer >> token;
    setOperation(std::stoi(token));

    tokenizer >> token;
    setMessageType((MessageType) std::stoi(token));

    tokenizer >> token;
    setRPCId(std::stoi(token));

    tokenizer >> token;
    setParamsSize((size_t) std::stoi(token));

    for (int i = 0; i < getParamsSize(); i++) {
        tokenizer >> token;
        parameters.push_back(token);
    }
}

// Marshalled Message should be of the following format:
// "operation MessageType rpc_id num_of_params param1 param2 ... message_size"
std::string Message::marshal() {
    std::string marshalled_msg;
    marshalled_msg.append(std::to_string(getOperation()) + " ");
    marshalled_msg.append(std::to_string(getMessageType()) + " ");
    marshalled_msg.append(std::to_string(getRPCId()) + " ");
    marshalled_msg.append(std::to_string(getParamsSize()) + " ");

    for (int i = 0; i < getParamsSize(); i++)
        marshalled_msg.append(parameters[i] + " ");

    return marshalled_msg;
}

int Message::getOperation() { return operation; }

int Message::getRPCId() { return rpc_id; }

std::vector<std::string> Message::getParams() { return parameters; }

size_t Message::getParamsSize() { return parameters_size; }

MessageType Message::getMessageType() { return message_type; }

void Message::setOperation(int op) { operation = op; }

void Message::setMessage(std::vector<std::string> params, size_t params_size) {
    parameters = params;
    parameters_size = params_size;
}

void Message::setMessageType(MessageType msg_type) { message_type = msg_type; }

Message::~Message() {

}

void Message::setRPCId(int _rpc_id) {
    rpc_id = _rpc_id;
}

void Message::setParamsSize(size_t _params_size) {
    parameters_size = _params_size;
}
