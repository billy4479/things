
#pragma once

class UUID {
  public:
    UUID();
    UUID(u64);

    operator u64() const;

    std::string ToString() const;

  private:
    u64 m_UUID;
};

namespace std {

template <typename T> struct hash;

template <> struct hash<UUID> {
    size_t operator()(const UUID &uuid) const { return std::hash<u64>{}(uuid); }
};
} // namespace std