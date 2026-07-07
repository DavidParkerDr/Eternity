#pragma once
#include <cmath>
#include "Irrlicht.h"

// Useful inlined maths functions

static const double PI	  = 3.1415926535897932384626433832795;
static const double PI360 = 2*PI;
static const double PI90  = PI*0.5;
static const double PI270 = PI*1.5;
static const float TINY   = 1.0E-08f;
static const float TORAD  = 0.01745329254f; // = PI / 180
static const float COS45  = 0.7071067812f;

inline double toRad(double angle)				{ return angle * TORAD; }
inline double toDeg(double angle)				{ return angle / TORAD; }
inline double toRad(float angle)				{ return angle * TORAD; }
inline double toDeg(float angle)				{ return angle / TORAD; }
inline float msin(float angle)					{ return (float)sin(toRad(angle)); }
inline float mcos(float angle)					{ return (float)cos(toRad(angle)); }
inline float msin(int angle)					{ return (float)sin(toRad((float)angle)); }
inline float mcos(int angle)					{ return (float)cos(toRad((float)angle)); }
inline float dist(float x, float y, float z)	{ return (float)(sqrt((x*x)+(y*y)+(z*z))); }
inline float dist2(float x, float y, float z)	{ return (float)((x*x)+(y*y)+(z*z)); }
inline float dist(float x, float y)				{ return (float)(sqrt((x*x)+(y*y))); }
inline float dist(float x1, float y1, 
				  float x2, float y2)			{ float dx = x2-x1; float dy = y2-y1; return (float)(sqrt((dx*dx)+(dy*dy))); }
inline float dist2(float x1, float y1, 
				   float x2, float y2)			{ float dx = x2-x1; float dy = y2-y1; return ((dx*dx)+(dy*dy)); }
inline float dist(float x1, float y1, float z1, 
				  float x2, float y2, float z2)	{ float dx = x2-x1; float dy = y2-y1; float dz = z2-z1; return (float)(sqrt((dx*dx)+(dy*dy)+(dz*dz))); }
inline float dist2(float x1, float y1, 
				   float z1, float x2, 
				   float y2, float z2)			{ float dx = x2-x1; float dy = y2-y1; float dz = z2-z1; return (float)((dx*dx)+(dy*dy)+(dz*dz)); }
inline float absf(float x)						{ return (float)fabs(x); }
inline float mmax(float a, float b)				{ if (a > b) return a; return b; }
inline float mmin(float a, float b)				{ if (a < b) return a; return b; }
inline int mmax(int a, int b)					{ if (a > b) return a; return b; }
inline int mmin(int a, int b)					{ if (a < b) return a; return b; }
inline float mmid(float a, float b)				{ return (a + b) /2; }
inline float xpos(float lat, float longt, 
				  float alt)					{ return alt * mcos(longt) * msin(lat); }
inline float ypos(float lat, float longt, 
				  float alt)					{ return mcos(lat); }
inline float zpos(float lat, float longt, 
				  float alt)					{ return alt * msin(longt) * msin(lat); }
inline double wrap(double v)					{ while (v >= 360) v -= 360; while (v < 0) v += 360; return v; }
inline double wrapr(double v)					{ while (v >= PI360) v -= PI360; while (v < 0) v += PI360; return v; }
inline float wrap(float v)						{ while (v >= 360) v -= 360; while (v < 0) v += 360; return v; }
inline float wrapr(float v)						{ while (v >= PI360) v -= (float)PI360; while (v < 0) v += (float)PI360; return v; }
inline int sgn(int i)							{ if (i > 0) return 1; if (i < 0) return -1; return 0; }
inline int sgn(double i)						{ if (i > 0) return 1; if (i < 0) return -1; return 0; }
inline double usgn(double i)					{ return (sgn(i) + 1)/2.0f; }
inline int usgn0(double i)						{ if (i>0) return 1; return 0; }

inline irr::core::vector3df rotateVector(irr::core::vector3df v, float angle)	
{ 
	return irr::core::vector3df(v.X * mcos(angle) + v.Y * msin(angle), v.X * -msin(angle) + v.Y * mcos(angle), v.Z); 
}

inline float bearingTo(const irr::core::vector3df& relativePosition)
{
	// Straight up or down
	if (relativePosition.X == 0)
	{
		if (relativePosition.Y < 0) // We're below
		{
			return 180;
		}
		else // We're above
		{
			return 0;
		}
	}
	// Otherwise
	else
	{
		double angle = atan(relativePosition.Y/relativePosition.X) * 180 / irr::core::PI;
		if (relativePosition.X > 0)
		{
			angle = 90 - angle;
		}
		else // relativePosition.X < 0
		{
			angle = 270 - angle;
		}
		return wrap((float)angle);
	}
}