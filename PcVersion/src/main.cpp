#include <cpr/cpr.h>

#include <chrono>
#include <iostream>
#include <memory>

#include "../include/Downloader.hpp"

int main() {
  std::string url_baidu =
      "https://pan.baidu.com/s/1RiOVjWgK3Rlrtsyj0k0iSA?pwd=ezuk";
  std::string user_cookes =
      "BDUSS="
      "TdZTFJqbXNYa1psd3lSOTFjOWpmRi0tMVg3VzI5bm9QY2xMYk44c3ZXc1N6NmRqS"
      "VFBQUF"
      "BJCQAAAAAAAAAAAEAAABi4udFbmZ5MjQwOQAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
      "AAAAAA"
      "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABJCgGMSQoBja; "
      "STOKEN="
      "db98458575d0ddbd9ecd94424b1a312cdc82ead56919797311045eb9f168b66"
      "3;";
  std::string dir = "/重要文件";
  std::string file_name = "idmchslsttb.zip";

  auto downloader = std::make_unique<baiduyun::Downloader>(
     url_baidu, user_cookes, dir, file_name);

  downloader->StartDownload();

  std::this_thread::sleep_for(std::chrono::milliseconds(10000));

  return 0;
}
