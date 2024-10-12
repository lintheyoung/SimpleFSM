# ESP32 Multi-App Framework Documentation

欢迎使用 **ESP32 Multi-App Framework**。本框架旨在为 ESP32 开发者提供一个灵活、高效的多应用管理解决方案，支持多个应用同时运行、事件驱动的通信机制，以及模块化的硬件抽象层（HAL）。本文档将详细介绍如何构建和使用该框架，包括创建和管理应用、框架的核心组件、对外接口等内容。

## 目录

1. [概述](#1-概述)
2. [项目结构](#2-项目结构)
3. [核心组件](#3-核心组件)
    - [3.1 AppAbility](#31-appability)
    - [3.2 AppManager](#32-appmanager)
    - [3.3 EventBus](#33-eventbus)
4. [创建和使用应用](#4-创建和使用应用)
    - [4.1 创建新应用](#41-创建新应用)
    - [4.2 安装和启动应用](#42-安装和启动应用)
5. [对外接口](#5-对外接口)
    - [5.1 AppAbility 接口](#51-appability-接口)
    - [5.2 AppManager 接口](#52-appmanager-接口)
    - [5.3 EventBus 接口](#53-eventbus-接口)
6. [示例应用](#6-示例应用)
    - [6.1 MyLEDApp](#61-myledapp)
    - [6.2 MyButtonApp](#62-mybuttonapp)
    - [6.3 MyHelloApp](#63-myhelloapp)
7. [配置和编译](#7-配置和编译)
---

## 1. 概述

**ESP32 Multi-App Framework** 是一个基于 C++ 的框架，旨在简化在 ESP32 上开发和管理多个应用程序的过程。该框架提供了应用生命周期管理、事件驱动的通信机制以及硬件抽象层，使开发者能够专注于应用逻辑，而无需过多关注底层细节。

**主要特点**：

- **多应用管理**：支持同时运行多个应用，管理其生命周期（启动、运行、暂停、关闭）。
- **事件驱动通信**：通过事件总线实现应用间的通信和协作。
- **硬件抽象层（HAL）**：封装硬件控制逻辑，支持灵活扩展和模块化设计。
- **模块化设计**：易于添加新功能和扩展现有功能。

---

## 2. 项目结构

以下是框架的推荐项目目录结构：

```
ESP32_MultiApp_Framework/
├── include/
│   ├── AppAbility.h
│   ├── AppManager.h
│   ├── EventBus.h
│   ├── MyLEDApp.h
│   ├── MyButtonApp.h
│   ├── MyHelloApp.h        // 新增应用
│   └── hal/
│       ├── LEDController.h
│       └── ButtonController.h
├── src/
│   ├── AppAbility.cpp
│   ├── AppManager.cpp
│   ├── EventBus.cpp
│   ├── MyLEDApp.cpp
│   ├── MyButtonApp.cpp
│   ├── MyHelloApp.cpp       // 新增应用
│   ├── hal/
│   │   ├── LEDController.cpp
│   │   └── ButtonController.cpp
│   └── main.cpp
├── lib/
│   └── (无需手动放置，PlatformIO 会自动管理)
├── platformio.ini
└── README.md
```

**目录说明**：

- **include/**：存放所有头文件。
    - **hal/**：硬件抽象层相关头文件。
    - **其他头文件**：框架核心组件和应用的头文件。
- **src/**：存放所有源文件。
    - **hal/**：硬件抽象层相关源文件。
    - **其他源文件**：框架核心组件和应用的源文件。
- **lib/**：存放外部库（由 PlatformIO 管理）。
- **platformio.ini**：PlatformIO 配置文件。
- **README.md**：项目说明文档。

---

## 4. 创建和使用应用

### 4.1 创建新应用

要创建一个新应用，需要继承自 `AppAbility` 类，并实现必要的生命周期和事件处理方法。以下是创建新应用的步骤：

1. **创建头文件**：在 `include/` 目录下创建应用的头文件，例如 `MyNewApp.h`。
2. **创建源文件**：在 `src/` 目录下创建应用的源文件，例如 `MyNewApp.cpp`。
3. **实现应用逻辑**：在源文件中实现应用的生命周期钩子和事件处理方法。
4. **安装和启动应用**：在 `main.cpp` 中安装并启动新应用。

#### 示例：创建一个每隔 3 秒发送 `"Ping"` 的应用 `MyPingApp`

**4.1.1 `include/MyPingApp.h`**

```cpp
#ifndef MY_PING_APP_H
#define MY_PING_APP_H

#include "AppAbility.h"

class MyPingApp : public AppAbility {
public:
    MyPingApp();
    virtual ~MyPingApp();

    void onStart() override;
    void onRun() override;
    void onClose() override;
    void onEvent(const std::string& event, const std::string& data) override;

private:
    uint32_t lastPingTime;
    const uint32_t pingInterval; // 3000 毫秒
};

#endif // MY_PING_APP_H
```

**4.1.2 `src/MyPingApp.cpp`**

```cpp
#include "MyPingApp.h"
#include <Arduino.h>

MyPingApp::MyPingApp()
    : AppAbility("MyPingApp", 1000), // 更新间隔为 1000ms
      lastPingTime(0),
      pingInterval(3000) { // 每隔 3000 毫秒发送一次 "Ping"
}

MyPingApp::~MyPingApp() {}

void MyPingApp::onStart() {
    Serial.println("[MyPingApp] Started");
    lastPingTime = millis();
}

void MyPingApp::onRun() {
    uint32_t currentTime = millis();
    if (currentTime - lastPingTime >= pingInterval) {
        Serial.println("Ping");
        lastPingTime = currentTime;
    }
}

void MyPingApp::onClose() {
    Serial.println("[MyPingApp] Closed");
}

void MyPingApp::onEvent(const std::string& event, const std::string& data) {
    // 此应用不处理任何事件
}
```

### 4.2 安装和启动应用

在 `main.cpp` 中，您需要安装并启动所有应用。以下是步骤：

1. **引入应用头文件**：在 `main.cpp` 中包含新应用的头文件。
2. **安装应用**：使用 `AppManager::installApp` 方法安装应用，并获取其应用 ID。
3. **启动应用**：使用 `AppManager::openApp` 方法启动应用。

**4.2.1 修改 `src/main.cpp`**

假设您已经创建了 `MyPingApp`，以下是如何在 `main.cpp` 中安装和启动它：

```cpp
#include <Arduino.h>
#include <memory> // 确保包含此头文件
#include "AppManager.h"
#include "MyLEDApp.h"
#include "MyButtonApp.h"
#include "MyHelloApp.h"
#include "MyPingApp.h" // 引入新应用头文件

AppManager appManager;
int ledAppID, buttonAppID, helloAppID, pingAppID; // 新增 pingAppID

void setup() {
    Serial.begin(115200);
    while (!Serial) {
        ; // 等待串口初始化
    }
    Serial.println("ESP32 Multi-App Framework Started");

    // 安装应用
    ledAppID = appManager.installApp(std::make_unique<MyLEDApp>());
    buttonAppID = appManager.installApp(std::make_unique<MyButtonApp>());
    helloAppID = appManager.installApp(std::make_unique<MyHelloApp>());
    pingAppID = appManager.installApp(std::make_unique<MyPingApp>()); // 安装新应用

    // 打开应用
    appManager.openApp(ledAppID);
    appManager.openApp(buttonAppID);
    appManager.openApp(helloAppID);
    appManager.openApp(pingAppID); // 打开新应用
}

void loop() {
    appManager.update();
    delay(10); // 小延迟以防止占用过多CPU资源
}
```

**注意事项**：

- **使用智能指针**：`std::make_unique` 用于创建应用实例，并由 `AppManager` 管理其生命周期。
- **更新间隔**：确保应用的更新间隔（`updateIntervalMs`）根据应用需求合理设置。

---

## 5. 对外接口

框架提供了多个对外接口，允许开发者创建和管理应用，并在应用之间进行通信。

### 5.1 AppAbility 接口

**`AppAbility`** 是所有应用的基类，定义了应用的基本接口和生命周期钩子。开发者需要继承自该类并实现必要的方法。

**主要方法**：

- **构造函数**：

    ```cpp
    AppAbility(const std::string& appName, uint32_t updateIntervalMs = 1000);
    ```

    - **`appName`**：应用名称。
    - **`updateIntervalMs`**：应用的更新间隔（毫秒），决定 `onRun` 的调用频率。

- **生命周期钩子**：

    - **`void onStart()`**：应用启动时调用。
    - **`void onRun()`**：根据 `updateIntervalMs` 调用，用于执行周期性任务。
    - **`void onClose()`**：应用关闭时调用。
    - **`void onDestroy()`**：应用销毁时调用。
    - **`void onPause()`**：应用暂停时调用。
    - **`void onResume()`**：应用恢复运行时调用。

- **事件处理钩子**：

    ```cpp
    virtual void onEvent(const std::string& event, const std::string& data);
    ```

    - **`event`**：事件名称。
    - **`data`**：事件数据。

- **状态管理**：

    - **`AppInfo getAppInfo() const`**：获取应用信息。
    - **`State getState() const`**：获取应用当前状态。
    - **`void setState(State newState)`**：设置应用状态（仅供 `AppManager` 使用）。
    - **`uint32_t getUpdateInterval() const`**：获取应用的更新间隔。
    - **`uint32_t getLastUpdateTime() const`**：获取应用上次更新时间。
    - **`void setLastUpdateTime(uint32_t time)`**：设置应用上次更新时间。

**示例**：

以下是一个简单应用的 `AppAbility` 实现：

```cpp
#include "AppAbility.h"
#include <Arduino.h>

class MySampleApp : public AppAbility {
public:
    MySampleApp() : AppAbility("MySampleApp", 1000), counter(0) {}
    virtual ~MySampleApp() {}

    void onStart() override {
        Serial.println("[MySampleApp] Started");
    }

    void onRun() override {
        Serial.printf("[MySampleApp] Run count: %d\n", counter++);
    }

    void onClose() override {
        Serial.println("[MySampleApp] Closed");
    }

    void onEvent(const std::string& event, const std::string& data) override {
        Serial.printf("[MySampleApp] Received event '%s' with data: %s\n", event.c_str(), data.c_str());
    }

private:
    int counter;
};
```

### 5.2 AppManager 接口

**`AppManager`** 负责管理应用的生命周期和调度。开发者可以通过 `AppManager` 安装、启动、关闭和卸载应用。

**主要方法**：

- **安装应用**：

    ```cpp
    int installApp(std::unique_ptr<AppAbility> app);
    ```

    - **参数**：`app` - 应用实例。
    - **返回**：应用的唯一 ID。

- **卸载应用**：

    ```cpp
    bool uninstallApp(int appID);
    ```

    - **参数**：`appID` - 应用的唯一 ID。
    - **返回**：卸载是否成功。

- **启动应用**：

    ```cpp
    bool openApp(int appID);
    ```

    - **参数**：`appID` - 应用的唯一 ID。
    - **返回**：启动是否成功。

- **关闭应用**：

    ```cpp
    bool closeApp(int appID);
    ```

    - **参数**：`appID` - 应用的唯一 ID。
    - **返回**：关闭是否成功。

- **获取应用数量**：

    ```cpp
    std::size_t getAppNum();
    ```

    - **返回**：当前安装的应用数量。

- **获取应用信息**：

    ```cpp
    AppAbility::AppInfo getAppInfo(int appID);
    ```

    - **参数**：`appID` - 应用的唯一 ID。
    - **返回**：应用的信息结构体。

- **获取所有应用信息**：

    ```cpp
    std::vector<AppAbility::AppInfo> getAllAppInfo();
    ```

    - **返回**：所有应用的信息列表。

- **更新应用**：

    ```cpp
    void update();
    ```

    - **功能**：遍历所有运行中的应用，根据其更新间隔调用 `onRun` 方法。

**使用示例**：

```cpp
#include "AppManager.h"
#include "MySampleApp.h"

AppManager appManager;
int sampleAppID;

void setup() {
    Serial.begin(115200);
    while (!Serial) {}

    // 安装应用
    sampleAppID = appManager.installApp(std::make_unique<MySampleApp>());

    // 启动应用
    appManager.openApp(sampleAppID);
}

void loop() {
    appManager.update();
    delay(10);
}
```

### 5.3 EventBus 接口

**`EventBus`** 实现了发布-订阅模式，允许应用之间通过事件进行通信。

**主要方法**：

- **获取单例实例**：

    ```cpp
    static EventBus& getInstance();
    ```

- **订阅事件**：

    ```cpp
    void subscribe(const std::string& event, EventCallback callback);
    ```

    - **参数**：
        - `event`：事件名称，支持具体事件和全局事件 `"*"`.
        - `callback`：事件回调函数，接收事件名称和数据。

- **取消所有订阅**：

    ```cpp
    void unsubscribeAll();
    ```

- **发布事件**：

    ```cpp
    void publish(const std::string& event, const std::string& data);
    ```

    - **参数**：
        - `event`：事件名称。
        - `data`：事件数据。

**使用示例**：

- **订阅事件**：

    ```cpp
    EventBus::getInstance().subscribe("MyEvent", [](const std::string& event, const std::string& data) {
        Serial.printf("Received event '%s' with data: %s\n", event.c_str(), data.c_str());
    });
    ```

- **发布事件**：

    ```cpp
    EventBus::getInstance().publish("MyEvent", "Hello from EventBus!");
    ```

**全局订阅**：

订阅所有事件，可以使用 `"*"` 作为事件名称：

```cpp
EventBus::getInstance().subscribe("*", [](const std::string& event, const std::string& data) {
    Serial.printf("Global subscriber received event '%s' with data: %s\n", event.c_str(), data.c_str());
});
```

---

## 6. 示例应用

以下是三个示例应用，展示了如何使用框架的核心组件创建功能丰富的应用。

### 6.1 MyLEDApp

**功能**：控制 WS2812 LED 的亮灭和颜色切换。当接收到 `"ChangeColor"` 事件时，LED 颜色在红、绿、蓝之间循环切换。

**头文件：`include/MyLEDApp.h`**

```cpp
#ifndef MY_LED_APP_H
#define MY_LED_APP_H

#include "AppAbility.h"
#include "hal/LEDController.h"
#include <vector>

class MyLEDApp : public AppAbility {
public:
    MyLEDApp();
    virtual ~MyLEDApp();

    void onStart() override;
    void onRun() override;
    void onClose() override;
    void onEvent(const std::string& event, const std::string& data) override;

private:
    LEDController* ledController;
    bool isOn;
    CRGB currentColor;
    std::vector<CRGB> colors;
    size_t currentColorIndex;
};

#endif // MY_LED_APP_H
```

**源文件：`src/MyLEDApp.cpp`**

```cpp
#include "MyLEDApp.h"
#include "EventBus.h"
#include <Arduino.h>

MyLEDApp::MyLEDApp()
    : AppAbility("MyLEDApp", 500), // 更新间隔 500ms，用于控制LED开关
      ledController(nullptr),
      isOn(false),
      currentColor(CRGB::Red),
      currentColorIndex(0) {
    // 定义三个颜色
    colors.push_back(CRGB::Red);
    colors.push_back(CRGB::Green);
    colors.push_back(CRGB::Blue);
}

MyLEDApp::~MyLEDApp() {
    delete ledController;
}

void MyLEDApp::onStart() {
    Serial.println("[MyLEDApp] Started");
    ledController = new LEDController();
    ledController->begin();
    ledController->setColor(currentColor);
    ledController->showColor();
}

void MyLEDApp::onRun() {
    if (isOn) {
        ledController->clear();
        isOn = false;
    } else {
        ledController->setColor(currentColor);
        ledController->showColor();
        isOn = true;
    }
}

void MyLEDApp::onClose() {
    Serial.println("[MyLEDApp] Closed");
    if (ledController) {
        ledController->clear();
    }
}

void MyLEDApp::onEvent(const std::string& event, const std::string& data) {
    // 处理接收到的事件
    Serial.printf("[MyLEDApp] Received event '%s' with data: %s\n", event.c_str(), data.c_str());
    // 根据需要处理不同的事件
    if (event == "ChangeColor") {
        currentColorIndex = (currentColorIndex + 1) % colors.size();
        currentColor = colors[currentColorIndex];
        Serial.printf("[MyLEDApp] Changing color to index %zu\n", currentColorIndex);
        if (isOn) {
            ledController->setColor(currentColor);
            ledController->showColor();
        }
    }
}
```

### 6.2 MyButtonApp

**功能**：轮询按钮状态，当按钮被按下时，发布 `"ChangeColor"` 事件。

**头文件：`include/MyButtonApp.h`**

```cpp
#ifndef MY_BUTTON_APP_H
#define MY_BUTTON_APP_H

#include "AppAbility.h"
#include "hal/ButtonController.h"

class MyButtonApp : public AppAbility {
public:
    MyButtonApp();
    virtual ~MyButtonApp();

    void onStart() override;
    void onRun() override;
    void onClose() override;
    void onEvent(const std::string& event, const std::string& data) override;

private:
    ButtonController* buttonController;
    void handleButtonPress();
};

#endif // MY_BUTTON_APP_H
```

**源文件：`src/MyButtonApp.cpp`**

```cpp
#include "MyButtonApp.h"
#include "EventBus.h"
#include <Arduino.h>

// 定义按钮引脚
#define BUTTON_PIN 4

MyButtonApp::MyButtonApp()
    : AppAbility("MyButtonApp", 100), // 更新间隔 100ms，用于按钮轮询
      buttonController(nullptr) {
}

MyButtonApp::~MyButtonApp() {
    delete buttonController;
}

void MyButtonApp::onStart() {
    Serial.println("[MyButtonApp] Started");
    buttonController = new ButtonController(BUTTON_PIN, std::bind(&MyButtonApp::handleButtonPress, this));
    buttonController->begin();
}

void MyButtonApp::onRun() {
    // 轮询按钮状态
    if (buttonController) {
        buttonController->update();
    }
}

void MyButtonApp::onClose() {
    Serial.println("[MyButtonApp] Closed");
}

void MyButtonApp::onEvent(const std::string& event, const std::string& data) {
    // 此应用不处理任何事件
}

void MyButtonApp::handleButtonPress() {
    Serial.println("[MyButtonApp] Button Pressed");
    // 发布 "ChangeColor" 事件
    EventBus::getInstance().publish("ChangeColor", "Button was pressed");
}
```

### 6.3 MyHelloApp

**功能**：每隔 2 秒发送一次 `"Hello World"` 消息到串口监视器。

**头文件：`include/MyHelloApp.h`**

```cpp
#ifndef MY_HELLO_APP_H
#define MY_HELLO_APP_H

#include "AppAbility.h"

class MyHelloApp : public AppAbility {
public:
    MyHelloApp();
    virtual ~MyHelloApp();

    void onStart() override;
    void onRun() override;
    void onClose() override;
    void onEvent(const std::string& event, const std::string& data) override;

private:
    uint32_t lastHelloTime;
    const uint32_t helloInterval; // 2000 毫秒
};

#endif // MY_HELLO_APP_H
```

**源文件：`src/MyHelloApp.cpp`**

```cpp
#include "MyHelloApp.h"
#include <Arduino.h>

MyHelloApp::MyHelloApp()
    : AppAbility("MyHelloApp", 1000), // 设定更新间隔为 1000ms，用于检查是否需要发送 "Hello World"
      lastHelloTime(0),
      helloInterval(2000) { // 每隔 2000 毫秒发送一次
}

MyHelloApp::~MyHelloApp() {}

void MyHelloApp::onStart() {
    Serial.println("[MyHelloApp] Started");
    lastHelloTime = millis();
}

void MyHelloApp::onRun() {
    uint32_t currentTime = millis();
    if (currentTime - lastHelloTime >= helloInterval) {
        Serial.println("Hello World");
        lastHelloTime = currentTime;
    }
}

void MyHelloApp::onClose() {
    Serial.println("[MyHelloApp] Closed");
}

void MyHelloApp::onEvent(const std::string& event, const std::string& data) {
    // 此应用不处理任何事件
}
```

---

## 7. 配置和编译

### 7.1 PlatformIO 配置文件

确保 `platformio.ini` 已正确配置，以支持 C++17 标准和所需的库。

**`platformio.ini`**

```ini
[env:arduino-esp32c3]
platform = espressif32
board = esp32-c3-devkitm-1
framework = arduino
monitor_speed = 115200
cpp_std = c++17

build_flags = 
    ; enable USB serial
    -D PIO_FRAMEWORK_ARDUINO_ENABLE_CDC
    -D USBCON
    -DARDUINO_USB_CDC_ON_BOOT=1
    -DARDUINO_USB_MODE=1
    -std=gnu++17    ; 保留此行以强制使用 C++17

lib_deps =
    adafruit/Adafruit NeoPixel @ ^1.12.2
    https://github.com/lintheyoung/SimpleFSM
    FastLED/FastLED @ ^3.4.0
```

**说明**：

- **`cpp_std = c++17`**：设置项目使用 C++17 标准。
- **`-std=gnu++17`**：强制编译器使用 C++17 标准，以确保兼容性。
- **`lib_deps`**：列出了项目所需的外部库。PlatformIO 将自动下载和管理这些库。

### 7.2 编译与上传

按照以下步骤编译和上传固件到 ESP32：

1. **安装 PlatformIO**：

    - 如果尚未安装，请参考 [PlatformIO 官方网站](https://platformio.org/) 的安装指南。

2. **导入项目**：

    - 将上述文件结构和代码放置在您的 PlatformIO 工作区中。

3. **连接 ESP32**：

    - 将 ESP32 开发板连接到计算机，并确保选择正确的串口端口。

4. **清理项目**（推荐）：

    - 在 PlatformIO IDE 中，点击左侧的“PlatformIO”图标。
    - 选择“Projects”。
    - 选择您的项目。
    - 点击“Clean”按钮（扫帚图标）。
    
    或者，在终端中运行：

    ```bash
    platformio run --target clean
    ```

5. **编译并上传**：

    - 在 PlatformIO IDE 中，点击“Build”按钮（勾号图标）进行编译。
    - 编译成功后，点击“Upload”按钮（箭头图标）将固件上传到 ESP32。
    
    或者，在终端中运行：

    ```bash
    platformio run --target upload
    ```

6. **监视串口输出**：

    - 打开串口监视器（波特率设置为 `115200`）。
    - 在 PlatformIO IDE 中，点击“PlatformIO”图标，选择“Serial Monitor”。
    - 您应该会看到类似如下的输出：

      ```
      ESP32 Multi-App Framework Started
      [MyLEDApp] Started
      [MyButtonApp] Started
      [MyHelloApp] Started
      [MyPingApp] Started
      [MyLEDApp] Running
      [MyLEDApp] Running
      Hello World
      [MyLEDApp] Running
      Hello World
      [MyLEDApp] Running
      Ping
      [MyLEDApp] Running
      ...
      [MyButtonApp] Button Pressed
      [MyLEDApp] Changing color to index 1
      [MyLEDApp] Running
      Hello World
      ...
      ```

    **说明**：

    - **`MyLEDApp`**：每 500 毫秒控制 LED 的亮灭和颜色切换。
    - **`MyButtonApp`**：轮询按钮状态，当按钮被按下时，发布 `"ChangeColor"` 事件。
    - **`MyHelloApp`**：每 2 秒发送一次 `"Hello World"` 到串口监视器。
    - **`MyPingApp`**：每 3 秒发送一次 `"Ping"` 到串口监视器。

---
