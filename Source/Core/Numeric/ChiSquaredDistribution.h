/*****************************************************************************/
/**
 *  @file   ChiSquaredDistribution.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <kvs/Type>
#include <kvs/ValueArray>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Chi-squared distribution class.
 */
/*===========================================================================*/
class ChiSquaredDistribution
{
private:
    size_t m_dof; ///< degree of freedom
    kvs::Real64 m_coef1; ///< coefficient 1
    kvs::Real64 m_coef2; ///< coefficient 2

public:
    ChiSquaredDistribution( const size_t dof );

    size_t dof() const { return m_dof; }
    kvs::Real64 pdf( const kvs::Real64 x ) const;
    kvs::Real64 cdf( const kvs::Real64 x ) const;

    template <typename T>
    kvs::ValueArray<T> pdf( const kvs::ValueArray<T>& x ) const
    {
        const size_t n = x.size();
        kvs::ValueArray<T> values( n );
        for ( size_t i = 0; i < n; i++ ) { values[i] = this->pdf( x[i] ); }
        return values;
    }

    template <typename T>
    kvs::ValueArray<T> cdf( const kvs::ValueArray<T>& x ) const
    {
        const size_t n = x.size();
        kvs::ValueArray<T> values( n );
        for ( size_t i = 0; i < n; i++ ) { values[i] = this->cdf( x[i] ); }
        return values;
    }
};

} // end of namespace kvs
