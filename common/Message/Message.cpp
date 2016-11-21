#include <string>
#include <iostream>
#include <sstream>
#include "Message.h"
//Message Type: Request OR Reply

//Attributes & Methods not needed in Exercise 1:
//operation
//rpc_id

// Empty Constructor
Message::Message() : payload(), header() {

}

// Constructor
Message::Message(MessageType msg_type, unsigned long long op, unsigned long long p_rpc_id, std::string _return_val,
                 size_t p_message_size, std::vector<std::string> p_message) :
        header(msg_type, op, p_rpc_id, 0), payload(_return_val, p_message_size, p_message, false) {}


//Marshalled Constructor
Message::Message(char *marshalled_base64) : header(marshalled_base64), payload(marshalled_base64, 0){ }

// Marshalled Message should be of the following format:
// "MessageType opeation rpc_id sequence_id fragmented return_val num_of_params param1 param2 ..."
std::string Message::marshal() {
    return header.str() + payload.str();
}

Message::~Message() {

}

unsigned long long Message::getOperation() { return header.operation; }

unsigned long long Message::getRPCId() { return header.rpc_id; }

std::vector<std::string> Message::getParams() { return payload.parameters; }

size_t Message::getParamsSize() { return payload.parameters_size; }

MessageType Message::getMessageType() { return header.message_type; }

void Message::setOperation(unsigned long long op) { header.operation = op; }

void Message::setMessage(std::vector<std::string> params, size_t params_size) {
    payload.parameters = params;
    payload.parameters_size = params_size;
}

void Message::setMessageType(MessageType msg_type) { header.message_type = msg_type; }

void Message::setRPCId(unsigned long long _rpc_id) {
    header.rpc_id = _rpc_id;
}

void Message::setParamsSize(size_t _params_size) {
    payload.parameters_size = _params_size;
}


unsigned long long Message::getSeqId() {
    return header.sequence_id;
}

void Message::setSeqId(unsigned long long _seq_id) {
    header.sequence_id = _seq_id;
}

void Message::setReturnVal(std::string _return_val) {
    payload.return_val = _return_val;
}

std::string Message::getReturnVal() {
    return payload.return_val;
}

int Message::getFrag() {
    return header.fragmented;
}

void Message::setFrag(int _frag) {
    header.fragmented = _frag;
}