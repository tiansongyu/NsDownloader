#ifndef __TINY_NETDISK_HPP__
#define __TINY_NETDISK_HPP__

// cpp
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <map>
#include <list>
#include <regex>
#include <thread>
#include <bitset>
#include <sstream>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <filesystem>

// c
#include <cstdlib>

// third-party
#include <json.hpp>
#include <cpr/cpr.h>
#include <openssl/md5.h>
#include <openssl/sha.h>

#define LOG_ENABLE true
#define _TinyN_LOG(fd, ...) if (LOG_ENABLE) fprintf (fd, __VA_ARGS__); fprintf (fd, "\033[0m\n")
#define TinyN_LOGI(...) fprintf (stdout, "\033[32mTinyN LOG [I]: \t%s:  ", __func__); _TinyN_LOG(stdout, __VA_ARGS__)
#define TinyN_LOGD(...) fprintf (stdout, "\033[37mTinyN LOG [D]: \t%s:  ", __func__); _TinyN_LOG(stdout, __VA_ARGS__)
#define TinyN_LOGW(...) fprintf (stdout, "\033[33mTinyN LOG [W]: \t%s:  ", __func__); _TinyN_LOG(stdout, __VA_ARGS__)
#define TinyN_LOGE(...) fprintf (stderr, "\033[31mTinyN LOG [E]: \t%s:  ", __func__); _TinyN_LOG(stderr, __VA_ARGS__)
#define TinyN_LOG_INIT() TinyN_LOGD("Init Done.")


namespace TinyN {

struct UTILS {
    
    UTILS() = delete;

    static int nowTimestamp() {
        const auto now = std::chrono::system_clock::now();
        return std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count();
    }

    static auto IEMICompute(const std::string key) -> std::string {
        uint64_t hs = 53202347234687234;

        for (const auto& k : key) {
            hs += (hs << 5) + static_cast<uint64_t>(k);
        }

        hs %= static_cast<uint64_t>(1e15);
        if (hs < static_cast<uint64_t>(1e14)) {
            hs += static_cast<uint64_t>(1e14);
        }

        std::stringstream ss;
        ss << std::setfill('0') << std::setw(15) << std::bitset<60>(hs).to_ullong();
        return ss.str();
    }

    static auto MD5SUM(std::string key) -> std::string {
        return __compute<MD5_DIGEST_LENGTH>(key, MD5);
    }

    static auto SHA1SUM(std::string key) -> std::string {
        return __compute<SHA_DIGEST_LENGTH>(key, SHA1);
    }

private:
    template <size_t CBufSize, typename Computer>
    static auto __compute(std::string key, Computer computer) -> std::string {

        unsigned char cbuf[CBufSize];

        computer(reinterpret_cast<const unsigned char*>(key.data()), key.size(), cbuf);

        std::stringstream ss;
        ss << std::hex << std::setfill('0');
        for (const auto& byte : cbuf) {
            ss << std::setw(2) << static_cast<unsigned int>(byte);
        }

        return ss.str();
    }
};

enum NetdiskNode : int8_t {
    LIST = 1,
    DOWNLOAD = 2,
};

// TODO: Configer, Data Driver from config file
class ResourceManager {

public:

    ResourceManager() { init(); TinyN_LOG_INIT(); }

    static std::shared_ptr<ResourceManager> Instance() {
        static std::shared_ptr<ResourceManager> rm { new ResourceManager() };
        return rm;
    }

    std::string getUrl(NetdiskNode nn) {
        return __mUrlSet[nn];
    }

private:
    std::map<NetdiskNode, std::string> __mUrlSet;

