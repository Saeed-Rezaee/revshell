////////////////////////////////////////////////////////////////////////////////
// core.h
// author: jcramb@gmail.com

#ifndef core_h
#define core_h

////////////////////////////////////////////////////////////////////////////////
// types / defines

#define SSL_CRTFILE "/root/revshell/vterm/ca/shell_crt.pem"
#define SSL_KEYFILE "/root/revshell/vterm/ca/shell_key.pem"

#define TPT_CLIENT 1
#define TPT_SERVER 2
#define TPT_CLOSE  0
#define TPT_EMPTY -1
#define TPT_ERROR -2

#define LOG_BUFSIZE 8192
#define LOG_FILE (1<<1)
#define LOG_ECHO (1<<2)

#define MSG_INVALID 0xFFFFFFFF
#define MSG_TTYKEYS 0xBEBAFEBA
#define MSG_WNDSIZE 0x10101010

#ifndef MAX
#define MAX(a, b) (a > b ? a : b)
#endif

#ifndef MIN
#define MIN(a, b) (a < b ? a : b)
#endif

#ifndef CLAMP
#define CLAMP(val, min, max) MAX(min, (MIN(max, val))
#endif

#ifndef LOG_DISABLE
#define LOG(...) log_print(__VA_ARGS__)
#else
#define LOG(...) 
#endif

// type proto
class transport;
class message;

// func proto
void log_init(const char * prefix, int flags);
void log_flags(int flags = 0);
void log_print(const char * fmt, ...);
void hexdump(const char * buf, int len, int cols = 16, bool ascii = true); 

////////////////////////////////////////////////////////////////////////////////
// abstract transport interface

class transport {
public:

    // ctors / dtors
    transport() {}
    virtual ~transport() {}

    // abstract interface
    virtual int init(int type) = 0;
    virtual int send(message & msg) = 0;
    virtual int recv(message & msg) = 0;
};

////////////////////////////////////////////////////////////////////////////////
// transport message container

class message {
public:

    // class 
    enum { header_len = sizeof(int) * 2 }; 
    enum { body_max_len = 1024 }; 

    // ctors / dtors
    message(int type = MSG_INVALID, size_t len = 0);
    message(int type, const char * buf, size_t len);
    message(const char * buf, size_t len);

    // buffer getters
    char * data();
    char * body();
    const char * data() const;
    const char * body() const;
    size_t data_len() const;
    size_t body_len() const;

    // misc getters / setters
    int type() const;
    size_t resize(size_t len);

protected:

    // internal data struct
    struct {
        int type;
        int body_len;
        char body[body_max_len];
    } m_data;
};

////////////////////////////////////////////////////////////////////////////////

#endif // core_h
