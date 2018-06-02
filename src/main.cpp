#include <unordered_map>
#include <algorithm>
#include <sstream>
#include "generator/generator.h"
#include "game/game.h"
#include "json.hpp"
#include "crow_all.h"
#include "restclient-cpp/connection.h"
#include "restclient-cpp/restclient.h"

using json = nlohmann::json;

const std::string ENDPOINT = "https://graph.facebook.com/v2.6/me/messages?access_token=EAACaA7moyIUBANyaZCyAmQpZCeLoPViwS46dniLZCMSU2bU1ZC3SvWcKrfMECUwPvLy7ZBZBVSfI6iiRsL9ZCYKZB3Gi77ZAlZATU2ZAjSZA5XsdWZBGJv1Bynkpb7Eej7hLEjLHCrbmOoU6EkZCZC2a03TI2pjQZCm45QKGKkUKhYmYHm8I4iSs0660ZBSqi";
const std::string SLACK_ENDPOINT = "https://slack.com";
const std::string SLACK_TOKEN = "xoxb-4767970651-373663936705-GoKhJSXhjpTX06EO27TQtW5L";

std::unordered_map<std::string, int> userStates;
std::unordered_map<std::string, problem> userProblems;
std::unordered_map<std::string, game> games;
std::unordered_map<std::string, std::string> slackPlayers;
generator mgenerator;

void sendMessageSlack(const std::string& channel, const std::string& text) {
	json j = {
		{ "channel", channel },
		{ "text", text },
		{ "link_names", 1 }
	};

	std::cout << j.dump() << std::endl;
	RestClient::Connection* conn = new RestClient::Connection(SLACK_ENDPOINT);
	conn->AppendHeader("Content-Type", "application/json; charset=utf-8");
	conn->AppendHeader("Authorization", "Bearer " + SLACK_TOKEN);
	RestClient::Response resp = conn->post("/api/chat.postMessage", j.dump());
	std::cout << resp.body << std::endl;
}

std::string resolveSlackPlayer(const std::string& user_id) {
	json j = {
		{ "user", user_id },
		{ "token", SLACK_TOKEN }
	};
	std::ostringstream sout;
	sout << "user=" << user_id << "&token=" << SLACK_TOKEN;
	std::string payload = sout.str();

	RestClient::Connection* conn = new RestClient::Connection(SLACK_ENDPOINT);
	conn->AppendHeader("Content-Type", "application/x-www-form-urlencoded; charset=utf-8");
	RestClient::Response resp = conn->post("/api/users.info", payload);
	std::cout << resp.body << std::endl;
	return json::parse(resp.body)["user"]["name"];
}

std::vector<std::string> getReplySlack(const json& event) {
	std::string key = event["channel"];
	std::string text = event["text"];
	std::string user_id = event["user"];

	if (slackPlayers.find(user_id) == slackPlayers.end()) {
		std::string name = resolveSlackPlayer(user_id);
		slackPlayers[user_id] = name;
	}

	if (text == "//help") {
		std::ostringstream sout;
		sout << "`//help` to show this help message\n";
		sout << "`//play` to start\n";
		sout << "`//rescramble` to rescramble the question\n";
		sout << "`//skip` to change the question\n";
		sout << "`//leaderboard` to show the leaderboard\n";
		sout << "`//end` to end the game\n";
		sout << "`//status to check whether there is a game\n";
		sout << "Non-alphanumerics are ignored, so both `esteler` and `es teler` would be a correct answer for `seteler`\n";
		return { sout.str() };
	}

	if (text == "//play") {
		if (games.find(key) == games.end()) {
			games[key] = game();
			return { "Hi I'm Anagraman! Let's play", games[key].get_problem() };
		} else {
			return { "There is a running session, send me `//end` to end the game or `//skip` if you want to change the question" };
		}
	}

	if (text == "//status") {
		if (games.find(key) == games.end()) {
			return { "There is no game at the moment" };
		} else {
			return { "The game is on" };
		}
	}

	if (games.find(key) == games.end()) {
		return { "" };
	}

	for (auto p: slackPlayers) {
		games[key].add_player(p.first, p.second);
	}
	
	if (text == "//rescramble") {
		return { games[key].rescramble() };
	} else if (text == "//skip") {
		std::ostringstream sout;
		sout << "The answer is " << games[key].get_ground() << "\n";
		sout << "Next:";
		return { sout.str(), games[key].get_next_problem() };
	} else if (text == "//leaderboard") {
		std::vector<player> players = games[key].get_players();
		std::ostringstream sout;
		for (const player& p: players) {
			sout << p.get_display_name() << " " << p.get_score() << "\n";
		}
		return { sout.str() };
	} else if (text == "//end") {
		std::vector<player> players = games[key].get_players();
		std::ostringstream sout;
		sout << "Cheers!\n\n";
		sout << "Final Leaderboard:\n";
		for (const player& p: players) {
			sout << p.get_display_name() << " " << p.get_score() << "\n";
		}
		games.erase(key);
		// slackPlayers.clear();
		return { sout.str() };
	}

	if (games[key].answer(event["user"], text)) {
		std::string user = event["user"];
		std::ostringstream sout;
		sout << "@" << slackPlayers[user];
		sout << " correct!";
		return { sout.str() , games[key].get_next_problem() };
	}

	return { "" };
}

