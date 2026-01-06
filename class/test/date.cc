#include "date.h"

date::date(size_t year, size_t month, size_t day)
{
  if(year < 0)
  {
    std::cerr<< "year less 0" <<std::endl;
  }
}


