#pragma once

#ifndef GORGONZOLA_API
#if defined(_WIN32)
    #if defined(GORGONZOLA_EXPORT)
        #define GORGONZOLA_API __declspec(dllexport)
    #else
        #define GORGONZOLA_API __declspec(dllimport)
    #endif
#else
    #if defined(GORGONZOLA_EXPORT)
        #define GORGONZOLA_API __attribute__((visibility("default")))
    #else
        #define GORGONZOLA_API
    #endif
#endif
#endif
