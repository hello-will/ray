/**
 * @file offset_alloc.hpp
 * @version 1.0
 * @author hello_will@yeah.net
 * @date 2016/09/21 16:07:53
**/

#ifndef __BASE_OFFSET_ALLOC_HPP_20160922105123_49AB1F26_9A66_47D6_B612_DD86900BD3C4__
#define __BASE_OFFSET_ALLOC_HPP_20160922105123_49AB1F26_9A66_47D6_B612_DD86900BD3C4__

#include <base/utils.hpp>
#include <base/unknown_alloc.hpp>

namespace ray
{
    class offset_alloc : public unknown_alloc<uint32_t>
    {
    public:
        offset_alloc( void )
            : m_buf( nullptr )
            , m_end( 0 )
            , m_cur( null() )
            , m_free( 0 )
        {
            uint16_t s = 0;
            for ( auto i = 0; i < 3; ++i )
            {
                m_size[i] = (8<<((i+3)>>2))+(2<<(i+2));
                for ( ; s <= m_size[i]; ++s )
                {
                    m_node[s] = i;
                }
            }

            for ( auto i = 3; i < MAX_BASKET; ++i )
            {
                m_size[i] = (4+((i+9)&3))<<(1+((i+9)>>2));
                for ( ; s <= m_size[i]; ++s )
                {
                    m_node[s] = i;
                }
            }

            for ( auto i = 0; i < MAX_BASKET; ++ i )
            {
                m_basket[i] = null();
            }
        }

        ~offset_alloc( void )
        {
            close();

            for ( auto i = 0; i < MAX_BASKET; ++i )
            {
                m_size[i] = 0;
            }

            for ( auto s = 0; s <= MAX_NODE; ++s )
            {
                m_node[s] = 0;
            }
        }

        /**
         * initialize
         * @param buf buff or alloc
         * @param sz  size of buf
         * @return if ok return RAY_OK
         **/
        int32_t init( void *buf, uint32_t sz )
        {
            close();

            auto sz_anode = aligned( sizeof(anode), ALIGNED );
            if ( nullptr == buf || sz < (MAX_BASKET+1)*sz_anode )
            {
                return RAY_INVAL;
            }
            else
            {
                m_buf = (uint8_t*)buf;
                m_end = sz;
                m_cur = 0;

                node val = {(uint32_t)sz_anode*(MAX_BASKET+1), 0};
                ::new ((void*)touch(m_cur)) anode(val);
                m_free.store( sz - val.data );
            }

            auto cur = sz_anode;
            for (  auto i = 0; i < MAX_BASKET; ++i )
            {
                m_basket[i] = (uint32_t)cur;
                cur += sz_anode;

                node val = {null(), 0};
                ::new ((void*)touch(m_basket[i])) anode(val);
            }

            return RAY_OK;
        }

        /**
         * close
         **/
        void close( void )
        {
            m_buf = nullptr;
            m_cur = 0;
            m_end = 0;
            m_free.store( 0 );

            for ( auto i = 0; i < MAX_BASKET; ++ i )
            {
                m_basket[i] = null();
            }
        }

        /**
         * alloc memory
         **/
        uint32_t alloc( size_t sz, const void *ptr = nullptr )
        {
            if ( sz > MAX_NODE )
            {
                return null();
            }

            auto idx = m_node[sz];
            auto mem = pop( m_basket[idx] );
            if ( mem != null() )
            {
                m_free.fetch_sub( m_size[idx] );
                if ( nullptr != ptr )
                {
                    ::memmove( touch(mem), ptr, sz );
                }

                return mem;
            }

            node old, val;
            auto cur = (anode*)touch( m_cur );
            do
            {
                old = cur->load();
                if ( old.data + m_size[idx] > m_end )
                {
                    break;
                }
                else
                {
                    val.data = old.data + m_size[idx];
                    val.aba = old.aba + 1;
                }
            } while (!cur->compare_exchange_weak(old,val));


            if ( old.data + m_size[idx] <= m_end )
            {
                mem = old.data;
                m_free.fetch_sub( m_size[idx] );

                if ( nullptr != ptr )
                {
                    ::memmove( touch(mem), ptr, sz );
                }
            }

            return mem;
        }

