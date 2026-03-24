#include "mod/FixRakNetBug.h"
#include "ll/api/mod/RegisterHelper.h"
#include "ll/api/memory/Hook.h"
#include "mc/deps/raknet/RakNet.h"
#include "mc/deps/raknet/RakPeer.h"
#include "mc/deps/raknet/RakNetSocket.h"
#include "mc/deps/raknet/SystemAddress.h"

namespace fix_raknet_bug {

// 修复 MCPE-228407: 处理 ID_PONG_ADDRESS_INFO (0x86) 包的缓冲区溢出问题
LL_STATIC_HOOK(
    RakNetHook,
    HookPriority::High,
    &RakNet::ProcessOfflineNetworkPacket,
    bool,
    RakNet::SystemAddress  systemAddress,
    char const*            data,
    int                    length,
    RakNet::RakPeer*       rakPeer,
    RakNet::RakNetSocket2* rakNetSocket,
    bool*                  isOfflineMessage,
    uint64                 timeRead
) {
    // 检查是否是 0x86 包且长度不足（会导致崩溃）
    if (static_cast<unsigned char>(data[0]) == 0x86 && 
        length < sizeof(unsigned char) + sizeof(RakNet::SystemAddress)) {
        return false;  // 丢弃这个包，不处理
    }
    return origin(systemAddress, data, length, rakPeer, rakNetSocket, isOfflineMessage, timeRead);
}

// 注册 Hook
void registerHooks() {
    static memory::HookRegistrar<RakNetHook> registrar;
}

// 插件实现
FixRakNetBug& FixRakNetBug::getInstance() {
    static FixRakNetBug instance;
    return instance;
}

bool FixRakNetBug::load() {
    getSelf().getLogger().info("FixRakNetBug 加载中...");
    return true;
}

bool FixRakNetBug::enable() {
    getSelf().getLogger().info("FixRakNetBug 已启用，RakNet 崩溃修复已激活");
    registerHooks();  // 激活 Hook 
    return true;
}

bool FixRakNetBug::disable() {
    getSelf().getLogger().info("FixRakNetBug 已禁用");
    // Hook 会自动清理，不需要手动处理
    return true;
}

} // namespace fix_raknet_bug

LL_REGISTER_MOD(fix_raknet_bug::FixRakNetBug, fix_raknet_bug::FixRakNetBug::getInstance());

/*
namespace my_mod {

MyMod& MyMod::getInstance() {
    static MyMod instance;
    return instance;
}

bool MyMod::load() {
    getSelf().getLogger().debug("Loading...");
    // Code for loading the mod goes here.
    return true;
}

bool MyMod::enable() {
    getSelf().getLogger().debug("Enabling...");
    // Code for enabling the mod goes here.
    return true;
}

bool MyMod::disable() {
    getSelf().getLogger().debug("Disabling...");
    // Code for disabling the mod goes here.
    return true;
}

} // namespace my_mod

LL_REGISTER_MOD(my_mod::MyMod, my_mod::MyMod::getInstance());
*/
