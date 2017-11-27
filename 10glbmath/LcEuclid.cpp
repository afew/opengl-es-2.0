// Implementation of the LCX Math Library.
//
////////////////////////////////////////////////////////////////////////////////

#if defined(_PC_WINDOW_)
  #pragma warning(disable: 4996)
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "LcType.h"
#include "LcMath.h"
#include "LcEuclid.h"


//++5++++1+++++++++2+++++++++3+++++++++4+++++++++5+++++++++6+++++++++7+++++++++8


INT LCXCollisionOBB2D(  LCXVECTOR2* src1, INT stride1, INT count1
					  , LCXVECTOR2* src2, INT stride2, INT count2, INT bCCW)
{
	INT i=0, j=0;

	LCXVECTOR2	D;
	LCXVECTOR2  K;


	BYTE*		p  = (BYTE*)src1;
	BYTE*		v  = (BYTE*)src2;

	LCXVECTOR2* p0 = NULL;
	LCXVECTOR2* p1 = NULL;
	LCXVECTOR2* v0 = NULL;
	LCXVECTOR2* v1 = NULL;


	FLOAT dir = 1.0F;

	INT   det = 1;

	if(bCCW)
		dir = -1.0F;


	for(i=0; i<count1; ++i)
	{
		p0 = (LCXVECTOR2*)(p+ i * stride1);
		p1 = (LCXVECTOR2*)(p+ ((i-1 + count1)%count1) * stride1);

		D.y = +p0->x - p1->x;
		D.x = -p0->y + p1->y;
		D *= dir;

		det = 1;

		for(j=0; j<count2; ++j)
		{
			v0= (LCXVECTOR2*)(v+ j * stride2);
			K.x = v0->x - p0->x;
			K.y = v0->y - p0->y;

			if(0 >= LCXVec2Dot(&D, &K))
			{
				det =0;
				break;
			}
		}

		if(det)
			return LC_EFAIL;
	}


	for(i=0; i<count2; ++i)
	{
		v0 = (LCXVECTOR2*)(v+ i * stride2);
		v1 = (LCXVECTOR2*)(v+ ((i-1 + count2)%count2) * stride2);

		D.y = +v0->x - v1->x;
		D.x = -v0->y + v1->y;
		D *= dir;

		det = 1;

		for(j=0; j<count1; ++j)
		{
			p0= (LCXVECTOR2*)(p+ j * stride1);

			K.x = p0->x - v0->x;
			K.y = p0->y - v0->y;

			if(0 >= LCXVec2Dot(&D, &K))
			{
				det =0;
				break;
			}
		}

		if(det)
			return LC_EFAIL;
	}


	return LC_OK;
}

//p and v have 4-convex points.
INT LCXCollisionOBBRECT(LCXVECTOR2* p, LCXVECTOR2* v, INT bCCW)
{
	INT i=0, j=0;

	LCXVECTOR2	D;
	LCXVECTOR2  K;

	FLOAT dir = 1.0F;
	INT   det = 1;

	if(bCCW)
		dir = -1.0F;

	for(i=0; i<4; ++i)
	{
		D.y = +(p+ i)->x - (p+ (i + 3)%4 )->x;
		D.x = -(p+ i)->y + (p+ (i + 3)%4 )->y;
		D *= dir;

		det = 1;
		for(j=0; j<4; ++j)
		{
			K = v[j] - p[i];

			if(0 >= LCXVec2Dot(&D, &K))
			{
				det =0;
				break;
			}
		}

		if(det)
			return LC_EFAIL;


		D.y = +(v+ i)->x - (v+ (i + 3)%4 )->x;
		D.x = -(v+ i)->y + (v+ (i + 3)%4 )->y;
		D *= dir;

		det = 1;
		for(j=0; j<4; ++j)
		{
			K = p[j] - v[i];

			if(0 >= LCXVec2Dot(&D, &K))
			{
				det =0;
				break;
			}
		}

		if(det)
			return LC_EFAIL;
	}


	return LC_OK;
}



//++5++++1+++++++++2+++++++++3+++++++++4+++++++++5+++++++++6+++++++++7+++++++++8


LCX_TRI::LCX_TRI()
{
	p[0] = VEC3(0.0F, 0.0F, 0.0F);
	p[1] = VEC3(0.0F, 0.0F, 0.0F);
	p[2] = VEC3(0.0F, 0.0F, 0.0F);
}

LCX_TRI::LCX_TRI(const LCX_TRI& rhs)
{
	*this = rhs;
}

LCX_TRI::LCX_TRI(const VEC3* v)
{
	if(v)
	{
		p[0] = v[0];
		p[1] = v[1];
		p[2] = v[2];
	}
	else
	{
		p[0] = VEC3(0.0F, 0.0F, 0.0F);
		p[1] = VEC3(0.0F, 0.0F, 0.0F);
		p[2] = VEC3(0.0F, 0.0F, 0.0F);
	}
}


LCX_TRI::LCX_TRI(const FLOAT* _p)
{
	p[0].x =_p[0]; p[0].y =_p[1]; p[0].z =_p[2];
	p[1].x =_p[3]; p[1].y =_p[4]; p[1].z =_p[5];
	p[2].x =_p[6]; p[2].y =_p[7]; p[2].z =_p[8];
}

LCX_TRI::LCX_TRI(const VEC3& _0, const VEC3& _1, const VEC3& _2)
{
	p[0] = _0;
	p[1] = _1;
	p[2] = _2;
}


LCX_TRI::LCX_TRI(const VEC3* _0, const VEC3* _1, const VEC3* _2)
{
	if(_0) p[0] = _0;
	if(_1) p[1] = _1;
	if(_2) p[2] = _2;
}



LCX_TRI::LCX_TRI(FLOAT _0x,FLOAT _0y,FLOAT _0z
	, FLOAT _1x,FLOAT _1y,FLOAT _1z
	, FLOAT _2x,FLOAT _2y,FLOAT _2z)
{
	p[0].x =_0x; p[0].y =_0y; p[0].z =_0z;
	p[1].x =_1x; p[1].y =_1y; p[1].z =_1z;
	p[2].x =_2x; p[2].y =_2y; p[2].z =_2z;
}


// casting
LCX_TRI::operator FLOAT*()
{
	return (FLOAT *) &p[0].x;
}

LCX_TRI::operator const FLOAT*() const
{
	return (const FLOAT *) &p[0].x;
}


const LCX_TRI& LCX_TRI::operator=(const LCX_TRI& rhs)
{
	//memcpy(p, &rhs.p, sizeof(p));
	p[0] = rhs.p[0];
	p[1] = rhs.p[1];
	p[2] = rhs.p[2];

	return *this;
}


INT LCX_TRI::operator==(const LCX_TRI& v) const
{
	return p[0] == v.p[0]
		&& p[1] == v.p[1]
		&& p[2] == v.p[2]
		;
}


INT LCX_TRI::operator!=(const LCX_TRI& v) const
{
	return p[0] != v.p[0]
		|| p[1] != v.p[1]
		|| p[2] != v.p[2]
		;
}




//++5++++1+++++++++2+++++++++3+++++++++4+++++++++5+++++++++6+++++++++7+++++++++8

LCX_PCK::LCX_PCK()
{
	pck = VEC3(0.0F, 0.0F, 0.0F);
	dist= LCX_FMAX;
	memset(idx, 0xFF, sizeof(idx));
	memset(xtr, 0xFF, sizeof(xtr));
}


LCX_PCK::LCX_PCK( const LCX_PCK& rhs)
{
	*this = rhs;
}


LCX_PCK::LCX_PCK(VEC3* _pck, FLOAT _dist)
{
	pck = VEC3(0.0F, 0.0F, 0.0F);
	memset(idx, 0xFF, sizeof(idx));
	memset(xtr, 0xFF, sizeof(xtr));

	dist= _dist;
}


LCX_PCK::LCX_PCK(   VEC3* _pck, FLOAT _dist
				, VEC3* _tri0, VEC3* _tri1, VEC3* _tri2
				, INT* _id, INT* _extra
				)
{
	pck = VEC3(0.0F, 0.0F, 0.0F);
	memset(idx, 0xFF, sizeof(idx));
	memset(xtr, 0xFF, sizeof(xtr));

	dist= _dist;
	tri = LCX_TRI(_tri0, _tri1, _tri2);

	if(_pck  ) pck = *_pck;
	if(_id   ) memcpy(idx, _id, sizeof(idx));
	if(_extra) memcpy(xtr, _extra, sizeof(xtr));
}


LCX_PCK::LCX_PCK(   VEC3* _pck, FLOAT _dist
				, VEC3* _tri0, VEC3* _tri1, VEC3* _tri2
				, INT _t0, INT _t1, INT _t2
				, INT _ext0, INT _ext1, INT _ext2, INT _ext3
				)
{
	pck = VEC3(0.0F, 0.0F, 0.0F);
	memset(idx, 0xFF, sizeof(idx));
	memset(xtr, 0xFF, sizeof(xtr));

	if(_pck  ) pck = *_pck;

	dist= _dist;
	tri = LCX_TRI(_tri0, _tri1, _tri2);

	idx.x = _t0;
	idx.y = _t1;
	idx.z = _t2;

	xtr.x = _ext0;
	xtr.y = _ext1;
	xtr.z = _ext2;
	xtr.w = _ext3;
}


const LCX_PCK& LCX_PCK::operator=(const LCX_PCK& v)
{
	pck  = v.pck;
	dist = v.dist;
	memcpy(&tri, &v.tri, sizeof(tri));
	memcpy(&idx, &v.idx, sizeof(idx));
	memcpy(&xtr, &v.xtr, sizeof(xtr));
	return *this;
}


INT LCX_PCK::operator==(const LCX_PCK& v) const
{
	return pck  == v.pck
		&& dist == v.dist
		&& tri  == v.tri
		&& idx  == v.idx
		&& xtr  == v.xtr
		;
}


INT LCX_PCK::operator!=(const LCX_PCK& v) const
{
	return pck  != v.pck
		|| dist != v.dist
		|| tri  != v.tri
		|| idx  != v.idx
		|| xtr  != v.xtr
		;
}


FLOAT 	LCX_PCK::BoundRadius(){ return dist;	}
VEC3	LCX_PCK::BoundCenter(){ return pck;	}
void  	LCX_PCK::Invalid()
{
	pck = VEC3(0.0F, 0.0F, 0.0F);
	dist= LCX_FMAX;
	memset(idx, 0xFF, sizeof(idx));
	memset(xtr, 0xFF, sizeof(xtr));
}




//++5++++1+++++++++2+++++++++3+++++++++4+++++++++5+++++++++6+++++++++7+++++++++8

//              y
//  z          /
//	|   6------------7(Max)
//	|  /|    /      /|
//	| / |   /      / |
//	|/  |  /      /  |
//	4------------5   |
//	|   |/       |   |
//	|   2--------|---3
//	|  /         |  /
//	| /          | /
//	|/           |/
//	0(Min)-------1--------- x


WORD LCX_CUBOID::idxLine[12][2] =
{
	{0,1},  {1,3},  {3,2},
	{1,5},  {5,7},  {7,3},
	{5,4},  {4,6},  {6,7},
	{4,0},  {0,2},  {2,6},
};


WORD LCX_CUBOID::idxTri1[ 6][3] =
{
	{0, 1, 4},
	{1, 3, 5},
	{3, 2, 7},
	{2, 0, 6},
	{4, 5, 6},
	{0, 2, 1},
};


WORD LCX_CUBOID::idxTri2[12][3] =
{
	{0, 1, 4},	{5, 4, 1},
	{1, 3, 5},	{7, 5, 3},
	{3, 2, 7},	{6, 7, 2},
	{2, 0, 6},	{4, 6, 0},
	{4, 5, 6},	{7, 6, 5},
	{0, 2, 1},	{3, 1, 2},
};


LCX_CUBOID::LCX_CUBOID()
{
	memset(edg, 0, sizeof(edg));
	memset(dim, 0, sizeof(dim));
	col = 0xFFFFFFFF;
}

LCX_CUBOID::LCX_CUBOID(const LCX_CUBOID& v)
{
	memcpy(edg, v.edg, sizeof(edg));
	memcpy(dim, v.dim, sizeof(dim));
	col = v.col;
}

LCX_CUBOID::LCX_CUBOID(FLOAT x, FLOAT y, FLOAT z)
{
	Dimension(x, y, z);
}

