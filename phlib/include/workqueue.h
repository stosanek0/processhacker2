#ifndef _PH_WORKQUEUE_H
#define _PH_WORKQUEUE_H

#if defined(DEBUG)
extern PPH_LIST PhDbgWorkQueueList;
extern PH_QUEUED_LOCK PhDbgWorkQueueListLock;
#endif

typedef struct _PH_WORK_QUEUE
{
    PH_RUNDOWN_PROTECT RundownProtect;
    BOOLEAN Terminating;

    LIST_ENTRY QueueListHead;
    PH_QUEUED_LOCK QueueLock;
    PH_CONDITION QueueEmptyCondition;

    ULONG MaximumThreads;
    ULONG MinimumThreads;
    ULONG NoWorkTimeout;

    PH_QUEUED_LOCK StateLock;
    HANDLE SemaphoreHandle;
    ULONG CurrentThreads;
    ULONG BusyCount;
} PH_WORK_QUEUE, *PPH_WORK_QUEUE;

typedef VOID (NTAPI *PPH_WORK_QUEUE_ITEM_DELETE_FUNCTION)(
    _In_ PUSER_THREAD_START_ROUTINE Function,
    _In_ PVOID Context
    );

typedef struct _PH_WORK_QUEUE_ITEM
{
    LIST_ENTRY ListEntry;
    PUSER_THREAD_START_ROUTINE Function;
    PVOID Context;
    PPH_WORK_QUEUE_ITEM_DELETE_FUNCTION DeleteFunction;
} PH_WORK_QUEUE_ITEM, *PPH_WORK_QUEUE_ITEM;

PHLIBAPI
VOID
NTAPI
PhInitializeWorkQueue(
    _Out_ PPH_WORK_QUEUE WorkQueue,
    _In_ ULONG MinimumThreads,
    _In_ ULONG MaximumThreads,
    _In_ ULONG NoWorkTimeout
    );

PHLIBAPI
VOID
NTAPI
PhDeleteWorkQueue(
    _Inout_ PPH_WORK_QUEUE WorkQueue
    );

PHLIBAPI
VOID
NTAPI
PhWaitForWorkQueue(
    _Inout_ PPH_WORK_QUEUE WorkQueue
    );

PHLIBAPI
VOID
NTAPI
PhQueueItemWorkQueue(
    _Inout_ PPH_WORK_QUEUE WorkQueue,
    _In_ PUSER_THREAD_START_ROUTINE Function,
    _In_opt_ PVOID Context
    );

VOID
PhQueueItemWorkQueueEx(
    _Inout_ PPH_WORK_QUEUE WorkQueue,
    _In_ PUSER_THREAD_START_ROUTINE Function,
    _In_opt_ PVOID Context,
    _In_opt_ PPH_WORK_QUEUE_ITEM_DELETE_FUNCTION DeleteFunction
    );

PHLIBAPI
VOID
NTAPI
PhQueueItemGlobalWorkQueue(
    _In_ PUSER_THREAD_START_ROUTINE Function,
    _In_opt_ PVOID Context
    );

#endif
