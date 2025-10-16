#include "roomProxy.hpp"
RoomProxy::RoomProxy(const std::string &roomFile):_id(RoomProxy::parseRoomID(roomFile)),_roomFile(roomFile) {
  

}