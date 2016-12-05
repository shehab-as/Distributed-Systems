#include <SQLiteCpp/Database.h>
#include <iostream>
#include <fstream>
#include "Registry.h"

int main() {
    //FOR FOREIGN KEY CONSTRAINT TO WORK, MUST WRITE -> PRAGMA foreign_keys = ON; IN SQLITE TERMINAL

    Registry Reg(NULL, 1234, 10, "/home/farida/Documents/Dist-DB.db");

    auto n = Reg.revoke_access_svc("maro.jpg", 548648251, "Zeyad");

    std::cout << "n: " << n <<std::endl;
}