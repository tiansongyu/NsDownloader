#! /usr/bin
###################################################################################################
# bdstoken:  a5ce92ccd232616767d94706d13a01e2
# 链接: https://pan.baidu.com/s/1RiOVjWgK3Rlrtsyj0k0iSA?pwd=ezuk 提取码: ezuk 复制这段内容后打开百度网盘手机App，操作更方便哦
###################################################################################################

set -eux

docker_compose_command=$(command -v jq)
if [ -z $docker_compose_command ]; then
    sudo apt update &&
        sudo apt install jq
fi

###################################################################################################
# 请编辑下面的变量
long_url="1RiOVjWgK3Rlrtsyj0k0iSA"
password="ezuk"
# user_cookes="BDUSS=3FoV3BBdzlqYXdqaGNJRGFQQW9WMTU2REVLRX4tNVpONDNWdXpCSjBIc2pjeGxrRVFBQUFBJCQAAAAAAAAAAAEAAAAmh5VOzOzQq-Pl4-XEwsTCAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAACPm8WMj5vFjU; H_BDCLCKID_SF=tbC8VCDKJKD3qbjkq45HMt00qxby26nkWJR9aJ5y-J7nhhjdX-583jcX5-Rf5xv9MmoaLfLbQpbZql5FQP-53R0h0PJkWp52JIFqKl0MLPb5hj6gQJoDj4TyDMnMBMPjamOnaPLy3fAKftnOM46JehL3346-35543bRTLnLy5KJYMDFRDjuBDj5WDHRabK6aKC5bL6rJabC3fI3VXU6q2bDeQN3CaU7a2HTnQPTcbbj18PT8-Rj4Dp0vWq54WbbvLT7johRTWqR4HIbIjxonDh83KNLLKUQtKJcBoKJO5hvvhb6O3M7-eMKmDloOW-TB5bbPLUQF5l8-sq0x0bOte-bQXH_E5bj2qRuH_K_K3H; H_BDCLCKID_SF_BFESS=tbC8VCDKJKD3qbjkq45HMt00qxby26nkWJR9aJ5y-J7nhhjdX-583jcX5-Rf5xv9MmoaLfLbQpbZql5FQP-53R0h0PJkWp52JIFqKl0MLPb5hj6gQJoDj4TyDMnMBMPjamOnaPLy3fAKftnOM46JehL3346-35543bRTLnLy5KJYMDFRDjuBDj5WDHRabK6aKC5bL6rJabC3fI3VXU6q2bDeQN3CaU7a2HTnQPTcbbj18PT8-Rj4Dp0vWq54WbbvLT7johRTWqR4HIbIjxonDh83KNLLKUQtKJcBoKJO5hvvhb6O3M7-eMKmDloOW-TB5bbPLUQF5l8-sq0x0bOte-bQXH_E5bj2qRuH_K_K3H; H_PS_PSSID=37779_36544_37558_38105_38093_38055_37906_38145_38267_38176_38170_38035_37927_38088_37900_26350_38120_38099_38008_37881; newlogin=1; PSINO=2; PSTM=1669795125; ZFY=EL3yBPy0oP8nQdWD5dLE:Ba04EE8DzS9acUEU3AfWSN8:C; BDCLND=66xPuyb0Nn+oGGjfNNtACHKczt+lZk6ZQ5BIIbrq7cw=; Hm_lpvt_7a3960b6f067eb0085b7f96ff5e660b0=1677389512; Hm_lvt_7a3960b6f067eb0085b7f96ff5e660b0=1675826528,1677340894,1677389329; PANPSC=17163607244613478564:inYEMI9z4kqZvP+LsTZtwFcS2d9ns3O5g0mIZdLHpdQGbqupDlB1goY3sPpDsegU2vhMfiT95/7iLaLUl0KD5De4ZdaW7CoOlL98c8Ccr9ch6uZoP3DwQ9YfJggg9xZJ4xp4QFjbVLUD17U05rclDNl/E8U7MKEk6cKtYSnhZrwTou/rY8UDg+Fpa28bKzm23GZ3PiThrcTlNGmCtkiR2NWlis3+nrf0Go/tPUFuQrj11A0lR01yRcaseoCJwDeL7W2GwAYIg0mSsVZaqyA8TA==;STOKEN=544dce74cfc9aea9067c9993083d8502d11d725d9e5ab8e45899fe22cdd99ad0; csrfToken=P0KKOYdL96ocRyK4blyqpHIb; ndut_fmt=17F774C974753BDE93613200D3B827048FB10A457E5397AA6B702E354746D0F5"
user_cookes="BDUSS=TdZTFJqbXNYa1psd3lSOTFjOWpmRi0tMVg3VzI5bm9QY2xMYk44c3ZXc1N6NmRqSVFBQUFBJCQAAAAAAAAAAAEAAABi4udFbmZ5MjQwOQAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABJCgGMSQoBja; STOKEN=db98458575d0ddbd9ecd94424b1a312cdc82ead56919797311045eb9f168b663"
dir="/重要文件"
file_name='idmchslsttb.zip'
###################################################################################################

