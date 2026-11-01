#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include <cpr/cpr.h>
#include <map>

using json = nlohmann::json; // same as import nlohmann::json as json


const std::string GetUrl = "https://openapi.api.govee.com/router/api/v1/user/devices";
const std::string PostControlUrl = "https://openapi.api.govee.com/router/api/v1/device/control";
const std::string PostStateUrl = "https://openapi.api.govee.com/router/api/v1/device/state";

json runPost( std::string url, std::string apiKey, json payload){
    cpr::Session session;
    session.SetUrl(cpr::Url{url});
    session.SetHeader(cpr::Header{{"Content-Type","application/json"},{"Govee-API-Key",apiKey}});
    session.SetBody({payload.dump()});
    cpr::Response r = session.Post();
    std::cout << r.status_code << " " << r.text << std::endl;
    return json::parse(r.text);
};


json runGET(std::string apiKey){
    cpr::Response r = cpr::Get(cpr::Url{GetUrl},
                            cpr::Header{{"Content-Type", "application/json"},{"Govee-API-Key",apiKey}});
    std::cout << r.status_code << std::endl;                  // 200
    std::cout << r.header["content-type"] << std::endl;       // application/json; charset=utf-8
    std::cout << r.text << std::endl;
    return json::parse(r.text);
}

// Read the config.json 
json GetStripParam(){
    std::ifstream file("config.json");
    if (!file.is_open()) {
        std::cerr << "Could not open config.json" << std::endl;
        return json::object();
    }
    return json::parse(file);
}

// Build the Payload for the Post
json BuildPayload(const json& param, std::string requestType, int value){
    json result = json::object();
    result["payload"]["sku"] = param.at("sku");
    result["payload"]["device"] = param.at("device");
    if (requestType == "control"){
        result["payload"]["capability"]["type"] = param.at("capability_type");
        result["payload"]["capability"]["instance"] = param.at("instance");
        result["payload"]["capability"]["value"] = value;
    }
    result["requestId"] = "uuid";
    return result;
}

// Find out whether the light is or not
int GetOnState(){
    json param = GetStripParam();
    std::string apiKey = param.at("apiKey");
    json payload = BuildPayload(param, "state",0);
    json PostResult = runPost(PostStateUrl, apiKey, payload);
    json capabilities = PostResult.at("payload").at("capabilities");
    // instead of hard coding the 
    int value =   capabilities[1].at("state").at("value");
    return value;
}

// If the light is on turns it off, if off turns it on
void powerSwitch(){
    json param = GetStripParam();
    std::string apiKey = param.at("apiKey");
    int target_value = (GetOnState()==1) ? 0 : 1;
    json payload = BuildPayload(param, "control",target_value);
    runPost(PostControlUrl,apiKey,payload);
}


int main() {
    powerSwitch();
    return 0;
}