#include "Kalman.h"

/*
**卡尔曼滤波
**R+Q=1,R越大滤波效果越好,迟滞性越强；Q越大跟随性越好
*/
u32 f_kalman_fliter(u32 z_measure)
{
        double  kg;
        double  x_mid;
        double  x_now;
        double  p_mid;
        double  p_now;
        
        static double Q=0.001;
        static double  R=0.999;
        static double x_last=3900;
        static double p_last=0.0946;
        
        x_mid=x_last;
        p_mid=p_last+Q;
        kg=p_mid/(p_mid+R);
        x_now=x_mid+kg*(z_measure-x_mid);//估计最优值
        p_now=(1-kg)*p_mid;//最优值对应的协方差
        
        p_last=p_now;//更新状态值
        x_last=x_now;
        
        return (u32)x_now;
}

