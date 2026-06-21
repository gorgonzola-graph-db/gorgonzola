#pragma once

// Helpers
#if defined _WIN32 || defined __CYGWIN__
#define GORGONZOLA_HELPER_DLL_IMPORT __declspec(dllimport)
#define GORGONZOLA_HELPER_DLL_EXPORT __declspec(dllexport)
#define GORGONZOLA_HELPER_DLL_LOCAL
#define GORGONZOLA_HELPER_DEPRECATED __declspec(deprecated)
#else
#define GORGONZOLA_HELPER_DLL_IMPORT __attribute__((visibility("default")))
#define GORGONZOLA_HELPER_DLL_EXPORT __attribute__((visibility("default")))
#define GORGONZOLA_HELPER_DLL_LOCAL __attribute__((visibility("hidden")))
#define GORGONZOLA_HELPER_DEPRECATED __attribute__((__deprecated__))
#endif

#ifdef GORGONZOLA_STATIC_DEFINE
#define GORGONZOLA_API
#else
#ifndef GORGONZOLA_API
#ifdef GORGONZOLA_EXPORTS
/* We are building this library */
#define GORGONZOLA_API GORGONZOLA_HELPER_DLL_EXPORT
#else
/* We are using this library */
#define GORGONZOLA_API GORGONZOLA_HELPER_DLL_IMPORT
#endif
#endif
#endif

#ifndef GORGONZOLA_DEPRECATED
#define GORGONZOLA_DEPRECATED GORGONZOLA_HELPER_DEPRECATED
#endif

#ifndef GORGONZOLA_DEPRECATED_EXPORT
#define GORGONZOLA_DEPRECATED_EXPORT GORGONZOLA_API GORGONZOLA_DEPRECATED
#endif
