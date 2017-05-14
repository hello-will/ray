/**
 * @file    md_array.hpp
 * @version 1.0
 * @author  hello_will@yeah.net
 * data     2016/10/28 10:01:33
**/

#ifndef __BASE_MD_ARRAY_HPP_20161028100133_CC2476EE_796A_44CC_B831_2AD469F8B6FC__
#define __BASE_MD_ARRAY_HPP_20161028100133_CC2476EE_796A_44CC_B831_2AD469F8B6FC__

#include <base/utils.hpp>
#include <base/singleton.hpp>

namespace ray
{
    /* multidimensional array */
    template <typename __type,
              typename __alloc,
              const uint32_t __buf_size>
    class md_array
    {
        typedef uint32_t size_type;
        typedef __type value_type;
        typedef __alloc alloc_type;
        typedef typename alloc_type::mem_type node_type;

        enum
        {
            MAX_NODE = __buf_size/sizeof(node_type),
            MAX_DATA = __buf_size/sizeof(value_type),
        };

        static node_type alloc( size_type sz )
        {
            return singleton<alloc_type>::geti()->alloc( sz );
        }

        static node_type realloc( size_type nsz, node_type mem, size_type osz )
        {
            return singleton<alloc_type>::geti()->realloc( nsz, mem, osz );
        }

        static void dealloc( node_type mem, size_type sz )
        {
            singleton<alloc_type>::geti()->dealloc( mem, sz );
        }

        static void* touch( node_type mem )
        {
            return singleton<alloc_type>::geti()->touch( mem );
        }

        static node_type null( void )
        {
            return singleton<alloc_type>::geti()->null();
        }

        static void init_array( node_type array,
                                size_type dimension,
                                size_type first,
                                size_type last,
                                size_type size,
                                size_type &capacity )
        {
            if ( null() == array )
            {
                return ;
            }

            switch ( dimension )
            {
                case 0:
                {
                    auto temp = (value_type*)touch( array );
                    for ( auto i = first; i < last; ++i, ++capacity )
                    {
                        ::new ((void*)(temp+i)) value_type();
                    }
                }
                break;

                case 1:
                {
                    auto temp = (node_type*)touch( array );
                    for ( auto i = first; i < last; ++i )
                    {
                        if ( capacity < size )
                        {
                            temp[i] = alloc( __buf_size );
                            init_array( temp[i],
                                              dimension-1,
                                              0,
                                              MAX_DATA,
                                              size,
                                              capacity );
                        }
                        else
                        {
                            temp[i] = null();
                        }
                    }
                }
                break;

                default:
                {
                    auto temp = (node_type*)touch( array );
                    for ( auto i = first; i <last; ++i )
                    {
                        if ( capacity < size )
                        {
                            temp[i] = alloc( __buf_size );
                            init_array( temp[i],
                                              dimension-1,
                                              0,
                                              MAX_NODE,
                                              size,
                                              capacity );
                        }
                        else
                        {
                            temp[i] = null();
                        }
                    }
                }
                break;
            }
        }

        static void init_array( node_type array,
                                size_type dimension,
                                size_type first,
                                size_type last,
                                size_type size,
                                size_type &capacity,
                                const value_type &value )
        {
            if ( null() == array )
            {
                return ;
            }

            switch ( dimension )
            {
                case 0:
                {
                    auto temp = (value_type*)touch( array );
                    for ( auto i = first; i < last; ++i, ++capacity )
                    {
                        ::new ((void*)(temp+i)) value_type(value);
                    }
                }
                break;

                case 1:
                {
                    auto temp = (node_type*)touch( array );
                    for ( auto i = first; i < last; ++i )
                    {
                        if ( capacity < size )
                        {
                            temp[i] = alloc( __buf_size );
                            init_array( temp[i],
                                              dimension-1,
                                              0,
                                              MAX_DATA,
                                              size,
                                              capacity );
                        }
                        else
                        {
                            temp[i] = null();
                        }
                    }
                }
                break;

                default:
                {
                    auto temp = (node_type*)touch( array );
                    for ( auto i = first; i <last; ++i )
                    {
                        if ( capacity < size )
                        {
                            temp[i] = alloc( __buf_size );
                            init_array( temp[i],
                                              dimension-1,
                                              0,
                                              MAX_NODE,
                                              size,
                                              capacity );
                        }
                        else
                        {
                            temp[i] = null();
                        }
                    }
                }
                break;
            }
        }

