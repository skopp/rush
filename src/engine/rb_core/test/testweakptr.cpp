#include "precompile.h"
#include "jgroup.h"
#include "jweakref.h"
#include "jweakrefreg.h"

TEST( JWeakRefIsWorkingProperly )
{
    int nHolders = JWeakRefReg::s_pInstance->GetNHolders();
    JGroup* pDialog = new JGroup();
    uint8_t*    pData   = new uint8_t[71];
    {
        JWeakRef<JGroup> ptr;
        CHECK( ptr == NULL );

        ptr = pDialog;
        CHECK( ptr == pDialog );
        CHECK_EQUAL( nHolders + 1, JWeakRefReg::s_pInstance->GetNHolders() );

        JWeakRef<uint8_t> bptr = pData;
        CHECK_EQUAL( nHolders + 2, JWeakRefReg::s_pInstance->GetNHolders() );
        CHECK( bptr == pData );

        JWeakRef<JGroup> ptr1;
        ptr1 = pDialog;
        CHECK( ptr1 == pDialog );
        CHECK_EQUAL( nHolders + 2, JWeakRefReg::s_pInstance->GetNHolders() );

        CHECK( ptr1 == ptr );

        JWeakRefReg::s_pInstance->Invalidate( pDialog );
        CHECK( ptr == NULL );
        CHECK( ptr1 == NULL );
        CHECK_EQUAL( nHolders + 2, JWeakRefReg::s_pInstance->GetNHolders() );
    }
    
    CHECK_EQUAL( nHolders, JWeakRefReg::s_pInstance->GetNHolders() );
    delete pDialog;
    delete []pData;
}

TEST( JWeakRefInArray )
{
    typedef JWeakRef<JObject> JMyRef;
    std::vector<JMyRef> arr;
    static const int c_NPtrs = 7;
    arr.reserve( 2 );
    for (int i = 0; i < c_NPtrs; i++)
    {
        arr.push_back( new JObject() );
    }
    for (int i = 0; i < c_NPtrs; i++)
    {
        CHECK( arr[i] != NULL );
    }
}