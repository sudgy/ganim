#include <iostream>

void basic_movement();
void square_create();

int main()
{
    std::cout << "Please specify which example you want to see:\n"
              << "1: basic_movement\n"
              << "2: square_create\n";
    int input = 0;
    std::cin >> input;
    switch (input) {
        case 1:
            basic_movement();
            break;
        case 2:
            square_create();
            break;
        default:
            std::cout << "Invalid input\n";
    }
}
