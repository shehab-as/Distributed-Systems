#ifndef MESSAGE_H
#define MESSAGE_H

#include <cstddef>
#include <vector>

enum MessageType {
    Request = 0, Reply = 1
};

class Message {

private:
    MessageType message_type;

    unsigned long long operation;      // RPC Operation
    unsigned long long rpc_id;         // RPC ID
    unsigned long long sequence_id;    // Sequence/Packet ID

//    // 0 indicates no fragmentation
//    // 1 indicates packet is part of a fragmented message
//    // -1 indicates packet is the last in a sequence of fragmented packets
//    int fragmented;

    std::vector<std::string> parameters;
    size_t parameters_size;

public:
    Message();

    Message(MessageType msg_type, unsigned long long op, unsigned long long p_rpc_id, size_t p_message_size,
            std::vector<std::string> p_message);

    // Unmarshalling occurs here
    explicit Message(char *marshalled_base64);

    std::string marshal();

    unsigned long long getOperation();

    unsigned long long getRPCId();

    unsigned long long getSeqId();

    std::vector<std::string> getParams();

    size_t getParamsSize();

    MessageType getMessageType();

    void setOperation(unsigned long long _operation);

    void setMessage(std::vector<std::string> params, size_t params_size);

    void setMessageType(MessageType message_type);

    void setParamsSize(size_t _params_size);

    void setRPCId(unsigned long long _rpc_id);

    void setSeqId(unsigned long long _seq_id);

    ~Message();

};

#endif // MESSAGE_H