#include <iostream>

void basic_movement();
void square_create();
void basic_text();
void transforms();
void paths();
void vectors();
void lighting();
void oit();
void bivectors();
void paths_3d();

int main()
{
    std::cout << "Please specify which example you want to see:\n"
              << "1: basic_movement\n"
              << "2: square_create\n"
              << "3: basic_text\n"
              << "4: transforms\n"
              << "5: paths\n"
              << "6: vectors\n"
              << "7: lighting\n"
              << "8: Order-independent Transparency\n"
              << "9: Bivectors\n"
              << "10: 3D Paths\n";
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
        case 6:
            vectors();
            break;
        case 7:
            lighting();
            break;
        case 8:
            oit();
            break;
        case 9:
            bivectors();
            break;
        case 10:
            paths_3d();
            break;
        default:
            std::cout << "Invalid input\n";
    }
}
