# **Console TUI Library**  
**(Cross-Platform Ready, Windows Implementation Included)**  

> **최초 작성**: 2025-02-24 (BeomJin Na)
> **마지막 업데이트**: 2025-02-24 (BeomJin Na)

이 문서는 **텍스트 기반 UI(TUI) 라이브러리**인 **Console TUI Library**에 대한 안내입니다.  
현재는 **Windows** 콘솔 환경에서 동작하는 구현체를 포함하고 있으나, **Unix** 계열 등 **다른 플랫폼에도 확장** 가능하도록 설계되어 있습니다.

---

## **1. 라이브러리 개념 및 특징**

1. **플랫폼 독립성 (추상화 구조)**  
   - Windows 환경에서는 `ConsoleWin32` 클래스를 통해 실제 콘솔 API(`WriteConsoleOutputW` 등)를 호출합니다.  
   - 다른 플랫폼(Unix 등)을 지원하려면 `ConsoleUnix` 등의 구현체를 추가할 수 있습니다.  
   - 상위 레벨(사용자 코드)에서는 **“Console”** 인터페이스만 사용하므로, **크로스 플랫폼** 지원이 용이합니다.

2. **Dirty Flag를 통한 부분 업데이트 & 입력 보존**  
   - 화면 중간에 입력 버퍼를 배치(예: 별도 `Panel`)해도, **입력 도중** `Refresh()`가 호출되어 화면이 덮어써지지 않습니다.  
   - **Dirty Flag**는 단순 최적화뿐 아니라, **부분적 UI 갱신** 및 **중간 입력 보존**에 핵심 역할을 합니다.

3. **Panel 기반 레이아웃**  
   - 하나의 콘솔 화면을 여러 `Panel`로 분할해, 독립적인 UI 영역(예: 채팅창, 상태창, 입력창 등)을 관리합니다.  
   - 최상단 Console이 `Panel`들을 합성하여 최종 화면을 그립니다.  
   - **마지막으로 생성된 패널이 최상단 레이어**로, 겹치는 영역에서는 나중에 추가된 패널이 우선 출력됩니다.

4. **메모리 관리(스마트 포인터)**  
   - `Console`, `Panel` 객체 모두 **`std::shared_ptr`**로 관리됩니다.  
   - 라이브러리 사용자도 `shared_ptr<Console>`에서 `AddPanel()`을 호출해 `shared_ptr<Panel>`을 받으며, 객체 생명주기를 쉽게 제어할 수 있습니다.

5. **사용 편의**  
   - 헤더 파일 정리: **`#include "Consoles.h"`** 한 번으로 라이브러리 전체를 사용할 수 있습니다.  
   - 키 입력이나 마우스 이벤트는 OS별로 다르므로(Windows에서는 콘솔 입력 이벤트, Unix에서는 ncurses 등), 필요하다면 별도의 입력 처리 로직을 연동할 수 있습니다.
   - 현재 입력 관력 로직은 다루고 있지 않습니다. (출력 관련 메소드만 제공)

---

## **2. 주요 클래스 개요**

### **(1) `Console`**

- **역할**: 콘솔 전체 화면(버퍼) 관리, 여러 `Panel`을 등록/제거하고 최종 화면을 출력.  
- **생성**: `auto console = std::make_shared<Console>(height, width);`  
- **주요 메서드**  

| 메서드                       | 설명                                                   |
|-----------------------------|--------------------------------------------------------|
| **`AddPanel`**(top, left, h, w) | 새 패널을 생성해 해당 좌표(top-left)에 배치 (shared_ptr로 반환) |
| **`RemovePanel`**(panel)       | 특정 패널을 제거                                     |
| **`Refresh`**()                | Dirty Flag를 참조하여 변경된 부분만 콘솔에 출력       |
| **`Clear`**()                  | 전체 패널 삭제 & 콘솔 화면 초기화                    |

### **(2) `Panel`**

- **역할**: 개별 UI 구성 요소(윈도우, 채팅창, 입력창 등)를 담당하는 사각형 영역.  
- **생성**: `auto panel = console->AddPanel(top, left, height, width);`  
- **주요 메서드**  

