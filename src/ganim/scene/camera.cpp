#include "camera.hpp"

using namespace ganim;
using namespace pga3;

Camera::Camera(double distance, double width, double height) :
    M_x_scale(2*distance / width),
    M_y_scale(2*distance / height),
    M_starting_width(width),
    M_starting_height(height)
{
    shift(distance*e3);
    M_original_rotor = get_rotor();
}

Camera& Camera::reset()
{
    Transformable::reset();
    apply_rotor(M_original_rotor);
    return *this;
}
