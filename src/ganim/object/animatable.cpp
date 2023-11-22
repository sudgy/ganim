#include "animatable.hpp"

using namespace ganim;

void Animatable::set_fps(int fps)
{
    M_fps = fps;
}

int Animatable::add_updater_void(std::function<void()> updater)
{
    return add_updater_bool([f = std::move(updater)] {
        f();
        return true;
    });
}

int Animatable::add_updater_bool(std::function<bool()> updater)
{
    auto handle = 0;
    if (!M_updaters.empty()) {
        handle = M_updaters.rbegin()->first + 1;
    }
    M_updaters.emplace(handle, std::move(updater));
    return handle;
}

void Animatable::remove_updater(int updater_handle)
{
    M_updaters.erase(updater_handle);
}

void Animatable::clear_updaters()
{
    M_updaters.clear();
}

void Animatable::update()
{
    for (auto it = M_updaters.begin(); it != M_updaters.end();) {
        if (!it->second()) {
            it = M_updaters.erase(it);
        }
        else ++it;
    }
}
