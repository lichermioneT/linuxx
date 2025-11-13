#include <stdio.h>
#include <pthread.h>

struct Person
{
  int age;
  int scores;
  double weight;
};

void* print(Person* p)
{
  printf("%d \n", p->age);
  printf("%d \n", p->scores);
  printf("%lf \n", p->weight);
  return NULL;
}

Person p = {10, 20, 44.6};

int main()
{
  
  pthread_t td;
  pthread_create(&td, NULL,print, &p);

  pthread_join(td, NULL);




  return 0;
}
