#include <iostream>

#include "offload/common/types.hpp"

int main()
{
    offload::Frame frame;

    frame.timestamp_us = 1000;
    frame.width = 1920;
    frame.height = 1080;
    frame.priority_score = 200;

    std::cout << "GPU Selective Offload Pipeline\n";

    std::cout << "Frame: "
              << frame.width
              << "x"
              << frame.height
              << std::endl;

    return 0;
}
