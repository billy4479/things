
#pragma once

class ID {
  public:
    ID();
    ID(u64);

    operator u64() const;

    std::string ToString() const;

  private:
    u64 m_ID;
};

namespace std {

template <typename T> struct hash;

template <> struct hash<ID> {
    size_t operator()(const ID &id) const { return std::hash<u64>{}(id); }
};
} // namespace std