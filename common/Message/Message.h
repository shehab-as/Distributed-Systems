#ifndef MESSAGE_H
#define MESSAGE_H

#include <cstddef>
#include <vector>

enum MessageType {
    Request=0, Reply=1
};

class Message {

private:
    MessageType message_type;
    int operation;      // RPC Operation
    std::vector<std::string>parameters;      // parameters, vector<void *> one for each param
    size_t parameters_size;
    int rpc_id;

public:
    Message();
    Message(MessageType msg_type,int op, int p_rpc_id, size_t p_message_size, std::vector<std::string> p_message);

    // Unmarshalling occurs here
    explicit Message(char *marshalled_base64);

    std::string marshal();

    int getOperation();

    int getRPCId();

    std::vector<std::string>getParams();

    size_t getParamsSize();

    MessageType getMessageType();

    void setOperation(int _operation);

    void setMessage(std::vector<std::string>params, size_t params_size);

    void setMessageType(MessageType message_type);

    void setParamsSize(size_t _params_size);

    void setRPCId(int _rpc_id);
    ~Message();

};

#endif // MESSAGE_H