###################################################################################################
# 不要修改以下内容
user_cookes="${user_cookes};ndut_fmt="
shorturl=${long_url#?}

# 1. 取bdstoken
curl -s 'https://pan.baidu.com/api/loginStatus?clienttype=0&app_id=250528&web=1' \
    -H "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/110.0.0.0 Safari/537.36" \
    -H "Cookie: $user_cookes" \
    --compressed >bdstoken.json

bdstoken=$(jq -r '.login_info.bdstoken' <bdstoken.json)
echo "bdstoken: $bdstoken"

###################################################################################################
# 2. 取分享链接的shareid和uk
curl -s "https://pan.baidu.com/api/shorturlinfo?shorturl=$long_url" \
    -H "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/110.0.0.0 Safari/537.36" \
    --compressed >shareid_and_uk.json
uk=$(jq -r '.uk' <shareid_and_uk.json)
shareid=$(jq -r '.shareid' <shareid_and_uk.json)

echo "uk: $uk"
echo "shareid: $shareid"

###################################################################################################
# 3.获取分享链接信息，首先要得到链接的randsk
curl -s "https://pan.baidu.com/share/verify?surl=$shorturl" \
    -H "Referer: https://pan.baidu.com/wap/init?surl=$shorturl" \
    -H "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/110.0.0.0 Safari/537.36" \
    --data-raw 'pwd=ezuk' \
    --compressed >randsk.json

randsk=$(jq -r '.randsk' <randsk.json)
echo "randsk: $randsk"
###################################################################################################
# 4.得到文件列表fs_id
curl -s "https://pan.baidu.com/share/list?order=name&desc=1&showempty=0&web=1&page=1&num=100&clienttype=0&shorturl=$shorturl&dir=$dir" \
    -H "Cookie: BDCLND=$randsk" \
    -H "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/110.0.0.0 Safari/537.36" \
    >file_lists.json
fs_id=$(jq -r ".list[] | select(.server_filename == \"${file_name}\") | .fs_id" <file_lists.json)
echo "fs_id: $fs_id"

###################################################################################################
# 5.得到timestamp和sign
curl -s "https://pan.baidu.com/share/tplconfig?surl=$long_url&fields=sign,timestamp&channel=chunlei&web=1&app_id=250528&bdstoken=$bdstoken&clienttype=0" \
    -H "Cookie: $user_cookes" \
    -H "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/110.0.0.0 Safari/537.36" \
    >timestamp.json
timestamp=$(jq -r '.data.timestamp' <timestamp.json)
sign=$(jq -r '.data.sign' <timestamp.json)
echo "timestamp is $timestamp"
echo "sign is $sign"

###################################################################################################
# 6.获取dlink
curl -s "https://pan.baidu.com/api/sharedownload?app_id=250528&channel=chunlei&clienttype=12&sign=$sign&timestamp=$timestamp&web=1" \
    -H "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/110.0.0.0 Safari/537.36" \
    -H 'Host: pan.baidu.com' \
    -H "Cookie: $user_cookes" \
    -H 'Referer: pan.baidu.com' \
    --data-raw "encrypt=0&extra=%7B%22sekey%22%3A%22$randsk%22%7D&product=share&uk=$uk&primaryid=$shareid&fid_list=%5B$fs_id%5D" \
    >dlink.json
dlink=$(jq -r '.list[0].dlink' <dlink.json)
echo "dlink is $dlink"

###################################################################################################
# 6.下载文件

curl -s -f -I $dlink >final_url.txt
# wget -U LogStatistic -i $dlink -O $file_name
location=$(grep -oP 'Location: \K.*' <final_url.txt)
echo $location >final_url_2.txt

wget -i final_url_2.txt -O $file_name

exit 0
