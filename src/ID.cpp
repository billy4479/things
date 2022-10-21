#include "ID.hpp"

#include <spdlog/fmt/fmt.h>

#include <random>

static std::random_device device;
static std::mt19937_64 engine(device());
static std::uniform_int_distribution<u64> distribution;

ID::ID() : m_ID(distribution(engine)) {}

ID::ID(u64 uuid) : m_ID(uuid) {}

ID::operator u64() const { return m_ID; }

std::string ID::ToString() const { return fmt::format("{:08x}", m_ID); }
