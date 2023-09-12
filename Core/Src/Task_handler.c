#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"


void process_command(command_t *cmd);
int extract_command(command_t *cmd);

const char *msg_inv = "//// INVALID OPTION ////\n";

void Menu_task(void* parameters)
{
uint32_t cmd_addr;
command_t *cmd;
int option;
   const char* msg_menu = "==================\n"
		   	   	   	   	   "|      MENU      |\n"
		                   "==================\n"
		                   "LED Effect 	---> 0\n"
		   	   	   	   	   "Date and Time 	---> 1\n"
           	   	   	   	   "Exit 	---> 2\n"
  	   	   	   	   	   	   "Enter your choice here: \n";
	while(1)
	{
		/*Print menu*/
		xQueueSend(Queue_print, &msg_menu, portMAX_DELAY);
		/*waits for menu commands*/
		xTaskNotifyWait(0,0,&cmd_addr, portMAX_DELAY);

		cmd = (command_t*)cmd_addr;

		if(cmd->len == 1)
		{
			option = cmd->payload[0] - 48; //converts from ASCII to decimal value
			switch(option)
			{
			case 0:
				curr_state = sLedEffect;
				xTaskNotify(LED_task_handle,0,eNoAction);
				break;
			case 1:
				curr_state = sRtcMenu;
				xTaskNotify(RTC_task_handle,0,eNoAction);
				break;
			case 2:
				/*IMPLEMENT EXIT*/
				break;
			default:
				xQueueSend(Queue_print, &msg_inv, portMAX_DELAY);
				continue;
			}
		}
		else
		{
			/*Print invalid message*/
			xQueueSend(Queue_print, &msg_inv, portMAX_DELAY);
		}
		/*waits to run again when some other task notifies*/
		xTaskNotifyWait(0,0,NULL, portMAX_DELAY);
	}
}
void LED_task(void* parameters)
{
	uint32_t cmd_addr;
	command_t *cmd;

	   const char* msg_led =   "======================\n"
			   	   	   	   	   "|      LED Effect     |\n"
			                   "======================\n"
			                   "none (e1,e2,e3,e4)\n"
	  	   	   	   	   	   	   "Enter your choice here: \n";
	while(1)
	{
		/*waits for notification, this notification will be sent by the Menu Task*/
		xTaskNotifyWait(0,0,NULL, portMAX_DELAY);
		/*Print menu*/
		xQueueSend(Queue_print, &msg_led, portMAX_DELAY);
		/*waits for LED command*/
		xTaskNotifyWait(0,0,&cmd_addr, portMAX_DELAY);
		cmd = (command_t*)cmd_addr;

		if(cmd->len <= 4)
		{
			if(! strcmp((char*)cmd->payload,"none"))
				led_effect_stop();
			else if (! strcmp((char*)cmd->payload,"e1"))
				led_effect(1);
			else if (! strcmp((char*)cmd->payload,"e2"))
				led_effect(2);
			else if (! strcmp((char*)cmd->payload,"e3"))
				led_effect(3);
			else if (! strcmp((char*)cmd->payload,"e4"))
				led_effect(4);
			else
				xQueueSend(Queue_print, &msg_inv, portMAX_DELAY);
		}
		else
		{
			xQueueSend(Queue_print, &msg_inv, portMAX_DELAY);
		}

		/*update state variable*/
		curr_state = sMainMenu;

		/*Notify menu task*/
		xTaskNotify(Menu_task_handle,0,eNoAction);

	}
}

void Print_task(void* parameters)
{
	uint32_t *msg;
	while(1)
	{
		xQueueReceive(Queue_print, &msg, portMAX_DELAY);
		HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen((char*)msg), portMAX_DELAY);
	}
}
void Command_handling_task(void* parameters)
{
	BaseType_t ret;
	command_t cmd;
	while(1)
	{
	/*implement notify wait*/
	ret = xTaskNotifyWait(0,0,NULL,portMAX_DELAY);
	/*If receives a notification from the UART ISR it will process the input from the user*/
	if(ret == pdTRUE)
	{
		process_command(&cmd);
	}
	}
}