LCX_CUBOID::LCX_CUBOID(FLOAT x, FLOAT y, FLOAT z, const VEC3* X, const VEC3* Y, const VEC3* Z)
{
	Dimension(x, y, z, X, Y, Z);
}


void LCX_CUBOID::Dimension(FLOAT _x, FLOAT _y, FLOAT _z)
{
	dim.x	= _x;
	dim.y	= _y;
	dim.z	= _z;

	if(0.0F == dim.x) dim.x = 1.0F;
	if(0.0F == dim.y) dim.y = 1.0F;
	if(0.0F == dim.z) dim.z = 1.0F;

	edg[0] = VEC3( -_x, -_y, -_z);	//edg[0] = VEC3( -1.0F, -1.0F, -1.0F);
	edg[1] = VEC3( +_x, -_y, -_z);	//edg[1] = VEC3( +1.0F, -1.0F, -1.0F);
	edg[2] = VEC3( -_x, +_y, -_z);	//edg[2] = VEC3( -1.0F, +1.0F, -1.0F);
	edg[3] = VEC3( +_x, +_y, -_z);	//edg[3] = VEC3( +1.0F, +1.0F, -1.0F);
	edg[4] = VEC3( -_x, -_y, +_z);	//edg[4] = VEC3( -1.0F, -1.0F, +1.0F);
	edg[5] = VEC3( +_x, -_y, +_z);	//edg[5] = VEC3( +1.0F, -1.0F, +1.0F);
	edg[6] = VEC3( -_x, +_y, +_z);	//edg[6] = VEC3( -1.0F, +1.0F, +1.0F);
	edg[7] = VEC3( +_x, +_y, +_z);	//edg[7] = VEC3( +1.0F, +1.0F, +1.0F);

	edg[0] *= 0.5F;
	edg[1] *= 0.5F;
	edg[2] *= 0.5F;
	edg[3] *= 0.5F;
	edg[4] *= 0.5F;
	edg[5] *= 0.5F;
	edg[6] *= 0.5F;
	edg[7] *= 0.5F;
}


void LCX_CUBOID::Dimension(FLOAT _x, FLOAT _y, FLOAT _z, const VEC3* _X, const VEC3* _Y, const VEC3* _Z)
{
	dim.x	= _x;
	dim.y	= _y;
	dim.z	= _z;

	if(0.0F == dim.x) dim.x = 1.0F;
	if(0.0F == dim.y) dim.y = 1.0F;
	if(0.0F == dim.z) dim.z = 1.0F;

	edg[0] = *_X *(-_x)  +  *_Y*(-_y)  +  *_Z*(-_z);	edg[0] *= 0.5F;
	edg[1] = *_X *(+_x)  +  *_Y*(-_y)  +  *_Z*(-_z);	edg[1] *= 0.5F;
	edg[2] = *_X *(-_x)  +  *_Y*(+_y)  +  *_Z*(-_z);	edg[2] *= 0.5F;
	edg[3] = *_X *(+_x)  +  *_Y*(+_y)  +  *_Z*(-_z);	edg[3] *= 0.5F;
	edg[4] = *_X *(-_x)  +  *_Y*(-_y)  +  *_Z*(+_z);	edg[4] *= 0.5F;
	edg[5] = *_X *(+_x)  +  *_Y*(-_y)  +  *_Z*(+_z);	edg[5] *= 0.5F;
	edg[6] = *_X *(-_x)  +  *_Y*(+_y)  +  *_Z*(+_z);	edg[6] *= 0.5F;
	edg[7] = *_X *(+_x)  +  *_Y*(+_y)  +  *_Z*(+_z);	edg[7] *= 0.5F;
}


void LCX_CUBOID::MinMax(const VEC3* v_min, const VEC3* v_max)
{
	LCXVECTOR3	center2 = *v_max + *v_min;
	FLOAT _x = v_max->x - v_min->x;
	FLOAT _y = v_max->y - v_min->y;
	FLOAT _z = v_max->z - v_min->z;

	dim.x	= _x;
	dim.y	= _y;
	dim.z	= _z;

	edg[0] = VEC3( -_x, -_y, -_z);	//edg[0] = VEC3( -1.0F, -1.0F, -1.0F);
	edg[1] = VEC3( +_x, -_y, -_z);	//edg[1] = VEC3( +1.0F, -1.0F, -1.0F);
	edg[2] = VEC3( -_x, +_y, -_z);	//edg[2] = VEC3( -1.0F, +1.0F, -1.0F);
	edg[3] = VEC3( +_x, +_y, -_z);	//edg[3] = VEC3( +1.0F, +1.0F, -1.0F);
	edg[4] = VEC3( -_x, -_y, +_z);	//edg[4] = VEC3( -1.0F, -1.0F, +1.0F);
	edg[5] = VEC3( +_x, -_y, +_z);	//edg[5] = VEC3( +1.0F, -1.0F, +1.0F);
	edg[6] = VEC3( -_x, +_y, +_z);	//edg[6] = VEC3( -1.0F, +1.0F, +1.0F);
	edg[7] = VEC3( +_x, +_y, +_z);	//edg[7] = VEC3( +1.0F, +1.0F, +1.0F);

	edg[0] += center2;
	edg[1] += center2;
	edg[2] += center2;
	edg[3] += center2;
	edg[4] += center2;
	edg[5] += center2;
	edg[6] += center2;
	edg[7] += center2;

	edg[0] *= 0.5F;
	edg[1] *= 0.5F;
	edg[2] *= 0.5F;
	edg[3] *= 0.5F;
	edg[4] *= 0.5F;
	edg[5] *= 0.5F;
	edg[6] *= 0.5F;
	edg[7] *= 0.5F;
}


void LCX_CUBOID::Position(const VEC3* pos)
{
	LCXVECTOR3 center = (edg[7] + edg[0]) * 0.5F;

	for(INT n=0; n<8; ++n)
	{
		edg[n] -= center;
		edg[n] += *pos;
	}

}


LCX_CUBOID::operator FLOAT*()
{
	return (FLOAT *) &edg[0].x;
}

LCX_CUBOID::operator const FLOAT*() const
{
	return (const FLOAT *) &edg[0].x;
}


INT LCX_CUBOID::operator==(const LCX_CUBOID& v) const
{
	return	edg[0] == v.edg[0] &&
			edg[1] == v.edg[1] &&
			edg[2] == v.edg[2] &&
			edg[3] == v.edg[3] &&
			edg[4] == v.edg[4] &&
			edg[5] == v.edg[5] &&
			edg[6] == v.edg[6] &&
			edg[7] == v.edg[7];
}

INT LCX_CUBOID::operator !=(const LCX_CUBOID& v) const
{
	return	edg[0] != v.edg[0] ||
			edg[1] != v.edg[1] ||
			edg[2] != v.edg[2] ||
			edg[3] != v.edg[3] ||
			edg[4] != v.edg[4] ||
			edg[5] != v.edg[5] ||
			edg[6] != v.edg[6] ||
			edg[7] != v.edg[7];
}

void LCX_CUBOID::operator =(const LCX_CUBOID& v)
{
	memcpy(edg, v.edg, sizeof(edg));
	memcpy(dim, v.dim, sizeof(dim));
	col = v.col;
}




//++5++++1+++++++++2+++++++++3+++++++++4+++++++++5+++++++++6+++++++++7+++++++++8
//

LCX_AABC::LCX_AABC()
{
	vcO = VEC3(0.0F, 0.0F, 0.0F);
	vcL = VEC3(0.0F, 0.0F, 0.0F);
}

LCX_AABC::LCX_AABC(const VEC3& _min, const VEC3& _max)
{
	vcO = (_max + _min) * 0.5F;
}

VEC3 LCX_AABC::Extent() const
{
	return vcL * 2.0F;
}

void LCX_AABC::Edge(VEC3* pvcEdge)
{
	FLOAT _x = vcL.x * 2.0F;
	FLOAT _y = vcL.y * 2.0F;
	FLOAT _z = vcL.z * 2.0F;

	VEC3	vMin = vcO - vcL;
	VEC3	vMax = vcO + vcL;

	pvcEdge[0] = vMin;
	pvcEdge[1] = vMin;			pvcEdge[1].x +=_x;
	pvcEdge[2] = vMin;			pvcEdge[2].y +=_y;
	pvcEdge[3] = vMax;			pvcEdge[3].z -=_z;
	pvcEdge[4] = vMin;			pvcEdge[4].z +=_z;
	pvcEdge[5] = pvcEdge[1];	pvcEdge[5].x +=_x;
	pvcEdge[6] = pvcEdge[2];	pvcEdge[6].z +=_z;
	pvcEdge[7] = vMax;
}

void LCX_AABC::Line(VEC3* pvcLine)
{
	VEC3 vcEdge[8];

	this->Edge(vcEdge);

	for(INT n=0; n<12; ++n)
	{
		INT a = LCX_CUBOID::idxLine[n][0];
		INT b = LCX_CUBOID::idxLine[n][1];

		pvcLine[ 2*n+0] = vcEdge[a];
		pvcLine[ 2*n+1] = vcEdge[b];
	}
}




//++5++++1+++++++++2+++++++++3+++++++++4+++++++++5+++++++++6+++++++++7+++++++++8
//


LCX_AABB::LCX_AABB()
{
	vMin = VEC3( LCX_FMAX, LCX_FMAX, LCX_FMAX);
	vMax = VEC3(-LCX_FMAX,-LCX_FMAX,-LCX_FMAX);
}

LCX_AABB::LCX_AABB(const LCX_AABB& v)
{
	vMin = v.vMin;
	vMax = v.vMax;
}

LCX_AABB::LCX_AABB(const VEC3* v)
{
	if(v)
	{
		vMin = v[0];
		vMax = v[1];
	}
	else
	{
		vMin = VEC3( LCX_FMAX, LCX_FMAX, LCX_FMAX);
		vMax = VEC3(-LCX_FMAX,-LCX_FMAX,-LCX_FMAX);
	}
}

LCX_AABB::LCX_AABB(const VEC3& _min, const VEC3& _max)
{
	vMin = _min;
	vMax = _max;
}

LCX_AABB::LCX_AABB(const VEC3& v)
{
	vMin = v;
	vMax = v;
}


LCX_AABB::LCX_AABB(FLOAT* _p)
{
	vMin.x =_p[0]; vMin.y =_p[1]; vMin.z =_p[2];
	vMax.x =_p[3]; vMax.y =_p[4]; vMax.z =_p[5];
}


LCX_AABB::LCX_AABB(FLOAT x0, FLOAT y0, FLOAT z0, FLOAT x1, FLOAT y1, FLOAT z1)
{
	vMin = VEC3(x0, y0, z0);
	vMax = VEC3(x1, y1, z1);
}


INT LCX_AABB::operator==(const LCX_AABB& v) const { return vMin == v.vMin && v.vMax == vMax;}
INT LCX_AABB::operator!=(const LCX_AABB& v) const { return vMin != v.vMin || v.vMax != vMax;}



void LCX_AABB::AddPoint(const VEC3* pV)
{
	AddPoint(pV->x, pV->y, pV->z);
}


void LCX_AABB::AddPoint(FLOAT x, FLOAT y, FLOAT z)
{
	if (x>vMax.x) vMax.x = x;
	if (y>vMax.y) vMax.y = y;
	if (z>vMax.z) vMax.z = z;

	if (x<vMin.x) vMin.x = x;
	if (y<vMin.y) vMin.y = y;
	if (z<vMin.z) vMin.z = z;
}


VEC3 LCX_AABB::Extent() const {	return vMax - vMin;			}


void LCX_AABB::Edge(VEC3* pvcEdge)
{
	FLOAT _x = vMax.x - vMin.x;
	FLOAT _y = vMax.y - vMin.y;
	FLOAT _z = vMax.z - vMin.z;

	pvcEdge[0] = vMin;
	pvcEdge[1] = vMin;			pvcEdge[1].x +=_x;
	pvcEdge[2] = vMin;			pvcEdge[2].y +=_y;
	pvcEdge[3] = vMax;			pvcEdge[3].z -=_z;
	pvcEdge[4] = vMin;			pvcEdge[4].z +=_z;
	pvcEdge[5] = pvcEdge[1];	pvcEdge[5].x +=_x;
	pvcEdge[6] = pvcEdge[2];	pvcEdge[6].z +=_z;
	pvcEdge[7] = vMax;
}


