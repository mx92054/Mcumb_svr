#ifndef __PLANT_H__
#define __PLANT_H__

typedef struct _plant_block
{
    float angle;      //当前方位角
    float angle_pre;  //上次方位角
    float dangle;     //当前角速度
    float dangle_pre; //上次角速度

    float ddangle; //角加速度
    float ts;      //采样时间
} plant_block;

extern plant_block plant ;

void plant_init(plant_block *pt);
void plant_step(plant_block *pt, float force);
float map_angle2resistance(float force) ;

#endif

/*-----------------------------------end of file--------*/
