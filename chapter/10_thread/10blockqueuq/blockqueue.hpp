#include <pthread.h>
#include <queue>

template<class T>
class blockqueue
{
private:
  std::queue<T> _q;
  size_t _maxcap;
  pthread_t _mutex;
  pthread_cond_t _pcond;
  pthread_cond_t _ccond;

public:
  blockqueue(int maxcap)
    :_maxcap(maxcap)
  {
    pthread_mutex_init(&_mutex, nullptr);
    pthread_cond_init(&_pcond, nullptr);
    pthread_cond_init(&_ccond, nullptr);
  }

  ~blockqueue()
  {
    pthread_mutex_destroy(&_mutex);
    pthread_cond_destroy(&_pcond);
    pthread_cond_destroy(&_ccond);
  }

private:
  bool is_empty()
  {
    return _q.empty();
  }

  bool is_full()
  {
    return _q.size() == _maxcap;
  }

public:
  void push(const T& in)
  {
    pthread_mutex_lock(&_mutex); 
    while(is_full())
    {
      pthread_cond_wait(&_pcond, &_mutex);
    }

    _q.push(in);
    pthread_cond_signal(&_ccond);
    pthread_mutex_unlock(&_mutex);
  }

  void pop(T* out)
  {
    pthread_mutex_lock(&_mutex);
    while(is_empty())
    {
      pthread_cond_wait(&_ccond, &_mutex);
    }

    *out = _q.front();
    _q.pop();

    pthread_cond_signal(&_pcond);
    pthread_mutex_unlock(&_mutex);
  }
};