        static void fill_array( node_type array,
                                size_type dimension,
                                size_type first,
                                size_type last,
                                size_type &capacity )
        {
            if ( null() == array )
            {
                return ;
            }

            switch ( dimension )
            {
                case 0:
                {
                    auto temp = (value_type*)touch( array );
                    for ( auto i = first; i < last; ++i, ++capacity )
                    {
                        ::new ((void*)(temp+i)) value_type();
                    }
                }
                break;

                case 1:
                {
                    auto temp = (node_type*)touch( array );
                    for ( auto i = first; i < last; ++i )
                    {
                        if ( null() == temp[i] )
                        {
                            temp[i] = alloc( __buf_size );
                            init_array( temp[i],
                                        dimension-1,
                                        0,
                                        MAX_DATA,
                                        (size_type)-1,
                                        capacity );
                        }
                        else
                        {
                            /* initalized */
                        }
                    }
                }
                break;

                default:
                {
                    auto temp = (node_type*)touch( array );
                    for ( auto i = first; i <last; ++i )
                    {
                        if ( null() == temp[i] )
                        {
                            temp[i] = alloc( __buf_size );
                            init_array( temp[i],
                                        dimension-1,
                                        0,
                                        MAX_NODE,
                                        (size_type)-1,
                                        capacity );
                        }
                        else
                        {
                            fill_array( temp[i],
                                        dimension-1,
                                        0,
                                        MAX_NODE,
                                        capacity );
                        }
                    }
                }
                break;
            }
        }

        static void fill_array( node_type array,
                                size_type dimension,
                                size_type first,
                                size_type last,
                                size_type &capacity,
                                const value_type &value )
        {
            if ( null() == array )
            {
                return ;
            }

            switch ( dimension )
            {
                case 0:
                {
                    auto temp = (value_type*)touch( array );
                    for ( auto i = first; i < last; ++i, ++capacity )
                    {
                        ::new ((void*)(temp+i)) value_type(value);
                    }
                }
                break;

                case 1:
                {
                    auto temp = (node_type*)touch( array );
                    for ( auto i = first; i < last; ++i )
                    {
                        if ( null() == temp[i] )
                        {
                            temp[i] = alloc( __buf_size );
                            init_array( temp[i],
                                        dimension-1,
                                        0,
                                        MAX_DATA,
                                        (size_type)-1,
                                        capacity,
                                        value );
                        }
                        else
                        {
                            /* initalized */
                        }
                    }
                }
                break;

                default:
                {
                    auto temp = (node_type*)touch( array );
                    for ( auto i = first; i <last; ++i )
                    {
                        if ( null() == temp[i] )
                        {
                            temp[i] = alloc( __buf_size );
                            init_array( temp[i],
                                        dimension-1,
                                        0,
                                        MAX_NODE,
                                        (size_type)-1,
                                        capacity,
                                        value );
                        }
                        else
                        {
                            fill_array( temp[i],
                                        dimension-1,
                                        0,
                                        MAX_NODE,
                                        capacity,
                                        value );
                        }
                    }
                }
                break;
            }
        }

        static void grow_array( node_type array,
                                size_type dimension,
                                size_type first,
                                size_type last,
                                size_type size,
                                size_type &capacity )
        {
            if ( null() == array )
            {
                return ;
            }

            switch ( dimension )
            {
                case 0:
                {
                    auto temp = (value_type*)touch( array );
                    for ( auto i = first; i < last; ++i, ++capacity )
                    {
                        ::new ((void*)(temp+i)) value_type();
                    }
                }
                break;

                case 1:
                {
                    auto temp = (node_type*)touch( array );
                    for ( auto i = first; i < last; ++i )
                    {
                        if (capacity < size)
                        {
                            if ( null() == temp[i] )
                            {
                                temp[i] = alloc( __buf_size );
                                init_array(temp[i],
                                           dimension-1,
                                           0,
                                           MAX_DATA,
                                           size,
                                           capacity );
                            }
                            else
                            {
                                /* initialized */
                            }
                        }
                    }
                }
                break;

                default:
                {
                    auto temp = (node_type*)touch( array );
                    for ( auto i = first; i <last; ++i )
                    {
                        if ( capacity < size )
                        {
                            if ( null() == temp[i] )
                            {
                                temp[i] = alloc( __buf_size );
                                init_array( temp[i],
                                            dimension-1,
                                            0,
                                            MAX_NODE,
                                            size,
                                            capacity );
                            }
                            else
                            {
                                grow_array( temp[i],
                                            dimension-1,
                                            0,
                                            MAX_NODE,
                                            size,
                                            capacity );
                            }
                        }
                    }
                }
                break;
            }
        }

