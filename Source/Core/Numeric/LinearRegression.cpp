#include "LinearRegression.h"
#include <kvs/Assert>
#include <kvs/Vector>
#include <kvs/Matrix>


namespace
{

template <typename T>
T DevSQ( const kvs::Vector<T>& vec )
{
    const size_t n = vec.size();
    T sum = T(0);
    T sum2 = T(0);
    for ( size_t i = 0; i < n; i++ )
    {
        sum += vec[i];
        sum2 += vec[i] * vec[i];
    }

    // Sum of squared deviations
    return sum2 - ( sum * sum ) / n;
}

}

namespace kvs
{

template <typename T>
LinearRegression<T>::LinearRegression()
{
}

template <typename T>
LinearRegression<T>::LinearRegression( const kvs::ValueArray<T>& dep, const kvs::ValueTable<T>& indep )
{
    this->fit( dep, indep );
}

template <typename T>
void LinearRegression<T>::fit( const kvs::ValueArray<T>& dep, const kvs::ValueTable<T>& indep )
{
    KVS_ASSERT( dep.size() == indep.column(0).size() );

    const size_t nrows = dep.size();
    const size_t ncols = indep.columnSize() + 1;
    kvs::Vector<T> Y( nrows );
    kvs::Matrix<T> X( nrows, ncols );
    for ( size_t i = 0; i < nrows; i++ )
    {
        kvs::Vector<T>& Xi = X[i];
        Y[i] = dep[i];
        Xi[0] = 1.0f;
        for ( size_t j = 1; j < ncols; j++ )
        {
            Xi[j] = indep[j-1][i];
        }
    }

    const kvs::Matrix<T> Xt = X.transposed();
    const kvs::Matrix<T> XtX = Xt * X;
    const kvs::Vector<T> XtY = Xt * Y;
    const kvs::Matrix<T> XtX_inv = XtX.inverted();
    m_coef = XtX_inv * XtY;

    const kvs::Vector<T> y = X * m_coef;
    m_rss = ( Y - y ).length2();

    m_r2 = 1.0 - m_rss / ::DevSQ(Y);

    const size_t p = indep.columnSize();
    const size_t n = dep.size();
    const size_t dof = n - p - 1;
    m_adjusted_r2 = 1.0 - ( 1.0 - m_r2 ) * ( n - 1.0 ) / dof;

    const kvs::Real64 ve = m_rss / dof;
    m_t_values.setSize( m_coef.size() );
    for ( size_t i = 0; i < m_t_values.size(); i++ )
    {
        m_t_values[i] = m_coef[i] / std::sqrt( ve * XtX_inv[i][i] );
    }
}

// template instantiation
template class LinearRegression<float>;
template class LinearRegression<double>;

} // end of namespace kvs