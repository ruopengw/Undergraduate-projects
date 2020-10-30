`timescale 1ns / 1ps

module syn_circuit_tb_v;
	parameter delay=10;
	reg in,clk;
	wire out;
	
	syn_circuit uut(
		.sys_clk(clk),
		.in(in),
		.out(out)
	);
	
	initial 
	  begin
		// Initialize Inputs
		clk = 0;
		in = 0;
		#(delay*2) in=1;
		 
		repeat (20)
		begin 
        	    #(delay*5)  in = 1;
		    #(delay*5)  in = 0;
		end 
		
		#(delay*10) in=0; 
		#(delay*6) $stop;
		
	  end
	//clock
	always 		#(delay/2) clk=~clk;
      
endmodule