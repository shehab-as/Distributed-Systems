#ifndef MESSAGE_H
#define MESSAGE_H

#include <cstddef>

enum MessageType {
    Request, Reply
};

class Message {

private:
    MessageType message_type;
    int operation;      // RPC Operation
    void *message;      // parameters, vector<void *> one for each param
    size_t message_size;
    int rpc_id;

public:
    Message(int operation, void *p_message, size_t p_message_size, int p_rpc_id);

    // Unmarshalling occurs here
    explicit Message(char *marshalled_base64);

    std::string marshal();

    int getOperation();

    int getRPCId();

    void *getMessage();

    size_t getMessageSize();

    MessageType getMessageType();

    void setOperation(int _operation);

    void setMessage(void *message, size_t message_size);

    void setMessageType(MessageType message_type);

    ~Message();

};

#endif // MESSAGE_H