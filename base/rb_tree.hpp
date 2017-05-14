/**
 * @file    rb_tree.hpp
 * @version 1.0
 * @author  hello_will@yeah.net
 * @date    2016/09/26 11:10:07
**/

#ifndef __BASE_RB_TREE_HPP_20160926111019_E2D47CBF_0B70_4873_8577_00F51066F84E__
#define __BASE_RB_TREE_HPP_20160926111019_E2D47CBF_0B70_4873_8577_00F51066F84E__

#include <base/pair.hpp>
#include <base/singleton.hpp>

namespace ray
{
    template <typename __key,
              typename __comp,
              typename __type,
              typename __keyof,
              typename __alloc>
    class rb_tree
    {
        typedef uint32_t size_type;
        typedef __key key_type;
        typedef __comp comp_type;
        typedef __type value_type;
        typedef __keyof keyof_type;
        typedef __alloc alloc_type;
        typedef typename alloc_type::mem_type node_type;
        typedef typename ray::rb_tree<key_type,
                                      comp_type,
                                      value_type,
                                      keyof_type,
                                      alloc_type> self_type;

        struct node
        {
            node_type  parent;
            node_type  left;
            node_type  right;
            node_type  color;
            value_type value;
        };

        static node_type rb_black/* = (node_type)0*/;
        static node_type rb_red/* = (node_type)1*/;

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

        static node_type minimun( node_type x )
        {
            auto __x = touch( x );

            while ( __x->left != null() )
            {
                x = __x->left;
                __x = touch( x );
            }

            return x;
        }

        static node_type maximun( node_type x )
        {
            auto __x = touch( x );

            while ( __x->right != null() )
            {
                x = __x->right;
                __x = touch( x );
            }

            return x;
        }

        static node_type decrement( node_type x )
        {
            auto __x = touch( x );
            auto __x_parent = touch( __x->parent );

            if ( __x->color == rb_red && __x_parent->parent == x )
            {
                x = __x->right;
            }
            else if ( __x->left != null() )
            {
                x = maximun( __x->left );
            }
            else
            {
                auto y = __x->parent;
                auto __y = __x_parent;

                while ( x == __y->left )
                {
                    x = y;
                    __x = __y;
                    y = __y->parent;
                    __y = touch( y );
                }
                x = y;
            }

            return x;
        }

        static node_type increment( node_type x )
        {
            auto __x = touch( x );

            if ( __x->right != null() )
            {
                x = minimun( __x->right );
            }
            else
            {
                auto y = __x->parent;
                auto __y = touch( y );

                while ( x == __y->right )
                {
                    x = y;
                    __x = __y;
                    y = __y->parent;
                    __y = touch( y );
                }

                if ( __x->right != y )
                {
                    x = y;
                }
            }

            return x;
        }

        static void rotate_left( node_type x, node_type &root )
        {
            auto __x = touch( x );
            auto y = __x->right;
            auto __y = touch( y );
            auto __root = touch( root );

            __x->right = __y->left;
            if ( __y->left != null() )
            {
                auto __y_left = touch( __y->left );

                __y_left->parent = x;
            }
            __y->parent = __x->parent;

            if ( x == root )
            {
                root = y;
            }
            else
            {
                auto __x_parent = touch( __x->parent );

                if ( x == __x_parent->left )
                {
                    __x_parent->left = y;
                }
                else
                {
                    __x_parent->right = y;
                }
            }
            __y->left = x;
            __x->parent = y;
        }

        static void rotate_right( node_type x, node_type &root )
        {
            auto __x = touch( x );
            auto y = __x->left;
            auto __y = touch( y );
            auto __root = touch( root );

            __x->left = __y->right;
            if ( __y->right != null() )
            {
                auto __y_right = touch( __y->right );

                __y_right->parent = x;
            }
            __y->parent = __x->parent;

            if ( x == root )
            {
                root = y;
            }
            else
            {
                auto __x_parent = touch( __x->parent );

                if ( x == __x_parent->right )
                {
                    __x_parent->right = y;
                }
                else
                {
                    __x_parent->left = y;
                }
            }
            __y->right = x;
            __x->parent = y;
        }

