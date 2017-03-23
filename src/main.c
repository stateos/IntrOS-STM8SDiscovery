#include <os.h>
#include <led.h>

OS_SEM(sem, 0);

void slave()
{
	sem_wait(sem);

	LED_Tick();
}

void master()
{
	tsk_delay(SEC);

	sem_give(sem);
}

OS_TSK(sla, slave);
OS_TSK(mas, master);

void main()
{
	LED_Init();

	sys_init();
	tsk_start(sla);
	tsk_start(mas);
	tsk_stop();
}
