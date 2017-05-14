/**
 * @file    list.hpp
 * @version 1.0
 * @author  hello_will@yeah.net
 * @date    2016/09/22 11:35:52
 **/

#ifndef __BASE_LIST_HPP_20160922113606_95CA148C_D5E7_4960_9832_B34E2A952257__
#define __BASE_LIST_HPP_20160922113606_95CA148C_D5E7_4960_9832_B34E2A952257__

#include <base/singleton.hpp>
#include <base/normal_alloc.hpp>

namespace ray
{
    template <typename __type,
              typename __alloc = ray::normal_alloc>
    class list
    {
        typedef uint32_t size_type;
        typedef __type value_type;
        typedef __alloc alloc_type;
        typedef typename alloc_type::mem_type node_type;
        typedef typename ray::list<value_type,alloc_type> self_type;

        struct node
        {
            node_type  prev;
            node_type  next;
            value_type value;
        };

        static node_type alloc( void )
        {
            return singleton<alloc_type>::geti()->alloc( sizeof(node) );
        }

        static void dealloc( node_type x )
        {
            singleton<alloc_type>::geti()->dealloc( x, sizeof(node) );
        }

        static node* touch( node_type x )
        {
            return (node*)singleton<alloc_type>::geti()->touch( x );
        }

        static node_type null( void )
        {
            return singleton<alloc_type>::geti()->null();
        }

    public:
        struct const_iterator;
        struct iterator
        {
            iterator( void )
                : iter( self_type::null() )
            { }

            iterator( node_type iter_ )
                : iter( iter_ )
            { }

            iterator( const iterator &x )
                : iter( x.iter )
            { }

            iterator( const const_iterator &x )
                : iter( x.iter )
            { }

            value_type& operator * ( void ) const
            {
                auto __iter = self_type::touch( iter );

                return __iter->value;
            }

            value_type* operator -> ( void ) const
            {
                auto __iter = self_type::touch( iter );

                return &__iter->value;
            }

            iterator operator ++ ( void )
            {
                auto __iter = self_type::touch( iter );

                iter = __iter->next;

                return *this;
            }

            iterator operator ++ ( int )
            {
                auto tmp = *this;

                ++(*this);

                return tmp;
            }

            iterator operator -- ( void )
            {
                auto __iter = self_type::touch( iter );

                iter = __iter->prev;

                return *this;
            }

            iterator operator -- ( int )
            {
                auto tmp = *this;

                --(*this);

                return tmp;
            }

            bool operator == ( const iterator &x ) const
            {
                return iter == x.iter;
            }

            bool operator != ( const iterator &x ) const
            {
                return iter != x.iter;
            }

            node_type iter;
        };

        struct const_iterator
        {
            const_iterator( void )
                : iter( self_type::null() )
            { }

            const_iterator( node_type iter_ )
                : iter( iter_ )
            { }

            const_iterator( const const_iterator &x )
                : iter( x.iter )
            { }

            const_iterator( const iterator &x )
                : iter( x.iter )
            { }

            const value_type& operator * ( void ) const
            {
                auto __iter = self_type::touch( iter );

                return __iter->value;
            }

            const value_type* operator -> ( void ) const
            {
                auto __iter = self_type::touch( iter );

                return &__iter->value;
            }

            const_iterator operator ++ ( void )
            {
                auto __iter = self_type::touch( iter );

                iter = __iter->next;

                return *this;
            }

            const_iterator operator ++ ( int )
            {
                auto tmp = *this;

                ++(*this);

                return tmp;
            }

            const_iterator operator -- ( void )
            {
                auto __iter = self_type::touch( iter );

                iter = __iter->prev;

                return *this;
            }

            const_iterator operator -- ( int )
            {
                auto tmp = *this;

                --(*this);

                return tmp;
            }

            bool operator ==( const const_iterator &x ) const
            {
                return iter == x.iter;
            }

            bool operator != ( const const_iterator &x ) const
            {
                return iter != x.iter;
            }

            node_type iter;
        };

        list( void )
            : m_size( 0 )
            , m_node( null() )
        {
            empty_initialize();
        }

        list( const self_type &x )
            : m_size( 0 )
            , m_node( null() )
        {
            insert_initialize( x.begin(), x.end() );
        }

        list( self_type &&x )
            : m_size( 0 )
            , m_node( null() )
        {
            empty_initialize();
            swap( x );
        }

        template <typename __iterator>
        list( __iterator first, __iterator last )
            : m_size( 0 )
            , m_node( null() )
        {
            insert_initialize( first, last );
        }

        ~list( void )
        {
            clear();
            dealloc( m_node );
            m_node = null();
        }

        self_type& operator = ( const self_type &x )
        {
            if ( this != &x )
            {
                auto first = begin();
                auto x_first = x.begin();

                for ( ; first != end() && x_first != end(); )
                {
                    *first++ = *x_first++;
                }

                if ( first != end() )
                {
                    erase( first, end() );
                }
                else
                {
                    insert( end(), x_first, x.end() );
                }
            }

            return (*this);
        }

