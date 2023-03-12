#include <TinyNetdisk.hpp>

void traversalDirTree(const TinyN::NetdiskFileManager::FNode &fNode, std::string indent = "", int layerLimit = 3) {
    if (!layerLimit) return;

    if (fNode.isDir()) {
        std::cout << indent << fNode.name << "    absPath: " << fNode.path << "    file nums: " << fNode.files.size() <<  std::endl;
        for (auto f : fNode.files)
            traversalDirTree(*f, indent + "    ", layerLimit--);
    }
#if 0
    else {
        std::cout << "start downloading: " << fNode.path << ", size " << fNode.size << std::endl;
        fm->downloadFile(fNode.path, "./hello.data");
    }
#endif
}

int main() {

    std::string userCookes =
        "BDUSS="
        "TdZTFJqbXNYa1psd3lSOTFjOWpmRi0tMVg3VzI5bm9QY2xMYk44c3ZXc1N6NmRqS"
        "VFBQUF"
        "BJCQAAAAAAAAAAAEAAABi4udFbmZ5MjQwOQAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
        "AAAAAA"
        "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABJCgGMSQoBja;"
        "STOKEN="
        "db98458575d0ddbd9ecd94424b1a312cdc82ead56919797311045eb9f168b663;";

    std::string yourDir = "/";

    auto fm =  TinyN::fileManagerFactory<TinyN::BaiduNetdiskClient>(userCookes);
    
    // default work dir is '/'
    fm->downloadFile("emummc.txt", "emummc.txt");
        
    fm->updateWorkDir(yourDir /*, false */ );  // not found

    std::this_thread::sleep_for(std::chrono::milliseconds(2000));

    auto nodes = fm->list();

    std::cout << yourDir << ":\n";
    for (auto n : nodes) {
        std::cout << "    " << n.name << std::endl;
    }
    
    return 0;
}