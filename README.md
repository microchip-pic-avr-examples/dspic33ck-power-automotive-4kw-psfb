<picture>
    <source media="(prefers-color-scheme: dark)" srcset="images/microchip_logo_white_red.png">
	<source media="(prefers-color-scheme: light)" srcset="images/microchip_logo_black_red.png">
    <img alt="Microchip Logo." src="images/microchip_logo_black_red.png">
</picture> 

## dspic33ck automotive lv dcdc


## Summary

## Related Documentation


## Software Used 


## Hardware Used


## Setup


## Operation



## feature notes 
### cmp3 - CT OCP
1. interrupt rising edge. higher priority. 
2. Vdac = Dacout * Avdd/4096 where Dacout is between 205 and 3890
3. with offset of 620, Dacout is limited between 620 and 3890. Corresponding to currents of 13 amps max.

### Input Voltage
1. gain at isolated side is 4.32mv
2. after voltage to duty cycle and 5% minimum, the scaler is simplified to  = ( ADC value read - 205 ) * 0.231
3. how to arrive at the number of 0.231 
4. Vsense = 1.728 from schematic. the Vref is 4.096 at primary side. results in 42.1875% duty. add 5% and we get 47.1875%
5. multiply this with full range of 4095 at secondary, i.e adc ref = 1932.3
6. minus 5% i.e 205, we get 1727.32. 
7. 400 divide by 1727.3281 we get 0.231

1. ov threshold is then ADC Val * 0.231 = 900, -> 3896.1 + 205 = 4101    -> higher than the max adc val -> max can be 895V
2. uv threshold is then ADC val = 1731.6 + 205  = 1936

### output voltage / V cap voltage
1. gain 154mv/v. 
2. Vcap voltage = (ADC Val * 3.3)/4095 * (1/0.154)
3. Vcap voltage = ADC VAL * 0.005232 
4. ADC VAL = Vcap * 191.131

1. VCAP OV Threshold 18V = 3440 adc
2. VCAP UV Threshold 6V = 1146 adc

### current sensors offset calculation

1. wait for some time in main before state machine starts for values to settle.
2. in the INIT state sample the i-pri and i-shunt lines for 64 times, calculate average. that is the offset

### PRECHARGE : charging vcap to 12 volts in Precharge state

1. after offsets are calculated, enable short circuit switch and pwm switching.
2. in precharge state check if vcap voltage is 12 volts, if not, then increase duty cycle. Dutycycle capped up to max duty cycle. this corresponds to 15V at 800 Vin. each increment is 1% at 100us rate.
3. if it is around 12 volts, then turn off pwm, and go to standby state.
4. if the state machine is at init state or precharge state, then the Under voltage fault at Vcap is not monitored.
5. max duty calculated  -> d% = (Vout  * 2 * 17 * 100) / Vin ---, vout = 11.5
6. d% =  39100 /  Vin
7. d% = 39100 / ( VinADC - 205 ) * 0.231 
8. d% = 39100 / ( VinADC - 205) / 4.329 -> approx divide by 4, 
9. d% = 39100 / ( VinADC - 205) >>2;
10. unimplemented still, division results in 0. TODO fix. temp fix capped at 63percent.



