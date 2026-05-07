#include <iostream>
#include <sys/wait.h>
#include <sys/types.h>
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
int reveTask(int readFd);

#define PROCESS_NUM 5
#define Make_Seed() srand((unsigned long)time(nullptr)^getpid()^0x1112^rand()%1234)

typedef void(*func_t)(); // 函数指针 类型

void downLoadTask()
{
  cout<< getpid()<<":下载任务" <<endl;
}

void ioTask()
{
  cout<< getpid() <<":io任务" <<endl;
}

void flushTask()
{
  cout<<getpid() <<":刷新任务"<<endl;

}

// 1.放任务
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
    snprintf(namebuffer, sizeof(namebuffer), "process-%d[(pid)%d-(fd)%d]", num++, subId_, writeFd_);
    name_ = namebuffer;
  }
public: 
  static int num;
  string name_;
  pid_t subId_;
  int writeFd_;
};

int SubEd::num = 0;


// 2.创建子进程，子进程的信息放在subs里面的
void cteateSubProcess(vector<SubEd>* subs, vector<func_t>& funcMap)
{
  vector<int> deletefd;
  for(int i = 0; i < PROCESS_NUM; i++)
  {
    int fds[2];
    int n = pipe(fds);
    assert(n == 0);
    (void)n;
    // bug 
    // 父进程打开的文件，是会被子进程共享的
    // 关闭前面的进程的管道描述符 

    pid_t id = fork();

    if(id == 0)
    {
      for(size_t i = 0; i < deletefd.size(); i++)
      {
        close(deletefd[i]);
      }

      // 处理任务,子进程赋值读取父进程发送的执行信息。
      // 读端会阻塞，等父进程发送信息的。
      close(fds[1]); // 关闭写端
      
      // 1接收命令码，没有就阻塞。
      while(true)
      {
        int commandcode = reveTask(fds[0]);
        if(commandcode >= 0 && commandcode < (int)funcMap.size()) // size_t类型和int类型的
        {
          funcMap[commandcode]();
        }
        else if(commandcode == -1) 
        {
          break;
        }
      }
      exit(0);
    }

    close(fds[0]);
    SubEd sub(id, fds[1]);
    subs->push_back(sub);
    deletefd.push_back(fds[1]);
  }
}

int reveTask(int readFd)
{
  int code = 0;
  ssize_t s = read(readFd, &code, sizeof(code));

  if(s == 4) return code;
  else if(s <= 0) return -1;
  else return 0;
}

void sendTask(const SubEd& process, int tasknum)
{
  cout<< "send tasknum:" << tasknum << " send to " << process.name_ <<endl;
  int n = write(process.writeFd_, &tasknum, sizeof(tasknum));
  assert(n == sizeof(int));
  (void)n;
}

//3.发送任务信息
void loadBalanceContrl(vector<SubEd>& subs, vector<func_t>& funcMap, int count)
{
  // 父进程控制子进程
  int processnum = subs.size();
  int tasknum = funcMap.size();
  
  bool forever = (count == 0 ? true : false);

  while(true)
  {
    // 1选择一个子进程-----vector 
    int subIdx = rand() % processnum; 

    // 2选择一个任务-------
    int taskIdx = rand() % tasknum;

    // 3任务发送给选择的进程
    sendTask(subs[subIdx], taskIdx);
    sleep(2);
    
    if(!forever)
    {
      count--;
      if(count == 0) break;
    }
  }
  // write quit---》read 0
  for(int i = 0; i < processnum; i++)
  {
    close(subs[i].writeFd_);
  }
}

void waitProcess(vector<SubEd> process)
{
  int processnum = process.size();
  for(int i = 0; i < processnum; i++)
  {
    waitpid(process[i].subId_, NULL,0);
    cout<< "wait sub process success ..." << process[i].subId_ <<endl;
  }
}

int main()
{
  // 随机数种子的
  Make_Seed(); 
  // 创建一批子进程
  // 子进程id和写入的fd
  
  vector<SubEd> subs; // 先描述，后组织起来,子进程的信息
  vector<func_t> funcMap;  // 进程的函数,执行的函数

  loadTaskFunc(&funcMap);               // 添加函数任务
  cteateSubProcess(&subs, funcMap);  // 创建子进程的信息

  int taskCnt = 3; // cnt == -1 

  // loadBal
  loadBalanceContrl(subs, funcMap, taskCnt); // 分发任务信息的

  // 回收子进程
  waitProcess(subs);
  return 0;
}


