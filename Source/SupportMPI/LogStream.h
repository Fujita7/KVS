/*****************************************************************************/
/**
 *  @file   LogStream.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <kvs/NullStream>
#include "Communicator.h"


namespace kvs
{

namespace mpi
{

/*===========================================================================*/
/**
 *  @brief  Log stream class.
 */
/*===========================================================================*/
class LogStream
{
private:
    int m_rank; ///< my rank
    std::string m_filename; ///< output filename
    std::ofstream m_stream; ///< output stream
    kvs::NullStream m_null_stream; ///< null stream

public:
    LogStream( const kvs::mpi::Communicator& comm ):
        m_rank( comm.rank() )
    {
    }

    LogStream( const kvs::mpi::Communicator& comm, const std::string& filename ):
        m_rank( comm.rank() ),
        m_filename( filename ),
        m_stream( filename.c_str() )
    {
    }

    std::ostream& operator ()()
    {
        return m_filename.empty() ? std::cout : m_stream;
    }

    std::ostream& operator ()( const int target_rank )
    {
        return ( target_rank == m_rank ) ? (*this)() : m_null_stream;
    }
};

} // end of namespace mpi

} // end of namespace kvs
