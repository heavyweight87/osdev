#ifndef _CLIPBOARD_HPP
#define _CLIPBOARD_HPP

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <iterator>

#include <util/rpc_serialization.hpp>

namespace btos_api{
namespace clipboard{

struct ContentHeader{
	uint64_t id;
	std::string type;
	size_t size;
	
	ContentHeader() = default;
	ContentHeader(const std::string &t, size_t s) : type(t), size(s) {}
};

enum class Clipboard{
	Primary, DragDrop
};

namespace RPCID{
	const uint32_t GetContentHeader = 1;
	const uint32_t CopyCut = 2;
	const uint32_t Paste = 3;
	const uint32_t Clear = 4;
}

inline void serialize(std::ostream &s, const ContentHeader &h){
	rpc::serialize(s, h.id);
	rpc::serialize(s, h.type);
	rpc::serialize(s, h.size);
}

inline void serialize(std::ostream &s, Clipboard c){
	rpc::serialize(s, (int)c);
}

inline void deserialize(std::istream &s, ContentHeader &h){
	rpc::deserialize(s, h.id);
	rpc::deserialize(s, h.type);
	rpc::deserialize(s, h.size);
}

inline void deserialize(std::istream &s, Clipboard &c){
	rpc::deserialize(s, (int&)c);
}

template<typename T> std::vector<char> RPCSerialize(const T &data){
	std::stringstream ss;
	using rpc::serialize;
	serialize(ss, data);
	auto str = ss.str();
	return {str.begin(), str.end()};
}

template<typename T> T RPCDeserialize(const std::vector<char> &data){
	std::stringstream ss;
	std::copy(data.begin(), data.end(), std::ostream_iterator<char>(ss));
	ss.seekg(0);
	T ret;
	using rpc::deserialize;
	deserialize(ss, ret);
	return ret;
}

ContentHeader GetContentHeader(Clipboard c);
uint64_t CopyCut(Clipboard c, const std::string &type, const std::vector<char> &data, bool linkedBack = false);
std::vector<char> Paste(Clipboard c, uint64_t id);
void Clear(Clipboard c, uint64_t id);

}
}

#endif