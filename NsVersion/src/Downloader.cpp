// Copyright [2023] <Copyright tiansongyu>

#include "../include/Downloader.hpp"

#include <fstream>
#include <iostream>
#include <regex>
#include <string>

#include "../include/json.hpp"
namespace baiduyun {

Downloader::Downloader(std::string url_baidu, std::string user_cookes,
                       std::string dir, std::string file_name_in_baiduyun,
                       std::string file_dirname_in_switch)
    : user_cookes_(user_cookes),
      dir_(dir),
      file_name_in_baiduyun_(file_name_in_baiduyun),
      file_dirname_in_switch_(file_dirname_in_switch) {
  std::regex pattern("/s/(\\w+)\\?pwd=(\\w+)");
  std::smatch match;
  std::regex_search(url_baidu, match, pattern);
  long_url_ = match[1];
  pwd_ = match[2];
  // TODO(tiansongyu): (throw check)
  short_url_ = long_url_.substr(1, long_url_.length() - 1);
  randsk_body_ = {"pwd=" + pwd_};
  user_cookes_ += ";ndut_fmt=";
  shareid_uk_url_ =
      "https://pan.baidu.com/api/shorturlinfo?shorturl=" + long_url_;
  header_ = cpr::Header{
      {"User-Agent",
       "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 "
       "(KHTML, like Gecko) Chrome/110.0.0.0 Safari/537.36"},
      {"Cookie", user_cookes_},
  };
  fs_id_url_ =
      "https://pan.baidu.com/share/"
      "list?order=name&desc=1&showempty=0&web=1&page=1&num=100&clienttype=0&"
      "shorturl=" +
      short_url_ + "&dir=" + dir_;
}

bool Downloader::init() {
  this->SetBdstoken();
  this->SetShareidAndUk();
  this->SetRandsk();
  this->SetFsid();
  this->SetTimestampAndSign();
  this->SetDlink();
  this->SetLocationLink();
  return true;
}

bool Downloader::SetBdstoken() {
  bdstoken_ = nlohmann::json::parse(GetBdstoken())["login_info"]["bdstoken"];
  std::cout << "bdstoken_: " << bdstoken_ << std::endl;
  return true;
}
bool Downloader::SetRandsk() {
  randsk_ = nlohmann::json::parse(GetRandsk())["randsk"];
  std::cout << "randsk_: " << randsk_ << std::endl;
  return true;
}
bool Downloader::SetShareidAndUk() {
  std::string shareid_and_uk_json_str = GetShareidAndUk();
  try {
    auto j_shareid = nlohmann::json::parse(shareid_and_uk_json_str)["shareid"];
    auto j_uk = nlohmann::json::parse(shareid_and_uk_json_str)["uk"];
    shareid_ = j_shareid.dump();
    uk_ = j_uk.dump();
    std::cout << "shareid_: " << shareid_ << std::endl;
    std::cout << "uk_: " << uk_ << std::endl;
  } catch (const std::exception& e) {
    std::cout << "SetShareidAndUk fault" << std::endl;
  }
  return true;
}

bool Downloader::SetFsid() {
  std::string fs_id_json_str = GetFsid();
  auto item = nlohmann::json::parse(fs_id_json_str)["list"];
  for (size_t i = 0; i < item.size(); ++i) {
    auto tmp_str = item.at(i)["server_filename"].dump();
    if (tmp_str.substr(1, tmp_str.length() - 2) == file_name_in_baiduyun_) {
      fs_id_ = item.at(i)["fs_id"].dump();
      std::cout << "fs_id_: " << fs_id_ << std::endl;
    }
  }
  return true;
}
bool Downloader::SetTimestampAndSign() {
  auto timestampandsign_json_str = nlohmann::json::parse(GetTimestampAndSign());
  timestamp_ = timestampandsign_json_str["data"]["timestamp"].dump();
  sign_ = timestampandsign_json_str["data"]["sign"].dump();
  sign_ = sign_.substr(1, sign_.length() - 2);
  std::cout << "timestamp_: " << timestamp_ << std::endl;
  std::cout << "sign_: " << sign_ << std::endl;
  return true;
}

bool baiduyun::Downloader::SetDlink() {
  std::string dlink_json_str = GetDlink();
  auto item =
      nlohmann::json::parse(dlink_json_str)["list"].at(0)["dlink"].dump();
  dlink_ = item.substr(1, item.length() - 2);

  std::cout << dlink_ << std::endl;
  return true;
}

bool baiduyun::Downloader::SetLocationLink() {
  std::string locationlink_json_str = GetLocationLink();
  std::regex re("Location:\\s+(.+)");
  std::smatch match;
  if (std::regex_search(locationlink_json_str, match, re)) {
    location_ = match[1];
  }
  std::cout << "location: " << location_ << std::endl;
  return true;
}

std::string Downloader::GetBdstoken() {
  auto Bdstoken_json_str = cpr::Get(bdstoken_url_, header_);
  return Bdstoken_json_str.text;
}
std::string Downloader::GetShareidAndUk() {
  auto shareidanduk_json_str = cpr::Get(shareid_uk_url_, base_header_refer_);
  return shareidanduk_json_str.text;
}
std::string Downloader::GetRandsk() {
  auto randsk_json_str = cpr::Post(randsk_url_, header_refer_, randsk_body_);
  return randsk_json_str.text;
}
std::string Downloader::GetFsid() {
  cpr::Header header_Fsid = cpr::Header{
      {"User-Agent",
       "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 "
       "(KHTML, like Gecko) Chrome/110.0.0.0 Safari/537.36"},
      {"Cookie", user_cookies_fs_id_ + randsk_},
  };

  auto fsid_json_str = cpr::Get(fs_id_url_, header_Fsid);
  return fsid_json_str.text;
}
std::string Downloader::GetTimestampAndSign() {
  cpr::Url timestampandsign{
      "https://pan.baidu.com/share/"
      "tplconfig?surl=" +
      long_url_ +
      "&fields=sign,timestamp&channel=chunlei&web=1&"
      "app_id=250528&bdstoken=" +
      bdstoken_ + "&clienttype=0"};
  auto timestampandsign_json_str = cpr::Get(timestampandsign, header_);
  return timestampandsign_json_str.text;
}
std::string Downloader::GetDlink() {
  cpr::Url url_dlink{
      "https://pan.baidu.com/api/"
      "sharedownload?app_id=250528&channel=chunlei&clienttype=12&sign=" +
      sign_ +
      "&"
      "timestamp=" +
      timestamp_ + "&web=1"};

  cpr::Header dlink_header = cpr::Header{
      {"User-Agent",
       "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) "
       "AppleWebKit/537.36 (KHTML, like Gecko) Chrome/110.0.0.0 Safari/537.36"},
      {"Host", "pan.baidu.com"},
      {"Cookie", user_cookes_},
      {"Referer", "pan.baidu.com"}};

  cpr::Body body_dlink{"encrypt=0&extra=%7B%22sekey%22%3A%22" + randsk_ +
                       "%22%7D&product="
                       "share&uk=" +
                       uk_ + "&primaryid=" + shareid_ + "&fid_list=%5B" +
                       fs_id_ + "%5D"};

  std::cout << "randsk_: " << randsk_ << std::endl;
  std::cout << "uk_: " << uk_ << std::endl;
  std::cout << "shareid_: " << shareid_ << std::endl;
  std::cout << "fs_id_: " << fs_id_ << std::endl;
  std::cout << "user_cookes: " << user_cookes_ << std::endl;

  std::cout << "url_get_rds: " << url_dlink << std::endl;
  auto dlink_json_str = cpr::Post(url_dlink, dlink_header, body_dlink);

  return dlink_json_str.text;
}

bool Downloader::StartDownload() {
  init();
  cpr::Session session;
  std::ofstream outFile(file_dirname_in_switch_, std::ios::binary);

  session.SetUrl(location_);
  file_size_ = session.GetDownloadFileLength();
  file_current_index_ = 0;
  float progress_old = 0.0F;
  auto r = session.Download(cpr::WriteCallback(
      [this, &progress_old](std::string data, intptr_t outFile) -> bool {
        std::ofstream* outFile_tmp = reinterpret_cast<std::ofstream*>(outFile);
        outFile_tmp->write(data.data(), data.size());
        file_current_index_ += data.size();
        float new_progress = file_current_index_ * 100 / file_size_;
        if (new_progress != progress_old) {
          progress_ = new_progress;
          progress_old = progress_;
          std::cout << progress_ << "%" << std::endl;
        }
        return true;
      },
      reinterpret_cast<intptr_t>(&outFile)));
  outFile.close();
  file_size_ = 0;
  progress_ = 0.0F;

  return false;
}
std::string Downloader::GetBdstokenAsync() {
  auto return_data = this->GetResultAsync(bdstoken_url_, header_);
  return_data.wait();
  return return_data.get();
}
std::string Downloader::GetShareidAndUkAsync() {
  auto shareidanduk = this->GetResultAsync(shareid_uk_url_, base_header_refer_);
  shareidanduk.wait();
  return shareidanduk.get();
}
std::string Downloader::GetRandskAsync() {
  auto post_return_data =
      this->PostResultAsync(randsk_url_, header_refer_, randsk_body_);
  post_return_data.wait();
  return post_return_data.get();
}
std::string Downloader::GetFsidAsync() {
  cpr::Header header_Fsid = cpr::Header{
      {"User-Agent",
       "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 "
       "(KHTML, like Gecko) Chrome/110.0.0.0 Safari/537.36"},
      {"Cookie", user_cookies_fs_id_ + randsk_},
  };

  auto Fsid = this->GetResultAsync(fs_id_url_, header_Fsid);
  Fsid.wait();
  return Fsid.get();
}
std::string Downloader::GetTimestampAndSignAsync() {
  cpr::Url timestampandsign{
      "https://pan.baidu.com/share/"
      "tplconfig?surl=" +
      long_url_ +
      "&fields=sign,timestamp&channel=chunlei&web=1&"
      "app_id=250528&bdstoken=" +
      bdstoken_ + "&clienttype=0"};
  auto TimestampAndSign = this->GetResultAsync(timestampandsign, header_);
  TimestampAndSign.wait();
  return TimestampAndSign.get();
}

std::string baiduyun::Downloader::GetDlinkAsync() {
  cpr::Url url_dlink{
      "https://pan.baidu.com/api/"
      "sharedownload?app_id=250528&channel=chunlei&clienttype=12&sign=" +
      sign_ +
      "&"
      "timestamp=" +
      timestamp_ + "&web=1"};

  cpr::Header dlink_header = cpr::Header{
      {"User-Agent",
       "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) "
       "AppleWebKit/537.36 (KHTML, like Gecko) Chrome/110.0.0.0 Safari/537.36"},
      {"Host", "pan.baidu.com"},
      {"Cookie", user_cookes_},
      {"Referer", "pan.baidu.com"}};

  cpr::Body body_dlink{"encrypt=0&extra=%7B%22sekey%22%3A%22" + randsk_ +
                       "%22%7D&product="
                       "share&uk=" +
                       uk_ + "&primaryid=" + shareid_ + "&fid_list=%5B" +
                       fs_id_ + "%5D"};

  std::cout << "randsk_: " << randsk_ << std::endl;
  std::cout << "uk_: " << uk_ << std::endl;
  std::cout << "shareid_: " << shareid_ << std::endl;
  std::cout << "fs_id_: " << fs_id_ << std::endl;
  std::cout << "user_cookes: " << user_cookes_ << std::endl;

  std::cout << "url_get_rds: " << url_dlink << std::endl;
  auto Dlink_data = this->PostResultAsync(url_dlink, dlink_header, body_dlink);
  Dlink_data.wait();
  return Dlink_data.get();
}

std::string baiduyun::Downloader::GetLocationLink() {
  cpr::Response r = cpr::Head(cpr::Url{dlink_});
  return r.raw_header;
}

cpr::AsyncWrapper<std::string, false> Downloader::GetResultAsync(
    const cpr::Url& url, const cpr::Header& Header) {
  auto response = cpr::GetCallback(
      [](cpr::Response r) {
        try {
          return r.text;
        } catch (const std::exception& e) {
          return r.text;
        }
      },
      url, Header, cpr::HttpVersion{cpr::HttpVersionCode::VERSION_2_0_TLS});
  return response;
}

cpr::AsyncWrapper<std::string, false> Downloader::PostResultAsync(
    const cpr::Url& url, const cpr::Header& Header, const cpr::Body& body) {
  auto response = cpr::PostCallback(
      [](cpr::Response r) {
        try {
          return r.text;
        } catch (const std::exception& e) {
          return r.text;
        }
      },
      url, Header, body,
      cpr::HttpVersion{cpr::HttpVersionCode::VERSION_2_0_TLS});
  return response;
}

}  // namespace baiduyun
