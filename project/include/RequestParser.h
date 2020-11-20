#if !defined(PROJECT_INCLUDE_REQUESTPARSER_H)
#define PROJECT_INCLUDE_REQUESTPARSER_H

#include <string>

typedef enum {
    UNKNOWN
    ,GET
    ,POST
}HTTPMethod;

struct request_HTTP_data {
    int size;
    int keep_alive;
    char* body;
    char* file_name;
    char* file_extension;
    HTTPMethod type;
};



class ParseRequest {
public:
    ParseRequest(std::string request);

    void url_parse(char* request, struct  request_HTTP_data* req_data);
    void body_parse(char* request, struct  request_HTTP_data* req_data);
    void extension_parse(struct  request_HTTP_data* req_data);
    void parse_http_full(char* request, struct  request_HTTP_data* req_data); // concat 3 previous
private:
    std::string http_request;
};



#endif // PROJECT_INCLUDE_REQUESTPARSER_H