        self_type& operator = ( self_type &&x )
        {
            if ( this != &x )
            {
                clear();
                swap( x );
            }

            return (*this);
        }

        bool empty( void ) const
        {
            auto __node = touch( m_node );

            return nullptr == __node || __node->next == m_node;
        }

        size_type size( void ) const
        {
            return m_size;
        }

        iterator begin( void )
        {
            auto __node = touch( m_node );

            return iterator( __node->next );
        }

        const_iterator begin( void ) const
        {
            auto __node = touch( m_node );

            return const_iterator( __node->next );
        }

        iterator end( void )
        {
            return iterator( m_node );
        }

        const_iterator end( void ) const
        {
            return const_iterator( m_node );
        }

        value_type& front( void )
        {
            return *begin();
        }

        const value_type& front( void ) const
        {
            return *begin();
        }

        value_type& back( void )
        {
            return *(--end());
        }

        const value_type& back( void ) const
        {
            return *(--end());
        }

        iterator insert( iterator pos )
        {
            auto next = pos.iter;
            auto __next = touch( next );
            auto prev = __next->prev;
            auto __prev = touch( prev );
            auto tmp = alloc();
            auto __tmp = touch( tmp );

            construct( &__tmp->value );
            __tmp->next = next;
            __tmp->prev = prev;
            __prev->next = tmp;
            __next->prev = tmp;
            ++m_size;

            return iterator( tmp );
        }

        iterator insert( iterator pos, const value_type &x )
        {
            auto next = pos.iter;
            auto __next = touch( next );
            auto prev = __next->prev;
            auto __prev = touch( prev );
            auto tmp = alloc();
            auto __tmp = touch( tmp );

            construct( &__tmp->value, x );
            __tmp->next = next;
            __tmp->prev = prev;
            __prev->next = tmp;
            __next->prev = tmp;
            ++m_size;

            return iterator( tmp );
        }

        iterator insert( iterator pos, value_type &&x )
        {
            auto next = pos.iter;
            auto __next = touch( next );
            auto prev = __next->prev;
            auto __prev = touch( prev );
            auto tmp = alloc();
            auto __tmp = touch( tmp );

            construct( &__tmp->value, ray::move(x) );
            __tmp->next = next;
            __tmp->prev = prev;
            __prev->next = tmp;
            __next->prev = tmp;
            ++m_size;

            return iterator( tmp );
        }

        template <typename __iterator>
        void insert( iterator pos, __iterator first, __iterator last )
        {
            if ( first != last )
            {
                self_type tmp( first, last );
                splice( pos, tmp );
            }
        }

        iterator erase( iterator pos )
        {
            auto __pos = touch( pos.iter );
            auto next = __pos->next;
            auto __next = touch( next );
            auto prev = __pos->prev;
            auto __prev = touch( prev );

            --m_size;
            __prev->next = next;
            __next->prev = prev;
            destroy( &__pos->value );
            dealloc( pos.iter );

            return iterator( next );
        }

        iterator erase( iterator first, iterator last )
        {
            while ( first != last )
            {
                erase( first++ );
            }

            return last;
        }

        void clear( void )
        {
            auto __node = touch( m_node );
            auto cur = __node->next;

            while ( cur != m_node && cur != null() )
            {
                auto __cur = touch( cur );
                auto next = __cur->next;

                destroy( &__cur->value );
                dealloc( cur );
                cur = next;
            }

            m_size = 0;
            __node->next = m_node;
            __node->prev = m_node;
        }

        void push_front( const value_type &val )
        {
            insert( begin(), val );
        }

        void push_front( value_type &&val )
        {
            insert( begin(), ray::move(val) );
        }

        void push_back( const value_type &val )
        {
            insert( end(), val );
        }

        void push_back( value_type &&val )
        {
            insert( end(), ray::move(val) );
        }

        void pop_front( void )
        {
            erase( begin() );
        }

        void pop_back( void )
        {
            erase( --end() );
        }

        void swap( self_type &x )
        {
            auto __empty = empty();
            auto __x_empty = x.empty();

            if ( !__empty || !__x_empty )
            {
                auto __node = touch( m_node );
                auto __x_node = touch( x.m_node );

                ray::swap( __node->next, __x_node->next );
                ray::swap( __node->prev, __x_node->prev );
                ray::swap( m_size, x.m_size );

                if ( __x_empty )
                {
                    empty_initialize();
                }
                else
                {
                    auto __next = touch( __node->next );
                    auto __prev = touch( __node->prev );

                    __next->prev = m_node;
                    __prev->next = m_node;
                }

                if ( __empty )
                {
                    x.empty_initialize();
                }
                else
                {
                    auto __x_next = touch( __x_node->next );
                    auto __x_prev = touch( __x_node->prev );

                    __x_next->prev = x.m_node;
                    __x_prev->next = x.m_node;
                }
            }
        }

