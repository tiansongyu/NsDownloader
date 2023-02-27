#include <cpr/cpr.h>

#include <chrono>
#include <iostream>
#include <memory>

#include "../include/Downloader.hpp"

int main() {
  auto downloader = std::make_unique<baiduyun::Downloader>();
  while (true) {
    std::cout << downloader->GetBdstoken() << std::endl;

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    std::cout << downloader->GetRandsk() << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  }
  return 0;
}