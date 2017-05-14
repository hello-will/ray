/**
 * @file    set.hpp
 * @version 1.0
 * @author  hello_will@yeah.net
 * @date    2016/09/29 15:50:14
**/

#ifndef __BASE_SET_HPP_20160929155022_C6FC265A_8A81_47B7_91D6_9D5A8691E87E__
#define __BASE_SET_HPP_20160929155022_C6FC265A_8A81_47B7_91D6_9D5A8691E87E__

#include <base/rb_tree.hpp>
#include <base/normal_alloc.hpp>

namespace ray
{
    template <typename __key,
              typename __comp = ray::less<__key>,
              typename __alloc = ray::normal_alloc>
    class set
    {
        typedef uint32_t size_type;
        typedef __key key_type;
        typedef __comp comp_type;
        typedef __alloc alloc_type;
        typedef typename ray::set<key_type,
                                  comp_type,
                                  alloc_type> self_type;
        typedef key_type value_type;
        struct keyof_type
        {
            const key_type& operator () ( const value_type &x ) const
            {
                return x;
            }
        };
        typedef typename ray::rb_tree<key_type,
                                      comp_type,
                                      value_type,
                                      keyof_type,
                                      alloc_type> rt_type;

    public:
        typedef typename rt_type::iterator iterator;
        typedef typename rt_type::const_iterator const_iterator;

        set( void )
        { }

        set( const self_type &x )
            : m_tree( x.m_tree )
        { }

        set( self_type &&x )
            : m_tree( ray::move(x.m_tree) )
        { }

        template <typename __iterator>
        set( __iterator first, __iterator last )
        {
            m_tree.insert_unique( first, last );
        }

        self_type& operator = ( const self_type &x )
        {
            m_tree = x.m_tree;

            return (*this);
        }

        self_type& operator = ( self_type &&x )
        {
            m_tree = ray::move( x.m_tree );

            return (*this);
        }

        bool empty( void ) const
        {
            return m_tree.empty();
        }

        size_type size( void ) const
        {
            return m_tree.size();
        }

        iterator begin( void )
        {
            return m_tree.begin();
        }

        const_iterator begin( void ) const
        {
            return m_tree.begin();
        }

        iterator end( void )
        {
            return m_tree.end();
        }

        const_iterator end( void ) const
        {
            return m_tree.end();
        }

        iterator find( const key_type &k )
        {
            return m_tree.find( k );
        }

        const_iterator find( const key_type &k ) const
        {
            return m_tree.find( k );
        }

        iterator lower_bound( const key_type &k )
        {
            return m_tree.lower_bound( k );
        }

        const_iterator lower_bound( const key_type &k ) const
        {
            return m_tree.lower_bound( k );
        }

        iterator upper_bound( const key_type &k )
        {
            return m_tree.upper_bound( k );
        }

        const_iterator upper_bound( const key_type &k ) const
        {
            return m_tree.upper_bound( k );
        }

        pair<iterator,iterator> equal_range( const key_type &k )
        {
            return m_tree.equal_range_unique( k );
        }

        pair<const_iterator,const_iterator> equal_range( const key_type &k ) const
        {
            return m_tree.equal_range_unique( k );
        }

        size_type count( const key_type &k ) const
        {
            return m_tree.count( k );
        }

        pair<iterator,bool> insert( const key_type &k )
        {
            return m_tree.insert_unique( k );
        }

        pair<iterator, bool> insert( key_type &&k )
        {
            return m_tree.insert_unique( ray::move(k) );
        }

        iterator insert( iterator pos, const key_type &k )
        {
            return m_tree.insert_unique( pos, k );
        }

        iterator insert( iterator pos, key_type &&k )
        {
            return m_tree.insert_unique( pos, ray::move(k) );
        }

        template <typename __iterator>
        void insert( __iterator first, __iterator last )
        {
            m_tree.insert_unique( first, last );
        }

        void erase( iterator pos )
        {
            m_tree.erase( pos );
        }

        void erase( iterator first, iterator last )
        {
            m_tree.erase( first, last );
        }

        size_type erase( const key_type &k )
        {
            return m_tree.erase( k );
        }

