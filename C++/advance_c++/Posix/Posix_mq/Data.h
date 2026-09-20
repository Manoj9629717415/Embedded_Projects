#ifndef MESSAGEQUEUE_DATA_H
#define MESSAGEQUEUE_DATA_H

#include <cstdint>

const char* queue_name = "/vehicle_queue";
const char* request_queue_name = "/server_queue";
const char* client_response_prefix = "/client";
const char* client_response_suffix = "_queue";
const int COMMAND_RPM = 1;
const int COMMAND_SPEED = 2;
const int POSITIVE_RESPONSE_OFFSET = 40;
const int NEGATIVE_RESPONSE_OFFSET = 70;
const int Client1 = 0;
const int client2 = 1;
const uint8_t number_of_clients = 2;

enum class ResponseType : u_int8_t{
    Positive,
    Negative
};

enum class RequestId : u_int8_t{
    DID_0x100,
    DID_0x101,
    DID_0x102
};

struct VehicleMessage{
    int rpm;
    int speed;

};

struct VehicleRequest
{
    uint8_t command;
    uint8_t request_id;
    uint8_t client_id;
};

struct VehicleResponse
{
    ResponseType response_code;
    uint8_t request_id;
    uint16_t value;
};

#endif