        /**
         * realloc memory
        **/
        uint32_t realloc( size_t nsz, uint32_t mem, size_t osz )
        {
            if ( nsz > MAX_NODE || osz > MAX_NODE )
            {
                dealloc( mem, osz );

                return null();
            }

            if ( m_node[nsz] == m_node[osz] )
            {
                return mem;
            }

            auto tmp = alloc( nsz );
            if ( null() == tmp )
            {
                dealloc( mem, osz );

                return null();
            }

            auto ptr = touch( mem );
            if ( nullptr != ptr )
            {
                ::memmove( touch(tmp), ptr, osz );
            }
            dealloc( mem, osz );

            return tmp;
        }

        /**
         * dealloc memory
         **/
        void dealloc( uint32_t mem, size_t sz )
        {
            auto cur = (anode*)touch( m_cur );
            auto sz_anode = aligned( sizeof(anode), ALIGNED );
            if ( mem != null() &&
                 mem >= (MAX_BASKET+1)*sz_anode &&
                 mem < cur->load().data &&
                 sz <= MAX_NODE )
            {
                auto idx = m_node[sz];

                push( m_basket[idx], mem );
                m_free.fetch_add( m_size[idx] );
            }
        }

        /**
         * convert to void*
         **/
        void* touch( uint32_t mem ) const
        {
            if ( m_buf != nullptr && mem != null() )
            {
                return m_buf + mem;
            }

            return nullptr;
        }

        /**
         * null memory
         **/
        uint32_t null( void ) const
        {
            return 0xffffffff;
        }

        /**
         * size of alloc memories
         **/
        size_t size( void ) const
        {
            return m_end;
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
         * push mem into basket
         * @param basket
         * @param mem
         **/
        void push( uint32_t basket, uint32_t mem )
        {
            auto tmp = (anode*)touch( mem );
            if ( nullptr != tmp )
            {
                auto bkt = (anode*)touch( basket );
                if ( nullptr != bkt )
                {
                    node nul = {null(), 0};
                    ::new ((void*)tmp) anode(nul);

                    node old, val;
                    do
                    {
                        old = bkt->load();
                        tmp->store( old );
                        val.data = mem;
                        val.aba = old.aba + 1;
                    } while (!bkt->compare_exchange_weak(old,val));
                }
            }
        }

        /**
         * pop mem from basket
         * @param basket
         * @return memory node
         **/
        uint32_t pop( uint32_t basket )
        {
            auto bkt = (anode*)touch( basket );
            if ( nullptr != bkt )
            {
                node old, val;
                do
                {
                    old = bkt->load();

                    auto tmp = (anode*)touch( old.data );
                    if ( nullptr != tmp )
                    {
                        val.data = (((node*)(tmp+1))-1)->data;
                        val.aba = old.aba + 1;
                    }
                    else
                    {
                        break;
                    }
                } while (!bkt->compare_exchange_weak(old,val));

                return old.data;
            }

            return null();
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
            MAX_NODE = 4096
        };

        struct node
        {
            uint32_t data;
            uint32_t aba;
        };
        typedef std::atomic<node> anode;

        uint8_t              *m_buf;
        uint32_t              m_end;
        uint32_t              m_cur;
        uint16_t              m_size[MAX_BASKET];
        uint32_t              m_basket[MAX_BASKET];
        uint8_t               m_node[MAX_NODE+1];
        std::atomic<uint32_t> m_free;
    };

} /* namespace ray */

#endif /* #ifndef __BASE_OFFSET_ALLOC_HPP_2016092210512_49AB1F26_9A66_47D6_B612_DD86900BD3C4__ */
