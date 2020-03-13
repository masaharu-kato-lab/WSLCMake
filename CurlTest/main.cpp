#include <iostream>
#include <fstream>
#include <vector>
#include <curl/curl.h>

size_t onReceive(char* ptr, size_t size, size_t nmemb, void* stream) {
    // streamはCURLOPT_WRITEDATAで指定したバッファへのポインタ
    std::vector<char>* recvBuffer = (std::vector<char>*)stream;
    const size_t sizes = size * nmemb;
    recvBuffer->insert(recvBuffer->end(), (char*)ptr, (char*)ptr + sizes);
    return sizes;
}

int main(int argc, const char* argv[]) {
    // curlのセットアップ
    CURL* curl = curl_easy_init();
    if (curl == nullptr) {
        curl_easy_cleanup(curl);
        return 1;
    }
    // レスポンスデータの格納先
    std::vector<char> responseData;
    // 接続先URL
    curl_easy_setopt(curl, CURLOPT_URL, "https://www.google.com");
    // サーバのSSL証明書の検証をしない
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
    // レスポンスのコールバック
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, onReceive);
    // 書き込みバッファを指定
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseData);

    // 通信実行
    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        curl_easy_cleanup(curl);
        return 1;
    }

    // 後始末
    curl_easy_cleanup(curl);

    std::ofstream ofs;
    ofs.open("google.html", std::ios::out);
    if (ofs.is_open()) {
        ofs << responseData.data() << std::endl;
    }
    return 0;
}
