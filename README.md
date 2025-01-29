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