void process_command(command_t *cmd)
{

	/*Extract the data items bytes from the input data queue*/
	extract_command(cmd);

	/*After extracting the command the processor will back to the task that was running before the input from the
	 * user was detected*/
	switch(curr_state)
	{
	case sMainMenu:
		/*notify menu task*/
		xTaskNotify(Menu_task_handle, (uint32_t)cmd, eSetValueWithOverwrite);
		break;
	case sLedEffect:
		xTaskNotify(LED_task_handle, (uint32_t)cmd, eSetValueWithOverwrite);
		break;
	case sRtcMenu:
	case sRtcTimeConfig:
	case sRtcDateConfig:
	case sRtcReport:
		xTaskNotify(RTC_task_handle, (uint32_t)cmd, eSetValueWithOverwrite);
		break;
	}

}

int extract_command(command_t *cmd)
{
	uint8_t item;
	BaseType_t status;

	/*In the next line the number of messages stored in the queue are stored in the status variable
	 * if there is no message stored this variable will be 0 and the function will return -1 due to
	 * there is no command to extract*/
	status = uxQueueMessagesWaiting(Queue_data);
	if(!status) return -1;


	uint8_t i = 0;

	do
	{
		/*In the next line each data item is received, the status variable will be true if the
		 * data item was received correctly*/
		status = xQueueReceive(Queue_data, &item, 0);
		/*If the reception of the item was successful then the payload[i] variable that is inside the cmd
		 * structure will be updated with the data item this payload can have a max of 10 data items */
		if(status == pdTRUE) cmd->payload[i++] = item;
		/*This process will be done until the char \n is detected that means the user made an enter*/
	}while(item != '\n');

	/*Here the last item of the payload array that should be /n will be replaced with \0 "null character" to follow
	 * a standard of UART */
	cmd->payload[i-1]='\0';
	/*the len variable of the cmd struct will store the number of items stored in the payload excluding the \0 character*/
	cmd -> len = i-1;

	return 0;
}


uint8_t getnumber(uint8_t *p , int len)
{
	int value;

	if(len > 1)
	{
		value = ( ((p[0]-48) * 10) + (p[1] - 48) );
	}
	else
	{
		value = p[0] - 48;
	}

	return value;
}

