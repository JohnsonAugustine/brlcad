/*
 *  Authors -
 *	John R. Anderson
 *	Susanne L. Muuss
 *	Earl P. Weaver
 *
 *  Source -
 *	VLD/ASB Building 1065
 *	The U. S. Army Ballistic Research Laboratory
 *	Aberdeen Proving Ground, Maryland  21005
 *  
 *  Copyright Notice -
 *	This software is Copyright (C) 1990 by the United States Army.
 *	All rights reserved.
 */

/* This routine reads the last record in the IGES file.
	That record contains the nunber of records in each section.
	These numbers are used to calculate the starting record number
	for the parameter section and the directory section.
	space is then reserved for the directory.  This routine depends on
	"fseek" and "ftell" operating with offsets given in bytes.	*/

#include "conf.h"

#include <stdio.h>
#include <errno.h>

#include "machine.h"
#include "vmath.h"
#include "./iges_struct.h"
#include "./iges_extern.h"

Findp()
{
	int saverec,rec2,i;
	long offset;
	char str[8];

	str[7] = '\0';


	saverec = currec;	/* save current record number */

	if( fseek( fd , 0L , 2 ) )	/* go to end of file */
	{
		fprintf( stderr , "Cannot seek to end of file\n" );
		perror( "Findp" );
		exit( 1 );
	}
	offset = ftell( fd );	/* get file length */
	rec2 = offset/reclen;	/* calculate record number for last record */
	Readrec( rec2 );	/* read last record into "card" buffer */
	dstart = 0;
	pstart = 0;
	for( i=0 ; i<3 ; i++ )
	{
		counter++;	/* skip the single letter section ID */
		Readcols( str , 7 );	/* read the number of records in the section */
		pstart += atoi( str );	/* increment pstart */
		if( i == 1 )	/* Global section */
		{
			/* set record number for start of directory section */
			dstart = pstart;
		}
	}

	/* restore current record */
	currec = saverec;
	Readrec( currec );

	/* make space for directory entries */
	totentities = (pstart - dstart)/2;
	dirarraylen = totentities;
	dir = (struct iges_directory **)rt_calloc( totentities ,
		sizeof( struct iges_directory *),
		"IGES directory*" );

	for( i=0 ; i<totentities ; i++ )
	{
		dir[i] = (struct iges_directory *)rt_malloc( sizeof( struct iges_directory ) , "IGES directory" );
		dir[i]->name = (char *)NULL;
	}

	return( pstart );
}
