/****************************************************************************/
/**
 *  @file   WriteVCProjectCUDA.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: WriteVCProjectCUDA.h 1409 2012-12-21 15:19:08Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#pragma once
#include <kvs/Compiler>
#if defined ( KVS_COMPILER_VC )
#include <string>


namespace kvsmake
{

bool WriteVCProjectCUDA( const std::string& project_name );

} // end of namespace kvsmake

#endif