        void reverse( void )
        {
            auto tmp = m_node;
            do
            {
                auto __tmp = touch( tmp );

                ray::swap( __tmp->next, __tmp->prev );
                tmp = __tmp->prev;
            }
            while ( tmp != m_node );
        }

        void splice( iterator pos, self_type &x )
        {
            if ( !x.empty() )
            {
                transfer( pos.iter, x.begin().iter, x.end().iter );
                m_size += x.m_size;
                x.m_size = 0;
            }
        }

        void splice( iterator pos, self_type &x, iterator i )
        {
            if ( i != x.end() )
            {
                auto next = i;

                transfer( pos.iter, i.iter, (++next).iter );
                ++ m_size;
                -- x.m_size;
            }
        }

        void splice( iterator pos, self_type &x, iterator first, iterator last )
        {
            while ( first != last )
            {
                auto next = first;

                transfer( pos.iter, first.iter, (++next).iter );
                ++ m_size;
                -- x.m_size;
                first = next;
            }
        }

        void unique( void )
        {
            unique( ray::equal<value_type>() );
        }

        template <typename __pred>
        void unique( __pred pred )
        {
            if ( !empty() )
            {
                auto first = begin();
                auto last = end();
                auto next = first;

                while ( ++next != last )
                {
                    if ( pred(*first,*next) )
                    {
                        erase( next );
                    }
                    else
                    {
                        first = next;
                    }
                    next = first;
                }
            }
        }

        void merge( self_type &x )
        {
            merge( x, ray::less<value_type>() );
        }

        template <typename __pred>
        void merge( self_type &x, __pred pred )
        {
            auto first1 = begin();
            auto last1 = end();
            auto first2 = x.begin();
            auto last2 = x.end();

            while ( first1 != last1 && first2 != last2 )
            {
                if ( pred(*first2,*first1) )
                {
                    auto next2 = first2;

                    transfer( first1.iter, first2.iter, (++next2).iter );
                    first2 = next2;
                    ++ m_size;
                    -- x.m_size;
                }
                else
                {
                    ++ first1;
                }
            }

            while ( first2 != last2 )
            {
                auto next2 = first2;

                transfer( last1.iter, first2.iter, (++next2).iter );
                first2 = next2;
                ++ m_size;
                -- x.m_size;
            }
        }

        void sort( void )
        {
            sort( ray::less<value_type>() );
        }

        template <typename __pred>
        void sort( __pred pred )
        {
            if ( size() > 1 )
            {
                self_type x, y;

                swap( x );
                while ( !x.empty() )
                {
                    y.splice( y.begin(), x, x.begin() );
                    merge( y, pred );
                }
            }
        }

        template <typename __pred>
        void remove_if( __pred pred )
        {
            if ( !empty() )
            {
                auto first = begin();
                auto last = end();

                while ( first != last )
                {
                    auto next = first;

                    ++next;
                    if ( pred(*first) )
                    {
                        erase( first );
                    }
                    first = next;
                }
            }
        }

    private:
        void empty_initialize( void )
        {
            m_size = 0;
            if ( null() == m_node )
            {
                m_node = alloc();
            }

            auto __node = touch( m_node );
            if ( nullptr != __node )
            {
                __node->next = m_node;
                __node->prev = m_node;
            }
        }

        template <typename __iterator>
        void insert_initialize( __iterator first, __iterator last )
        {
            m_size = 0;
            if ( null() == m_node )
            {
                m_node = alloc();
            }

            auto __node = touch( m_node );
            if ( nullptr != __node )
            {
                __node->next = m_node;
                __node->prev = m_node;
            }

            while ( first != last )
            {
                insert( begin(), *first++ );
            }
        }

        void transfer( node_type position, node_type first, node_type last )
        {
            if ( position != last )
            {
                auto __position = touch( position );
                auto __first = touch( first );
                auto __last = touch( last );
                auto __last_prev = touch( __last->prev );
                auto __first_prev = touch( __first->prev );
                auto __position_prev = touch( __position->prev );

                __last_prev->next = position;
                __first_prev->next = last;
                __position_prev->next = first;

                auto position_prev = __position->prev;
                __position->prev = __last->prev;
                __last->prev = __first->prev;
                __first->prev = position_prev;
            }
        }

        template <typename __other>
        void construct( __other *p )
        {
            ::new ((void*)p) __other();
        }

        template <typename __other>
        void construct( __other *p, const __other &x )
        {
            ::new ((void*)p) __other( x );
        }

        template <typename __other>
        void construct( __other *p, __other &&x )
        {
            ::new ((void*)p) __other( ray::move(x) );
        }

        template <typename __other>
        void destroy( __other *p )
        {
            if ( nullptr != p )
            {
                ((__other*)p)->~__other();
            }
        }

    private:
        size_type m_size;
        node_type m_node;
    };
} /* namespace ray */

#endif /* #ifndef __BASE_LIST_HPP_20160922113606_95CA148C_D5E7_4960_9832_B34E2A952257__ */
