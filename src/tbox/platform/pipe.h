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
 * @file        pipe.h
 * @ingroup     platform
 *
 */
#ifndef TB_PLATFORM_PIPE_H
#define TB_PLATFORM_PIPE_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "prefix.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_enter__

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */

/// the pipe file ref type
typedef __tb_typeref__(pipe_file);

/* //////////////////////////////////////////////////////////////////////////////////////
 * interfaces
 */

/*! init the file of named pipe
 *
 * @note the pipe files will be inherited in child process.
 *
 * @param name          the pipe name
 * @param mode          the file mode, only support TB_FILE_MODE_RO/TB_FILE_MODE_WO
 * @param buffer_size   the buffer size of pipe, it will use the default size if pass zero
 * 
 * @return              the pipe file
 */
tb_pipe_file_ref_t      tb_pipe_file_init(tb_char_t const* name, tb_size_t mode, tb_size_t buffer_size);

/*! init the file pair of anonymous pipe
 *
 * @note the pipe files will be inherited in child process.
 *
 * @param pair          the pipe file pair, read: pair[0], write: pair[1] 
 * @param buffer_size   the buffer size of pipe, it will use the default size if pass zero
 * 
 * @return              tb_true or tb_false
 */
tb_bool_t               tb_pipe_file_init_pair(tb_pipe_file_ref_t pair[2], tb_size_t buffer_size);

/*! exit the pipe file
 * 
 * @param file          the pipe file
 *
 * @return              tb_true or tb_false
 */
tb_bool_t               tb_pipe_file_exit(tb_pipe_file_ref_t file);

/*! read the pipe file data (non-block)
 * 
 * @param file          the pipe file 
 * @param data          the data
 * @param size          the size
 *
 * @return              the real size or -1
 */
tb_long_t               tb_pipe_file_read(tb_pipe_file_ref_t file, tb_byte_t* data, tb_size_t size);

/*! writ the pipe file data (non-block)
 * 
 * @param file          the file 
 * @param data          the data
 * @param size          the size
 *
 * @return              the real size or -1
 */
tb_long_t               tb_pipe_file_writ(tb_pipe_file_ref_t file, tb_byte_t const* data, tb_size_t size);

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tb_extern_c_leave__

    
#endif
