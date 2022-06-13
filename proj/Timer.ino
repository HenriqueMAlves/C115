

bool timer(TIMER* timer, unsigned int delayMs)
{  
    switch(timer->time_flag)
    {
      case false:
        timer->time_start = millis();
        timer->time_end = timer->time_start + delayMs;
        timer->time_flag = true;
      break;

      case true:
        timer->time_start = millis();
        if(timer->time_start >= timer->time_end)
        {
          timer->time_flag = false;
          return true;
        }
      break;
    }

    return false;
}