        static void grow_array( node_type array,
                                size_type dimension,
                                size_type first,
                                size_type last,
                                size_type size,
                                size_type &capacity,
                                const value_type &value )
        {
            if ( null() == array )
            {
                return ;
            }

            switch ( dimension )
            {
                case 0:
                {
                    auto temp = (value_type*)touch( array );
                    for ( auto i = first; i < last; ++i, ++capacity )
                    {
                        ::new ((void*)(temp+i)) value_type(value);
                    }
                }
                break;

                case 1:
                {
                    auto temp = (node_type*)touch( array );
                    for ( auto i = first; i < last; ++i )
                    {
                        if (capacity < size)
                        {
                            if ( null() == temp[i] )
                            {
                                temp[i] = alloc( __buf_size );
                                init_array(temp[i],
                                           dimension-1,
                                           0,
                                           MAX_DATA,
                                           size,
                                           capacity,
                                           value );
                            }
                            else
                            {
                                /* initialized */
                            }
                        }
                    }
                }
                break;

                default:
                {
                    auto temp = (node_type*)touch( array );
                    for ( auto i = first; i <last; ++i )
                    {
                        if ( capacity < size )
                        {
                            if ( null() == temp[i] )
                            {
                                temp[i] = alloc( __buf_size );
                                init_array( temp[i],
                                            dimension-1,
                                            0,
                                            MAX_NODE,
                                            size,
                                            capacity,
                                            value );
                            }
                            else
                            {
                                grow_array( temp[i],
                                            dimension-1,
                                            0,
                                            MAX_NODE,
                                            size,
                                            capacity,
                                            value );
                            }
                        }
                    }
                }
                break;
            }
        }

        static node_type grow_array( node_type array,
                                     size_type odimension,
                                     size_type ndimension )
        {
            for ( auto i = odimension; i < ndimension; ++i )
            {
                auto mem = alloc( __buf_size );
                auto temp = (node_type*)touch(mem);

                temp[0] = array;
                for (auto j = 1; j < MAX_NODE; ++j)
                {
                    temp[j] = null();
                }
                array = mem;
            }

            return array;
        }

        static size_type capacity( size_type dimension )
        {
            switch( dimension )
            {
                case 0:
                {
                    return 0;
                }
                break;

                case 1:
                {
                    return MAX_DATA;
                }
                break;

                default:
                {
                    return MAX_NODE*capacity(dimension-1);
                }
                break;
            }
        }

        static value_type& get_value( node_type &array,
                                      size_type dimension,
                                      size_type pos )
        {
            switch ( dimension )
            {
                case 0:
                {
                    auto temp = (value_type*)touch( array );

                    return temp[pos];
                }
                break;

                case 1:
                {
                    auto temp = (node_type*)touch( array );
                    auto size = capacity( dimension );
                    auto idx = pos/size;

                    array = temp[idx];
                    return get_value( array,
                                      dimension-1,
                                      pos-idx*size );
                }
                break;

                default:
                {
                    auto temp = (node_type*)touch( array );
                    auto size = capacity( dimension );
                    auto idx = pos/size;

                    array = temp[idx];
                    return get_value( array,
                                      dimension-1,
                                      pos-idx*size );
                }
                break;
            }
        }

        static node_type get_array( node_type array,
                                    size_type dimension,
                                    size_type pos )
        {
            if ( null() == array )
            {
                /* nothing */
            }
            else if ( dimension )
            {
                auto temp = (node_type*)touch( array );

                array = temp[pos];
            }
            else
            {
                array = null();
            }

            return array;
        }

