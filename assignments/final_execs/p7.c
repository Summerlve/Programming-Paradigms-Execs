#include "../../16/thread_107/thread_107.h"
#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h> // include curl for http request 
#define __APPLE__

int DownloadMediaFile(const char *server, const char *file)
{
    // TODO http/https request to server/file using c.
    CURL *curl;
    CURLcode res;
    
    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "https://example.com");
        /* example.com is redirected, so we tell libcurl to follow redirection */ 
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    
        /* Perform the request, res will get the return code */ 
        res = curl_easy_perform(curl);
        /* Check for errors */ 
        if(res != CURLE_OK)
        fprintf(stderr, "curl_easy_perform() failed: %s\n",
                curl_easy_strerror(res));
    
        /* always cleanup */ 
        curl_easy_cleanup(curl);
    }
    
    // TODO count size of downloaded content in bytes. 
    return 0;
}

void *DownloadMediaFileAdapter(void *args)
{
    const char *debugName = ((char **)args)[0]; 
    const char *server = ((char **)args)[1];
    const char *file = ((char **)args)[2];

    int *sum = ((int **)args)[3];
    Semaphore calculateSum = *(((Semaphore **)args)[4]);
    Semaphore maxConnections = *(((Semaphore **)args)[5]);
    Semaphore done = *(((Semaphore **)args)[6]);

    SemaphoreWait(maxConnections);
    int fileSize = DownloadMediaFile(server, file);
    SemaphoreSignal(maxConnections);

    SemaphoreWait(calculateSum);
    *sum += fileSize;
    SemaphoreSignal(calculateSum);

    SemaphoreSignal(done);
}

int DownloadMediaLibrary(const char *server, const char *files[], int numFiles)
{
    int sum = 0;
    Semaphore maxConnections = SemaphoreNew("maxConnections", 12);
    Semaphore done = SemaphoreNew("done", 0);
    Semaphore calculateSum = SemaphoreNew("calculateSum", 1);

    for (int i = 0; i < numFiles; i++)
    {
        char str[30];
        sprintf(str, "DownloadMediaFileAdapter_%d", i + 1);
        ThreadNew(str, DownloadMediaFileAdapter, 6, server, files[i], &sum,
                                                        &calculateSum, &maxConnections, &done);
    }

    RunAllThreads();

    for (int i = 0; i < numFiles; i++)
    {
        SemaphoreWait(done);
    }

    return sum;
}

int main(int argc, char **argv)
{
    InitThreadPackage(false);

    char server[] = "www.google.com";
    const char *files[] = {
        "file1",
        "file2"
    };

    int files_size_sum = DownloadMediaLibrary(server, files, 2); 
    fprintf(stdout, "files size sum: %d\n", files_size_sum);

    exit(EXIT_SUCCESS);
}