/*
 *			S T R S O L . C
 *
 *  Library for writing strsol solids to MGED databases.
 *  Assumes that some of the structure of such databases are known
 *  by the calling routines.
 *
 *
 *  Note that routines which are passed point_t or vect_t or mat_t
 *  parameters (which are call-by-address) must be VERY careful to
 *  leave those parameters unmodified (eg, by scaling), so that the
 *  calling routine is not surprised.
 *
 *  Return codes of 0 are OK, -1 signal an error.
 *
 *  Authors -
 *	John R. Anderson
 *  
 *  Source -
 *	BVLD/VMB Advanced Computer Systems Team
 *	The U. S. Army Research Laboratory
 *	Aberdeen Proving Ground, Maryland  21005-5066
 *
 *  Distribution Notice -
 *	Re-distribution of this software is restricted, as described in
 *	your "Statement of Terms and Conditions for the Release of
 *	The BRL-CAD Pacakge" agreement.
 *
 *  Copyright Notice -
 *	This software is Copyright (C) 1994 by the United States Army
 *	in all countries except the USA.  All rights reserved.
 */

#ifndef lint
static char RCSid[] = "@(#)$Header$ (BRL)";
#endif

#include "conf.h"

#include <stdio.h>
#include <math.h>
#include "machine.h"
#include "vmath.h"
#include "rtlist.h"
#include "rtgeom.h"
#include "raytrace.h"
#include "wdb.h"

int
mk_ebm( fp, name, file, xdim, ydim, tallness, mat )
FILE		*fp;
char		*name;
char		*file;		/* name of file containing bitmap */
int		xdim;		/* X dimansion of file (w cells) */
int		ydim;		/* Y dimension of file (n cells) */
fastf_t		tallness;	/* Z extrusion height (mm) */
mat_t		mat;		/* convert local coords to model space */
{
	struct rt_ebm_internal	ebm;

	ebm.magic = RT_EBM_INTERNAL_MAGIC;
	strncpy( ebm.file, file, RT_EBM_NAME_LEN );
	ebm.xdim = xdim;
	ebm.ydim = ydim;
	ebm.tallness = tallness;
	mat_copy( ebm.mat , mat );

	return( mk_export_fwrite( fp, name, (genptr_t)&ebm, ID_EBM ) );
}

int
mk_vol( fp, name, file, xdim, ydim, zdim, lo, hi, cellsize, mat )
FILE		*fp;
char		*name;
char		*file;		/* name of file containing bitmap */
int		xdim;		/* X dimansion of file (w cells) */
int		ydim;		/* Y dimension of file (n cells) */
int		zdim;		/* Z dimension of file (d cells) */
int		lo;		/* Low threshold */
int		hi;		/* High threshold */
vect_t		cellsize;	/* ideal coords: size of each cell */
mat_t		mat;		/* convert local coords to model space */
{
	struct rt_vol_internal	vol;

	vol.magic = RT_VOL_INTERNAL_MAGIC;
	strncpy( vol.file, file, RT_VOL_NAME_LEN );
	vol.xdim = xdim;
	vol.ydim = ydim;
	vol.zdim = zdim;
	vol.lo = lo;
	vol.hi = hi;
	VMOVE( vol.cellsize , cellsize );
	mat_copy( vol.mat , mat );

	return( mk_export_fwrite( fp, name, (genptr_t)&vol, ID_VOL ) );
}