void sendReply(const std::string& sender, const std::string& reply) {
    json j = {
        {"messaging_type", "response"},
        {"recipient", {
            {"id", sender}
        }},
        {"message",{
            {"text", reply}
        }}
    };
    RestClient::post(ENDPOINT, "application/json", j.dump());
}

std::string getReply(const std::string& sender, const std::string& text) {
    if (text == "//play") {
        userStates[sender] = 1;
        problem prob = mgenerator.get_problem();
        userProblems[sender] = prob;
        std::cout << prob.get_ground() << std::endl;
        std::cout << prob.get_scrambled() << std::endl;
        return prob.get_scrambled();
    }

    int userState = userStates[sender];
    if (userState == 0) {
        return "Hello.. Please type '/play' to start playing..";
    } else if (userState == 1) {
        problem prob = userProblems[sender];
        if (prob.check(text)) {
            // correct
            userStates[sender] = 0;
            return "Correct! Congratulations!";
        } else {
            // wrong
            return "Wrong.. Please try again..";
        }
    }
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
    std::cout << "text: " << message["text"] << std::endl;
    std::string reply = getReply(sender["id"], message["text"]);
    sendReply(sender["id"], reply);
}

crow::response handleGet(const crow::request& req) {
    std::string mode = "";
    if (req.url_params.get("hub.mode") != nullptr) {
        mode = boost::lexical_cast<std::string>(req.url_params.get("hub.mode"));
    }
    std::string challenge = "";
    if (req.url_params.get("hub.challenge") != nullptr) {
        challenge = boost::lexical_cast<std::string>(req.url_params.get("hub.challenge"));
    }
    std::string token = "";
    if (req.url_params.get("hub.verify_token") != nullptr) {
        token = boost::lexical_cast<std::string>(req.url_params.get("hub.verify_token"));
    }
    std::string response;
    if (token == "anagraman") {
        response = challenge;
    } else {
        response = "Invalid Token";
    }
    return crow::response{ response };
}

crow::response handlePost(const crow::request& req) {
    json j = json::parse(req.body);
    respondMessage(j);
    return crow::response(200);
}

crow::response handlePostSlack(const crow::request& req) {
	json j = json::parse(req.body);
	std::cout << j.dump() << std::endl;

	if (j.count("challenge")) {
		std::string challenge = j["challenge"];
		return crow::response(challenge);
	} else {
		json event = j["event"];
		if (event.count("bot_id")) {
			return crow::response(200);
		}
		std::vector<std::string> replies = getReplySlack(event);
		if (replies.size()) {
			for (const std::string& reply: replies) {
				sendMessageSlack(event["channel"], reply);
			}
		}
		return crow::response(200);
	}
}

int main(int argc, char *argv[]) {
    RestClient::init();
    crow::SimpleApp app;

    CROW_ROUTE(app, "/facebook").methods(crow::HTTPMethod::Get, crow::HTTPMethod::Post)
    ([](const crow::request& req) {
        if (req.method == crow::HTTPMethod::Get) {
            std::cout << "get request from facebook" << std::endl;
            return handleGet(req);
        } else if (req.method == crow::HTTPMethod::Post) {
            std::cout << "receive message from facebook" << std::endl;
            return handlePost(req);
        } else {
            return crow::response(405);
        }
    });

	CROW_ROUTE(app, "/slack").methods(crow::HTTPMethod::Get, crow::HTTPMethod::Post)
	([](const crow::request& req) {
		if (req.method == crow::HTTPMethod::Post) {
			std::cout << "New slack message" << std::endl;
			return handlePostSlack(req);
		} else {
			return crow::response(405);
		}
	});

    app.port(9080).multithreaded().run();
    return 0;
}