    void init() {
        const static std::string BD_PCS_SITE =  "https://pcs.baidu.com";
        const static std::string BD_PAN_SITE = "https://pan.baidu.com";

        __mUrlSet[LIST]       = BD_PAN_SITE + "/api/list";
        //__mUrlSet[DOWNLOAD]   = BD_PAN_SITE + "/api/download";
        __mUrlSet[DOWNLOAD]   = BD_PCS_SITE + "/rest/2.0/pcs/file";
    }
};

class NetdiskClient;

class NetdiskFileManager {
public:
    struct FNode {
        int id;
        std::string name;
        std::string path;
        int size;
        std::list<std::shared_ptr<FNode>> files;
        bool isDir() const { return size < 0; }
    };

public:
    NetdiskFileManager(std::shared_ptr<NetdiskClient> ndc) : __mNetdisk { ndc } {
        __mRootNode = std::make_shared<FNode>();
        __mRootNode->id = 0;
        __mRootNode->name = __mRootNode->path = "/";
        __mRootNode->size = -1;
        __updateDirTree(__mRootNode);
        __mCurrentNode = __mRootNode;
        TinyN_LOG_INIT();
    }

public:
    const std::string workDir();
    void updateWorkDir(const std::string &dir, bool updateCache = false);
    std::vector<FNode> list() const ;
    void downloadFile(std::string remoteFile, const std::string &localFile);
    void uploadFile(const std::string file, const std::string &dir);
    void moveFile(const std::string src, const std::string &dst);
    void deleteFile(const std::string file);
    void renameFile(const std::string src, const std::string &dst);

private:
    std::shared_ptr<NetdiskClient> __mNetdisk;
    std::shared_ptr<FNode> __mRootNode, __mCurrentNode;
    std::mutex __mMutex;

    void __updateDirTree(std::shared_ptr<FNode> fNode, bool block = false, int LayerLimit = 5);
};

class Account {
public:
    Account(const std::string &cookiesStr) : __mCookiesStr { cookiesStr } {
        const std::regex regex(R"((\w+)=(\w+);?)");
        std::sregex_iterator iter(__mCookiesStr.cbegin(), __mCookiesStr.cend(), regex);
        std::sregex_iterator end;
        while (iter != end) {
            __mCookies[(*iter)[1].str()] = (*iter)[2].str();
            TinyN_LOGD("regex: %s = %s", (*iter)[1].str().c_str(), (*iter)[2].str().c_str());
             ++iter;
        }
        TinyN_LOG_INIT();
    }

public:

    void updateUserInfo(std::string id, std::string name) {
        __mUser.id = id;
        __mUser.name = name;
    }

    auto getUserID() const { return __mUser.id; }
    std::string getCookiesStr() const { return __mCookiesStr; }
    std::map<std::string, std::string> getCookies() const { return __mCookies; }
    std::string getUsername() const { return __mUser.name; }

private:
    
    struct User {
        std::string id;
        std::string name;
        int currentUseSpace, maxUseSpace;
    };