void LCX_AABB::Line(VEC3* pvcLine)
{
	VEC3 vcEdge[8];

	this->Edge(vcEdge);


	for(INT n=0; n<12; ++n)
	{
		INT a = LCX_CUBOID::idxLine[n][0];
		INT b = LCX_CUBOID::idxLine[n][1];

		pvcLine[ 2*n+0] = vcEdge[a];
		pvcLine[ 2*n+1] = vcEdge[b];
	}
}


VEC3 LCX_AABB::Center() const {	return (vMin + vMax) / 2.0F; }


INT LCX_AABB::IsInside(const VEC3* pV)
{
	return (pV->x >= vMin.x && pV->x <= vMax.x &&
			pV->y >= vMin.y && pV->y <= vMax.y &&
			pV->z >= vMin.z && pV->z <= vMax.z);
}


void LCX_AABB::Repair()
{
	FLOAT t;

	if (vMin.x > vMax.x)
	{
		t=vMin.x; vMin.x = vMax.x; vMax.x=t;
	}

	if (vMin.y > vMax.y)
	{
		t=vMin.y; vMin.y = vMax.y; vMax.y=t;
	}

	if (vMin.z > vMax.z)
	{
		t=vMin.z; vMin.z = vMax.z; vMax.z=t;
	}
}




//++5++++1+++++++++2+++++++++3+++++++++4+++++++++5+++++++++6+++++++++7+++++++++8


LCX_ABX::LCX_ABX()
{
	o	= NULL;
}

LCX_ABX::LCX_ABX(const VEC3* _Min, const VEC3* _Max, LC_HANDLE _pOwn)
{
	o	= NULL;
	Set(_Min, _Max, _pOwn);
}

void LCX_ABX::Set(const VEC3* _Min, const VEC3* _Max)
{
	VEC3 n = *_Min;
	VEC3 m = *_Max;
	p = m + n;
	p *= 0.5f;

	c.dim.x = m.x - n.x;
	c.dim.y = m.y - n.y;
	c.dim.z = m.z - n.z;
	r = (FLOAT)sqrt( c.dim.x * c.dim.x + c.dim.y * c.dim.y + c.dim.z * c.dim.z) * 0.5F;

	c.edg[0] = n;
	c.edg[1] = n + VEC3(c.dim.x,       0,       0);
	c.edg[2] = n + VEC3(      0, c.dim.y,       0);
	c.edg[3] = n + VEC3(c.dim.x, c.dim.y,       0);
	c.edg[4] = n + VEC3(      0,       0, c.dim.z);
	c.edg[5] = n + VEC3(c.dim.x,       0, c.dim.z);
	c.edg[6] = n + VEC3(      0, c.dim.y, c.dim.z);
	c.edg[7] = m;
}


void LCX_ABX::Set(const VEC3* _Min, const VEC3* _Max, LC_HANDLE _pOwn)
{
	Set(_Min, _Max);
	if(_pOwn)
		o = _pOwn;
}


void      LCX_ABX::Owner    (LC_HANDLE v){ o = v;                    }
LC_HANDLE LCX_ABX::Owner    ()           { return o;                 }
void      LCX_ABX::Color    (COLOR32   v){ c.col = v;                }
COLOR32   LCX_ABX::Color    ()           { return	c.col;           }
VEC3*     LCX_ABX::Min      () const     { return (VEC3*)&c.edg[0].x;}
VEC3*     LCX_ABX::Max      () const     { return (VEC3*)&c.edg[7].x;}
VEC3*     LCX_ABX::Center   () const     { return (VEC3*)&p.x;       }
VEC3*     LCX_ABX::Demension() const     { return (VEC3*)&c.dim.x;   }
FLOAT     LCX_ABX::Radius   ()           { return r;                 }


LCX_ABXPCK::LCX_ABXPCK()
: pbb(0), dist(LCX_FMAX){}

LCX_ABXPCK::LCX_ABXPCK(const VEC3& vcP, LCX_ABX* _pBnd, FLOAT _fD)
: pck(vcP), pbb(_pBnd), dist(_fD){}





//++5++++1+++++++++2+++++++++3+++++++++4+++++++++5+++++++++6+++++++++7+++++++++8


TLC_IDX_POS::TLC_IDX_POS()
{
	idx = LCXVEC3i(-1, -1, -1);
	pos = LCXVECTOR3(0, 0, 0);
}

TLC_IDX_POS::TLC_IDX_POS(INT X, INT Y, INT Z, const LCXVECTOR3& P)
{
	idx = LCXVEC3i(X, Y, Z);
	pos = P;
}




//++5++++1+++++++++2+++++++++3+++++++++4+++++++++5+++++++++6+++++++++7+++++++++8
// 2D RECT info


TLCX_RECT2D::TLCX_RECT2D()
: r(0,0,0,0), s(1,1), p(0,0)
{
}

TLCX_RECT2D::TLCX_RECT2D(const TLCX_RECT2D& v)
: r(v.r), s(v.s), p(v.p)
{
}


TLCX_RECT2D::TLCX_RECT2D(const LCXRECT& rct)
: r(rct), s(1,1), p(0,0)
{
}

TLCX_RECT2D::TLCX_RECT2D(const LCXRECT& rct, const LCXVECTOR2& scl, const LCXVECTOR2& pos)
: r(rct), s(scl), p(pos)
{
}




//++5++++1+++++++++2+++++++++3+++++++++4+++++++++5+++++++++6+++++++++7+++++++++8
// Triangle


INT	LcMath_TriUV(FLOAT* fU,	FLOAT* fV, const VEC3* p, const VEC3* p0, const VEC3* p1, const VEC3* p2)
{
	VEC3	vcA = *p1 - *p0;
	VEC3	vcB = *p2 - *p0;
	VEC3	vcP = *p  - *p0;

	FLOAT fPA = LCXVec3Dot( &vcA, &vcP);
	FLOAT fPB = LCXVec3Dot( &vcB, &vcP);
	FLOAT fAB = LCXVec3Dot( &vcA, &vcB);
	FLOAT fAA = LCXVec3Dot( &vcA, &vcA);
	FLOAT fBB = LCXVec3Dot( &vcB, &vcB);

	FLOAT fD = fAA*fBB - fAB*fAB;

	if(fabsf(fD)<0.00001f)
		return -1;

	*fU = (fPA*fBB - fPB*fAB)/fD;
	*fV = (fPB*fAA - fPA*fAB)/fD;

	return 0;
}


void LcMath_TriTranslate(LCX_TRI* Out,const VEC3* pV,const LCX_TRI* In)
{
	LCX_TRI t;
	memcpy(&t, In, sizeof(LCX_TRI));

	Out->p[0] = *pV + t.p[0];
	Out->p[1] = *pV + t.p[1];
	Out->p[2] = *pV + t.p[2];
}


void LcMath_TriTransform(LCX_TRI* Out,const MAT4* pT,const LCX_TRI* In)
{
	LCX_TRI t;
	memcpy(&t, In, sizeof(LCX_TRI));

	LCXVec3TransformCoord(&Out->p[0], &t.p[0], pT);
	LCXVec3TransformCoord(&Out->p[1], &t.p[1], pT);
	LCXVec3TransformCoord(&Out->p[2], &t.p[2], pT);
}




//++5++++1+++++++++2+++++++++3+++++++++4+++++++++5+++++++++6+++++++++7+++++++++8
// Cuboid

void LcMath_CuboidTranslate(LCX_CUBOID* Out,const VEC3* pV,const LCX_CUBOID* pCube)
{
	for(INT i=0; i<8; ++i)
		Out->edg[0]	= *pV + pCube->edg[0];
}


void LcMath_CuboidTransform(LCX_CUBOID* Out,const MAT4* pT, const LCX_CUBOID* pCube)
{
	for(INT i=0; i<8; ++i)
		LCXVec3TransformCoord(&Out->edg[i], &pCube->edg[i], pT);
}


void LcMath_CuboidLineList(LC_HANDLE Out, INT nChunk,const LCX_CUBOID* pCube)
{
	for(INT n=0; n<12; ++n)
	{
		INT a = LCX_CUBOID::idxLine[n][0];
		INT b = LCX_CUBOID::idxLine[n][1];

		*( (VEC3*) ((char*)(Out) +  (2*n+0) * nChunk)  ) = pCube->edg[a];
		*( (VEC3*) ((char*)(Out) +  (2*n+1) * nChunk)  ) = pCube->edg[b];
	}
}


void LcMath_CuboidABTranslate(LCX_AABB* Out,const VEC3* pV,const LCX_AABB* In)
{
	LCX_AABB t;
	memcpy(&t, In, sizeof(LCX_AABB));

	Out->vMin = *pV + t.vMin;
	Out->vMax = *pV + t.vMax;
}



void LcMath_CuboidABTransform(LCX_AABB*Out,const MAT4* pT,const LCX_AABB* In)
{
	LCX_AABB t;
	memcpy(&t, In, sizeof(LCX_AABB));

	LCXVec3TransformCoord(&Out->vMin, &t.vMin, pT);
	LCXVec3TransformCoord(&Out->vMax, &t.vMax, pT);
	Out->Repair();
}


INT	LcMath_CuboidToLine(  VEC3* Out
						, FLOAT* fDist
						, const VEC3*	pRayPos								// Ray Position
						, const VEC3*	pRayDir								// Ray Direction
						, const VEC3*	pEdge								// Edge point has LCX_CUBOID index
						)
{
	VEC3	pRet;
	VEC3	pRayP   = *pRayPos;
	VEC3	pRayD   = *pRayDir;

	VEC3	vcBndCent= (pEdge[0] + pEdge[7]) * 0.5F;					// bound shere center of edges
	VEC3	vcT		 = vcBndCent - pRayP;								// bound shere center - ray position

	FLOAT fRadius2 = LCXVec3DistanceSq(&vcBndCent, &pEdge[0]);		// maximum radius
	FLOAT fDot2	= LCXVec3Dot(&pRayD, &vcT);
	FLOAT fL2		= 0.0F;


	// 디렉션과 반대이면 빠져나감
	if(fDot2<0.0F)
		return -1;


	fL2		 =LCXVec3LengthSq(&vcT);
	fDot2	*=fDot2;

	// 충돌 바운드 구와 비교
	if( fL2 >(fDot2 + fRadius2))		//fL2^2 = (fDot)^2 + radius^2
		return -1;


	VEC3*	p0;
	VEC3*	p1;
	VEC3*	p2;

	VEC3	C;		// center
	VEC3	A;		// a for u
	VEC3	B;		// b for v
	FLOAT U;
	FLOAT V;
	VEC3*	edg = (VEC3*)pEdge;

	// 6면 충돌
	for(INT i=0; i<6; ++i)
	{
		p0 = &edg[ LCX_CUBOID::idxTri1[i][0] ];
		p1 = &edg[ LCX_CUBOID::idxTri1[i][1] ];
		p2 = &edg[ LCX_CUBOID::idxTri1[i][2] ];

		C =     (*p1 + *p2) * 0.5F;
		A = C + (*p1 - *p0) * 0.5F;
		B = C + (*p2 - *p0) * 0.5F;

		LcMath_LineToTri(&pRet, &C, &A, &B, &pRayP, &pRayD, &U, &V, NULL);

		if( fabsf(U) <1 && fabsf(V) <1)
		{
			if(Out)
				*Out = pRet;

			if(fDist)
				*fDist =(FLOAT)sqrt( LCXVec3DistanceSq(&pRet, &pRayP) );

			return LC_OK;
		}
	}


	return LC_EFAIL;
}


