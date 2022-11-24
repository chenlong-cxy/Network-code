#pragma once

//请求
typedef struct request{
    int x;
    int y;
    char op;
}request_t;

//响应
typedef struct response{
    int code;
    int result;
}response_t;

