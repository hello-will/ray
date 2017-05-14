/**
 * @file    stack.hpp
 * @version 1.0
 * @author  hello_will@yeah.net
 * @date    2016/10/20 11:57:59
**/

#ifndef __BASE_STACK_HPP_20161020115815_1156EFE5_E7BC_4311_BF4E_BE1F1D886050__
#define __BASE_STACK_HPP_20161020115815_1156EFE5_E7BC_4311_BF4E_BE1F1D886050__

#include <base/singleton.hpp>
#include <base/normal_alloc.hpp>

namespace ray
{
    template <typename __type,
              typename __alloc = ray::normal_alloc>
    class stack : public non_copyable
    {
        typedef __type value_type;
        typedef __alloc alloc_type;
        typedef typename alloc_type::mem_type node_type;

        struct node
        {
            node_type data;
            uint32_t  aba;
        };
        typedef std::atomic<node> anode;

        struct st_node
        {
            anode  next;
            __type data;
        };

        static node_type alloc( void )
        {
            return singleton<alloc_type>::geti()->alloc( sizeof(st_node) );
        }

        static void dealloc( node_type x )
        {
            singleton<alloc_type>::geti()->dealloc( x, sizeof(st_node) );
        }

        static st_node* touch( node_type x )
        {
            return (st_node*)singleton<alloc_type>::geti()->touch( x );
        }

        static node_type null( void )
        {
            return singleton<alloc_type>::geti()->null();
        }

    public:
        stack( void )
            : m_head( null() )
            , m_size( 0 )
        {
            m_head = alloc();

            auto head = (anode*)touch( m_head );
            if ( nullptr != head )
            {
                node nul = {null(), 0};
                ::new ((void*)head) anode(nul);
            }
        }

        ~stack( void )
        {
            clear();
            dealloc( m_head );
            m_head = null();
        }

        /**
         * push value to stack
        **/
        void push( const value_type &x )
        {
            auto head = touch( m_head );
            if ( nullptr != head )
            {
                auto mem = alloc();
                auto stn = touch( mem );
                if ( nullptr != stn )
                {
                    node nul = {null(), 0};
                    ::new ((void*)&stn->next) anode(nul);
                    ::new ((void*)&stn->data) value_type(x);

                    node old, val;
                    auto tmp = (anode*)touch( mem );
                    do
                    {
                        old = head->load();
                        tmp->store( old );
                        val.data = mem;
                        val.aba = old.aba + 1;
                    }
                    while (!head->compare_exchange_weak(old,val));
                    m_size.fetch_add( 1 );
                }
            }
        }

        /**
         * push value to stack
        **/
        void push( value_type &&x )
        {
            auto head = (anode*)touch( m_head );
            if ( nullptr != head )
            {
                auto mem = alloc();
                auto stn = touch( mem );
                if ( nullptr != stn )
                {
                    node nul = {null(), 0};
                    ::new ((void*)&stn->next) anode(nul);
                    ::new ((void*)&stn->data) value_type(ray::move(x));

                    node old, val;
                    auto tmp = (anode*)touch( mem );
                    do
                    {
                        old = head->load();
                        tmp->store( old );
                        val.data = mem;
                        val.aba = old.aba + 1;
                    }
                    while (!head->compare_exchange_weak(old,val));
                    m_size.fetch_add( 1 );
                }
            }
        }

        /**
         * pop value from stack
        **/
        bool pop( value_type &x )
        {
            auto head = (anode*)touch( m_head );
            if ( nullptr != head )
            {
                node old, val;
                do
                {
                    old = head->load();

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
                } while (!head->compare_exchange_weak(old,val));

                auto stn = touch( old.data );
                if ( nullptr != stn )
                {
                    m_size.fetch_sub( 1 );
                    x = ray::move( stn->data );
                    stn->data.~value_type();
                    dealloc( old.data );

                    return true;
                }
            }

            return false;
        }

        /**
         * clear stack
        **/
        void clear( void )
        {
            auto head = (anode*)touch( m_head );
            while ( nullptr != head )
            {
                node old, val;
                do
                {
                    old = head->load();

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
                } while (!head->compare_exchange_weak(old,val));

                auto stn = touch( old.data );
                if ( nullptr != stn )
                {
                    m_size.fetch_sub( 1 );
                    stn->data.~value_type();
                    dealloc( old.data );
                }
                else
                {
                    break;
                }
            }
        }

    private:
        node_type             m_head;
        std::atomic<uint32_t> m_size;
    };
} /* namespace ray */

#endif /* #ifndef __BASE_STACK_HPP_20161020115815_1156EFE5_E7BC_4311_BF4E_BE1F1D886050__ */