    User __mUser;
    /* cpr::Cookies */ std::string __mCookiesStr; // TODO: use cpr::Cookies
    std::map<std::string, std::string> __mCookies;
};

class NetdiskClient {
public:
    NetdiskClient();
    NetdiskClient(const Account &account) : __mAccount { account } {
        TinyN_LOG_INIT();
    }

public:
    virtual void fileListImpl(NetdiskFileManager::FNode &fNode) const = 0;
    virtual std::string downloadLinksImpl(const std::string &file, bool pcs = false) const = 0;
    virtual void uploadFileImpl(const std::string &file, const std::string &dir) const { (void)file; (void)dir; };
    virtual void delFileFileImpl(const std::string &file) const { (void)file; };
    virtual void moveFileToImpl(const std::string &src, const std::string dst) const { (void)src; (void)dst; };
    virtual void updateStatus() const { };

protected:
    Account __mAccount;
    virtual void _updateUserInfo() { /* Update __mAccount.__mUser */ };
};


#define PCS_BAIDU_COM "https://pcs.baidu.com"
#define PAN_BAIDU_COM "https://pan.baidu.com"

#define PCS_UA "softxm;netdisk"
#define PAN_UA "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_14_6) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/77.0.3865.75 Safari/537.36"

#define PCS_HEADERS {"User-Agent", PCS_UA}
#define PAN_HEADERS {"User-Agent", PAN_UA}

#define PCS_APP_ID "778750"
#define PAN_APP_ID "250528"

class BaiduNetdiskClient : public NetdiskClient {
public:
    BaiduNetdiskClient(const Account &account)
        : NetdiskClient { account } {
        __mHeader = cpr::Header {
            PAN_HEADERS,
            {"Cookie", __mAccount.getCookiesStr()},
        };
        _updateUserInfo(/* __mAccount.__mUser */);
        TinyN_LOG_INIT();
    };

public:
    void fileListImpl(NetdiskFileManager::FNode &dirNode) const override {

        auto path = dirNode.path;
        auto isRoot = dirNode.name == "/";
        cpr::Response resp;
        //std::atomic_bool wait = false;

        if (!dirNode.isDir()) return;

        //TinyN_LOGI("req path: %s", path.c_str());
        int retry = 10;

        while (--retry > 0) {
            // Note: sleep in lock
            // while (wait) std::this_thread::sleep_for(std::chrono::milliseconds(100)); // wait a little
            // wait = false;

            try {
                resp = cpr::Get(
                    cpr::Url{ResourceManager::Instance()->getUrl(NetdiskNode::LIST)},
                    __mHeader,
                    cpr::Parameters {
                        {"dir", path},
                        {"order", "time"},
                    }
                );
            } catch (std::exception& e) {
                TinyN_LOGE(" request file list failed: %s", e.what());
                //wait = false;
                return;
            }

            if (resp.status_code == 443 || resp.status_code == 0) {
                TinyN_LOGW("%s list req failed, remaind retry: %d, error: %s", dirNode.path.c_str(), retry, resp.error.message.c_str());
                continue;
            }

            if (resp.status_code != 200) {
                TinyN_LOGE("%s list req failed, status_code %ld, error: %s", dirNode.path.c_str(), resp.status_code, resp.error.message.c_str());
                return;
            } else {
                break;
            }
        }
        //TinyN_LOGI("resp text: %s", resp.text.c_str());

        nlohmann::json jsonResp;
        try {
            jsonResp = nlohmann::json::parse(resp.text);
        } catch (const nlohmann::detail::parse_error& e) {
            TinyN_LOGW("Error parsing JSON: %s", e.what());
            return;
        }


        auto fileList = jsonResp["list"];

        dirNode.files.clear();
        
        for (auto & file : fileList) {
            auto node = std::make_shared<NetdiskFileManager::FNode>();
            node->name = file["server_filename"];
            node->path = path + (isRoot ? "" : "/") + node->name;
            node->size = file["isdir"] != 0 ? -1 : file["size"].get<int>();
            node->id = file["fs_id"];
            dirNode.files.push_back(std::move(node));
        }

        TinyN_LOGD("dir: %s, file nums: %ld", dirNode.path.c_str(), dirNode.files.size());

    }

    std::string downloadLinksImpl(const std::string &remotePath, bool pcs = false) const override {
        
        int retry = 40;
        auto bduss = __mAccount.getCookies()["BDUSS"];
        auto uid = __mAccount.getUserID();
        auto enc = UTILS::MD5SUM(__mAccount.getCookies()["BDUSS"]);
        auto devUid = enc + "|0";
        std::string magic = "ebrcUYiuxaZv2XGu7KIYKxUrqfnOfpDF"; // ?

        auto dlUrl = ResourceManager::Instance()->getUrl(NetdiskNode::DOWNLOAD);
        std::string downloadLink = "https://github.com/Sunrisepeak";

        (void)pcs;

        while (retry--) {
            std::string timestamp = std::to_string(UTILS::nowTimestamp());

            auto rand = UTILS::SHA1SUM(enc + uid + magic + timestamp + devUid);
            
            auto resp = cpr::Get(
                cpr::Url{ dlUrl },
                cpr::Header { 
                    PCS_HEADERS,
                    { "Cookie", __mAccount.getCookiesStr() }
                },
                cpr::Parameters { // auto cpr::util::urlDecode
                    {"apn_id", "1_0"},
                    {"app_id", PAN_APP_ID},
                    {"channel", "0"},
                    {"check_blue", "1"},
                    {"clienttype", "17"},
                    {"es", "1"},
                    {"esl", "1"},
                    {"freeisp", "0"},
                    {"method", "locatedownload"},
                    {"path", remotePath},
                    {"queryfree", "0"},
                    {"use", "0"},
                    {"ver", "4.0"},
                    {"time", timestamp},
                    {"rand", rand},
                    {"devuid", devUid},
                    {"cuid", devUid},
                }
            );

            if (resp.status_code != 200) {
                TinyN_LOGW("req url: %s", resp.url.c_str());
                TinyN_LOGW("resp: status code: %ld, text: %s", resp.status_code, resp.text.c_str());
                std::this_thread::sleep_for(std::chrono::milliseconds(1500));
                continue;
            }

            try {
                static auto jsonResp = nlohmann::json::parse(resp.text);
                downloadLink = jsonResp.value("urls", nlohmann::json::object())[0]["url"];
                break;
            } catch (std::exception &e) {
                TinyN_LOGE("%s", e.what());
            }
        }

        //TinyN_LOGD("%s", downloadLink.c_str());

        return downloadLink; // TODO: list
    }

/*
    void uploadFileImpl(const std::string &remotePath, const std::string &dir) const override { }
    void delFileFileImpl(const std::string &file) const override { }
    void moveFileToImpl(const std::string &src, const std::string dst) const override { }
*/

private:

