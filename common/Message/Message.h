#ifndef MESSAGE_H
#define MESSAGE_H

#include <cstddef>
#include <vector>
#include <string>
#include <sstream>
#include "../Base64/base64.h"

enum MessageType {
    Request = 0, Reply = 1, Ack = 2
};

class Header {
public:
    MessageType message_type;

    unsigned long long operation;      // RPC Operation
    unsigned long long rpc_id;         // RPC ID
    unsigned long long sequence_id;    // Sequence/Packet ID

    // 0 indicates no fragmentation
    // 1 indicates packet is part of a fragmented message
    // -1 indicates packet is the last in a sequence of fragmented packets
    int fragmented;

    Header() {}

    Header(MessageType _msg_type, unsigned long long _op, unsigned long long _rpc_id, int _fragmented) :
            message_type(_msg_type), operation(_op), rpc_id(_rpc_id), fragmented(_fragmented), sequence_id(0) {}

    Header(char *marshalled_header) {
        // Decode base64 header
//        std::string decoded_header = base64_decode(marshalled_header);

        // Create stringstream from decoded payload
        std::stringstream tokenizer(marshalled_header);
        std::string token;

        tokenizer >> token;
        message_type = (MessageType) std::stoi(token);

        tokenizer >> token;
        operation = std::stoull(token);

        tokenizer >> token;
        rpc_id = std::stoull(token);

        tokenizer >> token;
        sequence_id = std::stoull(token);

        tokenizer >> token;
        fragmented = std::stoi(token);
    }

    ~Header() {}

    std::string str() {
        std::string headers;

        headers.append(std::to_string(int(message_type)) + " ");
        headers.append(std::to_string(operation) + " ");
        headers.append(std::to_string(rpc_id) + " ");
        headers.append(std::to_string(sequence_id) + " ");
        headers.append(std::to_string(fragmented) + " ");

        // Encode to base64
//        headers = base64_encode((const unsigned char *) headers.c_str(), (unsigned int) headers.size());

        return headers;
    }
};

class Payload {
public:
    std::string return_val;     // Holds return value of called RPC method, Should be 0 in case Message is a request and not a reply
    size_t parameters_size;     // Contains the number of RPC parameters
    std::vector<std::string> parameters;        // Holds all the RPC paramters
    bool fragmented;

    Payload() {}

    Payload(std::string _return_val, size_t params_size, std::vector<std::string> params, bool _fragmented) :
            return_val(_return_val), parameters_size(params_size), parameters(params), fragmented(_fragmented) {}

    Payload(char *marshalled_payload, bool _fragmented) : fragmented(_fragmented) {
        // Decode base64 payload
//        std::string decoded_payload = base64_decode(marshalled_payload);

        // Create stringstream from decoded payload
        std::stringstream tokenizer(marshalled_payload);
        std::string token;

        for (int i = 0; i < 5; i++)
            tokenizer >> token;

        if (!fragmented) {

            tokenizer >> token;
            return_val = token;

            tokenizer >> token;
            parameters_size = (size_t) std::stoi(token);

            for (int i = 0; i < parameters_size; i++) {
                tokenizer >> token;
                parameters.push_back(base64_decode(token));
            }
        } else {
            parameters_size = 0;
            while (tokenizer.good()) {
                tokenizer >> token;
                parameters_size++;
                parameters.push_back(base64_decode(token));
            }
        }
    }

    ~Payload() {}

    std::string str() {
        std::string payload_str;
        if (!fragmented) {
            payload_str.append(return_val + " ");
            payload_str.append(std::to_string(parameters_size) + " ");
        }

        for (int i = 0; i < parameters_size - 1; i++)
            payload_str.append(base64_encode((const unsigned char *) parameters[i].c_str(),
                                             (unsigned int) parameters[i].size()) + " ");
        payload_str.append(
                base64_encode((const unsigned char *) parameters[parameters_size - 1].c_str(),
                              (unsigned int) parameters[parameters_size - 1].size())
        );

        return payload_str;
    }
};

// TODO: Remove p_message_size from constructor, calculate it based on input vector p_message
class Message {
protected:
    Header header;
    Payload payload;

public:
    Message();

    Message(MessageType msg_type, unsigned long long op, unsigned long long p_rpc_id, std::string _return_val,
            size_t p_message_size, std::vector<std::string> p_message);

    Message(Header _header, Payload _payload) : header(_header), payload(_payload) {}

    explicit Message(char *marshalled_base64);      // Unmarshalling Constructor

    ~Message();

    std::string marshal();

    // Getters
    unsigned long long getOperation();

    unsigned long long getRPCId();

    unsigned long long getSeqId();

    int getFrag();

    std::string getReturnVal();

    std::vector<std::string> getParams();

    size_t getParamsSize();

    MessageType getMessageType();

    Header getHeader() { return header; }

    Payload getPayload() { return payload; }

    // Setters
    void setOperation(unsigned long long _operation);

    void setMessage(std::vector<std::string> params, size_t params_size);

    void setMessageType(MessageType message_type);

    void setParamsSize(size_t _params_size);

    void setRPCId(unsigned long long _rpc_id);

    void setSeqId(unsigned long long _seq_id);

    void setReturnVal(std::string _return_val);

    void setFrag(int _frag);

};

#endif // MESSAGE_H