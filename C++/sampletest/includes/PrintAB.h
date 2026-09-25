#ifndef PRIANTAB_H
#define PRIANTAB_H

class Print{
public:

    void printA();

    void printB();

    void shutdown();

    char getData();

private:
    std::mutex m_mutex;
    std::condition_variable m_cv;
    char m_data{'b'};
    std::atomic_bool m_shutdown{false};
};


#endif