        static void clear_array( node_type array,
                                 size_type dimension,
                                 size_type first,
                                 size_type last,
                                 size_type &capacity )
        {
            if ( null() == array )
            {
                return ;
            }

            switch (dimension)
            {
                case 0:
                {
                    auto temp = (value_type*)touch( array );
                    for (auto i = first; i < last; ++i,--capacity )
                    {
                        temp[i].~value_type();
                    }
                }
                break;

                case 1:
                {
                    auto temp = (node_type*)touch( array );
                    for ( auto i = first; i < last; ++i )
                    {
                        clear_array( temp[i],
                                     0,
                                     MAX_DATA,
                                     dimension - 1,
                                     capacity );

                        dealloc( temp[i], __buf_size );
                        temp[i] = null();
                    }
                }
                break;

                default:
                {
                    auto temp = (node_type*)touch( array );
                    for ( auto i = first; i < last; ++i )
                    {
                        clear_array( temp[i],
                                     0,
                                     MAX_NODE,
                                     dimension - 1,
                                     capacity );

                        dealloc( temp[i], __buf_size );
                        temp[i] = null();
                    }
                }
                break;
            }
        }

        static void clear_data( node_type array,
                                size_type dimension,
                                size_type first,
                                size_type last,
                                size_type &capacity )
        {
            if ( null() == array )
            {
                return ;
            }

            switch (dimension)
            {
                case 0:
                {
                    auto temp = (value_type*)touch( array );
                    for ( auto i = first; i < last; ++i,--capacity )
                    {
                        temp[i].~value_type();
                    }
                }
                break;

                case 1:
                {
                    auto temp = (node_type*)touch( array );
                    for ( auto i = first; i < last; ++i )
                    {
                        clear_data( temp[i],
                                    dimension - 1,
                                    0,
                                    MAX_DATA,
                                    capacity );

                        dealloc( temp[i], __buf_size );
                        temp[i] = null();
                    }
                }
                break;

                default:
                {
                    auto temp = (node_type*)touch( array );
                    for ( auto i = first; i < last; ++i )
                    {
                        clear_data( temp[i],
                                    dimension - 1,
                                    0,
                                    MAX_NODE,
                                    capacity );
                    }
                }
                break;
            }
        }

        static void shrink_array( node_type array,
                                  size_type dimension,
                                  size_type first,
                                  size_type last,
                                  size_type size,
                                  size_type &capacity )
        {
            if ( null() == array )
            {
                return ;
            }

            switch ( dimension )
            {
                case 0:
                {
                    auto temp = (value_type*)touch( array );
                    if ( capacity < size )
                    {
                        for ( auto i = first; i < last; ++i, ++capacity )
                        {
                            if ( capacity >= size )
                            {
                                temp[i].~value_type();
                                ::new ((void*)(temp+i)) value_type();
                            }
                        }
                    }
                    else
                    {
                        for ( auto i = first; i < last; ++i )
                        {
                            temp[i].~value_type();
                        }
                    }
                }
                break;

                case 1:
                {
                    auto temp = (node_type*)touch( array );
                    for ( auto i = first; i < last; ++i )
                    {
                        if ( capacity < size )
                        {
                            shrink_array( temp[i],
                                          dimension-1,
                                          0,
                                          MAX_DATA,
                                          size,
                                          capacity );
                        }
                        else
                        {
                            shrink_array( temp[i],
                                          dimension-1,
                                          0,
                                          MAX_DATA,
                                          size,
                                          capacity );

                            dealloc( temp[i], __buf_size );
                            temp[i] = null();
                        }
                    }
                }
                break;

                default:
                {
                    auto temp = (node_type*)touch( array );
                    for ( auto i = first; i < last; ++i )
                    {
                        if ( capacity < size )
                        {
                            shrink_array( temp[i],
                                          dimension-1,
                                          0,
                                          MAX_NODE,
                                          size,
                                          capacity );
                        }
                        else
                        {
                            shrink_array( temp[i],
                                          dimension-1,
                                          0,
                                          MAX_NODE,
                                          size,
                                          capacity );

                            dealloc( temp[i], __buf_size );
                            temp[i] = null();
                        }
                    }
                }
                break;
            }
        }

