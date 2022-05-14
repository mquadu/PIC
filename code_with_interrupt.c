#include "C:\Users\cardi\Bureau\Code_3\code_with_interrupt.h"

char buffer[4];
int i = 0;
int16 diz, unit;
boolean flag = 0;
int16 maxDist = 100;
int16 dist, time  ;
int d, c,u;

#INT_TIMER1
void  TIMER1_isr(void) 
{
   set_timer1(0);


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
      
      while(!input(pin_c0)){} //attendre l'etat haut de la pin echo
      
      set_timer1(0);
      
      while(input(pin_c0)){} //attendre l'etat haut de la pin echo
      time = get_timer1();               
      dist = time*0.028;  
      printf("distance : %ld \n" , dist);
      printf("\n");
      delay_ms(100);
    
      if(dist<maxDist){
         output_high(PIN_E0);
         output_low(PIN_E1);
      }else{
         output_low(PIN_E0);
         output_toggle(PIN_E1);
      }
      
      if(dist<100){
         output_high(pin_e2);
         unit = dist%10;
         diz =((dist - unit)/10)%100;
         output_b((diz<<4)+unit);
      }else{
         output_low(PIN_E2);
         diz = dist/100;
         unit = (dist - (diz*100))/10;
         output_b((diz<<4)+unit);
         
        
      }
      
      if(flag == 1){
         flag =0;
         c = buffer[1]-48;
         d = buffer[2]-48;
         u = buffer[3]-48;
         maxDist = (int16)(c*100+d*10+u);
         
      }
      delay_ms(300);
     }

}
