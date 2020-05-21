#ifndef SAFETYPES_CXX11_INL_
#define SAFETYPES_CXX11_INL_

#include "SafeTypes.hpp"
#include <cassert>
#include <cmath>
#include <iostream>
#include <limits>
// Reading:
// https://www.boost.org/doc/libs/1_61_0/libs/math/doc/html/math_toolkit/float_comparison.html

// For intellisense. The file will not get included twice.
//
// static_assert seems to be moved out of the conditional scope by the compiler. Therefore,
// assert(...) is used instead.
//
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
    static_assert( DenominatorForMinBound > std::numeric_limits< int >::min( ),
                   "Minimum value of denominator is std::numeric_limits<int>::min() + 1" );
    static_assert( DenominatorForMaxBound > std::numeric_limits< int >::min( ),
                   "Minimum value of denominator is std::numeric_limits<int>::min() + 1" );

    if ( ( DenominatorForMinBound < 0 ) && ( NumeratorForMinBound > 0 )
         && ( DenominatorForMaxBound < 0 ) && ( NumeratorForMaxBound > 0 ) ) {
      constexpr int newNumeratorForMinBound = -NumeratorForMinBound;
      constexpr int newDenominatorForMinBound = -DenominatorForMinBound;
      constexpr int newNumeratorForMaxBound = -NumeratorForMaxBound;
      constexpr int newDenominatorForMaxBound = -DenominatorForMaxBound;
      assert( ( ( long long )newNumeratorForMinBound * newDenominatorForMaxBound )
                  < ( ( long long )newDenominatorForMinBound * newNumeratorForMaxBound )
              && "Provided lower bound is greater than the provided upper bound. Abort" );
    } else if ( ( DenominatorForMinBound < 0 ) && ( NumeratorForMinBound > 0 )
                && ( DenominatorForMaxBound > 0 ) && ( NumeratorForMaxBound > 0 ) ) {
      constexpr int newNumeratorForMinBound = -NumeratorForMinBound;
      constexpr int newDenominatorForMinBound = -DenominatorForMinBound;
      constexpr int newNumeratorForMaxBound = NumeratorForMaxBound;
      constexpr int newDenominatorForMaxBound = DenominatorForMaxBound;
      assert( ( ( long long )newNumeratorForMinBound * newDenominatorForMaxBound )
                  < ( ( long long )newDenominatorForMinBound * newNumeratorForMaxBound )
              && "Provided lower bound is greater than the provided upper bound. Abort" );
    } else if ( ( DenominatorForMinBound > 0 ) && ( NumeratorForMinBound > 0 )
                && ( DenominatorForMaxBound < 0 ) && ( NumeratorForMaxBound > 0 ) ) {
      // if ( ( DenominatorForMinBound > 0 ) && ( DenominatorForMaxBound < 0 ) ) {
      constexpr int newNumeratorForMinBound = NumeratorForMinBound;
      constexpr int newDenominatorForMinBound = DenominatorForMinBound;
      constexpr int newNumeratorForMaxBound = -NumeratorForMaxBound;
      constexpr int newDenominatorForMaxBound = -DenominatorForMaxBound;
      assert( ( ( long long )newNumeratorForMinBound * newDenominatorForMaxBound )
                  < ( ( long long )newDenominatorForMinBound * newNumeratorForMaxBound )
              && "Provided lower bound is greater than the provided upper bound. Abort" );
    } else
    // DenominatorForMinBound > 0 && DenominatorForMaxBound > 0 )
    {
      constexpr int newNumeratorForMinBound = NumeratorForMinBound;
      constexpr int newDenominatorForMinBound = DenominatorForMinBound;
      constexpr int newNumeratorForMaxBound = NumeratorForMaxBound;
      constexpr int newDenominatorForMaxBound = DenominatorForMaxBound;
      assert( ( ( long long )newNumeratorForMinBound * newDenominatorForMaxBound )
                  < ( ( long long )newDenominatorForMinBound * newNumeratorForMaxBound )
              && "Provided lower bound is greater than the provided upper bound. Abort" );
    }

    // min and max bounds are correct.
    if ( value < _min ) {
      _value = _min;
      _errorCode = SafeTypeErrorCode::UNDERFLOW;
    } else if ( value > _max ) {
      _value = _max;
      _errorCode = SafeTypeErrorCode::OVERFLOW;
    } else {
      _value = value;
      _errorCode = SafeTypeErrorCode::NO_ERROR;
    }

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
  SafeTypeErrorCode SafeType< NumeratorForMinBound,
                              DenominatorForMinBound,
                              NumeratorForMaxBound,
                              DenominatorForMaxBound >::getErrorCode( ) const
  {
    return this->_errorCode;
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

  // template < int NumeratorForMinBound,
  //            int DenominatorForMinBound,
  //            int NumeratorForMaxBound,
  //            int DenominatorForMaxBound >
  // SafeType< NumeratorForMinBound,
  //           DenominatorForMinBound,
  //           NumeratorForMaxBound,
  //           DenominatorForMaxBound >::operator float( ) const
  // {
  //   return ( this->_value );
  // }

  template < int NumeratorForMinBound,
             int DenominatorForMinBound,
             int NumeratorForMaxBound,
             int DenominatorForMaxBound >
  SafeType< NumeratorForMinBound,
            DenominatorForMinBound,
            NumeratorForMaxBound,
            DenominatorForMaxBound >::SafeType( const SafeType &other )
  {
    this->_min = other._min;
    this->_max = other._max;
    this->_value = other._value;
    this->_errorCode = other._errorCode;
  }

  // assignment operator
  template < int NumeratorForMinBound,
             int DenominatorForMinBound,
             int NumeratorForMaxBound,
             int DenominatorForMaxBound >
  SafeType< NumeratorForMinBound,
            DenominatorForMinBound,
            NumeratorForMaxBound,
            DenominatorForMaxBound > &
  SafeType< NumeratorForMinBound,
            DenominatorForMinBound,
            NumeratorForMaxBound,
            DenominatorForMaxBound >::operator=( const SafeType &other )
  {
    this->_min = other._min;
    this->_max = other._max;
    this->_value = other._value;
    this->_errorCode = other._errorCode;
    return *this;
  }

  template < int NumeratorForMinBound,
             int DenominatorForMinBound,
             int NumeratorForMaxBound,
             int DenominatorForMaxBound >
  SafeType< NumeratorForMinBound,
            DenominatorForMinBound,
            NumeratorForMaxBound,
            DenominatorForMaxBound >
  SafeType< NumeratorForMinBound,
            DenominatorForMinBound,
            NumeratorForMaxBound,
            DenominatorForMaxBound >::operator+( const SafeType &other )
  {
    if ( ( this->_value + other._value < _max ) && ( this->_value + other._value > _min ) ) {
      this->_value += other._value;
      _errorCode = SafeTypeErrorCode::NO_ERROR;
    } else if ( this->_value + other._value > _max ) {
      this->_value = _max;
      _errorCode = SafeTypeErrorCode::OVERFLOW;
    } else if ( this->_value + other._value < _min ) {
      this->_value = _min;
      _errorCode = SafeTypeErrorCode::UNDERFLOW;
    }

    return *this;
  }

  template < int NumeratorForMinBound,
             int DenominatorForMinBound,
             int NumeratorForMaxBound,
             int DenominatorForMaxBound >
  SafeType< NumeratorForMinBound,
            DenominatorForMinBound,
            NumeratorForMaxBound,
            DenominatorForMaxBound >
  SafeType< NumeratorForMinBound,
            DenominatorForMinBound,
            NumeratorForMaxBound,
            DenominatorForMaxBound >::operator-( const SafeType &other )
  {
    // The following does not work since the new temporary is constrained by the same bounds are
    // *this, therefore, negation may floor the value before subtraction leading to incorrect
    // results. return ( this->operator+( SafeType( -1.0 * other._value ) ) );
    if ( ( this->_value - other._value > _min ) && ( this->_value - other._value < _max ) ) {
      this->_value -= other._value;
      _errorCode = SafeTypeErrorCode::NO_ERROR;
    } else if ( this->_value - other._value > _max ) {
      this->_value = _max;
      _errorCode = SafeTypeErrorCode::OVERFLOW;
    } else if ( this->_value - other._value < _min ) {
      this->_value = _min;
      _errorCode = SafeTypeErrorCode::UNDERFLOW;
    }

    return *this;
  }

  template < int NumeratorForMinBound, int NumeratorForMaxBound >
  SafeType< NumeratorForMinBound, 1, NumeratorForMaxBound, 1 >::SafeType( float value )
      : _min( NumeratorForMinBound ), _max( NumeratorForMaxBound )
  {
    // min and max bounds are correct.
    if ( value < _min ) {
      _value = _min;
      _errorCode = SafeTypeErrorCode::UNDERFLOW;
    } else if ( value > _max ) {
      _value = _max;
      _errorCode = SafeTypeErrorCode::OVERFLOW;
    } else {
      _value = value;
      _errorCode = SafeTypeErrorCode::NO_ERROR;
    }
  }  // constructor

  template < int NumeratorForMinBound, int NumeratorForMaxBound >
  float SafeType< NumeratorForMinBound, 1, NumeratorForMaxBound, 1 >::getValue( )
  {
    return this->_value;
  }  // getValue

  template < int NumeratorForMinBound, int NumeratorForMaxBound >
  SafeTypeErrorCode
  SafeType< NumeratorForMinBound, 1, NumeratorForMaxBound, 1 >::getErrorCode( ) const
  {
    return this->_errorCode;
  }  // getValue

  // template < int NumeratorForMinBound, int NumeratorForMaxBound >
  // SafeType< NumeratorForMinBound, 1, NumeratorForMaxBound, 1 >::operator float( ) const
  // {
  //   return ( this->_value );
  // }

  template < int NumeratorForMinBound, int NumeratorForMaxBound >
  SafeType< NumeratorForMinBound, 1, NumeratorForMaxBound, 1 >::SafeType( const SafeType &other )
  {
    this->_min = other._min;
    this->_max = other._max;
    this->_value = other._value;
    this->_errorCode = other._errorCode;
  }

  // assignment operator
  template < int NumeratorForMinBound, int NumeratorForMaxBound >
  SafeType< NumeratorForMinBound, 1, NumeratorForMaxBound, 1 > &
  SafeType< NumeratorForMinBound, 1, NumeratorForMaxBound, 1 >::operator=( const SafeType &other )
  {
    this->_min = other._min;
    this->_max = other._max;
    this->_value = other._value;
    this->_errorCode = other._errorCode;
    return *this;
  }

  template < int NumeratorForMinBound, int NumeratorForMaxBound >
  SafeType< NumeratorForMinBound, 1, NumeratorForMaxBound, 1 >
  SafeType< NumeratorForMinBound, 1, NumeratorForMaxBound, 1 >::operator+( const SafeType &other )
  {
    if ( ( this->_value + other._value < _max ) && ( this->_value + other._value > _min ) ) {
      this->_value += other._value;
      _errorCode = SafeTypeErrorCode::NO_ERROR;
    } else if ( this->_value + other._value > _max ) {
      this->_value = _max;
      _errorCode = SafeTypeErrorCode::OVERFLOW;
    } else if ( this->_value + other._value < _min ) {
      this->_value = _min;
      _errorCode = SafeTypeErrorCode::UNDERFLOW;
    }
    return *this;
  }

  template < int NumeratorForMinBound, int NumeratorForMaxBound >
  SafeType< NumeratorForMinBound, 1, NumeratorForMaxBound, 1 >
  SafeType< NumeratorForMinBound, 1, NumeratorForMaxBound, 1 >::operator-( const SafeType &other )
  {
    // The following does not work since the new temporary is constrained by the same bounds are
    // *this, therefore, negation may floor the value before subtraction leading to incorrect
    // results. return ( this->operator+( SafeType( -1.0 * other._value ) ) );
    if ( ( this->_value - other._value > _min ) && ( this->_value - other._value < _max ) ) {
      this->_value -= other._value;
      _errorCode = SafeTypeErrorCode::NO_ERROR;
    } else if ( this->_value - other._value > _max ) {
      this->_value = _max;
      _errorCode = SafeTypeErrorCode::OVERFLOW;
    } else if ( this->_value - other._value < _min ) {
      this->_value = _min;
      _errorCode = SafeTypeErrorCode::UNDERFLOW;
    }

    return *this;
  }
}  // namespace RomanoViolet

#endif  //. #ifndef SAFETYPES_CXX11_INL_
