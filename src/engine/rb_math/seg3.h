//****************************************************************************/
//  File:  Seg3.h
//  Desc:  3D segment
//****************************************************************************/
#ifndef __SEG_H__
#define __SEG_H__

//****************************************************************************/
//  Class:  Seg3
//  Desc:   3D segment
//****************************************************************************/
class Seg3
{
public:
    Vec3    a, b;

    inline Seg3() {}
    inline Seg3( const Vec3& va, const Vec3& vb ) : a(va), b(vb) {}

    float  dist( Vec3& v ) const;

}; // class Seg3

#endif // __SEG_H__

