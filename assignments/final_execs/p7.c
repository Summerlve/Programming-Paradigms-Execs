#include <stdio.h>
#include <stdlib.h>
#include "../../16/thread_107/thread_107.h"
#define __APPLE__

int DownloadMediaFile(const char *server, const char *file)
{
    // TODO http/https request to server/file using c.

    // TODO count size of downloaded content in bytes. 

    return 0;
}

int DownloadMediaLibrary(const char *server, const char *files[], int numFiles)
{
    int files_size_sum = 0;
    Semaphore max_connections = SemaphoreNew("max_connections", 12);
    Semaphore files = SemaphoreNew("files", 0);

    for (int i = 0; i < numFiles; i++)
    {
        ThreadNew("DownloadMediaFile", DownloadMediaFile, 2, server, files[i]);
    }

    for (int i = 0; i < numFiles; i++)
    {
        SemaphoreWait(files);
    }

    return files_size_sum;
}

int main(int argc, char **argv)
{
    InitThreadPackage(false);

    char server[] = "www.google.com";
    const char *files[] = {
        "file1",
        "file2"
    };

    RunAllThreads(); 
    int files_size_sum = DownloadMediaFile(server, files); 

    return 0;
}