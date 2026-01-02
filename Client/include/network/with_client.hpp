#pragma once

class Client;

class WithClient {
  protected:
    Client* client;

  public:
    void setClient(Client* client);
};
