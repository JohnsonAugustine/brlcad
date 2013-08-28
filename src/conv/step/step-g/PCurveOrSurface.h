/*                 PCurveOrSurface.h
 * BRL-CAD
 *
 * Copyright (c) 1994-2013 United States Government as represented by
 * the U.S. Army Research Laboratory.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * version 2.1 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this file; see the file named COPYING for more
 * information.
 */
/** @file step/PCurveOrSurface.h
 *
 * Class definition used to convert STEP "PCurveOrSurface" to BRL-CAD BREP
 * structures.
 *
 */

#ifndef PCURVEORSURFACE_H_
#define PCURVEORSURFACE_H_

#include "STEPEntity.h"

class PCurve;
class Surface;

class PCurveOrSurface : virtual public STEPEntity
{
public:
    enum pcurve_or_surface_type {
	PCURVE,
	SURFACE,
	UNKNOWN
    };

private:
    static string entityname;
    static EntityInstanceFunc GetInstance;

protected:
    PCurve *pcurve;
    Surface *surface;
    pcurve_or_surface_type type;

public:
    PCurveOrSurface();
    virtual ~PCurveOrSurface();
    PCurveOrSurface(STEPWrapper *sw, int step_id);
    bool Load(STEPWrapper *sw, SDAI_Select *sse);
    virtual void Print(int level);

    //static methods
    static STEPEntity *Create(STEPWrapper *sw, SDAI_Application_instance *sse);
};

#endif /* PCURVEORSURFACE_H_ */

/*
 * Local Variables:
 * tab-width: 8
 * mode: C
 * indent-tabs-mode: t
 * c-file-style: "stroustrup"
 * End:
 * ex: shiftwidth=4 tabstop=8
 */