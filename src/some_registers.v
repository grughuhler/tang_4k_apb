/* Copyright 2024 Grug Huhler.  License SPDX BSD-2-Clause.
 *
 * This module implements 4 32-bit registers that can be read/written via
 * an APB2 interface. Their offsets are 0x0, 0x4, 0x8, and 0xc.  They may
 * be accessed from the core via word (32-bit), half-word, or byte sized
 * loads/stores.  The registers don't do anything besides being available
 * to read or write.
 */

module some_registers
  (
   input wire 	     pclk,
   input wire 	     preset_n,
   input wire 	     penable,
   input wire [7:0]  paddr,
   input wire 	     pwrite,
   input wire [31:0] pwdata,
   input wire [3:0]  pstrb,
   input wire [2:0]  pprot,
   input wire 	     psel,
   output reg [31:0] prdata,
   output reg 	     pready
);

   wire [1:0] 	     wordaddr;
   reg [31:0] 	     registers[3:2]; /* 4 registers, accessed by word address */

   assign wordaddr = paddr[3:2];

   always @(posedge pclk or negedge preset_n)
     if(!preset_n) begin
	pready <= 1'b0;
     end else begin
	if (psel & !penable) begin
	   /* setup state.  Inputs are stable.  Make outputs stable
	    * for next clock cycle, the access state */
           if (pwrite) begin
	      if (pstrb[0]) registers[wordaddr][7:0] <= pwdata[7:0];
	      if (pstrb[1]) registers[wordaddr][15:8] <= pwdata[15:8];
	      if (pstrb[2]) registers[wordaddr][23:16] <= pwdata[23:16];
	      if (pstrb[3]) registers[wordaddr][31:24] <= pwdata[31:24];
	   end else begin
	      prdata <= registers[wordaddr];
	   end
	   pready <= 1'b1;
	end else begin 
	   pready <= 1'b0;
	end
     end
   
endmodule
