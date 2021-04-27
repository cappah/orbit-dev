#pragma once

#include <set>
#include <map>
#include <vector>
#include <string>
#include <bitset>
#include <memory>
#include <stdio.h>
#include <cassert>
#include <iostream>
#include <typeinfo> 
#include <sstream>
#include <fstream>
#include <stdlib.h>
#include <algorithm>
#include <filesystem>
#include <functional>
#include <unordered_map>

#ifdef PLATFORM_WINDOWS
	#include <Windows.h>

	#ifdef OB_BUILD_DLL
		#define OB_MASHALL_API extern "C" __declspec(dllexport)
		#define	OB_API __declspec(dllexport)
	#else
		#define	OB_API __declspec(dllimport)
	#endif 
#else
	#error no support for window platform
#endif

#ifdef OPENGL_RENDERER_API
	#include <GL/glew.h>
	#include <GLFW/glfw3.h>
#endif

#ifdef COMPILER_MSVC
	#define FORCEINLINE __forceinline
#elif defined(COMPILER_GCC) || defined(COMPILER_CLANG)
	#define FORCEINLINE inline __attribute__ ((always_inline))
#else
	#define FORCEINLINE inline
#endif

#if defined(OB_DEBUG)
	#define ASSERT assert  
	#define STATIC_ASSERT static_assert

	#define DEBUG_LOG(level, msg, ...) \
			fprintf(stderr, "[%s] ", level); \
			fprintf(stderr, msg, ##__VA_ARGS__); \
			fprintf(stderr, "\n");

	#define OB_INFO(msg, ...) DEBUG_LOG("info", msg, ##__VA_ARGS__)
	#define OB_ERROR(msg, ...) DEBUG_LOG("error", msg, ##__VA_ARGS__)
	#define OB_WARNING(msg, ...) DEBUG_LOG("warning", msg, ##__VA_ARGS__)
#else
	#define NDEBUG 
	#define ASSERT
	#define OB_INFO
	#define OB_ERROR	
	#define OB_WARNING
	#define STATIC_ASSERT
#endif

using int8 = int8_t;
using int16 = int16_t;
using int32 = int32_t;
using int64 = int64_t;

using uint8 = uint8_t;
using uint16 = uint16_t;
using uint32 = uint32_t;
using uint64 = uint64_t;

template<typename T>
using Unique = std::unique_ptr<T>;
template<typename T, typename ... Args>
FORCEINLINE constexpr std::unique_ptr<T> MakeUnique(Args&& ... args) {
	return std::make_unique<T>(std::forward<Args>(args)...);
}

template<typename T>
using Shared = std::shared_ptr<T>;
template<typename T, typename ... Args>
FORCEINLINE constexpr std::shared_ptr<T> MakeShared(Args&& ... args) {
	return std::make_shared<T>(std::forward<Args>(args)...);
}

template<typename T1, typename T2>
using uMap = std::unordered_map<T1, T2>;
