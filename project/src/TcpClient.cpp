#include "TcpClient.h"

UserRegistration::UserRegistration(std::unique_ptr<User> user) {

}

void UserRegistration::GetDataFromUser() {

}

void UserRegistration::sendEmail(std::string email) {

}

void UserRegistration::uploadUser(std::string) {

}

void User::output(std::string_view text) {

}

User::User(const std::string_view name, const std::vector<std::string> &tag) {

}

bool User::is_connected() {
    return false;
}

void UserAuthorization::getUserData(std::string request) {

}

bool UserAuthorization::compareSignInData(sign_data user) {
    return false;
}

void actionManager::del_tag(const std::unique_ptr<User> user) {

}

void actionManager::add_tag(const std::unique_ptr<User> user) {

}

void actionManager::upload_own_song(std::unique_ptr<User> user, std::unique_ptr<std::string> path_to_mp3) {

}

void actionManager::play_sound() {

}

void actionManager::main_action(size_t &choice) {

}

actionServer::actionServer() {

}

int actionServer::response(std::string request) {
    return 0;
}

void actionServer::connectClient() {

}

void actionServer::closeConnectClient() {

}