        void clear( void )
        {
            m_tree.clear();
        }

        void swap( self_type &x )
        {
            m_tree.swap( x.m_tree );
        }

    private:
        rt_type m_tree;
    };

    template <typename __key,
              typename __comp = ray::less<__key>,
              typename __alloc = ray::normal_alloc>
    class multiset
    {
        typedef uint32_t size_type;
        typedef __key key_type;
        typedef __comp comp_type;
        typedef __alloc alloc_type;
        typedef typename ray::multiset<key_type,
                                       comp_type,
                                       alloc_type> self_type;
        typedef key_type value_type;
        struct keyof_type
        {
            const key_type& operator () ( const value_type &x ) const
            {
                return x;
            }
        };
        typedef typename ray::rb_tree<key_type,
                                      comp_type,
                                      value_type,
                                      keyof_type,
                                      alloc_type> rt_type;

    public:
        typedef typename rt_type::iterator iterator;
        typedef typename rt_type::const_iterator const_iterator;

        multiset( void )
        { }

        multiset( const self_type &x )
            : m_tree( x.m_tree )
        { }

        multiset( self_type &&x )
            : m_tree( ray::move(x.m_tree) )
        { }

        template <typename __iterator>
        multiset( __iterator first, __iterator last )
        {
            m_tree.insert_equal( first, last );
        }

        self_type& operator = ( const self_type &x )
        {
            m_tree = x.m_tree;

            return (*this);
        }

        self_type& operator = ( self_type &&x )
        {
            m_tree = ray::move( x.m_tree );

            return (*this);
        }

        bool empty( void ) const
        {
            return m_tree.empty();
        }

        size_type size( void ) const
        {
            return m_tree.size();
        }

        iterator begin( void )
        {
            return m_tree.begin();
        }

        const_iterator begin( void ) const
        {
            return m_tree.begin();
        }

        iterator end( void )
        {
            return m_tree.end();
        }

        const_iterator end( void ) const
        {
            return m_tree.end();
        }

        iterator find( const key_type &k )
        {
            return m_tree.find( k );
        }

        const_iterator find( const key_type &k ) const
        {
            return m_tree.find( k );
        }

        iterator lower_bound( const key_type &k )
        {
            return m_tree.lower_bound( k );
        }

        const_iterator lower_bound( const key_type &k ) const
        {
            return m_tree.lower_bound( k );
        }

        iterator upper_bound( const key_type &k )
        {
            return m_tree.upper_bound( k );
        }

        const_iterator upper_bound( const key_type &k ) const
        {
            return m_tree.upper_bound( k );
        }

        pair<iterator,iterator> equal_range( const key_type &k )
        {
            return m_tree.equal_range( k );
        }

        pair<const_iterator,const_iterator> equal_range( const key_type &k ) const
        {
            return m_tree.equal_range( k );
        }

        size_type count( const key_type &k ) const
        {
            return m_tree.count( k );
        }

        iterator insert( const key_type &k )
        {
            return m_tree.insert_equal( k );
        }

        iterator insert( key_type &&k )
        {
            return m_tree.insert_equal( ray::move(k) );
        }

        iterator insert( iterator pos, const key_type &k )
        {
            return m_tree.insert_equal( pos, k );
        }

        iterator insert( iterator pos, key_type &&k )
        {
            return m_tree.insert_equal( pos, ray::move(k) );
        }

        template <typename __iterator>
        void insert( __iterator first, __iterator last )
        {
            m_tree.insert_equal( first, last );
        }

        void erase( iterator pos )
        {
            m_tree.erase( pos );
        }

        void erase( iterator first, iterator last )
        {
            m_tree.erase( first, last );
        }

        size_type erase( const key_type &k )
        {
            return m_tree.erase( k );
        }

        void clear( void )
        {
            m_tree.clear();
        }

        void swap( self_type &x )
        {
            m_tree.swap( x.m_tree );
        }

    private:
        rt_type m_tree;
    };
} /* namespace ray */

#endif /* #ifndef __BASE_SET_HPP_20160929155022_C6FC265A_8A81_47B7_91D6_9D5A8691E87E__ */