    public:
        md_array( size_type n )
            : m_array( null() )
            , m_primary( 0 )
            , m_dimension( 0 )
            , m_size( 0 )
            , m_capacity( 0 )
        {
            initialize( n );
        }

        md_array( size_type n, const value_type &x )
            : m_array( null() )
            , m_primary( 0 )
            , m_dimension( 0 )
            , m_size( 0 )
            , m_capacity( 0 )
        {
            initialize( n, x );
        }

        ~md_array( void )
        {
            clear();
        }

        size_type size( void ) const
        {
            return m_size;
        }

        void resize( size_t n )
        {
            if ( !n )
            {
                clear();
            }
            else if ( !m_size )
            {
                initialize( n );
            }
            else
            {
                auto size = n;
                auto dimension = 0;

                /* last dimension test */
                if ( n > MAX_DATA )
                {
                    auto s = n/MAX_DATA;
                    if ( s*MAX_DATA < n )
                    {
                        n = s+1;
                    }
                    else
                    {
                        n = s;
                    }
                    ++ dimension;
                }

                /* other dimension test */
                while ( n > MAX_NODE )
                {
                    auto s = n/MAX_NODE;
                    if ( s*MAX_NODE < n )
                    {
                        n = s+1;
                    }
                    else
                    {
                        n = s;
                    }
                    ++ dimension;
                }

                auto primary = n;
                auto capacity = m_capacity;
                if ( dimension > m_dimension )
                {
                    if ( m_dimension )
                    {
                        fill_array( m_array,
                                    0,
                                    MAX_NODE,
                                    m_dimension,
                                    capacity );
                    }

                    /* grow dimension */
                    m_array = grow_array(m_array,
                                         m_dimension,
                                         dimension);

                    /* grow capacity */
                    grow_array( m_array,
                                dimension,
                                0,
                                primary,
                                size,
                                capacity );
                }
                else if ( dimension < m_dimension )
                {
                    auto pre = null();
                    auto mem = m_array;
                    auto tmp = m_dimension;
                    do
                    {
                        pre = mem;
                        mem = get_array( mem, tmp--, 0 );
                    } while (tmp > dimension);

                    auto array = (node_type*)touch( pre );
                    array[0] = null();
                    clear_array( m_array,
                                 m_dimension,
                                 0,
                                 MAX_NODE,
                                 capacity );
                    dealloc( m_array, __buf_size );

                    capacity = 0;
                    shrink_array( mem,
                                  dimension,
                                  0,
                                  dimension ? MAX_NODE : MAX_DATA,
                                  size,
                                  capacity );
                    m_array = mem;
                }
                else
                {
                    if ( size > m_capacity )
                    {
                        grow_array( m_array,
                                    m_dimension,
                                    0,
                                    primary,
                                    size,
                                    capacity );
                    }
                    else if ( m_dimension )
                    {
                        capacity = 0;
                        shrink_array( m_array,
                                      m_dimension,
                                      0,
                                      m_primary,
                                      size,
                                      capacity );
                    }
                }

                m_primary = primary;
                m_dimension = dimension;
                m_size = size;
                m_capacity = capacity;
            }
        }

