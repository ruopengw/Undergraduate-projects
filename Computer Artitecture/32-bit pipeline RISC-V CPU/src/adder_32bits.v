module adder_32bits(a,b,ci,s,co);
	input [31:0] a,b;
	input ci;
	output [31:0] s;
	output co;
	
	wire co_3, co_7, co_11, co_15, co_19, co_23, co_27;
	
	adder_4bits adder_0_3(
	.a(a[3:0]),
	.b(b[3:0]),
	.ci(ci),
	.s(s[3:0]),
	.co(co_3));
	
	adder_4bits_condition adder_4_7(
	.a(a[7:4]),
	.b(b[7:4]),
	.ci(co_3),
	.s(s[7:4]),
	.co(co_7));
	
	adder_4bits_condition adder_8_11(
	.a(a[11:8]),
	.b(b[11:8]),
	.ci(co_7),
	.s(s[11:8]),
	.co(co_11));
	
	adder_4bits_condition adder_12_15(
	.a(a[15:12]),
	.b(b[15:12]),
	.ci(co_11),
	.s(s[15:12]),
	.co(co_15));
	
	adder_4bits_condition adder_16_19(
	.a(a[19:16]),
	.b(b[19:16]),
	.ci(co_15),
	.s(s[19:16]),
	.co(co_19));
	
	adder_4bits_condition adder_20_23(
	.a(a[23:20]),
	.b(b[23:20]),
	.ci(co_19),
	.s(s[23:20]),
	.co(co_23));
	
	adder_4bits_condition adder_24_27(
	.a(a[27:24]),
	.b(b[27:24]),
	.ci(co_23),
	.s(s[27:24]),
	.co(co_27));
	
	adder_4bits_condition adder_28_31(
	.a(a[31:28]),
	.b(b[31:28]),
	.ci(co_27),
	.s(s[31:28]),
	.co(co));

endmodule