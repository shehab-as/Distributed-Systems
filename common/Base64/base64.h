//
// Created by shadyf on 11/25/16.
//

#ifndef DISTRIBUTED_SYSTEMS_BASE64_H
#define DISTRIBUTED_SYSTEMS_BASE64_H

#include <string>

std::string base64_encode(unsigned char const *, unsigned int len);

std::string base64_decode(std::string const &s);

#endif //DISTRIBUTED_SYSTEMS_BASE64_H
