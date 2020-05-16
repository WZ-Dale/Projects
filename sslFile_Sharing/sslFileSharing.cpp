#include "server.hpp"
#include "client.hpp"

void srv_start(){
  P2PServer server("./cert.pem", "./key.pem");
  server.Start(9000);
}


int main(){
  std::thread thr(srv_start);
  thr.detach();
  P2PClient client(9000);
  client.Start();
  return 0;
}
