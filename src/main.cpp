#include <cpr/cpr.h>

#include <chrono>
#include <iostream>
#include <memory>

#include "../include/Downloader.hpp"

int main() {
  std::string long_url = "1RiOVjWgK3Rlrtsyj0k0iSA";
  std::string pwd{"ezuk"};
  std::string user_cookes =
      "BDUSS="
      "TdZTFJqbXNYa1psd3lSOTFjOWpmRi0tMVg3VzI5bm9QY2xMYk44c3ZXc1N6NmRqSVFBQUF"
      "BJCQAAAAAAAAAAAEAAABi4udFbmZ5MjQwOQAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
      "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABJCgGMSQoBja; "
      "STOKEN="
      "db98458575d0ddbd9ecd94424b1a312cdc82ead56919797311045eb9f168b66"
      "3;ndut_fmt=";
  std::string dir = "/重要文件";
  std::string file_name = "idmchslsttb.zip";

  auto downloader = std::make_unique<baiduyun::Downloader>(
      long_url, pwd, user_cookes, dir, file_name);
  downloader->StartDownload();
  std::this_thread::sleep_for(std::chrono::milliseconds(10000));

  return 0;
}