        void resize( size_t n, const value_type &x )
        {
            if ( !n )
            {
                clear();
            }
            else if ( !m_size )
            {
                initialize( n, x );
            }
            else
            {
                auto size = n;
                auto dimension = 0;

                /* last dimension test */
                if ( n > MAX_DATA )
                {
                    auto s = n/MAX_DATA;
                    if ( s*MAX_DATA < n )
                    {
                        n = s+1;
                    }
                    else
                    {
                        n = s;
                    }
                    ++ dimension;
                }

                /* other dimension test */
                while ( n > MAX_NODE )
                {
                    auto s = n/MAX_NODE;
                    if ( s*MAX_NODE < n )
                    {
                        n = s+1;
                    }
                    else
                    {
                        n = s;
                    }
                    ++ dimension;
                }

                auto primary = n;
                auto capacity = m_capacity;
                if ( dimension > m_dimension )
                {
                    if ( m_dimension )
                    {
                        fill_array( m_array,
                                    0,
                                    MAX_NODE,
                                    m_dimension,
                                    capacity,
                                    x );
                    }

                    /* grow dimension */
                    m_array = grow_array(m_array,
                                         m_dimension,
                                         dimension);

                    /* grow capacity */
                    grow_array( m_array,
                                dimension,
                                0,
                                primary,
                                size,
                                capacity,
                                x );
                }
                else if ( dimension < m_dimension )
                {
                    auto pre = null();
                    auto mem = m_array;
                    auto tmp = m_dimension;
                    do
                    {
                        pre = mem;
                        mem = get_array( mem, tmp--, 0 );
                    } while (tmp > dimension);

                    auto array = (node_type*)touch( pre );
                    array[0] = null();
                    clear_array( m_array,
                                 m_dimension,
                                 0,
                                 MAX_NODE,
                                 capacity );
                    dealloc( m_array, __buf_size );

                    capacity = 0;
                    shrink_array( mem,
                                  dimension,
                                  0,
                                  dimension ? MAX_NODE : MAX_DATA,
                                  size,
                                  capacity );
                    m_array = mem;
                }
                else
                {
                    if ( size > m_capacity )
                    {
                        grow_array( m_array,
                                    m_dimension,
                                    0,
                                    primary,
                                    size,
                                    capacity,
                                    x );
                    }
                    else if ( m_dimension )
                    {
                        capacity = 0;
                        shrink_array( m_array,
                                      m_dimension,
                                      0,
                                      m_primary,
                                      size,
                                      capacity );
                    }
                }

                m_primary = primary;
                m_dimension = dimension;
                m_size = size;
                m_capacity = capacity;
            }
        }

        void clear( void )
        {
            clear_array( m_array,
                         m_dimension,
                         0,
                         m_dimension ? MAX_NODE : MAX_DATA,
                         m_capacity );
            dealloc( m_array, __buf_size );

            m_array = null();
            m_primary = 0;
            m_dimension = 0;
            m_size = 0;
            m_capacity = 0;
        }

        value_type& operator [] ( size_type n )
        {
            auto mem = m_array;

            assert( n < size() );
            return get_value( mem, m_dimension, n );
        }

    private:
        void initialize( size_type n )
        {
            if ( n > 0 )
            {
                m_size = n;

                /* last dimension test */
                if ( n > MAX_DATA )
                {
                    auto s = n/MAX_DATA;
                    if ( s*MAX_DATA < n )
                    {
                        n = s+1;
                    }
                    else
                    {
                        n = s;
                    }
                    ++ m_dimension;
                }

                /* other dimension test */
                while ( n > MAX_NODE )
                {
                    auto s = n/MAX_NODE;
                    if ( s*MAX_NODE < n )
                    {
                        n = s+1;
                    }
                    else
                    {
                        n = s;
                    }
                    ++ m_dimension;
                }

                m_primary = n;
                m_array = alloc( __buf_size );
                init_array( m_array,
                                  m_dimension,
                                  0,
                                  m_dimension ? MAX_NODE : MAX_DATA,
                                  m_size,
                                  m_capacity );
            }
        }

        void initialize( size_type n, const value_type &x )
        {
            if ( n > 0 )
            {
                m_size = n;

                /* last dimension test */
                if ( n > MAX_DATA )
                {
                    auto s = n/MAX_DATA;
                    if ( s*MAX_DATA < n )
                    {
                        n = s+1;
                    }
                    else
                    {
                        n = s;
                    }
                    ++ m_dimension;
                }

                /* other dimension test */
                while ( n > MAX_NODE )
                {
                    auto s = n/MAX_NODE;
                    if ( s*MAX_NODE < n )
                    {
                        n = s+1;
                    }
                    else
                    {
                        n = s;
                    }
                    ++ m_dimension;
                }

                m_primary = n;
                m_array = alloc( __buf_size );
                init_array( m_array,
                                  m_dimension,
                                  0,
                                  m_dimension ? MAX_NODE : MAX_DATA,
                                  m_size,
                                  m_capacity );
            }
        }

    private:
        node_type m_array;
        size_type m_primary;
        size_type m_dimension;
        size_type m_size;
        size_type m_capacity;
    };
} /* namespace ray */

#endif /* __BASE_MD_ARRAY_HPP_20161028100133_CC2476EE_796A_44CC_B831_2AD469F8B6FC__ */
