all::sslFileSharing
sslFileSharing:sslFileSharing.cpp
	g++ -std=c++11 -DCPPHTTPLIB_OPENSSL_SUPPORT $^ -o $@ -lboost_filesystem -lboost_system -lpthread -lboost_thread -lssl -lcrypto
