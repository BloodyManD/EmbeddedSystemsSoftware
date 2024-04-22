#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "includes.h"
#include "alt_ucosii_simple_error_check.h"
#include "altera_avalon_pio_regs.h"
#include "sys/alt_irq.h"

/* ����� ����� */
#define   TASK_STACKSIZE       1024
OS_STK    initialize_task_stk[TASK_STACKSIZE];
OS_STK    print_status_task_stk[TASK_STACKSIZE];
OS_STK    getsem_task1_stk[TASK_STACKSIZE];
OS_STK    getsem_task2_stk[TASK_STACKSIZE];
OS_STK    receive_task1_stk[TASK_STACKSIZE];
OS_STK    receive_task2_stk[TASK_STACKSIZE];
OS_STK    send_task_stk[TASK_STACKSIZE];
OS_STK    handle_auto_period_task_stk[TASK_STACKSIZE];
OS_STK	  useful_work_task_stk[TASK_STACKSIZE];

/* ���������� ����� */
#define INITIALIZE_TASK_PRIORITY   6
#define PRINT_STATUS_TASK_PRIORITY 7
#define GETSEM_TASK1_PRIORITY      8
#define GETSEM_TASK2_PRIORITY      9
#define RECEIVE_TASK1_PRIORITY    10
#define RECEIVE_TASK2_PRIORITY    11
#define SEND_TASK_PRIORITY        12
#define AUTO_REPEAT_PRIORITY 14
#define USEFUL_WORK_TASK_PRIORITY 13


/* ���������� ������� */
#define   MSG_QUEUE_SIZE  30            //Size of message queue used in example
OS_EVENT  *msgqueue;                    //Message queue pointer 
void      *msgqueueTbl[MSG_QUEUE_SIZE]; // Storage for messages

/* Mbox declaration */
OS_EVENT *msgmbox;

/* ���������� �������� */
OS_EVENT *shared_resource_sem;
OS_EVENT *sem1;
OS_EVENT *sem2;
OS_EVENT *shared_7seg_PIO;

/* ���������� ���������� */
INT32U number_of_messages_sent = 0;
INT32U number_of_messages_received_task1 = 0;
INT32U number_of_messages_received_task2 = 0;
INT32U getsem_task1_got_sem = 0;
INT32U getsem_task2_got_sem = 0;
char sem_owner_task_name[20];

volatile int edge_capture = 0;
BOOLEAN status;
volatile INT8U number_of_runs = 0;

//timers
OS_TMR *PeriodicButtonCheck;
OS_TMR *Speed1;
OS_TMR *Speed2;
OS_TMR *Speed3;


/* ��������� ������� */
int initOSDataStructs(void);
int initCreateTasks(void);

/**********************************************************************
 *
 * Function:    sevenseg_set_hex()
 *
 * Description: Decode hex number to format for 7-seg display.
 *              Also: sends data to the 7-seg display PIO
 *
 * Notes:
 *
 * Returns:     The 7-seg-decoded values of a hex digit input.
 *
 **********************************************************************/
void isr_button(void* context, alt_u32 id)
{
INT32U value = 1;
INT8U err;
		//signal enter in interrupt handler
OSIntEnter();
//read pio
//value = IORD_ALTERA_AVALON_PIO_DATA(BUTTONS_BASE);

//send message to a waiting task
err = OSMboxPostOpt(msgmbox, (void *)value, OS_POST_OPT_NO_SCHED);
OSSemPost(sem1);

//reset pio register
IOWR_ALTERA_AVALON_PIO_EDGE_CAP(BUTTONS_BASE,0);

//inform OS about exit from interrupt
OSIntExit();
}
static void init_button_pio()
{
/* Recast the edge_capture pointer to match the
alt_irq_register() function prototype. */
void* edge_capture_ptr = (void*) &edge_capture;
/* Enable all 4 button interrupts. */
IOWR_ALTERA_AVALON_PIO_IRQ_MASK(BUTTONS_BASE, 0xf);
/* Reset the edge capture register. */
IOWR_ALTERA_AVALON_PIO_EDGE_CAP(BUTTONS_BASE, 0x0);
/* Register the ISR. */
#ifdef ALT_ENHANCED_INTERRUPT_API_PRESENT
alt_ic_isr_register(BUTTONS_IRQ_INTERRUPT_CONTROLLER_ID, BUTTONS_IRQ, isr_button, edge_capture_ptr, 0x0);
#else
alt_irq_register( BUTTON_PIO_IRQ, edge_capture_ptr, isr_button );
#endif
}

void SemPostWrapper(){
	OSSemPost(sem1);
}

