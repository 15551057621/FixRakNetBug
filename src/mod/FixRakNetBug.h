#pragma once

#include "ll/api/mod/NativeMod.h"

#pragma once

#include "ll/api/mod/NativeMod.h"

namespace fix_raknet_bug {

class FixRakNetBug {
public:
    static FixRakNetBug& getInstance();

    FixRakNetBug() : mSelf(*ll::mod::NativeMod::current()) {}

    [[nodiscard]] ll::mod::NativeMod& getSelf() const { return mSelf; }

    bool load();
    bool enable();
    bool disable();

private:
    ll::mod::NativeMod& mSelf;
};

} // namespace fix_raknet_bug 

/*

namespace fix_raknet_bug {
    // 为啥都不喜欢换行啊，不换行是规范吗
    class FixRakNetBug {
        public:
            static FixRakNetBug& getInstance();
            FixRakNetBug() : mSelf(*ll::mod::NativeMod::current()) {}

            [[nodiscard]] ll::mod::NativeMod& getSelf() const { return mSelf; }

            bool load();
            bool enable();
            bool disable();

        private:
            ll::mod::NativeMod& mSelf;
    }; // 这里也要打分号吗？可以不打吗，我看.cpp哪个也没有
}


namespace my_mod {

class MyMod {

public:
    static MyMod& getInstance();

    MyMod() : 

    [[nodiscard]] ll::mod::NativeMod& getSelf() const { return mSelf; }

    /// @return True if the mod is loaded successfully.
    bool load();

    /// @return True if the mod is enabled successfully.
    bool enable();

    /// @return True if the mod is disabled successfully.
    bool disable();

    // TODO: Implement this method if you need to unload the mod.
    // /// @return True if the mod is unloaded successfully.
    // bool unload();

private:
    ll::mod::NativeMod& mSelf;
};

} // namespace my_mod
*/