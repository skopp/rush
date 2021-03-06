//****************************************************************************/
//  File:   PSSpherePlacer.h
//  Date:   12.07.2006
//  Author: Ruslan Shestopalyuk
//****************************************************************************/
#ifndef __PSSPHEREPLACER_H__
#define __PSSPHEREPLACER_H__

//****************************************************************************/
//  Class:  PSSpherePlacer
//  Desc:    
//****************************************************************************/
class PSSpherePlacer : public PSOperator
{
    bool                m_bPlanar;      
    bool                m_bSurface;    
    float               m_Radius;

public: 
    PSSpherePlacer();
    virtual void  Update ( EmitterInstance* pInst, float dt, uint32_t mode, ParticleServer* ps );

    expose( PSSpherePlacer )
    {
        parent(PSOperator);
        field( "Planar",        m_bPlanar       );
        field( "Surface",       m_bSurface      );
        field( "Radius",        m_Radius        );
    }
}; // class PSSpherePlacer

#endif // __PSSpherePlacer_H__


