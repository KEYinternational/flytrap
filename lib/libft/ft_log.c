/*-
 * Copyright (c) 2013-2016 The University of Oslo
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote
 *    products derived from this software without specific prior written
 *    permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>

#define FT_LOGV_REQUIRED
#include <ft/log.h>
#include <ft/strutil.h>

static char ft_prog_name[16];
ft_log_level_t ft_log_level;

#if 0
static int
ft_log_level_to_syslog(ft_log_level_t level)
{

	switch (level) {
	case FT_LOG_LEVEL_DEBUG:
		return (LOG_DEBUG);
	case FT_LOG_LEVEL_VERBOSE:
		return (LOG_INFO);
	case FT_LOG_LEVEL_NOTICE:
		return (LOG_NOTICE);
	case FT_LOG_LEVEL_WARNING:
		return (LOG_WARNING);
	case FT_LOG_LEVEL_ERROR:
		return (LOG_ERR);
	default:
		return (LOG_INFO);
	}
}
#endif

static const char *
ft_log_level_to_string(ft_log_level_t level)
{

	switch (level) {
	case FT_LOG_LEVEL_DEBUG:
		return ("debug");
	case FT_LOG_LEVEL_VERBOSE:
		return ("verbose");
	case FT_LOG_LEVEL_NOTICE:
		return ("notice");
	case FT_LOG_LEVEL_WARNING:
		return ("warning");
	case FT_LOG_LEVEL_ERROR:
		return ("error");
	default:
		return ("unknown");
	}
}

void
ft_logv(ft_log_level_t level, const char *fmt, va_list ap)
{

	fprintf(stderr, "%s: %s: ", ft_prog_name,
	    ft_log_level_to_string(level));
	vfprintf(stderr, fmt, ap);
	fprintf(stderr, "\n");
}

/*
 * Log a message if at or above selected log level.
 */
void
ft_log(ft_log_level_t level, const char *fmt, ...)
{
	va_list ap;
	int serrno;

	serrno = errno;
	if (level >= ft_log_level) {
		va_start(ap, fmt);
		ft_logv(level, fmt, ap);
		va_end(ap);
	}
	errno = serrno;
}

void
ft_fatal(const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	ft_logv(FT_LOG_LEVEL_ERROR, fmt, ap);
	va_end(ap);
	exit(1);
}

/*
 * Specify a destination for log messages.  Passing NULL or an empty
 * string resets the log destination to stderr.
 */
int
ft_log_init(const char *ident, const char *logspec)
{

	strlcpy(ft_prog_name, ident, sizeof ft_prog_name);
	(void)logspec;
	return (0);
}

/*
 * Close all log destinations
 */
int
ft_log_exit(void)
{

	return (0);
}
