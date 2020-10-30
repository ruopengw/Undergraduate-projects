module phase_adder(a,b,s,ci,co);
	parameter N=1;
	input [N-1:0] a,b;
	input ci;
	output [N-1:0] s;
	output co;
	assign {co,s}=a+b+ci;
endmodule
	