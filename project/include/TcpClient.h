#include <iostream>
#include <unistd.h>
#include <string>
#include <memory>
#include <string_view>
#include <vector>
#include <boost/asio.hpp>
#include <utility>


struct sign_data{
    std::string login;
    std::string password;
};

enum actions {
    ADD = 1,
    DEL,
    UPLOAD,
    PLAY
};

class User {
private:
    int id;
    const char* nickname = "guest";
    std::vector<std::string> personal_tags;
public:
    boost::shared_ptr<boost::asio::ip::tcp::socket> sock;
    bool is_connected();
    explicit User(std::string_view name = "guest", const std::vector<std::string> &tag = std::vector<std::string>());
    User(int ID, boost::shared_ptr<boost::asio::ip::tcp::socket> socket) {
        id = ID;
        sock = std::move(socket);

    }
    virtual ~User() = default;

protected:
    void output(std::string_view text);
};

class UserRegistration {
public:
    UserRegistration() = default;
    UserRegistration(std::unique_ptr<User> user);

    void GetDataFromUser();

private:
    void sendEmail(std::string email);
    void uploadUser(std::string);
};



class UserAuthorization {
public:
    UserAuthorization() = default;
private:
    void getUserData(std::string request);
    bool compareSignInData (sign_data user);
};


class actionManager: public User {
    User user;
public:
    void del_tag(const std::unique_ptr<User> user);
    void add_tag(const std::unique_ptr<User> user);
    void upload_own_song(std::unique_ptr<User> user, std::unique_ptr<std::string> path_to_mp3);
    void play_sound();
    void main_action(size_t &choice);
};

class actionServer: public User {
private:
    User network_user;

    std::string addr_server = "127.0.0.1";
    int port = 8001;

public:
    actionServer();
    ~actionServer() = default;

    int Id;
    int response(std::string request);
    void connectClient();
    void closeConnectClient();
};