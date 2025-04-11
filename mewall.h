/**************************************************
 * - Author: so2u
 * - Create Time: 2024-12-07 02:56:40
 * - Git: https://github.com/svan9/mewlib
 **************************************************/

#ifndef __MEWLIB_ALL__
#define __MEWLIB_ALL__

#ifdef RAYLIB_H
#define MEWALL_NO_WIN_ALIASES
#endif

#include "mewlib.h"
#include "mewmath.hpp"
#include "mewstack.hpp"
#include "mewstring.hpp"
#include "mewmap.hpp"
#include "mewlist.hpp"
#include "mewmatrix.hpp"
#include "mewsmap.hpp"
#include "mewiterator.hpp"
#include "mewprint.hpp"
#ifndef MEWALL_NO_WIN_ALIASES
#include "mewargs.hpp"
#include "mewdll.hpp"
#endif
#include "mewutils.hpp"
#include "mewfeature.hpp"
#include "mewbinary.hpp"
#include "mewvector.hpp"
#include "mewnmath.hpp"
#include "mewarena.hpp"

#endif