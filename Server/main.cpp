#include "Server.h"
#include <iostream>
#include <string>
#include <locale>
#include <windows.h>

int main() {
    // 콘솔 문자 인코딩 UTF‑8 설정
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    std::wcout.imbue(std::locale("ko_KR.UTF-8"));

    // Server 인스턴스 생성 (shared_ptr를 사용하여 lifetime 관리)
    auto server = std::make_shared<Server>();

    // 메시지 수신 콜백 등록: 수신한 메시지를 출력하고, 모든 클라이언트에 브로드캐스트합니다.
    server->SetMessageReceivedCallback(
        [server](const std::string& message, boost::asio::ip::tcp::socket* senderSocket) {
        std::cout << "Accept Message: " << message << std::endl;
        // 받은 메시지를 모든 클라이언트에게 브로드캐스트합니다.
        server->Broadcast(message);
    }
    );

    unsigned short port = 12345; // 서버가 사용할 포트 번호
    if (!server->InitializeAsServer(port)) {
        std::cerr << "faile Initialize Server ." << std::endl;
        return EXIT_FAILURE;
    }

    std::wcout << L"Server Port: " << port
        << L" is proceeding. Press Enter to quit." << std::endl;
    std::wcin.get();

    // 서버 정리 및 종료
    server->Stop();
    std::wcout << L"Server is closed" << std::endl;

    return EXIT_SUCCESS;
}
