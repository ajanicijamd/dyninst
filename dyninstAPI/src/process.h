/*
 *  Copyright 1993 Jeff Hollingsworth.  All rights reserved.
 *
 */

/*
 * process.h - interface to manage a process in execution. A process is a kernel
 *   visible unit with a seperate code and data space.  It might not be
 *   the only unit running the code, but it is only one changed when
 *   ptrace updates are applied to the text space.
 *
 * $Log: process.h,v $
 * Revision 1.1  1994/01/27 20:31:39  hollings
 * Iinital version of paradynd speaking dynRPC igend protocol.
 *
 * Revision 1.6  1993/08/11  01:46:24  hollings
 * added defs for baseMap.
 *
 * Revision 1.5  1993/07/13  18:29:49  hollings
 * new include file syntax.
 *
 * Revision 1.4  1993/06/25  22:23:28  hollings
 * added parent field to process.h
 *
 * Revision 1.3  1993/06/22  19:00:01  hollings
 * global inst state.
 *
 * Revision 1.2  1993/06/08  20:14:34  hollings
 * state prior to bc net ptrace replacement.
 *
 * Revision 1.1  1993/03/19  22:51:05  hollings
 * Initial revision
 *
 *
 */

#ifndef PROCESS_H
#define PROCESS_H

#include "util/h/list.h"
#include "rtinst/h/rtinst.h"
#include "dyninst.h"

typedef struct processRec process;

typedef enum { running, neonatal, stopped, exited } processState;

typedef struct freeListRec freeListEntry;

typedef enum { HEAPfree, HEAPallocated } heapStatus;

struct freeListRec {
    int addr;
    int length;
    heapStatus status;
    freeListEntry *next;
}; 

struct processRec {
    image *symbols;		/* information related to the process */
    int traceLink;		/* pipe to transfer traces data over */
    processState status;	/* running, stopped, etc. */
    int pid;			/* id of this process */
    int thread;			/* thread id for thread */
    Boolean aggregate;		/* is this process a pseudo process ??? */
    freeListEntry *heap;	/* inferior heap */
    resource rid;		/* handle to resource for this process */
    process *parent;		/* parent of this proces */
    List<void *> baseMap;	/* map and inst point to its base tramp */
};

extern List<process*> processList;

process *createProcess(char *file, char *argv[]);
process *allocateProcess(int pid, char *name);
process *findProcess(int pid);
void initInferiorHeap(process *proc, Boolean globalHeap);
void copyInferriorHeap(process *from, process *to);

int inferriorMalloc(process *proc, int size);
void inferriorFree(process *proc, int pointer);

#endif
