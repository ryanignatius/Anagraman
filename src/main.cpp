#include <algorithm>

#include <pistache/http.h>
#include <pistache/router.h>
#include <pistache/endpoint.h>
#include <pistache/client.h>
#include <pistache/stream.h>

#include "json.hpp"
#include "crow_all.h"

#include "restclient-cpp/connection.h"
#include "restclient-cpp/restclient.h"

using json = nlohmann::json;
using namespace std;
using namespace Pistache;

class StatsEndpoint {
public:
    StatsEndpoint(Address addr)
        : httpEndpoint(std::make_shared<Http::Endpoint>(addr))
    { }

    void init(size_t thr = 2) {
        RestClient::init();
        auto opts = Http::Endpoint::options()
            .threads(thr)
            .flags(Tcp::Options::InstallSignalHandler | Tcp::Options::ReuseAddr);
        httpEndpoint->init(opts);
        setupRoutes();
    }

    void start() {
        httpEndpoint->setHandler(router.handler());
        httpEndpoint->serve();
    }

    void shutdown() {
        RestClient::disable();
        httpEndpoint->shutdown();
    }

private:
    std::string ENDPOINT = "https://graph.facebook.com/v2.6/me/messages?access_token=EAACaA7moyIUBANyaZCyAmQpZCeLoPViwS46dniLZCMSU2bU1ZC3SvWcKrfMECUwPvLy7ZBZBVSfI6iiRsL9ZCYKZB3Gi77ZAlZATU2ZAjSZA5XsdWZBGJv1Bynkpb7Eej7hLEjLHCrbmOoU6EkZCZC2a03TI2pjQZCm45QKGKkUKhYmYHm8I4iSs0660ZBSqi";
    std::shared_ptr<Http::Endpoint> httpEndpoint;
    Rest::Router router;

    void setupRoutes() {
        using namespace Rest;

        Routes::Get(router, "/facebook", Routes::bind(&StatsEndpoint::doGet, this));
        Routes::Post(router, "/facebook", Routes::bind(&StatsEndpoint::doPost, this));
    }

    void doGet(const Rest::Request& request, Http::ResponseWriter response) {
        std::cout << "get request from facebook" << std::endl;
        auto mode = request.query().get("hub.mode").getOrElse("");
        auto challenge = request.query().get("hub.challenge").getOrElse("");
        auto token = request.query().get("hub.verify_token").getOrElse("");
        if (token == "anagraman") {
            response.send(Http::Code::Ok, challenge);
        } else {
            response.send(Http::Code::Ok, "Invalid Token");
        }
    }

    void doPost(const Rest::Request& request, Http::ResponseWriter response) {
        std::cout << "receive message from facebook" << std::endl;
        auto body = request.body();
        json j = json::parse(body);
        std::cout << "request body: " << j.dump(4) << std::endl;
        respondMessage(j);
        response.send(Http::Code::Ok, "");
    }

    void respondMessage(const json& j) {
        if (!j.count("entry")) return;
        json entryArr = j["entry"];
        if (!entryArr.is_array()) return;
        json entry = entryArr[0];
        if (!entry.count("messaging")) return;
        json messagingArr = entry["messaging"];
        if (!messagingArr.is_array()) return;
        json messaging = messagingArr[0];
        if (!messaging.count("sender")) return;
        json sender = messaging["sender"];
        if (!sender.count("id")) return;
        if (!messaging.count("message")) return;
        json message = messaging["message"];
        if (!message.count("text")) return;
        sendReply(sender["id"], message["text"]);
    }

    void sendReply(std::string sender, std::string reply) {
        json j = {
            {"messaging_type", "response"},
            {"recipient", {
                {"id", sender}
            }},
            {"message",{
                {"text", reply}
            }}
        };
        std::cout << "response: " << j.dump() << std::endl;
        std::cout << "response: " << j.dump(4) << std::endl;
        RestClient::post(ENDPOINT, "application/json", j.dump());
    }
};

/*
void handleGet(const crow::SimpleApp& app) {
    CROW_ROUTE(app, "/facebook")
    ([](const crow::request& req){
        std::ostringstream os;

        // To get a simple string from the url params
        // To see it in action /params?foo='blabla'
        os << "Params: " << req.url_params << "\n\n"; 
        os << "The key 'foo' was " << (req.url_params.get("foo") == nullptr ? "not " : "") << "found.\n";

        // To get a double from the request
        // To see in action submit something like '/params?pew=42'
        if(req.url_params.get("pew") != nullptr) {
            double countD = boost::lexical_cast<double>(req.url_params.get("pew"));
            os << "The value of 'pew' is " <<  countD << '\n';
        }

        // To get a list from the request
        // You have to submit something like '/params?count[]=a&count[]=b' to have a list with two values (a and b)
        auto count = req.url_params.get_list("count");
        os << "The key 'count' contains " << count.size() << " value(s).\n";
        for(const auto& countVal : count) {
            os << " - " << countVal << '\n';
        }

        // To get a dictionary from the request
        // You have to submit something like '/params?mydict[a]=b&mydict[abcd]=42' to have a list of pairs ((a, b) and (abcd, 42))
        auto mydict = req.url_params.get_dict("mydict");
        os << "The key 'dict' contains " << mydict.size() << " value(s).\n";
        for(const auto& mydictVal : mydict) {
            os << " - " << mydictVal.first << " -> " << mydictVal.second << '\n';
        }

        return crow::response{os.str()};
    });

    std::cout << "get request from facebook" << std::endl;
    auto mode = request.query().get("hub.mode").getOrElse("");
    auto challenge = request.query().get("hub.challenge").getOrElse("");
    auto token = request.query().get("hub.verify_token").getOrElse("");
    if (token == "anagraman") {
        response.send(Http::Code::Ok, challenge);
    } else {
        response.send(Http::Code::Ok, "Invalid Token");
    }
}
*/

int main(int argc, char *argv[]) {
    crow::SimpleApp app;

    CROW_ROUTE(app, "/")([](){
        return "Hello world";
    });

    app.port(9080).multithreaded().run();
    return 0;
}

