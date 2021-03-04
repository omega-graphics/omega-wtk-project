
#if defined(OMEGAWTK_BUILD_MODULE) && __cplusplus >= 202002L
#define OMEGAWTK_EXPORT export
#else

#ifdef TARGET_MACOS
#define OMEGAWTK_EXPORT
#endif

#ifdef TARGET_WIN32
// #define OMEGAWTK_EXPORT __declspec( dllexport)
#define OMEGAWTK_EXPORT
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
