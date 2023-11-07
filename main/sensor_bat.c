#include <stdio.h>
#include <stdlib.h>
#include "driver/adc.h"
#include "esp_adc_cal.h"
#include "config.h"

static esp_adc_cal_characteristics_t adc1_chars;
static float adc_value;

void Set_Voltage_ADC(void)
{
    esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_DEFAULT, 0, &adc1_chars);
    adc1_config_width(ADC_WIDTH_BIT_DEFAULT);
    adc1_config_channel_atten(ADC1_CHANNEL_4, ADC_ATTEN_DB_11);
}

float get_voltage(void)
{
    adc_value = adc1_get_raw(voltage_pin); //ADC1_CHANNEL_4
    printf("ADC Value: %f", adc_value);//un valor entre 0 (0V) y 4095 (3,3V)
    /*como vamos a dividir la tension de entrada, podemos tomar la mitad, entonces 
    el rango de la bateria será de 0V a 2,2V, osea de 0 a 2730*/
    //valores interesantes:
    //0V 0
    //2,2V 2730
    //1,85V 2290
    //3,3------4095
    //1,85----->2295     la cte es 1240
    adc_value=adc_value * 1240;
    printf("ADC Voltage Value: %f", adc_value);

    return adc_value;
}
