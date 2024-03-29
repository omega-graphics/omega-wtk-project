#ifndef OMEGAWTKEXPORT_H
#define OMEGAWTKEXPORT_H
#if defined(OMEGAWTK_BUILD_MODULE) && __cplusplus >= 202002L
#define OMEGAWTK_EXPORT export
#else

#if defined(TARGET_MACOS) || defined(TARGET_GTK) || defined(TARGET_IOS) || defined(TARGET_ANDROID)
#define OMEGAWTK_EXPORT
#endif

#if defined(TARGET_WIN32) && defined(OMEGAWTK_APP)
#define OMEGAWTK_EXPORT __declspec(dllimport)
#elif defined(TARGET_WIN32)
#define OMEGAWTK_EXPORT __declspec(dllexport)
#endif

#if defined(TARGET_WIN32)
#define _CRT_SECURE_NO_WARNINGS 1
#endif

#endif

/// OMEGAWTK_DEPRECATED Macro
#if __cplusplus >= 201402L
/// Use c++14 attributes
#define OMEGAWTK_DEPRECATED [[deprecated]]
#elif defined(__GNUC__) || defined(__clang__)
/// Below c++14, for GNU and Clang.
#define OMEGAWTK_DEPRECATED __attribute__((deprecated))
#elif defined(_MSC_VER)
/// Below c++14, for MSVC
#define OMEGAWTK_DEPRECATED __declspec(deprecated)
#endif

#endif
