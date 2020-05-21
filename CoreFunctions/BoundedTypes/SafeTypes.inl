#ifndef SAFETYPES_INL_
#define SAFETYPES_INL_

#include "SafeTypes.hpp"
#include <cassert>
#include <cmath>
#include <limits>
// Reading:
// https://www.boost.org/doc/libs/1_61_0/libs/math/doc/html/math_toolkit/float_comparison.html

// For intellisense. The file will not get included twice.
#include "SafeTypes.hpp"

namespace RomanoViolet
{
  template < int NumeratorForMinBound,
             int DenominatorForMinBound,
             int NumeratorForMaxBound,
             int DenominatorForMaxBound >
  SafeType< NumeratorForMinBound,
            DenominatorForMinBound,
            NumeratorForMaxBound,
            DenominatorForMaxBound >::SafeType( float value )
      : _min( NumeratorForMinBound / ( DenominatorForMinBound * 1.0F ) )
      , _max( NumeratorForMaxBound / ( DenominatorForMaxBound * 1.0F ) )
  {
    // assert that denominators are not zero.
    static_assert( DenominatorForMinBound != 0, "Denominator for lower bound cannot be zero." );
    static_assert( DenominatorForMaxBound != 0, "Denominator for upper bound cannot be zero." );

#if ( __cplusplus == 201703L )
    // Negative sign from denominator is always moved to the numerator.
    // if constexpr ( DenominatorForMinBound < 0 ) {
    //   NumeratorForMinBound = NumeratorForMinBound * -1;
    //   DenominatorForMinBound = DenominatorForMinBound * -1;
    // }

    constexpr NewFraction minBound = []( ) {
      NewFraction f;
      if ( DenominatorForMinBound < 0 ) {
        f.numerator = NumeratorForMinBound * -1;
        f.denominator = DenominatorForMinBound * -1;

      } else {
        f.numerator = NumeratorForMinBound;
        f.denominator = DenominatorForMinBound;
      }
      return f;
    }( );

    constexpr NewFraction maxBound = []( ) {
      NewFraction f;
      if ( DenominatorForMaxBound < 0 ) {
        f.numerator = NumeratorForMaxBound * -1;
        f.denominator = DenominatorForMaxBound * -1;

      } else {
        f.numerator = NumeratorForMaxBound;
        f.denominator = DenominatorForMaxBound;
      }
      return f;
    }( );

    static_assert( ( ( long long )minBound.numerator * maxBound.denominator )
                       < ( ( long long )minBound.denominator * maxBound.numerator ),
                   "Provided lower bound is greater than the provided upper bound. Abort" );

#elif ( __cplusplus == 201402L )
    // https://godbolt.org/z/RmA4hk
    NewFraction newMinBound = correctMinBound( );

    NewFraction newMaxBound = correctMaxBound( );
    // Although we are using runtime version of assert, the compiler is performing compile-time
    // check. See https://godbolt.org/z/RmA4hk
    assert( ( ( long long )newMinBound.numerator * newMaxBound.denominator )
                < ( ( long long )newMinBound.denominator * newMaxBound.numerator )
            && "Provided lower bound is greater than the provided upper bound. Abort" );

#elif ( __cplusplus == 201102L )
    // constexpr lambdas are not allowed until C++17.
    if ( DenominatorForMinBound < 0 ) {
    }

    // verified that the check is done at compile time only for which long long is acceptable.
    // https://godbolt.org/z/8eJKzR, if it is still alive.
    static_assert( ( ( long long )NumeratorForMinBound * DenominatorForMaxBound )
                       < ( ( long long )DenominatorForMinBound * NumeratorForMaxBound ),
                   "Provided lower bound is greater than the provided upper bound. Abort" );
#else
    static_assert( "You need minimum C++11 standard to use this library" );
#endif

    _value = value;
  }  // end of constructor

  template < int NumeratorForMinBound,
             int DenominatorForMinBound,
             int NumeratorForMaxBound,
             int DenominatorForMaxBound >
  float SafeType< NumeratorForMinBound,
                  DenominatorForMinBound,
                  NumeratorForMaxBound,
                  DenominatorForMaxBound >::getMinValue( )
  {
    return this->_min;
  }  // getMinValue

  template < int NumeratorForMinBound,
             int DenominatorForMinBound,
             int NumeratorForMaxBound,
             int DenominatorForMaxBound >
  float SafeType< NumeratorForMinBound,
                  DenominatorForMinBound,
                  NumeratorForMaxBound,
                  DenominatorForMaxBound >::getValue( )
  {
    return this->_value;
  }  // getValue

  template < int NumeratorForMinBound, int NumeratorForMaxBound >
  SafeType< NumeratorForMinBound, 1, NumeratorForMaxBound, 1 >::SafeType( int value )
      : _min( NumeratorForMinBound ), _max( NumeratorForMaxBound )
  {
    _value = value;
  }  // constructor

  template < int NumeratorForMinBound, int NumeratorForMaxBound >
  int SafeType< NumeratorForMinBound, 1, NumeratorForMaxBound, 1 >::getValue( )
  {
    return this->_value;
  }  // getValue

#if ( __cplusplus == 201402L )
  template < int NumeratorForMinBound,
             int DenominatorForMinBound,
             int NumeratorForMaxBound,
             int DenominatorForMaxBound >
  constexpr
      typename SafeType< NumeratorForMinBound,
                         DenominatorForMinBound,
                         NumeratorForMaxBound,
                         DenominatorForMaxBound >::NewFraction SafeType< NumeratorForMinBound,
                                                                         DenominatorForMinBound,
                                                                         NumeratorForMaxBound,
                                                                         DenominatorForMaxBound >::
          correctMinBound(
              // the compiler treats arguments are runtime changeable, therefore not allowed inside
              // a constexpr. const int Numerator, const int Denominator
              ) const
  {
    // the statement below is not allowed to be inside a constexpr until C++14.
    NewFraction f;
    if ( DenominatorForMinBound < 0 ) {
      f.denominator = DenominatorForMinBound * -1;
      f.numerator = NumeratorForMinBound * -1;
    } else {
      f.denominator = DenominatorForMinBound;
      f.numerator = NumeratorForMinBound;
    }
    return f;

  }  // correctMinBound

  template < int NumeratorForMinBound,
             int DenominatorForMinBound,
             int NumeratorForMaxBound,
             int DenominatorForMaxBound >
  constexpr
      typename SafeType< NumeratorForMinBound,
                         DenominatorForMinBound,
                         NumeratorForMaxBound,
                         DenominatorForMaxBound >::NewFraction SafeType< NumeratorForMinBound,
                                                                         DenominatorForMinBound,
                                                                         NumeratorForMaxBound,
                                                                         DenominatorForMaxBound >::
          correctMaxBound(
              // the compiler treats arguments are runtime changeable, therefore not allowed inside
              // a constexpr. const int Numerator, const int Denominator
              ) const
  {
    // the statement below is not allowed to be inside a constexpr until C++14.
    NewFraction f;
    if ( DenominatorForMaxBound < 0 ) {
      f.denominator = DenominatorForMaxBound * -1;
      f.numerator = NumeratorForMaxBound * -1;
    } else {
      f.denominator = DenominatorForMaxBound;
      f.numerator = NumeratorForMaxBound;
    }
    return f;

  }  // correctMaxBound

#endif  // __cplusplus == 201402L

}  // namespace RomanoViolet

#endif  //. #ifndef SAFETYPES_INL_
