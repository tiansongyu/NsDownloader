#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <switch.h>

#include <string>

#include "../include/Downloader.hpp"
#include "../include/windows.hpp"
#include "gui.hpp"
// The SD card is automatically mounted as the default device, usable with
// standard stdio. SD root dir is located at "/" (also "sdmc:/" but normally
// using the latter isn't needed). The default current-working-directory when
// using relative paths is normally the directory where your application is
// located on the SD card.

void download_test() {
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
  std::string file_name_in_baiduyun = "idmchslsttb.zip";
  std::string file_name_in_switch = "/config/idmchslsttb.zip";
  std::cout << "start init" << std::endl;
  appletMainLoop();
  auto downloader = std::make_unique<baiduyun::Downloader>(
      url_baidu, user_cookes, dir, file_name_in_baiduyun, file_name_in_switch);
  std::cout << "make_unique" << std::endl;
  appletMainLoop();
  downloader->StartDownload();
  std::cout << "end" << std::endl;
}

namespace Services {
int Init(void) {
  Result ret = 0;

  socketInitializeDefault();
  nxlinkStdio();

  if (R_FAILED(ret = plInitialize(PlServiceType_User))) {
    std::cout << "plinitialize error" << std::endl;
    return ret;
  }

  if (!GUI::Init()) std::cout << "GUI::Init() failed: \n" << std::endl;

  plExit();
  return 0;
}

void Exit(void) {
  GUI::Exit();
  socketExit();
}
}  // namespace Services

int main(int argc, char** argv) {
  u64 key = 0;

  Services::Init();

  while (GUI::Loop(key)) {
    Windows::MainWindow(key, false);
    GUI::Render();
  }
  Services::Exit();
  return 0;
}
