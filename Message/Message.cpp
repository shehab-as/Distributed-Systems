#include "MESSAGE_H"

//Message Type: Request OR Reply

//Attributes & Methods not needed in Exercise 1:
	//operation
	//rpc_id

//Constructor
Message::Message(int op, void *p_message, size_t p_message_size, int p_rpc_id)
{
	this.operation = op;
	this.message = &p_message;
	this.message_size = p_message_size;
	this.rpc_id = p_rpc_id;
}

//Marshalled Constructor
Message::Message(char * marshalled_base64)
{	}

char* Message::marshal()
{
//	//To do.
//	char *Marshy;
//
//
//	return Marshy;
}

int Message::getOperation()
{	return operation;	}

int Message::getRPCId()
{	return rpc_id;	}

void* Message::getMessage()
{	return message;	}

size_t Message::getMessageSize()
{	return message_size;	}

MessageType Message::getMessageType()
{	return message_type;	}

void Message::setOperation(int op)
{	this.operation = operation;	}

void Message::setMessage(void *msg, size_t msg_size)
{
	this->message = msg;
	this.message_size = msg_size;
}

void Message::setMessageType(MessageType msg_type)
{	this.message_type = msg_type;	}

Message::~Message()
{
	delete message;
}