INT   LcMath_CuboidABCToLine(LCX_AABC* pabb, const VEC3* pRayPos, const VEC3* pRayDir)
{
	INT hr = 0;
	INT i  = 0;

	VEC3	vcA[3];

	VEC3*	A = NULL;
	VEC3*	B = NULL;
	VEC3*	N = NULL;

	VEC3		L0 = *pRayPos;
	VEC3		Ld = *pRayDir;
	VEC3		Obj=  pabb->vcO;
	VEC3		K	;			// K = Lc - N;

	VEC3		P;
	FLOAT 		ldN;			// Ld dot N
	FLOAT 		ldA;			// Ld dot A
	FLOAT 		ldB;			// Ld dot B
	FLOAT 		ka, kb, kn;		//
	FLOAT 		a2, b2;			// length square of A, B
	FLOAT 		u, v;			//

	vcA[0] = VEC3(pabb->vcL.x,        0.0F,        0.0F);
	vcA[1] = VEC3(       0.0F, pabb->vcL.y,        0.0F);
	vcA[2] = VEC3(       0.0F,        0.0F, pabb->vcL.z);


	for(i=0; i<3; ++i)
	{
		A = &vcA[(i+0)%3];
		B = &vcA[(i+1)%3];
		N = &vcA[(i+2)%3];

		ldN = LCXVec3Dot(N, &Ld);
		if(0 == ldN)
			continue;


		ldA = LCXVec3Dot(A, &Ld);
		ldB = LCXVec3Dot(B, &Ld);

		a2 = LCXVec3LengthSq(A);
		b2 = LCXVec3LengthSq(B);


		// u+, v+
		K = Obj - L0 + *N;

		//u = - K dot A + (K dot N)/ldN * ldA;
		ka = LCXVec3Dot(&K, A);
		kb = LCXVec3Dot(&K, B);
		kn = LCXVec3Dot(&K, N);

		u = -ka + kn * ldA/ldN;
		u = fabsf(u)/a2;

		v = -kb + kn * ldB/ldN;
		v = fabsf(v)/b2;

		if(1.0F >u && 1.0F >v)
			return LC_OK;


		// u+, v+
		K = Obj - L0 - *N;

		//u = - K dot A + (K dot N)/ldN * ldA;
		ka = LCXVec3Dot(&K, A);
		kb = LCXVec3Dot(&K, B);
		kn = LCXVec3Dot(&K, N);

		u = -ka + kn * ldA/ldN;
		u = fabsf(u)/a2;

		v = -kb + kn * ldB/ldN;
		v = fabsf(v)/b2;

		if(1.0F >u && 1.0F >v)
			return LC_OK;
	}


	return LC_EFAIL;
}



INT LcMath_CuboidOBCToLine(LCX_OBBC* pObb			// Input Obb
							, const VEC3* pRayPos		// Input Line Position
							, const VEC3* pRayDir		// Input Line Direction
							)
{
	INT hr = 0;
	INT i  = 0;

	VEC3*	A = NULL;
	VEC3*	B = NULL;
	VEC3*	N = NULL;

	VEC3		L0 = *pRayPos;
	VEC3		Ld = *pRayDir;
	VEC3		Obj=  pObb->vcO;
	//VEC3		Lc = Obj - L0;		// Obj Center - Line Position
	VEC3		K	;				// K = Lc - N;

	VEC3		P;
	float			ldN;			// Ld dot N
	float			ldA;			// Ld dot A
	float			ldB;			// Ld dot B
	float			ka, kb, kn;		//
	float			a2, b2;			// length square of A, B
	float			u, v;			//

	for(i=0; i<3; ++i)
	{
		A = &pObb->vcA[(i+0)%3];
		B = &pObb->vcA[(i+1)%3];
		N = &pObb->vcA[(i+2)%3];

		ldN = LCXVec3Dot(N, &Ld);
		if(0 == ldN)
			continue;


		ldA = LCXVec3Dot(A, &Ld);
		ldB = LCXVec3Dot(B, &Ld);

		a2 = LCXVec3LengthSq(A);
		b2 = LCXVec3LengthSq(B);


		// u+, v+
		K = Obj - L0 + *N;

		//u = - K dot A + (K dot N)/ldN * ldA;
		ka = LCXVec3Dot(&K, A);
		kb = LCXVec3Dot(&K, B);
		kn = LCXVec3Dot(&K, N);

		u = -ka + kn * ldA/ldN;
		u = fabsf(u)/a2;

		v = -kb + kn * ldB/ldN;
		v = fabsf(v)/b2;

		if(1.0F >u && 1.0F >v)
			return LC_OK;


		// u+, v+
		K = Obj - L0 - *N;

		//u = - K dot A + (K dot N)/ldN * ldA;
		ka = LCXVec3Dot(&K, A);
		kb = LCXVec3Dot(&K, B);
		kn = LCXVec3Dot(&K, N);

		u = -ka + kn * ldA/ldN;
		u = fabsf(u)/a2;

		v = -kb + kn * ldB/ldN;
		v = fabsf(v)/b2;

		if(1.0F >u && 1.0F >v)
			return LC_OK;
	}


	return LC_EFAIL;
}




//++5++++1+++++++++2+++++++++3+++++++++4+++++++++5+++++++++6+++++++++7+++++++++8


//static INT XXX_LcMath_LineCross2D(LCXVECTOR2 * p)
//{
//	LCXVECTOR2 L1 = p[1] - p[0];
//	LCXVECTOR2 L2 = p[3] - p[2];
//	LCXVECTOR2 L3 = p[2] - p[0];
//
//
//	FLOAT fAlpha = L2.x * L1.y - L2.y * L1.x;
//
//	if(0.0F == fAlpha)
//		return -1;
//
//	FLOAT fBeta = fAlpha;
//
//	fAlpha = (L2.x * L3.y - L2.y * L3.x)/fAlpha;
//
//	if( (0.0F > fAlpha) || (fAlpha > 1.0F) )
//		return -1;
//
//	fBeta = (L2.x * L3.y - L1.y * L3.x)/fBeta;
//
//	if( (0.0F > fAlpha) || (fAlpha > 1.0F) )
//		return -1;
//
//	return 0;
//}


INT LcMath_LineCross2D(LCXVECTOR2* Out, const LCXVECTOR2* p)
{
	INT hr = LcMath_LineCross2D(Out, &p[0], &p[1], &p[2], &p[3]);
	return hr;
}


INT   LcMath_LineCross2D( LCXVECTOR2* Out
						, const LCXVECTOR2* p0
						, const LCXVECTOR2* p1
						, const LCXVECTOR2* p2
						, const LCXVECTOR2* p3
						)
{
	LCXVECTOR2 L1 = *p1 - *p0;
	LCXVECTOR2 L2 = *p2 - *p0;
	LCXVECTOR2 L3 = *p3 - *p0;

	LCXVECTOR2 L4 = *p3 - *p2;
	LCXVECTOR2 L5 = *p1 - *p2;
	LCXVECTOR2 L6 = -L2;

	LCXVECTOR2 pos = *p0;
	FLOAT       t1 = 0;


	FLOAT c0 = 0.0F;
	FLOAT c1 = 0.0F;

	c0 = LCXVec2Cross(&L2, &L1);
	c1 = LCXVec2Cross(&L1, &L3);
	if(0 >= c0 * c1)
		return LC_EFAIL;


	c0 = LCXVec2Cross(&L5, &L4);
	c1 = LCXVec2Cross(&L4, &L6);
	if(0 >= c0 * c1)
		return LC_EFAIL;

	c0 = LCXVec2Cross(&L2, &L4);
	c1 = LCXVec2Cross(&L1, &L4);

	c0 = LCXVec2Cross(&L2, &L4);
	c1 = LCXVec2Cross(&L1, &L4);

	if(0.0F == c1)
		return LC_EFAIL;


	if(Out)
	{
		t1   = c0/c1;
		pos += t1* L1;
		*Out = pos;
	}

	return LC_OK;
}



INT LcMath_Point3DtoScreen(	VEC3* Out				// Output
						  ,	const VEC3* vcIn		// In put
						  , const VEC3* vcCam		// Camera pos
						  ,	const VEC3* vcZ		// Camera Z vector
						  , const MAT4 * mtVP		// View matrix * Projection Matrix
						  , FLOAT fScnW					// Screen Width
						  , FLOAT fScnH					// Screen Height
				  )
{
	FLOAT fNear=1.0F;
	FLOAT beta;
	VEC3	vcB;
	VEC3	vcT	= (*vcIn - *vcCam);

	beta	= LCXVec3Dot(vcZ, &vcT);

	if(beta<=fNear)
		return -1;			// 프로젝션 평면 뒤에 있음.

	beta = fNear/beta;
	vcB = beta * vcT;
	vcB -= fNear * (*vcZ);

	Out->x	= vcB.x * mtVP->_11 + vcB.y * mtVP->_21 + vcB.z * mtVP->_31;			// 다음 Model view Matrix와 연산한다.
	Out->y	= vcB.x * mtVP->_12 + vcB.y * mtVP->_22 + vcB.z * mtVP->_32;
	Out->z	= vcB.x * mtVP->_13 + vcB.y * mtVP->_23 + vcB.z * mtVP->_33;

	Out->x	=  fScnW * (Out->x +1.0F) * 0.5f;
	Out->y	= -fScnH * (Out->y -1.0F) * 0.5f;

	return 0;
}





INT LcMath_MousePositionTo3D(  VEC3* vcOut
							 , MAT4 matView
							 , MAT4 matProj
							 , FLOAT fScnW
							 , FLOAT fScnH
							 , LC_POINT2 ptMouse)
{
	MAT4 matViewProj;
	MAT4 matViewInv;

	MAT4 matViewProjInv;

	LCXMatrixIdentity(&matViewProj);

	matViewProj = matView * matProj;
	LCXMatrixInverse(&matViewProjInv, NULL, &matViewProj);


	VEC3 vecBefore( 2.0F * ptMouse.x/fScnW - 1,  -2.0F * ptMouse.y/fScnH + 1,	0);

	VEC3 vecP(0,0,0);


	// 다음 Model view Matrix와 연산한다.

	vecP.x = vecBefore.x * matViewProjInv._11 + vecBefore.y * matViewProjInv._21 + vecBefore.z * matViewProjInv._31;
	vecP.y = vecBefore.x * matViewProjInv._12 + vecBefore.y * matViewProjInv._22 + vecBefore.z * matViewProjInv._32;
	vecP.z = vecBefore.x * matViewProjInv._13 + vecBefore.y * matViewProjInv._23 + vecBefore.z * matViewProjInv._33;

	//Ray Position
	LCXMatrixInverse(&matViewInv, NULL, &matView);
	VEC3 vecCamPos(matViewInv._41, matViewInv._42, matViewInv._43);

	//Normalvector
	VEC3 vecZ( matView._13, matView._23, matView._33);

	FLOAT beta =(vecZ.y + vecP.y);

	if (0.0F == beta)
		return -1;

	beta =  -vecCamPos.y/beta;

	*vcOut = beta * (vecZ + vecP) + vecCamPos;

	return 0;
}




FLOAT LcMath_PointToPlaneDistance(LCXPLANE* pPn, VEC3* p1)
{
	return LCXPlaneDotCoord(pPn, p1);
}


FLOAT LcMath_LineToPointDistance(const LCXLINE* pL, const VEC3* pP)
{
	VEC3 L;
	VEC3 N;
	L = *pP - pL->p;
	LCXVec3Cross(&N, &L, &pL->t);

	return LCXVec3Length(&N);
}



INT	LcMath_LineToSphere(VEC3* Out,FLOAT* fDst, const LCXLINE* pLine,const VEC3* pC,const FLOAT fR)
{
	VEC3	vcP;
	VEC3	vcT;
	FLOAT fT;
	FLOAT fN;

	vcT = *pC - pLine->p;
	fT = LCXVec3Dot(&pLine->t, &vcT);

	vcP = pLine->p + fT * pLine->t;

	fN = LCXVec3LengthSq(&vcT);
	fN -= fT * fT;

	if(fDst)
		*fDst = sqrtf(fN);

	if(Out)
		*Out = vcP;

	if(fN < fR*fR)
		return 0;

	return -1;
}





//FLOAT LcMath_LineToLineDistance(const LCXLINE* pL1, const LCXLINE* pL2)
//{
//	VEC3	n;
//	LCXVec3Cross(&n, &pL1->t, &pL2->t);
//	LCXVec3Normalize(&n,&n);
//
//	return fabs(LCXVec3Dot(&(pL2->p - pL1->p), &n));
//}



// 두직선 사이의 최단 거리 구하기
FLOAT LcMath_LineToLineDistance(																			// 직선과 직선 사이의 최단 거리
									FLOAT* pSinTheta														// 직선과 직선 사이의 각도
								,	VEC3* Out																// 직선과 직선사이의 노말벡터
								,	const LCXLINE* pV1														// 직선 1
								,	const LCXLINE* pV2														// 직선 2
								)
{
	VEC3	vcN;
	VEC3	vcL1 = pV1->t;
	VEC3	vcL2 = pV2->t;
	VEC3	vcP1P2=pV1->p - pV2->p;

	FLOAT fD = 0.0F;
	FLOAT fL = 0.0F;

	LCXVec3Cross(&vcN, &vcL1, &vcL2);

	fL = LCXVec3Length(&vcN);

	if(pSinTheta)
		*pSinTheta = fL;

	fL = 1.0F/fL;

	vcN.x *= fL;
	vcN.y *= fL;
	vcN.z *= fL;

	fD = LCXVec3Dot(&vcN, &vcP1P2);

	*Out = vcN;

	return fD;
}