void check_button_state(){
	INT32U button_state;
	INT8U err;
	button_state = IORD_ALTERA_AVALON_PIO_DATA(BUTTONS_BASE);
	err = OSMboxPostOpt(msgmbox, (void *)button_state, OS_POST_OPT_NONE);
}

void timer_message(){
	INT8U err;
	INT32U message = 0;
	err = OSMboxPostOpt(msgmbox, (void *)message, OS_POST_OPT_NONE);
}

void blink_green_led(){
	INT8U led_value;
	led_value = IORD_ALTERA_AVALON_PIO_DATA(GREEN_LED_BASE);

	if (led_value == 128) led_value = 1;
	else led_value = led_value << 1;

	IOWR_ALTERA_AVALON_PIO_DATA(GREEN_LED_BASE, led_value);

	if (number_of_runs < 255) number_of_runs++;
}

static void sevenseg_set_hex(INT32U hex, INT8U left)
{
  static alt_u8 segments[16] = {
    0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90, /* 0-9 */
    0x88, 0x83, 0xC6, 0xA1, 0x86, 0x8E };                       /* a-f */

  INT32U data = segments[hex & 15];
  if (left) data = data | (data << 16);
  IOWR_ALTERA_AVALON_PIO_DATA(SEVEN_SEG_BASE, data);
}


void print_status_task(void* pdata)
{
	INT32U ticks = 0;
	INT32U seconds;
	INT32U minutes;
	INT32U hours;
  while (1)
  { 
    OSTimeDlyHMSM(0, 0, 3, 0);
    ticks = OSTimeGet();
    seconds = ticks/OS_TICKS_PER_SEC;
    minutes = seconds/60;
    hours = minutes/60;
    ticks = ticks%OS_TICKS_PER_SEC;
    seconds = seconds%60;
    minutes = minutes%60;

    OS_Q_DATA que_data;

    printf("****************************************************************\n");
    printf("Hello From MicroC/OS-II Running on NIOS II.  Here is the status:\n");
    printf("\n");
    printf("Starting time of another cycle:\n%d:%d:%d:%d\n", hours, minutes, seconds, ticks);
    printf("The number of messages sent by the send_task:         %lu\n",
            number_of_messages_sent);
    printf("\n");
    printf("The number of messages received by the receive_task1: %lu\n",
            number_of_messages_received_task1);
    printf("\n");
    printf("The number of messages received by the receive_task2: %lu\n",
            number_of_messages_received_task2);
    printf("\n");
    printf("The shared resource is owned by: %s\n",
           &sem_owner_task_name[0]);
    printf("\n");
    printf("The Number of times getsem_task1 acquired the semaphore %lu\n",
            getsem_task1_got_sem);
    printf("\n");
    printf("The Number of times getsem_task2 acquired the semaphore %lu\n",
            getsem_task2_got_sem);
    printf("\n");
    OSQQuery(msgqueue, &que_data);
    printf("Number of messages: %d\n", que_data.OSNMsgs);
    printf("****************************************************************\n");
    printf("\n");
  }
}


void handle_auto_period(void *pdata){
	INT8U err;
	INT32U button_state;
	INT32U number_of_runs = 0;
	INT8U divisor = 0;
	while(1){
		OSSemPend(sem1, 0, &err);
		button_state = (INT32U)OSMboxAccept(msgmbox);
		if(button_state == 1){
			status = OSTmrStart(PeriodicButtonCheck, &err);
			button_state = 0;
			number_of_runs = 0;
			divisor = 0;
			OSSemPost(sem2); // single button press
		}
		else{
			button_state = IORD_ALTERA_AVALON_PIO_DATA(BUTTONS_BASE);
			if(button_state == 15){
				// stop condition
				button_state = 0;
				number_of_runs = 0;
				divisor = 0;
				OSTmrStop(PeriodicButtonCheck, OS_TMR_OPT_NONE, NULL, &err);
			}
			else{
				// run condition
				if(number_of_runs <= 200){
					divisor++;
					if(divisor == 20){
						divisor = 0;
						OSSemPost(sem2);
					}

				}
				else{
					if(number_of_runs <= 400){
						divisor++;
						if(divisor == 10){
							divisor = 0;
							OSSemPost(sem2);
						}

					}
				}
				if(number_of_runs > 400){
					divisor++;
					if(divisor == 5){
						divisor = 0;
						OSSemPost(sem2);
					}

				}

			}
		}
		if(number_of_runs <= 400){
					number_of_runs++;
				}
		}
}
void useful_work_task(void *pdata){
	INT8U err;
	while(1){
		OSSemPend(sem2, 0, &err);
		blink_green_led();
	}
}

