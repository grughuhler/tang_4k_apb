/* Copyright 2024 Grug Huhler.  License SPDX BSD-2-Clause.
 *
 * This module implements a pwm controller with an APB2
 * interface.  Two 32-bit registers can be read/written using
 * 32-bit loads/stores:
 * 
 * offset register       purpose
 * 0x0    counter_limit  counter counts from 0 to counter_limit - 1
 * 0x4    on_limit       pwm_out is on whenever on_limit < counter_limit
 * 
 * counter increments at the frequency of pclk.
 */

module pwm_controller
  (
   input wire        pclk,
   input wire        preset_n,
   input wire        penable,
   input wire [7:0]  paddr,
   input wire        pwrite,
   input wire [31:0] pwdata,
   input wire [3:0]  pstrb,
   input wire [2:0]  pprot,
   input wire        psel,
   output reg [31:0] prdata,
   output reg        pready,
   output wire       pwm_out
);

   reg [31:0]        counter;
   reg [31:0]        counter_limit; /* offset 0x0 */
   reg [31:0]        on_limit;      /* offset 0x4 */

   assign pwm_out = counter < on_limit;

   always @(posedge pclk or negedge preset_n)
     if(!preset_n) begin
        pready <= 1'b0;
        counter <= 32'b0;
        counter_limit <= 32'b0;
        on_limit <= 32'b0;
     end else begin

        if (psel & !penable) begin
           /* setup state.  Inputs are stable.  Make outputs stable
            * for next clock cycle, the access state */
           if (pwrite) begin
              if (paddr == 8'h0)
                counter_limit <= pwdata;
              else
                on_limit <= pwdata;
           end else begin
              if (paddr == 8'h0)
                prdata <= counter_limit;
              else
                prdata <= on_limit;
           end
           pready <= 1'b1;
        end else begin 
           pready <= 1'b0;
        end

        /* PWM counter, pwm_out will be on whenever counter < on_limit */

        if (counter >= counter_limit)
          counter <= 32'b0;
        else
          counter <= counter + 32'b1;
     end
   
endmodule
