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
