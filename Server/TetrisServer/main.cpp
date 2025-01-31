#include "Server.h"
#include <iostream>
#include <string>
#include <locale>
#include <windows.h> // Windows API를 사용하기 위함

int main() {

    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    std::wcout.imbue(std::locale("")); // 또는 std::locale("ko_KR.UTF-8")


    auto server = std::make_shared<Server>();




    server->SetMessageReceivedCallback(
        [server](const std::string& message, boost::asio::ip::tcp::socket* senderSocket) {
        std::cout << "받은 메시지: " << message << std::endl;

        // 클라이언트에게 받은 메시지를 브로드캐스트
        server->Broadcast(message);
    }
    );

    unsigned short port = 12345; // 서버가 열 포트 번호 설정

    if (!server->InitializeAsServer(port)) {
        std::cerr << "서버 초기화에 실패했습니다." << std::endl;
        return 1;
    }

    std::wcout << L"서버가 실행 중입니다. 종료하려면 Enter 키를 누르세요." << std::endl;
    std::wcin.get(); // 사용자가 Enter 키를 누를 때까지 대기

    server->Stop();
    std::wcout << "서버가 종료되었습니다." << std::endl;

    return 0;
}
