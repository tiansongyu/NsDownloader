#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <switch.h>

#include <string>

#include "../include/Downloader.hpp"

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
  std::string file_name = "idmchslsttb.zip";
  std::cout << "start init" << std::endl;
  appletMainLoop();
  auto downloader = std::make_unique<baiduyun::Downloader>(
      url_baidu, user_cookes, dir, file_name);
  std::cout << "make_unique" << std::endl;
  appletMainLoop();
  downloader->StartDownload();
  std::cout << "end" << std::endl;
}

int main(int argc, char** argv) {
  consoleInit(NULL);

  // Configure our supported input layout: a single player with standard
  // controller styles
  padConfigureInput(1, HidNpadStyleSet_NpadStandard);
  socketInitializeDefault();
  nxlinkStdio();  // Redirect stdout and stderr over the network to nxlink

  // Initialize the default gamepad (which reads handheld mode inputs as well as
  // the first connected controller)
  PadState pad;
  padInitializeDefault(&pad);

  DIR* dir;
  struct dirent* ent;

  dir = opendir("");  // Open current-working-directory.
  if (dir == NULL) {
    printf("Failed to open dir.\n");
  } else {
    printf("Dir-listing for '':\n");
    while ((ent = readdir(dir))) {
      printf("d_name: %s\n", ent->d_name);
    }
    closedir(dir);
    printf("Done.\n");
  }

  // Main loop
  while (appletMainLoop()) {
    // Scan the gamepad. This should be done once for each frame
    padUpdate(&pad);

    // padGetButtonsDown returns the set of buttons that have been newly pressed
    // in this frame compared to the previous one
    u64 kDown = padGetButtonsDown(&pad);

    if (kDown & HidNpadButton_Plus)
      break;  // break in order to return to hbmenu
    download_test();
    
    consoleUpdate(NULL);
  }

  consoleExit(NULL);
  return 0;
}
