#pragma once

#include "pch.h"
#define TO_CSTR(var) (#var)
constexpr const char* EMPTY_STR = "";

#define OB_SAFE_DELETE(ptr) if (ptr != nullptr) { delete (ptr); }
#define OB_BIND_FN(function) [this](auto&&... args)->decltype(auto) { return this->function(std::forward<decltype(args)>(args)...); }
