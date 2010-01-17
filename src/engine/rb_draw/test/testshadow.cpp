#include "precompile.h"
#include "mat4.h"
#include "vec4.h"
#include "vec3.h"


TEST( FlatShadow )
{
    Mat4 vptm( 2.747477f, 0.0f,	0.0f, 0.0f,
               0.0f, -0.6361259f, -0.9848089f, -0.98480785f,
	           0.0f, 3.6076496f, -0.17364839f, -0.17364819f,
	           0.0f, 0.0f, 149.90018f, 150.00002f );

    Mat4 lcViewTM(
	-0.70710683	,
	0.57735026	,
	0.40824831	,
	0.00000000	,
	0.70710683	,
	0.57735026	,
	0.40824831	,
	0.00000000	,
	0.00000000	,
	0.57735026	,
	-0.81649661	,
	0.00000000	,
	2571.2327	,
	2099.4023	,
	6484.5020	,
	1.0000000 );

    Mat4 lcProjTM(
    0.00048128571	,
	0.00018963437	,
	0.00000000	,
	-0.00018686865	,
	-0.00058944552	,
	0.00023225209	,
	0.00000000	,
	-0.00022886474	,
	0.00000000	,
	0.00000000	,
	7.2922978e-007	,
	0.00000000	,
	-9.4431552e-006	,
	-0.98541540	,
	0.00000000	,
	1.0000000 );


    Mat4 tm1(
    -0.17671488	,
	-0.98426217	,
	1.5909170e-006	,
	0.00000000	,
	0.98426217	,
	-0.17671488	,
	1.3908238e-013	,
	0.00000000	,
	0.00000000	,
	0.00000000	,
	1.0000000	,
	0.00000000	,
	0.64997220	,
	80.925201	,
	1.9073486e-006	,
	1.0000000 );


    Mat4 tm2(
	-0.030701172	,
	0.99952865	,
	1.8063814e-006	,
	0.00000000	,
	-0.99952865	,
	-0.030701172	,
	1.5791888e-013	,
	0.00000000	,
	0.00000000	,
	0.00000000	,
	1.0000000	,
	0.00000000	,
	-13.176891	,
	85.111465	,
	1.9073486e-006	,
	1.0000000 );

    Vec3 pos1 = tm1.translation();
    Vec3 pos2 = tm2.translation();

    Vec4 p1( pos1.x, pos1.y, pos1.z, 1.0f );
    Vec4 p2( pos2.x, pos2.y, pos2.z, 1.0f );

    lcViewTM.tm( p1 );
    lcProjTM.tm( p1 );
    p1 /= p1.w;

    lcViewTM.tm( p2 );
    lcProjTM.tm( p2 );
    p2 /= p2.w;

}
