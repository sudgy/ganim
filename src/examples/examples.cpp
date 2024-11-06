#include <iostream>

void basic_movement();
void square_create();
void basic_text();
void transforms();
void paths();

int main()
{
    std::cout << "Please specify which example you want to see:\n"
              << "1: basic_movement\n"
              << "2: square_create\n"
              << "3: basic_text\n"
              << "4: transforms\n"
              << "5: paths\n";
    int input = 0;
    std::cin >> input;
    switch (input) {
        case 1:
            basic_movement();
            break;
        case 2:
            square_create();
            break;
        case 3:
            basic_text();
            break;
        case 4:
            transforms();
            break;
        case 5:
            paths();
            break;
        default:
            std::cout << "Invalid input\n";
    }
}
