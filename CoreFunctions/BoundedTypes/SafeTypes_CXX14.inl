#ifndef SAFETYPES__CXX14_INL_
#define SAFETYPES__CXX14_INL_

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
    static_assert( DenominatorForMinBound > std::numeric_limits< int >::min( ),
                   "Minimum value of denominator is std::numeric_limits<int>::min() + 1" );
    static_assert( DenominatorForMaxBound > std::numeric_limits< int >::min( ),
                   "Minimum value of denominator is std::numeric_limits<int>::min() + 1" );

    // https://godbolt.org/z/RmA4hk
    NewFraction newMinBound = correctMinBound( );

    NewFraction newMaxBound = correctMaxBound( );
    // Although we are using runtime version of assert, the compiler is performing compile-time
    // check. See https://godbolt.org/z/RmA4hk
    assert( ( ( long long )newMinBound.numerator * newMaxBound.denominator )
                < ( ( long long )newMinBound.denominator * newMaxBound.numerator )
            && "Provided lower bound is greater than the provided upper bound. Abort" );

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
  }  // namespace RomanoViolet

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
  }  // getErrorCode

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
      this->_errorCode = SafeTypeErrorCode::NO_ERROR;
    } else if ( this->_value + other._value > _max ) {
      this->_value = _max;
      this->_errorCode = SafeTypeErrorCode::OVERFLOW;
    } else if ( this->_value + other._value < _min ) {
      this->_value = _min;
      this->_errorCode = SafeTypeErrorCode::UNDERFLOW;
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
      this->_errorCode = SafeTypeErrorCode::NO_ERROR;
    } else if ( this->_value - other._value > _max ) {
      this->_value = _max;
      this->_errorCode = SafeTypeErrorCode::OVERFLOW;
    } else if ( this->_value - other._value < _min ) {
      this->_value = _min;
      this->_errorCode = SafeTypeErrorCode::UNDERFLOW;
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
  }  // getErrorCode

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
      this->_errorCode = SafeTypeErrorCode::NO_ERROR;
    } else if ( this->_value + other._value > _max ) {
      this->_value = _max;
      this->_errorCode = SafeTypeErrorCode::OVERFLOW;
    } else if ( this->_value + other._value < _min ) {
      this->_value = _min;
      this->_errorCode = SafeTypeErrorCode::UNDERFLOW;
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
      this->_errorCode = SafeTypeErrorCode::NO_ERROR;
    } else if ( this->_value - other._value > _max ) {
      this->_value = _max;
      this->_errorCode = SafeTypeErrorCode::OVERFLOW;
    } else if ( this->_value - other._value < _min ) {
      this->_value = _min;
      this->_errorCode = SafeTypeErrorCode::UNDERFLOW;
    }

    return *this;
  }
}  // namespace RomanoViolet

#endif  //. #ifndef SAFETYPES__CXX14_INL_