// 두 직선 사이의 상호 작용
INT LcMath_LineToLineIntersection(FLOAT* pSinTheta, FLOAT* pDst,VEC3* Out,const LCXLINE* pV1,const LCXLINE* pV2)
{
	VEC3	vcN;
	VEC3	vcL1 = pV1->t;
	VEC3	vcL2 = pV2->t;
	VEC3	vcP1 = pV1->p;
	VEC3	vcP2 = pV2->p;

	VEC3	vcP2P1 = vcP2 - vcP1;

	FLOAT fD      = 0.0F;
	FLOAT fL1L2   = LCXVec3Dot(&vcL1, &vcL2);
	FLOAT fP2P1L1 = LCXVec3Dot(&vcP2P1, &vcL1);
	FLOAT fP2P1L2 = LCXVec3Dot(&vcP2P1, &vcL2);

	FLOAT fDet1 = -1.0F + fL1L2 * fL1L2;
	FLOAT fDet2 = fP2P1L2 - fL1L2 * fP2P1L1;
	FLOAT fBeta = 0.0F;

	// 평행이다.
	if( fabsf(fL1L2)>0.9999F)
	{
		VEC3 vcT;

		LCXVec3Cross(&vcT, &vcL1, &vcP2P1);
		fD = LCXVec3LengthSq(&vcT);

		if(pDst)
			*pDst = fD;

		// 겹쳐 있다면
		// 부정( 정할 수 없다.)
		if(fD<0.0001f)
			return -1;

		// 겹쳐 있지 않다면
		// 불능(구할 수 없다.)
		return -2;
	}


	fD=LcMath_LineToLineDistance(pSinTheta, &vcN, pV1, pV2);

	if(pDst)
		*pDst = fD;


	// 충돌
	if(fabsf(fD)<0.0001f)
	{
		fBeta = fDet2/ fDet1;
		Out[0] = vcP2 + fBeta * vcL2;
		Out[1] = Out[0];

		// 충돌한 점의 갯수를 돌려준다.
		return 1;
	}


	fBeta = fDet2/ fDet1;
	Out[1] = vcP2 + fBeta * vcL2;
	Out[0] = fD * vcN + Out[1];

	// 충돌한 점의 갯수를 돌려준다.
	return 2;
}




// 무한 직선과 무한 평면사이..
// p = p1 + k * (p2 - p1)
// n . p + d = 0
// n . p1 + k *( p2 - p1) . p + d = 0
//
// k = - (d + n . p1)/( ( p2 - p1) . p )
//

FLOAT LcMath_LineToPlaneKValue(const LCXPLANE* p1, const VEC3* p2, const VEC3* p3)
{
	VEC3 vcT = *p3 - *p2;
	FLOAT k = LCXPlaneDotNormal(p1, &vcT);
	return -(LCXPlaneDotCoord(p1, p2) / k);
}



INT LcMath_LineToPlaneIntersect(LCXVECTOR3* Out, const LCXLINE* line, const LCXPLANE* plane)
{
	FLOAT k1 = 0.0F;
	FLOAT k2 = LCXVec3Dot(&line->t, (LCXVECTOR3*)&plane->a);
	FLOAT k  = 0.0F;

	if(0.000001F >fabsf(k2))
		return LC_EFAIL;

	k1 = LCXPlaneDotCoord(plane, &line->p);

	k = -k1/k2;

	*Out = line->p + k * line->t;

	return LC_OK;
}


INT LcMath_LineToPlaneIntersect(LCXVECTOR3* Out, const LCXLINE* line, const LCXVECTOR3* plane_pos0, const LCXVECTOR3* plane_normal)
{
	FLOAT k1 = 0.0F;
	FLOAT k2 = LCXVec3Dot(&line->t, plane_normal);
	FLOAT k  = 0.0F;

	if(0.000001F >fabsf(k2))
		return LC_EFAIL;

	LCXVECTOR3 dst = *plane_pos0 - line->p;

	k1 = LCXVec3Dot(&dst, plane_normal);
	k  =  k1/k2;

	*Out = line->p + k * line->t;

	return LC_OK;
}





INT	LcMath_LineToTri( VEC3* Out
					, FLOAT* fDist
					, VEC3* pRayPos
					, VEC3* pRayDir
					, VEC3* pTri
					, INT bUseCull
					)
{
	VEC3	pRayP   = *pRayPos;
	VEC3	pRayD   = *pRayDir;
	VEC3	pRet;
	INT   hr = 0;

	if(bUseCull)
	{
		VEC3 vcN;
		VEC3 a = pTri[1]-pTri[0];
		VEC3 b = pTri[2]-pTri[0];
		LCXVec3Cross(&vcN, &a, &b);

		if( LCXVec3Dot(&vcN, &pRayD)>0)
			return -1;
	}

	hr = LcMath_LineToTri(&pRet, &pTri[0], &pTri[1], &pTri[2], &pRayP, &pRayD);
	if(LC_FAILED(hr))
		return hr;


	if(fDist)
		*fDist =(FLOAT)sqrt( LCXVec3DistanceSq(&pRet, &pRayP) );

	if(Out)
		*Out = pRet;

	return hr;
}


INT	LcMath_LineToTri(	VEC3* Out					// Pick Position
					,	FLOAT* fDist				// Distance from pRayPos to Out
					,	const VEC3* pRayPos			// Starting Ray Position
					,	const VEC3* pRayDir			// Starting Ray Position
					,	const VEC3* p0				// Triangle 0
					,	const VEC3* p1				// Triangle 1
					,	const VEC3* p2				// Triangle 2
					,	INT bUseCull
					)
{
	VEC3	pRayP   = *pRayPos;
	VEC3	pRayD   = *pRayDir;

	VEC3	vcA		= *p1-*p0;
	VEC3	vcB		= *p2-*p0;
	VEC3	pRet;

	FLOAT 	tU, tV, tD;
	INT   	hr = 0;

	if(bUseCull)
	{
		VEC3 vcN;
		LCXVec3Cross(&vcN, &vcA, &vcB);

		if( LCXVec3Dot(&vcN, &pRayD)>0)
			return -1;
	}

	hr = LcMath_LineToTri(&pRet, p0, p1, p2, &pRayP, &pRayD, &tU, &tV, &tD);
	if(LC_FAILED(hr))
		return hr;


	if(fDist)
		*fDist =(FLOAT)sqrt( LCXVec3DistanceSq(&pRet, &pRayP) );

	if(Out)
		*Out = pRet;

	return hr;
}



INT LcMath_LineToTri(	VEC3*			Out
					,	const VEC3*		pTri
					,	const LCXLINE*	pLine
					,	INT 			bRectangle
					,	INT 			positiveDir
					,	const VEC3* 	pBoundLine
					,	const VEC3* 	pTriNormal
					)
{
	VEC3	vcN;
	FLOAT 	fD;

	VEC3 vcA = pTri[1] - pTri[0];
	VEC3 vcB = pTri[2] - pTri[0];

	if(NULL == pTriNormal)
	{
		LCXVec3Cross(&vcN, &vcA, &vcB);
		LCXVec3Normalize(&vcN,&vcN);
	}
	else
	{
		vcN = *pTriNormal;
	}

	fD = -LCXVec3Dot(&vcN, &pTri[0]);

	if(pBoundLine)																// 직선이 유한일 경우
	{
		LCXPLANE	plane = LCXPLANE(vcN.x, vcN.y, vcN.z, fD);
		FLOAT fD1 = LCXPlaneDotCoord(&plane, &pBoundLine[0]);
		FLOAT fD2 = LCXPlaneDotCoord(&plane, &pBoundLine[1]);

		if(fD1 * fD2>0)															// Not Collision
			return -1;
	}

	VEC3	vcC;
	VEC3	vcD;

	FLOAT 	fT;
	FLOAT 	fAC;
	FLOAT 	fBC;
	FLOAT 	fAB;

	FLOAT 	fA;
	FLOAT 	fB;
	FLOAT 	fU;
	FLOAT 	fV;

	VEC3 vcL = pLine->t;

	fT = LCXVec3Dot(&vcL, &vcN);
	fT = -( fD+ LCXVec3Dot(&vcN, &pLine->p))/fT;

	if(positiveDir && fT<=0)
		return -1;

	vcC = pLine->p + fT * vcL;
	vcC -= pTri[0];

	fA = LCXVec3Length(&vcA);
	fB = LCXVec3Length(&vcB);

	fAC = LCXVec3Dot(&vcA, &vcC);
	fBC = LCXVec3Dot(&vcB, &vcC);
	fAB = LCXVec3Dot(&vcA, &vcB);

	fD = fA * fA * fB * fB - fAB*fAB+0.000001f;
	fU = (fAC * fB*  fB - fAB * fBC)/fD;
	fV = (fA * fA * fBC - fAB * fAC)/fD;

	vcD = fU * vcA + fV * vcB;
	vcD += pTri[0];

	if(Out)
		*Out = vcD;

	if(bRectangle)
	{
		if(fU>=-0.0001f && fU<=1.0001f && fV>=-0.0001f && fV<=1.0001f)
			return 0;
	}
	else
	{
		if(fU>=-0.0001f && fU<=1.0001f && fV>=-0.0001f && fV<=1.0001f && (fU+fV)<1.0001f)
			return 0;
	}

	return -1;
}


INT LcMath_LineToTri(      VEC3* Out		// 충돌 위치
					, const VEC3* V0		// 삼각형 꼭지점
					, const VEC3* V1		// 삼각형 꼭지점
					, const VEC3* V2		// 삼각형 꼭지점
					, const VEC3* L0		// 직선의 시작 위치
					, const VEC3* L			// 직선의 방향 벡터
					, FLOAT* pU				// Barycentric Hit Coordinates
					, FLOAT* pV				// Barycentric Hit Coordinates
					, FLOAT* pW)			// 충돌점까지 ratio
{
	VEC3		A = *V1 - *V0;
	VEC3		B = *V2 - *V0;
	VEC3		D = *L;
	VEC3		T = *L0 - *V0;

	VEC3		N;							// N = A X B
	VEC3		M;							// M = L X T
	VEC3		P;							// collision position

	FLOAT 	k, u, v, l;


	LCXVec3Cross(&N, &A, &B);
	LCXVec3Cross(&M, &D, &T);
	l = LCXVec3Dot(&D, &N);

	if(fabsf(l) < 0.00001f)
		return -1;

	u =  LCXVec3Dot(&B, &M);
	v = -LCXVec3Dot(&A, &M);
	k = -LCXVec3Dot(&T, &N);

	u /= l;
	v /= l;
	k /= l;

	if(pU)	*pU = u;
	if(pV)	*pV = v;
	if(pW)	*pW = k;

	if(Out)
	{
		P  = u * A + v * B;
		P += *V0;
		*Out = P;
	}

	if( 0.0F > u  || u > 1.0F ||
		0.0F > v  || v > 1.0F ||
		1.0F < u+v )
		return -1;


	return 0;
}




//++5++++1+++++++++2+++++++++3+++++++++4+++++++++5+++++++++6+++++++++7+++++++++8


INT LcMath_TriangleToTriangle(VEC3* Out, VEC3* pV1, VEC3* pV2)
{
	//삼각형과 삼각형의 충돌은 최소1점이상 이어야...
	// 직선과 삼각형 충돌을 응용한다.

	INT   iPoint=0;
	VEC3*	pTri1 = NULL;
	VEC3*	pTri2 = NULL;

	VEC3	pLine[2];
	VEC3	vcPick;
	INT   i=0;


	pTri1 = pV1;
	pTri2 = pV2;

	for(i=0; i<3; ++i)
	{
		pLine[0] = pTri2[(i+0)%3];
		pLine[1] = pTri2[(i+1)%3];

		VEC3	vcLineT = pLine[1] - pLine[0];
		LCXVec3Normalize(&vcLineT, &vcLineT);

		LCXLINE Line(pLine[0], vcLineT);

		if(LC_SUCCEEDED(LcMath_LineToTri(&vcPick, pTri1, &Line, LC_FALSE, LC_FALSE, pLine)))
		{
			if(Out)
				*(Out + iPoint) = vcPick;

			++iPoint;
		}
	}

	if(2==iPoint)
		return iPoint;

	pTri1 = pV2;
	pTri2 = pV1;

	for(i=0; i<3; ++i)
	{
		pLine[0] = pTri2[(i+0)%3];
		pLine[1] = pTri2[(i+1)%3];

		VEC3	vcLineT = pLine[1] - pLine[0];
		LCXVec3Normalize(&vcLineT, &vcLineT);

		LCXLINE Line(pLine[0], vcLineT);

		if(LC_SUCCEEDED(LcMath_LineToTri(&vcPick, pTri1, &Line, LC_FALSE, LC_FALSE, pLine)))
		{
			if(Out)
				*(Out + iPoint) = vcPick;

			++iPoint;
		}
	}

	if(iPoint<1)
		return -1;

	return iPoint;
}


