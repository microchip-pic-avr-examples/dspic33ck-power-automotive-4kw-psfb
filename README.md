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
