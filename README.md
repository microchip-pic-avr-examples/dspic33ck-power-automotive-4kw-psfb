<picture>
    <source media="(prefers-color-scheme: dark)" srcset="images/microchip_logo_white_red.png">
	<source media="(prefers-color-scheme: light)" srcset="images/microchip_logo_black_red.png">
    <img alt="Microchip Logo." src="images/microchip_logo_black_red.png">
</picture> 

## dspic33ck automotive lv dcdc


## Summary

Steps to operate

1. when the controller turns on, you will see adc readings on the ADC, and the state on pcs standby.
2. click on Start Charge. This will turn on the short circuit switch charging the output caps. state PRECHARGE
3. when the output caps are charged, that is the voltage difference between FP_VCAP and FB_VOUT pins is less then 80mv then the state changes to STANDBY
4. click on start power transfer, this will start switching. with 0° phase shift. State UP AND RUNNING
5. use the slider to change the phase shift.
6. click on start sync and stop sync to start stop sync switches.
7. click on stop vloop and stop vloop to start stop vloop.
8. when vloop jumps in the peak phase shift clamps to maximum/minimums defined in code. 
9. vloop slider refernce is used to set a output voltage rference.



## Related Documentation


## Software Used 


## Hardware Used


## Setup


## Operation