//void receive_task1(void* pdata)
//{
//  INT8U return_code = OS_NO_ERR;
//  INT32U *msg;
//
//  while (1)
//  {
//    msg = (INT32U *)OSQPend(msgqueue, 0, &return_code);
//    alt_ucosii_check_return_code(return_code);
//    number_of_messages_received_task1++;
//    OSSemPend(shared_7seg_PIO, 0, &return_code);
//    sevenseg_set_hex(~*msg, 0);
//    OSSemPost(shared_7seg_PIO);
////    OSTimeDlyHMSM(0, 0, 0, 333);
//  }
//}

//void receive_task2(void* pdata)
//{
//  INT8U return_code = OS_NO_ERR;
//  INT32U *msg;
//
//  while (1)
//  {
//    msg = (INT32U *)OSQPend(msgqueue, 0, &return_code);
//    alt_ucosii_check_return_code(return_code);
//    number_of_messages_received_task2++;
//    OSSemPend(shared_7seg_PIO, 0, &return_code);
//	sevenseg_set_hex(~*msg, 1);
//	OSSemPost(shared_7seg_PIO);
////    OSTimeDlyHMSM(0, 0, 1, 0);
//  }
//}

void  initialize_task(void* pdata)
{
  INT8U return_code = OS_NO_ERR;



  initOSDataStructs();

  IOWR_ALTERA_AVALON_PIO_DATA(GREEN_LED_BASE, 1);

  init_button_pio();

  initCreateTasks();
  

  return_code = OSTaskDel(OS_PRIO_SELF);
  alt_ucosii_check_return_code(return_code);
  while (1);
}

int main (int argc, char* argv[], char* envp[])
{
  INT8U return_code = OS_NO_ERR;
  return_code = OSTaskCreate(initialize_task,
                             NULL,
                             (void *)&initialize_task_stk[TASK_STACKSIZE-1],
                             INITIALIZE_TASK_PRIORITY);
  alt_ucosii_check_return_code(return_code);
  OSStart();
  return 0;
}

int initOSDataStructs(void)
{
  msgqueue = OSQCreate(&msgqueueTbl[0], MSG_QUEUE_SIZE);
  msgmbox = OSMboxCreate((void *)0);
  shared_resource_sem = OSSemCreate(0);
  sem1 = OSSemCreate(0);
  sem2 = OSSemCreate(0);
  shared_7seg_PIO = OSSemCreate(1);
  INT8U err;

  PeriodicButtonCheck = OSTmrCreate(40, 1, OS_TMR_OPT_PERIODIC, SemPostWrapper, NULL, "100Hz", &err);
  if (err == OS_ERR_NONE){
	  printf("Timer for Button Check Created Successfully\n");
  }
  else printf("error code: %d\n", err);



  return 0;
}
 
int initCreateTasks(void)
{
  INT8U return_code = OS_NO_ERR;

//  return_code = OSTaskCreate(getsem_task1,
//                             NULL,
//                             (void *)&getsem_task1_stk[TASK_STACKSIZE-1],
//                             GETSEM_TASK1_PRIORITY);
//  return_code = OSTaskCreate(getsem_task2,
//                             NULL,
//                             (void *)&getsem_task2_stk[TASK_STACKSIZE-1],
//                             GETSEM_TASK2_PRIORITY);
//  return_code = OSTaskCreate(receive_task1,
//                             NULL,
//                             (void *)&receive_task1_stk[TASK_STACKSIZE-1],
//                             RECEIVE_TASK1_PRIORITY);
//  alt_ucosii_check_return_code(return_code);
//  return_code = OSTaskCreate(receive_task2,
//                             NULL,
//                             (void *)&receive_task2_stk[TASK_STACKSIZE-1],
//                             RECEIVE_TASK2_PRIORITY);
//  alt_ucosii_check_return_code(return_code);
//  return_code = OSTaskCreate(send_task,
//                             NULL,
//                             (void *)&send_task_stk[TASK_STACKSIZE-1],
//                             SEND_TASK_PRIORITY);
//  alt_ucosii_check_return_code(return_code);
  return_code = OSTaskCreate(print_status_task,
                             NULL,
                             (void *)&print_status_task_stk[TASK_STACKSIZE-1],
                             PRINT_STATUS_TASK_PRIORITY);
  alt_ucosii_check_return_code(return_code);
  return_code = OSTaskCreate(handle_auto_period,
                               NULL,
                               (void *)&handle_auto_period_task_stk[TASK_STACKSIZE-1],
                               AUTO_REPEAT_PRIORITY);
  alt_ucosii_check_return_code(return_code);
  return_code = OSTaskCreate(useful_work_task,
                               NULL,
                               (void *)&useful_work_task_stk[TASK_STACKSIZE-1],
							   USEFUL_WORK_TASK_PRIORITY);
  alt_ucosii_check_return_code(return_code);


  return 0;
}
