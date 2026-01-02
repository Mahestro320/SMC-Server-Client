#include "network/with_client.hpp"

#include "network/client.hpp"

void WithClient::setClient(Client* client) {
    this->client = client;
}
