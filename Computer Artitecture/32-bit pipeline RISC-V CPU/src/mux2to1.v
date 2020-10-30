module mux2to1(out,in0,in1,addr);
	parameter n=4;
	input [n-1:0] in0, in1;
	input addr;
	output reg [n-1:0] out;
	
	always @(*)
	begin
		case(addr)
			0: out = in0;
			1: out = in1;
		endcase
	end
	
endmodule