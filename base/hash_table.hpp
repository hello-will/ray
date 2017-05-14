/**
 * @file    hash_table.hpp
 * @version 1.0
 * @author  hello_will@yeah.net
 * data     2016/10/27 13:06:07
**/

#ifndef __BASE_HASH_TABLE_HPP_20161027130607_2B032EC0_81F0_4202_B187_204A2F5D3020__
#define __BASE_HASH_TABLE_HPP_20161027130607_2B032EC0_81F0_4202_B187_204A2F5D3020__

#include <base/pair.hpp>
#include <base/hash_fun.hpp>

namespace ray
{
    template <typename __type,
              typename __key,
              typename __hash,
              typename __keyof,
              typename __keyeq,
              typename __alloc>
    class hash_table
    {
        typedef uint32_t size_type;
        typedef __type value_type;
        typedef __key  key_type;
        typedef __hash hash_type;
        typedef __keyof keyof_type;
        typedef __keyeq keyeq_type;
        typedef __alloc alloc_type;
        typedef typename alloc_type::mem_type node_type;
        typedef typename ray::hash_table<value_type,
                                         key_type,
                                         hash_type,
                                         keyof_type,
                                         keyeq_type,
                                         alloc_type> self_type;

        struct node
        {
            node_type  next;
            value_type value;
        };

        struct bucket
        {
            node_type prev;
            node_type next;
            node_type list[1];
        };

        enum
        {
            BUCKET_SIZE = sizeof(node_type)*1024,
            NODE_COUNT = 1022 /* (BUCKET_SIZE/sizeof(node_type) - 2) */
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

        static node_type alloc_bucket( void )
        {
            return singleton<alloc_type>::geti()->alloc( BUCKET_SIZE );
        }

        static void dealloc_bucket( node_type x )
        {
            singleton<alloc_type>::geti()->dealloc( x, BUCKET_SIZE );
        }

        static bucket* touch_bucket( node_type x )
        {
            return (bucket*)singleton<alloc_type>::geti()->touch( x );
        }

        static const size_type* primers( size_type &s )
        {
            static size_type nums[] =
            {
                7U,          23U,
                53U,         97U,         193U,       389U,       769U,
                1543U,       3079U,       6151U,      12289U,     24593U,
                49157U,      98317U,      196613U,    393241U,    786433U,
                1572869U,    3145739U,    6291469U,   12582917U,  25165843U,
                50331653U,   100663319U,  201326611U, 402653189U, 805306457U,
                1610612741U, 3221225473U, 4294967291U
            };

            s = sizeof(nums)/sizeof(nums[0]);
            return nums;
        }

        static size_type max_nb_buckets( void )
        {
            size_type s = 0;
            auto first = primers( s );

            return *(first + s - 1);
        }

        static size_type next_primer( size_type n )
        {
            size_type s = 0;
            auto first = primers( s );
            auto last = first + s;

            for ( ; first != last; ++first )
            {
                if ( *first >= n )
                {
                    return *first;
                }
            }

            return *(last - 1);
        }

        static size_type prev_primer( size_type n )
        {
            size_type s = 0;
            auto first = primers(s);
            auto last = first + s;
            auto pos = first;

            for ( ; pos != last; ++pos )
            {
                if ( *pos > n )
                {
                    break;
                }
            }

            if ( pos != begin )
            {
                -- pos;
            }

            return *pos;
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
        typedef iterator local_iterator;

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

            bool operator == ( const const_iterator &x ) const
            {
                return iter == x.iter;
            }

            bool operator != ( const const_iterator &x ) const
            {
                return iter != x.iter;
            }

            node_type iter;
        };
        typedef const_iterator const_local_iterator;

        hash_table( size_type n )
        {
        }

        hash_table( const self_type &x )
        {
        }

        hash_table( self_type &&x )
        {
        }

        ~hash_table( void )
        {
        }

        self_type& operator = ( const self_type &x )
        {
            return (*this);
        }

        self_type& operator = ( self_type &&x )
        {
            return (*this);
        }

        bool empty( void ) const
        {
            return false;
        }

        size_type size( void ) const
        {
            return m_num_elements;
        }

        size_type bucket_count( void ) const
        {
            return m_num_buckets;
        }

        size_type elems_in_bucket( size_type n ) const
        {
            return 0;
        }

        float load_factor( void ) const
        {
            return (float)size()/(float)bucket_count();
        }

        float max_load_factor( void ) const
        {
            return m_max_load_factor;
        }

        void max_load_factor( float x )
        {
        }

        iterator begin( void )
        {
            return iterator( m_elements );
        }

        const_iterator begin( void ) const
        {
            return const_iterator( m_elements );
        }

        local_iterator begin( size_type n )
        {
            return local_iterator( null() );
        }

        const_local_iterator begin( size_type n ) const
        {
            return local_iterator( null() );
        }

        iterator end( void )
        {
            return iterator( null() );
        }

        const_iterator end( void ) const
        {
            return const_iterator( null() );
        }

        local_iterator end( size_type n )
        {
            return local_iterator( null() );
        }

        const_local_iterator end( size_type n ) const
        {
            return const_local_iterator( null() );
        }

        iterator find( const key_type &k )
        {
            return iterator( null() );
        }

        const_iterator find( const key_type &k ) const
        {
            return const_iterator( null() );
        }

        pair<iterator,iterator> equal_range( const key_type &k )
        {
            return pair<iterator,iterator>(end(),end());
        }

        pair<const_iterator,const_iterator> equal_range( const key_type &k ) const
        {
            return pair<const_iterator,const_iterator>(end(),end());
        }

        size_type count( const key_type &k ) const
        {
            return 0;
        }

        pair<iterator,bool> insert_unique( const value_type &x )
        {
            return pair<iterator,bool>(end(),false);
        }

        pair<iterator,bool> insert_unique( value_type &&x )
        {
            return pair<iterator,bool>(end(),false);
        }

        template <typename __iterator>
        void insert_unique( __iterator first, __iterator last )
        {
            for ( ; first != last; ++first )
            {
                insert_unique( *first );
            }
        }

        iterator insert_equal( const value_type &x )
        {
            return end();
        }

        iterator insert_equal( value_type &&x )
        {
            return end();
        }

        template <typename __iterator>
        void insert_equal( __iterator first, __iterator last )
        {
            for ( ; first != last; ++first )
            {
                insert_equal( *first );
            }
        }

        void erase( const_iterator x )
        {
        }

        void erase( const_iterator first, const_iterator last )
        {
        }

        size_type erase( const key_type &k )
        {
            return 0;
        }

        void swap( self_type &x )
        {
        }

    private:
        void initialize_buckets( size_type n )
        {
            m_num_buckets = next_primer( n );
            auto count = m_num_buckets / NODE_COUNT;
            if ( m_num_buckets - count*NODE_COUNT > 0 )
            {
                ++ count;
            }

            
        }

    private:
        hash_type  m_hash;
        keyof_type m_keyof;
        keyeq_type m_keyeq;
        size_type  m_num_buckets;
        node_type  m_buckets;
        size_type  m_num_elements;
        node_type  m_elements;
        float      m_max_load_factor;
    };
} /* namespace ray */

#endif /* __BASE_HASH_TABLE_HPP_20161027130607_2B032EC0_81F0_4202_B187_204A2F5D3020__ */