| 메서드                             | 설명                                                              |
|-----------------------------------|-------------------------------------------------------------------|
| **`PrintAt`**(row, col, text)     | 해당 좌표에 텍스트 출력                                           |
| **`PrintLines`**(row, text)       | 여러 줄 텍스트를 연속 출력 (줄바꿈 포함)                          |
| **`SetCell`**(row, col, ch)       | 개별 셀(문자, 색상) 설정                                          |
| **`Clear`**()                     | 패널 전체를 초기화하여 공백으로 설정 (Dirty Flag도 설정)          |
| **`Scroll`**(lines)               | 내용 위로 스크롤 (아래쪽 라인은 공백 처리)                        |
| **`AppendLines`**(text)           | 가장 아래 영역에 텍스트를 추가(위쪽으로 밀림)                     |
| **`DrawRectangle`**(rect, cell)   | 테두리 그리기(또는 내부까지 채우기)                               |

---

## **3. Dirty Flag 동작 방식**

### **(1) Dirty Cells**

- 각 패널(`Panel`)은 내부적으로 `mBuffer`(콘텐츠)와 `mDirtyCells`(bool/uint8_t 플래그) 배열을 관리합니다.  
- 내용이 변경되면 해당 위치의 Dirty Flag가 `true`로 설정됩니다.
- Dirty 플래그는 범용성을 위해 `std::vector<bool>` 대신 `std::vector<uint8_t>`를 사용하고 있습니다.
- https://learn.microsoft.com/en-us/cpp/standard-library/vector-bool-class
- https://isocpp.org/blog/2012/11/on-vectorbool

### **(2) Console 합성 및 출력**

1. **`Refresh()`** 호출 시:  
   - Console는 등록된 모든 `Panel`을 순회하여, Panel의 Dirty 영역을 Console 내부 버퍼에 합성합니다.  
   - 이 때 Console도 Dirty Flag를 갱신해, 최종적으로 어느 위치가 바뀌었는지 파악합니다.
2. **Windows 구현(ConsoleWin32.cpp)**  
   - Console가 저장한 Dirty 셀만 골라서 OS 콘솔 화면에 출력합니다.  
   - 연속된 행/열을 묶어 한 번에 쓰는 등 최적화를 수행합니다.

### **(3) 입력 보존**

- 화면 중간에 위치한 입력 패널도 Dirty Flag 덕분에 내용이 덮어써지지 않습니다.  
- 예를 들어 “입력” 중간에 `Refresh()`가 호출되어도, 입력 패널 영역은 Dirty Flag에 의해 제대로 유지됩니다.

---

## **4. 사용 예시**

```cpp
#include "Consoles.h" // Console TUI Library 헤더 (플랫폼별 구현 포함)

int main()
{
    // 1) 콘솔 생성 (30행 x 100열)
    auto console = std::make_shared<Console>(30, 100);

    // 2) 패널 생성 (채팅창: 0,0 ~ 20,100)
    auto chatPanel = console->AddPanel(0, 0, 20, 100);
    // 3) 패널 생성 (입력창: 21,0 ~ 2,100)
    auto inputPanel = console->AddPanel(21, 0, 2, 100);

    // 채팅창에 메시지 출력
    chatPanel->PrintLines(0, L"User1: Hello World!");

    // 입력창에 안내 문구 출력
    inputPanel->PrintAt(0, 0, L"> Enter your message: ");

    // 4) 화면 갱신 (Dirty Flag 기반)
    console->Refresh();

    // ... (키 입력을 받아 inputPanel에 반영, Refresh 주기적 호출 등)
    return 0;
}
```

---

## **5. 팁: 커서 제어 및 입력 처리**

- **커서 이동**:  
  - Windows의 경우, `Console::MoveCursor(row, col)`(Win32 API) 등을 통해 실제 콘솔 커서를 이동할 수 있습니다.  
  - 사용자가 입력을 치면, 해당 콘솔 위치에서 입력받도록 구현 가능합니다(플랫폼별로 방법이 다름).

- **키 입력**:  
  - 라이브러리 자체에는 키 입력 처리가 포함되어 있지 않습니다.  

---

## **6. 권장 사용 시나리오**

1. **게임 루프**나 **메인 루프**가 존재하는 애플리케이션에서,  
2. 매 프레임(or 주기적으로) `console->Refresh()`를 호출.  
3. 입력/상태 변경 시 Panel에 내용 쓰기(DirtyFlag 자동 반영).  
4. 여러 패널을 오버레이(마지막 생성된 패널이 최상단)로 사용해 팝업 창 등을 표현.