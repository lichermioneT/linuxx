#include <iostream>
#include <vector>
#include <cstdlib>
#include <unistd.h>
#include <cassert>
#include <string>
#include <sys/types.h>
#include <functional>
#include <ctime>
#include <unistd.h>

using namespace std;

#define PROCESS_NUM 5
#define Make_Seed() srand((unsigned long)time(nullptr)^getpid()^0x1112^rand()%1234)

typedef void(*func_t)(); // 函数指针 类型

void downLoadTask()
{
  cout<< getpid()<<":下载任务" <<endl;
}

void ioTask()
{
  cout<< getpt() <<":io任务" <<endl;
}

void flushTask()
{
  cout<<getpt() <<":刷新任务"<<endl;
}

void loadTaskFunc(vector<func_t>* out)
{
  assert(out);
  out->push_back(downLoadTask);
  out->push_back(ioTask);
  out->push_back(flushTask);
}



class SubEd
{
public:
  SubEd(pid_t subId, int writeFd)
    :subId_(subId)
    ,writeFd_(writeFd)
  {
    char namebuffer[1024];
    snprintf(namebuffer, sizeof(namebuffer), "process-%d[(pid)%d-(fd)%d]", num++,subId_, writeFd_);
    name_ = namebuffer;
  }
public: 
  static int num;
  string name_;
  pid_t subId_;
  int writeFd_;
};

int SubEd::num = 0;

int reveTask(int readFd)
{
  int code = 0;
  ssize_t s = read(readFd, &code, sizeof(code));
  assert(s == sizeof(int));
  return code;
}


void cteateSubProcess(vector<SubEd>* subs, vector<func_t>& funcMap)
{
  for(int i = 0; i < PROCESS_NUM; i++)
  {
    int fds[2];
    int n = pipe(fds);
    assert(n == 0);
    (void)n;

    pid_t id = fork();

    if(id == 0)
    {
      // 处理任务
      close(fds[1]);
      
      // 1接收命令码，没有就阻塞。
      while(true)
      {
       int commandcode = reveTask(fds[0]);
       if(commandcode >= 0 && commandcode < funcMap.size())
       {
         funcMap[commandcode]();
       }
       else 
       {
         cout<< "sub rece code error!"<<endl;
       }
      }

      exit(0);
    }

    close(fds[0]);
    SubEd sub(id, fds[1]);
   // (*subs).push_back(sub);
    subs->push_back(sub);
  }
    
}


void sendTask(const SubEd& process, int tasknum)
{
  cout<< "send tasknum:" << tasknum << " send to " << process.name_ <<endl;
  int n = write(process.writeFd_, &tasknum, sizeof(tasknum));
  assert(n == sizeof(int));
  (void)n;
}


int main()
{
  Make_Seed(); 
  // 创建一批子进程
  // 子进程id和写入的fd
  
  vector<SubEd> subs; // 先描述，后组织起来
  vector<func_t> funcMap; 
  cteateSubProcess(&subs, funcMap);
  loadTaskFunc(&funcMap); 

  // loadBal
  // 父进程控制子进程
  int processnum = subs.size();
  int tasknum = funcMap.size();
  bool quit = false;
  while(!quit)
  {
    // 1选择一个子进程-----vector 
    int subIdx = rand() % processnum; 

    // 2选择一个任务-------
    int taskIdx = rand() % tasknum;

    // 3任务发送给选择的进程
    sendTask(subs[subIdx], taskIdx);

    sleep(2);

  }





  // 回收子进程
  
  










  return 0;
}


