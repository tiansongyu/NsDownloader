#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <switch.h>

#include <string>

#include "../include/Downloader.hpp"
#include "../include/windows.hpp"
#include "../libs/inih/INIReader.h"
#include "gui.hpp"
// The SD card is automatically mounted as the default device, usable with
// standard stdio. SD root dir is located at "/" (also "sdmc:/" but normally
// using the latter isn't needed). The default current-working-directory when
// using relative paths is normally the directory where your application is
// located on the SD card.

int download_test() {
  INIReader reader("romfs:/user.ini");
  if (reader.ParseError() < 0) {
    std::cout << "Can't load 'test.ini'\n";
    return -1;
  }
  std::cout << "Config loaded from 'test.ini':" << std::endl
            << "file_name_in_switch=" << std::endl
            << reader.Get("user", "file_name_in_switch", "UNKNOWN") << std::endl
            << "url_baidu=" << reader.Get("user", "url_baidu", "UNKNOWN")
            << "dir=" << reader.Get("user", "dir", "UNKNOWN") << std::endl
            << "user_cookies=" << reader.Get("user", "user_cookies", "UNKNOWN")
            << std::endl
            << "file_name_in_baiduyun="
            << reader.Get("user", "file_name_in_baiduyun", "UNKNOWN")
            << std::endl;

  std::string url_baidu = reader.Get("user", "url_baidu", "UNKNOWN");
  auto tmp_cookies = reader.Get("user", "user_cookies", "UNKNOWN");
  std::string user_cookes = tmp_cookies.substr(1, tmp_cookies.length() - 2);

  std::string dir = reader.Get("user", "dir", "UNKNOWN");
  std::string file_name_in_baiduyun =
      reader.Get("user", "file_name_in_baiduyun", "UNKNOWN");
  std::string file_name_in_switch =
      reader.Get("user", "file_name_in_switch", "UNKNOWN");
  auto downloader = std::make_unique<baiduyun::Downloader>(
      url_baidu, user_cookes, dir, file_name_in_baiduyun, file_name_in_switch);
  std::cout << "make_unique" << std::endl;
  downloader->StartDownload();
  std::cout << "end" << std::endl;
  return 0;
}

namespace Services {
int Init(void) {
  Result ret = 0;

  socketInitializeDefault();
  nxlinkStdio();
  romfsInit();

  if (R_FAILED(ret = plInitialize(PlServiceType_User))) {
    std::cout << "plinitialize error" << std::endl;
    return ret;
  }

  if (!GUI::Init()) std::cout << "GUI::Init() failed: \n" << std::endl;

  plExit();
  return 0;
}

void Exit(void) {
  romfsExit();
  GUI::Exit();
  socketExit();
}
}  // namespace Services

int main(int argc, char** argv) {
  u64 key = 0;

  Services::Init();
  //   if (download_test() == -1) {
  //     return 0;
  //   }
  while (GUI::Loop(key)) {
    Windows::MainWindow(key, false);
    GUI::Render();
  }
  Services::Exit();
  return 0;
}
