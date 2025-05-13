#include <nit/embedded/networking/tcp/protocol_legacy.hpp>

#include <cinttypes>
#include <iomanip>
#include <iostream>

using namespace nit::embedded::neworking;

std::ostream &operator<<(std::ostream &os, packet &req) {
  // os
  // << "{ "
  // << "\"route_id\": 0x%s", std::hex << std::setw(sizeof(uint16_t)) <<
  // std::setfill('0') << (int) req.route_id_get() << ", "
  // << "\"endpoint\": 0x%s", std::hex << std::setw(sizeof(uint8_t)) <<
  // std::setfill('0') << (int) req.endpoint << ", "
  // << "\"rw\": 0x%s", std::hex << std::setw(sizeof(uint8_t)) <<
  // std::setfill('0') << (int) req.rw << ", "
  // << "\"group\": 0x%s", std::hex << std::setw(sizeof(uint8_t)) <<
  // std::setfill('0') << (int) req.group << ", "
  // << "\"value\": 0x%s", std::hex << std::setw(sizeof(uint16_t)) <<
  // std::setfill('0') << (int) req.value
  // << "}";
  return os;
}