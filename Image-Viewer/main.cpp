#include "mainwindow.h"
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();

//    using namespace std;
//    CM client(NULL, 0);
//    std::ifstream t("/home/shadyf/Pictures/Desktop Wallpapers/pcrlFXm.jpg", ios::binary);
//    std::string str;
//
//    t.seekg(0, std::ios::end);
//    str.reserve(t.tellg());
//    t.seekg(0, std::ios::beg);
//
//    str.assign((std::istreambuf_iterator<char>(t)),
//               std::istreambuf_iterator<char>());
//
//    std::vector<std::string> v{str};
////    cout << base64_encode((const unsigned char *) str.c_str(), str.size()) << std::endl;
//    Message request(MessageType::Request, 0, 0, "null", v.size(), v);
//    std::cout << "Request Size: " << request.marshal().size() << std::endl;
////    std::cout << request.marshal() << std::endl;
//    Message reply;
//    int bytes_read = client.send_with_ack(request, reply, 1000, 10, (char *) "192.168.1.100", 1234);
//    if (bytes_read >= 0) {
//        std::cout << "Reply size: " << reply.marshal().size() << std::endl;
//        std::cout << reply.marshal() << std::endl;
//    }
}
