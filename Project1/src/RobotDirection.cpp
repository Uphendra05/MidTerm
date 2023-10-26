#include "RobotDirection.h"



RandomDirection GetRandomDirection()
{
    int randomValue = rand() % 4;

    switch (randomValue) {
    case 0:
        return RandomDirection::Up;
    case 1:
        return RandomDirection::Down;
    case 2:
        return RandomDirection::Left;
    case 3:
        return RandomDirection::Right;
    default:
        return RandomDirection::Up; // Default to Up if unexpected value
    }
}
