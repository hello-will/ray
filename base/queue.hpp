/**
 * @file    queue.hpp
 * @version 1.0
 * @author  hello_will@yeah.net
 * @date    2016/10/20 11:56:59
**/

#ifndef __BASE_QUEUE_HPP_20161020115712_4FFB428A_C8C8_43C2_890F_B5CF776D75AB__
#define __BASE_QUEUE_HPP_20161020115712_4FFB428A_C8C8_43C2_890F_B5CF776D75AB__

#include <base/singleton.hpp>
#include <base/normal_alloc.hpp>

namespace ray
{
    template<typename __type,
             typename __alloc = ray::normal_alloc>
    class queue : public non_copyable
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

        struct qu_node
        {
            anode      next;
            value_type data;
        };

        static node_type alloc( void )
        {
            return singleton<alloc_type>::geti()->alloc( sizeof(qu_node) );
        }

        static void dealloc( node_type x )
        {
            singleton<alloc_type>::geti()->dealloc( x, sizeof(qu_node) );
        }

        static qu_node* touch( node_type x )
        {
            return (qu_node*)singleton<alloc_type>::geti()->touch( x );
        }

        static node_type null( void )
        {
            return singleton<alloc_type>::geti()->null();
        }

    public:
        queue( void )
            : m_head( null() )
            , m_tail( null() )
            , m_size( 0 )
        {
            m_head = alloc();
            m_tail = alloc();

            auto mem = alloc();
            auto tmp = (anode*)touch( mem );
            auto head = (anode*)touch( m_head );
            auto tail = (anode*)touch( m_tail );
            if ( nullptr != tmp &&
                 nullptr != head &&
                 nullptr != tail )
            {
                node nul = {null(), 0};
                ::new ((void*)tmp) anode(nul);

                node val = {mem, 1};
                ::new ((void*)head) anode(val);
                ::new ((void*)tail) anode(val);
            }
        }

        ~queue( void )
        {
            clear();

            auto head = (anode*)touch( m_head );
            if ( nullptr != head )
            {
                auto hed = head->load();

                dealloc( hed.data );
                dealloc( m_head );
                dealloc( m_tail );
                m_head = null();
                m_tail = null();
            }
        }

        /**
         * push value into queue
        **/
        void push( const value_type &x )
        {
            auto head = (anode*)touch( m_head );
            auto tail = (anode*)touch( m_tail );
            if ( nullptr != head && nullptr != tail )
            {
                auto mem = alloc();
                auto qun = touch( mem );
                if ( nullptr != qun )
                {
                    node nul = {null(), 0};
                    ::new ((void*)&qun->next) anode(nul);
                    ::new ((void*)&qun->data) value_type(x);

                    node tal, net, val;
                    while ( true )
                    {
                        tal = tail->load();
                        auto next = (anode*)touch( tal.data );
                        net = next->load();

                        if ( tail->compare_exchange_weak(tal,tal))
                        {
                            if ( null() == net.data )
                            {
                                val.data = mem;
                                val.aba = net.aba + 1;
                                if ( next->compare_exchange_weak(net,val) )
                                {
                                    break;
                                }
                            }
                            else
                            {
                                val.data = net.data;
                                val.aba = tal.aba + 1;
                                tail->compare_exchange_weak( tal, val );
                            }
                        }
                    }

                    val.data = mem;
                    val.aba = tal.aba + 1;
                    tail->compare_exchange_weak( tal, val );
                    m_size.fetch_add( 1 );
                }
            }
        }

        /**
         * push value into queue
        **/
        void push( value_type &&x )
        {
            auto head = (anode*)touch( m_head );
            auto tail = (anode*)touch( m_tail );
            if ( nullptr != head && nullptr != tail )
            {
                auto mem = alloc();
                auto qun = touch( mem );
                if ( nullptr != qun )
                {
                    node nul = {null(), 0};
                    ::new ((void*)&qun->next) anode(nul);
                    ::new ((void*)&qun->data) value_type(ray::move(x));

                    node tal, net, val;
                    while ( true )
                    {
                        tal = tail->load();
                        auto next = (anode*)touch( tal.data );
                        net = next->load();

                        if ( tail->compare_exchange_weak(tal,tal))
                        {
                            if ( null() == net.data )
                            {
                                val.data = mem;
                                val.aba = net.aba + 1;
                                if ( next->compare_exchange_weak(net,val) )
                                {
                                    break;
                                }
                            }
                            else
                            {
                                val.data = net.data;
                                val.aba = tal.aba + 1;
                                tail->compare_exchange_weak( tal, val );
                            }
                        }
                    }

                    val.data = mem;
                    val.aba = tal.aba + 1;
                    tail->compare_exchange_weak( tal, val );
                    m_size.fetch_add( 1 );
                }
            }
        }

        /**
         * pop value from queue
        **/
        bool pop( value_type &x )
        {
            auto head = (anode*)touch( m_head );
            auto tail = (anode*)touch( m_tail );
            if ( nullptr != head && nullptr != tail )
            {
                node hed, tal, net, val;
                while ( true )
                {
                    hed = head->load();
                    tal = tail->load();
                    auto next = (anode*)touch( hed.data );
                    net = *(((node*)(next+1))-1);

                    if ( head->compare_exchange_weak(hed,hed) )
                    {
                        if ( hed.data == tal.data )
                        {
                            if ( null() == net.data )
                            {
                                break;
                            }
                            else
                            {
                                val.data = net.data;
                                val.aba = tal.aba + 1;
                                tail->compare_exchange_weak( tal, val );
                            }
                        }
                        else
                        {
                            val.data = net.data;
                            val.aba = hed.aba + 1;
                            if ( head->compare_exchange_weak(hed,val) )
                            {
                                break;
                            }
                        }
                    }
                }

                auto qun = (qu_node*)touch( net.data );
                if ( nullptr != qun )
                {
                    m_size.fetch_sub( 1 );
                    x = ray::move( qun->data );
                    qun->data.~value_type();
                    dealloc( hed.data );

                    return true;
                }
            }

            return false;
        }

