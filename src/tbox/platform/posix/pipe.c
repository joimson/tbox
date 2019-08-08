/*!The Treasure Box Library
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * 
 * Copyright (C) 2009 - 2019, TBOOX Open Source Group.
 *
 * @author      ruki
 * @file        pipe.c
 * @ingroup     platform
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "../pipe.h"
#include "../file.h"
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

/* //////////////////////////////////////////////////////////////////////////////////////
 * macros
 */

// fd to pipe file
#define tb_fd2pipefile(fd)              ((fd) >= 0? (tb_pipe_file_ref_t)((tb_long_t)(fd) + 1) : tb_null)

// pipe file to fd
#define tb_pipefile2fd(file)            (tb_int_t)((file)? (((tb_long_t)(file)) - 1) : -1)

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
#ifdef TB_CONFIG_POSIX_HAVE_MKFIFO
tb_pipe_file_ref_t tb_pipe_file_init(tb_char_t const* name, tb_size_t mode, tb_size_t buffer_size)
{
    // check
    tb_assert_and_check_return_val(name, tb_null);

    tb_bool_t ok = tb_false;
    tb_int_t  fd = -1;
    do
    {
        // this pipe is not exists? we create it first
        if (access(name, F_OK) != 0)
        {
            // readonly? We need to wait for other write-client to create a pipe
            if (mode & TB_FILE_MODE_RO)
                break;

            // 0644: -rw-r--r-- 
            if (mkfifo(name, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH) != 0)
                break;
        }

        // init flags
        tb_size_t flags = 0;
        if (mode & TB_FILE_MODE_RO) flags |= O_NONBLOCK | O_RDONLY;
        else if (mode & TB_FILE_MODE_WO) flags |= O_WRONLY;
        tb_assert_and_check_break(flags);

        // open pipe file
        fd = open(name, flags);
        tb_assert_and_check_break(fd >= 0);

        // ok
        ok = tb_true;

    } while (0);
    return ok? tb_fd2pipefile(fd) : tb_null;
}
#else
tb_pipe_file_ref_t tb_pipe_file_init(tb_char_t const* name, tb_size_t mode, tb_size_t buffer_size)
{
    tb_trace_noimpl();
    return tb_null;
}
#endif

#if defined(TB_CONFIG_POSIX_HAVE_PIPE) || defined(TB_CONFIG_POSIX_HAVE_PIPE2)
tb_bool_t tb_pipe_file_init_pair(tb_pipe_file_ref_t pair[2], tb_size_t buffer_size)
{
    // check
    tb_assert_and_check_return_val(pair, tb_false);

    tb_int_t  pipefd[2] = {0};
    tb_bool_t ok = tb_false;
    do
    {
        // create pipe fd pair
#ifdef TB_CONFIG_POSIX_HAVE_PIPE2
        if (pipe2(pipefd, O_NONBLOCK) == -1) break;
#else
        if (pipe(pipefd) == -1) break;

        // non-block
        fcntl(pipefd[0], F_SETFL, fcntl(pipefd[0], F_GETFL) | O_NONBLOCK);
        fcntl(pipefd[1], F_SETFL, fcntl(pipefd[1], F_GETFL) | O_NONBLOCK);
#endif

        // save to file pair
        pair[0] = tb_fd2pipefile(pipefd[0]);
        pair[1] = tb_fd2pipefile(pipefd[1]);
        
        // ok
        ok = tb_true;

    } while (0);
    return ok;
}
#else
tb_bool_t tb_pipe_file_init_pair(tb_pipe_file_ref_t pair[2], tb_size_t buffer_size)
{
    tb_trace_noimpl();
    return tb_false;
}
#endif
tb_bool_t tb_pipe_file_exit(tb_pipe_file_ref_t file)
{
    // check
    tb_assert_and_check_return_val(file, tb_false);

    // trace
    tb_trace_d("close: %p", file);

    // close it
    tb_bool_t ok = !close(tb_pipefile2fd(file));
    if (!ok)
    {
        // trace
        tb_trace_e("close: %p failed, errno: %d", file, errno);
    }
    return ok;
}
tb_long_t tb_pipe_file_read(tb_pipe_file_ref_t file, tb_byte_t* data, tb_size_t size)
{
    // check
    tb_assert_and_check_return_val(file && data, -1);
    tb_check_return_val(size, 0);

    // read
    return read(tb_pipefile2fd(file), data, (tb_int_t)size);
}
tb_long_t tb_pipe_file_writ(tb_pipe_file_ref_t file, tb_byte_t const* data, tb_size_t size)
{
    // check
    tb_assert_and_check_return_val(file && data, -1);
    tb_check_return_val(size, 0);

    // write
    return write(tb_pipefile2fd(file), data, (tb_int_t)size);
}