// 삼각형의 정교한 외접원
FLOAT LcMath_CircumcircleCorrect(VEC3* Out,const VEC3* pTri)
{
	VEC3	p1, p2, p3;
	VEC3	t1, t2, t3;
	VEC3	 n, n1, n2;
	VEC3	q1, q2, q3;
	VEC3	s1;

	FLOAT a1=0.0F;
	FLOAT fR=0.0F;
	VEC3	vcT;

	FLOAT fD=0.0F;

	p1 = pTri[0];
	p2 = pTri[1];
	p3 = pTri[2];

	t1 = (p2 - p1) * .5f;
	t2 = (p3 - p2) * .5f;
	t3 = (p1 - p3) * .5f;

	LCXVec3Cross(&n, &t1, &t2);
	n *=-1.0F;
	LCXVec3Normalize(&n, &n);

	LCXVec3Cross(&n1, &n, &t1);
	LCXVec3Cross(&n2, &n, &t2);

	q1 = p1 + t1;
	q2 = p2 + t2;
	q3 = p3 + t3;

	s1 = q2 - q1;

	FLOAT fN1 = LCXVec3LengthSq(&n1);
	FLOAT fN2 = LCXVec3LengthSq(&n2);

	FLOAT fN12 = LCXVec3Dot(&n1, &n2);

	fD	= fN12 * fN12 - fN1 * fN2;



	FLOAT fN1S1 = LCXVec3Dot(&n1, &s1);
	FLOAT fN2S1 = LCXVec3Dot(&n2, &s1);

	a1 = fN12 * fN2S1 - fN2 * fN1S1;
	a1 /= fD;

	vcT		= t1 + a1 * n1;
	*Out	= p1 + vcT;

	fR = LCXVec3Length(&vcT);

	// 반경을 리턴
	return fR;
}



// 삼각형의 비정교한 외접원
// 무게중심을 가운데 두고, 이 점에서 각꼭지점 사이의 거리가 가장 큰 값을 반경으로 결정.
FLOAT LcMath_TriangleBoundSphere(VEC3* Out,const VEC3* pTri)
{
	FLOAT fR=0.0F,	fR2=0.0F, fR3=0.0F;
	VEC3	q1, q2, q3;

	*Out =pTri[0];
	*Out +=pTri[1];
	*Out +=pTri[2];

	*Out /= 3.0F;

	q1 = *Out;
	q2 = *Out;
	q3 = *Out;

	q1 -= pTri[0];
	q2 -= pTri[1];
	q3 -= pTri[2];

	fR = LCXVec3Length(&q1);
	fR2 = LCXVec3Length(&q2);
	fR3 = LCXVec3Length(&q3);

	if(fR2>fR)
		fR= fR2;

	if(fR3>fR)
		fR= fR3;

	// 반경을 리턴
	return fR;
}





// 직선과 평면과의 충돌점
INT LcMath_PlaneToLine(VEC3* Out, const LCXPLANE* pPn, const VEC3* p1, const VEC3* vcT)
{
	FLOAT k = LCXPlaneDotNormal(pPn, vcT);

	if (k == 0)
		return -1;

	k =1.0F/k;
	k *= (- LCXPlaneDotCoord(pPn, p1) );

	if(Out)
		*Out = *p1 + k * (*vcT);

	return 0;
}





// 2점사이의 평면
INT LcMath_PlaneToLimitedLine(VEC3* Out, const LCXPLANE* pPn, const VEC3* p1, const VEC3* p2)
{
	FLOAT d1 = LCXPlaneDotCoord(pPn, p1);
	FLOAT d2 = LCXPlaneDotCoord(pPn, p2);

	d1 *=  d2;

	if(Out)
	{
		VEC3 vcT = *p2 - *p1;
		FLOAT k = LCXPlaneDotNormal(pPn, &vcT);
		k =1.0F/k;
		*Out = *p1 + k * vcT;
	}

	return d1<0.0F? 1: -1;
}




FLOAT LcMath_DistancePlane2Plane(const LCXPLANE* pP1, const LCXPLANE* pP2)	// 평행한 평면사이의 최단거리
{
//	FLOAT fP1;
//	FLOAT fP2;
//
//	fP1 = LCXVec3Length(&pP1->n);
//	fP1 = pP1->d/fP1;
//
//	fP2 = LCXVec3Length(&pP2->n);
//	fP2 = pP2->d/fP2;
//
//	return fabs(fP1 - fP2);

	return fabs(pP1->d - pP2->d);
}



//	LCXINLINE FLOAT LCXPlaneDotCoord( const LCXPLANE *pP, const VEC3 *pV)
//	{
//	#ifdef LCX_DEBUG
//		if(!pP || !pV)
//			return 0.0F;
//	#endif
//
//		return pP->a * pV->x + pP->b * pV->y + pP->c * pV->z + pP->d;
//	}


// 임의의 점이 평면의 위(뒤)에 있는 가 아래(앞)에 있는가?
INT LcMath_PlaneToPointRelation(const LCXPLANE* pPn, const VEC3* p1)
{
	FLOAT d = LCXPlaneDotCoord(pPn, p1);

	// 평면의 아래
	if (d < -LCX_R_DELTA)
		return -1;

	// 평면의 위에
	if (d > LCX_R_DELTA)
		return 1;

	// 평면 안에
	return 0;
}


INT LcMath_PlaneToPointIsFront(const LCXPLANE* pPn, const VEC3* p1, FLOAT fEpsilon)
{
	return (pPn->a * p1->x +
			pPn->b * p1->y +
			pPn->c * p1->z +
			pPn->d + fEpsilon ) > 0.0F ? 1: -1;
}


//평행하면 Dot product는 1
INT LcMath_PlaneToPlaneInterSect(const LCXPLANE* p1, const LCXPLANE* p2)
{
	VEC3 n1(p1->a, p1->b, p1->c);
	VEC3 n2(p2->a, p2->b, p2->c);

	FLOAT fD = LCXVec3Dot(&n1, &n2);
	return fabsf(fD)>0.9999f? -1: 1;
}

// 두 평면이 교차하는 직선의 방정식 구하기...
// 이것은 의외로 쉽다.
//
// N1 . P + d1 = 0
// N2 . P + d2 = 0
//
// 에서
//
// P = a * N1 + b * N2
//
// 로 표현할 수 있으면 된다.
//
// N1 . P = a * N1 . N1 + b * N1 . N2
// N2 . P = a * N1 . N2 + b * N2 . N2
//
// 나머지는 행렬을 이용하면 끝!!!

INT LcMath_PlaneToPlaneIntersectLine(LCXLINE* Out, const LCXPLANE* p1, const LCXPLANE* p2)
{
	VEC3	N1(p1->a, p1->b, p1->c);
	VEC3	N2(p2->a, p2->b, p2->c);
	VEC3	N;

	FLOAT f0 = LCXVec3Length(&N1);
	FLOAT f2 = LCXVec3Length(&N2);

	FLOAT f1 = LCXVec3Dot(&N1,&N2);

	FLOAT D = f0*f2 - f1*f1;

	if (fabsf(D) < 0.0001f)
		return -1;

	LCXVec3Cross(&N, &N1, &N2);
	LCXVec3Normalize(&N, &N);
	Out->t = N;

	D = 1.0F / D;
	FLOAT _t0 = (-f2 * p1->d + f1 * p2->d) * D;
	FLOAT _t1 = (-f0 * p2->d + f1 * p1->d) * D;

	Out->p = N1*_t0 + N2*_t1;

	return 0;
}


FLOAT LcMath_PlaneToPointMinDistance(const LCXPLANE* plane, const VEC3* p)
{
	return plane->a * p->x + plane->b * p->y + plane->c * p->z  +  plane->d;
}

INT   LcMath_IsInFrustum ( const LCXPLANE* frustum								// frustum index must be 0: NEAR, 1: far, 2: left, 3: right, 4: top, 5: bottom
						 , const VEC3* pos
						 , FLOAT eps
						 , INT bTestNear
						 , INT bTestFar)
{
	// culling process...

	LCXVECTOR3  obj_pos = *pos;
	FLOAT       obj_eps = eps;
	FLOAT		cull_test;


	if(bTestNear)
	{
		// near
		cull_test = LCXPlaneDotCoord(&frustum[0], &obj_pos) - obj_eps;
		if(0<= cull_test)
			return -1;
	}


	if(bTestFar)
	{
		// far
		cull_test = LCXPlaneDotCoord(&frustum[1], &obj_pos) - obj_eps;
		if(0<= cull_test)
			return -3;
	}


	// left
	cull_test = LCXPlaneDotCoord(&frustum[2], &obj_pos) - obj_eps;
	if(0<= cull_test)
		return -4;

	// right
	cull_test = LCXPlaneDotCoord(&frustum[3], &obj_pos) - obj_eps;
	if(0<= cull_test)
		return -5;

	// top
	cull_test = LCXPlaneDotCoord(&frustum[4], &obj_pos) - obj_eps;
	if(0<= cull_test)
		return -6;


	// bottom
	cull_test = LCXPlaneDotCoord(&frustum[5], &obj_pos) - obj_eps;
	if(0<= cull_test)
		return -2;


	return LC_OK;
}




//++5++++1+++++++++2+++++++++3+++++++++4+++++++++5+++++++++6+++++++++7+++++++++8


INT LcMath_CuboidAABBToLine(VEC3* vcOut, const VEC3* pMin, const VEC3* pMax, const LCXLINE* pLine, INT positiveDir)
{
	INT	hr= -1;
	FLOAT w;
	FLOAT h;
	FLOAT d;

	VEC3	vcP[8];
	VEC3	vcT;
	LCX_TRI	Tri;
	VEC3	vcBuf[3];
	INT   nCollision = LC_FALSE;
	INT   bRectangle = LC_TRUE;

	vcP[0] = *pMin;
	vcP[7] = *pMax;

	vcT = vcP[7]- vcP[0];
	w = vcT.x;
	h = vcT.y;
	d = vcT.z;

	vcP[1] = vcP[0] + VEC3(w, 0, 0);
	vcP[2] = vcP[0] + VEC3(0, h, 0);
	vcP[3] = vcP[0] + VEC3(w, h, 0);
	vcP[4] = vcP[0] + VEC3(0, 0, d);
	vcP[5] = vcP[0] + VEC3(w, 0, d);
	vcP[6] = vcP[0] + VEC3(0, h, d);


	for(INT i=0; i<6; ++i)
	{
		Tri = LCX_TRI(vcP[ LCX_CUBOID::idxTri1[i][0] ], vcP[ LCX_CUBOID::idxTri1[i][1] ], vcP[ LCX_CUBOID::idxTri1[i][2] ]);

		hr = LcMath_LineToTri(&vcT, (VEC3*)&Tri, pLine, bRectangle, positiveDir);

		if(LC_SUCCEEDED(hr))
		{
			vcBuf[nCollision] = vcT;
			++nCollision;
		}

		if(nCollision>=2)
			break;
	}

	if(1 == nCollision)
	{
		*vcOut = vcBuf[0];
		return nCollision;
	}

	else if(2 == nCollision)
	{
		VEC3	a(vcBuf[0] - pLine->p);
		VEC3	b(vcBuf[1] - pLine->p);

		FLOAT fD1 = LCXVec3LengthSq( &a);
		FLOAT fD2 = LCXVec3LengthSq( &b);

		if(fD1<fD2)
		{
			vcOut[0] = vcBuf[0];
			vcOut[1] = vcBuf[1];
		}
		else
		{
			vcOut[0] = vcBuf[1];
			vcOut[1] = vcBuf[0];
		}

		return nCollision;
	}


	return -1;
}


