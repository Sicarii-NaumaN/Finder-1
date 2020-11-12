#if !defined(PROJECT_INCLUDE_APLLICATION_H)
#define PROJECT_INCLUDE_APLLICATION_H
#include <memory>

#include "TcpClient.h" // Устанавливает соединение с пользователем, получает и отправляет запросы
#include "RequestParser.h" // Парсит запрос пользователя
#include "Database.h" // Берет данные из БД
#include "FileServer.h" // 
#include "HTMLGenerator.h" // На основе полученных данных генерирует страницу

class Application {

private:

    std::unique_ptr<TcpClient> tcp_client;
    std::unique_ptr<RequestParser> parser;
    std::unique_ptr<Database> database;
    std::unique_ptr<FileServer> fileserver;
    std::unique_ptr<HtmlGenerator> page_creator;

public:

    Application();

    // Приложение начинает слушать порт
    void start();

    // Остановка приложения
    void stop();
};

#endif // PROJECT_INCLUDE_APLLICATION_H