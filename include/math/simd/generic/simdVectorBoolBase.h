#pragma once 
#ifndef __SIMD_VECTOR_BOOL_BASE_H__
#define __SIMD_VECTOR_BOOL_BASE_H__

// big thanks to http://jmabille.github.io/blog/2014/10/10/writing-c-plus-plus-wrappers-for-simd-intrinsics-3/
// for the idea's on how to nicely wrap

template< typename TYPE >
class SimdVectorBoolBase
{
public:
   
    //downcast
    inline TYPE& operator()()
    {
        return *static_cast< TYPE* >( this );
    }
    
    inline const TYPE& operator()() const
    {
        return *static_cast< const TYPE* >( this );
    }
    
    //
    // handle all self operators here
    //
    inline TYPE& operator&= ( const TYPE& rhs )
    {
        (*this)() = (*this)() && rhs;
        return (*this)();
    } 
    
    inline TYPE& operator|= ( const TYPE& rhs )
    {
        (*this)() = (*this)() || rhs;
        return (*this)();
    }
    
    inline TYPE& operator^= ( const TYPE& rhs )
    {
        (*this)() = (*this)() ^ rhs;
        return (*this)();
    } 
    
protected:

    inline SimdVectorBoolBase(){}
    inline ~SimdVectorBoolBase(){}
    	
    inline SimdVectorBoolBase( const SimdVectorBoolBase & ){}
    inline SimdVectorBoolBase( const SimdVectorBoolBase && ){}
    inline SimdVectorBoolBase& operator= ( const SimdVectorBoolBase & ){ return *this; }
};

template< typename TYPE >
inline TYPE operator&&( const SimdVectorBoolBase< TYPE > &lhs, const SimdVectorBoolBase< TYPE > &rhs )
{
    return lhs() & rhs();
}

template< typename TYPE >
inline TYPE operator&&( const SimdVectorBoolBase< TYPE > &lhs, bool rhs )
{
    return lhs() & TYPE( rhs );
}

template< typename TYPE >
inline TYPE operator&&(  bool lhs, const SimdVectorBoolBase< TYPE > &rhs )
{
    return TYPE( lhs ) & rhs();
}

template< typename TYPE >
inline TYPE operator||( const SimdVectorBoolBase< TYPE > &lhs, const SimdVectorBoolBase< TYPE > &rhs )
{
    return lhs() | rhs();
}

template< typename TYPE >
inline TYPE operator||( const SimdVectorBoolBase< TYPE > &lhs, bool rhs )
{
    return lhs() | TYPE( rhs );
}

template< typename TYPE >
inline TYPE operator||(  bool lhs, const SimdVectorBoolBase< TYPE > &rhs )
{
    return TYPE( lhs ) | rhs();
}

template< typename TYPE >
inline TYPE operator!( const SimdVectorBoolBase< TYPE > &rhs )
{
    return rhs() == TYPE( 0 );
}

#endif