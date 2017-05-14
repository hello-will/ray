/**
 * @file    pool_alloc.hpp
 * @version 1.0
 * @author  hello_will@yeah.net
 * @date    2016/10/09 16:20:16
**/

#ifndef __BASE_POOL_ALLOC_HPP_20161009162040_3688C7AB_1EA2_4965_A1DD_CDBB77CAA81A__
#define __BASE_POOL_ALLOC_HPP_20161009162040_3688C7AB_1EA2_4965_A1DD_CDBB77CAA81A__

#include <base/utils.hpp>
#include <base/unknown_alloc.hpp>

namespace ray
{
    class pool_alloc : public unknown_alloc<void*>
    {
    public:
        pool_alloc( void )
            : m_size( 0 )
            , m_free( 0 )
        {
            node val = {nullptr, 0};
            m_block.next.store( val );
            m_block.data = nullptr;
            m_anode.next.store( val );
            m_anode.data = nullptr;

            uint16_t s = 0;
            for ( auto i = 0; i < 3; ++i )
            {
                m_basket[i].pool.store( val );
                m_basket[i].size = (8<<((i+3)>>2))+(2<<(i+2));
                for ( ; s <= m_basket[i].size; ++s )
                {
                    m_node[s] = i;
                }
            }

            for ( auto i = 3; i < MAX_BASKET; ++i )
            {
                m_basket[i].pool.store( val );
                m_basket[i].size = (4+((i+9)&3))<<(1+((i+9)>>2));
                for ( ; s <= m_basket[i].size; ++s )
                {
                    m_node[s] = i;
                }
            }
        }

        ~pool_alloc( void )
        {
            node val = {nullptr, 0};
            m_anode.next.store( val );
            m_anode.data = nullptr;
            m_size.store( 0 );
            m_free.store( 0 );

            for ( auto i = 0; i<MAX_BASKET; ++i )
            {
                m_basket[i].pool.store( val );
                m_basket[i].size = 0;
            }

            auto blk = (ablock*)pop( m_block.next );
            while ( nullptr != blk )
            {
                ::free( blk->data );
                blk = (ablock*)pop( m_block.next );
            }
            m_block.next.store( val );
        }

        /**
         * alloc memory
         **/
        void* alloc( size_t sz, const void *ptr = nullptr )
        {
            if ( sz > MAX_NODE )
            {
                auto  mem = ::malloc( aligned(sz,MAX_NODE) );

                if ( nullptr != mem )
                {
                    m_size.fetch_add( aligned(sz,MAX_NODE) );
                    if ( nullptr != ptr )
                    {
                        ::memmove( mem, ptr, sz );
                    }
                }

                return mem;
            }

            auto idx = m_node[sz];
            auto mem = pop( m_basket[idx].pool );
            if ( nullptr != mem )
            {
                m_free.fetch_sub( m_basket[idx].size );
                if ( nullptr != ptr )
                {
                    ::memmove( mem, ptr, sz );
                }

                return mem;
            }

            auto blk = (ablock*)pop( m_anode.next );
            while ( nullptr == blk )
            {
                auto tmp = (uint8_t*)::malloc( MAX_BLOCK );
                if ( nullptr != tmp )
                {
                    auto sz_ablock = aligned( sizeof(ablock), ALIGNED );
                    for ( auto i = 1U; i < MAX_BLOCK/sz_ablock; ++i )
                    {
                        blk = (ablock*)(tmp + i*sz_ablock);
                        blk->data = nullptr;
                        push( m_anode.next, blk );
                    }

                    blk = (ablock*)tmp;
                    blk->data = tmp;
                    push( m_block.next, blk );
                    blk = (ablock*)pop( m_anode.next );
                    m_size.fetch_add( MAX_BLOCK );
                }
                else
                {
                    break;
                }
            }

            if ( nullptr != blk )
            {
                auto tmp = (uint8_t*)::malloc( MAX_BLOCK );
                if ( nullptr != tmp )
                {
                    for ( auto i = 1U; i < MAX_BLOCK/m_basket[idx].size; ++i )
                    {
                        push( m_basket[idx].pool, tmp+i*m_basket[idx].size );
                        m_free.fetch_add( m_basket[idx].size );
                    }

                    mem = tmp;
                    blk->data = tmp;
                    push( m_block.next, blk );
                    m_size.fetch_add( MAX_BLOCK );
                    if ( nullptr != ptr )
                    {
                        ::memmove( mem, ptr, sz );
                    }

                    return mem;
                }
                else
                {
                    push( m_anode.next, blk );
                }
            }

            return nullptr;
        }