        static void rebalance( node_type x, node_type &root )
        {
            auto __x = touch( x );
            auto __x_parent = touch( __x->parent );

            __x->color = rb_red;
            while ( x != root && __x_parent->color == rb_red )
            {
                auto __x_parent_parent = touch( __x_parent->parent );
                if ( __x->parent == __x_parent_parent->left )
                {
                    auto __y = touch( __x_parent_parent->right );
                    if ( __y && __y->color == rb_red )
                    {
                        __x_parent->color = rb_black;
                        __y->color = rb_black;
                        __x_parent_parent->color = rb_red;

                        x = __x_parent->parent;
                        __x = __x_parent_parent;
                        __x_parent = touch( __x->parent );
                    }
                    else
                    {
                        if ( x == __x_parent->right )
                        {
                            x = __x->parent;
                            __x = __x_parent;
                            rotate_left( x, root );

                            __x_parent = touch( __x->parent );
                            __x_parent_parent = touch( __x_parent->parent );
                        }

                        __x_parent->color = rb_black;
                        __x_parent_parent->color = rb_red;
                        rotate_right( __x_parent->parent, root );
                    }
                }
                else
                {
                    auto __y = touch( __x_parent_parent->left );
                    if ( __y && __y->color == rb_red )
                    {
                        __x_parent->color = rb_black;
                        __y->color = rb_black;
                        __x_parent_parent->color = rb_red;

                        x = __x_parent->parent;
                        __x = __x_parent_parent;
                        __x_parent = touch( __x->parent );
                    }
                    else
                    {
                        if ( x == __x_parent->left )
                        {
                            x = __x->parent;
                            __x = __x_parent;
                            rotate_right( x, root );

                            __x_parent = touch( __x->parent );
                            __x_parent_parent = touch( __x_parent->parent );
                        }

                        __x_parent->color = rb_black;
                        __x_parent_parent->color = rb_red;
                        rotate_left( __x_parent->parent, root );
                    }
                }
            }

            auto __root = touch( root );
            __root->color = rb_black;
        }

