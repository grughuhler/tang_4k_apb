/* Copyright 2024 Grug Huhler.  License SPDX BSD-2-Clause.
 *
 * Top level module for an APB2 example for the Tang Nano 4K FPGA
 * development board.
 * 
 * Completer some_registers0 is connected via APB2 requester 1 and
 * completer pwm_controller0 is connected via requester 2.  This implies
 * the following address APB2 map:
 * 
 *    0x40002400: some_registers0
 *    0x40002500: pwm_controller0
 */

 module top (
   input wire clk,
   input wire reset_button,
   input wire uart0_rxd,
   output wire uart0_txd,
   output wire led
);

   wire clk9;
   wire        pclk;
   wire        preset_n;
   wire        penable;
   wire [7:0]  paddr;
   wire        pwrite;
   wire [31:0] pwdata;
   wire [3:0]  pstrb;
   wire [2:0]  pprot;
   wire        psel1;
   wire [31:0] prdata1;
   wire        pready1;
   wire        psel2;
   wire [31:0] prdata2;
   wire        pready2;

   Gowin_PLLVR your_instance_name
     (
      .clkout(clk9),
      .clkin(clk)
      );

   Gowin_EMPU_Top empu0
     (
      .sys_clk(clk9),
      .uart0_rxd(uart0_rxd),
      .uart0_txd(uart0_txd),
      .master_pclk(pclk),
      .master_prst(preset_n),
      .master_penable(penable),
      .master_paddr(paddr),
      .master_pwrite(pwrite),
      .master_pwdata(pwdata),
      .master_pstrb(pstrb),
      .master_pprot(pprot),
      .master_psel1(psel1),
      .master_prdata1(prdata1),
      .master_pready1(pready1),
      .master_pslverr1(1'b0),
      .master_psel2(psel2),
      .master_prdata2(prdata2),
      .master_pready2(pready2),
      .master_pslverr2(1'b0),
      .reset_n(reset_button)
      );

   some_registers some_registers0
     (
      .pclk(pclk),
      .preset_n(preset_n),
      .penable(penable),
      .paddr(paddr),
      .pwrite(pwrite),
      .pwdata(pwdata),
      .pstrb(pstrb),
      .pprot(pprot),
      .psel(psel1),
      .prdata(prdata1),
      .pready(pready1)
      );

   pwm_controller pwm_controller0
     (
      .pclk(pclk),
      .preset_n(preset_n),
      .penable(penable),
      .paddr(paddr),
      .pwrite(pwrite),
      .pwdata(pwdata),
      .pstrb(pstrb),
      .pprot(pprot),
      .psel(psel2),
      .prdata(prdata2),
      .pready(pready2),
      .pwm_out(led)
      );

endmodule
