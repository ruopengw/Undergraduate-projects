module adder_4bits_condition(a,b,ci,s,co);
	input wire [3:0] a,b;
	input wire ci;
	output wire [3:0] s;
	output wire co;
	wire [3:0] sum0, sum1;
	wire co0, co1;
	
	adder_4bits adder_4bits_0co(
	.a(a),
	.b(b),
	.ci(1'b0),
	.s(sum0),
	.co(co0));
	
	adder_4bits adder_4bits_1co(
	.a(a),
	.b(b),
	.ci(1'b1),
	.s(sum1),
	.co(co1));
	
	mux2to1 #(.n(4))mux(
	.out(s),
	.in0(sum0),
	.in1(sum1),
	.addr(ci));
	
	assign co=(ci&co1)|co0;

endmodule