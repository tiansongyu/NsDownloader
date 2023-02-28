#pragma once

#include <cpr/cpr.h>

#include <string>

namespace baiduyun {

class Downloader final {
 public:
  Downloader() = default;
  ~Downloader() = default;

  void SetBdstoken();
  void SetShareidAndUk();
  void SetRandsk();
  void SetFsid();
  void SetTimestampAndSign();
  void SetDlink();
  void SetLocationLink();

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

  std::string GetBdstoken();
  std::string GetShareidAndUk();
  std::string GetRandsk();
  std::string GetFsid();
  std::string GetTimestampAndSign();
  std::string GetDlink();
  std::string GetLocationLink();

  cpr::AsyncWrapper<std::string, false> GetResultAsync(const std::string& url,
                                                       cpr::Header Header);
  cpr::AsyncWrapper<std::string, false> PostResultAsync(const std::string& url,
                                                        cpr::Header Header,
                                                        cpr::Body body);

  std::string long_url = "1RiOVjWgK3Rlrtsyj0k0iSA";

  std::string shareid_uk_url_str =
      "https://pan.baidu.com/api/shorturlinfo?shorturl=" + long_url;
  std::string url_str{
      "https://pan.baidu.com/api/"
      "loginStatus?clienttype=0&app_id=250528&web=1"};

  std::string url_get_rds{
      "https://pan.baidu.com/share/verify?surl=RiOVjWgK3Rlrtsyj0k0iSA"};
  cpr::Header header_refer{
      {"Referer", "https://pan.baidu.com/wap/init?surl=RiOVjWgK3Rlrtsyj0k0iSA"},
      {"User-Agent",
       "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, "
       "like Gecko) Chrome/110.0.0.0 Safari/537.36"}};

  cpr::Header base_header_refer{
      {"User-Agent",
       "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, "
       "like Gecko) Chrome/110.0.0.0 Safari/537.36"}};

  cpr::Body body_randsk{"pwd=ezuk"};

  std::string password = "ezuk";
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
  std::string shorturl = "RiOVjWgK3Rlrtsyj0k0iSA";
  cpr::Header header = cpr::Header{
      {"User-Agent",
       "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 "
       "(KHTML, like Gecko) Chrome/110.0.0.0 Safari/537.36"},
      {"Cookie", user_cookes},
  };



  std::string url_fs_id =
      "https://pan.baidu.com/share/"
      "list?order=name&desc=1&showempty=0&web=1&page=1&num=100&clienttype=0&"
      "shorturl=" +
      shorturl + "&dir=" + dir;
  std::string user_cookies_fs_id = "BDCLND=";
};
}  // namespace baiduyun