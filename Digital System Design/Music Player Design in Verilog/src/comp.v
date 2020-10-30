module comp(a,b,agb,aeb,alb);
	parameter n=1;
	input [n-1:0] a, b;
	output agb; //a is greater than b
	output aeb; // a is equal to b
	output alb; // a is less than b
	assign agb=(a>b);
	assign aeb=(a==b);
	assign alb=(a<b);
endmodule
 