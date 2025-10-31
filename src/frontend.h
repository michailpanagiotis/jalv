// Copyright 2007-2022 David Robillard <d@drobilla.net>
// SPDX-License-Identifier: ISC

#ifndef JALV_FRONTEND_H
#define JALV_FRONTEND_H

#include "attributes.h"
#include "options.h"

#include "lilv/lilv.h"

#include <stdbool.h>
#include <stdint.h>

JALV_BEGIN_DECLS

typedef struct JalvImpl Jalv;

/**
   Initialize the frontend.

   Consumes command-line arguments from argc/argv for frontend-specific
   configuration and stores remaining arguments back for plugin selection.
*/
int
jalv_frontend_init(int* argc, char*** argv, JalvOptions* opts);

/**
   Return the URI of the "native" LV2 UI type for this frontend.
*/
const char*
jalv_frontend_ui_type(void);

/**
   Discover if an interactive frontend is available.
*/
bool
jalv_frontend_discover(Jalv* jalv);

/**
   Return the ideal refresh rate of the frontend in Hz.
*/
float
jalv_frontend_refresh_rate(Jalv* jalv);

/**
   Return the scale factor of the frontend.
*/
float
jalv_frontend_scale_factor(Jalv* jalv);

/**
   Attempt to get a plugin URI selection from the user.
*/
LilvNode*
jalv_frontend_select_plugin(Jalv* jalv);

/**
   Open and run the frontend interface.
*/
int
jalv_frontend_open(Jalv* jalv);

/**
   Quit and close the frontend interface.
*/
int
jalv_frontend_close(Jalv* jalv);

JALV_END_DECLS

#endif // JALV_FRONTEND_H
