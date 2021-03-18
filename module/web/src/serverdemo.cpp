//
// Created by 金韬 on 2021/3/17.
//

#include "web.hpp"

const char *say_hello() {
    return "hello_world";
}

void startServer() {
    auto const address = boost::asio::ip::make_address("127.0.0.1");
    unsigned short port = 9000;
    std::string root = "~/";
    boost::asio::io_context ioc{1};
    tcp::acceptor acceptor{ioc, {address, port}};
    _web_HttpServer server(acceptor,root);
    server.start();
    ioc.run();
}


int main() {
    startServer();
    // for test interface
//    int serverId = _web_getServerId("127.0.0.1", 9000);
//    int res = _web_addUrlHandler(serverId, "POST", "/hello", say_hello);
//    if (res != ROK) {
//        return res;
//    }
//    return _web_startServe(serverId);
    return 0;
}