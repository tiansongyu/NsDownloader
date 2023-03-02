// Copyright [2023] <Copyright tiansongyu>

#pragma once

#include <cpr/cpr.h>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <ostream>
#include <regex>
#include <string>
#include <vector>

namespace baiduyun {

class Downloader final {
 public:
  Downloader(std::string url_baidu, std::string user_cookes, std::string dir,
             std::string file_name);
  ~Downloader() = default;

  // blocking function
  bool StartDownload();

 private:
  std::string bdstoken_{};
  std::string uk_{};
  std::string shareid_{};
  std::string randsk_{};
  std::string fs_id_{};
  std::string timestamp_{};
  std::string sign_{};
  std::string dlink_{};
  std::string location_{};

  bool init();

  bool SetBdstoken();
  bool SetShareidAndUk();
  bool SetRandsk();
  bool SetFsid();
  bool SetTimestampAndSign();
  bool SetDlink();
  bool SetLocationLink();

  std::string GetBdstoken();
  std::string GetShareidAndUk();
  std::string GetRandsk();
  std::string GetFsid();
  std::string GetTimestampAndSign();
  std::string GetDlink();
  std::string GetLocationLink();

  // not use start
  std::string GetBdstokenAsync();
  std::string GetShareidAndUkAsync();
  std::string GetRandskAsync();
  std::string GetFsidAsync();
  std::string GetTimestampAndSignAsync();
  std::string GetDlinkAsync();
  // not use end

  std::string long_url_{};
  std::string short_url_{};
  cpr::Url shareid_uk_url_{};

  cpr::Url bdstoken_url_{
      "https://pan.baidu.com/api/"
      "loginStatus?clienttype=0&app_id=250528&web=1"};

  cpr::Url randsk_url_{
      "https://pan.baidu.com/share/verify?surl=RiOVjWgK3Rlrtsyj0k0iSA"};
  cpr::Header header_refer_{
      {"Referer", "https://pan.baidu.com/wap/init?surl=RiOVjWgK3Rlrtsyj0k0iSA"},
      {"User-Agent",
       "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, "
       "like Gecko) Chrome/110.0.0.0 Safari/537.36"}};

  cpr::Header base_header_refer_{
      {"User-Agent",
       "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, "
       "like Gecko) Chrome/110.0.0.0 Safari/537.36"}};

  std::string pwd_{};
  cpr::Body randsk_body_{};

  std::string user_cookes_{};
  std::string dir_{};
  std::string file_name_{};
  cpr::Header header_{};

  cpr::Url fs_id_url_{};
  std::string user_cookies_fs_id_ = "BDCLND=";

  size_t file_size_{0};
  size_t file_current_index_{0};
  float progress_{0.0F};

 private:
  static cpr::AsyncWrapper<std::string, false> GetResultAsync(
      const cpr::Url& url, const cpr::Header& Header);
  static cpr::AsyncWrapper<std::string, false> PostResultAsync(
      const cpr::Url& url, const cpr::Header& Header, const cpr::Body& body);
};
}  // namespace baiduyun