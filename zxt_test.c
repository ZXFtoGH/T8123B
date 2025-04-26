&hall_1 {
	status = "okay";

	pinctrl-names = "pin_default", "pin_cfg";
	pinctrl-0 = <&hall_intpin_default>;
	pinctrl-1 = <&hall_intpin_cfg>;

	interrupt-parent = <&pio>;
	interrupts = <14 IRQ_TYPE_LEVEL_LOW 14 0>;
	gpio_det_level_hall_pin1 = <&pio 14 0>;
};

hall_intpin_default: pin_default {
    pins_cmd_dat {
        pinmux = <PINMUX_GPIO14__FUNC_GPIO14>;
        output-high;
    };
};
hall_intpin_cfg: pin_cfg {
    pins_cmd_dat {
        pinmux = <PINMUX_GPIO14__FUNC_GPIO14>;
        input-enable;
    };
};

<eint14>
<varName>HALL_1</varName>
<debounce_time>0</debounce_time>
<polarity>Low</polarity>
<sensitive_level>Level</sensitive_level>
<debounce_en>Disable</debounce_en>
</eint14>

<gpioXX>
    <eint_mode>true</eint_mode>             //使能外部中断
    <def_mode>0</def_mode>                   //默认功能，GPIO模式

    <inpull_en>false</inpull_en>            //禁用内部上拉
    <inpull_selhigh>false</inpull_selhigh>  //无上拉，此项无效

    <def_dir>IN</def_dir>                   //默认方向：输入
    <out_high>false</out_high>              //输出时默认低电平（但当前是输入）

    <smt>true</smt>                         //使能施密特触发器（抗噪声） 
    <ies>true</ies>                         //使能输入同步（防亚稳态）
</gpioXX>

<gpioxx>
    <eint_mode>   </eint_mode>       
    <def_mode>  </def_mode>            
    <inpull_en>   </inpull_en>      
    <inpull_selhigh>   </inpull_selhigh> 
    <def_dir>   </def_dir>             
    <out_high>   </out_high>        
    <smt>   </smt>                   
    <ies>   </ies>                   
</gpioxx>