    cpr::Header __mHeader;

    void _updateUserInfo() override {
        auto bduss = __mAccount.getCookies()["BDUSS"];
        auto timestamp = std::to_string(UTILS::nowTimestamp());
        auto iemi = UTILS::IEMICompute(bduss);

        std::map<std::string, std::string> data = {
            {"bdusstoken", bduss + "|null"},
            //{"channel_id", ""},
            //{"channel_uid", ""},
            {"stErrorNums", "0"},
            {"subapp_type", "mini"},
            {"timestamp", timestamp + "922"}
        };

        data["_client_type"] =  "2";
        data["_client_version"] =  "7.0.0";
        data["_phone_imei"] =  iemi;
        data["from"] =  "NyniT";
        data["model"] = "MI6";
        data["cuid"] =  UTILS::MD5SUM(
                bduss + "_" +
                data["_client_version"] + "_" +
                data["_phone_imei"] + "_" +
                data["from"]
        ) + "|" + iemi;

        data["sign"] = UTILS::MD5SUM([&]() -> std::string {
                std::string result;
                for (const auto& [key, value] : data) {
                    result += key + "=" + value;
                }
                result += "tiebaclient!!!";
                return result;
            }()
        );

        std::map<std::string, std::string> headers = {
            {"Content-Type", "application/x-www-form-urlencoded"},
            {"Cookie", "ka=open"},
            {"net", "1"},
            {"User-Agent", "bdtb for Android 6.9.2.1"},
            {"client_logid", timestamp + "416"},
            {"Connection", "Keep-Alive"}
        };
        
        auto bodyString = [&]() -> std::string {
                std::string result;
                for (const auto& [key, value] : data) {
                    result += key + "=" + value + "&";
                }
                result += "NyniT=1";
                return result;
        }();
        
        const auto resp = cpr::Post(
            cpr::Url{"http://tieba.baidu.com/c/s/login"},
            cpr::Header(headers.begin(), headers.end()),
            cpr::Payload(data.begin(), data.end())
        );
        
        //std::cout << resp.text << std::endl;

        nlohmann::json jsonResp;
        try {
            jsonResp = nlohmann::json::parse(resp.text);
            __mAccount.updateUserInfo(
                jsonResp.value("user", nlohmann::json::object()).value("id", ""),
                jsonResp.value("user", nlohmann::json::object()).value("name", "")
            );
        } catch (const nlohmann::detail::parse_error& e) {
            TinyN_LOGE("Error parsing JSON: %s", e.what());
            return;
        }

        TinyN_LOGI("userId: %s, userName: %s", __mAccount.getUserID().c_str(), __mAccount.getUsername().c_str());

    }

