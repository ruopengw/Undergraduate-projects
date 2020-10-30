`timescale 1ns / 1ps
module counter_n_tb;
	//inputs
	reg clk,r,en;
	//outputs
	wire [9:0] q;
	wire co;
	//example n=10
	counter_n #(.n(1000),.counter_bits(10))counter_3(
		.clk(clk),.r(r),.en(en),.q(q),.co(co));
	always #5 clk=~clk;
	initial
		begin
			clk=0;r=0;en=0;
			#(5)r=1;
			#(10)r=0;en=1;
			#(10000)repeat(100)begin  #(10*10)en=1;  # 10 en=0;  end
			#100 $stop;
		end
endmodule