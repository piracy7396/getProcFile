#include <stdio.h>
#include <jni.h>
#include <string>
#include <fstream>
#include <unistd.h>
#include <sys/syscall.h>
#include <android/log.h>
#include <bits/glibc-syscalls.h>
#include <linux/fcntl.h>



/* file flags fcntl.h */



#define IN_MESSAGE_LEN 20
#define OUT_MESSAGE_LEN 9

extern "C" int my_openat(int, const char*, int, int);
extern "C" int my_read(int, void*, int);


char inBuffer[IN_MESSAGE_LEN];


void killProcess();
int getProcFile();
int getProcFile2();

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_myapplication_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {


    int result = getProcFile2();

    //killProcess();
    // exit(0);

    const char* hello = "Hello from C++";

    return env->NewStringUTF(hello);
}


void killProcess(){
    pid_t tid;
    tid = syscall(SYS_gettid);
    syscall(SYS_tgkill, getpid(), tid, SIGHUP);
}


int getProcFile(){
    int inFileDesc;

    // Apertura fine di input

    //inFileDesc = syscall(SYS_OPEN, "/sdcard/testinput.txt", O_RDONLY,0);  4.4
    inFileDesc = syscall(__NR_openat, AT_FDCWD,"/sdcard/testinput.txt", O_RDONLY,0);

    if(inFileDesc == -1) {
        __android_log_print(ANDROID_LOG_VERBOSE,"piracy","error-->%s",strerror(errno));

        char str[] = "Unable to open \"testinput.txt\"\n";
//        syscall(SYS_WRITE, FILE_DESC_STDOUT, str, sizeof(str)/sizeof(char));
//        syscall(SYS_EXIT, 1);
        return 1;
    }
    else {
        __android_log_print(ANDROID_LOG_VERBOSE,"piracy","fd-->%d",inFileDesc);

    }

    syscall(__NR_read, inFileDesc, inBuffer, IN_MESSAGE_LEN);
    __android_log_print(ANDROID_LOG_VERBOSE,"piracy","输出值%s",inBuffer);
//    syscall(__NR_close, inFileDesc);
//    syscall(__NR_exit, 0);
    return 0;
}
int getProcFile2(){
    int inFileDesc;
    inFileDesc = my_openat(AT_FDCWD, "/sdcard/testinput.txt", O_RDONLY, 0);
    if(inFileDesc == -1) {
        __android_log_print(ANDROID_LOG_VERBOSE,"piracy","error-->%s",strerror(errno));

        char str[] = "Unable to open \"testinput.txt\"\n";
//        syscall(SYS_WRITE, FILE_DESC_STDOUT, str, sizeof(str)/sizeof(char));
//        syscall(SYS_EXIT, 1);
        return 1;
    }
    else {
        __android_log_print(ANDROID_LOG_VERBOSE,"piracy","fd-->%d",inFileDesc);

    }
    return 0;
}

extern "C" long __set_errno_internal(int n) {
    errno = n;
    return -1;
}