    void __update() {

    }
};

void NetdiskFileManager::updateWorkDir(const std::string &dir, bool updateCache) {

    if (dir[0] == '/') {// abs path reset
        
        bool exit = false;
        auto fListPtr = __mRootNode;

        while (fListPtr->files.size() && !exit) {
            
            TinyN_LOGD("Enter: %s", fListPtr->path.c_str());
            
            exit = true;

            for (auto node : fListPtr->files) {
                if (node->isDir()) {
                    if (node->path == dir) {
                        __mCurrentNode = node;
                        break;
                    } else if (dir.find(node->path) != std::string::npos) {
                        fListPtr = node;
                        exit = false;
                        break;
                    }
                }
            }
        }
    } else {

        bool found = false;
        
        for (auto node : __mCurrentNode->files) {
            TinyN_LOGW(" file name: %s, dir name: %s", node->name.c_str(), dir.c_str());
            if (node->name == dir) {
                __mCurrentNode = node; // TODO: ?
                found = true;
                break;
            }
        }

        if (!found) {
            updateCache = true;
            TinyN_LOGW("%s not found, start update cache...", dir.c_str());
        }
    } 

    if (updateCache)
        __updateDirTree(__mCurrentNode, true, 2);
}

std::vector<NetdiskFileManager::FNode> NetdiskFileManager::list() const {
    NetdiskFileManager::FNode node;
    std::vector<NetdiskFileManager::FNode> nodeVec;
    for (auto f : __mCurrentNode->files) {
        
        //TinyN_LOGD("file name: %s", f->name.c_str());

        node.id = f->id;
        node.name = f->name;
        node.path = f->path;
        node.size = f->size;

        nodeVec.push_back(node);
    }
    return nodeVec;
}

void NetdiskFileManager::__updateDirTree(std::shared_ptr<FNode> fNode, bool block, int LayerLimit) {


    if (LayerLimit <= 0 && !fNode->isDir()) return;

    if (!block) {

        std::thread([ & ]() {
            this->__updateDirTree(fNode, true);
        }).detach();
        
        TinyN_LOGI("start thread to update dir tree...");

    } else {

        std::lock_guard<std::mutex> al(__mMutex);
        auto nextLayerFirstNode = fNode;
        std::queue<decltype(fNode)> nodeQ;

        nodeQ.push(fNode);

        while (!nodeQ.empty() && LayerLimit > 0) {
            auto fN = nodeQ.front(); nodeQ.pop();

            __mNetdisk->fileListImpl(*fN);

            for (auto node : fN->files)
                if (LayerLimit > 0)
                    nodeQ.push(node);

            if (nextLayerFirstNode == fN) {
                LayerLimit--;
                nextLayerFirstNode = fN->files.front();
                TinyN_LOGI("enter next layer(lmt %d): %s -> %s", LayerLimit, fN->name.c_str(), nextLayerFirstNode->name.c_str());
            }
        }

    }

}

void NetdiskFileManager::downloadFile(std::string remoteFile, const std::string &localFile) {

    if (remoteFile[0] != '/')
        remoteFile = (__mCurrentNode->path != "/" ? __mCurrentNode->path : "") + "/" + remoteFile;

    cpr::Session session;

    session.SetUrl(__mNetdisk->downloadLinksImpl(remoteFile));

    std::ofstream ofs(localFile, std::ios::binary);
    
    auto writeCB = cpr::WriteCallback(
        [](std::string data, intptr_t outFile) -> bool {
            std::ofstream* ofsTmp = reinterpret_cast<std::ofstream*>(outFile);
            ofsTmp->write(data.data(), data.size());
            TinyN_LOGD("blk size: %ld", data.size());
            return true;
        },
        reinterpret_cast<intptr_t>(&ofs)
    );

    auto rep = session.Download(writeCB);
    
    TinyN_LOGI("dl status: %ld, remote file: %s, local file: %s",
        rep.status_code,
        remoteFile.c_str(),
        std::filesystem::absolute(std::filesystem::path(localFile)).c_str()
    );

    ofs.close();

}

template<typename Client>
std::shared_ptr<NetdiskFileManager> fileManagerFactory(std::string cookies) {
    static auto acc = std::make_shared<Client>(cookies);
    static auto fm = std::make_shared<NetdiskFileManager>(acc);
    return fm;
}

}; // namespace TBN

#endif