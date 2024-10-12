///////////////////////////////////////////////
#include "Mooncake.h"
#include "Arduino.h"

///////////////////////////////////////////////

Mooncake::Mooncake() {
    // 构造函数
}

Mooncake::~Mooncake() {
    uninstallAllApps();
}

int Mooncake::installApp(std::unique_ptr<AppAbility> appAbility) {
    int appID = nextAppID++;
    appAbility->setAppID(appID); // 使用 setter 设置 appID
    AppEntry entry = { appID, std::move(appAbility), false };
    apps.emplace(appID, std::move(entry));
    return appID;
}

bool Mooncake::uninstallApp(int appID) {
    auto it = apps.find(appID);
    if (it != apps.end()) {
        if (it->second.isOpen) {
            it->second.app->onClose();
        }
        apps.erase(it);
        return true;
    }
    return false;
}

void Mooncake::uninstallAllApps() {
    // 替换结构化绑定为传统循环
    for (auto it = apps.begin(); it != apps.end(); ++it) {
        if (it->second.isOpen) {
            it->second.app->onClose();
        }
    }
    apps.clear();
}

bool Mooncake::openApp(int appID) {
    auto it = apps.find(appID);
    if (it != apps.end() && !it->second.isOpen) {
        it->second.app->onOpen();
        it->second.isOpen = true;
        return true;
    }
    return false;
}

bool Mooncake::closeApp(int appID) {
    auto it = apps.find(appID);
    if (it != apps.end() && it->second.isOpen) {
        it->second.app->onClose();
        it->second.isOpen = false;
        return true;
    }
    return false;
}

std::size_t Mooncake::getAppNum() const {
    return apps.size();
}

AppAbility::AppInfo_t Mooncake::getAppInfo(int appID) const {
    AppAbility::AppInfo_t info;
    auto it = apps.find(appID);
    if (it != apps.end()) {
        info.appID = it->second.appID;
        info.name = it->second.app->getName();
        info.isOpen = it->second.isOpen;
    }
    return info;
}

std::vector<AppAbility::AppInfo_t> Mooncake::getAllAppInfo() const {
    std::vector<AppAbility::AppInfo_t> allInfo;
    for (auto it = apps.begin(); it != apps.end(); ++it) {
        AppAbility::AppInfo_t info;
        info.appID = it->second.appID;
        info.name = it->second.app->getName();
        info.isOpen = it->second.isOpen;
        allInfo.push_back(info);
    }
    return allInfo;
}

void Mooncake::update() {
    for (auto it = apps.begin(); it != apps.end(); ++it) {
        if (it->second.isOpen) {
            it->second.app->onRunning();
        }
    }
}
///////////////////////////////////////////////
