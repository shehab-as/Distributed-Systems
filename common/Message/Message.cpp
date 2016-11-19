#include <string>
#include <iostream>
#include <sstream>
#include "Message.h"
//Message Type: Request OR Reply

//Attributes & Methods not needed in Exercise 1:
//operation
//rpc_id

// Empty Constructor
Message::Message() {

}

// Constructor
Message::Message(MessageType msg_type, unsigned long long op, unsigned long long p_rpc_id, std::string _return_val,
                 size_t p_message_size, std::vector<std::string> p_message)
        : message_type(msg_type), operation(op), parameters(p_message),
          parameters_size(p_message_size),
          rpc_id(p_rpc_id), sequence_id(0), return_val(_return_val)//, fragmented(0)
{

}


//Marshalled Constructor
Message::Message(char *marshalled_base64) {
    std::stringstream tokenizer(marshalled_base64);
    std::string token;

    tokenizer >> token;
    setMessageType((MessageType) std::stoi(token));

    tokenizer >> token;
    setOperation(std::stoull(token));

    tokenizer >> token;
    setRPCId(std::stoull(token));

    tokenizer >> token;
    setSeqId(std::stoull(token));

    tokenizer >> token;
    fragmented = std::stoi(token);

    tokenizer >> token;
    setReturnVal(token);

    tokenizer >> token;
    setParamsSize((size_t) std::stoi(token));

    for (int i = 0; i < getParamsSize(); i++) {
        tokenizer >> token;
        parameters.push_back(token);
    }
}

// Marshalled Message should be of the following format:
// "MessageType opeation rpc_id sequence_id fragmented return_val num_of_params param1 param2 ..."
std::string Message::marshal() {
    std::string marshalled_msg;
    marshalled_msg.append(getHeaders());
    marshalled_msg.append(getReturnVal() + " ");
    marshalled_msg.append(std::to_string(getParamsSize()) + " ");

    for (int i = 0; i < getParamsSize(); i++)
        marshalled_msg.append(parameters[i] + " ");

    return marshalled_msg;
}

unsigned long long Message::getOperation() { return operation; }

unsigned long long Message::getRPCId() { return rpc_id; }

std::vector<std::string> Message::getParams() { return parameters; }

size_t Message::getParamsSize() { return parameters_size; }

MessageType Message::getMessageType() { return message_type; }

void Message::setOperation(unsigned long long op) { operation = op; }

void Message::setMessage(std::vector<std::string> params, size_t params_size) {
    parameters = params;
    parameters_size = params_size;
}

void Message::setMessageType(MessageType msg_type) { message_type = msg_type; }

Message::~Message() {

}

void Message::setRPCId(unsigned long long _rpc_id) {
    rpc_id = _rpc_id;
}

void Message::setParamsSize(size_t _params_size) {
    parameters_size = _params_size;
}


unsigned long long Message::getSeqId() {
    return sequence_id;
}

void Message::setSeqId(unsigned long long _seq_id) {
    sequence_id = _seq_id;
}

void Message::setReturnVal(std::string _return_val) {
    return_val = _return_val;
}

std::string Message::getReturnVal() {
    return return_val;
}

int Message::getFrag() {
    return fragmented;
}

void Message::setFrag(int _frag) {
    fragmented = _frag;
}

std::string Message::getHeaders() {
    std::string headers;

    headers.append(std::to_string(int(getMessageType())) + " ");
    headers.append(std::to_string(getOperation()) + " ");
    headers.append(std::to_string(getRPCId()) + " ");
    headers.append(std::to_string(getSeqId()) + " ");
    headers.append(std::to_string(fragmented) + " ");

    return headers;
}
