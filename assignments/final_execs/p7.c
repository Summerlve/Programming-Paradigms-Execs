#include "../../16/thread_107/thread_107.h"
#include <stdio.h>
#include <stdlib.h>
#define __APPLE__

int DownloadMediaFile(const char *server, const char *file)
{
    // TODO http/https request to server/file using c.
    // TODO count size of downloaded content in bytes. 

    return 0;
}

void *DownloadMediaFileAdapter(void *args)
{
    const char *server = ((char **)args)[0];
    const char *file = ((char **)args)[1];
    int *filesSizeSumPointer = ((int **)args)[2];
    Semaphore filesSizeSumLock = *(((Semaphore **)args)[3]);
    Semaphore maxConnections = *(((Semaphore **)args)[4]);
    Semaphore filesCounterLock = *(((Semaphore **)args)[5]);

    printf("%s %s\n", server, file);
    SemaphoreWait(maxConnections);

    int fileSize = DownloadMediaFile(server, file);

    SemaphoreWait(filesSizeSumLock);
    *filesSizeSumPointer += fileSize;
    SemaphoreSignal(filesSizeSumLock);

    SemaphoreSignal(filesCounterLock);
    SemaphoreSignal(maxConnections);
}

int DownloadMediaLibrary(const char *server, const char *files[], int numFiles)
{
    int filesSizeSum = 0;
    Semaphore maxConnectionsLock = SemaphoreNew("maxConnectionsLock", 12);
    Semaphore filesCounterLock = SemaphoreNew("filesLock", 0);
    Semaphore filesSizeSumLock = SemaphoreNew("filesSizeSumLock", 1);

    for (int i = 0; i < numFiles; i++)
    {
        ThreadNew("DownloadMediaFileAdapter", DownloadMediaFileAdapter, 5, &server, &(files[i]),
                                                                            &filesSizeSum, &filesSizeSumLock,
                                                                            &maxConnectionsLock, &filesCounterLock);
    }

    RunAllThreads();

    for (int i = 0; i < numFiles; i++)
    {
        SemaphoreWait(filesCounterLock);
    }

    return filesSizeSum;
}

int main(int argc, char **argv)
{
    InitThreadPackage(false);

    char server[] = "www.google.com";
    const char *files[] = {
        "file1",
        "file2"
    };

    int files_size_sum = DownloadMediaFile(server, files); 
    fprintf("files size sum: %d", files_size_sum);
    return 0;
}