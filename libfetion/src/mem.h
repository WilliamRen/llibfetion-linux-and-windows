/***************************************************************************
 *   Copyright (C) 2010 by programmeboy                                    *
 *   programmeboy@gmail.com                                                *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.                                        *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             * 
 ***************************************************************************/

#ifndef MEM_H_INCLUDED
#define MEM_H_INCLUDED

typedef struct _mem_struct
{
    size_t size;
    char* mem_ptr;
	
}MEM_STRUCT, *PMEM_STRUCT;

void* myrealloc(void *ptr, size_t size);
size_t write_mem_call_back(void *ptr, size_t size, \
                            size_t nmemb, void *data);
void myfree( PMEM_STRUCT mem );

#endif // MEM_H_INCLUDED