INT LcMath_CuboidAABBToLine(VEC3* vcOut, const LCX_AABB* pCube, const LCXLINE* pLine, INT positiveDir)
{
	return LcMath_CuboidAABBToLine(vcOut, &pCube->vMin, &pCube->vMax, pLine, positiveDir);
}


void LcMath_CuboidAABBLineList(LC_HANDLE Out, INT nChunk,const LCX_AABB* In)
{
	LCX_CUBOID		t;
	t.MinMax(&In->vMin, &In->vMax);
	LcMath_CuboidLineList(Out, nChunk, &t);
}



INT LcMath_CuboidToSphere(const LCX_CUBOID* pHexa, const VEC3* pPoint,  /*In*/ FLOAT fRadius)
{
	INT   i=0, j=0;
	INT	bColl = LC_FALSE;

	// 2. 두 육면체의 점이 각각 전부 안에 있는가 혹은 밖에 있는가?
	LCXPLANE	Plane1[6];
	INT   	hr[6];


	for(i=0;i<6; ++i)
	{
		LCXPlaneFromPoints(	&Plane1[i]
					, (VEC3*)&pHexa->edg[ LCX_CUBOID::idxTri1[i][0] ]
					, (VEC3*)&pHexa->edg[ LCX_CUBOID::idxTri1[i][1] ]
					, (VEC3*)&pHexa->edg[ LCX_CUBOID::idxTri1[i][2] ]);
	}



	for(i=0;i<6; ++i)
	{
		hr[i] = LcMath_PlaneToPointIsFront(&Plane1[i], pPoint, -fRadius);
	}

	if( hr[0]<0 &&
		hr[1]<0 &&
		hr[2]<0 &&
		hr[3]<0 &&
		hr[4]<0 &&
		hr[5]<0
		)
	{
		bColl = LC_TRUE;
	}

	if( LC_TRUE == bColl)
		return 0;

	return -1;
}



INT LcMath_CuboidToTri(const LCX_CUBOID* pHexa, const LCX_TRI* pTri)
{
	INT   i=0;
	INT	bCollision = LC_FALSE;


	// 2. 두 육면체의 점이 각각 전부 안에 있는 가 혹은 밖에 있는가?

	for(i=0; i<3; ++i)
	{
		VEC3	vcP = pTri->p[i];

		bCollision = LcMath_CuboidToSphere(pHexa, &vcP, 0);

		if( LC_TRUE == bCollision)
			return 0;
	}


	// 3. 두 육면체가 교차하지만 포함하는 점이 없다.
	// 삼각형 vs 삼각형 충돌로 검사.

	for(i=0; i<12; ++i)
	{
		VEC3	pTri1[3];
		VEC3	Out[3];


		pTri1[0] = pHexa->edg[LCX_CUBOID::idxTri2[i][0] ];
		pTri1[1] = pHexa->edg[LCX_CUBOID::idxTri2[i][1] ];
		pTri1[2] = pHexa->edg[LCX_CUBOID::idxTri2[i][2] ];


		INT hr = LcMath_TriangleToTriangle(Out, pTri1, (VEC3*)pTri);

		if(LC_SUCCEEDED(hr))
		{
			bCollision = LC_TRUE;
			break;
		}
	}

	if(LC_TRUE == bCollision)
		return 0;


	return -1;

}



INT LcMath_CuboidToH(const LCX_CUBOID* pHexagon1, const LCX_CUBOID* pHexagon2)
{
	INT   i=0, j=0;
	INT	bCollision = LC_FALSE;

	// 1. 최대 Bound Sphere로 체크

	FLOAT fBnd1 = 0.0F;
	FLOAT fBnd2 = 0.0F;
	VEC3	vcCenter1(0,0,0);
	VEC3	vcCenter2(0,0,0);

	VEC3	vcT;
	FLOAT fB=0;


	for(i=0;i<8; ++i)
	{
		vcCenter1 += pHexagon1->edg[i];
		vcCenter2 += pHexagon2->edg[i];
	}

	vcCenter1 /= 8.0F;
	vcCenter2 /= 8.0F;


	for(i=0;i<8; ++i)
	{
		vcT = vcCenter1 - pHexagon1->edg[i];
		fB = LCXVec3Length(&vcT);

		if(fB>fBnd1)
			fBnd1 = fB;


		vcT = vcCenter2 - pHexagon2->edg[i];
		fB = LCXVec3Length(&vcT);

		if(fB>fBnd2)
			fBnd2 = fB;
	}


	vcT = vcCenter1 - vcCenter2;
	fB = LCXVec3Length(&vcT);

	// 박스의 중심 거리와 바운드 구와의 비교

	if(fB>=(fBnd1 + fBnd2))
		return -1;


	// 2. 두 육면체의 점이 각각 전부 안에 있는 가 혹은 밖에 있는가?
	LCXPLANE	Plane1[6];
	LCXPLANE	Plane2[6];
	INT   	hr[6];


	for(i=0;i<6; ++i)
	{
		LCXPlaneFromPoints(	&Plane1[i]
					, (VEC3*)&pHexagon1->edg[ LCX_CUBOID::idxTri1[i][0] ]
					, (VEC3*)&pHexagon1->edg[ LCX_CUBOID::idxTri1[i][1] ]
					, (VEC3*)&pHexagon1->edg[ LCX_CUBOID::idxTri1[i][2] ]);


		LCXPlaneFromPoints(	&Plane2[i]
					, (VEC3*)&pHexagon2->edg[ LCX_CUBOID::idxTri1[i][0] ]
					, (VEC3*)&pHexagon2->edg[ LCX_CUBOID::idxTri1[i][1] ]
					, (VEC3*)&pHexagon2->edg[ LCX_CUBOID::idxTri1[i][2] ]);
	}



	for(j=0; j<8; ++j)
	{
		for(i=0;i<6; ++i)
		{
			hr[i] = LcMath_PlaneToPointIsFront(&Plane1[i], (VEC3*)&pHexagon2->edg[j]);
		}

		if( hr[0]<0 &&
			hr[1]<0 &&
			hr[2]<0 &&
			hr[3]<0 &&
			hr[4]<0 &&
			hr[5]<0
			)
		{
			bCollision = LC_TRUE;
			break;
		}
	}

	if( LC_TRUE == bCollision)
		return 0;


	for(j=0; j<8; ++j)
	{
		for(i=0;i<6; ++i)
		{
			hr[i] = LcMath_PlaneToPointIsFront(&Plane2[i], (VEC3*)&pHexagon1->edg[j]);
		}

		if( hr[0]<0 &&
			hr[1]<0 &&
			hr[2]<0 &&
			hr[3]<0 &&
			hr[4]<0 &&
			hr[5]<0
			)
		{
			bCollision = LC_TRUE;
			break;
		}
	}

	if( LC_TRUE == bCollision)
		return 0;



	// 3. 두 육면체가 교차하지만 포함하는 점이 없다.
	// 삼각형 vs 삼각형 충돌로 검사.
	for(i=0; i<12; ++i)
	{
		VEC3	pTri1[3];
		VEC3	pTri2[3];

		VEC3	Out[3];


		pTri1[0] = pHexagon1->edg[ LCX_CUBOID::idxTri2[i][0] ];
		pTri1[1] = pHexagon1->edg[ LCX_CUBOID::idxTri2[i][1] ];
		pTri1[2] = pHexagon1->edg[ LCX_CUBOID::idxTri2[i][2] ];


		for(j=0; j<12; ++j)
		{
			pTri2[0] = pHexagon2->edg[ LCX_CUBOID::idxTri2[j][0] ];
			pTri2[1] = pHexagon2->edg[ LCX_CUBOID::idxTri2[j][1] ];
			pTri2[2] = pHexagon2->edg[ LCX_CUBOID::idxTri2[j][2] ];

			INT ret = LcMath_TriangleToTriangle(Out, pTri1, pTri2);

			if(LC_SUCCEEDED(ret))
				bCollision = LC_TRUE;
		}

		if(LC_TRUE == bCollision)
			break;
	}

	if(LC_TRUE == bCollision)
		return 0;


	return -1;

}


INT LcMath_CylinderToLine(  LCXVECTOR3* Out
						  , const LCXCYLINDER* cyld
						  , const LCXLINE*     line
						  , const LCXVECTOR3*  pV3)
{
	INT		iInsc=0;

	VEC3	vcInsc[2];								// 실린더 중심선과 직선이 교차하는 지점
	FLOAT	fD = 0.0F;								// 실린더와 직선의 거리
	FLOAT	fR = cyld->r;							// 실린더의 반지름
	FLOAT	fA = 0.0F;								// 장반경

	VEC3	vcL1 = line->t;
	VEC3	vcL2 = cyld->t;
	FLOAT	fSinTheta=0.0F;

	FLOAT	d0, d1;



	// 실린더의 양 끝 원과 직선의 충돌을 구한다.
	LCXVECTOR3	line_plane_pos;
	LCXVECTOR3	plane_pos  = cyld->p;				// bottom circle
	FLOAT		det = 0;

	if(LC_SUCCEEDED(LcMath_LineToPlaneIntersect(&line_plane_pos, line, &plane_pos, &cyld->t)))
	{
		det = LCXVec3DistanceSq(&line_plane_pos, &plane_pos);
		if(det < fR*fR)
		{
			*Out = line_plane_pos;
			return 1;
		}
	}

	plane_pos = cyld->p +  cyld->p * cyld->h;		// top circle
	if(LC_SUCCEEDED(LcMath_LineToPlaneIntersect(&line_plane_pos, line, &plane_pos, &cyld->t)))
	{
		det = LCXVec3DistanceSq(&line_plane_pos, &plane_pos);
		if(det < fR*fR)
		{
			*Out = line_plane_pos;
			return 1;
		}
	}



	// 실린더 몸통의 충돌을 구한다.

	iInsc = LcMath_LineToLineIntersection(&fSinTheta, &fD, vcInsc, line, (LCXLINE*)cyld);

	// 부정이나 불능 두가지 중 하나.
	if(LC_FAILED(iInsc))
	{
		// 원통의 반경과 비교한다.
		// 직선이 원통 안에 완전히 있다.
		if(fD <= cyld->r)
			return 0;	// 점을 정할 수가 없다.

		// 직선이 원통 바깥에 있다.
		else
			return -1;
	}

	// 직선이 원통 바깥에 있다.
	if(fabsf(fD)>=fR)
		return -1;


	// 충돌한 두 점의 위치를 찾는다.
	fA = fR/fabsf(fSinTheta);

	FLOAT fPi = fD/fR;
	fPi = asinf(fPi);

	FLOAT fK = fA * cosf( fPi);

	Out[0] = vcInsc[0] - fK * vcL1;
	Out[1] = vcInsc[0] + fK * vcL1;


	if(NULL == pV3)
	{
		VEC3 t0 = Out[0]- cyld->p;
		VEC3 t1 = Out[1]- cyld->p;

		d0 =  LCXVec3Dot(&cyld->t, &t0);
		d1 =  LCXVec3Dot(&cyld->t, &t1);


		if(0 > d0 || d0 > cyld->h)			// 원통 밖
		{
			if(0 > d1 || d1 > cyld->h)		// 원통 밖
				return -1;

			Out[0] = Out[1];
			return 1;
		}

		if(0 > d1 || d1 > cyld->h)			// 원통 밖
		{
			if(0 > d0 || d0 > cyld->h)		// 원통 밖
				return -1;

			return 1;
		}

		return 2;
	}


	//
	FLOAT fDot[2];

	VEC3 vcDot0;
	VEC3 vcDot1;

	vcDot0 = pV3[0] - Out[0];
	vcDot1 = pV3[1] - Out[0];
	fDot[0] = LCXVec3Dot(&vcDot0, &vcDot1);

	vcDot0 = pV3[0] - Out[1];
	vcDot1 = pV3[1] - Out[1];
	fDot[1] = LCXVec3Dot(&vcDot0, &vcDot1);


	// 0번째만 영역 안에 있다.
	if( fDot[0] <0 && fDot[1] >0)
	{
		return 0;
	}

	// 1번째만 영역 안에 있다.
	else if( fDot[0] >0 && fDot[1] <0)
	{
		return 1;
	}

	// 둘다 영역 안에 있다.
	else if( fDot[0] <0 && fDot[1] <0)
	{
		return 2;
	}



	return -1;
}





