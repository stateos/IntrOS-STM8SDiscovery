#include "led.h"
#include "os.h"

OS_SEM(sem, 0, semBinary);

OS_TSK_DEF(sla)
{
	sem_wait(sem);
	led_toggle();
}

OS_TSK_DEF(mas)
{
	tsk_delay(SEC);
	sem_give(sem);
}

void main()
{
	led_init();
	sys_init();
	tsk_start(sla);
	tsk_start(mas);
	tsk_stop();
}
