#include "../include/Downloader.hpp"

#include <string>

namespace baiduyun {

std::string Downloader::GetBdstoken() {
  auto return_data = this->GetResultAsync(url_str, header);
  return_data.wait();
  return return_data.get();
}
std::string Downloader::GetShareidAndUk() { return std::string(); }
std::string Downloader::GetRandsk() {
  auto post_return_data =
      this->PostResultAsync(url_get_rds, header_refer, paylaod);
  post_return_data.wait();
  return post_return_data.get();
}
std::string Downloader::GetFsid() { return std::string(); }
std::string Downloader::GetTimestampAndSign() { return std::string(); }

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