module syn_circuit(sys_clk,in,out);
	input sys_clk;
	input in;
	output out;
	wire q1,q2;
	
	dffre #(.n(1)) dff1(
	.d(in),.en(1),.r(0),.clk(sys_clk),.q(q1));
	dffre #(.n(1)) dff2(
	.d(q1),.en(1),.r(0),.clk(sys_clk),.q(q2));
	assign
		out=q1&&(~q2);
endmodule