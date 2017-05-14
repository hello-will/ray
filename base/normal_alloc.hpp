/**
 * @file    normal_alloc.hpp
 * @version 1.0
 * @author  hello_will@yeah.net
 * @date    2016/09/21 15:45:51
**/

#include <base/utils.hpp>
#include <base/unknown_alloc.hpp>

#ifndef __BASE_NORMAL_ALLOC_HPP_20161009084949_EE8B3C0F_9B39_4EF2_851B_9389256C4501__
#define __BASE_NORMAL_ALLOC_HPP_20161009084949_EE8B3C0F_9B39_4EF2_851B_9389256C4501__

#include <base/utils.hpp>
#include <base/unknown_alloc.hpp>

namespace ray
{
    class normal_alloc : public unknown_alloc<void*>
    {
    public:
        /**
         * alloc memory
        **/
        void* alloc( size_t sz, const void *ptr = nullptr )
        {
            auto mem = ::malloc( aligned(sz,ALIGNED) );

            if ( nullptr != mem )
            {
                m_size.fetch_add( aligned(sz,ALIGNED) );
                if ( nullptr != ptr )
                {
                    ::memmove( mem, ptr, sz );
                }
            }

            return mem;
        }

        /**
         * realloc memory
        **/
        void* realloc( size_t nsz, void *mem, size_t osz )
        {
            nsz = aligned(nsz,ALIGNED);
            osz = aligned(osz,ALIGNED);
            if ( nsz != osz )
            {
                mem = ::realloc( mem, nsz );
                if ( nullptr != mem )
                {
                    m_size.fetch_add( nsz - osz );
                }
            }

            return mem;
        }

        /**
         * dealloc memory
        **/
        void dealloc( void *mem, size_t sz )
        {
            if ( nullptr != mem )
            {
                ::free( mem );
                m_size.fetch_sub( aligned(sz,ALIGNED) );
            }
        }

        /**
         * convert to void*
        **/
        void* touch( void *mem ) const
        {
            return mem;
        }

        /**
         * null memory
        **/
        void* null( void ) const
        {
            return nullptr;
        }

        /**
        * size of alloc memories
        **/
        size_t size( void ) const
        {
            return m_size.load();
        }

        /**
         * size of free memories
        **/
        size_t free( void ) const
        {
            return 0;
        }

        /**
         * max size can be alloc
         **/
        size_t max_size( void ) const
        {
            return SIZE_MAX;
        }

    private:
        std::atomic<size_t> m_size;
    };
} /* namespace ray */

#endif /* #ifndef __BASE_NORMAL_ALLOC_HPP_20161009084949_EE8B3C0F_9B39_4EF2_851B_9389256C4501__ */
