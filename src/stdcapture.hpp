#ifdef _MSC_VER
#include <io.h>
#define popen _popen 
#define pclose _pclose
#define stat _stat 
#define dup _dup
#define dup2 _dup2
#define close _close
#define pipe _pipe
#define read _read
#else
#include <unistd.h>
#endif
#include <fcntl.h>
#include <stdio.h>
#include <mutex>
#include <chrono>
#include <thread>

#ifndef STD_OUT_FD 
#define STD_OUT_FD (_fileno(stdout)) 
#endif 

#ifndef STD_ERR_FD 
#define STD_ERR_FD (_fileno(stderr)) 
#endif

class StdCapture
{
public:

    StdCapture();

    void BeginCapture();
    bool IsCapturing();
    bool EndCapture();
    std::string GetCapture();

private:
    enum PIPES { READ, WRITE };
    
    int secure_dup(int src);
    void secure_pipe(int * pipes);
    void secure_dup2(int src, int dest);
    void secure_close(int & fd);

    int m_pipe[2];
    int m_oldStdOut;
    int m_oldStdErr;
    bool m_capturing;
    std::mutex m_mutex;
    std::string m_captured;
};