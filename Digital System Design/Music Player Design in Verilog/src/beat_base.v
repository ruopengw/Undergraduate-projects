module beat_base(ready,sys_clk,beat);
	parameter n=1000;
	parameter counter_bits=10;
	//仿真
	/*
	parameter n=64;
	parameter counter_bits=6;*/
	input sys_clk;
	input ready;
	output  beat;
	reg [counter_bits-1:0] q=0;
	assign beat=ready&&(q==(n-1));
	always@(posedge sys_clk)
	begin
		if(ready)
			begin 
				if(q==(n-1)) q=0; 
				else q=q+1; 
			end
	end
endmodule
