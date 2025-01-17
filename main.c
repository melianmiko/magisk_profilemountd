#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mount.h>

#define ROOT_PATH "/data/media"
#define SOURCE_PATH "/data/media/0"
#define TARGET_DIR_NAME "PrimaryFS"
#define LOG_FILE "/data/adb/modules/profilemountd/service.log"
#define SLEEP_DELAY 5

int mountState[128];
FILE* logFile;

void joinPath(char* dst, char* child) {
    strcat(dst, "/");
    strcat(dst, child);
}

int loop() {
    DIR* baseDir;
    char path[128];
    int uid = 0;
    int ret = 0;
    struct dirent* dp;
    struct stat statBuf;

    baseDir = opendir(ROOT_PATH);
    if(baseDir == 0) {
        fprintf(logFile, "[!!] Root directory seems to be not ready, delay...\n");
        return 0;
    }

    while((dp = readdir(baseDir)) != 0) {
        if(dp->d_name[0] == '.') {
            continue;
        }

        uid = atoi(dp->d_name);
        if(uid == 0) {
            continue;
        }

        strcpy(path, ROOT_PATH);
        joinPath(path, dp->d_name);
        joinPath(path, TARGET_DIR_NAME);
        ret = stat(path, &statBuf) == 0 && S_ISDIR(statBuf.st_mode);

        if(ret && !mountState[uid]) {
            fprintf(logFile, "[==] Mounting %s over %s... ", SOURCE_PATH, path);
            if((ret = mount(SOURCE_PATH, path, 0, MS_BIND, 0)) == 0) {
                fprintf(logFile, "success\n");
                mountState[uid] = 1;
            } else {
                fprintf(logFile, "failed with %d\n", ret);
                mountState[uid] = 0;
            }
        } else if(!ret && mountState[uid]) {
            fprintf(logFile, "[==] Umounting %s... ", path);
            if((ret = umount(path)) == 0) {
                fprintf(logFile, "success\n");
            } else {
                fprintf(logFile, "failed with %d\n", ret);
            }
            mountState[uid] = 0;
        }
    }

    (void)closedir(baseDir);
    return 0;
}

int main() {
    int ret = 0;

    logFile = fopen(LOG_FILE, "w");

    fprintf(logFile, "[--] Service started\n");

    while(1) {
        if((ret = loop()) != 0) {
            fprintf(logFile, "[!!] Exiting with error code %d\n", ret);
            return ret;
        }

        fflush(logFile);
        sleep(SLEEP_DELAY);
    }

    fprintf(logFile, "[!!] Loop exited, which is impossible\n");

    (void)fclose(logFile);

    return 99;
}
