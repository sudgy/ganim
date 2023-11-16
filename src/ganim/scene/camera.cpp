#include "camera.hpp"

using namespace ganim;
using namespace pga3;

Camera::Camera(double distance, double width, double height) :
    M_x_scale(2*distance / width),
    M_y_scale(2*distance / height)
{
    shift(distance*e3);
}