        static node_type rebalance_for_erase( node_type z,
                                              node_type &root,
                                              node_type &leftmost,
                                              node_type &rightmost )
        {
            auto x = null();
            auto x_parent = null();
            auto y = z;
            auto __y = touch( y );
            node *__x = nullptr;
            node *__x_parent = nullptr;

            if ( __y->left == null() )
            {
                x = __y->right;
                __x = touch( x );
            }
            else
            {
                if ( __y->right == null() )
                {
                    x = __y->left;
                    __x = touch( x );
                }
                else
                {
                    y = minimun( __y->right );
                    __y = touch( y );
                    x = __y->right;
                    __x = touch( x );
                }
            }

            if ( y != z )
            {
                auto __z = touch( z );
                auto __z_left = touch( __z->left );

                __z_left->parent = y;
                __y->left = __z->left;
                if ( y != __z->right )
                {
                    auto __y_parent = touch( __y->parent );

                    x_parent = __y->parent;
                    __x_parent = __y_parent;
                    if ( __x )
                    {
                        __x->parent = __y->parent;
                    }
                    __y_parent->left = x;
                    __y->right = __z->right;

                    auto __z_right = touch( __z->right );
                    __z_right->parent = y;
                }
                else
                {
                    x_parent = y;
                    __x_parent = __y;
                }

                if ( root == z )
                {
                    root = y;
                }
                else
                {
                    auto __z_parent = touch( __z->parent );
                    if ( __z_parent->left == z )
                    {
                        __z_parent->left = y;
                    }
                    else
                    {
                        __z_parent->right = y;
                    }
                }

                __y->parent = __z->parent;
                auto __color = __y->color;
                __y->color = __z->color;
                __z->color = __color;
                y = z;
                __y = __z;
            }
            else
            {
                x_parent = __y->parent;
                __x_parent = touch( __y->parent );
                if ( __x )
                {
                    __x->parent = __y->parent;
                }

                if ( root == z )
                {
                    root = x;
                }
                else
                {
                    auto __z = touch( z );
                    auto __z_parent = touch( __z->parent );
                    if ( __z_parent->left == z )
                    {
                        __z_parent->left = x;
                    }
                    else
                    {
                        __z_parent->right = x;
                    }
                }

                if ( leftmost == z )
                {
                    auto __z = touch( z );
                    if ( __z->right == null() )
                    {
                        leftmost = __z->parent;
                    }
                    else
                    {
                        leftmost = minimun( x );
                    }
                }

                if ( rightmost == z )
                {
                    auto __z = touch( z );
                    if ( __z->left == null() )
                    {
                        rightmost = __z->parent;
                    }
                    else
                    {
                        rightmost = maximun( x );
                    }
                }
            }

            if ( __y->color != rb_red )
            {
                while ( x != root && (x == null() || __x->color == rb_black) )
                {
                    if ( x == __x_parent->left )
                    {
                        auto w = __x_parent->right;
                        auto __w = touch( w );
                        if ( __w->color == rb_red )
                        {
                            __w->color = rb_black;
                            __x_parent->color = rb_red;

                            rotate_left( x_parent, root );
                            w = __x_parent->right;
                            __w = touch( w );
                        }

                        auto __w_left = touch( __w->left );
                        auto __w_right = touch( __w->right );
                        if ( (__w->left == null() || __w_left->color == rb_black) &&
                             (__w->right == null() || __w_right->color == rb_black) )
                        {
                            __w->color = rb_red;
                            x = x_parent;
                            __x = __x_parent;
                            x_parent = __x_parent->parent;
                            __x_parent = touch( x_parent );
                        }
                        else
                        {
                            if ( __w->right == null() || __w_right->color == rb_black )
                            {
                                if ( __w_left )
                                {
                                    __w_left->color = rb_black;
                                }
                                __w->color = rb_red;

                                rotate_right( w, root );
                                w = __x_parent->right;
                                __w = touch( w );
                                __w_left = touch( __w->left );
                                __w_right = touch( __w->right );
                            }

                            __w->color = __x_parent->color;
                            __x_parent->color = rb_black;
                            if ( __w_right )
                            {
                                __w_right->color = rb_black;
                            }

                            rotate_left( x_parent, root );
                            break;
                        }
                    }
                    else
                    {
                        auto w = __x_parent->left;
                        auto __w = touch( w );
                        if ( __w->color == rb_red )
                        {
                            __w->color = rb_black;
                            __x_parent->color = rb_black;

                            rotate_right( x_parent, root );
                            w = __x_parent->left;
                            __w = touch( w );
                        }

                        auto __w_left = touch( __w->left );
                        auto __w_right = touch( __w->right );
                        if ( (__w->right == null() || __w_right->color == rb_black) &&
                             (__w->left == null() || __w_left->color == rb_black) )
                        {
                            __w->color = rb_red;
                            x = x_parent;
                            __x = __x_parent;
                            x_parent = __x_parent->parent;
                            __x_parent = touch( x_parent );
                        }
                        else
                        {
                            if ( __w->left == null() || __w_left->color == rb_black )
                            {
                                if ( __w_right )
                                {
                                    __w_right->color = rb_black;
                                }
                                __w->color = rb_red;

                                rotate_left( w, root );
                                w = __x_parent->left;
                                __w = touch( w );
                                __w_left = touch( __w->left );
                                __w_right = touch( __w->right );
                            }

                            __w->color = __x_parent->color;
                            __x_parent->color = rb_black;
                            if ( __w_left )
                            {
                                __w_left->color = rb_black;
                            }

                            rotate_right( x_parent, root );
                            break;
                        }
                    }
                }

                if ( __x )
                {
                    __x->color = rb_black;
                }
            }

            return y;
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
                iter = self_type::increment( iter );

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
                iter = self_type::decrement( iter );

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

            const value_type* operator -> (void) const
            {
                auto __iter = self_type::touch( iter );

                return &__iter->value;
            }

            const_iterator operator ++ ( void )
            {
                iter = self_type::increment( iter );

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
                iter = self_type::decrement( iter );

                return *this;
            }

            const_iterator operator -- ( int )
            {
                auto tmp = *this;

                --(*this);

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

        rb_tree( void )
            : m_size( 0 )
            , m_node( null() )
        {
            empty_initialize();
        }

        rb_tree( const self_type &x )
            : m_size( 0 )
            , m_node( null() )
        {
            empty_initialize();

            auto __node = touch( m_node );
            auto __x_node = touch( x.m_node );
            if ( __x_node->parent != null() )
            {
                __node->parent = copy_node( __x_node->parent, m_node );
                __node->left = minimun( __node->parent );
                __node->right = maximun( __node->parent );
                __node->color = rb_red;
            }
            m_size = x.m_size;
        }

        rb_tree( self_type &&x )
            : m_size( 0 )
            , m_node( null() )
        {
            empty_initialize();
            swap( x );
        }

        ~rb_tree( void )
        {
            clear();
            dealloc( m_node );
            m_node = null();
        }

        self_type& operator = ( const self_type &x )
        {
            if ( this != &x )
            {
                clear();

                auto __node = touch( m_node );
                auto __x_node = touch( x.m_node );
                if ( __x_node->parent != null() )
                {
                    __node->parent = copy_node( __x_node->parent, m_node );
                    __node->left = minimun( __node->parent );
                    __node->right = maximun( __node->parent );
                    __node->color = rb_red;
                }
                m_size = x.m_size;
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
            return 0 == m_size;
        }

        size_type size( void ) const
        {
            return m_size;
        }

        iterator begin( void )
        {
            auto __node = touch( m_node );

            return iterator( __node->left );
        }

        const_iterator begin( void ) const
        {
            auto __node = touch( m_node );

            return const_iterator( __node->left );
        }

        iterator end( void )
        {
            return iterator( m_node );
        }

        const_iterator end( void ) const
        {
            return iterator( m_node );
        }

        iterator find( const key_type &k )
        {
            return iterator( find_node(k) );
        }

        const_iterator find( const key_type &k ) const
        {
            return const_iterator( find_node(k) );
        }

        iterator lower_bound( const key_type &k )
        {
            return iterator( lower_node(k) );
        }

        const_iterator lower_bound( const key_type &k ) const
        {
            return const_iterator( lower_node(k) );
        }

        iterator upper_bound( const key_type &k )
        {
            return iterator( upper_node(k) );
        }

        const_iterator upper_bound( const key_type &k ) const
        {
            return const_iterator( upper_node(k) );
        }

        pair<iterator, iterator> equal_range( const key_type &k )
        {
            return pair<iterator,iterator>( lower_bound(k), upper_bound(k) );
        }

        pair<const_iterator,const_iterator> equal_range( const key_type &k ) const
        {
            return pair<const_iterator,const_iterator>( lower_bound(k), upper_bound(k) );
        }

        pair<iterator,iterator> equal_range_unique( const key_type &k )
        {
            pair<iterator,iterator> r;
            node *__r_second = nullptr;

            r.second = lower_bound( k );
            __r_second = touch( r.second.iter );
            if ( r.second.iter != m_node && !m_comp(k,m_keyof(__r_second->value)) )
            {
                r.first = r.second++;
            }
            else
            {
                r.first = r.second;
            }

            return r;
        }

        pair<const_iterator,const_iterator> equal_range_unique( const key_type &k ) const
        {
            pair<const_iterator,const_iterator> r;
            node *__r_second = nullptr;

            r.second = lower_bound( k );
            __r_second = touch( r.second.iter );
            if ( r.second.iter != m_node && !m_comp(k,m_keyof(__r_second->value)) )
            {
                r.first = r.second++;
            }
            else
            {
                r.first = r.second;
            }

            return r;
        }

        size_type count( const keyof_type &k ) const
        {
            size_type n = 0;
            auto r = equal_range( k );

            while ( r.first != r.second )
            {
                ++ n;
                ++ r.first;
            }

            return n;
        }

        iterator insert_equal( const value_type &__val )
        {
            auto __node = touch( m_node );
            auto y = m_node;
            auto x = __node->parent;
            auto __x = touch( x );

            while ( x != null() )
            {
                y = x;
                if ( m_comp(m_keyof(__val),m_keyof(__x->value)) )
                {
                    x = __x->left;
                    __x = touch( x );
                }
                else
                {
                    x = __x->right;
                    __x = touch( x );
                }
            }

            return iterator( insert_with_balance(y,__val,x) );
        }

        iterator insert_equal( value_type &&__val )
        {
            auto __node = touch( m_node );
            auto y = m_node;
            auto x = __node->parent;
            auto __x = touch( x );

            while ( x != null() )
            {
                y = x;
                if ( m_comp(m_keyof(__val),m_keyof(__x->value)) )
                {
                    x = __x->left;
                    __x = touch( x );
                }
                else
                {
                    x = __x->right;
                    __x = touch( x );
                }
            }

            return iterator( insert_with_balance(y,ray::move(__val),x) );
        }

        iterator insert_equal( iterator pos, const value_type &__val )
        {
            auto __pos = touch( pos.iter );
            auto __node = touch( m_node );

            if ( pos.iter == __node->left )
            {
                if ( size() <= 0 )
                {
                    return insert_equal( __val );
                }

                if ( !m_comp(m_keyof(__pos->value),m_keyof(__val)) )
                {
                    return iterator( insert_with_balance(pos.iter,__val,pos.iter) );
                }
                else
                {
                    if ( __pos->left == pos.iter )
                    {
                        return iterator( insert_with_balance(pos.iter,__val) );
                    }

                    auto after = pos; ++after;
                    auto __after = touch( after.iter );
                    if ( after.iter == m_node || !m_comp(m_keyof(__after->value),m_keyof(__val)) )
                    {
                        if ( __pos->right == null() )
                        {
                            return iterator( insert_with_balance(pos.iter,__val,null(),pos.iter) );
                        }
                        else
                        {
                            return iterator( insert_with_balance(pos.iter,__val,after.iter) );
                        }
                    }
                    else
                    {
                        return insert_equal( __val );
                    }
                }
            }
            else if ( pos.iter == m_node )
            {
                auto rightmost = __node->right;
                auto __rightmost = touch( rightmost );
                if ( !m_comp(m_keyof(__val),m_keyof(__rightmost->value)) )
                {
                    return iterator( insert_with_balance(rightmost,__val,null(),pos.iter) );
                }
                else
                {
                    return insert_equal( __val );
                }
            }
            else
            {
                auto before = pos; --before;
                auto __before = touch( before.iter );
                auto __comp_pos_v = m_comp( m_keyof(__pos->value), m_keyof(__val) );

                if ( !__comp_pos_v && !m_comp(m_keyof(__val),m_keyof(__before->value)) )
                {
                    if ( __before->right == null() )
                    {
                        return iterator( insert_with_balance(before.iter,__val,null(),before.iter) );
                    }
                    else
                    {
                        return iterator( insert_with_balance(pos.iter,__val,pos.iter) );
                    }
                }
                else
                {
                    auto after = pos; ++after;
                    auto __after = touch( after.iter );

                    if ( __comp_pos_v && (after.iter == m_node || !m_comp(m_keyof(__after->value),m_keyof(__val))) )
                    {
                        if ( __pos->right == null() )
                        {
                            return iterator( insert_with_balance(pos.iter,__val,null(),pos.iter) );
                        }
                        else
                        {
                            return iterator( insert_with_balance(after.iter,__val,after.iter) );
                        }
                    }
                    else
                    {
                        return insert_equal( __val );
                    }
                }
            }
        }

        iterator insert_equal( iterator pos, value_type &&__val )
        {
            auto __pos = touch( pos.iter );
            auto __node = touch( m_node );

            if ( pos.iter == __node->left )
            {
                if ( size() <= 0 )
                {
                    return insert_equal( ray::move(__val) );
                }

                if ( !m_comp(m_keyof(__pos->value),m_keyof(__val)) )
                {
                    return iterator( insert_with_balance(pos.iter,ray::move(__val),pos.iter) );
                }
                else
                {
                    if ( __pos->left == pos.iter )
                    {
                        return iterator( insert_with_balance(pos.iter,ray::move(__val)) );
                    }

                    auto after = pos; ++after;
                    auto __after = touch( after.iter );
                    if ( after.iter == m_node || !m_comp(m_keyof(__after->value),m_keyof(__val)) )
                    {
                        if ( __pos->right == null() )
                        {
                            return iterator( insert_with_balance(pos.iter,ray::move(__val),null(),pos.iter) );
                        }
                        else
                        {
                            return iterator( insert_with_balance(pos.iter,ray::move(__val),after.iter) );
                        }
                    }
                    else
                    {
                        return insert_equal( ray::move(__val) );
                    }
                }
            }
            else if ( pos.iter == m_node )
            {
                auto rightmost = __node->right;
                auto __rightmost = touch( rightmost );
                if ( !m_comp(m_keyof(__val),m_keyof(__rightmost->value)) )
                {
                    return iterator( insert_with_balance(rightmost,ray::move(__val),null(),pos.iter) );
                }
                else
                {
                    return insert_equal( ray::move(__val) );
                }
            }
            else
            {
                auto before = pos; --before;
                auto __before = touch( before.iter );
                auto __comp_pos_v = m_comp( m_keyof(__pos->value), m_keyof(__val) );

                if ( !__comp_pos_v && !m_comp(m_keyof(__val),m_keyof(__before->value)) )
                {
                    if ( __before->right == null() )
                    {
                        return iterator( insert_with_balance(before.iter,ray::move(__val),null(),before.iter) );
                    }
                    else
                    {
                        return iterator( insert_with_balance(pos.iter,ray::move(__val),pos.iter) );
                    }
                }
                else
                {
                    auto after = pos; ++after;
                    auto __after = touch( after.iter );

                    if ( __comp_pos_v && (after.iter == m_node || !m_comp(m_keyof(__after->value),m_keyof(__val))) )
                    {
                        if ( __pos->right == null() )
                        {
                            return iterator( insert_with_balance(pos.iter,ray::move(__val),null(),pos.iter) );
                        }
                        else
                        {
                            return iterator( insert_with_balance(after.iter,ray::move(__val),after.iter) );
                        }
                    }
                    else
                    {
                        return insert_equal( ray::move(__val) );
                    }
                }
            }
        }

        pair<iterator,bool> insert_unique( const value_type &__val )
        {
            auto __node = touch( m_node );
            auto y = m_node;
            auto x = __node->parent;
            auto __x = touch( x );
            auto _comp = true;

            while ( x != null() )
            {
                y = x;
                _comp = m_comp( m_keyof(__val), m_keyof(__x->value) );
                x = _comp ? __x->left : __x->right;
                __x = touch( x );
            }

            auto j = iterator( y );
            if ( _comp )
            {
                if ( j == begin() )
                {
                    return pair<iterator,bool>( insert_with_balance(y,__val,y), true );
                }
                else
                {
                    --j;
                }
            }

            auto __j = touch( j.iter );
            if ( m_comp(m_keyof(__j->value),m_keyof(__val)) )
            {
                return pair<iterator,bool>( insert_with_balance(y,__val,x), true );
            }

            return pair<iterator,bool>( j, false );
        }

        pair<iterator,bool> insert_unique( value_type &&__val )
        {
            auto __node = touch( m_node );
            auto y = m_node;
            auto x = __node->parent;
            auto __x = touch( x );
            auto _comp = true;

            while ( x != null() )
            {
                y = x;
                _comp = m_comp( m_keyof(__val), m_keyof(__x->value) );
                x = _comp ? __x->left : __x->right;
                __x = touch( x );
            }

            auto j = iterator( y );
            if ( _comp )
            {
                if ( j == begin() )
                {
                    return pair<iterator,bool>( insert_with_balance(y,ray::move(__val),y), true );
                }
                else
                {
                    --j;
                }
            }

            auto __j = touch( j.iter );
            if ( m_comp(m_keyof(__j->value),m_keyof(__val)) )
            {
                return pair<iterator,bool>( insert_with_balance(y,ray::move(__val),x), true );
            }

            return pair<iterator,bool>( j, false );
        }

        iterator insert_unique( iterator pos, const value_type &__val )
        {
            auto __pos = touch( pos.iter );
            auto __node = touch( m_node );

            if ( pos.iter == __node->left )
            {
                if ( empty() )
                {
                    return insert_unique(__val).first;
                }

                if ( m_comp(m_keyof(__val),m_keyof(__pos->value)) )
                {
                    return iterator( insert_with_balance(pos.iter,__val,pos.iter) );
                }
                else
                {
                    auto __comp_pos_v = m_comp( m_keyof(__pos->value), m_keyof(__val) );
                    if ( !__comp_pos_v )
                    {
                        return pos;
                    }

                    auto after = pos; ++after;
                    if ( after.iter == m_node )
                    {
                        return iterator( insert_with_balance(pos.iter,__val,null(),pos.iter) );
                    }

                    auto __after = touch( after.iter );
                    if ( m_comp(m_keyof(__val),m_keyof(__after->value)) )
                    {
                        if ( __pos->right == null() )
                        {
                            return iterator( insert_with_balance(pos.iter,__val,null(),pos.iter) );
                        }
                        else
                        {
                            return iterator( insert_with_balance(after.iter,__val,after.iter) );
                        }
                    }
                    else
                    {
                        return insert_unique(__val).first;
                    }
                }
            }
            else if ( pos.iter == m_node )
            {
                auto rightmost = __node->right;
                auto __rightmost = touch( rightmost );
                if ( m_comp(m_keyof(__rightmost->value),m_keyof(__val)) )
                {
                    return iterator( insert_with_balance(rightmost,__val,null(),pos.iter) );
                }
                else
                {
                    return insert_unique(__val).first;
                }
            }
            else
            {
                auto before = pos; --before;
                auto __before = touch( before.iter );
                auto __comp_v_pos = m_comp( m_keyof(__val), m_keyof(__pos->value) );

                if ( __comp_v_pos && m_comp(m_keyof(__before->value),m_keyof(__val)) )
                {
                    if ( __before->right == null() )
                    {
                        return iterator( insert_with_balance(before.iter,__val,null(),before.iter) );
                    }
                    else
                    {
                        return iterator( insert_with_balance(pos.iter,__val,pos.iter) );
                    }
                }
                else
                {
                    auto after = pos; ++after;
                    auto __comp_pos_v = !__comp_v_pos;
                    if ( !__comp_v_pos )
                    {
                        __comp_pos_v = m_comp( m_keyof(__pos->value), m_keyof(__val) );
                    }

                    auto __after = touch( after.iter );
                    if ( !__comp_v_pos && __comp_pos_v && (after.iter == m_node || m_comp(m_keyof(__val),m_keyof(__after->value))) )
                    {
                        if ( __pos->right == null() )
                        {
                            return iterator( insert_with_balance(pos.iter,__val,null(),pos.iter) );
                        }
                        else
                        {
                            return iterator( insert_with_balance(after.iter,__val,after.iter) );
                        }
                    }
                    else
                    {
                        if ( __comp_v_pos == __comp_pos_v )
                        {
                            return pos;
                        }
                        else
                        {
                            return insert_unique(__val).first;
                        }
                    }
                }
            }
        }

        iterator insert_unique( iterator pos, value_type &&__val )
        {
            auto __pos = touch( pos.iter );
            auto __node = touch( m_node );

            if ( pos.iter == __node->left )
            {
                if ( empty() )
                {
                    return insert_unique(ray::move(__val)).first;
                }

                if ( m_comp(m_keyof(__val),m_keyof(__pos->value)) )
                {
                    return iterator( insert_with_balance(pos.iter,ray::move(__val),pos.iter) );
                }
                else
                {
                    auto __comp_pos_v = m_comp(m_keyof(__pos->value),m_keyof(__val));
                    if ( !__comp_pos_v )
                    {
                        return pos;
                    }

                    auto after = pos; ++after;
                    if ( after.iter == m_node )
                    {
                        return iterator( insert_with_balance(pos.iter,ray::move(__val),null(),pos.iter) );
                    }

                    auto __after = touch( after.iter );
                    if ( m_comp(m_keyof(__val),m_keyof(__after->value)) )
                    {
                        if ( __pos->right == null() )
                        {
                            return iterator( insert_with_balance(pos.iter,ray::move(__val),null(),pos.iter) );
                        }
                        else
                        {
                            return iterator( insert_with_balance(after.iter,ray::move(__val),after.iter) );
                        }
                    }
                    else
                    {
                        return insert_unique(ray::move(__val)).first;
                    }
                }
            }
            else if ( pos.iter == m_node )
            {
                auto rightmost = __node->right;
                auto __rightmost = touch( rightmost );
                if ( m_comp(m_keyof(__rightmost->value),m_keyof(__val)) )
                {
                    return iterator( insert_with_balance(rightmost,ray::move(__val),null(),pos.iter) );
                }
                else
                {
                    return insert_unique(ray::move(__val)).first;
                }
            }
            else
            {
                auto before = pos; --before;
                auto __before = touch( before.iter );
                auto __comp_v_pos = m_comp( m_keyof(__val), m_keyof(__pos->value) );

                if ( __comp_v_pos && m_comp(m_keyof(__before->value),m_keyof(__val)) )
                {
                    if ( __before->right == null() )
                    {
                        return iterator( insert_with_balance(before.iter,ray::move(__val),null(),before.iter) );
                    }
                    else
                    {
                        return iterator( insert_with_balance(pos.iter,ray::move(__val),pos.iter) );
                    }
                }
                else
                {
                    auto after = pos; ++after;
                    auto __comp_pos_v = !__comp_v_pos;
                    if ( !__comp_v_pos )
                    {
                        __comp_pos_v = m_comp( m_keyof(__pos->value), m_keyof(__val) );
                    }

                    auto __after = touch( after.iter );
                    if ( !__comp_v_pos && __comp_pos_v && (after.iter == m_node || m_comp(m_keyof(__val),m_keyof(__after->value))) )
                    {
                        if ( __pos->right == null() )
                        {
                            return iterator( insert_with_balance(pos.iter,ray::move(__val),null(),pos.iter) );
                        }
                        else
                        {
                            return iterator( insert_with_balance(after.iter,ray::move(__val),after.iter) );
                        }
                    }
                    else
                    {
                        if ( __comp_v_pos == __comp_pos_v )
                        {
                            return pos;
                        }
                        else
                        {
                            return insert_unique(ray::move(__val)).first;
                        }
                    }
                }
            }
        }

        template <typename __iterator>
        void insert_equal( __iterator first, __iterator last )
        {
            while ( first != last )
            {
                insert_equal( *first++ );
            }
        }

        template <typename __iterator>
        void insert_unique( __iterator first, __iterator last )
        {
            while ( first != last )
            {
                insert_unique( *first++ );
            }
        }

        void erase( iterator pos )
        {
            auto __node = touch( m_node );
            auto x = rebalance_for_erase( pos.iter,
                                          __node->parent,
                                          __node->left,
                                          __node->right );
            auto __x = touch( x );

            --m_size;
            destroy( &__x->value );
            dealloc( x );
        }

        void erase( iterator first, iterator last )
        {
            if ( first == begin() &&
                 last == end() )
            {
                clear();
            }
            else
            {
                while ( first != last )
                {
                    erase( first++ );
                }
            }
        }

        void erase( const key_type *first, const keyof_type *last )
        {
            while ( first != last )
            {
                erase( *first++ );
            }
        }

        size_type erase( const key_type &k )
        {
            size_type n = 0;
            auto r = equal_range( k );

            if ( r.first == begin() &&
                 r.second == end() )
            {
                n = m_size;
                clear();
            }
            else
            {
                while ( r.first != r.second )
                {
                    ++ n;
                    erase( r.first++ );
                }
            }

            return n;
        }

        size_type erase_unique( const key_type &k )
        {
            iterator it = find( k );

            if ( it != end() )
            {
                erase( it );

                return 1;
            }

            return 0;
        }

        void clear( void )
        {
            auto __node = touch( m_node );

            erase_without_rebalance( __node->parent );
            empty_initialize();
        }

        void swap( self_type &x )
        {
            auto __empty = empty();
            auto __x_empty = x.empty();

            if ( !__empty || !__x_empty )
            {
                auto __node = touch( m_node );
                auto __x_node = touch( x.m_node );

                ray::swap( __node->parent, __x_node->parent );
                ray::swap( __node->left, __x_node->left );
                ray::swap( __node->right, __x_node->right );
                ray::swap( __node->color, __x_node->color );
                ray::swap( m_size, x.m_size );

                if ( __x_empty )
                {
                    empty_initialize();
                }
                else
                {
                    auto __parent = touch( __node->parent );
                    __parent->parent = m_node;
                }

                if ( __empty )
                {
                    x.empty_initialize();
                }
                else
                {
                    auto __x_parent = touch( __x_node->parent );
                    __x_parent->parent = x.m_node;
                }
            }
        }

    private:
        void empty_initialize( void )
        {
            m_size = 0;
            if ( m_node == null() )
            {
                m_node = alloc();
            }

            auto __node = touch( m_node );
            if ( nullptr != __node )
            {
                __node->parent = null();
                __node->left = m_node;
                __node->right = m_node;
                __node->color = rb_red;
            }
        }

        node_type insert_with_balance( node_type parent,
                                       const value_type &__val,
                                       node_type on_left = null(),
                                       node_type on_right = null() )
        {
            auto new_node = null();
            auto __node = touch( m_node );
            auto __parent = touch( parent );
            node *__new_node = nullptr;

            if ( parent == m_node )
            {
                new_node = alloc();

                __new_node = touch( new_node );
                __new_node->parent = null();
                __new_node->left = null();
                __new_node->right = null();
                __new_node->color = rb_black;
                construct( &__new_node->value, __val );

                __node->parent = new_node;
                __node->left = new_node;
                __node->right = new_node;
            }
            else if ( on_right == null() &&
                      (on_left != null() || m_comp(m_keyof(__val),m_keyof(__parent->value))) )
            {
                new_node = alloc();

                __new_node = touch( new_node );
                __new_node->parent = null();
                __new_node->left = null();
                __new_node->right = null();
                __new_node->color = rb_black;
                construct( &__new_node->value, __val );

                __parent->left = new_node;
                if ( parent == __node->left )
                {
                    __node->left = new_node;
                }
            }
            else
            {
                new_node = alloc();

                __new_node = touch( new_node );
                __new_node->parent = null();
                __new_node->left = null();
                __new_node->right = null();
                __new_node->color = rb_black;
                construct( &__new_node->value, __val );

                __parent->right = new_node;
                if ( parent == __node->right )
                {
                    __node->right = new_node;
                }
            }

            __new_node->parent = parent;
            rebalance( new_node, __node->parent );
            ++m_size;

            return new_node;
        }

        node_type insert_with_balance( node_type parent,
                                       value_type &&__val,
                                       node_type on_left = null(),
                                       node_type on_right = null() )
        {
            auto new_node = null();
            auto __node = touch( m_node );
            auto __parent = touch( parent );
            node *__new_node = nullptr;

            if ( parent == m_node )
            {
                new_node = alloc();

                __new_node = touch( new_node );
                __new_node->parent = null();
                __new_node->left = null();
                __new_node->right = null();
                __new_node->color = rb_black;
                construct( &__new_node->value, ray::move(__val) );

                __node->parent = new_node;
                __node->left = new_node;
                __node->right = new_node;
            }
            else if ( on_right == null() &&
                      (on_left != null() || m_comp(m_keyof(__val),m_keyof(__parent->value))) )
            {
                new_node = alloc();

                __new_node = touch( new_node );
                __new_node->parent = null();
                __new_node->left = null();
                __new_node->right = null();
                __new_node->color = rb_black;
                construct( &__new_node->value, ray::move(__val) );

                __parent->left = new_node;
                if ( parent == __node->left )
                {
                    __node->left = new_node;
                }
            }
            else
            {
                new_node = alloc();

                __new_node = touch( new_node );
                __new_node->parent = null();
                __new_node->left = null();
                __new_node->right = null();
                __new_node->color = rb_black;
                construct( &__new_node->value, ray::move(__val) );

                __parent->right = new_node;
                if ( parent == __node->right )
                {
                    __node->right = new_node;
                }
            }

            __new_node->parent = parent;
            rebalance( new_node, __node->parent );
            ++m_size;

            return new_node;
        }

        void erase_without_rebalance( node_type x )
        {
            if ( x != null() )
            {
                auto __x = touch( x );
                erase_without_rebalance( __x->right );

                auto y = __x->left;
                auto __y = touch( y );

                destroy( &__y->value );
                dealloc( y );

                x = y;
                __x = __y;
            }
        }

        node_type copy_node( node_type x, node_type p )
        {
            auto __x = touch( x );
            auto __p = touch( p );
            auto top = alloc();
            auto __top = touch( top );

            __top->parent = p;
            __top->left = null();
            __top->right = null();
            __top->color = __x->color;
            construct( &__top->value, __x->value );

            if ( __x->right != null() )
            {
                __top->right = copy_node( __x->right, top );
            }
            p = top;
            __p = __top;
            x = __x->left;
            __x = touch( x );

            while ( x != null() )
            {
                auto y = alloc();
                auto __y = touch( y );

                __p->left = y;
                __y->parent = p;
                __y->left = null();
                __y->right = null();
                __y->color = __x->color;
                construct( &__y->value, __x->value );

                if ( __x->right != null() )
                {
                    __y->right = copy_node( __x->right, y );
                }
                p = y;
                __p = __y;
                x = __x->left;
                __x = touch( x );
            }

            return top;
        }

        node_type find_node( const key_type &k ) const
        {
            auto __node = touch( m_node );
            auto y = m_node;
            auto x = __node->parent;
            auto __x = touch( x );

            while ( x != null() )
            {
                if ( !m_comp(m_keyof(__x->value),k) )
                {
                    y = x;
                    x = __x->left;
                    __x = touch( x );
                }
                else
                {
                    x = __x->right;
                    __x = touch( x );
                }
            }

            if ( y != m_node )
            {
                auto __y = touch( y );
                if ( m_comp(k,m_keyof(__y->value)) )
                {
                    y = m_node;
                }
            }

            return y;
        }

        node_type lower_node( const key_type &k ) const
        {
            auto __node = touch( m_node );
            auto y = m_node;
            auto x = __node->parent;
            auto __x = touch( x );

            while ( x != null() )
            {
                if ( !m_comp(m_keyof(__x->value),k) )
                {
                    y = x;
                    x = __x->left;
                    __x = touch( x );
                }
                else
                {
                    x = __x->right;
                    __x = touch( x );
                }
            }

            return y;
        }

        node_type upper_node( const key_type &k ) const
        {
            auto __node = touch( m_node );
            auto y = m_node;
            auto x = __node->parent;
            auto __x = touch( x );

            while ( x != null() )
            {
                if ( m_comp(k,m_keyof(__x->value)) )
                {
                    y = x;
                    x = __x->left;
                    __x = touch( x );
                }
                else
                {
                    x = __x->right;
                    __x = touch( x );
                }
            }

            return y;
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
        size_type  m_size;
        node_type  m_node;
        comp_type  m_comp;
        keyof_type m_keyof;
    };

    template <typename __key,
              typename __comp,
              typename __type,
              typename __keyof,
              typename __alloc>
    typename __alloc::mem_type rb_tree<__key,__comp,__type,__keyof,__alloc>::rb_black = (typename __alloc::mem_type)0;
    template <typename __key,
              typename __comp,
              typename __type,
              typename __keyof,
              typename __alloc>
    typename __alloc::mem_type rb_tree<__key,__comp,__type,__keyof,__alloc>::rb_red = (typename __alloc::mem_type)1;
} /* namespace ray */

#endif /* #ifndef __BASE_RB_TREE_HPP_20160926111019_E2D47CBF_0B70_4873_8577_00F51066F84E__ */