        /**
         * clear queue
        **/
        void clear( void )
        {
            auto head = (anode*)touch( m_head );
            auto tail = (anode*)touch( m_tail );
            if ( nullptr != head && nullptr != tail )
            {
                node hed, tal, net, val;
                do
                {
                    while ( true )
                    {
                        hed = head->load();
                        tal = tail->load();
                        auto next = (anode*)touch( hed.data );
                        net = *(((node*)(next+1))-1);

                        if ( head->compare_exchange_weak(hed,hed) )
                        {
                            if ( hed.data == tal.data )
                            {
                                if ( null() == net.data )
                                {
                                    break;
                                }
                                else
                                {
                                    val.data = net.data;
                                    val.aba = tal.aba + 1;
                                    tail->compare_exchange_weak( tal, val );
                                }
                            }
                            else
                            {
                                val.data = net.data;
                                val.aba = hed.aba + 1;
                                if ( head->compare_exchange_weak(hed,val) )
                                {
                                    break;
                                }
                            }
                        }
                    }

                    auto qun = (qu_node*)touch( net.data );
                    if ( nullptr != qun )
                    {
                        m_size.fetch_sub( 1 );
                        qun->data.~value_type();
                        dealloc( hed.data );
                    }
                } while (null() != net.data);
            }
        }

    private:
        node_type             m_head;
        node_type             m_tail;
        std::atomic<uint32_t> m_size;
    };

    template <typename __type,
              typename __alloc = ray::normal_alloc>
    class ring_queue : public non_copyable
    {
        typedef __type value_type;
        typedef __alloc alloc_type;
        typedef typename alloc_type::mem_type node_type;

        typedef std::atomic<node_type> anode;
        struct qu_node
        {
            node_type  next;
            value_type data;
        };

        static node_type alloc( void )
        {
            return singleton<alloc_type>::geti()->alloc( sizeof(qu_node) );
        }

        static void dealloc( node_type x )
        {
            singleton<alloc_type>::geti()->dealloc( x, sizeof(qu_node) );
        }

        static qu_node* touch( node_type x )
        {
            return (qu_node*)singleton<alloc_type>::geti()->touch( x );
        }

        static node_type null( void )
        {
            return singleton<alloc_type>::geti()->null();
        }

    public:
        ring_queue( void )
            : m_head( null() )
            , m_tail( null() )
            , m_size( 0 )
        {
            auto mem = alloc();
            auto tmp = touch( mem );
            if ( nullptr != tmp )
            {
                m_head = mem;
                m_tail = mem;
                tmp->next = mem;
            }
        }

        ~ring_queue( void )
        {
            clear();

            auto head = m_head.load();
            auto last = head;
            do
            {
                auto temp = head;
                auto node = (qu_node*)touch( temp );

                head = node->next;
                dealloc( temp );
            } while ( head != last );
            m_head.store( null() );
            m_tail.store( null() );
        }

        /**
         * push value into queue
        **/
        void push( const value_type &x )
        {
            auto tail = (qu_node*)touch( m_tail );
            if ( nullptr != tail )
            {
                if ( tail->next == m_head.load() )
                {
                    auto mem = alloc();
                    auto tmp = touch( mem );
                    if ( nullptr != tmp )
                    {
                        tmp->next = tail->next;
                        tail->next = mem;
                    }
                }

                if ( tail->next != m_head.load() )
                {
                    ::new ((void*)&tail->data) value_type(x);
                    m_tail.store( tail->next );
                }
            }
        }

        /**
         * push value into queue
        **/
        void push( value_type &&x )
        {
            auto tail = (qu_node*)touch( m_tail );
            if ( nullptr != tail )
            {
                if ( tail->next == m_head.load() )
                {
                    auto mem = alloc();
                    auto tmp = touch( mem );
                    if ( nullptr != tmp )
                    {
                        tmp->next = tail->next;
                        tail->next = mem;
                    }
                }

                if ( tail->next != m_head.load() )
                {
                    ::new ((void*)&tail->data) value_type(ray::move(x));
                    m_tail.store( tail->next );
                }
            }
        }

        /**
         * pop value from queue
        **/
        bool pop( value_type &x )
        {
            auto head = m_head.load();
            if ( null() != head )
            {
                if ( head != m_tail.load() )
                {
                    auto node = (qu_node*)touch( head );
                    if ( nullptr != node )
                    {
                        x = ray::move(node->data);
                        node->data.~value_type();
                        m_size.fetch_sub( 1 );
                        m_head.store( node->next );

                        return true;
                    }
                }
            }

            return false;
        }

        /**
         * clear queue
        **/
        void clear( void )
        {
            auto head = m_head.load();
            if ( null() != head )
            {
                while ( head != m_tail.load() )
                {
                    auto node = (qu_node*)touch( head );
                    if ( nullptr != node )
                    {
                        node->data.~value_type();
                        m_size.fetch_sub( 1 );
                        m_head.store( node->next );
                    }
                    head = m_head.load();
                }
            }
        }

    private:
        std::atomic<node_type> m_head;
        std::atomic<node_type> m_tail;
        std::atomic<uint32_t>  m_size;
    };
} /* namespace ray */

#endif /* #ifndef __BASE_QUEUE_HPP_20161020115712_4FFB428A_C8C8_43C2_890F_B5CF776D75AB__ */
