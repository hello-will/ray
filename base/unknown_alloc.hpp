/**
 * @file    unknown_alloc.hpp
 * @version 1.0
 * @author  hello_will@yeah.net
 * @date    2016/09/21 11:58:18
**/

#ifndef __BASE_UNKNOWN_ALLOC_HPP_20160921115827_C3783185_924A_4E10_80C1_75126F071D20__
#define __BASE_UNKNOWN_ALLOC_HPP_20160921115827_C3783185_924A_4E10_80C1_75126F071D20__

#include <base/non_copyable.hpp>

namespace ray
{
    template<typename __type>
    class unknown_alloc : public non_copyable
    {
    public:
        typedef __type mem_type;

        unknown_alloc( void ) = default;
        virtual ~unknown_alloc( void ) = default;

        /**
         * alloc memory
        **/
        virtual mem_type alloc( size_t sz, const void *ptr = nullptr ) = 0;

        /**
         * realloc memory
        **/
        virtual mem_type realloc( size_t nsz, mem_type mem, size_t osz ) = 0;

        /**
         * dealloc memory
        **/
        virtual void dealloc( mem_type mem, size_t sz ) = 0;

        /**
         * convert to void*
        **/
        virtual void* touch( mem_type mem ) const = 0;

        /**
         * null memory
        **/
        virtual mem_type null( void ) const = 0;

        /**
         * size of alloc memories
        **/
        virtual size_t size( void ) const = 0;

        /**
         * size of free memories
        **/
        virtual size_t free( void ) const = 0;

        /**
         * max size can be alloc
        **/
        virtual size_t max_size( void ) const = 0;
    };
} /* namespace ray */

#endif /* #ifndef __BASE_UNKNOWN_ALLOC_HPP_20160921115827_C3783185_924A_4E10_80C1_75126F071D20__ */
