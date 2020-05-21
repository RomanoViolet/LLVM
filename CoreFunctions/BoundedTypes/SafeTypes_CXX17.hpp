#ifndef SAFETYPES_CXX17_HPP_
#define SAFETYPES_CXX17_HPP_

#include <cstdint>
namespace RomanoViolet
{
  enum class SafeTypeErrorCode : short { NO_ERROR = 0U, UNDERFLOW = 1U, OVERFLOW = 2U };
  // It would be convenient to have a custom type as a template parameter, but see
  // https://stackoverflow.com/q/15896579
  template < int NumeratorForMinBound = 1,
             int DenominatorForMinBound = 1,
             int NumeratorForMaxBound = 1,
             int DenominatorForMaxBound = 1 >
  class SafeType
  {
  public:
    SafeType( float value );
    float getMinValue( );
    float getValue( );
    SafeTypeErrorCode getErrorCode( ) const;

    // copy constructor
    SafeType( const SafeType &other );

    // assignment operator
    SafeType &operator=( const SafeType &other );

    // addition operator
    SafeType operator+( const SafeType &other );

    // subtraction operator
    SafeType operator-( const SafeType &other );

    // allows: float x = Object.
    // operator float( ) const;

  private:
    float _min;
    float _max;
    float _value;
    SafeTypeErrorCode _errorCode;

    struct NewFraction {
      int numerator = 1;
      int denominator = 1;
    };
    // the compiler treats arguments are runtime changeable, therefore not allowed inside a
    // constexpr.
    //
    // constexpr NewFraction correctBound(
    // const int Numerator,
    // const int Denominator
    //    ) const;
    //
    constexpr typename SafeType::NewFraction correctMinBound( ) const;
    constexpr typename SafeType::NewFraction correctMaxBound( ) const;
  };
  template < int NumeratorForMinBound, int NumeratorForMaxBound >
  class SafeType< NumeratorForMinBound, 1, NumeratorForMaxBound, 1 >
  {
  public:
    SafeType( float value );
    float getValue( );
    SafeTypeErrorCode getErrorCode( ) const;

    // copy constructor
    SafeType( const SafeType &other );

    // assignment operator
    SafeType &operator=( const SafeType &other );

    // addition operator
    SafeType operator+( const SafeType &other );

    // subtraction operator
    SafeType operator-( const SafeType &other );

  private:
    int _min;
    int _max;
    float _value;
    SafeTypeErrorCode _errorCode;

    // define temporary data structure to hold new numerators and denominators if these need to be
    // transformed.
    struct NewFraction {
      int numerator = 1;
      int denominator = 1;
    };
    constexpr NewFraction correctBound( int Numerator, int Denominator ) const;
  };

#define Fraction( a, b ) a, b

}  // namespace RomanoViolet

#include "SafeTypes_CXX17.inl"

#endif  // !SAFETYPES_CXX17_HPP_
