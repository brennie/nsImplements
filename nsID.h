#ifndef NSID_H
#define NSID_H

#include <cstring>
#include <ios>
#include <iomanip>
#include <ostream>

struct nsID {
  uint32_t m0;
  uint16_t m1;
  uint16_t m2;
  uint8_t m3[8];
};

bool operator==(const nsID& id, const nsID& otherId) {
  return std::memcmp(&id, &otherId, sizeof(nsID)) == 0;
}

std::ostream&
operator<<(std::ostream& s, const nsID& id) {
  return s << std::hex
      << std::setfill('0')
      << std::setw(8)
      << id.m0 << "-"
      << std::setw(4)
      << id.m1 << "-"
      << id.m2 << "-"
      << std::setw(2)
      << uint16_t(id.m3[0]) << "-"
      << uint16_t(id.m3[1]) << "-"
      << uint16_t(id.m3[2]) << "-"
      << uint16_t(id.m3[3]) << "-"
      << uint16_t(id.m3[4]) << "-"
      << uint16_t(id.m3[5]) << "-"
      << uint16_t(id.m3[6]) << "-"
      << uint16_t(id.m3[7]);
}


#endif  // NSID_H
