#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <set>
#include <memory>
#include <map>
#include <btos/bt_msg.h>
#include "surface.hpp"

extern std::map<uint64_t, std::weak_ptr<Surface>> allSurfaces;

namespace btos_api{
	class Message;
}

class Client {
private:
	std::set<std::shared_ptr<Surface>> surfaces;
	std::shared_ptr<Surface> currentSurface;
public:
	Client();
	~Client();
	
	void ProcessMessage(const Message &msg);
};

void Service(bt_pid_t root_pid);

#endif // CLIENT_HPP
