#pragma once

#include "ofMain.h"

// Based on https://curl.haxx.se/libcurl/c/postit2.html
// And https://github.com/roxlu/ofxCurl/blob/master/src/ofxCurlForm.cpp

#include <curl/curl.h>
class SimpleCurl {
private:
    CURL *curl = NULL;
    struct curl_httppost *formpost = NULL;
    struct curl_httppost *lastptr = NULL;
    struct curl_slist *headerlist = NULL;
    
public:
    std::string url;
    int timeoutAfterMs;
    std::string response;
    
    SimpleCurl(std::string url = "")
    :url(url)
    ,timeoutAfterMs(15000) {
        curl_global_init(CURL_GLOBAL_ALL);
    }
    ~SimpleCurl() {
        if(curl) {
            curl_easy_cleanup(curl);
            curl_formfree(formpost);
            curl_slist_free_all (headerlist);
        }
    }
    void addFile(std::string key, std::string filename) {
        filename = ofToDataPath(filename);
        curl_formadd(&formpost,
                     &lastptr,
                     CURLFORM_COPYNAME, key.c_str(),
                     CURLFORM_FILE, filename.c_str(),
                     CURLFORM_END);
    }
    void addField(std::string key, std::string value) {
        curl_formadd(&formpost,
                     &lastptr,
                     CURLFORM_COPYNAME, key.c_str(),
                     CURLFORM_COPYCONTENTS, value.c_str(),
                     CURLFORM_END);
    }
    static size_t writeFunction(char* data, size_t data_size, size_t num_data_size_entries, void* userdata) {
        size_t real_num_bytes = data_size * num_data_size_entries;
        SimpleCurl* req = static_cast<SimpleCurl*>(userdata);
        std::copy(data, data+real_num_bytes, std::back_inserter(req->response));
        return real_num_bytes;
    }
    bool send() {
        curl = curl_easy_init();
        static const char buf[] = "Expect:";
        headerlist = curl_slist_append(headerlist, buf);
        if(curl) {
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);
            curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, timeoutAfterMs);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, SimpleCurl::writeFunction);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);
            CURLcode res = curl_easy_perform(curl);
            if(res != CURLE_OK) {
                cout << "curl_easy_perform() failed: " << curl_easy_strerror(res) << endl;
                return false;
            }
            return true;
        } else {
            cout << "problem initializing curl" << endl;
            return false;
        }
    }
};
