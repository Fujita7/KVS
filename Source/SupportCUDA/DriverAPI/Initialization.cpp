/*****************************************************************************/
/**
 *  @file   Initialization.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include "Initialization.h"
#include "DriverAPI.h"
#include <kvs/Message>


namespace kvs
{

namespace cuda
{

namespace DriverAPI
{

/*===========================================================================*/
/**
 *  @brief  Initializes CUDA.
 *  @param  argc [in] argument count
 *  @param  argv [in] argument values
 *  @return true, if the initialization is done successfully
 */
/*===========================================================================*/
bool Initialization( int argc, char** argv )
{
    KVS_CU_CALL( cuInit( 0 ) );
    if ( kvs::cuda::DriverAPI::HasError() ) return false;

    return true;
}

} // end of namespace DriverAPI

} // end of namespace cuda

} // end of namespace kvs
