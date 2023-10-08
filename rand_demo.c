#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>


int main()
{

	long t;
	int h, m, s, w;
	int totall_days,days=0,month=0;
	int i=0,j=0;
	int now_years=1970,now_days;
	

	const char* weekday[] = {"日", "一", "二", "三", "四", "五", "六"};

	        t = time(NULL);
                totall_days=t/(24*60*60);
                //printf("当前时间戳：%ld\n", t);

                for(i=0;i<totall_days;)
                {
                        ++j;
                        if(j % 4 == 0 && j  % 100 != 0 || j % 400 == 0)
                        {
                                i+=365;
                        }
                        else
                        {

                                i+=366;

                        }
                        now_years=now_years+1;
                }
                now_days=totall_days-i;
		printf("%d\n",now_days);
                while(days<now_days)
                {
                        days++;
                        if(month=2)
                        {
                                if(j % 4 == 0 && j % 100 != 0 || j % 400 == 0)
                                {
                                        if(days=30)days=1;
                                        else if(days=29)days=1;
                                        month++;

                                }
                        }
                        if(month = 4 || month == 6 || month == 10 || month == 12)
                        {
                                if(days=31)days=1;month++;

                        }
                        else
                        {
                                if(days=32)days=1;month++;
                        }
                        if(month=12) month=0;
                }
		
                while(1)
       {
		

		h = (t / (60 * 60) + 8) % 24;        // 计算小时
		m = t / 60 % 60;                     // 计算分钟
		s = t % 60;                          // 计算秒数
		w = (t / (24 * 60 * 60) + 4) % 7;    // 计算星期
                

		printf("\r%2d年%2d月%2d:%02d:%02d 星期%s",now_years,month, h, m, s, weekday[w]);
		fflush(stdout);  // 刷新输出缓冲区

		sleep(1);  // 休眠 1 秒
	}


/*	int i, j;

	srand(time(NULL));

	for(j = 0; j < 5; j++)
	{
		i = rand();
		printf("%d\n", i);
	}
	return 0;
*/
}
