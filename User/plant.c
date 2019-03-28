#include "stm32f10x.h"
#include "plant.h"
#include <string.h>

plant_block plant;
/*********************************************************
 *	@brief	目标初始化化
 *	@param	None
 *	@retval	None
 * ******************************************************/
void plant_init(plant_block *pt)
{
    memset(pt, 0, sizeof(plant_block));
    pt->ts = 0.1f; //采样时间为100ms
    pt->Jm = 4090.0f;
    pt->Lm = 3.08f;
}

/*********************************************************
 *	@brief	目标计算
 *	@param	None
 *	@retval	None
 * ******************************************************/
void plant_step(plant_block *pt, float force)
{
    float val1;
    float val2;

    val1 = pt->angle + pt->ts * pt->dangle;
    val2 = map_angle2resistance(pt->dangle);
    val2 += force * pt->Lm + 4265.0f * pt->dangle * pt->dangle - 13.2 * pt->dangle;
    val2 = pt->dangle + pt->ts * val2 / pt->Jm;

    pt->angle_pre = pt->angle;
    pt->dangle_pre = pt->dangle_pre;
    pt->angle = val1;
    pt->dangle = val2;
}

/*********************************************************
 *	@brief	水阻力计算
 *	@param	None
 *	@retval	None
 * ******************************************************/
float map_angle2resistance(float dangle)
{
    return dangle/1.5f;
}