// 원통과 삼각형의 충돌
// 삼각형이 원통안에 있으면 삼ㄱ각형과 원통의 최단거리 지점을 반환
// 원통의 중심선이 삼각형과 교차하면 교차 지점을 반환
// 나머지... 삼각형과 원통의 충돌 지점을 반환
INT LcMath_CylinderToTri(	VEC3* Out				// Intersection point
						  ,	const LCXCYLINDER* pV1	// Input Cylinder
						  , const VEC3* pV2			// Input Triangle
						  )
{

	// 1. 충돌하지 않는 삼각형들은 제외시키낟.
	// 간단하게 삼각형을 둘러싸고 있는 구와 원통의 충돌을 구한다.
	// 원통 반지름 + 삼각형 충돌 반지름  < 원통와 삼각형의 최단 거리 이면 충돌이 아님.

	VEC3	TriCenter;
	FLOAT TriRadius = LcMath_TriangleBoundSphere(&TriCenter, pV2);
	VEC3	pTriToSphere(0,0,0);

	// 원통과 삼각형 중심의 최단거리
	FLOAT fDstLineFromPoint;

	LcMath_LineToSphere(NULL, &fDstLineFromPoint, (LCXLINE*)pV1, &TriCenter, TriRadius);

	if( fDstLineFromPoint>= (TriRadius + pV1->r))
		return -1;


	// 2. 삼각형이 원통 내부에 있을 경우
	// 완전히 있다면 충돌이며 중돌 포인트는 삼각형의 중심으로 한다.

	if(TriRadius<pV1->r)
	{
		// 삼각형이 원통 안에 완전히 있는가?
		if(fDstLineFromPoint<= (pV1->r - TriRadius))
		{
			*Out = TriCenter;
			return 0;
		}
	}


	// 나머지 충돌은 두개가 남아 있다. 하나는 원통의 중심 직선이
	// 삼각형과 충돌 여부,  다른 하나는 삼각형의 세 변과 원통이 충돌하는 경우다.


	// 3. 원통의 중심 직선이 삼각형과 충돌 하는가?
	if(LC_SUCCEEDED(LcMath_LineToTri(Out, pV2, (LCXLINE*)pV1)))
		return 0;


	// 4. 나머지...
	// 삼각형을 이루고 있는 세 유한 직선이 원동과 충돌하는 지점을 구한다.

	VEC3	vcCol[6];
	INT   nCnt=-1;

	for(INT i=0; i<3; ++i)
	{
		VEC3	vcOutT[2];
		INT   hr;
		VEC3	vcBnd[2];
		LCXLINE	Line;


		INT   nTriIdx0 = (i+0)%3;
		INT   nTriIdx1 = (i+1)%3;

		vcBnd[0] = pV2[nTriIdx0];
		vcBnd[1] = pV2[nTriIdx1];

		Line.p = vcBnd[0];
		Line.t = vcBnd[1] - vcBnd[0];
		LCXVec3Normalize(&Line.t, &Line.t);

		hr = LcMath_CylinderToLine(vcOutT, pV1, &Line, vcBnd);

		if(LC_FAILED(hr))
			continue;

		if(0 == hr)
		{
			++nCnt;
			vcCol[nCnt] = vcOutT[0];
		}

		else if(  1== hr)
		{
			++nCnt;
			vcCol[nCnt] = vcOutT[1];
		}

		else if( 2 == hr)
		{
			++nCnt;
			vcCol[nCnt] = vcOutT[0];

			++nCnt;
			vcCol[nCnt] = vcOutT[1];
		}
	}

	if(nCnt>=0)
	{
		memcpy(Out, vcCol, (nCnt+1) * sizeof (VEC3));
		return (nCnt+1);
	}

	return -1;
}




// 원통과 삼각형의 충돌
// 삼각형이 원통안에 있으면 삼각형과 원통의 최단거리 지점을 반환
// 원통의 중심선이 삼각형과 교차하면 교차 지점을 반환
// 나머지... 삼각형과 원통의 충돌 지점을 반환
INT LcMath_CylinderToTri(	VEC3* Out					// Intersection point
						,	const LCXCYLINDER* pV1		// Input Cylinder
						,	LCX_PCK* pV2				// Input Bound Triangle
						,	INT bRectangle				// 충돌영역을 삼각형의 확장 평행사변형으로 쓸것인지..
						,	INT positiveDir		// 직선의 방향으로만 충돌점을 구하기
						,	const VEC3* pBoundLine		// 유한 직선인 경우 양 끝 지점
						,	const VEC3* pTriNormal		// 삼각형의 Normal Vector
						)
{
	INT   hr = 0;

	// 1. 충돌하지 않는 삼각형들은 제외시키낟.
	// 간단하게 삼각형을 둘러싸고 있는 구와 원통의 충돌을 구한다.
	// 원통 반지름 + 삼각형 충돌 반지름  < 원통와 삼각형의 최단 거리 이면 충돌이 아님.

	VEC3	TriCenter = pV2->BoundCenter();
	FLOAT 	TriRadius = pV2->BoundRadius();
	VEC3	pTriToSphere(0,0,0);

	// 원통과 삼각형 중심의 최단거리
	FLOAT fDstLineFromPoint;

	LcMath_LineToSphere(NULL, &fDstLineFromPoint, (LCXLINE*)pV1, &TriCenter, TriRadius);

	if( fDstLineFromPoint>= (TriRadius + pV1->r))
		return -1;


	// 2. 삼각형이 원통 내부에 있을 경우
	// 완전히 있다면 충돌이며 중돌 포인트는 삼각형의 중심으로 한다.

	if(TriRadius<pV1->r)
	{
		// 삼각형이 원통 안에 완전히 있는가?
		if(fDstLineFromPoint<= (pV1->r - TriRadius))
		{
			*Out = TriCenter;
			return 0;
		}
	}


	// 나머지 충돌은 두개가 남아 있다. 하나는 원통의 중심 직선이
	// 삼각형과 충돌 여부,  다른 하나는 삼각형의 세 변과 원통이 충돌하는 경우다.


	// 3. 원통의 중심 직선이 삼각형과 충돌 하는가?
	hr = LcMath_LineToTri(Out, &pV2->tri.p[0], (LCXLINE*)pV1, bRectangle, positiveDir, pBoundLine, pTriNormal);
	if(LC_SUCCEEDED(hr))
		return 0;


	// 4. 나머지...
	// 삼각형을 이루고 있는 세 유한 직선이 원동과 충돌하는 지점을 구한다.

	VEC3	vcCol[6];
	INT   nCnt=-1;

	for(INT i=0; i<3; ++i)
	{
		VEC3	vcOutT[2];
		INT		ret;
		VEC3	vcBnd[2];
		LCXLINE	Line;


		INT   nTriIdx0 = (i+0)%3;
		INT   nTriIdx1 = (i+1)%3;

		vcBnd[0] = pV2->tri.p[ nTriIdx0 ];
		vcBnd[1] = pV2->tri.p[ nTriIdx1 ];

		Line.p = vcBnd[0];
		Line.t = vcBnd[1] - vcBnd[0];
		LCXVec3Normalize(&Line.t, &Line.t);

		ret = LcMath_CylinderToLine(vcOutT, pV1, &Line, vcBnd);

		if(LC_FAILED(ret))
			continue;

		if(0 == ret)
		{
			++nCnt;
			vcCol[nCnt] = vcOutT[0];
		}

		else if( 1 == ret)
		{
			++nCnt;
			vcCol[nCnt] = vcOutT[1];
		}

		else if( 2 == ret)
		{
			++nCnt;
			vcCol[nCnt] = vcOutT[0];

			++nCnt;
			vcCol[nCnt] = vcOutT[1];
		}
	}

	if(nCnt>=0)
	{
		memcpy(Out, vcCol, (nCnt+1) * sizeof (VEC3));

		// 다른 것과 구분하기 위해서 <<2한다.
		return (nCnt+1)<<2;
	}

	return -1;
}



INT LcMath_SphereToTri(		VEC3* Out						// 구와 삼각형의 최단거리의 지점
						,	FLOAT* fDist					// 구와 삼각형의 최단거리
						,	const VEC3* pSphereCenter		// 구의 중앙
						,	const FLOAT* pSphereRadius		// 구의 반경
						,	const VEC3* pTri				// 삼각형의 세점
						,	VEC3* pTriCenter				// 삼각형의 바운드구의 중심
						,	FLOAT* pTriRadius				// 삼각형의 바운드구의 반경
						,	VEC3* pTriNormal				// 삼각형의 노말멕터
						)
{
	VEC3	vTbndR;
	FLOAT fTbndR;

	INT		i=0;

	// 1. 삼각형의 충돌 바운드가 있다면..
	if(NULL == pTriCenter)
	{
		fTbndR = LcMath_TriangleBoundSphere(&vTbndR, pTri);
	}
	else
	{
		vTbndR	= *pTriCenter;
		fTbndR	= *pTriRadius;
	}

	VEC3	vcR = vTbndR - *pSphereCenter;
	FLOAT fRadiusA = LCXVec3LengthSq( &vcR);


	FLOAT fRadius12 = *pSphereRadius + fTbndR;

	fRadius12 = fRadius12 * fRadius12;

	if(fRadius12 < fRadiusA)
	{
		return -1;
	}


	// 1. 삼각형 만드는 평면에서 구의 중심까지의 최단거리를 만드는 지점과 거리를 구한다.
	// 최단거리가 구의 반경보다 크면 충돌을 안함.
	LCXPLANE plane;
	LCXPlaneFromPoints(&plane, &pTri[0], &pTri[1], &pTri[2]);

	*fDist = LcMath_PlaneToPointMinDistance(&plane, pSphereCenter);

	if( fRadiusA < (*fDist) * (*fDist))
		return -1;

	FLOAT k = LCXVec3Dot( (VEC3*)&plane, &vcR);

	*Out = *pSphereCenter + k *  *((VEC3*)&plane);






	FLOAT fU, fV;

	if(LC_FAILED(LcMath_TriUV(&fU, &fV, Out, &pTri[0], &pTri[1], &pTri[2])))
		return -1;

	// 2. 앞서 구한 지점이 삼각형 내부에 있다면 무조건 충돌.
	if(fU>=-0.0001f && fU<=1.0001f && fV>=-0.0001f && fV<=1.0001f && (fU+fV)<1.0001f)
		return 0;

	// 3. 삼각형의 세 점우 하나라도 구 안에 있는가 검사.
	for(i=0; i<3; ++i)
	{
		VEC3	vcTc = pTri[i] - *pSphereCenter;
		FLOAT fTr	= LCXVec3LengthSq( &vcTc);

		if( fTr< (*pSphereRadius) * (*pSphereRadius))
			return 0;
	}


	// 4. 나머지는 삼각형이 구에 걸치는 형태
	// 앞서 구한 지점이 삼각형 외부에 존재한다면 삼각형이 만드는 세직선과 구의 충돌을 구한다.

	for(i=0; i<3; ++i)
	{
		VEC3 vcTstOut;
		FLOAT fTstDst;
		LCXLINE pLine;

		INT n0 = (i+0)%3;
		INT n1 = (i+1)%3;

		pLine.p = pTri[n0];
		pLine.t = pTri[n1] - pTri[n0];

		LCXVec3Normalize(&pLine.t, &pLine.t);

		// 구와 충돌한 지점을 구하고...
		if(LC_SUCCEEDED(LcMath_LineToSphere(&vcTstOut, &fTstDst, &pLine, pSphereCenter, *pSphereRadius)))
		{
			// 이 점이 유한 직선 안에 있는가 검사.

			VEC3 vcTA = pTri[n0] - vcTstOut;
			VEC3 vcTB = pTri[n1] - vcTstOut;

			FLOAT fTst = LCXVec3Dot(&vcTA, &vcTB);

			//만약 안에 있다면 fTst<0이어야 한다.
			if(fTst<0.0F)
				return 0;

		}
	}


	return -1;
}



INT LcMath_SphereToSphere(	const VEC3* pSphereCenter1							// 구의 중앙1
						,	const FLOAT* pSphereRadius1							// 구의 반경1
						,	const VEC3* pSphereCenter2							// 구의 중앙2
						,	const FLOAT* pSphereRadius2							// 구의 반경2
						)
{
	FLOAT fDst;
	VEC3 vcAB = *pSphereCenter2 - *pSphereCenter1;
	fDst = LCXVec3Length(&vcAB);

	if( fDst < ( *pSphereRadius1 + *pSphereRadius2))
		return 0;

	return -1;
}


