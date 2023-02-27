#include "../include/Downloader.hpp"

#include <iostream>
#include <string>

#include "../include/json.hpp"
namespace baiduyun {

void Downloader::SetBdstoken() {
  bdstoken_ = nlohmann::json::parse(GetBdstoken())["login_info"]["bdstoken"];
  std::cout << bdstoken_ << std::endl;
}
void Downloader::SetShareidAndUk() {
  std::string shareid_and_uk_json = GetShareidAndUk();
  try {
    auto j_shareid = nlohmann::json::parse(shareid_and_uk_json)["shareid"];
    auto j_uk = nlohmann::json::parse(shareid_and_uk_json)["uk"];
    shareid_ = j_shareid.dump();
    uk_ = j_uk.dump();
  } catch (const std::exception& e) {
    std::cout << "SetShareidAndUk fault" << std::endl;
  }
}
void Downloader::SetRandsk() {
  randsk_ = nlohmann::json::parse(GetRandsk())["randsk"];
  std::cout << "randsk_: " << randsk_ << std::endl;
}
void Downloader::SetFsid() {
  std::string fs_id_json = GetFsid();

  std::cout << fs_id_json << std::endl << std::endl << std::endl;

  std::cout << nlohmann::json::parse(fs_id_json)["list"] << std::endl;

  std::cout << nlohmann::json::parse(fs_id_json)["list"] << std::endl;
  // 遍历list字段
  for (const auto& item : nlohmann::json::parse(fs_id_json)["list"].array()) {
    // 如果server_filename是"idmchslsttb.zip"
    std::cout << "server_filename" << item["server_filename"] << std::endl;

    std::cout << "file_name" << file_name << std::endl;
    std::cout << "item[\"server_filename\"].dump()"
              << item["server_filename"].dump() << std::endl;

    if (item["server_filename"].dump() == file_name) {
      // 输出对应的fs_id
      std::cout << "fs_id: " << item["fs_id"] << std::endl;
      break;
    }
  }
  for (auto it = nlohmann::json::parse(fs_id_json)["list"].begin();
       it != nlohmann::json::parse(fs_id_json)["list"].end();++it)
       {
    std::cout << 
       }
}
void Downloader::SetTimestampAndSign() {}

void baiduyun::Downloader::SetDlink() {}

void baiduyun::Downloader::SetLocationLink() {}

std::string Downloader::GetBdstoken() {
  auto return_data = this->GetResultAsync(url_str, header);
  return_data.wait();
  return return_data.get();
}
std::string Downloader::GetShareidAndUk() {
  auto shareidanduk =
      this->GetResultAsync(shareid_uk_url_str, base_header_refer);
  shareidanduk.wait();
  return shareidanduk.get();
}
std::string Downloader::GetRandsk() {
  auto post_return_data =
      this->PostResultAsync(url_get_rds, header_refer, paylaod);
  post_return_data.wait();
  return post_return_data.get();
}
std::string Downloader::GetFsid() {
  cpr::Header header_Fsid = cpr::Header{
      {"User-Agent",
       "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 "
       "(KHTML, like Gecko) Chrome/110.0.0.0 Safari/537.36"},
      {"Cookie", user_cookies_fs_id + randsk_},
  };

  auto Fsid = this->GetResultAsync(url_fs_id, header_Fsid);
  Fsid.wait();
  return Fsid.get();
}
std::string Downloader::GetTimestampAndSign() { return std::string(); }

std::string baiduyun::Downloader::GetDlink() { return std::string(); }

std::string baiduyun::Downloader::GetLocationLink() { return std::string(); }

cpr::AsyncWrapper<std::string, false> Downloader::GetResultAsync(
    const std::string& url, cpr::Header Header) {
  auto response = cpr::GetCallback(
      [](cpr::Response r) {
        try {
          return r.text;
        } catch (const std::exception& e) {
          return r.text;
        }
      },
      cpr::Url{url}, Header,
      cpr::HttpVersion{cpr::HttpVersionCode::VERSION_2_0_TLS});
  return response;
}

cpr::AsyncWrapper<std::string, false> Downloader::PostResultAsync(
    const std::string& url, cpr::Header Header, cpr::Payload payload) {
  auto response = cpr::PostCallback(
      [](cpr::Response r) {
        try {
          return r.text;
        } catch (const std::exception& e) {
          return r.text;
        }
      },
      cpr::Url{url}, Header, payload,
      cpr::HttpVersion{cpr::HttpVersionCode::VERSION_2_0_TLS});
  return response;
}

}  // namespace baiduyun