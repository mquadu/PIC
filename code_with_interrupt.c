#include "C:\Users\cardi\Bureau\Code_3\code_with_interrupt.h"
//#include <lcd.c>
char buffer[4];
int i = 0;
int16 diz, unit;
boolean flag = 0;
int16 limitDist = 100;
int16 dist ;

int d, c,u;
#INT_TIMER1
//int stop_timer = 0;
//int timer_overflow = 0;
void  TIMER1_isr(void) 
{
   set_timer1(0);
   //if (stop_timer){
     // stop_timer = 0;
      
   //}else{
     // set_timer1(0);
    //  timer_overflow ++;
   //}

}
#int_RDA
void RDA_isr(void) 
{
   buffer[i] = getc();
    if(buffer[0] == ':' && flag == 0 ){
        i++;
        if(i>=4){
            i = 0;
            flag = 1;
        }
    }
 
}


void main()
{

   setup_adc_ports(NO_ANALOGS);
   setup_adc(ADC_OFF);
   setup_psp(PSP_DISABLED);
   setup_spi(FALSE);
   setup_wdt(WDT_OFF);
   setup_timer_0(RTCC_INTERNAL);
   setup_timer_1(T1_INTERNAL|T1_DIV_BY_8);
   setup_timer_2(T2_DISABLED,0,1);
   setup_comparator(NC_NC_NC_NC);
   setup_vref(FALSE);
   enable_interrupts(INT_RDA);
   enable_interrupts(GLOBAL);
   setup_oscillator(False);
   
  
     // TODO: USER CODE!!
     while(true){
      
      //read value of sensor
      output_high(pin_c1);
      delay_us(20);
      output_low(pin_c1);
      
      //while(!input(pin_c0)){} //attendre l'etat haut de la pin echo
      
      set_timer1(0);
      
      //while(input(pin_c0)){} //attendre l'etat haut de la pin echo    
                         
      dist = get_timer1()*0.028;  
      printf("distance : %ld \n" , dist);
      printf("\n");
      delay_ms(100);
    
      if(dist<limitDist){
         output_high(PIN_E0);
         output_low(PIN_E1);
         printf("A1_OFF\n");
      }else{
         output_low(PIN_E0);
         output_toggle(PIN_E1);
         printf("A1_ON\n");   //tel python application not alarm
      }
      
      //affichage
      if(dist<100){
         output_low(PIN_E2);
         diz = dist/10;
         unit = dist - (diz*10);
         //output_b(diz+unit);
         output_b((diz<<4)+unit);
      }else{
         output_high(PIN_E2);
         diz = dist/100;
         unit = (dist - (diz*100))/10;
         
         printf(" Unité : %ld\n ", unit);
         output_b((diz<<4)+unit);
         
        
      }
      
      //A envoyer vers le lcd
      if(flag == 1){
         flag =0;
         c = buffer[1]-48;
         d = buffer[2]-48;
         u = buffer[3]-48;
         limitDist = (int16)(c*100+d*10+u);
      }
      c =  limitDist/100;
      d = (limitDist-(c*100))/10;
      u = (limitDist-(c*100))-(d*10);
      delay_ms(300);
     }

}
