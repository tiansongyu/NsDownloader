#include <cpr/cpr.h>

#include <chrono>
#include <iostream>
#include <memory>

#include "../include/Downloader.hpp"

int main() {
  auto downloader = std::make_unique<baiduyun::Downloader>();

  downloader->SetBdstoken();
  downloader->SetShareidAndUk();
  downloader->SetRandsk();
  downloader->SetFsid();
  downloader->SetTimestampAndSign();
  downloader->SetDlink();
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  return 0;
}