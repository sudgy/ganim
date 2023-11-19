#include "animatable.hpp"

#include <stdexcept>
#include <cmath>

#include "ganim/rate_functions.hpp"

using namespace ganim;

void Animatable::set_fps(int fps)
{
    M_fps = fps;
}

Animatable& Animatable::animate()
{
    return animate(1, rf::smoothererstep);
}

Animatable& Animatable::animate(double duration)
{
    return animate(duration, rf::smoothererstep);
}

Animatable& Animatable::animate(std::function<double(double)> rate_func)
{
    return animate(1, std::move(rate_func));
}

Animatable& Animatable::animate(
    double duration,
    std::function<double(double)> rate_func
)
{
    if (M_fps == -1) {
        throw std::logic_error("An animation was run without setting the fps.  "
                "Did you forget to add something to the scene?");
    }
    if (M_animation_time > 0) {
        throw std::logic_error("An animation was run while another one was "
                "already running on the same object.  This is not allowed.");
    }
    on_animate();
    M_animation_time = std::round(duration * M_fps);
    M_animation_progress = 0;
    add_updater([this, rf = std::move(rate_func)]{
        ++M_animation_progress;
        if (M_animation_progress == 1) on_animation_start();
        auto t = static_cast<double>(M_animation_progress) / M_animation_time;
        update_animation(rf(t));
        if (M_animation_progress == M_animation_time) {
            M_animation_time = 0;
            on_animation_end();
            return false;
        }
        return true;
    });
    return *this;
}

bool Animatable::starting_animation() const
{
    return M_animation_time > 0 and M_animation_progress == 0;
}

bool Animatable::in_animation() const
{
    return M_animation_time > 0;
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
    if (M_animation_time > 0) {
        throw std::logic_error("You can't clear updaters while an animation is "
                "running.");
    }
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
