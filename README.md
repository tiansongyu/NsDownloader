# NsDownloader
ns文件下载工具

### 功能

- ns自制程序，百度云下载工具

### 目前进度

- 目前项目是一个baiduyun的download库

# TODO list
[详细列表](https://github.com/users/tiansongyu/projects/2/views/1)
- [x] ~downloader库的基本下载功能模块实现~
- [ ] downloader库的基本下载功能模块代码重构，更易用
- [ ] 百度云加速原理？
- [ ] 移植到NS


# 开发
``` shell
# 拉取代码
git clone --recursive https://github.com/tiansongyu/NsDownloader
cd NsDownloader
``` 
## 开发PC程序
### linux 

``` shell
# pre-install 
sudo apt update && sudo apt install openssl ssl libssl-dev libcurl curl libcurl4-openssl-dev libcurl4-openssl-dev 

# build 
mkdir build && cd build && cmake .. && make -j12 
./cpr_demo

# bash 版本
bash bash.sh
```
##  开发switch程序

``` bash
# linux
docker run -i -t  -v  $(pwd):/data   devkitpro/devkita64  /bin/bash
# windows
docker run -i -t  -v  /d/dockerfile:/home/data   devkitpro/devkita64  /bin/bash
```
### ns版本开发学习资料
- [ns官方demo示例](https://github.com/switchbrew/switch-examples)
- [ns开发仓库](https://github.com/switchbrew/libnx)
- [ns自制程序官网](https://switchbrew.org/wiki/Main_Page)
- [ns应用开发指南](https://switch.homebrew.guide/homebrew_dev/app_dev.html)
### PC端开源ns模拟器，支持linux windows mac所有设备
- 下载[yuzu](https://yuzu-emu.org/)  
- 安装yuzu
- [将yuzu设置为联网模式](https://qianp.com/knowledge/46747.html)
- 加载nro文件(ns上的应用程序格式)
- [nx-python文档，ns上运行python](https://nx-python.readthedocs.io/en/latest/index.html)


# Acknowledgement

- https://github.com/whoshuu/cpr
- https://github.com/nlohmann/json
- https://github.com/switchbrew/libnx
- https://github.com/switchbrew/switch-examples
