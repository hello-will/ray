/**
 * @file    backtrace.hpp
 * @version 1.0
 * @author  hello_will@yeah.net
 * data     2016/11/18 16:04:58
**/

#ifndef __BASE_BACKTRACE_HPP_20161118160458_E59351BF_1BA6_4AFB_9A56_7DC7E600F30A__
#define __BASE_BACKTRACE_HPP_20161118160458_E59351BF_1BA6_4AFB_9A56_7DC7E600F30A__

#if defined(WIN32)||defined(_WIN32)
    #include <windows.h>
    #include <dbghelp.h>
    #include <iostream>
#elif defined(__linux__)
    #include <stdio.h>
    #include <stdlib.h>
    #include <execinfo.h>
    #include <iostream>
#endif

namespace ray
{
    template<int __max>
    struct backtrace
    {
        void  *buff[__max];
        size_t size;

        backtrace( void )
            : size( 0 )
        {}
    };
} /* namespace ray */

#if defined(WIN32)||defined(_WIN32)
    #define BACKTRACE(__max) \
            { \
                auto __max = sizeof(__backtrace.buff)/sizeof(void*); \
                __backtrace.size = CaptureStackBackTrace(0,__max,__backtrace.buff,nullptr); \
            }
    #define BACKTRACE_WRITE(__backtrace,__prefix,__stream) \
            { \
                DWORD __dword = 0; \
                DWORD __dword64 = 0; \
                IMAGEHLP_LINE __line; \
                BYTE __buff[512] = {0}; \
                SYMBOL_INFO __symbol = (SYMBOL_INFO*)__buff; \
                HANDLE __process = GetCurrentProcess(); \
                \
                __line.SizeOfStruct = sizeof(IMAGEHLP_LINE); \
                __symbol->SizeOfStruct = sizeof(SYMBOL_INFO); \
                __symbol->MaxNameLen = sizeof(buf) - sizeof(SYMBOL_INFO) + 1; \
                for ( auto __i = 0; __i < __backtrace.size; ++__i ) \
                { \
                    if ( SymGetLineFromAddr64(__process,(DWORD64)__backtrace.buff[__i],&__dword,&__line) ) \
                    { \
                        if ( SymFromAddr(__process,(DWORD64)__backtrace.buff[__i],&__dword64,__symbol) ) \
                        { \
                            __stream << __prefix \
                                     << "0x" \
                                     << __backtrace.buff[__i] \
                                     << " " \
                                     << __symbol->Name \
                                     << " " \
                                     << __line.FileName \
                                     << "(" \
                                     << __line.LineNumber \
                                     << ")" \
                                     << std::endl; \
                        } \
                        else \
                        { \
                            __stream << __prefix \
                                     << "0x" \
                                     << __backtrace.buff[__i] \
                                     << " " \
                                     << __symbol->Name \
                                     << std::endl; \
                        } \
                    } \
                    else if ( SymFromAddr(__process,(DWORD64)__backtrace.buff[__i],&__dword64,__symbol) ) \
                    { \
                        __stream << __prefix \
                                 << "0x" \
                                 << __backtrace.buff[__i] \
                                 << " " \
                                 << __symbol->Name \
                                 << std::endl; \
                    } \
                } \
            }
#elif defined(__linux__)
    #define BACKTRACE(__backtrace) \
            { \
                auto __max = sizeof(__backtrace.buff)/sizeof(void*); \
                __backtrace.size = ::backtrace(__backtrace.buff,__max); \
            }
    #define BACKTRACE_WRITE(__backtrace,__prefix,__stream) \
            { \
                char __name[256] = {'\0'};\
                char __func[256] = {'\0'}; \
                char **__symbols = ::backtrace_symbols(__backtrace.buff,__backtrace.size); \
                for ( auto __i = 0; __i < __backtrace.size; ++__i ) \
                { \
                    if ( nullptr != __symbols[__i] ) \
                    { \
                        auto rc = sscanf( __symbols[__i], "%[^(](%[^)])", __name, __func ); \
                        if ( rc > 0 ) \
                        { \
                                __stream << __prefix \
                                         << __backtrace.buff[__i] \
                                         << " " \
                                         << __func \
                                         << " " \
                                         << __name \
                                         << std::endl; \
                        } \
                    } \
                } \
                ::free(__symbols); \
            }
#endif

#endif /* __BASE_BACKTRACE_HPP_20161118160458_E59351BF_1BA6_4AFB_9A56_7DC7E600F30A__ */
