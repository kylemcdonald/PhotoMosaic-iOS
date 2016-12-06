#pragma once

#include "SimpleCurl.h"
#include "ofxiOS.h"

class NetLogThread : public ofThread {
private:
    string logUrl, uuid;
    ofThreadChannel<std::pair<string, string>> toSend;
    
public:
    void setup(string logUrl) {
        this->logUrl = logUrl;
        this->uuid = ofxiOSNSStringToString([[[UIDevice currentDevice] identifierForVendor] UUIDString]);
    }
    void send(string msg) {
        string time = ofGetTimestampString("%Y-%m-%d %H:%M:%S.%i");
        toSend.send(std::pair<string, string>(time, msg));
    }
    
protected:
    void threadedFunction() {
        std::pair<string, string> timedMsg;
        while(toSend.receive(timedMsg)) {
            SimpleCurl req(logUrl);
            req.addField("uuid", uuid);
            req.addField("time", timedMsg.first);
            req.addField("msg", timedMsg.second);
            req.send();
        }
    }
};

class NetLog {
private:
    static NetLogThread thread;
    std::ostringstream msg;
    
public:
    static void setup(string logUrl) {
        thread.setup(logUrl);
        thread.startThread();
    }
    
    ~NetLog() {
        cout << msg.str() << endl;
        thread.send(msg.str());
    }
    template <class T>
    NetLog& operator<<(const T& value){
        msg << value;
        return *this;
    }
    NetLog& operator<<(std::ostream& (*func)(std::ostream&)){
        func(msg);
        return *this;
    }
};
