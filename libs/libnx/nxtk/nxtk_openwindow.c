/****************************************************************************
 * libs/libnx/nxtk/nxtk_openwindow.c
 *
 *   Copyright (C) 2008-2009, 2012-2013, 2019 Gregory Nutt. All rights
 *     reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#include <stdlib.h>
#include <errno.h>
#include <debug.h>

#include <nuttx/nx/nx.h>

#include "nxcontext.h"
#include "nxtk.h"

/****************************************************************************
 * Public Data
 ****************************************************************************/

nxgl_mxpixel_t g_bordercolor1[CONFIG_NX_NPLANES] =
{
  CONFIG_NXTK_BORDERCOLOR1
#if CONFIG_NX_NPLANES > 1
#  error "Multiple plane border colors not defined"
#endif
};

nxgl_mxpixel_t g_bordercolor2[CONFIG_NX_NPLANES] =
{
  CONFIG_NXTK_BORDERCOLOR2
#if CONFIG_NX_NPLANES > 1
#  error "Multiple plane border colors not defined"
#endif
};

nxgl_mxpixel_t g_bordercolor3[CONFIG_NX_NPLANES] =
{
  CONFIG_NXTK_BORDERCOLOR3
#if CONFIG_NX_NPLANES > 1
#  error "Multiple plane border colors not defined"
#endif
};

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: nxtk_openwindow
 *
 * Description:
 *   Create a new, framed window.
 *
 * Input Parameters:
 *   handle - The handle returned by nx_connect
 *   flags  - Optional flags.  Must be zero unless CONFIG_NX_RAMBACKED is
 *            enabled.  In that case, it may be zero or
 *            NXBE_WINDOW_RAMBACKED
 *   cb     - Callbacks used to process window events
 *   arg    - User provided value that will be returned with NXTK callbacks.
 *
 * Returned Value:
 *   Success: A non-NULL handle used with subsequent NXTK window accesses
 *   Failure:  NULL is returned and errno is set appropriately
 *
 ****************************************************************************/

NXTKWINDOW nxtk_openwindow(NXHANDLE handle, uint8_t flags,
                           FAR const struct nx_callback_s *cb,
                           FAR void *arg)
{
  FAR struct nxtk_framedwindow_s *fwnd;
  int ret;

#ifdef CONFIG_DEBUG_FEATURES
  if (handle == NULL || cb == NULL ||  (flags & ~NXTK_WINDOW_USER) != 0)
    {
      set_errno(EINVAL);
      return NULL;
    }
#endif

  /* Pre-allocate the window structure */

  fwnd = (FAR struct nxtk_framedwindow_s *)
    lib_uzalloc(sizeof(struct nxtk_framedwindow_s));

  if (fwnd == NULL)
    {
      set_errno(ENOMEM);
      return NULL;
    }

  /* Initialize the window structure */

  fwnd->fwcb  = cb;
  fwnd->fwarg = arg;

  /* Then let nx_constructwindow do the rest */

  ret = nx_constructwindow(handle, (NXWINDOW)&fwnd->wnd,
                           flags | NXBE_WINDOW_FRAMED, &g_nxtkcb, NULL);
  if (ret < 0)
    {
      /* An error occurred, the window has been freed */

      return NULL;
    }

  /* Return the initialized window reference */

  return (NXTKWINDOW)fwnd;
}