void RTC_task(void* parameters)
{
	const char* msg_rtc1 = "========================\n"
							"|         RTC          |\n"
							"========================\n";

	const char* msg_rtc2 = "Configure Time            ----> 0\n"
							"Configure Date            ----> 1\n"
							"Enable reporting          ----> 2\n"
							"Exit                      ----> 4\n"
							"Enter your choice here : ";


	const char *msg_rtc_hh = "Enter hour(1-12):";
	const char *msg_rtc_mm = "Enter minutes(0-59):";
	const char *msg_rtc_ss = "Enter seconds(0-59):";

	const char *msg_rtc_dd  = "Enter date(1-31):";
	const char *msg_rtc_mo  ="Enter month(1-12):";
	const char *msg_rtc_dow  = "Enter day(1-7 sun:1):";
	const char *msg_rtc_yr  = "Enter year(0-99):";

	const char *msg_conf = "Configuration successful\n";
	const char *msg_rtc_report = "Enable time&date reporting(y/n)?: ";


	uint32_t cmd_addr;
	command_t *cmd;
	static int rtc_state = 0;
	int menu_code;

	RTC_TimeTypeDef time;
	RTC_DateTypeDef date;

#define HH_CONFIG    0
#define MM_CONFIG 	 1
#define SS_CONFIG 	 2

#define DATE_CONFIG  0
#define MONTH_CONFIG 1
#define YEAR_CONFIG  2
#define DAY_CONFIG   3

	while(1){
		/*Notify wait (wait till someone notifies)*/
		xTaskNotifyWait(0,0,NULL,portMAX_DELAY);
		/*Print the menu and show current date and time information */
		xQueueSend(Queue_print, &msg_rtc1, portMAX_DELAY);
		show_time_date();
		xQueueSend(Queue_print, &msg_rtc2, portMAX_DELAY);
		while(curr_state != sMainMenu){

			/*Wait for command notification (Notify wait) */
			xTaskNotifyWait(0,0,&cmd_addr,portMAX_DELAY);
			cmd = (command_t*)cmd_addr;

			switch(curr_state)
			{
				case sRtcMenu:{

					/*process RTC menu commands */
					if(cmd->len == 1)
					{
						menu_code=cmd->payload[0]-48;
						switch(menu_code)
						{
						case 0:
							curr_state = sRtcTimeConfig;
							xQueueSend(Queue_print,&msg_rtc_hh,portMAX_DELAY);
							break;
						case 1:
							curr_state = sRtcDateConfig;
							xQueueSend(Queue_print,&msg_rtc_dd,portMAX_DELAY);
							break;
						case 2:
							curr_state = sRtcReport;
							xQueueSend(Queue_print,&msg_rtc_report,portMAX_DELAY);
							break;
						case 3:
							curr_state = sMainMenu;
							break;
						default:
							curr_state = sMainMenu;
							xQueueSend(Queue_print,&msg_inv,portMAX_DELAY);
						}
					}
					else
					{
						curr_state = sMainMenu;
						xQueueSend(Queue_print,&msg_inv,portMAX_DELAY);
					}
					break;}

				case sRtcTimeConfig:{
					/*get hh, mm, ss infor and configure RTC */
					/*take care of invalid entries */
					switch(rtc_state)
					{
						case HH_CONFIG:{
							uint8_t hour = getnumber(cmd->payload , cmd->len);
							time.Hours = hour;
							rtc_state = MM_CONFIG;
							xQueueSend(Queue_print,&msg_rtc_mm,portMAX_DELAY);
							break;}
						case MM_CONFIG:{
							uint8_t min = getnumber(cmd->payload , cmd->len);
							time.Minutes = min;
							rtc_state = SS_CONFIG;
							xQueueSend(Queue_print,&msg_rtc_ss,portMAX_DELAY);
							break;}
						case SS_CONFIG:{
							uint8_t sec = getnumber(cmd->payload , cmd->len);
							time.Seconds = sec;
							if(!validate_rtc_information(&time, NULL))
							{
								rtc_configure_time(&time);
								xQueueSend(Queue_print,&msg_conf,portMAX_DELAY);
								show_time_date();
							}
							else
							{
								xQueueSend(Queue_print,&msg_inv,portMAX_DELAY);
							}
							curr_state = sMainMenu;
							rtc_state = 0;
							break;}
					}

					break;}

				case sRtcDateConfig:{

					/*get date, month, day , year info and configure RTC */
					/*take care of invalid entries */
					switch(rtc_state)
					{
					case DATE_CONFIG:{
						uint8_t d = getnumber(cmd->payload , cmd->len);
						date.Date = d;
						rtc_state = MONTH_CONFIG;
						xQueueSend(Queue_print,&msg_rtc_mo,portMAX_DELAY);
						break;}
					case MONTH_CONFIG:{
						uint8_t month = getnumber(cmd->payload , cmd->len);
						date.Month = month;
						rtc_state = DAY_CONFIG;
						xQueueSend(Queue_print,&msg_rtc_dow,portMAX_DELAY);
						break;}
					case DAY_CONFIG:{
						uint8_t day = getnumber(cmd->payload , cmd->len);
						date.WeekDay = day;
						rtc_state = YEAR_CONFIG;
						xQueueSend(Queue_print,&msg_rtc_yr,portMAX_DELAY);
						break;}
					case YEAR_CONFIG:{
						uint8_t year = getnumber(cmd->payload , cmd->len);
						date.Year = year;

						if(!validate_rtc_information(NULL, &date))
						{
							rtc_configure_date(&date);
							xQueueSend(Queue_print,&msg_conf,portMAX_DELAY);
							show_time_date();
						}
						else
						{
							xQueueSend(Queue_print,&msg_inv,portMAX_DELAY);
						}

						curr_state = sMainMenu;
						rtc_state = 0;
						break;}

					}
					break;}

				case sRtcReport:{
					/*TODO: enable or disable RTC current time reporting over ITM printf */
					break;}

			}// switch end

		} //while end

		   /*TODO : Notify menu task */


		}//while super loop end
}