        /**
         * realloc memory
        **/
        void* realloc( size_t nsz, void *mem, size_t osz )
        {
            if ( nsz > MAX_NODE &&
                 osz > MAX_NODE &&
                 aligned(nsz,MAX_NODE) == aligned(osz,MAX_NODE) )
            {
                return mem;
            }

            if ( nsz <= MAX_NODE &&
                 osz <= MAX_NODE &&
                 m_node[nsz] == m_node[osz] )
            {
                return mem;
            }

            auto tmp = alloc( nsz );
            if ( nullptr == tmp )
            {
                dealloc( mem, osz );

                return nullptr;
            }

            if ( nullptr != mem )
            {
                ::memmove( tmp, mem, osz );
            }
            dealloc( mem, osz );

            return tmp;
        }

        /**
         * dealloc memory
         **/
        void dealloc( void *mem, size_t sz )
        {
            if ( nullptr != mem )
            {
                if ( sz > MAX_NODE )
                {
                    ::free( mem );
                    m_size.fetch_sub( aligned(sz,MAX_NODE) );
                }
                else
                {
                    auto idx = m_node[sz];

                    push( m_basket[idx].pool, mem );
                    m_free.fetch_add( m_basket[idx].size );
                }
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
            return m_free.load();
        }

        /**
         * max size can be alloc
        **/
        size_t max_size( void ) const
        {
            return MAX_NODE;
        }

    private:
        /**
         * 16   32   48   64
         * 80   96   112  128
         * 160  192  224  256
         * 320  384  448  512
         * 640  768  896  1024
         * 1280 1536 1792 2048
         * 2560 3072 3584 4096
        **/
        enum
        {
            MAX_BASKET = 28,
            MAX_NODE = 4096,
            MAX_BLOCK = 65536
        };

        struct node
        {
            void  *data;
            size_t aba;
        };
        typedef std::atomic<node> anode;

        struct ablock
        {
            anode next;
            void *data;
        };

        struct abasket
        {
            anode    pool;
            uint16_t size;
        };

        /**
         * push mem into basket
         * @param bkt basket
         * @param mem memory node
         **/
        void push( anode &bkt, void *mem )
        {
            auto tmp = (anode*)mem;
            if ( nullptr != tmp )
            {
                node nul = {nullptr, 0};
                ::new ((void*)tmp) anode(nul);

                node old, val;
                do
                {
                    old = bkt.load();
                    tmp->store( old );
                    val.data = mem;
                    val.aba = old.aba + 1;
                } while (!bkt.compare_exchange_weak(old,val));
            }
        }

        /**
         * pop mem from basket
         * @param bkt basket
         * @return memory node
         **/
        void* pop( anode &bkt )
        {
            node old, val;
            do
            {
                old = bkt.load();

                auto tmp =(anode*)old.data;
                if ( nullptr != tmp )
                {
                    val.data = (((node*)(tmp+1))-1)->data;
                    val.aba = old.aba + 1;
                }
                else
                {
                    break;
                }
            } while (!bkt.compare_exchange_weak(old,val));

            return old.data;
        }

    private:
        ablock              m_block;
        ablock              m_anode;
        abasket             m_basket[MAX_BASKET];
        uint8_t             m_node[MAX_NODE+1];
        std::atomic<size_t> m_size;
        std::atomic<size_t> m_free;
    };
} /* namespace ray */

#endif /* #ifndef __BASE_POOL_ALLOC_HPP_20161009162040_3688C7AB_1EA2_4965_A1DD_CDBB77CAA81A__ */
