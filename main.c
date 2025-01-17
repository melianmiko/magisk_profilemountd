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
#define SLEEP_DELAY 5

int mountState[128];

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
        printf("Can't open root directory, looks like it's bug.\n");
        return 1;
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
            printf("[==] Mounting %s over %s... ", SOURCE_PATH, path);
            if((ret = mount(SOURCE_PATH, path, 0, MS_BIND, 0)) == 0) {
                printf("success\n");
                mountState[uid] = 1;
            } else {
                printf("failed with %d\n", ret);
                mountState[uid] = 0;
            }
        } else if(!ret && mountState[uid]) {
            printf("[==] Umounting %s... ", path);
            if((ret = umount(path)) == 0) {
                printf("success\n");
                mountState[uid] = 1;
            } else {
                printf("failed with %d\n", ret);
                mountState[uid] = 0;
            }
        }
    }

    (void)closedir(baseDir);
    return 0;
}

int main() {
    int ret = 0;
    while(1) {
        if((ret = loop()) != 0) {
            return ret;
        }

        sleep(SLEEP_DELAY);
    }

    printf("[!!] Loop exited, which is impossible\n");
    return 99;
}
