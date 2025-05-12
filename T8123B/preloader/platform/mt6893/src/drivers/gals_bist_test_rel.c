#define DRV_WriteReg32(addr, data)   ((*(volatile unsigned int *)(addr)) = (unsigned int)(data))
#define DRV_Reg32(addr)              (*(volatile unsigned int *)(addr))


//input: none; output: 1->pass; 0->fail
int gals_bist_test()
{
  //MD PLL INIT: set MD related frequency to test frequency
  {
    unsigned int REGBASE_PLLMIXED = 0x20140000;
    unsigned int REGBASE_CLKSW = 0x20150000;

    // default md_srclken_ack settle time = 150T 32k
    DRV_WriteReg32(REGBASE_PLLMIXED+0x4, 0x02021c96);
    // change APBPLL_SETTLE_26M to 0x2f2 => 29us
    DRV_WriteReg32(REGBASE_PLLMIXED+0x20, 0x17920803);
    DRV_WriteReg32(REGBASE_PLLMIXED+0x40, 0x801FAC4D); // Fvco = 2700Mhz. 2700/3 = 900Mhz
    DRV_WriteReg32(REGBASE_PLLMIXED+0x10, 0x0);

    /* TODO: wait 50 ms */
    mdelay(50);

    //switch clock source to PLL
    DRV_WriteReg32(REGBASE_CLKSW+0x24, DRV_Reg32(REGBASE_CLKSW+0x24) | 0x3); // switch MDMCU & MDBUS clock to PLL freq
    DRV_WriteReg32(REGBASE_CLKSW+0x24, DRV_Reg32(REGBASE_CLKSW+0x24) | 0x8000003); // switch specific clock
    DRV_WriteReg32(REGBASE_CLKSW+0x28, DRV_Reg32(REGBASE_CLKSW+0x28) | 0x4);
    DRV_WriteReg32(REGBASE_PLLMIXED+0x314, 0xffff); // Clear PLL ADJ RDY IRQ fired by initial period adjustment
    DRV_WriteReg32(REGBASE_PLLMIXED+0x318, 0xffff); // Mask all PLL ADJ RDY IRQ
  }

  //DO GALS BIST TEST
  {
    DRV_WriteReg32(0x20360144, 0xffff9ffc);
    DRV_WriteReg32(0x20360140, 0x00000001);

    /* TODO: wait 100 ms */
    mdelay(100);

    unsigned int res1 = DRV_Reg32(0x20360148);
    unsigned int res2 = DRV_Reg32(0x2036014C);

    if(res1==0xffffffff && res2==0x0)
    {
      return 1;
    }
    else
    {
      return 0;
    }
  }
}
