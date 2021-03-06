/*****************************************************************************/
/**
 *  @file   QuadraticTetrahedralCell.h
 *  @author Takuma Kawamura, Naohisa Sakamoto
 */
/*****************************************************************************/
#ifndef KVS__QUADRATIC_HEXAHEDRAL_CELL_H_INCLUDE
#define KVS__QUADRATIC_HEXAHEDRAL_CELL_H_INCLUDE

#include <kvs/Type>
#include <kvs/Vector4>
#include <kvs/Matrix44>
#include <kvs/UnstructuredVolumeObject>
#include "CellBase.h"


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  QuadraticHexahedralCell class.
 */
/*===========================================================================*/
class QuadraticHexahedralCell : public kvs::CellBase
{
public:

    typedef kvs::CellBase BaseClass;

public:

    QuadraticHexahedralCell( const kvs::UnstructuredVolumeObject* volume );

    void updateInterpolationFunctions( const kvs::Vec3& local ) const;
    void updateDifferentialFunctions( const kvs::Vec3& local ) const;
    kvs::Real32 volume() const;
};

} // end of namespace kvs

#endif // KVS__QUADRATIC_HEXAHEDRAL_CELL_H_INCLUDE
